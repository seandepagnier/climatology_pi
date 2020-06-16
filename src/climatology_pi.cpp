/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Climatology Plugin
 * Author:   Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Sean D'Epagnier                                 *
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


ClimatologyOverlayFactory *g_pOverlayFactory = NULL;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new climatology_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

static climatology_pi *s_climatology_pi;

wxString ClimatologyDataDirectory()
{
    wxString s =wxFileName::GetPathSeparator();
    return *GetpSharedDataLocation() + "plugins"
        + s + "climatology_pi" + s + "data" + s;
}

wxString ClimatologyUserDataDirectory()
{
    wxString s = wxFileName::GetPathSeparator();
    return *GetpPrivateApplicationDataLocation() + s + "plugins"
        + s + "climatology_pi" + s + "data" + s;
}

climatology_pi::climatology_pi(void *ppimgr)
      :opencpn_plugin_113(ppimgr)
{
      m_pClimatologyDialog = nullptr;
      // Create the PlugIn icons
      initialize_images();
      s_climatology_pi = this;
}

climatology_pi::~climatology_pi()
{
      FreeData();
      delete _img_climatology;
}

int climatology_pi::Init()
{
      AddLocaleCatalog( _T("opencpn-climatology_pi") );

      // Set some default private member parameters
      m_climatology_dialog_x = 0;
      m_climatology_dialog_y = 0;
      m_climatology_dialog_sx = 200;
      m_climatology_dialog_sy = 400;

      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      // Get a pointer to the opencpn display canvas, to use as a parent for the  dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
#ifdef OCPN_USE_SVG
      m_leftclick_tool_id = InsertPlugInToolSVG( "Climatology" , _svg_climatology, _svg_climatology_rollover, _svg_climatology_toggled,
                                              wxITEM_CHECK, _("Climatology"),  "" , NULL, CLIMATOLOGY_TOOL_POSITION, 0, this);
#else
      m_leftclick_tool_id  = InsertPlugInTool("", _img_climatology, _img_climatology, wxITEM_NORMAL,
                                              _("Climatology"), "", NULL,
                                              CLIMATOLOGY_TOOL_POSITION, 0, this);
#endif
      SendClimatology(true);

      return (WANTS_OVERLAY_CALLBACK |
           WANTS_OPENGL_OVERLAY_CALLBACK |
           WANTS_CURSOR_LATLON       |
           WANTS_TOOLBAR_CALLBACK    |
           INSTALLS_TOOLBAR_TOOL     |
           WANTS_CONFIG              |
           WANTS_PLUGIN_MESSAGING
            );
}

bool climatology_pi::DeInit()
{
    SendClimatology(false);
    FreeData();
    RemovePlugInTool(m_leftclick_tool_id);

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
      return new wxBitmap(_img_climatology->ConvertToImage().Copy());
}

wxString climatology_pi::GetCommonName()
{
     return _T(PLUGIN_COMMON_NAME);
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
- Sea Surface Currents\n\
- Average swell and seastate (not yet implemented)\n\
- Precipitation, Humidity, Cloud Cover, and Lightning Strikes \n\
- Monthly average Sea Level pressure and Sea Temperature, Air Temperature\n\
- Tropical Cyclone tracks\
");

}

void climatology_pi::CreateOverlayFactory()
{
    if(m_pClimatologyDialog)
        return;

    //    And load the configuration items
    LoadConfig();
    
    m_pClimatologyDialog = new ClimatologyDialog(m_parent_window, this);
    m_pClimatologyDialog->Move(wxPoint(m_climatology_dialog_x, m_climatology_dialog_y));
    
    wxIcon icon;
    icon.CopyFromBitmap(*_img_climatology);
    m_pClimatologyDialog->SetIcon(icon);
    
    // Create the drawing factory
    g_pOverlayFactory = new ClimatologyOverlayFactory( *m_pClimatologyDialog );
    
    if(g_pOverlayFactory->m_bCompletedLoading) {
        SendClimatology(true);
        m_pClimatologyDialog->UpdateTrackingControls();
        m_pClimatologyDialog->FitLater(); // buggy wx
    }
    m_pClimatologyDialog->Hide();
}

void climatology_pi::SetDefaults(void)
{
}

int climatology_pi::GetToolbarToolCount(void)
{
      return 1;
}

static bool ClimatologyData(int setting, wxDateTime &date, double lat, double lon,
                            double &dir, double &speed)
{
    s_climatology_pi->CreateOverlayFactory();
    // if ClimatologyData is called again while loading data in CreateOverlayFactory
    if (!g_pOverlayFactory)
        return false;

    speed = g_pOverlayFactory->getValue(MAG, setting, lat, lon, &date);
    if(isnan(speed))
        return false;

    dir = g_pOverlayFactory->getValue(DIRECTION, setting, lat, lon, &date);
    if(isnan(dir))
        return false;

    return true;
}

static bool ClimatologyWindAtlasData(wxDateTime &date, double lat, double lon,
                                     int &count, double *directions, double *speeds,
                                     double &storm, double &calm)
{
    if(!g_pOverlayFactory)
        return false;

    if(count != 8)
        return false;
    
    return g_pOverlayFactory->InterpolateWindAtlas
        (date, lat, lon, directions, speeds, storm, calm);
}

