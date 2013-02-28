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
 *
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/progdlg.h>

#include "trends_pi.h"
#include "TrendsDialog.h"

//----------------------------------------------------------------------------------------------------------
//    Trends Overlay Factory Implementation
//----------------------------------------------------------------------------------------------------------
TrendsOverlayFactory::TrendsOverlayFactory( TrendsDialog &dlg )
    : m_dlg(dlg)
{
    wxString hurricane_text_path = *GetpSharedDataLocation()
        + _T("plugins/trends/data/hurricanetracks.txt");

    FILE *f = fopen(hurricane_text_path.mb_str(), "r");
    if (!f) {
        wxLogMessage(_T("trends_pi: failed to open file: ") + hurricane_text_path);
        return;
    }

    char line[128];
    while(fgets(line, sizeof line, f)) {
        char *year  = line+12; line[16] = 0;
        char *days  = line+19; line[21] = 0;
//        char *synum = line+22; line[24] = 0; /* storm this year */
//        char *snum  = line+30; line[34] = 0; /* storm number in total */
//        char *sname = line+35; line[46] = 0;
        
        long lyear = strtol(year, 0, 10);
        long ldays = strtol(days, 0, 10);

        Storm *storm = new Storm;

        /* daily data */
        for(int i=0; i<ldays; i++) {
            if(!fgets(line, sizeof line, f)) {
                wxLogMessage(_T("trends_pi: hurricane text file ended before it should"));
                goto done;
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

                StormState::State stormstate;
                switch(state) {
                case '*': stormstate = StormState::TROPICAL; break;
                case 'S': stormstate = StormState::SUBTROPICAL; break;
                case 'E': stormstate = StormState::EXTRATROPICAL; break;
                case 'W': stormstate = StormState::WAVE; break;
                case 'L': stormstate = StormState::REMANENT; break;
                default: stormstate = StormState::UNKNOWN; break;
                }

                double dlat = strtod(lat, 0), dlon = -strtod(lon, 0);
                if(dlat || dlon)
                    storm->states.push_back
                        (new StormState(stormstate, wxDateTime(lday, lmonth, lyear, h*6),
                                        dlat, dlon, strtod(wk, 0), strtod(press, 0)));

                s += 17;
            }
        }

        /* trailer */
        if(!fgets(line, sizeof line, f)) {
            wxLogMessage(_T("trends_pi: hurricane text file ended before it should"));
            goto done;
        }

        if(line[6] == 'H' && line[7] == 'R')
            storm->type = Storm::HURRICANE;
        else
        if(line[6] == 'T' && line[7] == 'S')
            storm->type = Storm::TROPICAL_STORM;
        else
        if(line[6] == 'S' && line[7] == 'S')
            storm->type = Storm::SUBTROPICAL_STORM;
        else
            storm->type = Storm::UNKNOWN;
        
        storms.push_back(storm);
    }

done:
    fclose(f);
}

TrendsOverlayFactory::~TrendsOverlayFactory()
{
}

bool TrendsOverlayFactory::RenderOverlay( wxDC &dc, PlugIn_ViewPort *vp )
{
    return true;
}

bool TrendsOverlayFactory::RenderGLOverlay( wxGLContext *pcontext, PlugIn_ViewPort *vp )
{
    glLineWidth(1);

    for(std::list<Storm*>::iterator it = storms.begin(); it != storms.end(); it++) {
        Storm *s = *it;
        glBegin(GL_LINES);

        bool first = true;
        double lastlon;
        wxPoint lastp;

        for(std::list<StormState*>::iterator it2 = s->states.begin(); it2 != s->states.end(); it2++) {
            wxPoint p;
            StormState *ss = *it2;
            double lat = ss->latitude, lon = ss->longitude;
            GetCanvasPixLL( vp, &p, lat, lon );

            if(first) {
                first = false;
            } else {

#if 1 /* if you want to zoom in really really far and still see tracks */
                if((lastlon+180 > vp->clon || lon+180 < vp->clon) &&
                   (lastlon+180 < vp->clon || lon+180 > vp->clon) &&
                   (lastlon-180 > vp->clon || lon-180 < vp->clon) &&
                   (lastlon-180 < vp->clon || lon-180 > vp->clon)) {
#else
                if(abs(lastp.x-p.x) < vp->pix_width) {
#endif

                    double intense =  (ss->windknots / 150);

                    if(intense > 1) intense = 1;
                    switch(ss->state) {
                    case StormState::TROPICAL:      glColor3d(intense, 0, 0); break;
                    case StormState::SUBTROPICAL:   glColor3d(intense/2, intense/2, 0); break;
                    case StormState::EXTRATROPICAL: glColor3d(0, intense, 0); break;
                    default:                        glColor3d(0, 0, intense); break;
                    }
                    
                    glVertex2i(lastp.x, lastp.y);
                    glVertex2i(p.x, p.y);
                }
            }

            lastlon = lon;
            lastp = p;
        }
        glEnd();
    }
    return true;
}
