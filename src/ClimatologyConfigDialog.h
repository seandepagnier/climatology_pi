/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Climatology Plugin Friends
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

#ifndef __ClimatologyConfigDIALOG_H__
#define __ClimatologyConfigDIALOG_H__

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ClimatologyUI.h"

struct ClimatologyOverlaySettings
{
    void Read();
    void Write();

    enum SettingsType {WIND, CURRENT, SLP, SST, SETTINGS_COUNT};

    struct OverlayDataSettings {
        int m_Units;

        bool m_bIsoBars;
        int m_iIsoBarSpacing;
        wxArrayPtrVoid *m_pIsobarArray;

        bool m_bOverlayMap;

        bool m_bNumbers;
        double m_iNumbersSpacing;
    } Settings[SETTINGS_COUNT];
};

class ClimatologyDialog;

class ClimatologyConfigDialog : public ClimatologyConfigDialogBase {
public:

    ClimatologyConfigDialog(ClimatologyDialog *parent);
    ~ClimatologyConfigDialog();

    ClimatologyOverlaySettings m_Settings;

private:

    void SetDataTypeSettings(int settings);
    void ReadDataTypeSettings(int settings);
    void PopulateUnits(int settings);
    void OnDataTypeChoice( wxCommandEvent& event );

    void OnUpdate();
    void OnUpdate( wxCommandEvent& event ) { OnUpdate(); }
    void OnUpdate( wxSpinEvent& event ) { OnUpdate(); }

    void OnCycloneConfig();
    void OnCycloneConfig( wxCommandEvent& event ) { OnCycloneConfig(); }
    void OnCycloneConfig( wxDateEvent& event ) { OnCycloneConfig(); }
    void OnCycloneConfig( wxSpinEvent& event ) { OnCycloneConfig(); }

    int m_lastdatatype;

    
    ClimatologyDialog *pParent;
};

#endif

