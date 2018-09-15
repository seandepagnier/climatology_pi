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
 *
 */

#include "wx/wx.h"
#include "wx/datetime.h"
#include <wx/dcbuffer.h>
#include <wx/dir.h>
#include <wx/debug.h>
#include <wx/graphics.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "climatology_pi.h"
#include "ClimatologyConfigDialog.h"

static const wxString units0_names[] = {"Knots", "M/S", "MPH", "KPH", wxEmptyString};
static const wxString units1_names[] = {"MilliBars", "mmHG", wxEmptyString};
static const wxString units2_names[] = {"mm/day", "in/day", "mm/month", "M/month",
                                        "in/month", "ft/month", "M/year", "in/year",
                                        "ft/year", wxEmptyString};
static const wxString units3_names[] = {"Celcius", "Fahrenheit", wxEmptyString};
static const wxString units4_names[] = {"Percent", wxEmptyString};
static const wxString units5_names[] = {"Unknown", wxEmptyString};
static const wxString units6_names[] = {"Meters", "Feet", wxEmptyString};
static const wxString *unit_names[] = {units0_names, units1_names, units2_names,
                                       units3_names, units4_names, units5_names,
				       units6_names};

static const wxString name_from_index[] = {"Wind", "Current",
                                           "Sea Level Pressure", "Sea Surface Temperature",
                                           "Air Temperature",
                                           "Cloud Cover", "Precipitation",
                                           "Relative Humidity", "Lightning", "Sea Depth"};

static const int unittype[ClimatologyOverlaySettings::SETTINGS_COUNT] = {0, 0, 1, 3, 3, 4, 2, 4, 5, 6};

wxString ClimatologyConfigDialog::SettingName(int setting)
{
    return _(name_from_index[setting]);
}

double ClimatologyOverlaySettings::CalibrationOffset(int setting)
{
    /* only have offset for fahrenheit */
    if(unittype[setting] == 3 && Settings[setting].m_Units == FAHRENHEIT)
        return 32*5/9.0;
    return 0;
}

double ClimatologyOverlaySettings::CalibrationFactor(int setting)
{
    switch(unittype[setting]) {
    case 0: switch(Settings[setting].m_Units) {
        case KNOTS:  return 1;
        case M_S:    return 1.852 / 3.6;
        case MPH:    return 1.15;
        case KPH:    return 1.85;
        } break;
    case 1: switch(Settings[setting].m_Units) {
        case MILLIBARS: return 1;
        case MMHG: return 1 / (1.33);
        } break;
    case 2: {
        double mul = 1;
        switch(Settings[setting].m_Units) {
        case MM_DAY:   case IN_DAY:   mul /= 365.24; break;
        case MM_MONTH: case IN_MONTH: mul /= 12.; break;
        }
        switch(Settings[setting].m_Units) {
        case MM_DAY: case MM_MONTH:               mul *= 1000.; break;
        case IN_DAY: case IN_MONTH: case IN_YEAR: mul *= 39.37; break;
        case FT_MONTH: case FT_YEAR:              mul *= 3.28; break;
        }
        return mul;
    }
    case 3: switch(Settings[setting].m_Units) {
        case CELCIUS:     return 1;
        case FAHRENHEIT: return 9./5;
        } break;
    case 4: return 1;
    case 5: return 1;
    case 6: switch(Settings[setting].m_Units) {
        case METERS: return 1;
        case FEET:   return 3.28;
        } break;
    }
        
    return 1;
}

