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

#ifndef _TRENDSPI_H_
#define _TRENDSPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif //precompiled headers

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    1

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    8

#include "../../../include/ocpn_plugin.h"

#include "TrendsOverlayFactory.h"
#include "TrendsDialog.h"
#include "TrendsUI.h"


//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define TRENDS_TOOL_POSITION    -1          // Request default positioning of toolbar tool

class trends_pi : public opencpn_plugin_18
{
public:
      trends_pi(void *ppimgr);
      ~trends_pi(void);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

//    The override PlugIn Methods
      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
      void SetCursorLatLon(double lat, double lon);
      bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);


      void SetDefaults(void);

      int GetToolbarToolCount(void);

      void ShowPreferencesDialog( wxWindow* parent );

      void OnToolbarToolCallback(int id);


// Other public methods

      void SetTrendsDialogX    (int x){ m_trends_dialog_x = x;};
      void SetTrendsDialogY    (int x){ m_trends_dialog_y = x;}
      void SetTrendsDialogSizeX(int x){ m_trends_dialog_sx = x;}
      void SetTrendsDialogSizeY(int x){ m_trends_dialog_sy = x;}
      void SetColorScheme(PI_ColorScheme cs);

      void OnTrendsDialogClose();
      TrendsOverlayFactory *GetOverlayFactory(){ return m_pOverlayFactory; }

private:
      bool LoadConfig(void);
      bool SaveConfig(void);

      wxFileConfig     *m_pconfig;
      wxWindow         *m_parent_window;

      TrendsDialog       *m_pTrendsDialog;
      TrendsOverlayFactory *m_pOverlayFactory;

      int              m_display_width, m_display_height;
      int              m_leftclick_tool_id;

      int              m_trends_dialog_x, m_trends_dialog_y;
      int              m_trends_dialog_sx, m_trends_dialog_sy;

};

#endif
