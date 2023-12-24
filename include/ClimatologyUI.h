///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/bmpbuttn.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/clrpicker.h>
#include <wx/panel.h>
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
		wxSpinCtrl* m_sDay;
		wxBitmapButton* m_bpNow;
		wxStaticText* m_stSpeed;
		wxStaticText* m_stDirection;
		wxTextCtrl* m_tWind;
		wxTextCtrl* m_tWindDir;
		wxTextCtrl* m_tCurrent;
		wxTextCtrl* m_tCurrentDir;
		wxTextCtrl* m_tPressure;
		wxTextCtrl* m_tSeaTemperature;
		wxTextCtrl* m_tAirTemperature;
		wxTextCtrl* m_tCloudCover;
		wxButton* m_bConfig;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMonth( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDay( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnAll( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNow( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTimeline( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnTimelineDown( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnTimelineUp( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnUpdateDisplay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnConfig( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxChoice* m_cMonth;
		wxCheckBox* m_cbAll;
		wxSlider* m_sTimeline;
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
		wxCheckBox* m_cbLightning;
		wxTextCtrl* m_tLightning;
		wxCheckBox* m_cbSeaDepth;
		wxTextCtrl* m_tSeaDepth;
		wxCheckBox* m_cbCyclones;

		ClimatologyDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Climatology Display Control"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );

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
		wxStaticText* m_staticText3;
		wxChoice* m_cDataUnits;
		wxCheckBox* m_cbEnabled;
		wxCheckBox* m_cbOverlayMap;
		wxStaticText* m_staticText18;
		wxSlider* m_sOverlayTransparency;
		wxCheckBox* m_cbOverlayInterpolation;
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
		wxStaticText* m_staticText30;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		wxPanel* m_panel51;
		wxStaticText* m_staticText21;
		wxPanel* m_panel511;
		wxStaticText* m_staticText22;
		wxPanel* m_panel5111;
		wxStaticText* m_staticText221;
		wxPanel* m_panel51111;
		wxStaticText* m_staticText2211;
		wxPanel* m_panel511111;
		wxStaticText* m_staticText22111;
		wxPanel* m_panel5111111;
		wxStaticText* m_staticText221111;
		wxPanel* m_panel51111111;
		wxStaticText* m_staticText2211111;
		wxPanel* m_panel511111111;
		wxStaticText* m_staticText22111111;
		wxPanel* m_panel5111111111;
		wxStaticText* m_staticText221111111;
		wxStaticText* m_staticText20;
		wxPanel* m_panel4;
		wxStaticText* m_staticText141;
		wxStaticText* m_stVersion;
		wxStaticText* m_staticText16;
		wxTextCtrl* m_tDataDirectory;
		wxHtmlWindow* m_htmlInformation;
		wxButton* m_bAboutAuthor;
		wxButton* m_bClose;

		// Virtual event handlers, override them in your derived class
		virtual void OnPageChanged( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnDataTypeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnabled( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateOverlayConfig( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnUpdateSpinIsobar( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdateIsobar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUpdateSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdateColor( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnUpdateCyclonesDate( wxDateEvent& event ) { event.Skip(); }
		virtual void OnUpdateCyclonesSpin( wxSpinEvent& event ) { event.Skip(); }
		virtual void OnUpdateCyclones( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPaintKey( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnAboutAuthor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxChoice* m_cDataType;
		wxCheckBox* m_cbWindAtlasEnable;
		wxSpinCtrl* m_sWindAtlasSize;
		wxSpinCtrl* m_sWindAtlasSpacing;
		wxSlider* m_sWindAtlasOpacity;
		wxDatePickerCtrl* m_dPStart;
		wxDatePickerCtrl* m_dPEnd;
		wxSpinCtrl* m_sCycloneDaySpan;
		wxSpinCtrl* m_sMinWindSpeed;
		wxSpinCtrl* m_sMaxPressure;
		wxCheckBox* m_cbElNino;
		wxCheckBox* m_cbLaNina;
		wxCheckBox* m_cbNeutral;
		wxCheckBox* m_cbNotAvailable;
		wxCheckBox* m_cbTropical;
		wxCheckBox* m_cbSubTropical;
		wxCheckBox* m_cbExtraTropical;
		wxCheckBox* m_cbRemanent;

		ClimatologyConfigDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Climatology Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );

		~ClimatologyConfigDialogBase();

};