void ClimatologyOverlaySettings::Load()
{
    /* read settings here */
    wxFileConfig *pConf = GetOCPNConfigObject();
    if (!pConf)
        return;

    pConf->SetPath("/PlugIns/Climatology");

    for(int i=0; i<SETTINGS_COUNT; i++) {
        wxString Name=name_from_index[i];

        int units;
        pConf->Read ( Name +   "Units" , &units, 0);
        Settings[i].m_Units = (SettingsType)units;

        pConf->Read ( Name +   "Enabled" , &Settings[i].m_bEnabled,
                      i == WIND || i == CURRENT );
        pConf->Read ( Name +   "OverlayMap" , &Settings[i].m_bOverlayMap,
                      i == SST || i == AT || i==CLOUD || i == PRECIPITATION
                      || i == RELATIVE_HUMIDITY || i == LIGHTNING || i == SEADEPTH);
        pConf->Read ( Name +   "OverlayTransparency" , &Settings[i].m_iOverlayTransparency,
                      0 );
        pConf->Read ( Name +   "OverlayInterpolation" , &Settings[i].m_bOverlayInterpolation,
                      true );

        pConf->Read ( Name +   "IsoBars" , &Settings[i].m_bIsoBars, i==SLP);
        double defspacing[SETTINGS_COUNT] = {5, 2, 10, 5, 5, 20, 1, 10, 30, 1000};
        pConf->Read ( Name +   "IsoBarSpacing" , &Settings[i].m_iIsoBarSpacing, defspacing[i]);
        pConf->Read ( Name +   "IsoBarStep" , &Settings[i].m_iIsoBarStep, 2);

        for(int m = 0; m<13; m++)
            Settings[i].m_pIsobars[m] = NULL;

        pConf->Read ( Name +   "Numbers" , &Settings[i].m_bNumbers, 0);
        pConf->Read ( Name +   "NumbersSpacing" , &Settings[i].m_iNumbersSpacing, 50);

        if(i > CURRENT) {
            Settings[i].m_bDirectionArrows = false;
            continue;
        }

        pConf->Read ( Name +   "DirectionArrows" , &Settings[i].m_bDirectionArrows, i==CURRENT);
        pConf->Read ( Name +   "DirectionArrowsLengthType" , &Settings[i].m_iDirectionArrowsLengthType, 1);
        pConf->Read ( Name +   "DirectionArrowsWidth" , &Settings[i].m_iDirectionArrowsWidth, 2);

        wxString DirectionArrowsColorStr;
        wxString defarrowcolor[CURRENT+1] = {"#0022ff", "#c51612"};
        pConf->Read(Name + "DirectionArrowsColor", &DirectionArrowsColorStr, defarrowcolor[i]);
        Settings[i].m_cDirectionArrowsColor = wxColour(DirectionArrowsColorStr);

        int opacity;
        pConf->Read ( Name +   "DirectionArrowsOpacity" , &opacity, 205);
        Settings[i].m_cDirectionArrowsColor.Set(Settings[i].m_cDirectionArrowsColor.Red(),
                                                Settings[i].m_cDirectionArrowsColor.Green(),
                                                Settings[i].m_cDirectionArrowsColor.Blue(),
                                                opacity);

        double defarrowsize[CURRENT+1] = {10, 7};
        pConf->Read ( Name +   "DirectionArrowsSize" , &Settings[i].m_iDirectionArrowsSize,
                      defarrowsize[i]);

        double defarrowspacing[CURRENT+1] = {24, 18};
        pConf->Read ( Name +   "DirectionArrowsSpacing" , &Settings[i].m_iDirectionArrowsSpacing,
                      defarrowspacing[i]);
    }
}

void ClimatologyOverlaySettings::Save()
{
    /* save settings here */
    wxFileConfig *pConf = GetOCPNConfigObject();
    if (!pConf)
        return;

    pConf->SetPath (  "/PlugIns/Climatology"  );

    for(int i=0; i<SETTINGS_COUNT; i++) {
        wxString Name=name_from_index[i];

        pConf->Write ( Name +   "Units" , (int)Settings[i].m_Units);
        pConf->Write ( Name +   "Enabled" , Settings[i].m_bEnabled);
        pConf->Write ( Name +   "OverlayMap" , Settings[i].m_bOverlayMap);
        pConf->Write ( Name +   "OverlayTransparency" , Settings[i].m_iOverlayTransparency);
        pConf->Write ( Name +   "OverlayInterpolation" , Settings[i].m_bOverlayInterpolation);
        pConf->Write ( Name +   "IsoBars" , Settings[i].m_bIsoBars);
        pConf->Write ( Name +   "IsoBarSpacing" , Settings[i].m_iIsoBarSpacing);
        pConf->Write ( Name +   "IsoBarStep" , Settings[i].m_iIsoBarStep);
        pConf->Write ( Name +   "Numbers" , Settings[i].m_bNumbers);
        pConf->Write ( Name +   "NumbersSpacing" , Settings[i].m_iNumbersSpacing);

        if(i > CURRENT)
            continue;

        pConf->Write ( Name +   "DirectionArrows" , Settings[i].m_bDirectionArrows);
        pConf->Write ( Name +   "DirectionArrowsLengthType" , Settings[i].m_iDirectionArrowsLengthType);
        pConf->Write ( Name +   "DirectionArrowsWidth" , Settings[i].m_iDirectionArrowsWidth);

        wxString DirectionArrowsColorStr = Settings[i].m_cDirectionArrowsColor.GetAsString();
        pConf->Write( Name + "DirectionArrowsColor", DirectionArrowsColorStr);
        pConf->Write ( Name +   "DirectionArrowsOpacity" ,
                       Settings[i].m_cDirectionArrowsColor.Alpha());
        pConf->Write ( Name +   "DirectionArrowsSize" , Settings[i].m_iDirectionArrowsSize);
        pConf->Write ( Name +   "DirectionArrowsSpacing" , Settings[i].m_iDirectionArrowsSpacing);
    }
}

