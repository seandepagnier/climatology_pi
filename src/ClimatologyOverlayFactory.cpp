/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Climatology Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 *
 */

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/progdlg.h>

#include "climatology_pi.h"
#include "gldefs.h"

static int s_multitexturing = 0;
static PFNGLACTIVETEXTUREARBPROC s_glActiveTextureARB = 0;
static PFNGLMULTITEXCOORD2DARBPROC s_glMultiTexCoord2dARB = 0;

static GLboolean QueryExtension( const char *extName )
{
    /*
     ** Search for extName in the extensions string. Use of strstr()
     ** is not sufficient because extension names can be prefixes of
     ** other extension names. Could use strtok() but the constant
     ** string returned by glGetString might be in read-only memory.
     */
    char *p;
    char *end;
    int extNameLen;

    extNameLen = strlen( extName );

    p = (char *) glGetString( GL_EXTENSIONS );
    if( NULL == p ) {
        return GL_FALSE;
    }

    end = p + strlen( p );

    while( p < end ) {
        int n = strcspn( p, " " );
        if( ( extNameLen == n ) && ( strncmp( extName, p, n ) == 0 ) ) {
            return GL_TRUE;
        }
        p += ( n + 1 );
    }
    return GL_FALSE;
}

#if defined(__WXMSW__)
#define systemGetProcAddress(ADDR) wglGetProcAddress(ADDR)
#elif defined(__WXOSX__)
#include <dlfcn.h>
#define systemGetProcAddress(ADDR) dlsym( RTLD_DEFAULT, ADDR)
#else
#define systemGetProcAddress(ADDR) glXGetProcAddress((const GLubyte*)ADDR)
#endif

double deg2rad(double degrees)
{
  return M_PI * degrees / 180.0;
}

ClimatologyOverlay::~ClimatologyOverlay()
{
    if(m_iTexture)
        glDeleteTextures( 1, &m_iTexture );
    delete m_pDCBitmap, delete[] m_pRGBA;
}

static const wxString climatology_pi = _T("climatology_pi: ");

double ClimatologyIsoBarMap::CalcParameter(double lat, double lon)
{
//    return m_factory.getCurCalibratedValue(MAG, m_setting, lat, lon);
    return m_factory.getCalibratedValueMonth(MAG, m_setting, lat, lon, m_month);
}

#define CYCLONE_CACHE_SEMAPHORE_COUNT 4
ClimatologyOverlayFactory::ClimatologyOverlayFactory( ClimatologyDialog &dlg )
    : //m_bUpdateCyclones(true),
    m_cyclone_cache_semaphore(CYCLONE_CACHE_SEMAPHORE_COUNT),
      m_dlg(dlg), m_Settings(dlg.m_cfgdlg->m_Settings),
      m_cyclonesDisplayList(0), m_cyclone_drawn_counter(0),
      m_bFailedLoading(false)
{
    // assume we have GL_ARB_multitexture if this passes
    if(QueryExtension( "GL_ARB_texture_env_combine" )) {
        s_glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)
            systemGetProcAddress("glActiveTextureARB");
        s_glMultiTexCoord2dARB = (PFNGLMULTITEXCOORD2DARBPROC)
            systemGetProcAddress("glMultiTexCoord2dARB");
        s_multitexturing = s_glActiveTextureARB && s_glMultiTexCoord2dARB;

        if(s_multitexturing) {
            GLint MaxTextureUnits;
            glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &MaxTextureUnits);
            if(MaxTextureUnits > 2)
                s_multitexturing = 2; /* with blending */
        }
    }
    
    for(int month = 0; month<13; month++) {
        m_WindData[month] = NULL;
        m_CurrentData[month] = NULL;
    }

    m_CurrentTimeline = wxDateTime::Now();
    m_CurrentTimeline.SetYear(1999); /* without leap year */

    wxProgressDialog progressdialog( _("Climatology"), wxString(), 38, &m_dlg,
                                     wxPD_CAN_ABORT | wxPD_ELAPSED_TIME);

    wxString path = ClimatologyDataDirectory();
    wxString fmt = _T("%02d");

    /* load wind */
    for(int month = 0; month < 12; month++) {
        wxString filename = wxString::Format(_T("wind")+fmt, month+1);
        if(!progressdialog.Update(month, filename))
            return;
        ReadWindData(month, path+filename);
    }

    if(!progressdialog.Update(12, _("averaging wind")))
        return;
    AverageWindData();

    /* load current */
    for(int month = 0; month < 12; month++) {
        wxString filename = wxString::Format(_T("current")+fmt, month+1);
        if(!progressdialog.Update(month+13, filename))
            return;
        ReadCurrentData(month, path+filename);
    }

    if(!progressdialog.Update(25, _("averaging current")))
        return;
    AverageCurrentData();

    ZUFILE *f;
    /* load sea level pressure and sea surface temperature */
    if(!progressdialog.Update(26, _("sea level presure")))
        return;
    wxString slp_path = path + _T("sealevelpressure");
    if((f = TryOpenFile(slp_path))) {
        wxInt16 slp[12][90][180];
        if(zu_read(f, slp, sizeof slp) != sizeof slp)
            wxLogMessage(climatology_pi + _("slp file truncated"));
        else {
            for(int j=0; j<90; j++)
                for(int k=0; k<180; k++) {
                    long total = 0, totalcount = 0;
                    for(int i=0; i<12; i++) {
                        m_slp[i][j][k] = slp[i][j][k];
                        if(slp[i][j][k] != 32767) {
                            total += slp[i][j][k];
                            totalcount++;
                        }
                        if(totalcount)
                            m_slp[12][j][k] = total / totalcount;
                        else
                            m_slp[12][j][k] = 32767;
                    }
                }
            m_dlg.m_cbPressure->Enable();
        }
    }
    zu_close(f), free(f);

    if(!progressdialog.Update(27, _("sea surface tempertature")))
        return;
    wxString sst_path = path + _T("seasurfacetemperature");
    if((f = TryOpenFile(sst_path))) {
        wxInt8 sst[12][180][360];
        if(zu_read(f, sst, sizeof sst) != sizeof sst)
            wxLogMessage(climatology_pi + _("sst file truncated"));
        else {
            for(int j=0; j<180; j++)
                for(int k=0; k<360; k++) {
                    long total = 0, totalcount = 0;
                    for(int i=0; i<12; i++) {
                        if(sst[i][j][k] == -128)
                            m_sst[i][j][k] = 32767;
                        else {
                            m_sst[i][j][k] = sst[i][j][k]*200;
                            total += m_sst[i][j][k];
                            totalcount++;
                        }
                        if(totalcount)
                            m_sst[12][j][k] = total / totalcount;
                        else
                            m_sst[12][j][k] = 32767;
                    }
                }
            m_dlg.m_cbSeaTemperature->Enable();
        }
    }
    zu_close(f), free(f);

    if(!progressdialog.Update(28, _("air tempertature")))
        return;
    wxString at_path = path + _T("airtemperature");
    if((f = TryOpenFile(at_path))) {
        wxInt8 at[12][90][180];
        if(zu_read(f, at, sizeof at) != sizeof at)
            wxLogMessage(climatology_pi + _("at file truncated"));
        else {
            for(int j=0; j<90; j++)
                for(int k=0; k<180; k++) {
                    long total = 0, totalcount = 0;
                    for(int i=0; i<12; i++) {
                        if(at[i][j][k] == -128)
                            m_at[i][j][k] = 32767;
                        else {
                            m_at[i][j][k] = at[i][j][k];
                            total += m_at[i][j][k];
                            totalcount++;
                        }
                        if(totalcount)
                            m_at[12][j][k] = total / totalcount;
                        else
                            m_at[12][j][k] = 32767;
                    }
                }
            m_dlg.m_cbAirTemperature->Enable();
        }
    }
    zu_close(f), free(f);

    if(!progressdialog.Update(28, _("cloud cover")))
        return;
    wxString cld_path = path + _T("cloud");
    if((f = TryOpenFile(cld_path))) {
        wxUint8 cld[12][90][180];
        if(zu_read(f, cld, sizeof cld) != sizeof cld)
            wxLogMessage(climatology_pi + _("cld file truncated"));
        else {
            for(int j=0; j<90; j++)
                for(int k=0; k<180; k++) {
                    long total = 0, totalcount = 0;
                    for(int i=0; i<12; i++) {
                        if(cld[i][j][k] == 255)
                            m_cld[i][j][k] = 32767;
                        else {
                            m_cld[i][j][k] = cld[i][j][k]*40;
                            total += m_cld[i][j][k];
                            totalcount++;
                        }
                        if(totalcount)
                            m_cld[12][j][k] = total / totalcount;
                        else
                            m_cld[12][j][k] = 32767;
                    }
                }
            m_dlg.m_cbCloudCover->Enable();
        }
    }
    zu_close(f), free(f);

    if(!progressdialog.Update(29, _("precipitation")))
        return;
    wxString precip_path = path + _T("precipitation");
    if((f = TryOpenFile(precip_path))) {
        wxUint8 precip[12][72][144];
        if(zu_read(f, precip, sizeof precip) != sizeof precip)
            wxLogMessage(climatology_pi + _("precip file truncated"));
        else {
            for(int j=0; j<72; j++)
                for(int k=0; k<144; k++) {
                    long total = 0, totalcount = 0;
                    for(int i=0; i<12; i++) {
                        if(precip[i][j][k] == 255)
                            m_precip[i][j][k] = 32767;
                        else {
                            m_precip[i][j][k] = precip[i][j][k]*100;
                            total += m_precip[i][j][k];
                            totalcount++;
                        }
                        if(totalcount)
                            m_precip[12][j][k] = total / totalcount;
                        else
                            m_precip[12][j][k] = 32767;
                    }
                }
            m_dlg.m_cbPrecipitation->Enable();
        }
    }
    zu_close(f), free(f);

    if(!progressdialog.Update(30, _("relative humidity")))
        return;
    wxString rhum_path = path + _T("relativehumidity");
    if((f = TryOpenFile(rhum_path))) {
        wxUint8 rhum[12][180][360];
        if(zu_read(f, rhum, sizeof rhum) != sizeof rhum)
            wxLogMessage(climatology_pi + _("relative humidity file truncated"));
        else {
            for(int j=0; j<180; j++)
                for(int k=0; k<360; k++) {
                    long total = 0, totalcount = 0;
                    for(int i=0; i<12; i++) {
                        if(rhum[i][j][k] == 255)
                            m_rhum[i][j][k] = 32767;
                        else {
                            m_rhum[i][j][k] = rhum[i][j][k];
                            total += m_rhum[i][j][k];
                            totalcount++;
                        }
                        if(totalcount)
                            m_rhum[12][j][k] = total / totalcount;
                        else
                            m_rhum[12][j][k] = 32767;
                    }
                }
            m_dlg.m_cbRelativeHumidity->Enable();
        }
    }
    zu_close(f), free(f);

    /* load lightning */
    if(!progressdialog.Update(30, _("lightning")))
        return;
    wxString lightn_path = path + _T("lightning");
    if((f = TryOpenFile(lightn_path))) {
        wxUint8 lightn[12][180][360];
        if(zu_read(f, lightn, sizeof lightn) != sizeof lightn)
            wxLogMessage(climatology_pi + _("lightning file truncated"));
        else {
            for(int j=0; j<180; j++)
                for(int k=0; k<360; k++) {
                    long total = 0, totalcount = 0;
                    for(int i=0; i<12; i++) {
                        m_lightn[i][j][k] = lightn[i][j][k];
                        total += m_lightn[i][j][k];
                        totalcount++;
                    }
                    m_lightn[12][j][k] = total / totalcount;
                }
            m_dlg.m_cbLightning->Enable();
        }
    }
    zu_close(f), free(f);

    /* load sea depth */
    if(!progressdialog.Update(30, _("sea depth")))
        return;
    wxString seadepth_path = path + _T("seadepth");
    if((f = TryOpenFile(seadepth_path))) {
        wxInt8 seadepth[180][360];
        if(zu_read(f, seadepth, sizeof seadepth) != sizeof seadepth)
            wxLogMessage(climatology_pi + _("seadepth file truncated"));
        else {
            for(int j=0; j<180; j++)
                for(int k=0; k<360; k++)
                    if(seadepth[j][k] == -128)
                        m_seadepth[j][k] = 32767;
                    else
                        m_seadepth[j][k] = seadepth[j][k];

            m_dlg.m_cbSeaDepth->Enable();
        }
    }
    zu_close(f), free(f);

    /* load cyclone tracks */
    bool allcyclone = true;
    if(!progressdialog.Update(30, _("cyclone (east pacific)")))
        return;
    if(!ReadCycloneData(path + _T("cyclone-epa"), m_epa))
        allcyclone = false;

    if(!progressdialog.Update(31, _("cyclone (west pacific)")))
        return;
    if(!ReadCycloneData(path + _T("cyclone-wpa"), m_wpa))
        allcyclone = false;

    if(!progressdialog.Update(32, _("cyclone (south pacific)")))
        return;
    if(!ReadCycloneData(path + _T("cyclone-spa"), m_spa, true))
        allcyclone = false;

    if(!progressdialog.Update(33, _("cyclone (atlantic)")))
        return;
    if(!ReadCycloneData(path + _T("cyclone-atl"), m_atl))
        allcyclone = false;

    if(!progressdialog.Update(34, _("cyclone (north indian)")))
        return;
    if(!ReadCycloneData(path + _T("cyclone-nio"), m_nio))
        allcyclone = false;

    if(!progressdialog.Update(35, _("cyclone (south indian)")))
        return;
    if(!ReadCycloneData(path + _T("cyclone-she"), m_she, true))
        allcyclone = false;

    if(allcyclone)
        m_dlg.m_cbCyclones->Enable();

    if(!progressdialog.Update(36, _("el nino years")))
        return;

    if(!ReadElNinoYears(path + _T("elnino_years.txt"))) {
        m_dlg.m_cfgdlg->m_cbElNino->Disable();
        m_dlg.m_cfgdlg->m_cbLaNina->Disable();
        m_dlg.m_cfgdlg->m_cbNeutral->Disable();
    }

    if(!progressdialog.Update(37, _("cyclone cache")))
        return;

    BuildCycloneCache();

    if(m_bFailedLoading) {
        wxMessageDialog mdlg(&m_dlg, 
                             _("Some Data Failed to load:\n")
                             + m_sFailedMessage +
                             _("Would you like to try to download?"),
                             _("Climatology"), wxYES | wxNO | wxICON_WARNING);
        if(mdlg.ShowModal() == wxID_YES) {
            wxLaunchDefaultBrowser(
                _T("http://sourceforge.net/projects/opencpnplugins/files/climatology_pi/"));
            wxMessageDialog mdlg(&m_dlg, _("You must extract this data, and place in: ") + path +
                                 _("\nthen restart opencpn"),
                                 _("Climatology"), wxOK);
            mdlg.ShowModal();
        }
    }
}

