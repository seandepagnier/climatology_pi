/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Climatology Object
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
*/
    }

    m_cfgdlg = new ClimatologyConfigDialog(this);

    PopulateTrackingControls();

    wxDateTime now = wxDateTime::Now();

    m_cMonth->SetSelection(now.GetMonth());
    m_sDay->SetValue(now.GetDay());

    int timeline = now.GetDayOfYear();
    if(timeline <= 67)
        timeline += 356;
    m_sTimeline->SetValue(timeline);

    m_cursorlat = m_cursorlon = 0;

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
*/
    }

    delete m_cfgdlg;
}

void ClimatologyDialog::UpdateTrackingControls()
{
    if(!pPlugIn->GetOverlayFactory())
        return;

    m_tWind->SetValue(GetValue(ClimatologyOverlaySettings::WIND));
    m_tWindDir->SetValue(GetValue(ClimatologyOverlaySettings::WIND, DIRECTION));
    m_tCurrent->SetValue(GetValue(ClimatologyOverlaySettings::CURRENT));
    m_tCurrentDir->SetValue(GetValue(ClimatologyOverlaySettings::CURRENT, DIRECTION));
    m_tPressure->SetValue(GetValue(ClimatologyOverlaySettings::SLP));
    m_tSeaTemperature->SetValue(GetValue(ClimatologyOverlaySettings::SST));
    m_tAirTemperature->SetValue(GetValue(ClimatologyOverlaySettings::AT));
    m_tCloudCover->SetValue(GetValue(ClimatologyOverlaySettings::CLOUD));
    m_tPrecipitation->SetValue(GetValue(ClimatologyOverlaySettings::PRECIPITATION));
    m_tRelativeHumidity->SetValue(GetValue(ClimatologyOverlaySettings::RELATIVE_HUMIDITY));
    m_tLightning->SetValue(GetValue(ClimatologyOverlaySettings::LIGHTNING));
    m_tSeaDepth->SetValue(GetValue(ClimatologyOverlaySettings::SEADEPTH));
}