ClimatologyConfigDialog::ClimatologyConfigDialog(ClimatologyDialog *parent)
#ifndef __WXOSX__
    : ClimatologyConfigDialogBase(parent)
#else
    : ClimatologyConfigDialogBase(parent, wxID_ANY, _("Climatology Configuration"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxSTAY_ONOP)
#endif
{
    pParent = parent;

#ifdef WIN32 // windows is too slow for very fine steps
    m_cIsoBarStep->Delete(4);
#endif
#ifdef __OCPN__ANDROID__
    GetHandle()->setStyleSheet( qtStyleSheet);
#endif


    m_Settings.Load();
    LoadSettings();

    m_cDataType->Clear();
    for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++)
        m_cDataType->Append(_(name_from_index[i]));

    m_cDataType->SetSelection(m_lastdatatype);
    PopulateUnits(m_lastdatatype);
    ReadDataTypeSettings(m_lastdatatype);

    m_stVersion->SetLabel(wxString::Format("%d.%d",
                                           PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR));
    m_tDataDirectory->SetValue(ClimatologyDataDirectory());

    m_refreshTimer.Connect(wxEVT_TIMER, wxTimerEventHandler(ClimatologyConfigDialog::OnRefreshTimer) , NULL, this);

    DimeWindow( this );
}

ClimatologyConfigDialog::~ClimatologyConfigDialog()
{
    m_Settings.Save();
    SaveSettings();
}

void ClimatologyConfigDialog::DisableIsoBars(int setting)
{
    m_Settings.Settings[setting].m_bIsoBars = false;
    
    if(setting == m_cDataType->GetSelection())
        m_cbIsoBars->SetValue(false);
}

void ClimatologyConfigDialog::LoadSettings()
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    assert(pConf != NULL);

    pConf->SetPath (   "/Settings/Climatology"  );

    pConf->Read (   "lastdatatype" , &m_lastdatatype, 0);

    /* wind atlas settings */
    pConf->SetPath (  "/PlugIns/Climatology/WindAtlas"  );

    m_cbWindAtlasEnable->SetValue((bool)pConf->Read (   "Enabled" , true));
    m_sWindAtlasSize->SetValue(pConf->Read (   "Size1" , 120L));
    m_sWindAtlasSpacing->SetValue(pConf->Read (   "Spacing1" , 90L));
    m_sWindAtlasOpacity->SetValue(pConf->Read (   "Opacity" , 205L));

    /* cyclone settings */
    pConf->SetPath (  "/PlugIns/Climatology/Cyclones"  );

    wxDateTime StartDate = wxDateTime::Now();
    StartDate.SetYear(1972);

    wxString StartDateString = StartDate.FormatDate();
    pConf->Read(   "StartDate" , &StartDateString, StartDateString);
    StartDate.ParseDate(StartDateString);
    m_dPStart->SetValue(StartDate);

    wxDateTime now = wxDateTime::Now();
    wxString EndDateString = now.FormatDate();
    pConf->Read(   "EndDate" , &EndDateString, EndDateString);
    wxDateTime EndDate;
    EndDate.ParseDate(EndDateString);
    if(EndDate.GetYear() > now.GetYear())
        EndDate = now;
    m_dPEnd->SetValue(EndDate);

    m_sCycloneDaySpan->SetValue(pConf->Read (   "CycloneDaySpan" , 30L ));
    m_sMinWindSpeed->SetValue(pConf->Read (   "MinWindSpeed" , 35L ));
    m_sMaxPressure->SetValue(pConf->Read (   "MaxPressure" , 1080L ));

    /* implement check boxes too? */
}

