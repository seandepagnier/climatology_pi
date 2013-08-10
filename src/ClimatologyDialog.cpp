/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Climatology Object
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
 *
 */

#include "wx/wx.h"
#include "wx/datetime.h"
#include <wx/dir.h>
#include <wx/debug.h>
#include <wx/graphics.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "climatology_pi.h"
#include "ClimatologyDialog.h"
#include "ClimatologyConfigDialog.h"

ClimatologyDialog::ClimatologyDialog(wxWindow *parent, climatology_pi *ppi)
    : ClimatologyDialogBase(parent)
{
    pParent = parent;
    pPlugIn = ppi;

    wxFileConfig *pConf = GetOCPNConfigObject();

    if(pConf) {
        pConf->SetPath ( _T ( "/Settings/Climatology" ) );
/*
        bool value;
        pConf->Read( _T ( "WindPlot" ), &value, true );
        m_cbWind->SetValue(value);
        pConf->Read( _T ( "WavePlot" ), &value, false );
        m_cbWave->SetValue(value);
        pConf->Read( _T ( "CurrentPlot" ), &value, false );
        m_cbCurrent->SetValue(value);
        pConf->Read( _T ( "PressurePlot" ), &value, true );
        m_cbPressure->SetValue(value);
        pConf->Read( _T ( "SeaTemperaturePlot" ), &value, false );
        m_cbSeaTemperature->SetValue(value);

        pConf->Read ( _T ( "lastdatatype" ), &m_lastdatatype, 0);

        pConf->SetPath ( _T ( "/Directories" ) );
        pConf->Read ( _T ( "ClimatologyDirectory" ), &m_climatology_dir );
*/
    }

    m_cfgdlg = new ClimatologyConfigDialog(this);

    PopulateTrackingControls();

    DimeWindow( this );
}


ClimatologyDialog::~ClimatologyDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();;

    if(pConf) {
        pConf->SetPath ( _T ( "/Settings/Climatology" ) );
/*
        pConf->Write( _T ( "WindPlot" ), m_cbWind->GetValue());
        pConf->Write( _T ( "WavePlot" ), m_cbWave->GetValue());
        pConf->Write( _T ( "CurrentPlot" ), m_cbCurrent->GetValue());
        pConf->Write( _T ( "PressurePlot" ), m_cbPressure->GetValue());
        pConf->Write( _T ( "SeaTemperaturePlot" ), m_cbSeaTemperature->GetValue());
        pConf->Write ( _T ( "lastdatatype" ), m_lastdatatype);

        pConf->SetPath ( _T ( "/Directories" ) );
        pConf->Write ( _T ( "ClimatologyDirectory" ), m_climatology_dir );
*/
    }

    delete m_cfgdlg;
}

void ClimatologyDialog::UpdateTrackingControls()
{
    if(!pPlugIn->GetOverlayFactory())
        return;

    pPlugIn->GetOverlayFactory()->m_CurrentMonth = m_cbAll->GetValue() ? 12 : (m_sMonth->GetValue() % 12);

    m_tWind->SetValue(GetValue(ClimatologyOverlaySettings::WIND));
    m_tWindDir->SetValue(GetValue(ClimatologyOverlaySettings::WIND, MDIRECTION));
    m_tCurrent->SetValue(GetValue(ClimatologyOverlaySettings::CURRENT));
    m_tCurrentDir->SetValue(GetValue(ClimatologyOverlaySettings::CURRENT, DIRECTION));
    m_tPressure->SetValue(GetValue(ClimatologyOverlaySettings::SLP));
    m_tSeaTemperature->SetValue(GetValue(ClimatologyOverlaySettings::SST));
    m_tAirTemperature->SetValue(GetValue(ClimatologyOverlaySettings::AT));
    m_tCloudCover->SetValue(GetValue(ClimatologyOverlaySettings::CLOUD));
    m_tPrecipitation->SetValue(GetValue(ClimatologyOverlaySettings::PRECIPITATION));
    m_tRelativeHumidity->SetValue(GetValue(ClimatologyOverlaySettings::RELATIVE_HUMIDITY));
    m_tSeaDepth->SetValue(GetValue(ClimatologyOverlaySettings::SEADEPTH));
}

