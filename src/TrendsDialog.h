/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Trends Plugin Friends
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

#ifndef __TrendsDIALOG_H__
#define __TrendsDIALOG_H__

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers
#include <wx/fileconf.h>
#include <wx/glcanvas.h>

#include "TrendsUI.h"

#ifndef PI
#define PI        3.1415926535897931160E0      /* pi */
#endif

class TrendsOverlayFactory;

class wxFileConfig;
class trends_pi;

//----------------------------------------------------------------------------------------------------------
//    Trends Selector/Control Dialog Specification
//----------------------------------------------------------------------------------------------------------
class TrendsDialog : public TrendsDialogBase {
public:

    TrendsDialog(wxWindow *parent, trends_pi *ppi);

    ~TrendsDialog();
    void Init();

    void SetFactoryOptions();

private:
    void OnMonth( wxCommandEvent& event );
    void OnMonth( wxScrollEvent& event );
    void OnConfig( wxCommandEvent& event );

    void OnClose( wxCloseEvent& event );
    void OnCBAny( wxCommandEvent& event );

    wxWindow *pParent;
    trends_pi *pPlugIn;
};

#endif

