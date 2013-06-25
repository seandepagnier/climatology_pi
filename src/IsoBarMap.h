/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Iso Bars
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2013 by Sean D'Epagnier   *
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


/* must be a power of 2, and also divide 360 and 176;
   really only 8 works without more modifications */
#define ZONE_SIZE 8

/* divisible by 8 and any closer to the pole than this plots
   horribly anyway on a flattened earth. */
#define MAX_LAT 88
#define LATITUDE_ZONES (2*MAX_LAT/ZONE_SIZE) /* perfectly divisible */
#define LONGITUDE_ZONES (360/ZONE_SIZE)

/* a single line segment in the plot */
class PlotLineSeg
{
public:
    PlotLineSeg(double _lat1, double _lon1, double _lat2, double _lon2, double _contour)
        : lat1(_lat1), lon1(_lon1), lat2(_lat2), lon2(_lon2), contour(_contour) {}
    double lat1, lon1, lat2, lon2;
    double contour;
};

/* cache values computed to improve speed */
class ParamCache
{
public:
ParamCache() : values(NULL), m_step(0) {}
    ~ParamCache() { delete [] values; }
    void Initialize(double step);
    bool Read(double lat, double lon, double &value);

    double *values;
    double m_step;
    double m_lat;
};

/* cache text bitmap data to speed up rendering.  This contains the rgba
   values in data for opengl, or the image in wxformat */
class ContourBitmap
{
public:
    wxImage image;
    unsigned char *data;

    int lastx, lasty; /* when rendering to prevent overcluttering */
};

/* main model map suitable for a single plot type */
class IsoBarMap
{
public:
    IsoBarMap(wxString name, double spacing, double step);
    virtual ~IsoBarMap();

    bool Recompute(wxWindow *parent);

    void Plot(wxDC *dc, PlugIn_ViewPort &vp);

    bool m_bNeedsRecompute, m_bComputing;
protected:
    double m_Spacing, m_Step, m_PoleAccuracy;

private:
    virtual double CalcParameter(double lat, double lon) = 0;
    double Parameter(double lat, double lon);

    void PlotRegion(std::list<PlotLineSeg*> &region,
                    double lat1, double lon1, double lat2, double lon2);
    void BuildParamCache(ParamCache &cache, double lat);
    double CachedParameter(double lat, double lon);
    bool Interpolate(double x1, double x2, double y1, double y2, bool lat,
                     double lonval, double &rx, double &ry);

    void ClearMap();
    ContourBitmap ContourCacheData(double value);
    void DrawContour(wxDC *dc, PlugIn_ViewPort &VP, double contour, double lat, double lon);

    /* two caches for all longitudes alternate
       places (step over each other) to cover the two latitudes
       currently being built */
    ParamCache m_Cache[2];

    /* the line segments for the entire globe split into zones */
    std::list<PlotLineSeg*> m_map[LATITUDE_ZONES][LONGITUDE_ZONES];

    double m_MinContour, m_MaxContour;
    int m_contourcachesize;
    ContourBitmap *m_contourcache;
    int lastx, lasty; /* when rendering to prevent overcluttering */

    wxString m_Name;
    bool m_bPolar;
    wxColour m_Color;
};

#if 0
enum MagneticPlotType {DECLINATION, INCLINATION, FIELD_STRENGTH};

class MagneticPlotMap : public IsoBarMap
{
    MagneticPlotMap(IsoBarType type,
                    WMMtype_MagneticModel *&mm,
                    WMMtype_MagneticModel *&tmm,
                    WMMtype_Ellipsoid *ellip)
        : m_type(type), MagneticModel(mm), TimedMagneticModel(tmm), Ellip(ellip)
    {
        switch(m_type) {
        case DECLINATION:
            m_Name = _("Declination");
            m_bPolar = true;
            break;
        case INCLINATION:
            m_Name = _("Inclination");
            break;
        default:
            m_Name = _("Field Strength");
        }
    }

    bool m_bEnabled;
    MagneticPlotType m_type;

    WMMtype_MagneticModel *&MagneticModel;
    WMMtype_MagneticModel *&TimedMagneticModel;
    WMMtype_Ellipsoid *Ellip;
    WMMtype_Date UserDate;

    bool Recompute(wxDateTime date);
    void ConfigureAccuracy(int step, int poleaccuracy);

    void Plot(wxDC *dc, PlugIn_ViewPort &vp)
    {
        if(m_bEnabled)
            IsoBarMap::Plot(dc, vp);
    }
};
#endif