ClimatologyOverlayFactory::~ClimatologyOverlayFactory()
{
//    glDeleteLists(m_cyclonelist, 1);
#if 0
    for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++)
        for(int m=0; m<13; m++)
            delete m_Settings.Settings[i].m_pIsobars[m];
#endif
}

void ClimatologyOverlayFactory::GetDateInterpolation(const wxDateTime *cdate,
                                                     int &month, int &nmonth, double &dpos)
{
    if(!cdate) {
        if(m_bAllTimes) {
            month = nmonth = 12;
            dpos = 1;
            return;
        }
        cdate = &m_CurrentTimeline;
    }
    
    month = cdate->GetMonth();
    int day = cdate->GetDay();
    int daysinmonth = wxDateTime::GetNumberOfDays(cdate->GetMonth());
    dpos = (day-.5) / daysinmonth;
    
    if(dpos > .5) {
        nmonth = month + 1;
        if(nmonth == 12)
            nmonth = 0;
        dpos = 1.5 - dpos;
    } else {
        nmonth = month - 1;
        if(nmonth == -1)
            nmonth = 11;
        dpos = .5 + dpos;
    }
}

bool ClimatologyOverlayFactory::InterpolateWindAtlasTime(int month, int nmonth, double dpos,
                                                         double lat, double lon,
                                                         double *directions, double *speeds,
                                                         double &storm, double &calm)
{
    WindData::WindPolar *polar1 = m_WindData[month]->GetPolar(lat, positive_degrees(lon));
    WindData::WindPolar *polar2 = m_WindData[nmonth]->GetPolar(lat, positive_degrees(lon));
    if(!polar1 || !polar2)
        return false;

    storm = (dpos*polar1->storm + (1-dpos)*polar2->storm) / 100.0;
    calm = (dpos*polar1->calm + (1-dpos)*polar2->calm) / 100.0;

    int dir_cnt = m_WindData[month]->dir_cnt;

    double totald1 = 0, totald2 = 0;
    for(int i=0; i<dir_cnt; i++) {
        totald1 += polar1->directions[i];
        totald2 += polar2->directions[i];
    }

    for(int i=0; i<dir_cnt; i++) {
        double direction1 = polar1->directions[i]/totald1;
        double direction2 = polar2->directions[i]/totald2;
        directions[i] = dpos*direction1 + (1-dpos)*direction2;

        double speed1 = (double)polar1->speeds[i] / polar1->directions[i];
        double speed2 = (double)polar2->speeds[i] / polar2->directions[i];

        if(direction1) {
            if(direction2)
                speeds[i] = dpos*speed1 + (1-dpos)*speed2;
            else
                speeds[i] = speed1;
        } else if(direction2)
            speeds[i] = speed2;
        else
            speeds[i] = 0;
    }

    return true;
}

static double interpquad(double v1, double v2, double v3, double v4, double d1, double d2)
{
    double w1 = d1*v3 + (1-d1)*v1;
    double w2 = d1*v4 + (1-d1)*v2;
    return      d2*w2 + (1-d2)*w1;
}

bool ClimatologyOverlayFactory::InterpolateWindAtlas(wxDateTime &date,
                                                     double lat, double lon,
                                                     double *directions, double *speeds,
                                                     double &storm, double &calm)
{
    int month, nmonth;
    double dpos;
    GetDateInterpolation(&date, month, nmonth, dpos);

    double idirections[4][8], ispeeds[4][8], istorm[4], icalm[4];

    double lats[2] = {floor(lat), ceil(lat)}, lons[2] = {floor(lon), ceil(lon)};
    double latd = lat - lats[0], lond = lon - lons[0];
    bool havedata[4];

    for(int lati = 0; lati<2; lati++)
        for(int loni = 0; loni<2; loni++) {
            int i = 2*lati + loni;
            havedata[i] = InterpolateWindAtlasTime(month, nmonth, dpos, lats[lati], lons[loni],
                                                   idirections[i], ispeeds[i], istorm[i], icalm[i]);
        }

    /* fill in missing data */
    int searchdata[4][3] = {{1, 2, 3}, {0, 3, 2}, {3, 0, 1}, {2, 1, 0}};
    for(int i = 0; i<4; i++)
        if(!havedata[i]) {
            for(int j = 0; j < 3; j++) {
                int k = searchdata[i][j];
                if(havedata[k]) {
                    memcpy(idirections[i], idirections[k], sizeof *idirections);
                    memcpy(ispeeds[i], ispeeds[k], sizeof *ispeeds);
                    istorm[i] = istorm[k];
                    icalm[i] = icalm[k];
                    goto outer_continue;
                }
            }
            return false;
        outer_continue:;
        }

    int dir_cnt = m_WindData[month]->dir_cnt;
    for(int i=0; i<dir_cnt; i++) {
        directions[i] = interpquad(idirections[0][i], idirections[1][i],
                                   idirections[2][i], idirections[3][i], latd, lond);
        speeds[i] = interpquad(ispeeds[0][i], ispeeds[1][i],
                               ispeeds[2][i], ispeeds[3][i], latd, lond);
    }

    storm = interpquad(istorm[0], istorm[1], istorm[2], istorm[3], latd, lond);
    calm = interpquad(icalm[0], icalm[1], icalm[2], icalm[3], latd, lond);

    return true;
}

void ClimatologyOverlayFactory::ReadWindData(int month, wxString filename)
{
    ZUFILE *f;
    if(!(f = TryOpenFile(filename)))
        return;

    m_dlg.m_cbWind->Enable();

    wxUint16 header[3];
    if(zu_read(f, header, sizeof header) != sizeof header ||
       header[0] > 180*16 || header[1] > 360*16 || header[2] > 64)
        goto corrupt;

    m_WindData[month] = new WindData(header[0], header[1], header[2]);

    for(int lati = 0; lati < m_WindData[month]->latitudes; lati++) {
        for(int loni = 0; loni < m_WindData[month]->longitudes; loni++) {
            WindData::WindPolar &wp = m_WindData[month]->data[lati*m_WindData[month]->longitudes + loni];
            int dirs = m_WindData[month]->dir_cnt;
            if(zu_read(f, &wp.storm, 1) != 1)
                goto corrupt;
            if(wp.storm != 255) {
                if(zu_read(f, &wp.calm, 1) != 1)
                    goto corrupt;

                wxUint8 directions[8], speeds[8];
                if(zu_read(f, directions, dirs) != dirs)
                    goto corrupt;
                if(zu_read(f, speeds, dirs) != dirs)
                    goto corrupt;

                // note Wind Polar has 0 indice at west, going counter clockwise, we return data 0 north clockwise
                // this will be corrected in future revisions of climatology when the data is rebuilt

                wp.directions = new wxUint8[dirs];
                wp.speeds = new wxUint8[dirs];
                int mapping[8] = {6, 5, 4, 3, 2, 1, 0, 7};
                for(int i=0; i<8; i++) {
                    wp.directions[i] = directions[mapping[i]];
                    wp.speeds[i] = speeds[mapping[i]];
                }
            }
        }
    }
    zu_close(f), free(f);
    return;

corrupt:
    delete m_WindData[month];
    m_WindData[month] = NULL;
    wxLogMessage(climatology_pi + _("wind data file corrupt: ") + filename);
    zu_close(f), free(f);
}

float max_value(float *values, int cnt)
{
    float max = 0;
    for(int i=0; i<cnt; i++)
        if(values[i] > max)
            max = values[i];
    return max;
}

