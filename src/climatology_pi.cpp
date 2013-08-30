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

#include "climatology_pi.h"
#include "icons.h"
#include "ClimatologyDialog.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new climatology_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}


wxString ClimatologyDataDirectory()
{
    wxString s =wxFileName::GetPathSeparator();
    return *GetpSharedDataLocation() + _T("plugins")
        + s + _T("climatology") + s + _T("data") + _T("/");
}

climatology_pi::climatology_pi(void *ppimgr)
      :opencpn_plugin_19(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();

}

climatology_pi::~climatology_pi(void)
{
      delete _img_climatology;
}

int climatology_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-climatology_pi") );

      // Set some default private member parameters
      m_climatology_dialog_x = 0;
      m_climatology_dialog_y = 0;
      m_climatology_dialog_sx = 200;
      m_climatology_dialog_sy = 400;
      m_pClimatologyDialog = NULL;
      m_pOverlayFactory = NULL;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      // Get a pointer to the opencpn display canvas, to use as a parent for the  dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_climatology, _img_climatology, wxITEM_NORMAL,
                                              _("Climatology"), _T(""), NULL,
                                              CLIMATOLOGY_TOOL_POSITION, 0, this);

      return (WANTS_OVERLAY_CALLBACK |
           WANTS_OPENGL_OVERLAY_CALLBACK |
           WANTS_CURSOR_LATLON       |
           WANTS_TOOLBAR_CALLBACK    |
           INSTALLS_TOOLBAR_TOOL     |
           WANTS_CONFIG              |
//           WANTS_PREFERENCES         |
           WANTS_PLUGIN_MESSAGING
            );
}

bool climatology_pi::DeInit(void)
{
    SendClimatology(NULL);
    if(m_pClimatologyDialog) {
        m_pClimatologyDialog->Close();
        delete m_pClimatologyDialog;
        m_pClimatologyDialog = NULL;
    }

    delete m_pOverlayFactory;
    m_pOverlayFactory = NULL;

    return true;
}

int climatology_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int climatology_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int climatology_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int climatology_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *climatology_pi::GetPlugInBitmap()
{
      return _img_climatology;
}

wxString climatology_pi::GetCommonName()
{
      return _("Climatology");
}


wxString climatology_pi::GetShortDescription()
{
      return _("Climatology PlugIn for OpenCPN");
}


wxString climatology_pi::GetLongDescription()
{
      return _("Climatology PlugIn for OpenCPN\n\
Provides overlay capabilities for historic weather data.\n\n\
Supported Climatology types include:\n\
- Average wind directions and speed\n\
- Percentage of gale and calm conditions\n\
- Average swell and seastate (not yet implemented)\n\
- Precipitation, Humidity, and Cloud Cover \n\
- Monthly average Sea Level pressure and Sea Temperature, Air Temperature\n\
- Tropical Cyclone tracks\
");

}

void climatology_pi::SetDefaults(void)
{
}

int climatology_pi::GetToolbarToolCount(void)
{
      return 1;
}

void climatology_pi::ShowPreferencesDialog( wxWindow* parent )
{
      wxDialog *dialog = new wxDialog( parent, wxID_ANY, _("Climatology Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE );

   dialog->Fit();

    if(dialog->ShowModal() == wxID_OK)
    {
        SaveConfig();
    }
}

static ClimatologyOverlayFactory *s_pOverlayFactory = NULL;
static bool ClimatologyData(int setting, wxDateTime &date, double lat, double lon,
                            double &dir, double &speed)
{
    if(!s_pOverlayFactory)
        return false;

    if(isnan(speed = s_pOverlayFactory->getValue(MAG, setting, lat, lon, &date)))
        return false;

    if(isnan(dir = s_pOverlayFactory->getValue(DIRECTION, setting, lat, lon, &date)))
        return false;

    return true;
}

void climatology_pi::OnToolbarToolCallback(int id)
{
    if(!m_pClimatologyDialog)
    {
        m_pClimatologyDialog = new ClimatologyDialog(m_parent_window, this);
        m_pClimatologyDialog->Move(wxPoint(m_climatology_dialog_x, m_climatology_dialog_y));

        // Create the drawing factory
        m_pOverlayFactory = new ClimatologyOverlayFactory( *m_pClimatologyDialog );

        s_pOverlayFactory = m_pOverlayFactory;
        SendClimatology(ClimatologyData);

        RequestRefresh(m_parent_window); // refresh main window
    }

    m_pClimatologyDialog->Show(!m_pClimatologyDialog->IsShown());

//    SetToolbarItemState( m_leftclick_tool_id, m_pClimatologyDialog->IsShown() );
}

void climatology_pi::OnClimatologyDialogClose()
{
//    SetToolbarItemState( m_leftclick_tool_id, false );

    if(m_pClimatologyDialog)
        m_pClimatologyDialog->Show(false);

    SaveConfig();
}

bool climatology_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if(!m_pClimatologyDialog || !m_pClimatologyDialog->IsShown() ||
       !m_pOverlayFactory)
        return false;

    m_pOverlayFactory->RenderOverlay ( &dc, *vp );
    return true;
}

bool climatology_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(!m_pClimatologyDialog || !m_pClimatologyDialog->IsShown() ||
       !m_pOverlayFactory)
        return false;

    m_pOverlayFactory->RenderOverlay ( NULL, *vp );
    return true;
}

void climatology_pi::SetCursorLatLon(double lat, double lon)
{
    if(m_pClimatologyDialog)
        m_pClimatologyDialog->SetCursorLatLon(lat, lon);
}

void climatology_pi::SendClimatology(bool (*ClimatologyData)(int, wxDateTime &, double, double,
                                                             double &, double &))
{
    wxJSONValue v;
    v[_T("ClimatologyVersionMajor")] = GetPlugInVersionMajor();
    v[_T("ClimatologyVersionMinor")] = GetPlugInVersionMinor();

    char ptr[64];
    snprintf(ptr, sizeof ptr, "%p", ClimatologyData);
    v[_T("ClimatologyDataPtr")] = wxString::From8BitData(ptr);
    
    wxJSONWriter w;
    wxString out;
    w.Write(v, out);
    SendPluginMessage(wxString(_T("CLIMATOLOGY")), out);
}

void climatology_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    if(message_id == _T("CLIMATOLOGY_REQUEST")) {
        SendClimatology(ClimatologyData);
    }
}

bool climatology_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/Settings/Climatology" ) );

    m_climatology_dialog_sx = pConf->Read ( _T ( "DialogSizeX" ), 300L );
    m_climatology_dialog_sy = pConf->Read ( _T ( "DialogSizeY" ), 540L );
    m_climatology_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 20L );
    m_climatology_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 170L );

    return true;
}

bool climatology_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T ( "/Settings/Climatology" ) );

    pConf->Write ( _T ( "DialogSizeX" ), m_climatology_dialog_sx );
    pConf->Write ( _T ( "DialogSizeY" ), m_climatology_dialog_sy );
    pConf->Write ( _T ( "DialogPosX" ),  m_climatology_dialog_x );
    pConf->Write ( _T ( "DialogPosY" ),  m_climatology_dialog_y );
    
    return true;
}

void climatology_pi::SetColorScheme(PI_ColorScheme cs)
{
    DimeWindow(m_pClimatologyDialog);
}
