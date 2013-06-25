/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Isobars
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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/progdlg.h>

#include "../../../include/ocpn_plugin.h"

#include "IsoBarMap.h"
#include "defs.h"

/* initialize cache to contain data */
void ParamCache::Initialize(double step)
{
    if(m_step != step) {
        m_step = step;
        delete [] values;
        int size = 360/step;
        values = new double[size];
    }
    m_lat=100; /* invalidate data */
}

/* attempt a cache read returning a hit or miss */
bool ParamCache::Read(double lat, double lon, double &value)
{
    if(lat != m_lat)
        return false;
    lon += 180;
    if(lon > 360)
        lon -= 360;
    if(lon < 0 || lon >= 360)
        return false;
    double div = lon / m_step;
    if(div != floor(div))
        return false;

    value = values[(int)div];
    return true;
}


#if 0
/* set the accuracy of the map */
void MagneticPlotMap::ConfigureAccuracy(int step, int poleaccuracy)
{
    /* keeping m_Step powers of 2 */
    switch(step) {
    case 1: m_Step = .0625; break;
    case 2: m_Step = .125; break;
    case 3: m_Step = .25; break;
    case 4: m_Step = .5; break;
    case 5: m_Step = 1; break;
    case 6: m_Step = 2; break;
    case 7: m_Step = 4; break;
    default: m_Step = 8; break;
    }

    /* keeping m_PoleAccuracy logarithmic */
    switch(poleaccuracy) {
    case 1: m_PoleAccuracy = 5e-1; break;
    case 2: m_PoleAccuracy = 1e-1; break;
    case 3: m_PoleAccuracy = 1e-2; break;
    case 4: m_PoleAccuracy = 1e-3; break;
    default: m_PoleAccuracy = 1e-4; break;
    }
}

/* compute the graphed parameter for one lat/lon location */
double MagneticPlotMap::CalcParameter(double lat, double lon)
{
      WMMtype_CoordSpherical CoordSpherical;
      WMMtype_CoordGeodetic CoordGeodetic;
      WMMtype_GeoMagneticElements GeoMagneticElements;

      CoordGeodetic.lambda = lon;
      CoordGeodetic.phi = lat;
      CoordGeodetic.HeightAboveEllipsoid = 0;
      CoordGeodetic.HeightAboveGeoid = 0;
      CoordGeodetic.UseGeoid = 0;

      /* Convert from geodeitic to Spherical Equations: 17-18, WMM Technical report */
      WMM_GeodeticToSpherical(*Ellip, CoordGeodetic, &CoordSpherical);

      /* Computes the geoMagnetic field elements and their time change */
      WMM_Geomag(*Ellip, CoordSpherical, CoordGeodetic, TimedMagneticModel, &GeoMagneticElements);
      WMM_CalculateGridVariation(CoordGeodetic, &GeoMagneticElements);

      double ret = 0;
      switch(m_type) {
      case DECLINATION: ret = GeoMagneticElements.Decl >= 180 ?
              GeoMagneticElements.Decl - 360 : GeoMagneticElements.Decl;
          break;
      case INCLINATION: ret = GeoMagneticElements.Incl;
          break;
      case FIELD_STRENGTH: ret = GeoMagneticElements.F;
          break;
      }

      return ret;
}
#endif

/* build up cache for all longitudes */
void IsoBarMap::BuildParamCache(ParamCache &cache, double lat)
{
    int i=0;
    for(double lon = -180; lon < 180; lon += m_Step, i++)
        cache.values[i] = Parameter(lat, lon);
    cache.m_lat = lat;
}

IsoBarMap::IsoBarMap(wxString name, double spacing, double step) :
    m_bNeedsRecompute(false), m_bComputing(false),
    m_Spacing(spacing), m_Step(step), m_PoleAccuracy(1e-4),
    m_MinContour(NAN), m_MaxContour(NAN),
    m_contourcachesize(0), m_contourcache(NULL),
    m_Name(name), m_bPolar(false), m_Color(*wxBLACK)
     {}

IsoBarMap::~IsoBarMap() { ClearMap(); }

/* a possible speedup would be to cache the last 4-10 values
   calculated as well as the two main cache banks to speed
   up the recursion in PlotRegion */
double IsoBarMap::CachedParameter(double lat, double lon)
{
    double value;
    if(!m_Cache[0].Read(lat, lon, value) &&
       !m_Cache[1].Read(lat, lon, value))
        value = Parameter(lat, lon);
    return value;
}