void ClimatologyOverlayFactory::AverageWindData()
{
    int fmonth;
    for(fmonth=0; fmonth<12; fmonth++)
        if(m_WindData[fmonth])
            goto havedata;
    return;

havedata:
    int latitudes = m_WindData[fmonth]->latitudes;
    int longitudes = m_WindData[fmonth]->longitudes;
    int dir_cnt = m_WindData[fmonth]->dir_cnt;
    m_WindData[12] = new WindData(latitudes, longitudes, dir_cnt);


    float *directions = new float[dir_cnt];
    float *speeds = new float[dir_cnt];

    for(int lati = 0; lati < latitudes; lati++)
        for(int loni = 0; loni < longitudes; loni++) {
            double lat = 180.0*((double)lati/latitudes-.5);
            double lon = 360.0*loni/longitudes;

            double storm = 0, calm = 0;
            for(int i=0; i<dir_cnt; i++)
                directions[i] = speeds[i] = 0;

            int mcount = 0;
            for(int month=0; month<12; month++) {
                if(!m_WindData[month])
                    continue;
                WindData::WindPolar *polar = m_WindData[month]->GetPolar(lat, lon);
                if(!polar) {
                    mcount = 0; /* average invalid if missing any month */
                    break;
                }

                int mdir_cnt = m_WindData[month]->dir_cnt;
                storm += polar->storm;
                calm +=  polar->calm;
                for(int i=0; i<dir_cnt; i++) {
                    directions[i] += polar->directions[i*mdir_cnt/dir_cnt];
                    speeds[i] += polar->speeds[i*mdir_cnt/dir_cnt];
                }
                mcount++;
            }

            WindData::WindPolar &wp = m_WindData[12]->data[lati*longitudes + loni];
            if(mcount == 0) {
                wp.storm = 255;
                goto done;
            }

            wp.storm = storm / mcount;
            wp.calm = calm / mcount;

            /* fit back to 256 */
            while(max_value(directions, dir_cnt) >= 256 || max_value(speeds, dir_cnt) >= 256) {
                if(max_value(directions, dir_cnt) == 0) { /* corrupted data */
                    wp.storm = 255;
                    goto done;
                }

                for(int i=0; i<dir_cnt; i++)
                    if(directions[i]) {
                        speeds[i] /= directions[i];
                        directions[i] /= 2;
                        directions[i] = floor(directions[i]);
                        speeds[i] *= directions[i];
                    }
            }

            wp.directions = new wxUint8[dir_cnt];
            wp.speeds = new wxUint8[dir_cnt];

            for(int i=0; i<dir_cnt; i++) {
                wp.directions[i] = directions[i];
                wp.speeds[i] = speeds[i];
            }

        done:;
        }

    delete [] directions;
    delete [] speeds;
}

void ClimatologyOverlayFactory::ReadCurrentData(int month, wxString filename)
{
    ZUFILE *f;
    if(!(f = TryOpenFile(filename)))
        return;

    m_dlg.m_cbCurrent->Enable();

    wxUint16 header[3];
    zu_read(f, header, sizeof header);
    m_CurrentData[month] = new CurrentData(header[0], header[1], header[2]);

    for(int dim = 0; dim<2; dim++)
        for(int lati = 0; lati < m_CurrentData[month]->latitudes; lati++)
            for(int loni = 0; loni < m_CurrentData[month]->longitudes; loni++) {
                int ind = m_CurrentData[month]->longitudes * lati + loni;
                wxInt8 v;
                zu_read(f, &v, 1);
                if(v == -128)
                    m_CurrentData[month]->data[dim][ind] = NAN;
                else
                    m_CurrentData[month]->data[dim][ind] = (float)v / m_CurrentData[month]->multiplier;
            }

    zu_close(f), free(f);
}

void ClimatologyOverlayFactory::AverageCurrentData()
{
    int fmonth;
    for(fmonth=0; fmonth<12; fmonth++)
        if(m_CurrentData[fmonth])
            goto havedata;
    return;

havedata:
    int latitudes = m_CurrentData[fmonth]->latitudes;
    int longitudes = m_CurrentData[fmonth]->longitudes;
    m_CurrentData[12] = new CurrentData(latitudes, longitudes, 1);

    for(int lati = 0; lati < latitudes; lati++)
        for(int loni = 0; loni < longitudes; loni++) {
            double u = 0, v = 0;
            int mcount = 0;
            for(int month=0; month<12; month++) {
                if(!m_CurrentData[month]
                   || m_CurrentData[month]->latitudes != latitudes
                   || m_CurrentData[month]->longitudes != longitudes)
                    continue;
                
                u += m_CurrentData[month]->Value(U, lati, loni);
                v += m_CurrentData[month]->Value(V, lati, loni);
                mcount++;
            }

            static bool nwarned = true;
            if(nwarned && mcount < 12) {
                wxString fmt = _T(" %d ");
                wxLogMessage(climatology_pi + wxString::Format(_("Average Current includes only")
                                                               + fmt + _("months"), mcount));

                nwarned = false;
            }

            m_CurrentData[12]->data[0][lati*longitudes + loni] = u / mcount;
            m_CurrentData[12]->data[1][lati*longitudes + loni] = v / mcount;
        }
}

bool ClimatologyOverlayFactory::ReadCycloneData(wxString filename, std::list<Cyclone*> &cyclones, bool south)
{
    ZUFILE *f = TryOpenFile(filename);
    if (!f)
        return false;

    wxUint16 lyear, llastmonth;
    while(zu_read(f, &lyear, sizeof lyear)==sizeof lyear) {
#ifdef __MSVC__
        if(lyear < 1972)
            lyear = 1972;
#endif
        Cyclone *cyclone = new Cyclone;
        llastmonth = 0;

        wxUint8 wk;
        wxUint16 press;
        CycloneState::State lastcyclonestate;
        CycloneDateTime lastdatetime;
        wxInt16 lastlat=-10000, lastlon;
        for(;;) {
            signed char state;
            if(zu_read(f, &state, sizeof state) != sizeof state)
                goto corrupted;

            if(state == -128)
                break;

            CycloneState::State cyclonestate;
            switch(state) {
            case '*': cyclonestate = CycloneState::TROPICAL; break;
            case 'S': cyclonestate = CycloneState::SUBTROPICAL; break;
            case 'E': cyclonestate = CycloneState::EXTRATROPICAL; break;
            case 'W': cyclonestate = CycloneState::WAVE; break;
            case 'L': cyclonestate = CycloneState::REMANENT; break;
            case 'D': case 'X': cyclonestate = CycloneState::UNKNOWN; break;
            default: goto corrupted;
            }

            char lday, lmonth;
            if(zu_read(f, &lday, sizeof lday) != sizeof lday ||
               zu_read(f, &lmonth, sizeof lmonth) != sizeof lmonth)
                goto corrupted;

            if(lmonth < llastmonth)
                lyear++;
            llastmonth = lmonth;

            wxDateTime::Month month = (wxDateTime::Month)(lmonth-1);
            int day = lday/4, hour = (lday%4)*6;
            if(lmonth < 1 || lmonth > 12 ||
               day < 1 || day > wxDateTime::GetNumberOfDays(month, lyear) ||
               hour < 0 || hour >= 24)
                goto corrupted;

            wxInt16 lat, lon;
            if(zu_read(f, &lat, sizeof lat) != sizeof lat ||
               zu_read(f, &lon, sizeof lon) != sizeof lon)
                goto corrupted;

            // make sure it's in range
            if(fabsf((double)lat/10) >= 90 || (double)lon/10 > 15 || (double)lon/10 < -360)
                goto corrupted;

            if(lastlat != -10000) {
                cyclone->states.push_back
                    (new CycloneState(lastcyclonestate, lastdatetime,
                                      (south ? -1 : 1) * (double)lastlat/10, (double)lastlon/10,
                                      (south ? -1 : 1) * (double)lat/10, (double)lon/10,
                                      wk, press));
            }

            lastcyclonestate = cyclonestate;
            lastdatetime = CycloneDateTime(day, month, lyear, hour);
            lastlat = lat, lastlon = lon;
                
            if(zu_read(f, &wk, sizeof wk) != sizeof wk ||
               zu_read(f, &press, sizeof press) != sizeof press)
                goto corrupted;
        }
        cyclones.push_back(cyclone);
    }

    zu_close(f);
    free(f);
    return true;

corrupted:
    wxLogMessage(climatology_pi + _("cyclone data corrupt: ") + filename
                 + wxString::Format(_T(" at %ld"), zu_tell(f)));
    zu_close(f);
    free(f);
    return false;
}

void ClimatologyOverlayFactory::BuildCycloneCache()
{
    std::list<Cyclone*> *cyclones[6] = {&m_epa, &m_wpa, &m_spa, &m_atl, &m_nio, &m_she};

    /* make sure we have all the cyclone theatres */
    for(int i=0; i < 6; i++)
        if(cyclones[i]->empty())
            return;

    int statemask = 0;
    statemask |= 1*m_dlg.m_cfgdlg->m_cbTropical->GetValue();
    statemask |= 2*m_dlg.m_cfgdlg->m_cbSubTropical->GetValue();
    statemask |= 4*m_dlg.m_cfgdlg->m_cbExtraTropical->GetValue();
    statemask |= 8*m_dlg.m_cfgdlg->m_cbRemanent->GetValue();

    for(int i=0; i<CYCLONE_CACHE_SEMAPHORE_COUNT; i++)
        m_cyclone_cache_semaphore.Wait();

    m_cyclone_cache.clear();
    for(int i=0; i < 6; i++) {
        for(std::list<Cyclone*>::iterator it = cyclones[i]->begin(); it != cyclones[i]->end(); it++) {
            Cyclone *s = *it;

            for(std::list<CycloneState*>::iterator it2 = s->states.begin(); it2 != s->states.end(); it2++) {
                if((*it2)->windknots < m_dlg.m_cfgdlg->m_sMinWindSpeed->GetValue())
                    continue;

                if((*it2)->pressure > m_dlg.m_cfgdlg->m_sMaxPressure->GetValue())
                    continue;

                /* rebuild cache for these? */
                wxDateTime dt = (*it2)->datetime.DateTime();
                if((dt < m_dlg.m_cfgdlg->m_dPStart->GetValue()) ||
                   (dt > m_dlg.m_cfgdlg->m_dPEnd->GetValue()))
                    continue;

                /* el nino test */
                int year = (*it2)->datetime.year;
                std::map<int, ElNinoYear>::iterator ipt = m_ElNinoYears.find(year);
                if (ipt == m_ElNinoYears.end()) {
                    if(!m_dlg.m_cfgdlg->m_cbNotAvailable->GetValue() && m_ElNinoYears.size())
                        continue;
                } else {
                    ElNinoYear elninoyear = m_ElNinoYears[year];
                    int month = (*it2)->datetime.month;
                    double value = elninoyear.months[month];
        
                    if(isnan(value)) {
                        if(!m_dlg.m_cfgdlg->m_cbNotAvailable->GetValue())
                            continue;
                    } else {
                        if(value >= .75) {
                            if(!m_dlg.m_cfgdlg->m_cbElNino->GetValue())
                                continue;
                        } else if(value <= -.75) {
                            if(!m_dlg.m_cfgdlg->m_cbLaNina->GetValue())
                                continue;
                        } else if(!m_dlg.m_cfgdlg->m_cbNeutral->GetValue())
                            continue;
                    }
                }

                if(!((1<<(*it2)->state) & statemask))
                    continue;

                int lon[2], lat[2];
                for(int j = 0; j<2; j++) {
                    lon[j] = (*it2)->lon[0] < 15 ? (*it2)->lon[j] : (*it2)->lon[j] - 360;
                    lat[j] = (*it2)->lat[j];
                }

                for(int loni = wxMin(lon[0], lon[1]); loni <= wxMax(lon[0], lon[1]); loni++)
                    for(int lati = wxMin(lat[0], lat[1]); lati <= wxMax(lat[0], lat[1]); lati++) {
                        int hash = (loni * 180 + lati)*12 + (*it2)->datetime.month;
                        m_cyclone_cache[hash].push_back(*it2);
                    }
            }
        }
    }

    for(int i=0; i<CYCLONE_CACHE_SEMAPHORE_COUNT; i++)
        m_cyclone_cache_semaphore.Post();
}

static double strtod_nan(const char *str)
{
    if(!str)
        return NAN;

    double ret;
    if(wxString::FromUTF8(str).ToDouble(&ret))
        return ret;

    return NAN;
}