void ClimatologyDialog::PopulateTrackingControls()
{
    SetControlsVisible(ClimatologyOverlaySettings::WIND, m_cbWind, m_tWind, m_tWindDir);
    SetControlsVisible(ClimatologyOverlaySettings::CURRENT, m_cbCurrent, m_tCurrent, m_tCurrentDir);
    SetControlsVisible(ClimatologyOverlaySettings::SLP, m_cbPressure, m_tPressure);
    SetControlsVisible(ClimatologyOverlaySettings::SST, m_cbSeaTemperature, m_tSeaTemperature);
    SetControlsVisible(ClimatologyOverlaySettings::AT, m_cbAirTemperature, m_tAirTemperature);
    SetControlsVisible(ClimatologyOverlaySettings::CLOUD, m_cbCloudCover, m_tCloudCover);
    SetControlsVisible(ClimatologyOverlaySettings::PRECIPITATION, m_cbPrecipitation, m_tPrecipitation);
    SetControlsVisible(ClimatologyOverlaySettings::RELATIVE_HUMIDITY, m_cbRelativeHumidity, m_tRelativeHumidity);
    SetControlsVisible(ClimatologyOverlaySettings::SEADEPTH, m_cbSeaDepth, m_tSeaDepth);

    Fit();
    Refresh();
}

void ClimatologyDialog::RefreshRedraw()
{
    RequestRefresh( pParent );
}

void  ClimatologyDialog::SetCursorLatLon(double lat, double lon)
{
    m_cursorlat = lat;
    m_cursorlon = lon;

    UpdateTrackingControls();
}

void ClimatologyDialog::SetControlsVisible(ClimatologyOverlaySettings::SettingsType type,
                                           wxControl *ctrl1, wxControl *ctrl2, wxControl *ctrl3)
{
    if(m_cfgdlg->m_Settings.Settings[type].m_bEnabled) {
        ctrl1->Show();
        if(ctrl2) ctrl2->Show();
        if(ctrl3) ctrl3->Show();
    } else {
        ctrl1->Hide();
        if(ctrl2) ctrl2->Hide();
        if(ctrl3) ctrl3->Hide();
    }
}

wxString ClimatologyDialog::GetValue(int index, Coord coord)
{
    return wxString::Format(_T("%.2f"), pPlugIn->
                            GetOverlayFactory()->getCurCalibratedValue
                            (coord, index, m_cursorlat, m_cursorlon));
}

void ClimatologyDialog::OnMonth( wxCommandEvent& event )
{
    m_sMonth->SetValue(event.GetSelection());
    UpdateTrackingControls();
    RefreshRedraw();
}

void ClimatologyDialog::OnMonth( wxScrollEvent& event )
{
    m_cMonth->SetSelection(event.GetPosition() % 12);
    UpdateTrackingControls();
    RefreshRedraw();
}

void ClimatologyDialog::OnAll( wxCommandEvent& event )
{
    m_cMonth->Enable(!m_cbAll->GetValue());
    m_sMonth->Enable(!m_cbAll->GetValue());
    UpdateTrackingControls();
    RefreshRedraw();
}

void ClimatologyDialog::OnUpdate( wxCommandEvent& event )
{
    RefreshRedraw();
}

void ClimatologyDialog::OnConfig( wxCommandEvent& event )
{
    m_cfgdlg->Show(!m_cfgdlg->IsShown());
}

void ClimatologyDialog::OnClose( wxCloseEvent& event )
{
    pPlugIn->OnClimatologyDialogClose();
}

void ClimatologyDialog::OnCBAny( wxCommandEvent& event )
{
    RefreshRedraw();                     // Reload the visibility options
}
