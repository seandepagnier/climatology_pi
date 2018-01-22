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
 */

#ifndef __ClimatologyConfigDIALOG_H__
#define __ClimatologyConfigDIALOG_H__

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ClimatologyUI.h"

class ClimatologyIsoBarMap;

struct ClimatologyOverlaySettings
{
    double CalibrationOffset(int settings);
    double CalibrationFactor(int settings);
    double CalibrateValue(int setting, double v) {
        return CalibrationFactor(setting)*(v + CalibrationOffset(setting));
    }

    void Load();
    void Save();

    enum SettingsType {WIND, CURRENT, SLP, SST, AT, CLOUD, PRECIPITATION,
                       RELATIVE_HUMIDITY, LIGHTNING, SEADEPTH, SETTINGS_COUNT};
    enum Units0 {KNOTS, M_S, MPH, KPH};
    enum Units1 {MILLIBARS, MMHG};
    enum Units2 {MM_DAY, IN_DAY, MM_MONTH, M_MONTH, IN_MONTH, FT_MONTH, M_YEAR, IN_YEAR, FT_YEAR};
    enum Units3 {CELCIUS, FAHRENHEIT};
    enum Units4 {PERCENTAGE};
    enum Units6 {METERS, FEET};

    struct OverlayDataSettings {
        int m_Units;

        bool m_bEnabled;

        bool m_bOverlayMap;
        int m_iOverlayTransparency;
        bool m_bOverlayInterpolation;

        bool m_bIsoBars;
        int m_iIsoBarSpacing, m_iIsoBarStep;
        ClimatologyIsoBarMap *m_pIsobars[13];

        bool m_bNumbers;
        double m_iNumbersSpacing;

        bool m_bDirectionArrows;
        int m_iDirectionArrowsLengthType, m_iDirectionArrowsWidth;
        wxColour m_cDirectionArrowsColor;
        int m_iDirectionArrowsSize, m_iDirectionArrowsSpacing;
    } Settings[SETTINGS_COUNT];
};

class ClimatologyDialog;

class ClimatologyConfigDialog : public ClimatologyConfigDialogBase {
public:

    ClimatologyConfigDialog(ClimatologyDialog *parent);
    ~ClimatologyConfigDialog();

    wxString SettingName(int setting);
    void DisableIsoBars(int setting);

    void OnDataTypeChoice( wxCommandEvent& event );

    ClimatologyOverlaySettings m_Settings;

private:
    void LoadSettings();
    void SaveSettings();

    void SetDataTypeSettings(int settings);
    void ReadDataTypeSettings(int settings);
    void PopulateUnits(int settings);

    void OnPageChanged( wxNotebookEvent& event );

    void OnUpdate();
    void OnUpdate( wxCommandEvent& event ) { OnUpdate(); }
    void OnUpdateSpin( wxSpinEvent& event ) { OnUpdate(); }
    void OnUpdateColor( wxColourPickerEvent& event ) { OnUpdate(); }
    void OnUpdateOverlayConfig( wxCommandEvent& event );
    void OnUpdateScroll( wxScrollEvent& event ) { OnUpdate(); }
    void OnUpdateIsobar();
    void OnUpdateSpinIsobar( wxSpinEvent& event ) { OnUpdateIsobar(); }
    void OnUpdateIsobar( wxCommandEvent& event ) { OnUpdateIsobar(); }

    void OnUpdateCyclones();
    void OnUpdateCyclonesDate( wxDateEvent& event ) { OnUpdateCyclones(); }
    void OnUpdateCyclonesSpin( wxSpinEvent& event ) { OnUpdateCyclones(); }
    void OnUpdateCyclones( wxCommandEvent& event ) { OnUpdateCyclones(); }

    void OnPaintKey( wxPaintEvent& event );

    void OnEnabled( wxCommandEvent& event );
    void OnAboutAuthor( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event ) { Hide(); }

    void OnRefreshTimer( wxTimerEvent& event );

    int m_lastdatatype;

    ClimatologyDialog *pParent;
    
    wxTimer m_refreshTimer;
};

#endif