bool ClimatologyOverlayFactory::ReadElNinoYears(wxString filename)
{
    FILE *f = fopen(filename.mb_str(), "r");
    if (!f) {
        wxLogMessage(climatology_pi + _("failed to open file: ") + filename);
        return false;
    }

    char line[128];
    int header = 1;
    while(fgets(line, sizeof line, f)) {
        if(header)
            header--;
        else {
            //char *saveptr;
            int year = strtol(strtok(line, " "), 0, 10);
            ElNinoYear elninoyear;
            for(int i=0; i<12; i++)
                elninoyear.months[i] = strtod_nan(strtok(0, " \n"));

            m_ElNinoYears[year] = elninoyear;
        }
    }
    return true;
}

void ClimatologyOverlayFactory::DrawLine( double x1, double y1, double x2, double y2,
                                          const wxColour &color, double width )
{
    if( m_pdc ) {
        m_pdc->SetPen( wxPen(color, width ) );
        m_pdc->SetBrush( *wxTRANSPARENT_BRUSH);
        m_pdc->DrawLine(x1, y1, x2, y2);
    } else {
        glEnable( GL_LINE_SMOOTH );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

        glColor4ub(color.Red(), color.Green(), color.Blue(), color.Alpha());
        glLineWidth( width );

        glBegin( GL_LINES );
        glVertex2d( x1, y1 );
        glVertex2d( x2, y2 );
        glEnd();
    }
}

void ClimatologyOverlayFactory::DrawCircle( double x, double y, double r,
                                            const wxColour &color, double width )
{
    if( m_pdc ) {
        m_pdc->SetPen( wxPen(color, width ) );
        m_pdc->SetBrush( *wxTRANSPARENT_BRUSH);
        m_pdc->DrawCircle(x, y, r);
    } else {
        glColor4ub(color.Red(), color.Green(), color.Blue(), color.Alpha());
        glLineWidth( width );

        glBegin( GL_LINE_LOOP );
        for(double t=0; t<2*M_PI; t+=M_PI/24)
            glVertex2d(x+r*cos(t), y+r*sin(t));
        glEnd();
    }
}

struct ColorMap {
    double val;
    wxString text;
    wxUint8 transp;
};

ColorMap WindMap[] =
{{0,  _T("#ffffff"), 0}, {2, _T("#00ffff"), 0},  {4,  _T("#00e4e4"), 0}, {5,  _T("#00d9e4"), 0},
 {6,  _T("#00d9d4"), 0}, {7, _T("#00d9b2"), 0},  {8,  _T("#00d96e"), 0}, {9,  _T("#00d92a"), 0},
 {10, _T("#00d900"), 0}, {11, _T("#2ad900"), 0}, {12, _T("#6ed900"), 0}, {13, _T("#b2d900"), 0},
 {14, _T("#d4d400"), 0}, {15, _T("#d9a600"), 0}, {16, _T("#d90000"), 0}, {17, _T("#d90040"), 0},
 {18, _T("#d90060"), 0}, {19, _T("#ae0080"), 0}, {20, _T("#8300a0"), 0}, {21, _T("#5700c0"), 0},
 {25, _T("#0000d0"), 0}, {30, _T("#0000f0"), 0}, {35, _T("#00a0ff"), 0}, {40, _T("#a0a0ff"), 0},
 {50, _T("#ffffff"), 0}};

ColorMap CurrentMap[] =
{{0,   _T("#0000d9"), 255}, {.1,  _T("#002ad9"), 196},  {.2, _T("#006ed9"), 128},  {.3, _T("#00b2d9"), 64},
 {.4,  _T("#04d4d4"), 0}, {.5,  _T("#06d9a6"), 0},  {.7, _T("#a0d906"), 0},  {.9, _T("#b0d900"), 0},
 {1.2, _T("#c0d900"), 0}, {1.5, _T("#d0ae00"), 0}, {1.8, _T("#e08300"), 0}, {2.1, _T("#e05700"), 0},
 {2.4, _T("#f00000"), 0}, {2.7, _T("#f00004"), 0}, {3.0, _T("#f0001c"), 0}, {3.6, _T("#f00048"), 0},
 {4.2, _T("#f00069"), 0}, {4.8, _T("#f000a0"), 0}, {5.6, _T("#f000f0"), 0}};

ColorMap PressureMap[] =
{{900,  _T("#283282"), 0}, {980,  _T("#273c8c"), 0}, {990,  _T("#264696"), 0}, {1000,  _T("#2350a0"), 0},
 {1001, _T("#1f5aaa"), 0}, {1002, _T("#1a64b4"), 0}, {1003, _T("#136ec8"), 0}, {1004, _T("#0c78e1"), 0},
 {1005, _T("#0382e6"), 0}, {1006, _T("#0091e6"), 0}, {1007, _T("#009ee1"), 0}, {1008, _T("#00a6dc"), 0},
 {1009, _T("#00b2d7"), 0}, {1010, _T("#00bed2"), 0}, {1011, _T("#28c8c8"), 0}, {1012, _T("#78d2aa"), 0},
 {1013, _T("#8cdc78"), 0}, {1014, _T("#a0eb5f"), 0}, {1015, _T("#c8f550"), 0}, {1016, _T("#f3fb02"), 0},
 {1017, _T("#ffed00"), 0}, {1018, _T("#ffdd00"), 0}, {1019, _T("#ffc900"), 0}, {1020, _T("#ffab00"), 0},
 {1021, _T("#ff8100"), 0}, {1022, _T("#f1780c"), 0}, {1024, _T("#e26a23"), 0}, {1028, _T("#d5453c"), 0},
 {1040, _T("#b53c59"), 0}};

ColorMap SeaTempMap[] =
{{0, _T("#0000d9"), 0},  {3, _T("#002ad9"), 0},  {6, _T("#006ed9"), 0},  {9, _T("#00b2d9"), 0},
 {12, _T("#00d4d4"), 0}, {15, _T("#00d9a6"), 0}, {18, _T("#00d900"), 0}, {20, _T("#95d900"), 0},
 {22, _T("#d9d900"), 0}, {23, _T("#d9ae00"), 0}, {24, _T("#d98300"), 0}, {25, _T("#d95700"), 0},
 {26, _T("#d90000"), 0}, {27, _T("#ae0000"), 0}, {28, _T("#8c0000"), 0}, {29, _T("#870000"), 0},
 {30, _T("#690000"), 0}, {32, _T("#550000"), 0}, {35, _T("#410000"), 0}};

ColorMap AirTempMap[] =
{{-50, _T("#0000d9"), 0},  {-40, _T("#002ad9"), 0},  {-30, _T("#006ed9"), 0},  {-20, _T("#00b2d9"), 0},
 {-10, _T("#00d4d4"), 0}, {0, _T("#00d9a6"), 0}, {5, _T("#00d900"), 0}, {8, _T("#95d900"), 0},
 {12, _T("#d9d900"), 0}, {15, _T("#d9ae00"), 0}, {18, _T("#d98300"), 0}, {21, _T("#d95700"), 0},
 {24, _T("#d90000"), 0}, {27, _T("#ae0000"), 0}, {30, _T("#8c0000"), 0}, {35, _T("#870000"), 0},
 {40, _T("#690000"), 0}, {45, _T("#550000"), 0}, {50, _T("#410000"), 0}};

ColorMap CloudMap[] =
{{0, _T("#f0f0e6"), 255},  {10, _T("#e6e6dc"), 224}, {20, _T("#dcdcd2"), 192},
 {30, _T("#c8c8b4"), 160}, {40, _T("#aaaa8c"), 128}, {50, _T("#969678"), 96}, {60, _T("#787864"), 64},
 {70, _T("#646450"), 32},  {80, _T("#5a5a46"), 0},   {90, _T("#505036"), 0}, {100, _T("#404036"), 0}};

ColorMap PrecipitationMap[] =
{{0, _T("#000080"), 255}, {1, _T("#0000a0"), 208}, {2, _T("#4040f0"), 164},
 {3, _T("#8080f0"), 128}, {4, _T("#f0f0f0"), 64}, {5, _T("#f0a0f0"), 32}, {8, _T("#f080f0"), 0},
 {10, _T("#f04080"), 0}, {13, _T("#f00040"), 0}, {16, _T("#f00000"), 0}, {19, _T("#800000"), 0}};

ColorMap RelativeHumidityMap[] =
{{0,  _T("#000000"), 0}, {30, _T("#303030"), 0}, {60, _T("#606060"), 0},
 {65, _T("#a06060"), 0}, {70, _T("#ff6060"), 0}, {75, _T("#ffc080"), 0},
 {80, _T("#a0f0a0"), 0}, {85, _T("#60f0f0"), 0}, {95, _T("#40a0f0"), 0},
 {100, _T("#2080f0"), 0}};

ColorMap LightningMap[] =
{{0, _T("#490000"), 255},  {64, _T("#890000"), 128}, {128, _T("#a98900"), 64},
 {192, _T("#ffd900"), 0}, {255, _T("#ffff00"), 0}};

ColorMap SeaDepthMap[] =
{{0, _T("#0000d9"), 255},  {20, _T("#002ad9"), 0},   {50, _T("#006ed9"), 0},   {100, _T("#00b2d9"), 0},
 {150, _T("#00d4d4"), 0},  {250, _T("#00d9a6"), 0},  {400, _T("#00d900"), 0},  {600, _T("#95d900"), 0},
 {800, _T("#a9d900"), 0},  {1000, _T("#d9d900"), 0}, {1200, _T("#d9f000"), 0}, {1400, _T("#d9c040"), 0},
 {2000, _T("#a9c040"), 0}, {3000, _T("#a0a040"), 0}, {4000, _T("#808060"), 0}, {5000, _T("#606060"), 0},
 {6000, _T("#404040"), 0}, {8000, _T("#202020"), 0}, {10000, _T("#000000"), 0}};

ColorMap CycloneMap[] =
{{0, _T("#0000d9"), 255}, {10, _T("#002ad9"), 200}, {20, _T("#006ed9"), 128}, {30, _T("#00b2d9"), 96},
 {40, _T("#00d4d4"), 64},  {50, _T("#00d9a6"), 64}, {60, _T("#00d900"), 64}, {70, _T("#75d900"), 64},
 {80, _T("#a9d900"), 32},  {90, _T("#a9ae00"), 32}, {100, _T("#d98000"), 32}, {110, _T("#d94000"), 32},
 {120, _T("#d90000"), 0}, {130, _T("#ff0000"), 0}, {140, _T("#ff0080"), 0}, {150, _T("#ff00ff"), 0},
 {160, _T("#ff40ff"), 0}, {180, _T("#ff80ff"), 0}, {200, _T("#ffffff"), 0}};

ColorMap *ColorMaps[] = {WindMap, CurrentMap, PressureMap, SeaTempMap, AirTempMap,
                         CloudMap, PrecipitationMap, RelativeHumidityMap, LightningMap,
                         SeaDepthMap, CycloneMap};

const int ColorMapLens[] = { (sizeof WindMap) / (sizeof *WindMap),
                             (sizeof CurrentMap) / (sizeof *CurrentMap),
                             (sizeof PressureMap) / (sizeof *PressureMap),
                             (sizeof SeaTempMap) / (sizeof *SeaTempMap),
                             (sizeof AirTempMap) / (sizeof *AirTempMap),
                             (sizeof CloudMap) / (sizeof *CloudMap),
                             (sizeof PrecipitationMap) / (sizeof *PrecipitationMap),
                             (sizeof RelativeHumidityMap) / (sizeof *RelativeHumidityMap),
                             (sizeof LightningMap) / (sizeof *LightningMap),
                             (sizeof SeaDepthMap) / (sizeof *SeaDepthMap),
                             (sizeof CycloneMap) / (sizeof *CycloneMap)};


