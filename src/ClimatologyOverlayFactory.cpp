/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Climatology Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/progdlg.h>

#include <netcdfcpp.h>

#include "climatology_pi.h"
#include "ClimatologyDialog.h"
#include "ClimatologyConfigDialog.h"
#include "IsoLine.h"

ClimatologyOverlay::~ClimatologyOverlay()
{
    if(m_iTexture)
        glDeleteTextures( 1, &m_iTexture );
    delete m_pDCBitmap, delete[] m_pRGBA;
}

//----------------------------------------------------------------------------------------------------------
//    Climatology Overlay Factory Implementation
//----------------------------------------------------------------------------------------------------------
ClimatologyOverlayFactory::ClimatologyOverlayFactory( ClimatologyDialog &dlg )
    : m_dlg(dlg), m_Settings(dlg.m_OverlaySettings), m_cyclonelist(0)
{
    ClearCachedData();

    wxString path = *GetpSharedDataLocation() + _T("plugins/climatology/data/");

    /* load sea level pressure data */
    wxString slp_path = path + _T("slpcoadsclim5079.nc");

    NcError err(NcError::silent_nonfatal);
    NcFile slp(slp_path.mb_str(), NcFile::ReadOnly);
    if(!slp.is_valid() || slp.num_dims() != 3 || slp.num_vars() != 4)
        wxLogMessage(_("climatology_pi: failed to read file: ") + slp_path);
    else {
        NcVar* data = slp.get_var("data");
        if(!data->is_valid() || data->num_dims() != 3)
            wxLogMessage(_("climatology_pi: file, ") + slp_path + _(" Has incorrect dimensions"));
        else {
            m_dlg.m_cbPressure->Enable();
            data->get(&m_slp[0][0][0], 13, 90, 180);
        }
    }

    /* load sea temperature data */
    wxString sst_path = path + _T("sstcoadsclim6079.1deg.nc");

    NcFile sst(sst_path.mb_str(), NcFile::ReadOnly);
    if(!sst.is_valid() || sst.num_dims() != 3 || sst.num_vars() != 4)
        wxLogMessage(_("climatology_pi: failed to read file: ") + sst_path);
    else {
        NcVar* data = sst.get_var("data");
        if(!data->is_valid() || data->num_dims() != 3)
            wxLogMessage(_("climatology_pi: file, ") + slp_path + _(" Has incorrect dimensions"));
        else {
            m_dlg.m_cbSeaTemperature->Enable();
            data->get(&m_sst[0][0][0], 13, 180, 360);
        }
    }

    /* load cyclone tracks */
    if(!ReadCycloneDatabase(path + _T("tracks.epa"), m_epa))
        m_dlg.m_cfgdlg->m_cbEastPacific->Disable();
    if(!ReadCycloneDatabase(path + _T("tracks.bwp"), m_bwp))
        m_dlg.m_cfgdlg->m_cbWestPacific->Disable();
    if(!ReadCycloneData(path + _T("tracks.spa.dat"), m_spa))
        m_dlg.m_cfgdlg->m_cbSouthPacific->Disable();
    if(!ReadCycloneDatabase(path + _T("tracks.atl"), m_atl))
        m_dlg.m_cfgdlg->m_cbAtlantic->Disable();
    if(!ReadCycloneDatabase(path + _T("tracks.nio"), m_nio))
        m_dlg.m_cfgdlg->m_cbNorthIndian->Disable();
    if(!ReadCycloneDatabase(path + _T("tracks.she"), m_she, true))
        m_dlg.m_cfgdlg->m_cbSouthIndian->Disable();

    m_dlg.m_cbCyclones->Enable();
}

ClimatologyOverlayFactory::~ClimatologyOverlayFactory()
{
    glDeleteLists(m_cyclonelist, 1);
    ClearCachedData();
}


