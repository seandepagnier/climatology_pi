/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Trends Plugin
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#include <wx/treectrl.h>
#include <wx/fileconf.h>

#include "trends_pi.h"
#include "icons.h"
#include "TrendsDialog.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new trends_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    Trends PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

trends_pi::trends_pi(void *ppimgr)
      :opencpn_plugin_18(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

}

trends_pi::~trends_pi(void)
{
      delete _img_trends;
}

int trends_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-trends_pi") );

      // Set some default private member parameters
      m_trends_dialog_x = 0;
      m_trends_dialog_y = 0;
      m_trends_dialog_sx = 200;
      m_trends_dialog_sy = 400;
      m_pTrendsDialog = NULL;
      m_pOverlayFactory = NULL;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      // Get a pointer to the opencpn display canvas, to use as a parent for the  dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_trends, _img_trends, wxITEM_NORMAL,
                                              _("Trends"), _T(""), NULL,
                                              TRENDS_TOOL_POSITION, 0, this);

      return (WANTS_OVERLAY_CALLBACK |
           WANTS_OPENGL_OVERLAY_CALLBACK |
           WANTS_CURSOR_LATLON       |
           WANTS_TOOLBAR_CALLBACK    |
           INSTALLS_TOOLBAR_TOOL     |
           WANTS_CONFIG              |
           WANTS_PREFERENCES
            );
}

bool trends_pi::DeInit(void)
{
    if(m_pTrendsDialog) {
        m_pTrendsDialog->Close();
        delete m_pTrendsDialog;
        m_pTrendsDialog = NULL;
    }

    delete m_pOverlayFactory;
    m_pOverlayFactory = NULL;

      return true;
}

int trends_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int trends_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int trends_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int trends_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *trends_pi::GetPlugInBitmap()
{
      return _img_trends;
}

wxString trends_pi::GetCommonName()
{
      return _("TRENDS");
}


wxString trends_pi::GetShortDescription()
{
      return _("Trends PlugIn for OpenCPN");
}


wxString trends_pi::GetLongDescription()
{
      return _("Trends PlugIn for OpenCPN\n\
Provides overlay capabilities for history weather data.\n\n\
Supported TRENDS file types include:\n\
- Average wind directions and speed\n\
- Percentage of gale and calm conditions\n\
- Average swell and seastate\n\
- ITCZ (doldrum) positions\n\
- Tropical Storm tracks.\
");

}

void trends_pi::SetDefaults(void)
{
}


int trends_pi::GetToolbarToolCount(void)
{
      return 1;
}

void trends_pi::ShowPreferencesDialog( wxWindow* parent )
{
      wxDialog *dialog = new wxDialog( parent, wxID_ANY, _("TRENDS Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE );

   dialog->Fit();

    if(dialog->ShowModal() == wxID_OK)
    {
        SaveConfig();
    }
}

void trends_pi::OnToolbarToolCallback(int id)
{
    if(!m_pTrendsDialog)
    {
        m_pTrendsDialog = new TrendsDialog(m_parent_window, this);
        m_pTrendsDialog->Move(wxPoint(m_trends_dialog_x, m_trends_dialog_y));

        // Create the drawing factory
        m_pOverlayFactory = new TrendsOverlayFactory( *m_pTrendsDialog );
    }

    m_pTrendsDialog->Show(!m_pTrendsDialog->IsShown());

//    SetToolbarItemState( m_leftclick_tool_id, m_pTrendsDialog->IsShown() );
}

void trends_pi::OnTrendsDialogClose()
{
//    SetToolbarItemState( m_leftclick_tool_id, false );

    if(m_pTrendsDialog)
        m_pTrendsDialog->Show(false);

    SaveConfig();
}

bool trends_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if(!m_pTrendsDialog ||
       !m_pTrendsDialog->IsShown() ||
       !m_pOverlayFactory)
        return false;

    m_pOverlayFactory->RenderOverlay ( dc, vp );
    return true;
}

bool trends_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(!m_pTrendsDialog ||
       !m_pTrendsDialog->IsShown() ||
       !m_pOverlayFactory)
        return false;

    m_pOverlayFactory->RenderGLOverlay ( pcontext, vp );
    return true;
}

void trends_pi::SetCursorLatLon(double lat, double lon)
{
/*
    if(m_pTrendsDialog)
        m_pTrendsDialog->SetCursorLatLon(lat, lon);
*/
}

bool trends_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/Settings/Trends" ) );

    m_trends_dialog_sx = pConf->Read ( _T ( "DialogSizeX" ), 300L );
    m_trends_dialog_sy = pConf->Read ( _T ( "DialogSizeY" ), 540L );
    m_trends_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
    m_trends_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 170L );

    return true;
}

bool trends_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T ( "/Settings/Trends" ) );

    pConf->Write ( _T ( "DialogSizeX" ),  m_trends_dialog_sx );
    pConf->Write ( _T ( "DialogSizeY" ),  m_trends_dialog_sy );
    pConf->Write ( _T ( "DialogPosX" ),   m_trends_dialog_x );
    pConf->Write ( _T ( "DialogPosY" ),   m_trends_dialog_y );
    
    return true;
}

void trends_pi::SetColorScheme(PI_ColorScheme cs)
{
    DimeWindow(m_pTrendsDialog);
}