wxColour ClimatologyOverlayFactory::GetGraphicColor(int setting, double val_in)
{
    if(isnan(val_in))
        return wxColour(0, 0, 0, 0); /* transparent */

    int colormap_index = setting;
    ColorMap *map = ColorMaps[colormap_index];
    int maplen = ColorMapLens[colormap_index];

    double cmax = 1;
    for(int i=1; i<maplen; i++) {
        double nmapvala = map[i-1].val/cmax;
        double nmapvalb = map[i].val/cmax;
        if(nmapvalb > val_in || i==maplen-1) {
            wxColour b, c;
            c.Set(map[i].text);
            b.Set(map[i-1].text);
            double d = (val_in-nmapvala)/(nmapvalb-nmapvala);
            c.Set((1-d)*b.Red()   + d*c.Red(),
                  (1-d)*b.Green() + d*c.Green(),
                  (1-d)*b.Blue()  + d*c.Blue(),
                  255 - ((1-d)*map[i-1].transp + d*map[i].transp));
            return c;
        }
    }
    return *wxBLACK; /* unreachable */
}

bool ClimatologyOverlayFactory::CreateGLTexture(ClimatologyOverlay &O,
                                                int setting, int month,
                                                PlugIn_ViewPort &vp)
{
    double s;
    switch(setting) {
    case ClimatologyOverlaySettings::WIND:   
        s = (m_WindData[month] && m_WindData[month]->longitudes > 360) ? 2 : 1;
        break;
    case ClimatologyOverlaySettings::CURRENT: s = 3;  break;
    case ClimatologyOverlaySettings::SLP:     s = .5; break;
    case ClimatologyOverlaySettings::AT:      s = .5; break;
    case ClimatologyOverlaySettings::CLOUD:   s = .5; break;
    default: s=1;
    }

    wxProgressDialog *progressdialog = NULL;
    wxDateTime start = wxDateTime::Now();

    int width = s*360+1;
    int height = s*360;

    unsigned char *data = new unsigned char[width*height*4];

    for(int x = 0; x < width; x++) {
        if(x % 40 == 0) {
            if(progressdialog)
                progressdialog->Update(x);
            else {
                wxDateTime now = wxDateTime::Now();
                if((now-start).GetMilliseconds() > 1000 && x < width/2) {
                    progressdialog = new wxProgressDialog(
                        _("Building Overlay map"), _("Climatology"), width+1, &m_dlg,
                        wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
                }
            }
        }

        for(int y = 0; y < height; y++) {
            /* put in mercator coordinates */
            double lat = M_PI*(2.0*y/height-1);
            lat = 2*rad2deg(atan(exp(lat))) - 90;
            double lon = x/s;

            double v = getValueMonth(MAG, setting, lat, lon, month);
            wxColour c = GetGraphicColor(setting, v);

            int doff = 4*(y*width + x);
            data[doff + 0] = c.Red();
            data[doff + 1] = c.Green();
            data[doff + 2] = c.Blue();
            data[doff + 3] = c.Alpha();
        }
    }
    delete progressdialog;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);

    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glPopClientAttrib();

    delete [] data;

    O.m_iTexture = texture;
    O.m_width = width - 1;
    O.m_height = height;

    return true;
}

static inline void glTexCoord2d_2(int multitexturing, double x, double y)
{
    if(multitexturing) {
        s_glMultiTexCoord2dARB(GL_TEXTURE0_ARB, x, y);
        s_glMultiTexCoord2dARB(GL_TEXTURE1_ARB, x, y);
    } else
        glTexCoord2d(x, y);
}

void ClimatologyOverlayFactory::DrawGLTexture( ClimatologyOverlay &O1, ClimatologyOverlay &O2,
                                               double dpos, PlugIn_ViewPort &vp, double transparency)
{ 
    if( !O1.m_iTexture || !O2.m_iTexture )
        return;

    int multitexturing;
    if(&O1 == &O2)
        multitexturing = 0;
    else
        multitexturing = s_multitexturing;

    if(multitexturing) {
        s_glActiveTextureARB (GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_RECTANGLE_ARB);
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, O2.m_iTexture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        s_glActiveTextureARB (GL_TEXTURE1_ARB);
    } else
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, O1.m_iTexture);

    if(multitexturing) {
        float fpos = dpos;
        GLfloat constColor[4] = {0, 0, 0, fpos};

        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);

        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);
        glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_INTERPOLATE_ARB);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);

        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
        glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
        glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);

        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);
        glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PREVIOUS_ARB);
        glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
        glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, GL_SRC_ALPHA);

        if(multitexturing > 1) {
            s_glActiveTextureARB (GL_TEXTURE2_ARB);

            glEnable(GL_TEXTURE_RECTANGLE_ARB);
            glBindTexture(GL_TEXTURE_RECTANGLE_ARB, O2.m_iTexture);

            constColor[3] = 1 - transparency;
            glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor);
        
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
            glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);

            glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
            glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB);
            glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
            glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);

            glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_CONSTANT_ARB);
            glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_CONSTANT_ARB);
            glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PRIMARY_COLOR_ARB);
            glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
        }
    }

    glColor4f(1, 1, 1, 1 - transparency);
    
    int x = vp.rv_rect.x, y = vp.rv_rect.y;
    int w = vp.rv_rect.width, h = vp.rv_rect.height;

    double lat1, lon1, lat2, lon2;
    double tr = vp.rotation;
    vp.rotation = 0;
    GetCanvasLLPix( &vp, wxPoint(x, y), &lat1, &lon1 );
    GetCanvasLLPix( &vp, wxPoint(w, h), &lat2, &lon2 );
    vp.rotation = tr;

    lon1 = positive_degrees(lon1), lon2 = positive_degrees(lon2);
    lon2 *= O1.m_width/360.0, lon1 *= O1.m_width/360.0;

    double s1 = sin(deg2rad(lat1)), s2 = sin(deg2rad(lat2));
    double y1 = .5 * log((1 + s1) / (1 - s1)), y2 = .5 * log((1 + s2) / (1 - s2));
    y1 = O1.m_height*(1 + y1/M_PI)/2, y2 = O1.m_height*(1 + y2/M_PI)/2;

    lon2+=1;
    y2+=1;
    

    glPushMatrix();

    double u = vp.pix_width/2, v = vp.pix_height/2;
    glTranslated(u, v, 0);
    glRotatef(tr*180/M_PI, 0, 0, 1);
    glTranslated(-u, -v, 0);

    glBegin(GL_QUADS);
    double x0 = x;
    if(lon1 > lon2) {
        wxPoint p;
        GetCanvasPixLL( &vp, &p, 0, 0 );
        x = p.x;

        glTexCoord2d_2(multitexturing, lon1,        y1),  glVertex2i(x0, y);
        glTexCoord2d_2(multitexturing, O1.m_width+1, y1), glVertex2i(x, y);
        glTexCoord2d_2(multitexturing, O1.m_width+1, y2), glVertex2i(x, h);
        glTexCoord2d_2(multitexturing, lon1,        y2),  glVertex2i(x0, h);
        lon1 = 0;
    }

    glTexCoord2d_2(multitexturing, lon1, y1), glVertex2i(x, y);
    glTexCoord2d_2(multitexturing, lon2, y1), glVertex2i(w, y);
    glTexCoord2d_2(multitexturing, lon2, y2), glVertex2i(w, h);
    glTexCoord2d_2(multitexturing, lon1, y2), glVertex2i(x, h);
    glEnd();

    glPopMatrix();


    if(multitexturing) {
        if(multitexturing > 1) {
            glDisable(GL_TEXTURE_RECTANGLE_ARB);
            s_glActiveTextureARB (GL_TEXTURE1_ARB);
        }
        glDisable(GL_TEXTURE_RECTANGLE_ARB);
        s_glActiveTextureARB (GL_TEXTURE0_ARB);
    }

    glDisable(GL_TEXTURE_RECTANGLE_ARB);
}

/* return cached wxImage for a given number, or create it if not in the cache */
wxImage &ClimatologyOverlayFactory::getLabel(double value)
{
    std::map <double, wxImage >::iterator it;
    double nvalue = isnan(value) ? 99999999 : value;
            
    it = m_labelCache.find(nvalue);
    if (it != m_labelCache.end())
        return m_labelCache[nvalue];

    wxString labels;
    if(isnan(value))
        labels = _("N/A");
    else
        labels.Printf(_T("%.0f"), value);
    wxColour text_color;
    GetGlobalColor( _T ( "DILG3" ), &text_color );
    wxColour back_color;
    wxPen penText(text_color);

    GetGlobalColor( _T ( "DILG0" ), &back_color );
    wxBrush backBrush(back_color);

    wxMemoryDC mdc(wxNullBitmap);

    wxFont mfont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
    mdc.SetFont( mfont );

    int w, h;
    mdc.GetTextExtent(labels, &w, &h);

    int label_offset = 5;

    wxBitmap bm(w +  label_offset*2, h + 2);
    mdc.SelectObject(bm);
    mdc.Clear();

    mdc.SetPen(penText);
    mdc.SetBrush(backBrush);
    mdc.SetTextForeground(text_color);
    mdc.SetTextBackground(back_color);
          
    int xd = 0;
    int yd = 0;
    mdc.DrawText(labels, label_offset + xd, yd+1);
          
    mdc.SelectObject(wxNullBitmap);

    m_labelCache[nvalue] = bm.ConvertToImage();
    m_labelCache[nvalue].InitAlpha();

    wxImage &image = m_labelCache[nvalue];

    unsigned char *d = image.GetData();
    unsigned char *a = image.GetAlpha();

    w = image.GetWidth(), h = image.GetHeight();
    for( int y = 0; y < h; y++ )
        for( int x = 0; x < w; x++ ) {
            int r, g, b;
            int ioff = (y * w + x);
            r = d[ioff* 3 + 0];
            g = d[ioff* 3 + 1];
            b = d[ioff* 3 + 2];

            a[ioff] = 255-(r+g+b)/3;
        }

    return m_labelCache[nvalue];
}

/* give value for y at a given x location on a segment */
static double interp_value(double v0, double v1, double d)
{
    return (1-d)*v0 + d*v1;
}

// interpolate two angles in range +- PI, with resulting angle in the same range
static double interp_angle(double a0, double a1, double d)
{
    if(isnan(a0)) return a1;
    if(isnan(a1)) return a0;
    if(a0 - a1 > M_PI) a0 -= 2*M_PI;
    else if(a1 - a0 > M_PI) a1 -= 2*M_PI;
    double a = (1-d)*a0 + d*a1;
    if(a < -M_PI) a += 2*M_PI;
    return a;
}

double ArrayValue(wxInt16 *a, int index)
{
    int v = a[index];
    if(v == 32767)
        return NAN;
    return v;
}

double InterpArray(double x, double y, wxInt16 *a, int h)
{
    if(y<0) y+=h;
    int x0 = floor(x), x1 = x0+1;
    int y0 = floor(y), y1 = y0+1;
    int y1v = y1;
    if(y1v == h) y1v = 0;

    double v00 = ArrayValue(a, x0*h + y0), v01 = ArrayValue(a, x0*h + y1v);
    double v10 = ArrayValue(a, x1*h + y0), v11 = ArrayValue(a, x1*h + y1v);

    double v0 = interp_value(v00, v01, y-y0);
    double v1 = interp_value(v10, v11, y-y0);
    return      interp_value(v0,   v1, x-x0);
}

double WindData::WindPolar::Value(enum Coord coord, int dir_cnt)
{
    if(storm == 255)
        return NAN;

    if(coord == DIRECTION) // maybe should do most likely here rather than vector average?
        return atan2(Value(U, dir_cnt), Value(V, dir_cnt));

    int totald = 0, totals = 0;
    for(int i=0; i<dir_cnt; i++) {
        totald += directions[i];

        double mul = 0;
        switch(coord) {
        case U: mul = sin(i*2*M_PI/dir_cnt); break;
        case V: mul = cos(i*2*M_PI/dir_cnt); break;
        case MAG: mul = 1; break;
        default: printf("error, invalid coord: %d\n", coord);
        }

        totals += mul*speeds[i];
    }
    return (double)totals / totald;
}