bool ClimatologyOverlayFactory::ReadCycloneDatabase(
    wxString filename, std::list<Cyclone*> &cyclones, bool south)
{
    FILE *f = fopen(filename.mb_str(), "r");
    if (!f) {
        wxLogMessage(_T("climatology_pi: failed to open file: ") + filename);
        return false;
    }

    char line[128];
    while(fgets(line, sizeof line, f)) {
        char *year  = line+12; line[16] = 0;
        char *days  = line+19; line[21] = 0;
//        char *synum = line+22; line[24] = 0; /* cyclone this year */
//        char *snum  = line+30; line[34] = 0; /* cyclone number in total */
//        char *sname = line+35; line[46] = 0;
        
        long lyear = strtol(year, 0, 10);
        long ldays = strtol(days, 0, 10);

        Cyclone *cyclone = new Cyclone;

        /* daily data */
        for(int i=0; i<ldays; i++) {
            if(!fgets(line, sizeof line, f)) {
                wxLogMessage(_T("climatology_pi: cyclone text file ended before it should"));
                fclose(f);
                return false;
            }

            char month[3] = {line[6], line[7], 0};
            char day[3] =   {line[9], line[10], 0};

            int lmonth = strtol(month, 0, 10);
            int lday = strtol(day, 0, 10);

            char *s = line + 11;
            for(int h = 0; h<4; h++) {
                char state = s[0];
                char lat[5] = {s[1], s[2], '.', s[3], 0};
                char lon[6] = {s[4], s[5], s[6], '.', s[7], 0};
                char *wk = s+9; s[12] = 0;
                char press[5] = {s[13], s[14], s[15], s[15], 0};

                CycloneState::State cyclonestate;
                switch(state) {
                case '*': cyclonestate = CycloneState::TROPICAL; break;
                case 'S': cyclonestate = CycloneState::SUBTROPICAL; break;
                case 'E': cyclonestate = CycloneState::EXTRATROPICAL; break;
                case 'W': cyclonestate = CycloneState::WAVE; break;
                case 'L': cyclonestate = CycloneState::REMANENT; break;
                default: cyclonestate = CycloneState::UNKNOWN; break;
                }

                double dlat = (south ? -1 : 1) * strtod(lat, 0), dlon = -strtod(lon, 0);
                if(dlat || dlon) {
                    wxDateTime datetime;
                    datetime.SetDay(lday);
                    datetime.SetMonth((wxDateTime::Month)(lmonth-1));
                    datetime.SetYear(lyear);
                    datetime.SetHour(h*6);
                    cyclone->states.push_back
                        (new CycloneState(cyclonestate, datetime,
                                        dlat, dlon, strtod(wk, 0), strtod(press, 0)));
                }

                s += 17;
            }
        }

        /* trailer */
        if(!fgets(line, sizeof line, f)) {
            wxLogMessage(_T("climatology_pi: cyclone text file ended before it should"));
            fclose(f);
            return false;
        }

        if(line[6] == 'H' && line[7] == 'R')
            cyclone->type = Cyclone::HURRICANE;
        else
        if(line[6] == 'T' && line[7] == 'S')
            cyclone->type = Cyclone::TROPICAL_CYCLONE;
        else
        if(line[6] == 'S' && line[7] == 'S')
            cyclone->type = Cyclone::SUBTROPICAL_CYCLONE;
        else
            cyclone->type = Cyclone::UNKNOWN;
        
        cyclones.push_back(cyclone);
    }

    fclose(f);
    return true;
}

bool ClimatologyOverlayFactory::ReadCycloneData(wxString filename, std::list<Cyclone*> &cyclones)
{
    FILE *f = fopen(filename.mb_str(), "r");
    if (!f) {
        wxLogMessage(_T("climatology_pi: failed to open file: ") + filename);
        return false;
    }

    char line[1024];
    int year;
    wxDateTime datetime;
    Cyclone *cyclone = NULL;
    int header = 0;
    while(fgets(line, sizeof line, f)) {
        if(header) {
            header--;
        } else
        if(!strncmp(line, "Date", 4)) {
            wxString s = wxString::FromUTF8(line+6);
            char *saveptr;
            strtok_r(line, " ", &saveptr);
            strtok_r(0, " ", &saveptr);
            strtok_r(0, " ", &saveptr);
            year = strtol(strtok_r(0, " ", &saveptr), 0, 10);
            datetime.SetMonth((wxDateTime::Month)0);

            if(cyclone)
                cyclones.push_back(cyclone);
            cyclone = new Cyclone;
            header = 2;
        } else if(cyclone) {
            char *saveptr;
            strtok_r(line, " ", &saveptr);
            double lat = strtod(strtok_r(0, " ", &saveptr), 0);
            double lon = strtod(strtok_r(0, " ", &saveptr), 0);
            int month = strtol(strtok_r(0, "/", &saveptr), 0, 10);
            int day = strtol(strtok_r(0, "/", &saveptr), 0, 10);
            int hour = strtol(strtok_r(0, " ", &saveptr), 0, 10);
            double wind = strtod(strtok_r(0, " ", &saveptr), 0);
            double pressure = strtod(strtok_r(0, " ", &saveptr), 0);
            char *stat = strtok_r(0, "\n", &saveptr);

            if(month < datetime.GetMonth())
                year++;

            datetime.SetYear(year);
            datetime.SetMonth((wxDateTime::Month)(month-1));
            datetime.SetDay(day);
            datetime.SetHour(hour);

            CycloneState::State s;
            if(!strncmp(stat, "TROPICAL DEPRESSION", 19))
                s = CycloneState::REMANENT;
            else
                s = CycloneState::TROPICAL;

            cyclone->states.push_back(new CycloneState(s, datetime, lat, lon, wind, pressure));
        }
    }

    if(cyclone)
        cyclones.push_back(cyclone);
    fclose(f);
    return true;
}

