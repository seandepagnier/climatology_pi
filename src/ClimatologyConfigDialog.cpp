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
#include "ClimatologyConfigDialog.h"

static const wxString units0_names[] = {_("Knots"), _("M/S"), _("MPH"), _("KPH"), wxEmptyString};
static const wxString units1_names[] = {_("MilliBars"), _("mmHG"), wxEmptyString};
static const wxString units2_names[] = {_("Meters"), _("Feet"), _("Inches"), wxEmptyString};
static const wxString units3_names[] = {_("Celcius"), _("Fahrenheit"), wxEmptyString};
static const wxString units4_names[] = {_("Percent"), wxEmptyString};
static const wxString units5_names[] = {_("Unknown"), wxEmptyString};
static const wxString *unit_names[] = {units0_names, units1_names, units2_names,
                                       units3_names, units4_names, units5_names};

static const wxString name_from_index[] = {_T("Wind"), _T("Current"),
                                           _T("SeaLevelPressure"), _T("SeaSurfaceTemperature"),
                                           _T("AirTemperature"),
                                           _T("CloudCover"), _T("Precipitation"),
                                           _T("RelativeHumidity"), _T("Depth")};
static const wxString tname_from_index[] = {_("Wind"), _("Current"),
                                            _("Sea Level Pressure"), _("Sea Surface Temperature"),
                                            _T("Air Temperature"),
                                            _("Cloud Cover"), _("Precipitation"),
                                            _("Relative Humidity"), _("Sea Depth")};

static const int unittype[ClimatologyOverlaySettings::SETTINGS_COUNT] = {0, 0, 1, 3, 3, 4, 2, 4, 2};

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
    case 2: switch(Settings[setting].m_Units) {
        case METERS: return 1;
        case FEET:   return 3.28;
        case INCHES: return 39.37;
        } break;
    case 3: switch(Settings[setting].m_Units) {
        case CELCIUS:     return 1;
        case FAHRENHEIT: return 9./5;
        } break;
    case 4: switch(Settings[setting].m_Units) {
        case METERS: return 1;
        case INCHES: return 39.37;
        } break;
    case 5: return 1;
    }
        
    return 1;
}

void ClimatologyOverlaySettings::Read()
{
    /* read settings here */
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return;

    pConf->SetPath ( _T( "/PlugIns/Climatology" ) );

    for(int i=0; i<SETTINGS_COUNT; i++) {
        wxString Name=name_from_index[i];

        int units;
        pConf->Read ( Name + _T ( "Units" ), &units, 0);
        Settings[i].m_Units = (SettingsType)units;

        pConf->Read ( Name + _T ( "Enabled" ), &Settings[i].m_bEnabled,
                      i == WIND || i == CURRENT );
        pConf->Read ( Name + _T ( "OverlayMap" ), &Settings[i].m_bOverlayMap,
                      i == SST || i == AT || i==CLOUD || i == PRECIPITATION
                      || i == RELATIVE_HUMIDITY || i == SEADEPTH);
        pConf->Read ( Name + _T ( "OverlayTransparency" ), &Settings[i].m_iOverlayTransparency,
                      0 );

        pConf->Read ( Name + _T ( "IsoBars" ), &Settings[i].m_bIsoBars, i==SLP);
        double defspacing[SETTINGS_COUNT] = {5, 2, 10, 5, 5, 20, 1, 10, 1000};
        pConf->Read ( Name + _T ( "IsoBarSpacing" ), &Settings[i].m_iIsoBarSpacing, defspacing[i]);
        pConf->Read ( Name + _T ( "IsoBarStep" ), &Settings[i].m_iIsoBarStep, 2);

        for(int m = 0; m<13; m++)
            Settings[i].m_pIsobars[m] = NULL;

        pConf->Read ( Name + _T ( "Numbers" ), &Settings[i].m_bNumbers, 0);
        pConf->Read ( Name + _T ( "NumbersSpacing" ), &Settings[i].m_iNumbersSpacing, 50);

        if(i > CURRENT)
            continue;

        pConf->Read ( Name + _T ( "DirectionArrows" ), &Settings[i].m_bDirectionArrows, i==CURRENT);
        pConf->Read ( Name + _T ( "DirectionArrowsLengthType" ), &Settings[i].m_iDirectionArrowsLengthType, 1);
        pConf->Read ( Name + _T ( "DirectionArrowsWidth" ), &Settings[i].m_iDirectionArrowsWidth, 2);

        wxString DirectionArrowsColorStr;
        wxString defarrowcolor[CURRENT+1] = {_T("#0022ff"), _T("#c51612")};
        pConf->Read(Name + _T("DirectionArrowsColor"), &DirectionArrowsColorStr, defarrowcolor[i]);
        Settings[i].m_cDirectionArrowsColor = wxColour(DirectionArrowsColorStr);

        pConf->Read ( Name + _T ( "DirectionArrowsOpacity" ), &Settings[i].m_iDirectionArrowsOpacity, 205);

        double defarrowsize[CURRENT+1] = {10, 7};
        pConf->Read ( Name + _T ( "DirectionArrowsSize" ), &Settings[i].m_iDirectionArrowsSize,
                      defarrowsize[i]);

        double defarrowspacing[CURRENT+1] = {24, 18};
        pConf->Read ( Name + _T ( "DirectionArrowsSpacing" ), &Settings[i].m_iDirectionArrowsSpacing,
                      defarrowspacing[i]);
    }
}