double CurrentData::Value(enum Coord coord, int xi, int yi)
{
    if(xi < 0 || xi >= latitudes)
        return NAN;

    double u = data[0][xi*longitudes + yi], v = data[1][xi*longitudes + yi];
    switch(coord) {
    case U: return u;
    case V: return v;
    case MAG: return hypot(u, v);
    case DIRECTION: return !u && !v ? NAN : atan2(u, v);
    default: printf("error, invalid coord: %d\n", coord);
    }
    return NAN;
}

double WindData::InterpWind(enum Coord coord, double x, double y)
{
    y = positive_degrees(y - 135.0/longitudes + .25);
    double xi = latitudes*(.5 - (90.0/latitudes - x)/180.0);
    double yi = longitudes*y/360.0;
    int h = longitudes, d = dir_cnt;

    if(yi<0) yi+=h;

    int x0 = floor(xi), x1 = x0+1;
    int y0 = floor(yi), y1 = y0+1;
    int y1v = y1;
    if(x1 == latitudes)
        x1 = x0;
    if(y1v == h) y1v = 0;

    double v00 = data[x0*h + y0].Value(coord, d), v01 = data[x0*h + y1v].Value(coord, d);
    double v10 = data[x1*h + y0].Value(coord, d), v11 = data[x1*h + y1v].Value(coord, d);

    if(coord == DIRECTION) {
        double a0 = interp_angle(v00, v01, yi-y0);
        double a1 = interp_angle(v10, v11, yi-y0);
        return      interp_angle(a0,  a1,  xi-x0 ) * 180/M_PI;
    }

    double v0 = interp_value(v00, v01, yi-y0);
    double v1 = interp_value(v10, v11, yi-y0);
    return      interp_value(v0,  v1,  xi-x0 );
}

double CurrentData::InterpCurrent(enum Coord coord, double x, double y)
{
    y = positive_degrees(y);
    double xi = (latitudes-1)*(.5 - x/160.0);
    double yi = longitudes*y/360.0;
    int h = longitudes;

    if(xi<0) xi+=latitudes;

    int x0 = floor(xi), x1 = x0+1;
    int y0 = floor(yi), y1 = y0+1;
    int y1v = y1;
    if(y1v == h) y1v = 0;

    double v00 = Value(coord, x0, y0), v01 = Value(coord, x0, y1);
    double v10 = Value(coord, x1, y0), v11 = Value(coord, x1, y1);

    if(coord == DIRECTION) {
        double a0 = interp_angle(v00, v01, yi-y0);
        double a1 = interp_angle(v10, v11, yi-y0);
        return      interp_angle(a0,  a1,  xi-x0 ) * 180/M_PI;
    }

    double v0 = interp_value(v00, v01, yi-y0);
    double v1 = interp_value(v10, v11, yi-y0);
    return      interp_value(v0,  v1,  xi-x0 );
}

static double interp_table_value(double x, double x1, double x2, double y1, double y2)
{
    if(x == x1)
        return y1;
    if(x == x2)
        return y2;
    return (y2 - y1)*(x - x1)/(x2 - x1) + y1;
}

static double InterpTable(double ind, const double table[], int tablesize)
{
    int ind1 = floor(ind), ind2 = ceil(ind);
    if(ind1 < 0)
        return table[0];
   if(ind2 >= tablesize)
        return table[tablesize - 1];

    return interp_table_value(ind, ind1, ind2, table[ind1], table[ind2]);
}
 
double ClimatologyOverlayFactory::getValueMonth(enum Coord coord, int setting,
                                                double lat, double lon, int month)
{
    if(coord != MAG &&
       setting != ClimatologyOverlaySettings::WIND &&
       setting != ClimatologyOverlaySettings::CURRENT)
        return NAN;

    if(isnan(lat) || isnan(lon))
        return NAN;

    switch(setting) {
    case ClimatologyOverlaySettings::WIND:
        if(m_WindData[month])
            return m_WindData[month]->InterpWind(coord, lat, lon);
        break;
    case ClimatologyOverlaySettings::CURRENT:
        if(m_CurrentData[month])
            return m_CurrentData[month]->InterpCurrent(coord, lat, lon);
        break;
    case ClimatologyOverlaySettings::SLP:
        return InterpArray((-lat+90)/2-.5, positive_degrees(lon-1.5)/2,
                           m_slp[month][0], 180) * .01f + 1000.0;
    case ClimatologyOverlaySettings::SST:
        return InterpArray((-lat+90)-.5, positive_degrees(lon-.5),
                           m_sst[month][0], 360) * .001f + 15.0;
    case ClimatologyOverlaySettings::AT:
        return InterpArray((-lat+90)/2-.5, positive_degrees(lon-.5)/2,
                           m_at[month][0], 180) / 3.0;
    case ClimatologyOverlaySettings::CLOUD:
        return InterpArray((-lat+90)/2-.5, positive_degrees(lon-.5)/2,
                           m_cld[month][0], 180) * .001f * 12.5;
    case ClimatologyOverlaySettings::PRECIPITATION:
        return InterpArray((-lat+90)/2.5, positive_degrees(lon-2)/2.5,
                           m_precip[month][0], 144) * .002f;
    case ClimatologyOverlaySettings::RELATIVE_HUMIDITY:
        return InterpArray((-lat+90), positive_degrees(lon-.5),
                           m_rhum[month][0], 360)/2.0;
    case ClimatologyOverlaySettings::LIGHTNING:
        return InterpArray((-lat+90), positive_degrees(lon-.5),
                           m_lightn[month][0], 360);
    case ClimatologyOverlaySettings::SEADEPTH:
    {
        double ind = InterpArray((-lat+90), positive_degrees(lon-.5),
                           m_seadepth[0], 360);
        const double table[] = {0, 10, 20, 30, 50, 75, 100, 125, 150,
                                200, 250, 300, 400, 500, 600, 700, 800,
                                900, 1000, 1100, 1200, 1300, 1400, 1500,
                                1750, 2000, 2500, 3000, 3500, 4000, 4500,
                                5000, 5500, 6000, 6500, 7000, 7500, 8000,
                                9000, 10000};
        return InterpTable(ind, table, (sizeof table) / (sizeof *table));
    }
    }
    return NAN;
}

double ClimatologyOverlayFactory::getValue(enum Coord coord, int setting,
                                           double lat, double lon, wxDateTime *date)
{
    int month, nmonth;
    double dpos;
    GetDateInterpolation(date, month, nmonth, dpos);

    double v1 = getValueMonth(coord, setting, lat, lon, month);
    double v2 = getValueMonth(coord, setting, lat, lon, nmonth);

    if(coord == DIRECTION) {
        if(v1 - v2 > 180) v1 -= 360;
        if(v2 - v1 > 180) v2 -= 360;
        return positive_degrees(dpos * v1 + (1-dpos) * v2);
    }

    return dpos * v1 + (1-dpos) * v2;
}

double ClimatologyOverlayFactory::getCurCalibratedValue(enum Coord coord, int setting, double lat, double lon)
{
    double v = getCurValue(coord, setting, lat, lon);
    if(coord == DIRECTION)
        return v;

    return m_dlg.m_cfgdlg->m_Settings.CalibrateValue(setting, v);
}

double ClimatologyOverlayFactory::getCalibratedValueMonth(enum Coord coord, int setting, double lat, double lon, int month)
{
    double v = getValueMonth(coord, setting, lat, lon, month);
    if(coord == DIRECTION)
        return v;

    return m_dlg.m_cfgdlg->m_Settings.CalibrateValue(setting, v);
}

double ClimatologyOverlayFactory::GetMin(int setting)
{
    switch(setting) {
    case ClimatologyOverlaySettings::WIND: return 0;
    case ClimatologyOverlaySettings::CURRENT: return 0;
    case ClimatologyOverlaySettings::SLP: return 920;
    case ClimatologyOverlaySettings::SST: return 0;
    case ClimatologyOverlaySettings::AT:  return -50;
    case ClimatologyOverlaySettings::CLOUD: return 0;
    case ClimatologyOverlaySettings::PRECIPITATION:  return 0;
    case ClimatologyOverlaySettings::RELATIVE_HUMIDITY:  return 0;
    case ClimatologyOverlaySettings::LIGHTNING:  return 0;
    case ClimatologyOverlaySettings::SEADEPTH:  return 0;
    default: return 0;
    }
}

double ClimatologyOverlayFactory::GetMax(int setting)
{
    switch(setting) {
    case ClimatologyOverlaySettings::WIND: return 100;
    case ClimatologyOverlaySettings::CURRENT: return 10;
    case ClimatologyOverlaySettings::SLP: return 1080;
    case ClimatologyOverlaySettings::SST: return 35;
    case ClimatologyOverlaySettings::AT:  return 50;
    case ClimatologyOverlaySettings::CLOUD: return 8;
    case ClimatologyOverlaySettings::PRECIPITATION:  return 1000;
    case ClimatologyOverlaySettings::RELATIVE_HUMIDITY:  return 100;
    case ClimatologyOverlaySettings::LIGHTNING:  return 100; // ???
    case ClimatologyOverlaySettings::SEADEPTH:  return 40;
    default: return NAN;
    }
}

#define EPS 2e-14
#define EPS2 2e-7

inline int TestIntersectionXY(double x1, double y1, double x2, double y2,
                              double x3, double y3, double x4, double y4)
{
    double ax = x2 - x1, ay = y2 - y1;
    double bx = x3 - x4, by = y3 - y4;
    double cx = x1 - x3, cy = y1 - y3;

    double denom = ay * bx - ax * by;

    if(fabs(denom) < EPS) { /* parallel or really close to parallel */
//        if(fabs((y1*ax - ay*x1)*bx - (y3*bx - by*x3)*ax) > EPS2)
//            return 0; /* different intercepts, no intersection */
      return 1;
//        return 0;
    }

    double recip = 1 / denom;
    double na = (by * cx - bx * cy) * recip;
    if(na < -EPS2 || na > 1 + EPS2)
        return 0;

    double nb = (ax * cy - ay * cx) * recip;
    if(nb < -EPS2 || nb > 1 + EPS2)
        return 0;

    return 1;
}

int ClimatologyOverlayFactory::CycloneTrackCrossings(double lat1, double lon1, double lat2, double lon2,
                                                     const wxDateTime &date, int dayrange)
{
    // if dayrange is zero, we are just probing, so other parameters are likely invalid
    if(!dayrange)
        return 0;

    m_cyclone_cache_semaphore.Wait();
    /* build hash table cache to speed up cyclone crossing calculation */
    if(m_cyclone_cache.empty()) {
        m_cyclone_cache_semaphore.Post();
        return -1;
    }
        
    int lon_min = wxMin(lon1, lon2), lon_max = wxMax(lon1, lon2);

    if(lon_min > 15 || lon_max > 15)
        lon_min -= 360, lon_max -= 360;

    int lat_min = wxMin(lat1, lat2), lat_max = wxMax(lat1, lat2);

    int day = date.GetMonth()*365/12 + date.GetDay()-1;
    int day1 = day - dayrange/2, day2 = day + dayrange/2;
    if(day1 < 0) day1 += 365;
    if(day2 >= 365) day2 -= 365;
    int day_min = wxMin(day1, day2), day_max = wxMax(day1, day2);

    int month_min = day_min * 12 / 365, month_max = day_max * 12 / 365;

    for(int loni = lon_min; loni <= lon_max; loni++)
        for(int lati = lat_min; lati <= lat_max; lati++) {
            int monthi = month_min;
            do {
                if(monthi==12) monthi = 0;

                int hash = (floor((double)loni) * 180
                            + floor((double)lati))*12 + monthi;

                std::list<CycloneState*> &cyclonestates = m_cyclone_cache[hash];
                for(std::list<CycloneState*>::iterator it = cyclonestates.begin();
                    it != cyclonestates.end(); it++) {
                    wxPoint p;
                    CycloneState *ss = *it;
                        
                    int cday = ss->datetime.month*365/12 + ss->datetime.day - 1;
                    int daydiff = cday - day;
                    if(daydiff > 183)
                        daydiff = 365 - daydiff;

                    if(daydiff < dayrange/2) {
                        /* we should split all cyclones at 15 degrees longitude... until then... */
                        while(lon1 - ss->lon[0] > 180) lon1 -= 360, lon2 -= 360;
                        while(lon1 - ss->lon[0] < -180) lon1 += 360, lon2 += 360;
                                
                        if(TestIntersectionXY(lat1, lon1, lat2, lon2,
                                              ss->lat[0], ss->lon[0],
                                              ss->lat[1], ss->lon[1])) {
                            m_cyclone_cache_semaphore.Post();
                            return 1;
                        }
                    }
                }
            } while(++monthi <= month_max);
        }

    m_cyclone_cache_semaphore.Post();
    return 0;
}