void ClimatologyOverlayFactory::ClearCachedData()
{
    for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++) {
        delete m_Settings.Settings[i].m_pIsobarArray;
        m_Settings.Settings[i].m_pIsobarArray = NULL;
    }
}

void ClimatologyOverlayFactory::DrawGLLine( double x1, double y1, double x2, double y2, double width )
{
    {
        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_ENABLE_BIT |
                     GL_POLYGON_BIT | GL_HINT_BIT ); //Save state
        {

            //      Enable anti-aliased lines, at best quality
            glEnable( GL_LINE_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
            glLineWidth( width );

            glBegin( GL_LINES );
            glVertex2d( x1, y1 );
            glVertex2d( x2, y2 );
            glEnd();
        }

        glPopAttrib();
    }
}

/* return cached wxImage for a given number, or create it if not in the cache */
wxImage &ClimatologyOverlayFactory::getLabel(double value)
{
    std::map <double, wxImage >::iterator it;
    it = m_labelCache.find(value);
    if (it != m_labelCache.end())
        return m_labelCache[value];

    wxString labels;
    labels.Printf(_T("%d"), (int)(value+0.5));

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
//    mdc.DrawRoundedRectangle(xd, yd, w+(label_offset * 2), h+2, -.25);
    mdc.DrawRectangle(xd, yd, w+(label_offset * 2), h+2);
    mdc.DrawText(labels, label_offset + xd, yd+1);
          
    mdc.SelectObject(wxNullBitmap);

    m_labelCache[value] = bm.ConvertToImage();

    m_labelCache[value].InitAlpha();

    wxImage &image = m_labelCache[value];

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

    return m_labelCache[value];
}

/* give value for y at a given x location on a segment */
double interp_value(double x, double x1, double x2, double y1, double y2)
{
    return x2 - x1 ? (y2 - y1)*(x - x1)/(x2 - x1) + y1 : y1;
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
    double v00 = ArrayValue(a, x0*h + y0), v01 = ArrayValue(a, x0*h + y1);
    double v10 = ArrayValue(a, x1*h + y0), v11 = ArrayValue(a, x1*h + y1);

    double v0 = interp_value(x, x0, x1, v00, v01);
    double v1 = interp_value(x, x0, x1, v10, v11);
    return      interp_value(y, y0, y1, v0,  v1 );
}

double ClimatologyOverlayFactory::getValue(int setting, double lat, double lon)
{
    switch(setting) {
    case ClimatologyOverlaySettings::SLP:
        return InterpArray((-lat+90)/2, lon/2, m_slp[m_CurrentMonth][0], 180) * .01f + 1000.0;
    case ClimatologyOverlaySettings::SST:
        return InterpArray((-lat+90), lon, m_sst[m_CurrentMonth][0], 360) * .001f + 15.0;
    default: return NAN;
    }
}

double ClimatologyOverlayFactory::GetMin(int setting)
{
    switch(setting) {
    case ClimatologyOverlaySettings::SLP: return 880;
    case ClimatologyOverlaySettings::SST: return -2;
    default: return NAN;
    }
}

double ClimatologyOverlayFactory::GetMax(int setting)
{
    switch(setting) {
    case ClimatologyOverlaySettings::SLP: return 1040;
    case ClimatologyOverlaySettings::SST: return 35;
    default: return NAN;
    }
}