void ClimatologyOverlaySettings::Write()
{
    /* save settings here */
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return;

    pConf->SetPath ( _T( "/PlugIns/Climatology" ) );

    for(int i=0; i<SETTINGS_COUNT; i++) {
        wxString Name=name_from_index[i];

        pConf->Write ( Name + _T ( "Units" ), (int)Settings[i].m_Units);
        pConf->Write ( Name + _T ( "Enabled" ), Settings[i].m_bEnabled);
        pConf->Write ( Name + _T ( "OverlayMap" ), Settings[i].m_bOverlayMap);
        pConf->Write ( Name + _T ( "OverlayTransparency" ), Settings[i].m_iOverlayTransparency);
        pConf->Write ( Name + _T ( "IsoBars" ), Settings[i].m_bIsoBars);
        pConf->Write ( Name + _T ( "IsoBarSpacing" ), Settings[i].m_iIsoBarSpacing);
        pConf->Write ( Name + _T ( "IsoBarStep" ), Settings[i].m_iIsoBarStep);
        pConf->Write ( Name + _T ( "Numbers" ), Settings[i].m_bNumbers);
        pConf->Write ( Name + _T ( "NumbersSpacing" ), Settings[i].m_iNumbersSpacing);

        if(i > CURRENT)
            continue;

        pConf->Write ( Name + _T ( "DirectionArrows" ), Settings[i].m_bDirectionArrows);
        pConf->Write ( Name + _T ( "DirectionArrowsLengthType" ), Settings[i].m_iDirectionArrowsLengthType);
        pConf->Write ( Name + _T ( "DirectionArrowsWidth" ), Settings[i].m_iDirectionArrowsWidth);

        wxString DirectionArrowsColorStr = Settings[i].m_cDirectionArrowsColor.GetAsString();
        pConf->Write( Name + _T("DirectionArrowsColor"), DirectionArrowsColorStr);

        pConf->Write ( Name + _T ( "DirectionArrowsOpacity" ), Settings[i].m_iDirectionArrowsOpacity);
        pConf->Write ( Name + _T ( "DirectionArrowsSize" ), Settings[i].m_iDirectionArrowsSize);
        pConf->Write ( Name + _T ( "DirectionArrowsSpacing" ), Settings[i].m_iDirectionArrowsSpacing);
    }
}

ClimatologyConfigDialog::ClimatologyConfigDialog(ClimatologyDialog *parent)
  : ClimatologyConfigDialogBase(parent)
{
    pParent = parent;

    m_Settings.Read();

    wxFileConfig *pConf = GetOCPNConfigObject();;
    if(!pConf)
        return;

    pConf->SetPath ( _T ( "/Settings/Climatology" ) );
    pConf->Read ( _T ( "lastdatatype" ), &m_lastdatatype, 0);

    bool bWindAtlas;
    pConf->Read ( _T ( "WindAtlas" ), &bWindAtlas, 1);
    m_cbWindAtlasEnable->SetValue(bWindAtlas);

    int iWindAtlasSize;
    pConf->Read ( _T ( "WindAtlasSize" ), &iWindAtlasSize, 100);
    m_sWindAtlasSize->SetValue(iWindAtlasSize);

    int iWindAtlasSpacing;
    pConf->Read ( _T ( "WindAtlasSpacing" ), &iWindAtlasSpacing, 100);
    m_sWindAtlasSpacing->SetValue(iWindAtlasSpacing);

    int iWindAtlasOpacity;
    pConf->Read ( _T ( "WindAtlasOpacity" ), &iWindAtlasOpacity, 205);
    m_sWindAtlasOpacity->SetValue(iWindAtlasOpacity);
 
    for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++)
        m_cDataType->Append(SettingName(i));

    m_cDataType->SetSelection(m_lastdatatype);
    PopulateUnits(m_lastdatatype);
    ReadDataTypeSettings(m_lastdatatype);

    wxDateTime dt = wxDateTime::Now();
