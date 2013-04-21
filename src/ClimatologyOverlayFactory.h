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
 */

#include <list>
#include <map>

void DrawGLLine( double x1, double y1, double x2, double y2, double width );

class PlugIn_ViewPort;

struct WindData
{
    struct WindPolar
    {
        WindPolar() : directions(NULL), speeds(NULL) {}
        ~WindPolar() { delete [] directions; delete [] speeds; }
        wxUint8 storm, calm, *directions, *speeds;

        double AvgSpeed(int dir_cnt) {
            if(storm == 255)
                return NAN;
            int totald = 0, totals = 0;
            for(int i=0; i<dir_cnt; i++) {
                totald += directions[i];
                totals += speeds[i];
            }
            return (double)totals / totald * 3.6 / 1.852; /* knots */
        }
    };

    WindData(int lats, int lons, int dirs)
    : latitudes(lats), longitudes(lons), dir_cnt(dirs), data(new WindPolar[lats*lons]) {}

    double InterpWindSpeed(double lat, double lon);
    WindPolar &GetPolar(double lat, double lon) {
        int lati = round(latitudes*(.5+lat/180.0));
        int loni = round(longitudes*lon/360.0);
        return data[lati*longitudes + loni];
    }

    int latitudes, longitudes, dir_cnt;
    WindPolar *data;
};

struct ElNinoYear
{
    double months[12];
};

class CycloneState
{
public:
    enum State {TROPICAL, SUBTROPICAL, EXTRATROPICAL, WAVE, REMANENT, UNKNOWN};
    CycloneState(State s, wxDateTime dt, double lat, double lon, double wk, double press)
    : state(s), datetime(dt), latitude(lat), longitude(lon), windknots(wk), pressure(press)
    {}

    State state;
    wxDateTime datetime;
    double latitude, longitude, windknots, pressure;
};

class Cyclone
{
public:
    Cyclone() { }

//    int stormindex, stormindex_year;
//    wxString name;

    std::list<CycloneState*> states;

    enum CycloneType {HURRICANE, TROPICAL_CYCLONE, SUBTROPICAL_CYCLONE, UNKNOWN} type;
};

//----------------------------------------------------------------------------------------------------------
//    Climatology Overlay Specification
//----------------------------------------------------------------------------------------------------------

class ClimatologyOverlay {
public:
    ClimatologyOverlay( void )
    {
        m_iTexture = 0;
        m_pDCBitmap = NULL, m_pRGBA = NULL;
    }

    ~ClimatologyOverlay( void );

    unsigned int m_iTexture; /* opengl mode */

    wxBitmap *m_pDCBitmap; /* dc mode */
    unsigned char *m_pRGBA;

    int m_width;
    int m_height;
};

//----------------------------------------------------------------------------------------------------------
//    Climatology Overlay Factory Specification
//----------------------------------------------------------------------------------------------------------

class ClimatologyDialog;
class wxGLContext;

class ClimatologyOverlayFactory {
public:
    ClimatologyOverlayFactory( ClimatologyDialog &dlg );
    ~ClimatologyOverlayFactory();

    void ReadWindData(int month, wxString filename);
    bool ReadCycloneDatabase(wxString filename, std::list<Cyclone*> &cyclones, bool south=false);
    bool ReadCycloneData(wxString filename, std::list<Cyclone*> &cyclones);
    bool ReadElNinoYears(wxString filename);

    void ClearCachedData();

    wxImage &getLabel(double value);

    double GetMin(int setting);
    double GetMax(int setting);
    double getValue(int setting, double lat, double lon);

    bool RenderOverlay( wxDC &dc, PlugIn_ViewPort &vp );
    bool RenderGLOverlay( wxGLContext *pcontext, PlugIn_ViewPort &vp );

    int m_CurrentMonth;

private:
    void RenderNumber(wxPoint p, double v);

    void RenderIsoBars(int setting, PlugIn_ViewPort &vp);
    void RenderNumbers(int setting, PlugIn_ViewPort &vp);

    void RenderWindAtlas(PlugIn_ViewPort &vp);
    void RenderCyclonesTheatre(PlugIn_ViewPort &vp, std::list<Cyclone*> &cyclones);
    void RenderCyclones(PlugIn_ViewPort &vp);

    wxColour GetGraphicColor(int setting, double val_in);
    bool CreateGLTexture(ClimatologyOverlay &O, int setting, PlugIn_ViewPort &vp);
    void DrawGLTexture(ClimatologyOverlay &O, PlugIn_ViewPort &vp);

    void RenderOverlayMap( int setting, PlugIn_ViewPort &vp);

    ClimatologyDialog &m_dlg;
    ClimatologyOverlaySettings &m_Settings;

    ClimatologyOverlay m_pOverlay[13][ClimatologyOverlaySettings::SETTINGS_COUNT];

    wxDC *m_pdc;
    wxGraphicsContext *m_gdc;

    std::map < double , wxImage > m_labelCache;

    WindData *m_WindData[13];

    wxInt16 m_slp[13][90][180]; /* 12 months + year total and average at 2 degree intervals */

    wxInt16 m_sst[13][180][360]; /* 12 months + year total and average at 1 degree intervals */

    unsigned int m_cyclonelist; /* for opengl display list */
    bool m_cyclonelistok;

    std::list<Cyclone*> m_bwp, m_epa, m_spa, m_atl, m_she, m_nio;

    std::map<int, ElNinoYear> m_ElNinoYears;
};