double IsoBarMap::Parameter(double lat, double lon)
{
    double ret = CalcParameter(lat, lon);
    if(isnan(m_MinContour) || ret < m_MinContour)
        m_MinContour = ret;
    if(isnan(m_MaxContour) || ret > m_MaxContour)
        m_MaxContour = ret;
    return ret;
}

/* given a line segment (x1, y1) (x2, y2), find the point (rx, ry) along it
   which crosses a contour.  if lat is true, x1 and x2 are latitudes, other
   wise longitudes.  lonval is the complement to this which is for both x1 and x2
   to allow computing new y values along te segment.   rx is set to nan if
   there is no intersection.  True is returned if success, otherwise false
   to signify that we need to dig deeper to get a decent map. */
bool IsoBarMap::Interpolate(double x1, double x2, double y1, double y2, bool lat,
                                  double lonval, double&rx, double &ry)
{
    if(fabs(x1-x2) < m_PoleAccuracy) { /* to avoid recursing too far. make this value
                                          smaller to get more accuracy especially near the magnetic poles */
        rx = NAN; /* set as no intersections */
        return true;
    }

    /* this really only happens between geographic and magnetic pole, but to correct it... */
    if(m_bPolar)
    {
        if(y1-y2 > 180)
            y2+=360;
        if(y2-y1 > 180)
            y1+=360;
    }

    y1 /= m_Spacing;
    y2 /= m_Spacing;

    double fy1 = floor(y1), fy2 = floor(y2);
    if(fy1 == fy2) {
        rx = NAN; /* no intersections occured */
        return true;
    }

    if(fabs(fy1 - fy2) > 1) /* stepped over too many lines, trigger more recursion */
        return false;

    /* make y2 larger */
    if(y1 > y2) {
        double t = y2;
        y2 = y1;
        y1 = t;
        t = x2;
        x2 = x1;
        x1 = t;
    }

    ry = floor(y2);
    for(int i=0;;i++) {

//  x1=m*y1+b   x2=m*y2+b
//  (x1-b)/y1=(x2-b)/y2    x1/y1-x2/y2=b*(1/y1-1/y2)
//  b = (x1/y1-x2/y2)/(1/y1-1/y2)
//  b = [(x1/y1-x2/y2)* (y1*y2)]/[(1/y1-1/y2) *(y1*y2)]
//  b = (x1*y2-x2*y1)/(y2-y1)
    
        rx = (x1*(y2-ry)-x2*(y1-ry))/(y2-y1);

        if(fabs(x1-x2) < m_PoleAccuracy) /* to avoid recursing too far close */
            return true;
   
        double p;
        if(lat)
            p = Parameter(rx, lonval);
        else
            p = Parameter(lonval, rx);

        if(isnan(p)) /* is this actually correct? */
            return true;

        if(m_bPolar && p-ry*m_Spacing < -180) /* way off, try other way around */
            p += 360;

        p/=m_Spacing;

        double err = p-ry;

        /* this value of 1e-3 could be reduced to increase accuracy, this value seems ok */
        if(fabs(err) < 1e-5 || p == y1 || p == y2) /* close enough */
            return true;

        if(err<0) {
            if(p < y1) /* undershot */
                return false;
            x1 = rx;
            y1 = p;
        } else {
            if(p > y2) /* overshot */
                return false;
            x2 = rx;
            y2 = p;
        }
    }    
}

/* once we have a final line segment, store it in the database */
void AddLineSeg(std::list<PlotLineSeg*> &region, double lat1, double lon1, double lat2, double lon2,
                double contour1, double contour2)
{
    if(contour1 != contour2) /* this should not be possible */
        return;

    PlotLineSeg *seg = new PlotLineSeg(lat1, lon1, lat2, lon2, contour1);
    region.push_back(seg);
}