#ifdef __MSVC__
    dt.SetYear(1972);
#else
    dt.SetYear(1945);
#endif
    m_dPStart->SetValue(dt);

    m_stVersion->SetLabel(wxString::Format(_T("%d.%d"),
                                           PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR));
    m_tDataDirectory->SetValue(ClimatologyDataDirectory());

    DimeWindow( this );
}

ClimatologyConfigDialog::~ClimatologyConfigDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return;

    pConf->SetPath ( _T ( "/Settings/Climatology" ) );

    pConf->Write ( _T ( "lastdatatype" ), m_lastdatatype);

    pConf->Write ( _T ( "WindAtlas" ), m_cbWindAtlasEnable->GetValue());
    pConf->Write ( _T ( "WindAtlasSize" ), m_sWindAtlasSize->GetValue());
    pConf->Write ( _T ( "WindAtlasSpacing" ), m_sWindAtlasSpacing->GetValue());
    pConf->Write ( _T ( "WindAtlasOpacity" ), m_sWindAtlasOpacity->GetValue());

    m_Settings.Write();
}

wxString ClimatologyConfigDialog::SettingName(int setting)
{
    return tname_from_index[setting];
}

void ClimatologyConfigDialog::DisableIsoBars(int setting)
{
    m_Settings.Settings[setting].m_bIsoBars = false;
    
    if(setting == m_cDataType->GetSelection())
        m_cbIsoBars->SetValue(false);
}

void ClimatologyConfigDialog::SetDataTypeSettings(int settings)
{
    ClimatologyOverlaySettings::OverlayDataSettings &odc = m_Settings.Settings[settings];
    odc.m_Units = m_cDataUnits->GetSelection();
    odc.m_bEnabled = m_cbEnabled->GetValue();
    odc.m_bOverlayMap = m_cbOverlayMap->GetValue();
    odc.m_iOverlayTransparency = m_sOverlayTransparency->GetValue();
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
    odc.m_cDirectionArrowsColor = m_cpDirectionArrows->GetColour();
    odc.m_iDirectionArrowsOpacity = m_sDirectionArrowsOpacity->GetValue();
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
    m_sDirectionArrowsOpacity->SetValue(odc.m_iDirectionArrowsOpacity);
    m_sDirectionArrowsSize->SetValue(odc.m_iDirectionArrowsSize);
    m_sDirectionArrowsSpacing->SetValue(odc.m_iDirectionArrowsSpacing);
}

void ClimatologyConfigDialog::PopulateUnits(int settings)
{
    m_cDataUnits->Clear();
    for(int i=0; !unit_names[unittype[settings]][i].empty(); i++)
        m_cDataUnits->Append(unit_names[unittype[settings]][i]);
}

void ClimatologyConfigDialog::OnPageChanged( wxNotebookEvent& event )
{
    /* delay loading html until last moment because it can take a few seconds */
    if(event.GetSelection() == 3)
        m_htmlInformation->LoadFile(ClimatologyDataDirectory() + _T("ClimatologyInformation.html"));
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
    pParent->RefreshRedraw();
}

void ClimatologyConfigDialog::OnEnabled( wxCommandEvent& event )
{
    OnUpdate();
    pParent->PopulateTrackingControls();
}

void ClimatologyConfigDialog::OnDonate( wxCommandEvent& event )
{
      wxLaunchDefaultBrowser(_T("https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=sean%40depagnier%2ecom&lc=US&item_name=climatology&no_note=0&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHostedGuest"));
}