void ClimatologyConfigDialog::SaveSettings()
{
    wxFileConfig *pConf = GetOCPNConfigObject();
    if (!pConf)
        return;

    pConf->SetPath (   "/Settings/Climatology"  );

    pConf->Write (   "lastdatatype" , m_lastdatatype);

    pConf->SetPath (  "/PlugIns/Climatology/WindAtlas"  );

    /* wind atlas settings */
    pConf->Write (   "Enabled" , m_cbWindAtlasEnable->GetValue());
    pConf->Write (   "Size1" , m_sWindAtlasSize->GetValue());
    pConf->Write (   "Spacing1" , m_sWindAtlasSpacing->GetValue());
    pConf->Write (   "Opacity" , m_sWindAtlasOpacity->GetValue());

    /* cyclone settings */
    pConf->SetPath (  "/PlugIns/Climatology/Cyclones"  );

    pConf->Write(   "StartDate" , m_dPStart->GetDateCtrlValue().FormatDate());
    pConf->Write(   "EndDate" , m_dPEnd->GetDateCtrlValue().FormatDate());

    pConf->Write (   "CycloneDaySpan" , m_sCycloneDaySpan->GetValue() );
    pConf->Write (   "MinWindSpeed" , m_sMinWindSpeed->GetValue() );
    pConf->Write (   "MaxPressure" , m_sMaxPressure->GetValue() );

    /* implement check boxes too? */
}

void ClimatologyConfigDialog::SetDataTypeSettings(int settings)
{
    ClimatologyOverlaySettings::OverlayDataSettings &odc = m_Settings.Settings[settings];
    odc.m_Units = m_cDataUnits->GetSelection();
    odc.m_bEnabled = m_cbEnabled->GetValue();
    odc.m_bOverlayMap = m_cbOverlayMap->GetValue();
    odc.m_iOverlayTransparency = m_sOverlayTransparency->GetValue();
    odc.m_bOverlayInterpolation = m_cbOverlayInterpolation->GetValue();
    odc.m_bIsoBars = m_cbIsoBars->GetValue();
    odc.m_iIsoBarSpacing = m_sIsoBarSpacing->GetValue();
    odc.m_iIsoBarStep = m_cIsoBarStep->GetSelection();
    odc.m_bNumbers = m_cbNumbers->GetValue();
    odc.m_iNumbersSpacing = m_sNumbersSpacing->GetValue();

    if(settings > ClimatologyOverlaySettings::CURRENT)
        return;

    odc.m_bDirectionArrows = m_cbDirectionArrowsEnable->GetValue();
    odc.m_iDirectionArrowsLengthType = m_rbDirectionArrowsLength->GetValue();
    odc.m_iDirectionArrowsWidth = m_sDirectionArrowsWidth->GetValue();
    wxColour c = m_cpDirectionArrows->GetColour();
    odc.m_cDirectionArrowsColor.Set(c.Red(), c.Green(), c.Blue(),
                                    m_sDirectionArrowsOpacity->GetValue());
    odc.m_iDirectionArrowsSize = m_sDirectionArrowsSize->GetValue();
    odc.m_iDirectionArrowsSpacing = m_sDirectionArrowsSpacing->GetValue();
}

void ClimatologyConfigDialog::ReadDataTypeSettings(int settings)
{
    ClimatologyOverlaySettings::OverlayDataSettings &odc = m_Settings.Settings[settings];
    m_cDataUnits->SetSelection(odc.m_Units);
    m_cbEnabled->SetValue(odc.m_bEnabled);
    m_cbOverlayMap->SetValue(odc.m_bOverlayMap);
    m_sOverlayTransparency->SetValue(odc.m_iOverlayTransparency);
    m_cbOverlayInterpolation->SetValue(odc.m_bOverlayInterpolation);
    m_cbIsoBars->SetValue(odc.m_bIsoBars);
    m_sIsoBarSpacing->SetValue(odc.m_iIsoBarSpacing);
    m_cIsoBarStep->SetSelection(odc.m_iIsoBarStep);
    m_cbNumbers->SetValue(odc.m_bNumbers);
    m_sNumbersSpacing->SetValue(odc.m_iNumbersSpacing);

    bool enable = settings <= ClimatologyOverlaySettings::CURRENT;
    m_cbDirectionArrowsEnable->Enable(enable);
    m_rbDirectionArrowsBarbs->Enable(enable);
    m_rbDirectionArrowsLength->Enable(enable);
    m_sDirectionArrowsWidth->Enable(enable);
    m_cpDirectionArrows->Enable(enable);
    m_sDirectionArrowsOpacity->Enable(enable);
    m_sDirectionArrowsSize->Enable(enable);
    m_sDirectionArrowsSpacing->Enable(enable);

    if(!enable)
        return;

    m_cbDirectionArrowsEnable->SetValue(odc.m_bDirectionArrows);
    m_rbDirectionArrowsBarbs->SetValue(odc.m_iDirectionArrowsLengthType==0);
    m_rbDirectionArrowsLength->SetValue(odc.m_iDirectionArrowsLengthType==1);
    m_sDirectionArrowsWidth->SetValue(odc.m_iDirectionArrowsWidth);
    m_cpDirectionArrows->SetColour(odc.m_cDirectionArrowsColor);
    m_sDirectionArrowsOpacity->SetValue(odc.m_cDirectionArrowsColor.Alpha());
    m_sDirectionArrowsSize->SetValue(odc.m_iDirectionArrowsSize);
    m_sDirectionArrowsSpacing->SetValue(odc.m_iDirectionArrowsSpacing);
}