/* we generate contour maps by sampling the value at various
   latitude and longitude positions:

         lon1  lon3  lon2
    lat1  1-----*-----2
          |           |
    lat3  *           * lat4
          |           |
    lat2  3-----*-----4
              lon4

*/
void IsoBarMap::PlotRegion(std::list<PlotLineSeg*> &region,
                                 double lat1, double lon1, double lat2, double lon2)
{
    double p1 = CachedParameter(lat1, lon1);
    double p2 = CachedParameter(lat1, lon2);
    double p3 = CachedParameter(lat2, lon1);
    double p4 = CachedParameter(lat2, lon2);

    if(isnan(p1) || isnan(p2) || isnan(p3) || isnan(p4))
        return;

    double ry1, ry2, ry3, ry4;
    double lon3, lon4, lat3, lat4;
    /* horizontal interpolate to determine intermediate longitudes as well
       as the contours they are on. */
    if(!Interpolate(lon1, lon2, p1, p2, false, lat1, lon3, ry1) ||
       !Interpolate(lon1, lon2, p3, p4, false, lat2, lon4, ry2)) {
        lon3 = (lon1+lon2)/2;
        PlotRegion(region, lat1, lon1, lat2, lon3);
        PlotRegion(region, lat1, lon3, lat2, lon2);
        return;
    }
    
    /* vertical interpolate */
    if(!Interpolate(lat1, lat2, p1, p3, true, lon1, lat3, ry3) ||
       !Interpolate(lat1, lat2, p2, p4, true, lon2, lat4, ry4)) {
        lat3 = (lat1+lat2)/2;
        PlotRegion(region, lat1, lon1, lat3, lon2);
        PlotRegion(region, lat3, lon1, lat2, lon2);
        return;
    }

    /* un-normalize contours */
    ry1*=m_Spacing, ry2*=m_Spacing, ry3*=m_Spacing, ry4*=m_Spacing;

    /* determine which interpolations need line segments */
    switch(((isnan(lat4)*2 + isnan(lat3))*2 + isnan(lon4))*2 + isnan(lon3)) {
    case 0: /* all 4 sides? need to recurse to get better resolution */
        lon3 = (lon1+lon2)/2;
        lat3 = (lat1+lat2)/2;
        PlotRegion(region, lat1, lon1, lat3, lon3);
        PlotRegion(region, lat1, lon3, lat3, lon2);
        PlotRegion(region, lat3, lon1, lat2, lon3);
        PlotRegion(region, lat3, lon3, lat2, lon2);
        break;
    case 1: case 2: case 4: case 8: case 7: case 11: case 13: case 14: break; /* impossible! */
    case 3: /* horizontal */ AddLineSeg(region, lat3, lon1, lat4, lon2, ry3, ry4); break;
    case 5: /* diagonal */   AddLineSeg(region, lat2, lon4, lat4, lon2, ry2, ry4); break;
    case 6: /* diagonal */   AddLineSeg(region, lat1, lon3, lat4, lon2, ry1, ry4); break;
    case 9: /* diagonal */   AddLineSeg(region, lat3, lon1, lat2, lon4, ry2, ry3); break;
    case 10:/* diagonal */   AddLineSeg(region, lat3, lon1, lat1, lon3, ry1, ry3); break;
    case 12:/* vertical */   AddLineSeg(region, lat1, lon3, lat2, lon4, ry1, ry2); break;
    case 15: /* no intersections */ break;
    }
}

#if 0
/* rebuild the map at a given date */
bool MagneticPlotMap::Recompute(wxDateTime date)
{
    if(!m_bEnabled)
        return true;

    UserDate.Year = date.GetYear();
    UserDate.Month = date.GetMonth();
    UserDate.Day = date.GetDay();

    char err[255];
    WMM_DateToYear(&UserDate, err);

    /* Time adjust the coefficients, Equation 19, WMM Technical report */
    WMM_TimelyModifyMagneticModel(UserDate, MagneticModel, TimedMagneticModel);

    return IsoBarMap::Recompute()
}
#endif


