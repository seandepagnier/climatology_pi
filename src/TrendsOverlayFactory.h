/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Trends Plugin
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

class StormState
{
public:
    enum State {TROPICAL, SUBTROPICAL, EXTRATROPICAL, WAVE, REMANENT, UNKNOWN};
    StormState(State s, wxDateTime dt, double lat, double lon, double wk, double press)
    : state(s), datetime(dt), latitude(lat), longitude(lon), windknots(wk), pressure(press)
    {}

    State state;
    wxDateTime datetime;
    double latitude, longitude, windknots, pressure;
};

class Storm
{
public:
    Storm() { }

//    int stormindex, stormindex_year;
//    wxString name;

    std::list<StormState*> states;

    enum StormType {HURRICANE, TROPICAL_STORM, SUBTROPICAL_STORM, UNKNOWN} type;
};

//----------------------------------------------------------------------------------------------------------
//    Trends Overlay Specification
//----------------------------------------------------------------------------------------------------------

class TrendsOverlay {
public:
    TrendsOverlay( void )
    {
        m_iTexture = 0;
        m_pDCBitmap = NULL, m_pRGBA = NULL;
    }
    ~TrendsOverlay( void )
    {
        if(m_iTexture)
          glDeleteTextures( 1, &m_iTexture );
        delete m_pDCBitmap, delete[] m_pRGBA;
    }

    unsigned int m_iTexture; /* opengl mode */

    wxBitmap *m_pDCBitmap; /* dc mode */
    unsigned char *m_pRGBA;

    int m_width;
    int m_height;
};

//----------------------------------------------------------------------------------------------------------
//    Trends Overlay Factory Specification
//----------------------------------------------------------------------------------------------------------

class TrendsDialog;

class TrendsOverlayFactory {
public:
    TrendsOverlayFactory( TrendsDialog &dlg );
    ~TrendsOverlayFactory();

    void ClearCachedData() {}

    bool RenderOverlay( wxDC &dc, PlugIn_ViewPort *vp );
    bool RenderGLOverlay( wxGLContext *pcontext, PlugIn_ViewPort *vp );

private:
    TrendsDialog &m_dlg;
    std::list<Storm*> storms;
};