void ClimatologyDialog::PopulateTrackingControls()
{
    SetControlsVisible(ClimatologyOverlaySettings::WIND, m_cbWind, m_tWind, m_tWindDir);
    SetControlsVisible(ClimatologyOverlaySettings::CURRENT, m_cbCurrent, m_tCurrent, m_tCurrentDir);
    bool windorcurrent =
        m_cfgdlg->m_Settings.Settings[ClimatologyOverlaySettings::WIND].m_bEnabled
     || m_cfgdlg->m_Settings.Settings[ClimatologyOverlaySettings::CURRENT].m_bEnabled;
    m_stSpeed->Show(windorcurrent);
    m_stDirection->Show(windorcurrent);

    SetControlsVisible(ClimatologyOverlaySettings::SLP, m_cbPressure, m_tPressure);
    SetControlsVisible(ClimatologyOverlaySettings::SST, m_cbSeaTemperature, m_tSeaTemperature);
    SetControlsVisible(ClimatologyOverlaySettings::AT, m_cbAirTemperature, m_tAirTemperature);
    SetControlsVisible(ClimatologyOverlaySettings::CLOUD, m_cbCloudCover, m_tCloudCover);
    SetControlsVisible(ClimatologyOverlaySettings::PRECIPITATION, m_cbPrecipitation, m_tPrecipitation);
    SetControlsVisible(ClimatologyOverlaySettings::RELATIVE_HUMIDITY, m_cbRelativeHumidity, m_tRelativeHumidity);
    SetControlsVisible(ClimatologyOverlaySettings::LIGHTNING, m_cbLightning, m_tLightning);
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

bool ClimatologyDialog::SettingEnabled(int setting)
{
    return GetSettingControl(setting)->GetValue();
}

void ClimatologyDialog::DisableSetting(int setting)
{
    GetSettingControl(setting)->SetValue(false);
}

wxCheckBox *ClimatologyDialog::GetSettingControl(int setting)
{
    switch(setting) {
    case ClimatologyOverlaySettings::WIND:          return m_cbWind;
    case ClimatologyOverlaySettings::CURRENT:       return m_cbCurrent;
    case ClimatologyOverlaySettings::SLP:           return m_cbPressure;
    case ClimatologyOverlaySettings::SST:           return m_cbSeaTemperature;
    case ClimatologyOverlaySettings::AT:            return m_cbAirTemperature;
    case ClimatologyOverlaySettings::CLOUD:         return m_cbCloudCover;
    case ClimatologyOverlaySettings::PRECIPITATION: return m_cbPrecipitation;
    case ClimatologyOverlaySettings::RELATIVE_HUMIDITY:      return m_cbRelativeHumidity;
    case ClimatologyOverlaySettings::LIGHTNING:     return m_cbLightning;
    case ClimatologyOverlaySettings::SEADEPTH:      return m_cbSeaDepth;
    }
    return NULL;
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
    double val = pPlugIn->GetOverlayFactory()->getCurCalibratedValue
        (coord, index, m_cursorlat, m_cursorlon);
    if(isnan(val))
        return _T("");
    return wxString::Format(_T("%.2f"), val);
}

void ClimatologyDialog::DayMonthUpdate()
{
    wxDateTime &timeline =     pPlugIn->GetOverlayFactory()->m_CurrentTimeline;
    m_sDay->SetRange(1, wxDateTime::GetNumberOfDays((wxDateTime::Month)m_cMonth->GetSelection(),
                                                    1999)); // not a leap year

    timeline.SetMonth((wxDateTime::Month)m_cMonth->GetSelection());
    timeline.SetDay(m_sDay->GetValue());

    int yearday = pPlugIn->GetOverlayFactory()->m_CurrentTimeline.GetDayOfYear();
    if(yearday < 67)
        yearday += 365;
    m_sTimeline->SetValue(yearday);

    UpdateTrackingControls();

//    pPlugIn->GetOverlayFactory()->m_bUpdateCyclones = true;
    RefreshRedraw();
}

void ClimatologyDialog::OnTimeline( wxScrollEvent& event )
{
    wxDateTime &timeline = pPlugIn->GetOverlayFactory()->m_CurrentTimeline;
    timeline.SetToYearDay((event.GetPosition() - 1) % 365 + 1);
    m_cMonth->SetSelection(timeline.GetMonth());

    m_sDay->SetRange(1, wxDateTime::GetNumberOfDays(timeline.GetMonth(),
                                                    1999)); // not a leap year
    m_sDay->SetValue(timeline.GetDay());

    UpdateTrackingControls();
//    pPlugIn->GetOverlayFactory()->m_bUpdateCyclones = true;
    RefreshRedraw();
}

void ClimatologyDialog::OnTimelineDown( wxScrollEvent& event )
{
    if(event.GetPosition() >= 432)
        m_sTimeline->SetValue(event.GetPosition() - 365);
}

void ClimatologyDialog::OnTimelineUp( wxScrollEvent& event )
{
    if(event.GetPosition() <= 67)
        m_sTimeline->SetValue(event.GetPosition() + 365);
}

void ClimatologyDialog::OnAll( wxCommandEvent& event )
{
    m_cMonth->Enable(!m_cbAll->GetValue());
    m_sDay->Enable(!m_cbAll->GetValue());
    m_sTimeline->Enable(!m_cbAll->GetValue());

    pPlugIn->GetOverlayFactory()->m_bAllTimes = event.IsChecked();

    UpdateTrackingControls();
//    pPlugIn->GetOverlayFactory()->m_bUpdateCyclones = true;
    RefreshRedraw();
}

void ClimatologyDialog::OnUpdateDisplay( wxCommandEvent& event )
{
    if(event.IsChecked()) {
        int cursetting = 0;
        for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++)
            if(wxDynamicCast(event.GetEventObject(), wxCheckBox) == GetSettingControl(i)) {
                m_cfgdlg->m_cDataType->SetSelection(i);
                m_cfgdlg->OnDataTypeChoice( event );
                cursetting = i;
                break;
            }

        if(m_cfgdlg->m_Settings.Settings[cursetting].m_bOverlayMap)
            for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++) {
                if(i == cursetting)
                    continue;
                ClimatologyOverlaySettings::OverlayDataSettings &odc = m_cfgdlg->m_Settings.Settings[i];
                if(SettingEnabled(i) && odc.m_bOverlayMap)
                    DisableSetting(i);
            }
    }

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