void ClimatologyOverlayFactory::RenderOverlayMap( int setting, PlugIn_ViewPort &vp)
{
    if(!m_Settings.Settings[setting].m_bOverlayMap)
        return;

    int month, nmonth;
    double dpos;

    if(setting == ClimatologyOverlaySettings::SEADEPTH) {
        month = nmonth = 0;
        dpos = 1;
    } else
        GetDateInterpolation(NULL, month, nmonth, dpos);

    if(!m_Settings.Settings[setting].m_bOverlayInterpolation) {
        nmonth = month;
        dpos = 1;
    }

    ClimatologyOverlay &O1 = m_pOverlay[month][setting];
    ClimatologyOverlay &O2 = m_pOverlay[nmonth][setting];

    if( !m_pdc )
    {
        if( !O1.m_iTexture )
            CreateGLTexture( O1, setting, month, vp);

        if( !O2.m_iTexture )
            CreateGLTexture( O2, setting, nmonth, vp);

        DrawGLTexture( O1, O2, dpos, vp, m_Settings.Settings[setting].m_iOverlayTransparency/100.0 );
    }
    else
    {
#if 0  
        if( !pO.m_pDCBitmap ) {
            wxImage bl_image = CreateImage( settings, pGRA, vp, porg );
            if( bl_image.IsOk() ) {
                //    Create a Bitmap
                pGO->m_pDCBitmap = new wxBitmap( bl_image );
                wxMask *gr_mask = new wxMask( *( pGO->m_pDCBitmap ), wxColour( 0, 0, 0 ) );
                pGO->m_pDCBitmap->SetMask( gr_mask );
            }
        }
      
        if( pGO->m_pDCBitmap )
            m_pdc->DrawBitmap( *( pGO->m_pDCBitmap ), porg.x, porg.y, true );
#else
    wxString msg = _("Climatology overlay map unsupported unless OpenGL is enabled");

    wxMemoryDC mdc;
    wxBitmap bm( 1000, 1000 );
    mdc.SelectObject( bm );
    mdc.Clear();

    wxFont font( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );

    mdc.SetFont( font );
    mdc.SetPen( *wxTRANSPARENT_PEN);

    mdc.SetBrush( wxColour(243, 47, 229 ) );
    int w, h;
    mdc.GetMultiLineTextExtent( msg, &w, &h );
    h += 2;
    int label_offset = 10;
    int wdraw = w + ( label_offset * 2 );
    mdc.DrawRectangle( 0, 0, wdraw, h );

    mdc.DrawLabel( msg, wxRect( label_offset, 0, wdraw, h ), wxALIGN_LEFT| wxALIGN_CENTRE_VERTICAL);
    mdc.SelectObject( wxNullBitmap );

    wxBitmap sbm = bm.GetSubBitmap( wxRect( 0, 0, wdraw, h ) );
    int x = vp.pix_width, y = vp.pix_height;
    m_pdc->DrawBitmap( sbm, (x-wdraw)/2, y - ( GetChartbarHeight() + h ), false );
#endif        
    }
}

ZUFILE *ClimatologyOverlayFactory::TryOpenFile(wxString filename)
{
    const wxString ext = _T(".gz");
    ZUFILE *f = zu_open(filename.mb_str(), "rb", ZU_COMPRESS_AUTO);
    if(!f)
        f = zu_open((filename+ext).mb_str(), "rb", ZU_COMPRESS_AUTO);
    if(!f) {
        m_bFailedLoading = true;
        wxString msg = _("failed to read file: ") + filename;
        m_sFailedMessage += msg + _T("\n");
        wxLogMessage(climatology_pi + msg);
    }
    return f;
}

void ClimatologyOverlayFactory::RenderNumber(wxPoint p, double v, const wxColour &color)
{
    wxImage &label = getLabel(v);
    int w = label.GetWidth(), h = label.GetHeight();

    if( m_pdc ) {
        m_pdc->SetPen( wxPen(color) );        
        m_pdc->DrawBitmap(label, p.x-w/2, p.y-h/2, true);
    } else {
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
        glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA,
                     w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, label.GetData());
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, w, h,
                        GL_ALPHA, GL_UNSIGNED_BYTE, label.GetAlpha());
        
        glColor4ub(color.Red(), color.Green(), color.Blue(), color.Alpha());

        glEnable(GL_TEXTURE_RECTANGLE_ARB);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0), glVertex2i(p.x-w/2, p.y-h/2);
        glTexCoord2i(w, 0), glVertex2i(p.x+w/2, p.y-h/2);
        glTexCoord2i(w, h), glVertex2i(p.x+w/2, p.y+h/2);
        glTexCoord2i(0, h), glVertex2i(p.x-w/2, p.y+h/2);
        glEnd();
        glDisable(GL_TEXTURE_RECTANGLE_ARB);
    }
}

void ClimatologyOverlayFactory::RenderIsoBars(int setting, PlugIn_ViewPort &vp)
{
recompute:
    if(!m_Settings.Settings[setting].m_bIsoBars)
        return;

    int month = m_bAllTimes ? 12 : m_CurrentTimeline.GetMonth();
    int day = 15; //m_CurrentTimeline.GetDay();
    if(setting == ClimatologyOverlaySettings::SEADEPTH)
        month = 0;

    ClimatologyIsoBarMap *&pIsobars = m_Settings.Settings[setting].m_pIsobars[month];
    double spacing = m_Settings.Settings[setting].m_iIsoBarSpacing, step;
    switch(m_Settings.Settings[setting].m_iIsoBarStep) {
    default: step = 4; break;
    case 1: step = 2; break;
    case 2: step = 1; break;
    case 3: step = .5; break;
    case 4: step = .25; break;
    }

    int units = m_Settings.Settings[setting].m_Units;
    if(pIsobars && !pIsobars->SameSettings(spacing, step, units, month, day)) {
        if(pIsobars->m_bComputing) {
            pIsobars->m_bNeedsRecompute = true;
            return;
        }

        delete pIsobars;
        pIsobars = NULL;
    }

    if( !pIsobars ) {
        pIsobars = new ClimatologyIsoBarMap(m_dlg.m_cfgdlg->SettingName(setting),
                                            spacing, step, *this, setting, units, month, day);
        bool ret = pIsobars->Recompute(&m_dlg);
        if(!ret) {
            if(pIsobars->m_bNeedsRecompute)
                goto recompute;
            pIsobars = NULL;

            m_dlg.m_cfgdlg->DisableIsoBars(setting);
            return;
        }
        goto recompute;
    }

    pIsobars->Plot(m_pdc, vp);
}

void ClimatologyOverlayFactory::RenderNumbers(int setting, PlugIn_ViewPort &vp)
{
    if(!m_Settings.Settings[setting].m_bNumbers)
        return;

    double space = m_Settings.Settings[setting].m_iNumbersSpacing;
    wxPoint p;
    for(p.y = space/2; p.y <= vp.rv_rect.height-space/4; p.y+=space)
        for(p.x = space/2; p.x <= vp.rv_rect.width-space/4; p.x+=space) {
            double lat, lon;
            GetCanvasLLPix( &vp, p, &lat, &lon);
            RenderNumber(p, getCurCalibratedValue(MAG, setting, lat, lon), *wxBLACK);
        }
}

void ClimatologyOverlayFactory::RenderDirectionArrows(int setting, PlugIn_ViewPort &vp)
{
    if(!m_Settings.Settings[setting].m_bDirectionArrows)
        return;

    int month = m_bAllTimes ? 12 : m_CurrentTimeline.GetMonth();

    double step;
    switch(setting) {
    case ClimatologyOverlaySettings::WIND:
        if(!m_WindData[month])
            return;
        step = 360.0 / m_WindData[month]->longitudes;
        break;
    case ClimatologyOverlaySettings::CURRENT:
        if(!m_CurrentData[month])
            return;
        step = 360.0 / m_CurrentData[month]->longitudes;
        break;
    default: return;
    }

    double lengthtype = m_Settings.Settings[setting].m_iDirectionArrowsLengthType;
    int width = m_Settings.Settings[setting].m_iDirectionArrowsWidth;
    wxColour color = m_Settings.Settings[setting].m_cDirectionArrowsColor;
    double size = m_Settings.Settings[setting].m_iDirectionArrowsSize;
    double spacing = m_Settings.Settings[setting].m_iDirectionArrowsSpacing;

    int w = vp.pix_width, h = vp.pix_height;
    while((vp.lat_max - vp.lat_min) / step > h / spacing ||
          (vp.lon_max - vp.lon_min) / step > w / spacing)
        step *= 2;

    for(double lat = round(vp.lat_min/step)*step-1; lat <= vp.lat_max+1; lat+=step)
        for(double lon = round(vp.lon_min/step)*step-1; lon <= vp.lon_max+1; lon+=step) {
            double u = getCurValue(U, setting, lat, lon), v = getCurValue(V, setting, lat, lon);

            // for wind, flip direction to render where the wind is blowing
            if(setting == ClimatologyOverlaySettings::WIND)
               u = -u, v = -v;

            double mag = hypot(u, v);

            double cstep, minv;
            if(setting == ClimatologyOverlaySettings::CURRENT) {
                cstep = .25;
                minv = .25;
            } else {
                cstep = 5;
                minv = 2;

                if(lengthtype) {
                    u /= 15;
                    v /= 15;
                }
            }

            if(!lengthtype)
                u/=mag, v/=mag;
            else if(mag < minv)
                continue;
            
            double t = vp.rotation;
            double x = -size*(u*cos(t) + v*sin(t)), y = size*(v*cos(t) - u*sin(t));
            wxPoint p;
            GetCanvasPixLL( &vp, &p, lat, lon );
            DrawLine(p.x+x, p.y+y, p.x-x, p.y-y, color, width);

            if(!lengthtype) {
                double ix = x, iy = y, dir = 1;
                while(mag > cstep) {
                    DrawLine(p.x+ix, p.y+iy, p.x+ix+x/3+dir*y/2, p.y+iy+y/3-dir*x/2,
                             color, width);
                    dir = -dir;
                    if(dir > 0) {
                        ix = ix*2/3;
                        iy = iy*2/3;
                    }
                    mag -= cstep;
                }
            }

            DrawLine(p.x-x, p.y-y, p.x-x/3+y*2/3, p.y-y/3-x*2/3,
                     color, width);
            DrawLine(p.x-x, p.y-y, p.x-x/3-y*2/3, p.y-y/3+x*2/3,
                     color, width);
        }
}