static int ClimatologyCycloneTrackCrossings(double lat1, double lon1, double lat2, double lon2,
                                            const wxDateTime &date, int dayrange)
{
    if(!g_pOverlayFactory)
        return -1;

    return g_pOverlayFactory->CycloneTrackCrossings(lat1, lon1, lat2, lon2,
                                                    date, dayrange);
}

void climatology_pi::OnToolbarToolCallback(int id)
{
    CreateOverlayFactory();

    if(m_pClimatologyDialog->IsShown() && m_pClimatologyDialog->m_cfgdlg)
        m_pClimatologyDialog->m_cfgdlg->Hide();

    m_pClimatologyDialog->Show(!m_pClimatologyDialog->IsShown());
    RequestRefresh(m_parent_window); // refresh main window
}

void climatology_pi::OnClimatologyDialogClose()
{
    if(m_pClimatologyDialog) {
        if(m_pClimatologyDialog->m_cfgdlg)
            m_pClimatologyDialog->m_cfgdlg->Hide();
        m_pClimatologyDialog->Show(false);
        RequestRefresh(m_parent_window); // refresh main window
    }
    SaveConfig();
}

bool climatology_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
    if(!m_pClimatologyDialog || !m_pClimatologyDialog->IsShown() ||
       !g_pOverlayFactory)
        return false;

    piDC pidc(dc);
    g_pOverlayFactory->RenderOverlay ( pidc, *vp );
    return true;
}

bool climatology_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
    if(!m_pClimatologyDialog || !m_pClimatologyDialog->IsShown() ||
       !g_pOverlayFactory)
        return false;

    piDC pidc;
    glEnable( GL_BLEND );
    pidc.SetVP(vp);
    
    g_pOverlayFactory->RenderOverlay ( pidc, *vp );
    return true;
}

void climatology_pi::SetCursorLatLon(double lat, double lon)
{
    if(m_pClimatologyDialog)
        m_pClimatologyDialog->SetCursorLatLon(lat, lon);
}

void climatology_pi::SendClimatology(bool valid)
{
    Json::Value v;
    v["ClimatologyVersionMajor"] = GetPlugInVersionMajor();
    v["ClimatologyVersionMinor"] = GetPlugInVersionMinor();

    char ptr[64];
    snprintf(ptr, sizeof ptr, "%p", valid ? ClimatologyData : NULL);
    v["ClimatologyDataPtr"] = ptr;

    snprintf(ptr, sizeof ptr, "%p", valid ? ClimatologyWindAtlasData : NULL);
    v["ClimatologyWindAtlasDataPtr"] = ptr;

    snprintf(ptr, sizeof ptr, "%p", valid ? ClimatologyCycloneTrackCrossings : NULL);
    v["ClimatologyCycloneTrackCrossingsPtr"] = ptr;
    
    Json::FastWriter writer;
    SendPluginMessage(wxT("CLIMATOLOGY"), writer.write( v ));
}

void climatology_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    if(message_id == "CLIMATOLOGY_REQUEST") {
        SendClimatology(true);
    }
}

// -------------------------------------------------------
// GRIBMELINE is a misnomer
void climatology_pi::SendTimelineMessage(wxDateTime time)
{
    Json::Value v;
    if (time.IsValid()) {
        v["Day"] = time.GetDay();
        v["Month"] = time.GetMonth();
        v["Year"] = time.GetYear();
        v["Hour"] = time.GetHour();
        v["Minute"] = time.GetMinute();
        v["Second"] = time.GetSecond();
    } else {
        v["Day"] = -1;
        v["Month"] = -1;
        v["Year"] = -1;
        v["Hour"] = -1;
        v["Minute"] = -1;
        v["Second"] = -1;
    }
    Json::FastWriter writer;
    SendPluginMessage("GRIB_TIMELINE", writer.write( v ));
}

void climatology_pi::FreeData()
{
    delete g_pOverlayFactory;
    g_pOverlayFactory = NULL;
    if(m_pClimatologyDialog) {
        m_pClimatologyDialog->Save();
        m_pClimatologyDialog->Destroy();
        m_pClimatologyDialog = nullptr;
    }
}

bool climatology_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath (  "/Settings/Climatology"  );

    m_climatology_dialog_sx = pConf->Read ( "DialogSizeX" , 300L );
    m_climatology_dialog_sy = pConf->Read ( "DialogSizeY" , 540L );
    m_climatology_dialog_x =  pConf->Read ( "DialogPosX" , 20L );
    m_climatology_dialog_y =  pConf->Read ( "DialogPosY" , 170L );

    return true;
}

bool climatology_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ("/Settings/Climatology");

    pConf->Write("DialogSizeX", m_climatology_dialog_sx );
    pConf->Write("DialogSizeY", m_climatology_dialog_sy );
    pConf->Write("DialogPosX",  m_climatology_dialog_x );
    pConf->Write("DialogPosY",  m_climatology_dialog_y );
    
    return true;
}

void climatology_pi::SetColorScheme(PI_ColorScheme cs)
{
    DimeWindow(m_pClimatologyDialog);
}
