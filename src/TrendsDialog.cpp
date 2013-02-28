/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Trends Object
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

#include "trends_pi.h"
#include "TrendsDialog.h"
#include "TrendsConfigDialog.h"

TrendsDialog::TrendsDialog(wxWindow *parent, trends_pi *ppi)
  : TrendsDialogBase(parent)
{
    pParent = parent;
    pPlugIn = ppi;

    wxFileConfig *pConf = GetOCPNConfigObject();;

    if(pConf) {
        pConf->SetPath ( _T ( "/Settings/Trends" ) );
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
        pConf->Read ( _T ( "TrendsDirectory" ), &m_trends_dir );
*/
    }


    DimeWindow( this );

    Fit();
    SetMinSize( GetBestSize() );
}


TrendsDialog::~TrendsDialog()
{
    wxFileConfig *pConf = GetOCPNConfigObject();;

    if(pConf) {
        pConf->SetPath ( _T ( "/Settings/Trends" ) );
/*
        pConf->Write( _T ( "WindPlot" ), m_cbWind->GetValue());
        pConf->Write( _T ( "WavePlot" ), m_cbWave->GetValue());
        pConf->Write( _T ( "CurrentPlot" ), m_cbCurrent->GetValue());
        pConf->Write( _T ( "PressurePlot" ), m_cbPressure->GetValue());
        pConf->Write( _T ( "SeaTemperaturePlot" ), m_cbSeaTemperature->GetValue());
        pConf->Write ( _T ( "lastdatatype" ), m_lastdatatype);

        pConf->SetPath ( _T ( "/Directories" ) );
        pConf->Write ( _T ( "TrendsDirectory" ), m_trends_dir );
*/
    }
}

void TrendsDialog::SetFactoryOptions()
{
    pPlugIn->GetOverlayFactory()->ClearCachedData();

    RequestRefresh( pParent );
}

void TrendsDialog::OnMonth( wxCommandEvent& event )
{
    m_sMonth->SetValue(event.GetSelection());
}

void TrendsDialog::OnMonth( wxScrollEvent& event )
{
    m_cMonth->SetSelection(event.GetPosition());
}

void TrendsDialog::OnConfig( wxCommandEvent& event )
{
    TrendsConfigDialog dlg(pParent);
    dlg.ShowModal();
}

void TrendsDialog::OnClose( wxCloseEvent& event )
{
    pPlugIn->OnTrendsDialogClose();
}

void TrendsDialog::OnCBAny( wxCommandEvent& event )
{
    SetFactoryOptions();                     // Reload the visibility options
}