void ClimatologyOverlayFactory::RenderIsoBars(int setting, PlugIn_ViewPort &vp)
{
    if(!m_Settings.Settings[setting].m_bNumbers)
        return;

    wxArrayPtrVoid *&pIsobarArray = m_Settings.Settings[setting].m_pIsobarArray;
    if( !pIsobarArray ) {
        pIsobarArray = new wxArrayPtrVoid;
        IsoLine *piso;

        wxProgressDialog *progressdialog = NULL;
        wxDateTime start = wxDateTime::Now();

        double min = GetMin(setting);
        double max = GetMax(setting);

        /* convert min and max to units being used */
        for( double press = min; press <= max; press += m_Settings.Settings[setting].m_iIsoBarSpacing) {
            if(progressdialog)
                progressdialog->Update(press-min);
            else {
                wxDateTime now = wxDateTime::Now();
                if((now-start).GetSeconds() > 3 && press-min < (max-min)/2) {
                    progressdialog = new wxProgressDialog(
                        _("Building Isobar map"), _("Wind"), max-min+1, NULL,
                        wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
                }
            }

            piso = new IsoLine( press, *this, setting);
            pIsobarArray->Add( piso );
        }
        delete progressdialog;
    }

    //    Draw the Isobars
    for( unsigned int i = 0; i < pIsobarArray->GetCount(); i++ ) {
        IsoLine *piso = (IsoLine *) pIsobarArray->Item( i );
        piso->drawIsoLine( this, m_pdc, &vp, true);

        // Draw Isobar labels
        int density = 40;
        int first = 0;
        piso->drawIsoLineLabels( this, m_pdc, &vp, density,
                                 first, getLabel(piso->getValue()) );
    }
}

void ClimatologyOverlayFactory::RenderNumbers(int setting, PlugIn_ViewPort &vp)
{
    if(!m_Settings.Settings[setting].m_bNumbers)
        return;

    double space = m_Settings.Settings[setting].m_iNumbersSpacing;
    for(double lat = vp.lat_min; lat <= vp.lat_max; lat+=space)
        for(double lon = vp.lon_max; lon <= vp.lon_max; lon+=space) {
            wxPoint p;
            GetCanvasPixLL( &vp, &p, lat, lon );
            double value = getValue(setting, lat, lon);
            wxImage &label = getLabel(value);

            if( m_pdc )
                m_pdc->DrawBitmap(label, p.x, p.y, true);
            else {
                int w = label.GetWidth(), h = label.GetHeight();
                glEnable( GL_BLEND );
                glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                glColor4f(0, 0, 0, 1);
                glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
                glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
                glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA,
                             w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, label.GetData());
                glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, w, h,
                                                GL_ALPHA, GL_UNSIGNED_BYTE, label.GetAlpha());

                glEnable(GL_TEXTURE_RECTANGLE_ARB);
                glBegin(GL_QUADS);
                glTexCoord2i(0, 0), glVertex2i(p.x,   p.y);
                glTexCoord2i(w, 0), glVertex2i(p.x+w, p.y);
                glTexCoord2i(w, h), glVertex2i(p.x+w, p.y+h);
                glTexCoord2i(0, h), glVertex2i(p.x,   p.y+h);
                glEnd();
                glDisable(GL_TEXTURE_RECTANGLE_ARB);
             }
        }
}