bool IsoBarMap::Recompute(wxWindow *parent)
{
    /* clear out old data */
  ClearMap();

  m_bComputing = true;

#if 1
  wxProgressDialog *progressdialog = NULL;
  wxDateTime start = wxDateTime::Now();
#endif

  int cachepage = 0;
   m_Cache[0].Initialize(m_Step);
   m_Cache[1].Initialize(m_Step);

   double min = -MAX_LAT, max = MAX_LAT;

  BuildParamCache(m_Cache[cachepage], min);

  double lastupdate = min;
  for(double lat = min; lat + m_Step <= max; lat += m_Step) {
      if(m_bNeedsRecompute) {
          delete progressdialog;
          return false;
      }

      if(lat - lastupdate > 8) {
          lastupdate = lat;
          if(progressdialog) {
              if(!progressdialog->Update(lat - min)) {
                  delete progressdialog;
                  return false;
              }
          } else {
              wxDateTime now = wxDateTime::Now();
              if((now-start).GetMilliseconds() > 500 && lat < (max - min)/2) {
                  progressdialog = new wxProgressDialog(
                      _("Building Isobar Map"), m_Name, max - min + 1, parent,
                      wxPD_ELAPSED_TIME | wxPD_CAN_ABORT);
              }
          }
      }
      cachepage = !cachepage;
      BuildParamCache(m_Cache[cachepage], lat+m_Step);

      int latind = floor((lat+MAX_LAT)/ZONE_SIZE);
      if(latind>LATITUDE_ZONES-1)
          latind = LATITUDE_ZONES-1;
      for(double lon = -180; lon+m_Step <= 180; lon += m_Step) {
          int lonind = floor((lon+180)/ZONE_SIZE);

          PlotRegion(m_map[latind][lonind], lat, lon, lat+m_Step, lon+m_Step);
      }
  }

  delete progressdialog;

  m_bComputing = false;

  m_MinContour /= m_Spacing;
  m_MinContour = floor(m_MinContour);
  m_MinContour *= m_Spacing;

  m_MaxContour /= m_Spacing;
  m_MaxContour = ceil(m_MaxContour);
  m_MaxContour *= m_Spacing;

  if(isnan(m_MaxContour) || isnan(m_MinContour))
      m_contourcachesize = 0;
  else
      m_contourcachesize = (m_MaxContour - m_MinContour) / m_Spacing + 1;

  m_contourcache = new ContourBitmap[m_contourcachesize];
  for(int i=0; i<m_contourcachesize; i++)
      m_contourcache[i] = ContourCacheData(m_MinContour + i*m_Spacing);

  return true;
}

/* draw a line segment in opengl from lat/lon and viewport */
void DrawLineSeg(wxDC *dc, PlugIn_ViewPort &VP, double lat1, double lon1, double lat2, double lon2)
{
    /* avoid lines which cross over the view port the long way */
    if(lon1+180 < VP.clon && lon2+180 > VP.clon)
        return;
    if(lon1+180 > VP.clon && lon2+180 < VP.clon)
        return;
    if(lon1-180 < VP.clon && lon2-180 > VP.clon)
        return;
    if(lon1-180 > VP.clon && lon2-180 < VP.clon)
        return;

    wxPoint r1, r2;
    GetCanvasPixLL(&VP, &r1, lat1, lon1);
    GetCanvasPixLL(&VP, &r2, lat2, lon2);

    if(dc)
        dc->DrawLine(r1.x, r1.y, r2.x, r2.y);
    else {
        glBegin(GL_LINES);
        glVertex2i(r1.x, r1.y);
        glVertex2i(r2.x, r2.y);
        glEnd();
    }
}

/* reset the map and clear all the data so it can be reused */
void IsoBarMap::ClearMap()
{
    for(int latind=0; latind<LATITUDE_ZONES; latind++)
        for(int lonind=0; lonind<LONGITUDE_ZONES; lonind++)
            m_map[latind][lonind].clear();

    for(int i=0; i<m_contourcachesize; i++) {
        delete [] m_contourcache[i].data;
    }
    delete [] m_contourcache;

    m_MinContour = m_MaxContour = NAN;
    m_contourcachesize = 0;
    m_contourcache = NULL;
}

/* generate a cache bitmap of a given number */
ContourBitmap IsoBarMap::ContourCacheData(double value)
{
    wxString msg;
    msg.Printf(_("%.0f"), value);

    wxBitmap bm( 120, 25 );
    wxMemoryDC mdc( bm );
    mdc.Clear();

    wxFont mfont( 15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL );
    mdc.SetFont( mfont );
    mdc.SetPen( *wxBLACK_PEN);
    mdc.SetBrush( *wxWHITE_BRUSH);

    int w, h;
    mdc.GetTextExtent( msg, &w, &h );

    /* in the case that the font or something fails, and the dimensions are off,
       we will still create a valid image. */
    if(w <= 0)
        w = 1;
    else if(w > 120)
        w = 120;
    if(h <= 0)
        h = 1;
    else if(h > 25)
        h = 25;

    mdc.DrawText( msg, 0, 0 );

    mdc.SelectObject( wxNullBitmap );

    wxRect r(0, 0, w, h);
    wxBitmap sbm = bm.GetSubBitmap(r);
    wxImage image = sbm.ConvertToImage();
    image.InitAlpha();

    unsigned char *d = image.GetData();
    unsigned char *a = image.GetAlpha();

    w = image.GetWidth(), h = image.GetHeight();
    unsigned char *e = new unsigned char[4 * w * h];
    for( int y = 0; y < h; y++ )
        for( int x = 0; x < w; x++ ) {
            unsigned char r, g, b;
            int ioff = (y * w + x);
            r = d[ioff* 3 + 0];
            g = d[ioff* 3 + 1];
            b = d[ioff* 3 + 2];

            a[ioff] = 255-r;
            
            int off = ( y * w + x );
            e[off * 4 + 0] = r;
            e[off * 4 + 1] = g;
            e[off * 4 + 2] = b;
            e[off * 4 + 3] = 255-r;
        }

    ContourBitmap t;
    t.image = image;
    t.data = e;
    return t;
}

