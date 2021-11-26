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
 */

#include <list>
#include <map>

#include "zuFile.h"

#include "IsoBarMap.h"
#include "pidc.h"

class PlugIn_ViewPort;
enum Coord {U, V, MAG, DIRECTION};

struct WindData
{
    struct WindPolar
    {
        WindPolar()  {}
        ~WindPolar() { /*delete [] directions; delete [] speeds;*/ }
        wxUint8 gale, calm, directions[8] /*{}*/, speeds[8] /*{}*/;
        double Value(enum Coord coord, int dir_cnt);
    };

    WindData(int lats, int lons, int dirs, float dir_res, float spd_mul)
    : latitudes(lats), longitudes(lons), dir_cnt(dirs),
        direction_resolution(dir_res), speed_multiplier(spd_mul),
        data(new WindPolar[lats*lons]/*()*/) {}
    ~WindData() { delete [] data; }

    double InterpWind(enum Coord coord, double lat, double lon);
    WindPolar *GetPolar(double lat, double lon) {
        double latoff = 90.0/latitudes, lonoff = 180.0/longitudes;

        int lati = round(latitudes*(.5 + (lat-latoff)/180.0));
        int loni = round(longitudes*(lon-lonoff)/360.0);
        if(lati < 0 || lati >= latitudes || loni < 0 || loni >= longitudes)
            return NULL;

        WindPolar *polar = &data[lati*longitudes + loni];
        if(polar->gale == 255)
            return NULL;

        return polar;
    }

    int latitudes, longitudes, dir_cnt;
    float direction_resolution, speed_multiplier;
    WindPolar *data;
};

struct CurrentData
{
    CurrentData(int lats, int lons, int mul)
    : latitudes(lats), longitudes(lons), multiplier(mul)
        { data[0] = new float[lats*lons], data[1] = new float[lats*lons]; }
    double Value(enum Coord coord, int xi, int yi);
    double InterpCurrent(enum Coord coord, double lat, double lon);

    int latitudes, longitudes, multiplier;
    float *data[2];
};

struct ElNinoYear
{
    double months[12];
};

/* because wxDateTime is too slow */
class CycloneDateTime
{
public:
    CycloneDateTime() {}
    CycloneDateTime(int day1, int month1, int year1, int hour1)
        : hour(hour1), day(day1), month(month1), year(year1) {}
    int hour, day, month, year;

    wxDateTime DateTime() { return wxDateTime(day, (wxDateTime::Month)month, year, hour); }
};

class CycloneState
{
public:
    enum State {TROPICAL, SUBTROPICAL, EXTRATROPICAL, WAVE, REMANENT, UNKNOWN};
    CycloneState(State s, CycloneDateTime dt,
                 double lat0, double lon0, double lat1, double lon1,
                 double wk, double press)
        : state(s), datetime(dt),
        windknots(wk), pressure(press), drawn_counter(0)
    {
        lat[0] = lat0, lat[1] = lat1;
        lon[0] = lon0, lon[1] = lon1;
    }

    State state;
    CycloneDateTime datetime;
    double lat[2], lon[2], windknots, pressure;

    // avoid drawing the same segment twice as it may appear multiple times
    // in the hash table
    long drawn_counter;
};

struct Cyclone
{
    std::list<CycloneState*> states;
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
        m_latoff = m_lonoff = 0;
    }

    ~ClimatologyOverlay( void );

    unsigned int m_iTexture; /* opengl mode */

    wxBitmap *m_pDCBitmap; /* dc mode */
    unsigned char *m_pRGBA;

    int m_width, m_height;
    double m_latoff, m_lonoff;
};

//----------------------------------------------------------------------------------------------------------
//    Climatology Overlay Factory Specification
//----------------------------------------------------------------------------------------------------------

class ClimatologyDialog;
class wxGLContext;
class ClimatologyOverlayFactory;

class ClimatologyIsoBarMap : public IsoBarMap
{
public:
 ClimatologyIsoBarMap(wxString name, double spacing, double step,
                      ClimatologyOverlayFactory &factory, int setting, int units,
                      int month, int day)
     : IsoBarMap(name, spacing, step),
        m_factory(factory), m_setting(setting), m_units(units), m_month(month), m_day(day) {}

    double CalcParameter(double lat, double lon);
    bool SameSettings(double spacing, double step, int units, int month, int day)
    {
        return spacing == m_Spacing && step == m_Step && units == m_units
            && month == m_month && day == m_day;
    }

private:
    
    ClimatologyOverlayFactory &m_factory;
    int m_setting, m_units, m_month, m_day;
};

enum {WIND_SETTING, CURRENT_SETTING, PRESSURE_SETTING, SEATEMP_SETTING,
      AIRTEMP_SETTING, CLOUD_SETTING, PRECIPITATION_SETTING, RELHUMIDIY_SETTING,
      LIGHTNING_SETTING, SEADEPTH_SETTING, CYCLONE_SETTING};

class ClimatologyOverlayFactory {
public:
    ClimatologyOverlayFactory( ClimatologyDialog &dlg );
    ~ClimatologyOverlayFactory();