void ClimatologyConfigDialog::PopulateUnits(int settings)
{
    m_cDataUnits->Clear();
    for(int i=0; !unit_names[unittype[settings]][i].empty(); i++)
        m_cDataUnits->Append(_(unit_names[unittype[settings]][i]));
}

void ClimatologyConfigDialog::OnPageChanged( wxNotebookEvent& event )
{
    /* delay loading html until last moment because it can take a few seconds */
    if(event.GetSelection() == 3)
        m_htmlInformation->LoadFile(ClimatologyDataDirectory() + _("ClimatologyInformation.html"));
    event.Skip();
}

void ClimatologyConfigDialog::OnDataTypeChoice( wxCommandEvent& event )
{
    m_lastdatatype = m_cDataType->GetSelection();
    PopulateUnits(m_lastdatatype);
    ReadDataTypeSettings(m_lastdatatype);
}

void ClimatologyConfigDialog::OnUpdate()
{
    int setting = m_cDataType->GetSelection();
    SetDataTypeSettings(setting);

    m_refreshTimer.Start(200, true);
}

void ClimatologyConfigDialog::OnUpdateOverlayConfig( wxCommandEvent& event )
{
    // must disable all others to have only 1 overlay at a time
    int cursetting = m_cDataType->GetSelection();
    if(event.IsChecked() && m_cbEnabled->GetValue() && pParent->SettingEnabled(cursetting)) {
        for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++) {
            if(i == cursetting)
                continue;
            ClimatologyOverlaySettings::OverlayDataSettings &odc = m_Settings.Settings[i];
            if(pParent->SettingEnabled(i) && odc.m_bOverlayMap)
                pParent->DisableSetting(i);
        }
    }
    OnUpdate();
}

void ClimatologyConfigDialog::OnUpdateIsobar()
{
#ifdef WIN32 // windows is too slow to have interactive update
    m_cbIsoBars->SetValue(false);
#endif
    OnUpdate();
}

void ClimatologyConfigDialog::OnUpdateCyclones()
{
    g_pOverlayFactory->BuildCycloneCache();
    OnUpdate();
}

void ClimatologyConfigDialog::OnPaintKey( wxPaintEvent& event )
{
    wxWindow *window = dynamic_cast<wxWindow*>(event.GetEventObject());
    if(!window) {
        // broken wxqt hack!!
        static int panel;
        wxWindow *panels[] = {m_panel51, m_panel511, m_panel5111, m_panel51111,
                             m_panel511111, m_panel5111111, m_panel51111111,
                             m_panel511111111, m_panel5111111111};
        window = panel[panels];
        if(++panel==(sizeof panels) / (sizeof *panels))
            panel = 0;
    }
    
    wxPaintDC dc( window );

    double knots;
    wxString name = window->GetName();

    window->GetName().ToDouble(&knots);

    wxColour c = ClimatologyOverlayFactory::GetGraphicColor(CYCLONE_SETTING, knots);

    dc.SetBackground(c);
    dc.Clear();
}

void ClimatologyConfigDialog::OnEnabled( wxCommandEvent& event )
{
    OnUpdate();
    pParent->PopulateTrackingControls();
}

void ClimatologyConfigDialog::OnAboutAuthor( wxCommandEvent& event )
{
    wxLaunchDefaultBrowser(ABOUT_AUTHOR_URL);
}

void ClimatologyConfigDialog::OnRefreshTimer( wxTimerEvent& event )
{
    pParent->RefreshRedraw();
}
