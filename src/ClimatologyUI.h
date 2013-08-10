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
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include <wx/clrpicker.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/html/htmlwin.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ClimatologyDialogBase
///////////////////////////////////////////////////////////////////////////////
class ClimatologyDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_tWind;
		wxTextCtrl* m_tWindDir;
		wxTextCtrl* m_tCurrent;
		wxTextCtrl* m_tCurrentDir;
		wxTextCtrl* m_tPressure;
		wxTextCtrl* m_tSeaTemperature;
		wxTextCtrl* m_tAirTemperature;
		wxTextCtrl* m_tCloudCover;
		wxButton* m_bConfig;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMonth( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMonth( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnConfig( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_cMonth;
		wxCheckBox* m_cbAll;
		wxSlider* m_sMonth;
		wxCheckBox* m_cbWind;
		wxCheckBox* m_cbCurrent;
		wxCheckBox* m_cbPressure;
		wxCheckBox* m_cbSeaTemperature;
		wxCheckBox* m_cbAirTemperature;
		wxCheckBox* m_cbCloudCover;
		wxCheckBox* m_cbPrecipitation;
		wxTextCtrl* m_tPrecipitation;
		wxCheckBox* m_cbRelativeHumidity;
		wxTextCtrl* m_tRelativeHumidity;
		wxCheckBox* m_cbSeaDepth;
		wxTextCtrl* m_tSeaDepth;
		wxCheckBox* m_cbCyclones;
		
		ClimatologyDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Climatology Display Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxDIALOG_NO_PARENT|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
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
		wxCheckBox* m_cbEnabled;
		wxCheckBox* m_cbOverlayMap;
		wxStaticText* m_staticText18;
		wxSlider* m_sOverlayTransparency;
		wxCheckBox* m_cbIsoBars;
		wxStaticText* m_staticText41;
		wxSpinCtrl* m_sIsoBarSpacing;
		wxStaticText* m_staticText17;
		wxChoice* m_cIsoBarStep;
		wxCheckBox* m_cbNumbers;
		wxStaticText* m_staticText6;
		wxSpinCtrl* m_sNumbersSpacing;
		wxCheckBox* m_cbDirectionArrowsEnable;
		wxRadioButton* m_rbDirectionArrowsBarbs;
		wxRadioButton* m_rbDirectionArrowsLength;
		wxStaticText* m_staticText7;
		wxSpinCtrl* m_sDirectionArrowsWidth;
		wxStaticText* m_staticText8;
		wxColourPickerCtrl* m_cpDirectionArrows;
		wxStaticText* m_staticText9;
		wxSlider* m_sDirectionArrowsOpacity;
		wxStaticText* m_staticText11;
		wxSpinCtrl* m_sDirectionArrowsSize;
		wxStaticText* m_staticText10;
		wxSpinCtrl* m_sDirectionArrowsSpacing;
		wxPanel* m_panel41;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText13;
		wxStaticText* m_staticText14;
		wxStaticText* m_staticText19;
		wxPanel* m_panel2;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxPanel* m_panel4;
		wxStaticText* m_staticText141;
		wxStaticText* m_stVersion;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_tDataDirectory;
		wxHtmlWindow* m_htmlInformation;
		wxButton* m_bOnDonate;
		wxButton* m_bClose;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPageChanged( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnDataTypeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnabled( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxDateEvent& event ) { event.Skip(); }
		virtual void OnDonate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxCheckBox* m_cbWindAtlasEnable;
		wxSpinCtrl* m_sWindAtlasSize;
		wxSpinCtrl* m_sWindAtlasSpacing;
		wxSlider* m_sWindAtlasOpacity;
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
		
		ClimatologyConfigDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Climatology Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,480 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ClimatologyConfigDialogBase();
	
};

#endif //__CLIMATOLOGYUI_H__