    void GetDateInterpolation(const wxDateTime *cdate,
                              int &month, int &nmonth, double &dpos);

    bool InterpolateWindAtlasTime(int month, int nmonth, double dpos,
                                  double lat, double lon,
                                  double *directions, double *speeds,
                                  double &gale, double &calm);

    bool InterpolateWindAtlas(wxDateTime &date,
                              double lat, double lon,
                              double *directions, double *speeds,
                              double &gale, double &calm);

    double GetMin(int setting);
    double GetMax(int setting);

    double getValueMonth(enum Coord coord, int setting, double lat, double lon, int month);
    double getValue(enum Coord coord, int setting, double lat, double lon, wxDateTime *date);
    double getCurValue(enum Coord coord, int setting, double lat, double lon)
    { return getValue(coord, setting, lat, lon, 0); }
    double getCurCalibratedValue(enum Coord coord, int setting, double lat, double lon);
    double getCalibratedValueMonth(enum Coord coord, int setting, double lat, double lon, int month);

    int CycloneTrackCrossings(
        double lat1, double lon1, double lat2, double lon2,
        const wxDateTime &date, int dayrange);

    wxSemaphore m_cyclone_cache_semaphore;
    std::map<int, std::list<CycloneState*> > m_cyclone_cache;

    void BuildCycloneCache();
    bool RenderOverlay( piDC &dc, PlugIn_ViewPort &vp );

    static wxColour GetGraphicColor(int setting, double val_in);

    wxDateTime m_CurrentTimeline;
    bool m_bAllTimes;

    std::list<wxString> m_FailedFiles; // maybe loaded some data, but some is corrupted or missing
    bool m_bCompletedLoading; // finished loading climatology data without abort

private:
    void Load();
    void LoadInternal(wxGenericProgressDialog *progressdialog);
    void Free();

    void ReadWindData(int month, wxString filename);
    void AverageWindData();
    void ReadCurrentData(int month, wxString filename);
    void AverageCurrentData();
    ZUFILE *OpenClimatologyDataFile(wxString filename);
    void ReadSeaLevelPressureData(wxString filename);
    void ReadSeaSurfaceTemperatureData(wxString filename);
    void ReadAirTemperatureData(wxString filename);
    void ReadCloudData(wxString filename);
    void ReadPrecipitationData(wxString filename);
    void ReadRelativeHumidityData(wxString filename);
    void ReadLightningData(wxString filename);
    void ReadSeaDepthData(wxString filename);
    bool ReadCycloneData(wxString filename, std::list<Cyclone*> &cyclones, bool south=false);
    bool ReadElNinoYears(wxString filename);

    void DrawLine( double x1, double y1, double x2, double y2,
                   const wxColour &color, double width );
    void DrawCircle( double x, double y, double r, const wxColour &color, double width );

    ZUFILE *TryOpenFile(wxString filename);

    void RenderNumber(wxPoint p, double v, const wxColour &color);

    void RenderIsoBars(int setting, PlugIn_ViewPort &vp);
    void RenderNumbers(int setting, PlugIn_ViewPort &vp);
    void RenderDirectionArrows(int setting, PlugIn_ViewPort &vp);

    void RenderWindAtlas(PlugIn_ViewPort &vp);
    void RenderCycloneSegment(CycloneState &ss, PlugIn_ViewPort &vp, int dayspan);
    void RenderCyclones(PlugIn_ViewPort &vp);

    bool CreateGLTexture(ClimatologyOverlay &O, int setting, int month, PlugIn_ViewPort &vp);
    void DrawGLTexture( ClimatologyOverlay &O1, ClimatologyOverlay &O2,
                        double dpos, PlugIn_ViewPort &vp, double transparency);

    void RenderOverlayMap( int setting, PlugIn_ViewPort &vp);

    ClimatologyDialog &m_dlg;
    ClimatologyOverlaySettings &m_Settings;

    ClimatologyOverlay m_pOverlay[13][ClimatologyOverlaySettings::SETTINGS_COUNT];


    piDC *m_dc;

    std::map < double , wxImage > m_labelCache;

    WindData *m_WindData[13];
    CurrentData *m_CurrentData[13];

    /* 12 months + year total and average */
    wxInt16 m_slp[13][90][180];     /* 2 degree intervals   */
    wxInt16 m_sst[13][180][360];    /* 1 degree intervals   */
    wxInt16 m_at[13][90][180];      /* 2 degree intervals   */
    wxInt16 m_cld[13][90][180];     /* 2 degree intervals   */
    wxInt16 m_precip[13][72][144];  /* 2.5 degree intervals */
    wxInt16 m_rhum[13][180][360];   /* 1 degree intervals */
    wxInt16 m_lightn[13][180][360]; /* 1 degree intervals */
    wxInt16 m_seadepth[180][360];   /* 1 degree intervals   */

    int m_cyclonesDisplayList;
    long m_cyclone_drawn_counter;

    std::list<Cyclone*> m_wpa, m_epa, m_spa, m_atl, m_she, m_nio;

    std::map<int, ElNinoYear> m_ElNinoYears;

    wxString m_sFailedMessage;
};