void ClimatologyOverlayFactory::RenderWindAtlas(PlugIn_ViewPort &vp)
{
    if(!m_dlg.m_cfgdlg->m_cbWindAtlasEnable->GetValue())
        return;

    int month, nmonth;
    double dpos;

    GetDateInterpolation(NULL, month, nmonth, dpos);

    if(!m_WindData[month] || !m_WindData[nmonth])
        return;

    double step = 360.0 / (m_WindData[month]->longitudes);
    const double r = 12;
    double size = m_dlg.m_cfgdlg->m_sWindAtlasSize->GetValue();
    double spacing = m_dlg.m_cfgdlg->m_sWindAtlasSpacing->GetValue();
    int w = vp.pix_width, h = vp.pix_height;
    while((vp.lat_max - vp.lat_min) / step > h / spacing ||
          (vp.lon_max - vp.lon_min) / step > w / spacing)
        step *= 2;

    int dir_cnt = m_WindData[month]->dir_cnt;
    for(double lat = round(vp.lat_min/step)*step-1; lat <= vp.lat_max+1; lat+=step)
        for(double lon = round(vp.lon_min/step)*step-1; lon <= vp.lon_max+1; lon+=step) {
            double directions[64], speeds[64], storm, calm;
            if(!InterpolateWindAtlasTime(month, nmonth, dpos, lat, lon, directions, speeds, storm, calm))
                continue;

            wxPoint p;
            GetCanvasPixLL(&vp, &p, lat, lon);

            int opacity = m_dlg.m_cfgdlg->m_sWindAtlasOpacity->GetValue();

            if(storm*2 > calm)
                RenderNumber(p, 100.0*storm, wxColour(255, 0, 0, opacity));
            else if(calm > 0)
                RenderNumber(p, 100.0*calm, wxColour(0, 0, 180, opacity));
            
            wxColour c(0, 0, 0, opacity);
            DrawCircle(p.x, p.y, r, c, 2);

            for(int d = 0; d<dir_cnt; d++) {
                double theta = 2*M_PI*d/dir_cnt + vp.rotation; 
                double x1 = p.x+r*sin(theta), y1 = p.y-r*cos(theta);
                
                if(directions[d] == 0)
                    continue;

                const double maxdirection = .35;
                bool split = directions[d] >= maxdirection;
                double u = r + size*(split ? maxdirection : directions[d]);
                double x2 = p.x+u*sin(theta), y2 = p.y-u*cos(theta);

                if(split) {
                    wxPoint q((x1 + x2)/2, (y1 + y2)/2);
                    RenderNumber(q, 100*directions[d], c);

                    DrawLine(x1, y1, (3*x1+x2)/4, (3*y1+y2)/4, c, 2);
                    DrawLine((x1+3*x2)/4, (y1+3*y2)/4, x2, y2, c, 2);
                } else
                    DrawLine(x1, y1, x2, y2, c, 2);

                /* draw wind barbs */
                double cur_speed = speeds[d];
                double dir = 1;
                const double a = 10, b = M_PI*2/3;
                while(cur_speed > 2) {
                    DrawLine(x2, y2, x2-a*sin(theta+dir*b), y2+a*cos(theta+dir*b), c, 2);
                    dir = -dir;
                    if(dir > 0)
                        x2 -= 3*sin(theta), y2 += 3*cos(theta);
                    cur_speed -= 5;
                }
            }
        }
}

void ClimatologyOverlayFactory::RenderCycloneSegment(CycloneState &ss, PlugIn_ViewPort &vp,
                                                     int dayspan)
{
    if(ss.drawn_counter == m_cyclone_drawn_counter)
        return;

    ss.drawn_counter = m_cyclone_drawn_counter;
                    
    if(!m_dlg.m_cbAll->GetValue()) {
        int daydiff = abs(ss.datetime.day - m_CurrentTimeline.GetDay()
                          + 30.42*(ss.datetime.month - m_CurrentTimeline.GetMonth()));
        if(daydiff > 183)
            daydiff = 365 - daydiff;
        if(daydiff > dayspan/2)
            return;
    }

    double *lat = ss.lat, *lon = ss.lon;
#if 0
    /* prevent wrong crossover */
    if((lastlon+180 > vp.clon || lon+180 < vp.clon) &&
       (lastlon+180 < vp.clon || lon+180 > vp.clon) &&
       (lastlon-180 > vp.clon || lon-180 < vp.clon) &&
       (lastlon-180 < vp.clon || lon-180 > vp.clon))
#endif
    {
        wxPoint p[2];
        GetCanvasPixLL( &vp, &p[0], lat[0], lon[0] );
        GetCanvasPixLL( &vp, &p[1], lat[1], lon[1] );

        wxColour c = GetGraphicColor(CYCLONE_SETTING, ss.windknots);
                            
        DrawLine(p[0].x, p[0].y, p[1].x, p[1].y, c, 2);
                            
        /* direction arrow */
        wxPoint a((p[0].x+p[1].x)/2, (p[0].y+p[1].y)/2);
        wxPoint d(p[0].x-p[1].x, p[0].y-p[1].y);
                            
        DrawLine(a.x, a.y, a.x + (d.x+d.y)/5, a.y + (d.y-d.x)/5, c, 2);
        DrawLine(a.x, a.y, a.x + (d.x-d.y)/5, a.y + (d.x+d.y)/5, c, 2);
    }
}

void ClimatologyOverlayFactory::RenderCyclones(PlugIn_ViewPort &vp)
{
//    if(m_pdc) /* display list optimization not possible */
//        m_bUpdateCyclones = true;

    /* no cyclones ever existed between 10 and 20 longitude
       so use 15 east as the meridian to split the world on.. */
//#define USE_DL
#ifdef USE_DL
    PlugIn_ViewPort nvp = vp;
    double cclon = 15;
    static const double NORM_FACTOR = 16;

    if(!m_cyclonesDisplayList)
        m_cyclonesDisplayList = glGenLists(1);

    if(!m_pdc) {
        glPushMatrix();

        wxPoint point;
        GetCanvasPixLL(&vp, &point, 0, cclon - 180);
        glTranslatef(point.x, point.y, 0);
        glScalef(vp.view_scale_ppm/NORM_FACTOR, vp.view_scale_ppm/NORM_FACTOR, 1);
        glRotated(vp.rotation*180/M_PI, 0, 0, 1);
    }
    
    if(!m_bUpdateCyclones)
        glCallList(m_cyclonesDisplayList);
    else {
        if(!m_pdc) {
            m_bUpdateCyclones = false;
        
            nvp.clat = 0;
            nvp.clon = cclon - 180;
            nvp.view_scale_ppm = NORM_FACTOR;
            nvp.rotation = nvp.skew = 0;
    
            if(!m_cyclonesDisplayList)
                m_cyclonesDisplayList = glGenLists(1);
            glNewList(m_cyclonesDisplayList, GL_COMPILE_AND_EXECUTE);
        }
#endif

    int dayspan = m_dlg.m_cfgdlg->m_sCycloneDaySpan->GetValue();

    int month_start, month_end;
    if(m_dlg.m_cbAll->GetValue()) {
        month_start = 0;
        month_end = 11;
    } else {
        wxDateTime dt2(m_CurrentTimeline.GetDay(), m_CurrentTimeline.GetMonth(), 1999);
        wxTimeSpan ts_dayspan = wxTimeSpan::Days(dayspan/2);
        month_start = (dt2 - ts_dayspan).GetMonth();
        month_end = (dt2 + ts_dayspan).GetMonth();
    }

    m_cyclone_drawn_counter++;

    wxDateTime start = wxDateTime::Now();
    for(int lati = floor(vp.lat_min); lati <= ceil(vp.lat_max); lati++)
        for(int loni = floor(vp.lon_min); loni <= ceil(vp.lon_max); loni++) {
            int monthi = month_start;
            for(;;) {
                int lonin = loni < 15 ? loni : loni - 360;
                int hash = (lonin * 180 + lati)*12 + monthi;

                std::list<CycloneState*> &states = m_cyclone_cache[hash];
                for(std::list<CycloneState*>::iterator it = states.begin();
                    it != states.end(); it++)
                    RenderCycloneSegment(**it, vp, dayspan);

                if(monthi == month_end)
                    break;
                if(++monthi == 12) monthi = 0;
            }
        }

        wxDateTime end = wxDateTime::Now();

        /* rendering is taking too long, and display lists not used */
        if(m_pdc && (end - start).GetMilliseconds() >= 1200) {
            m_dlg.m_cbCyclones->SetValue(false);

            wxMessageDialog mdlg(&m_dlg, _("Computer too slow to render cyclones, disabling theater"),
                             _("Climatology"), wxOK | wxICON_WARNING);
            mdlg.ShowModal();
        }

#ifdef USE_DL
        if(!m_pdc)
            glEndList();
    }

    if(!m_pdc) {
        //  Does current vp cross cclon ?
        // if so, call the display list again translated
        // to the other side of it..

        if( vp.lon_min < cclon && vp.lon_max > cclon ) {
            double ts = 40058986*NORM_FACTOR; /* 360 degrees in normalized viewport */

            glPushMatrix();
            if( vp.clon > cclon )
                glTranslated(-ts, 0, 0);
            else
                glTranslated(ts, 0, 0);
            glCallList(m_cyclonesDisplayList);
            glPopMatrix();
        }
        glPopMatrix();
    }
#endif
}

bool ClimatologyOverlayFactory::RenderOverlay( wxDC *dc, PlugIn_ViewPort &vp )
{
    m_pdc = dc;

    if(!dc) {
        glPushAttrib( GL_LINE_BIT | GL_ENABLE_BIT | GL_HINT_BIT ); //Save state

        //      Enable anti-aliased lines, at best quality
        glEnable( GL_LINE_SMOOTH );
        glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    for(int overlay = 1; overlay >= 0; overlay--)
    for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++) {
        if((i == ClimatologyOverlaySettings::WIND           && !m_dlg.m_cbWind->GetValue()) ||
           (i == ClimatologyOverlaySettings::CURRENT        && !m_dlg.m_cbCurrent->GetValue()) ||
           (i == ClimatologyOverlaySettings::SLP            && !m_dlg.m_cbPressure->GetValue()) ||
           (i == ClimatologyOverlaySettings::SST            && !m_dlg.m_cbSeaTemperature->GetValue()) ||
           (i == ClimatologyOverlaySettings::AT             && !m_dlg.m_cbAirTemperature->GetValue()) ||
           (i == ClimatologyOverlaySettings::CLOUD          && !m_dlg.m_cbCloudCover->GetValue()) ||
           (i == ClimatologyOverlaySettings::PRECIPITATION  && !m_dlg.m_cbPrecipitation->GetValue()) ||
           (i == ClimatologyOverlaySettings::RELATIVE_HUMIDITY  && !m_dlg.m_cbRelativeHumidity->GetValue()) ||
           (i == ClimatologyOverlaySettings::LIGHTNING      && !m_dlg.m_cbLightning->GetValue()) ||
           (i == ClimatologyOverlaySettings::SEADEPTH       && !m_dlg.m_cbSeaDepth->GetValue()))
            continue;

        if(!m_Settings.Settings[i].m_bEnabled)
            continue;

        if(overlay) /* render overlays first */
            RenderOverlayMap( i, vp );
        else {
            RenderIsoBars(i, vp);
            RenderNumbers(i, vp);
            RenderDirectionArrows(i, vp);
        }
    }

    if(m_dlg.m_cbWind->GetValue())
        RenderWindAtlas(vp);

    if(m_dlg.m_cbCyclones->GetValue())
        RenderCyclones(vp);

    if(!dc)
        glPopAttrib();

    return true;
}
