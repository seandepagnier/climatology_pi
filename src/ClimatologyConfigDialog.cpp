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
static const wxString units2_names[] = {_("Meters"), _("Feet"), wxEmptyString};
static const wxString units3_names[] = {_("Celcius"), _("Farenheight"), wxEmptyString};
static const wxString units4_names[] = {_("Millimeters"), _("Inches"), wxEmptyString};
static const wxString units5_names[] = {_("Percentage"), wxEmptyString};
static const wxString *unit_names[] = {units0_names, units1_names, units2_names,
                                       units3_names, units4_names, units5_names};

static const wxString name_from_index[] = {_T("Wind"), _T("Current"),
                                           _T("SeaLevelPressure"), _T("SeaSurfaceTemperature")};
static const wxString tname_from_index[] = {_("Wind"), _("Current"),
                                            _("Sea Level Pressure"), _("Sea Surface Temperature")};

static const int unittype[ClimatologyOverlaySettings::SETTINGS_COUNT] = {0, 0, 1, 3};

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
        pConf->Read ( Name + _T ( "Units" ), &units);
        Settings[i].m_Units = (SettingsType)units;

        pConf->Read ( Name + _T ( "IsoBars" ), &Settings[i].m_bIsoBars, i==SLP);
        double defspacing[SETTINGS_COUNT] = {10, 1, 10, 1};
        pConf->Read ( Name + _T ( "IsoBarSpacing" ), &Settings[i].m_iIsoBarSpacing, defspacing[i]);
        Settings[i].m_pIsobarArray = NULL;

        pConf->Read ( Name + _T ( "OverlayMap" ), &Settings[i].m_bOverlayMap, 0);

        pConf->Read ( Name + _T ( "Numbers" ), &Settings[i].m_bNumbers, i==SST);
        pConf->Read ( Name + _T ( "NumbersSpacing" ), &Settings[i].m_iNumbersSpacing, 50);
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
        pConf->Write ( Name + _T ( "IsoBars" ), Settings[i].m_bIsoBars);
        pConf->Write ( Name + _T ( "IsoBarSpacing" ), Settings[i].m_iIsoBarSpacing);
        pConf->Write ( Name + _T ( "OverlayMap" ), Settings[i].m_bOverlayMap);
        pConf->Write ( Name + _T ( "Numbers" ), Settings[i].m_bNumbers);
        pConf->Write ( Name + _T ( "NumbersSpacing" ), Settings[i].m_iNumbersSpacing);
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
 
    for(int i=0; i<ClimatologyOverlaySettings::SETTINGS_COUNT; i++)
        m_cDataType->Append(tname_from_index[i]);

    m_cDataType->SetSelection(m_lastdatatype);
    PopulateUnits(m_lastdatatype);
    ReadDataTypeSettings(m_lastdatatype);


    wxDateTime dt(1, 1, 1900);
    m_dPStart->SetValue(dt);

    DimeWindow( this );
}


ClimatologyConfigDialog::~ClimatologyConfigDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();

    if(!pConf)
        return;

    pConf->SetPath ( _T ( "/Settings/Climatology" ) );

    pConf->Write ( _T ( "lastdatatype" ), m_lastdatatype);

    m_Settings.Write();
}

void ClimatologyConfigDialog::SetDataTypeSettings(int settings)
{
    ClimatologyOverlaySettings::OverlayDataSettings &odc = m_Settings.Settings[settings];
    odc.m_Units = m_cDataUnits->GetSelection();
    odc.m_bIsoBars = m_cbIsoBars->GetValue();
    odc.m_iIsoBarSpacing = m_sIsoBarSpacing->GetValue();
    odc.m_bOverlayMap = m_cbOverlayMap->GetValue();
    odc.m_bNumbers = m_cbNumbers->GetValue();
    odc.m_iNumbersSpacing = m_sNumbersSpacing->GetValue();
}

void ClimatologyConfigDialog::ReadDataTypeSettings(int settings)
{
    ClimatologyOverlaySettings::OverlayDataSettings &odc = m_Settings.Settings[settings];
    m_cDataUnits->SetSelection(odc.m_Units);
    m_cbIsoBars->SetValue(odc.m_bIsoBars);
    m_sIsoBarSpacing->SetValue(odc.m_iIsoBarSpacing);
    m_cbOverlayMap->SetValue(odc.m_bOverlayMap);
    m_cbNumbers->SetValue(odc.m_bNumbers);
    m_sNumbersSpacing->SetValue(odc.m_iNumbersSpacing);
}

void ClimatologyConfigDialog::PopulateUnits(int settings)
{
    m_cDataUnits->Clear();
    for(int i=0; !unit_names[unittype[settings]][i].empty(); i++)
        m_cDataUnits->Append(unit_names[unittype[settings]][i]);
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
    pParent->SetFactoryOptions();
}

void ClimatologyConfigDialog::OnCycloneConfig()
{
    pParent->SetFactoryOptions();
}
