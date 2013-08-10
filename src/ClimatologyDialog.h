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

#ifndef __ClimatologyDIALOG_H__
#define __ClimatologyDIALOG_H__

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers
#include <wx/fileconf.h>
#include <wx/glcanvas.h>

#include "ClimatologyConfigDialog.h"
#include "ClimatologyOverlayFactory.h"
#include "ClimatologyUI.h"

class ClimatologyOverlayFactory;

class wxFileConfig;
class climatology_pi;

//----------------------------------------------------------------------------------------------------------
//    Climatology Selector/Control Dialog Specification
//----------------------------------------------------------------------------------------------------------
class ClimatologyDialog : public ClimatologyDialogBase {
public:
    ClimatologyDialog(wxWindow *parent, climatology_pi *ppi);
    ~ClimatologyDialog();

    void Init();
    void UpdateTrackingControls();
    void PopulateTrackingControls();
    void RefreshRedraw();
    void SetCursorLatLon(double lat, double lon);

    ClimatologyConfigDialog *m_cfgdlg;

private:
    void SetControlsVisible(ClimatologyOverlaySettings::SettingsType type,
                            wxControl *ctrl1, wxControl *ctrl2=NULL, wxControl *ctrl3=NULL);

    wxString GetValue(int index, Coord coord=MAG);

    void OnMonth( wxCommandEvent& event );
    void OnMonth( wxScrollEvent& event );
    void OnAll( wxCommandEvent& event );
    void OnUpdate( wxCommandEvent& event );
    void OnConfig( wxCommandEvent& event );

    void OnClose( wxCloseEvent& event );
    void OnCBAny( wxCommandEvent& event );

    wxWindow *pParent;
    climatology_pi *pPlugIn;

    double m_cursorlat, m_cursorlon;
};

#endif
