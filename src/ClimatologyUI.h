///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CLIMATOLOGYUI_H__
#define __CLIMATOLOGYUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/filepicker.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ClimatologyDialogBase
///////////////////////////////////////////////////////////////////////////////
class ClimatologyDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxCheckBox* m_cbWind;
		wxTextCtrl* m_tWind;
		wxTextCtrl* m_tCurrent;
		wxTextCtrl* m_tPressure;
		wxTextCtrl* m_tSeaTemperature;
		wxButton* m_bConfig;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMonth( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMonth( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnConfig( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_cMonth;
		wxSlider* m_sMonth;
		wxCheckBox* m_cbCurrents;
		wxCheckBox* m_cbPressure;
		wxCheckBox* m_cbSeaTemperature;
		wxCheckBox* m_cbCyclones;
		
		ClimatologyDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Climatology Display Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxDIALOG_NO_PARENT|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~ClimatologyDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ClimatologyConfigDialogBase
///////////////////////////////////////////////////////////////////////////////
class ClimatologyConfigDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook1;
		wxPanel* m_panel5;
		wxChoice* m_cDataType;
		wxStaticText* m_staticText3;
		wxChoice* m_cDataUnits;
		wxCheckBox* m_cbIsoBars;
		wxStaticText* m_staticText41;
		wxSpinCtrl* m_sIsoBarSpacing;
		wxCheckBox* m_cbOverlayMap;
		wxCheckBox* m_cbNumbers;
		wxStaticText* m_staticText6;
		wxSpinCtrl* m_sNumbersSpacing;
		wxPanel* m_panel41;
		wxStaticText* m_staticText7;
		wxDirPickerCtrl* m_dirWindData;
		wxCheckBox* m_cbWindNumbers;
		wxPanel* m_panel2;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnDataTypeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnCycloneConfig( wxDateEvent& event ) { event.Skip(); }
		virtual void OnCycloneConfig( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnCycloneConfig( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxDatePickerCtrl* m_dPStart;
		wxDatePickerCtrl* m_dPEnd;
		wxSpinCtrl* m_sMinWindSpeed;
		wxSpinCtrl* m_sMaxPressure;
		wxCheckBox* m_cbEastPacific;
		wxCheckBox* m_cbWestPacific;
		wxCheckBox* m_cbSouthPacific;
		wxCheckBox* m_cbAtlantic;
		wxCheckBox* m_cbNorthIndian;
		wxCheckBox* m_cbSouthIndian;
		wxCheckBox* m_cbElNino;
		wxCheckBox* m_cbLaNina;
		wxCheckBox* m_cbNeutral;
		wxCheckBox* m_cbNotAvailable;
		wxCheckBox* m_cbTropical;
		wxCheckBox* m_cbSubTropical;
		wxCheckBox* m_cbExtraTropical;
		wxCheckBox* m_cbRemanent;
		
		ClimatologyConfigDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Climatology Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,480 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ClimatologyConfigDialogBase();
	
};

#endif //__CLIMATOLOGYUI_H__