void ClimatologyOverlayFactory::RenderCyclonesTheatre(PlugIn_ViewPort &vp, std::list<Cyclone*> &cyclones)
{
    for(std::list<Cyclone*>::iterator it = cyclones.begin(); it != cyclones.end(); it++) {
        Cyclone *s = *it;

        bool first = true;
        double lastlon;
        wxPoint lastp;

        for(std::list<CycloneState*>::iterator it2 = s->states.begin(); it2 != s->states.end(); it2++) {
            wxPoint p;
            CycloneState *ss = *it2;

            if(m_dlg.m_sMonth->GetValue() != 12 &&
               m_dlg.m_sMonth->GetValue() != ss->datetime.GetMonth())
                continue;

            if((ss->datetime < m_dlg.m_cfgdlg->m_dPStart->GetValue()) ||
               (ss->datetime > m_dlg.m_cfgdlg->m_dPEnd->GetValue()))
                continue;

            switch(ss->state) {
            case CycloneState::TROPICAL: if(m_dlg.m_cfgdlg->m_cbTropical->GetValue()) break;
            case CycloneState::SUBTROPICAL: if(m_dlg.m_cfgdlg->m_cbSubTropical->GetValue()) break;
            case CycloneState::EXTRATROPICAL: if(m_dlg.m_cfgdlg->m_cbExtraTropical->GetValue()) break;
            case CycloneState::REMANENT: if(m_dlg.m_cfgdlg->m_cbRemanent->GetValue()) break;
            default: continue;
            }

            if(ss->windknots < m_dlg.m_cfgdlg->m_sMinWindSpeed->GetValue())
                continue;

            if(ss->pressure > m_dlg.m_cfgdlg->m_sMaxPressure->GetValue())
                continue;

            double lat = ss->latitude, lon = ss->longitude;
            GetCanvasPixLL( &vp, &p, lat, lon );

            if(first) {
                first = false;
            } else {

#if 1 /* if you want to zoom in really really far and still see tracks */
                if((lastlon+180 > vp.clon || lon+180 < vp.clon) &&
                   (lastlon+180 < vp.clon || lon+180 > vp.clon) &&
                   (lastlon-180 > vp.clon || lon-180 < vp.clon) &&
                   (lastlon-180 < vp.clon || lon-180 > vp.clon))
#else
                if(abs(lastp.x-p.x) < vp.pix_width)
#endif
                {

                    double intense =  (ss->windknots / 150);

                    if(intense > 1) intense = 1;
                    switch(ss->state) {
                    case CycloneState::TROPICAL:      glColor3d(intense, 0, 0); break;
                    case CycloneState::SUBTROPICAL:   glColor3d(intense/2, intense/2, 0); break;
                    case CycloneState::EXTRATROPICAL: glColor3d(0, intense, 0); break;
                    default:                        glColor3d(0, 0, intense); break;
                    }
                    
                    glVertex2i(lastp.x, lastp.y);
                    glVertex2i(p.x, p.y);
                }
            }

            lastlon = lon;
            lastp = p;
        }
    }
}

void ClimatologyOverlayFactory::RenderCyclones(PlugIn_ViewPort &vp)
{
#if 0
    if(!m_cyclonelist)
        m_cyclonelist = glGenLists(1);

    if(m_cyclonelistok) {
        glCallList(m_cyclonelist);
        return;
    }

    glNewList(m_cyclonelist, GL_COMPILE_AND_EXECUTE);
#endif

    glLineWidth(1);

    glBegin(GL_LINES);

    if(m_dlg.m_cfgdlg->m_cbEastPacific->GetValue())
        RenderCyclonesTheatre(vp, m_epa);
    if(m_dlg.m_cfgdlg->m_cbWestPacific->GetValue())
        RenderCyclonesTheatre(vp, m_bwp);
    if(m_dlg.m_cfgdlg->m_cbSouthPacific->GetValue())
        RenderCyclonesTheatre(vp, m_spa);
    if(m_dlg.m_cfgdlg->m_cbAtlantic->GetValue())
        RenderCyclonesTheatre(vp, m_atl);
    if(m_dlg.m_cfgdlg->m_cbNorthIndian->GetValue())
        RenderCyclonesTheatre(vp, m_nio);
    if(m_dlg.m_cfgdlg->m_cbSouthIndian->GetValue())
        RenderCyclonesTheatre(vp, m_she);

    glEnd();
    
#if 0
    glEndList();
    m_cyclonelistok = true;
#endif
}

bool ClimatologyOverlayFactory::RenderOverlay( wxDC &dc, PlugIn_ViewPort &vp )
{
    m_pdc = &dc;
    return true;
}

bool ClimatologyOverlayFactory::RenderGLOverlay( wxGLContext *pcontext, PlugIn_ViewPort &vp )
{
    m_pdc = NULL;

    for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++) {
        if((i == ClimatologyOverlaySettings::SLP && !m_dlg.m_cbPressure->GetValue()) ||
           (i == ClimatologyOverlaySettings::SST && !m_dlg.m_cbSeaTemperature->GetValue()) ||
           (i == ClimatologyOverlaySettings::CYCLONES && !m_dlg.m_cbCyclones->GetValue()))
            continue;

        RenderIsoBars(i, vp);
        RenderNumbers(i, vp);
        RenderCyclones(vp);
    }
    return true;
}