/* draw text of the value of a conptour at a given location */
void IsoBarMap::DrawContour(wxDC *dc, PlugIn_ViewPort &VP, double contour, double lat, double lon)
{
    int index = (contour - m_MinContour) / m_Spacing;
    if(index < 0 || index >= m_contourcachesize)
        return;

    wxPoint r;

    GetCanvasPixLL(&VP, &r, lat, lon);

    double dist_squared1 = square(r.x-m_contourcache[index].lastx)
        + square(r.y-m_contourcache[index].lasty);
    double dist_squared2 = square(r.x-lastx) + square(r.y-lasty);
    /* avoid printing numbers on top of each other */
    if(dist_squared1 < 100000 || dist_squared2 < 40000)
        return;

    m_contourcache[index].lastx = lastx = r.x;
    m_contourcache[index].lasty = lasty = r.y;

    int w = m_contourcache[index].image.GetWidth();
    int h = m_contourcache[index].image.GetHeight();

    if(dc) {
        wxBitmap bmp(m_contourcache[index].image);
        dc->DrawBitmap(bmp, r.x - w/2, r.y - h/2, true);
    } else {
        glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
        glColor4f( 1, 1, 1, 1 );

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        /* center */
        glRasterPos2i( r.x - w/2, r.y - h/2);
        glPixelZoom( 1, -1 ); /* draw data from top to bottom */
        glDrawPixels( w, h, GL_RGBA, GL_UNSIGNED_BYTE, m_contourcache[index].data);
        glPixelZoom( 1, 1 );

        glPopAttrib();
    }
}

/* plot to dc, or opengl is dc is NULL */
void IsoBarMap::Plot(wxDC *dc, PlugIn_ViewPort &vp)
{
    if(dc) {
        dc->SetPen(wxPen(m_Color, 3));
    } else {
        glLineWidth(3.0);
        glColor4ub(m_Color.Red(), m_Color.Green(), m_Color.Blue(), m_Color.Alpha());
    }

    int startlatind = floor((vp.lat_min+MAX_LAT)/ZONE_SIZE);
    if(startlatind < 0) startlatind = 0;

    int endlatind = floor((vp.lat_max+MAX_LAT)/ZONE_SIZE);
    if(endlatind > LATITUDE_ZONES-1) endlatind = LATITUDE_ZONES-1;

    double lon_min = vp.lon_min; /* expected +- 360 convert to +- 180 */
    if(lon_min<-180) lon_min+=360; else if(lon_min>=180) lon_min-=360;
    int startlonind = floor((lon_min+180)/ZONE_SIZE);
    if(startlonind < 0) startlonind = LONGITUDE_ZONES-1;
    if(startlonind > LONGITUDE_ZONES-1) startlonind = 0;

    double lon_max = vp.lon_max; /* expected +- 360 convert to +- 180 */
    if(lon_max<-180) lon_max+=360; else if(lon_max>=180) lon_max-=360;
    int endlonind = floor((lon_max+180)/ZONE_SIZE);
    if(endlonind < 0) endlonind = LONGITUDE_ZONES-1;
    if(endlonind > LONGITUDE_ZONES-1) endlonind = 0;

    for(int latind = startlatind; latind <= endlatind; latind++)
        for(int lonind = startlonind;;lonind++) {
            if(lonind > LONGITUDE_ZONES-1)
                lonind = 0;
            for(std::list<PlotLineSeg*>::iterator it = m_map[latind][lonind].begin();
                it!=m_map[latind][lonind].end(); it++) {
                DrawLineSeg(dc, vp, (*it)->lat1, (*it)->lon1, (*it)->lat2, (*it)->lon2);
                wxString msg;
                DrawContour(dc, vp, (*it)->contour,
                            ((*it)->lat1 + (*it)->lat2)/2,
                            ((*it)->lon1 + (*it)->lon2)/2);
            }
            if(lonind == endlonind)
                break;
        }
}
