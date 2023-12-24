///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ClimatologyUI.h"

///////////////////////////////////////////////////////////////////////////

ClimatologyDialogBase::ClimatologyDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxVERTICAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	wxStaticBoxSizer* sbSizer21;
	sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Month") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer23;
	fgSizer23 = new wxFlexGridSizer( 0, 0, 0, 0 );
	fgSizer23->AddGrowableCol( 0 );
	fgSizer23->SetFlexibleDirection( wxBOTH );
	fgSizer23->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxString m_cMonthChoices[] = { _("January"), _("Febuary"), _("March"), _("April"), _("May"), _("June"), _("July"), _("August"), _("September"), _("October"), _("November"), _("December") };
	int m_cMonthNChoices = sizeof( m_cMonthChoices ) / sizeof( wxString );
	m_cMonth = new wxChoice( sbSizer21->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 80,-1 ), m_cMonthNChoices, m_cMonthChoices, 0 );
	m_cMonth->SetSelection( 0 );
	fgSizer23->Add( m_cMonth, 0, wxALL|wxEXPAND, 5 );

	m_sDay = new wxSpinCtrl( sbSizer21->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxSP_ARROW_KEYS, 1, 31, 1 );
	fgSizer23->Add( m_sDay, 0, wxALL, 5 );

	m_cbAll = new wxCheckBox( sbSizer21->GetStaticBox(), wxID_ANY, _("All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer23->Add( m_cbAll, 0, wxALL, 0 );

	m_bpNow = new wxBitmapButton( sbSizer21->GetStaticBox(), wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	fgSizer23->Add( m_bpNow, 0, wxALL, 0 );


	sbSizer21->Add( fgSizer23, 1, wxEXPAND, 5 );

	m_sTimeline = new wxSlider( sbSizer21->GetStaticBox(), wxID_ANY, 0, 1, 500, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizer21->Add( m_sTimeline, 0, wxEXPAND, 5 );


	fgSizer1->Add( sbSizer21, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Climatology Data") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );


	fgSizer15->Add( 0, 0, 1, wxEXPAND, 5 );

	m_stSpeed = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, _("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stSpeed->Wrap( -1 );
	fgSizer15->Add( m_stSpeed, 0, 0, 5 );

	m_stDirection = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, _("Direction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stDirection->Wrap( -1 );
	fgSizer15->Add( m_stDirection, 0, 0, 5 );

	m_cbWind = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbWind->Enable( false );

	fgSizer15->Add( m_cbWind, 0, wxALL, 5 );

	m_tWind = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
	fgSizer15->Add( m_tWind, 0, wxALL, 5 );

	m_tWindDir = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( m_tWindDir, 0, wxALL, 5 );

	m_cbCurrent = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Current"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCurrent->Enable( false );

	fgSizer15->Add( m_cbCurrent, 0, wxALL, 5 );

	m_tCurrent = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
	fgSizer15->Add( m_tCurrent, 0, wxALL, 5 );

	m_tCurrentDir = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( m_tCurrentDir, 0, wxALL, 5 );


	fgSizer14->Add( fgSizer15, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbPressure = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbPressure->Enable( false );

	fgSizer4->Add( m_cbPressure, 0, wxALL, 5 );

	m_tPressure = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tPressure, 0, wxALL, 5 );

	m_cbSeaTemperature = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Sea Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSeaTemperature->Enable( false );

	fgSizer4->Add( m_cbSeaTemperature, 0, wxALL, 5 );

	m_tSeaTemperature = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tSeaTemperature, 0, wxALL, 5 );

	m_cbAirTemperature = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Air Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbAirTemperature->Enable( false );

	fgSizer4->Add( m_cbAirTemperature, 0, wxALL, 5 );

	m_tAirTemperature = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tAirTemperature, 0, wxALL, 5 );

	m_cbCloudCover = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Cloud Cover"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCloudCover->Enable( false );

	fgSizer4->Add( m_cbCloudCover, 0, wxALL, 5 );

	m_tCloudCover = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tCloudCover, 0, wxALL, 5 );

	m_cbPrecipitation = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Precipitation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbPrecipitation->Enable( false );

	fgSizer4->Add( m_cbPrecipitation, 0, wxALL, 5 );

	m_tPrecipitation = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tPrecipitation, 0, wxALL, 5 );

	m_cbRelativeHumidity = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Rel Humidity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbRelativeHumidity->Enable( false );

	fgSizer4->Add( m_cbRelativeHumidity, 0, wxALL, 5 );

	m_tRelativeHumidity = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tRelativeHumidity, 0, wxALL, 5 );

	m_cbLightning = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Lightning"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbLightning->Enable( false );

	fgSizer4->Add( m_cbLightning, 0, wxALL, 5 );

	m_tLightning = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tLightning, 0, wxALL, 5 );

	m_cbSeaDepth = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Sea Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSeaDepth->Enable( false );

	fgSizer4->Add( m_cbSeaDepth, 0, wxALL, 5 );

	m_tSeaDepth = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tSeaDepth, 0, wxALL, 5 );

	m_cbCyclones = new wxCheckBox( sbSizer2->GetStaticBox(), wxID_ANY, _("Cyclones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCyclones->Enable( false );

	fgSizer4->Add( m_cbCyclones, 0, wxALL, 5 );

	m_bConfig = new wxButton( sbSizer2->GetStaticBox(), wxID_ANY, _("Config"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_bConfig, 0, wxALL, 5 );


	fgSizer14->Add( fgSizer4, 1, wxEXPAND, 5 );


	sbSizer2->Add( fgSizer14, 1, wxEXPAND, 5 );


	fgSizer1->Add( sbSizer2, 1, wxFIXED_MINSIZE, 5 );


	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ClimatologyDialogBase::OnClose ) );
	m_cMonth->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sDay->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyDialogBase::OnDay ), NULL, this );
	m_cbAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnAll ), NULL, this );
	m_bpNow->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnNow ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnTimelineDown ), NULL, this );
	m_sTimeline->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnTimelineUp ), NULL, this );
	m_cbWind->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbCurrent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbPressure->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbSeaTemperature->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbAirTemperature->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbCloudCover->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbPrecipitation->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbRelativeHumidity->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbLightning->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbSeaDepth->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbCyclones->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_bConfig->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnConfig ), NULL, this );
}

ClimatologyDialogBase::~ClimatologyDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ClimatologyDialogBase::OnClose ) );
	m_cMonth->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sDay->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyDialogBase::OnDay ), NULL, this );
	m_cbAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnAll ), NULL, this );
	m_bpNow->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnNow ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyDialogBase::OnTimeline ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnTimelineDown ), NULL, this );
	m_sTimeline->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnTimelineUp ), NULL, this );
	m_cbWind->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbCurrent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbPressure->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbSeaTemperature->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbAirTemperature->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbCloudCover->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbPrecipitation->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbRelativeHumidity->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbLightning->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbSeaDepth->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_cbCyclones->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdateDisplay ), NULL, this );
	m_bConfig->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnConfig ), NULL, this );

}

ClimatologyConfigDialogBase::ClimatologyConfigDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel5 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxString m_cDataTypeChoices[] = { _("------------") };
	int m_cDataTypeNChoices = sizeof( m_cDataTypeChoices ) / sizeof( wxString );
	m_cDataType = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cDataTypeNChoices, m_cDataTypeChoices, 0 );
	m_cDataType->SetSelection( 0 );
	fgSizer18->Add( m_cDataType, 0, wxALL, 5 );

	m_staticText3 = new wxStaticText( m_panel5, wxID_ANY, _("Units"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer18->Add( m_staticText3, 0, wxALL, 5 );

	wxString m_cDataUnitsChoices[] = { _("------------") };
	int m_cDataUnitsNChoices = sizeof( m_cDataUnitsChoices ) / sizeof( wxString );
	m_cDataUnits = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cDataUnitsNChoices, m_cDataUnitsChoices, 0 );
	m_cDataUnits->SetSelection( 0 );
	fgSizer18->Add( m_cDataUnits, 0, wxALL, 5 );

	m_cbEnabled = new wxCheckBox( m_panel5, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer18->Add( m_cbEnabled, 0, wxALL, 5 );


	fgSizer7->Add( fgSizer18, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer19->AddGrowableCol( 2 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbOverlayMap = new wxCheckBox( m_panel5, wxID_ANY, _("OverlayMap"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer19->Add( m_cbOverlayMap, 0, wxALL, 5 );

	m_staticText18 = new wxStaticText( m_panel5, wxID_ANY, _("Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer19->Add( m_staticText18, 0, wxALL, 5 );

	m_sOverlayTransparency = new wxSlider( m_panel5, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer19->Add( m_sOverlayTransparency, 0, wxALL|wxEXPAND, 5 );

	m_cbOverlayInterpolation = new wxCheckBox( m_panel5, wxID_ANY, _("Interpolation"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer19->Add( m_cbOverlayInterpolation, 0, wxALL, 5 );


	fgSizer7->Add( fgSizer19, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbIsoBars = new wxCheckBox( m_panel5, wxID_ANY, _("Iso Bars"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer20->Add( m_cbIsoBars, 0, wxALL, 5 );

	m_staticText41 = new wxStaticText( m_panel5, wxID_ANY, _("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer20->Add( m_staticText41, 0, wxALL, 5 );

	m_sIsoBarSpacing = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 1 );
	fgSizer20->Add( m_sIsoBarSpacing, 0, wxALL, 5 );

	m_staticText17 = new wxStaticText( m_panel5, wxID_ANY, _("Step"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText17->Wrap( -1 );
	fgSizer20->Add( m_staticText17, 0, wxALL, 5 );

	wxString m_cIsoBarStepChoices[] = { _("Very Course"), _("Course"), _("Normal"), _("Fine"), _("Very Fine") };
	int m_cIsoBarStepNChoices = sizeof( m_cIsoBarStepChoices ) / sizeof( wxString );
	m_cIsoBarStep = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cIsoBarStepNChoices, m_cIsoBarStepChoices, 0 );
	m_cIsoBarStep->SetSelection( 0 );
	fgSizer20->Add( m_cIsoBarStep, 0, wxALL, 5 );


	fgSizer7->Add( fgSizer20, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbNumbers = new wxCheckBox( m_panel5, wxID_ANY, _("Numbers"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_cbNumbers, 0, wxALL, 5 );

	m_staticText6 = new wxStaticText( m_panel5, wxID_ANY, _("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer21->Add( m_staticText6, 0, wxALL, 5 );

	m_sNumbersSpacing = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, 50 );
	fgSizer21->Add( m_sNumbersSpacing, 0, wxALL, 5 );


	fgSizer7->Add( fgSizer21, 1, wxEXPAND, 5 );


	fgSizer12->Add( fgSizer7, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer15->AddGrowableCol( 0 );
	fgSizer15->AddGrowableRow( 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer81;
	sbSizer81 = new wxStaticBoxSizer( new wxStaticBox( m_panel5, wxID_ANY, _("Direction Arrows") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbDirectionArrowsEnable = new wxCheckBox( sbSizer81->GetStaticBox(), wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_cbDirectionArrowsEnable, 0, wxALL, 5 );


	fgSizer16->Add( 0, 0, 1, wxEXPAND, 5 );

	m_rbDirectionArrowsBarbs = new wxRadioButton( sbSizer81->GetStaticBox(), wxID_ANY, _("Barbs"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_rbDirectionArrowsBarbs, 0, wxALL, 5 );

	m_rbDirectionArrowsLength = new wxRadioButton( sbSizer81->GetStaticBox(), wxID_ANY, _("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_rbDirectionArrowsLength, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( sbSizer81->GetStaticBox(), wxID_ANY, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer16->Add( m_staticText7, 0, wxALL, 5 );

	m_sDirectionArrowsWidth = new wxSpinCtrl( sbSizer81->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 5, 3 );
	fgSizer16->Add( m_sDirectionArrowsWidth, 0, wxALL, 5 );

	m_staticText8 = new wxStaticText( sbSizer81->GetStaticBox(), wxID_ANY, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer16->Add( m_staticText8, 0, wxALL, 5 );

	m_cpDirectionArrows = new wxColourPickerCtrl( sbSizer81->GetStaticBox(), wxID_ANY, wxColour( 0, 221, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer16->Add( m_cpDirectionArrows, 0, wxALL, 5 );

	m_staticText9 = new wxStaticText( sbSizer81->GetStaticBox(), wxID_ANY, _("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer16->Add( m_staticText9, 0, wxALL, 5 );

	m_sDirectionArrowsOpacity = new wxSlider( sbSizer81->GetStaticBox(), wxID_ANY, 205, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer16->Add( m_sDirectionArrowsOpacity, 0, wxALL|wxEXPAND, 5 );

	m_staticText11 = new wxStaticText( sbSizer81->GetStaticBox(), wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer16->Add( m_staticText11, 0, wxALL, 5 );

	m_sDirectionArrowsSize = new wxSpinCtrl( sbSizer81->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 30 );
	fgSizer16->Add( m_sDirectionArrowsSize, 0, wxALL, 5 );

	m_staticText10 = new wxStaticText( sbSizer81->GetStaticBox(), wxID_ANY, _("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer16->Add( m_staticText10, 0, wxALL, 5 );

	m_sDirectionArrowsSpacing = new wxSpinCtrl( sbSizer81->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 100 );
	fgSizer16->Add( m_sDirectionArrowsSpacing, 0, wxALL, 5 );


	sbSizer81->Add( fgSizer16, 1, wxEXPAND, 5 );


	fgSizer15->Add( sbSizer81, 1, wxEXPAND, 5 );


	fgSizer12->Add( fgSizer15, 1, wxEXPAND, 5 );


	m_panel5->SetSizer( fgSizer12 );
	m_panel5->Layout();
	fgSizer12->Fit( m_panel5 );
	m_notebook1->AddPage( m_panel5, _("Standard Displays"), true );
	m_panel41 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer61->AddGrowableCol( 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panel41, wxID_ANY, _("Wind Atlas") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer24->AddGrowableCol( 1 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbWindAtlasEnable = new wxCheckBox( sbSizer9->GetStaticBox(), wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer17->Add( m_cbWindAtlasEnable, 0, wxALL, 5 );


	fgSizer17->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText12 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer17->Add( m_staticText12, 0, wxALL, 5 );

	m_sWindAtlasSize = new wxSpinCtrl( sbSizer9->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 5, 500, 60 );
	fgSizer17->Add( m_sWindAtlasSize, 0, wxALL, 5 );

	m_staticText13 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, _("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer17->Add( m_staticText13, 0, wxALL, 5 );

	m_sWindAtlasSpacing = new wxSpinCtrl( sbSizer9->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, 80 );
	fgSizer17->Add( m_sWindAtlasSpacing, 0, wxALL, 5 );

	m_staticText14 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, _("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer17->Add( m_staticText14, 0, wxALL, 5 );

	m_sWindAtlasOpacity = new wxSlider( sbSizer9->GetStaticBox(), wxID_ANY, 205, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer17->Add( m_sWindAtlasOpacity, 0, wxALL|wxEXPAND, 5 );


	fgSizer24->Add( fgSizer17, 1, wxEXPAND, 5 );

	m_staticText19 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, _("The wind atlas display shows the percentage of time wind is in each of eight directions as a length of the arrow, or in the case it is over 29% a number is shown.\n\nThe barbs indicate wind speed, each barb being 5 knots.\n\nThe numbers inside the circle, if blue give percentage of average wind speed as calm (3 knots or less) or in red at least gale force (34 knots and above)."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( 240 );
	fgSizer24->Add( m_staticText19, 0, wxALL|wxEXPAND, 5 );


	sbSizer9->Add( fgSizer24, 1, wxEXPAND, 5 );


	fgSizer61->Add( sbSizer9, 1, wxEXPAND, 5 );


	m_panel41->SetSizer( fgSizer61 );
	m_panel41->Layout();
	fgSizer61->Fit( m_panel41 );
	m_notebook1->AddPage( m_panel41, _("Wind"), false );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer26;
	fgSizer26 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer26->SetFlexibleDirection( wxBOTH );
	fgSizer26->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Start Date") ), wxVERTICAL );

	m_dPStart = new wxDatePickerCtrl( sbSizer7->GetStaticBox(), wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_SHOWCENTURY );
	sbSizer7->Add( m_dPStart, 0, wxALL|wxEXPAND, 5 );


	fgSizer5->Add( sbSizer7, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("End Date") ), wxVERTICAL );

	m_dPEnd = new wxDatePickerCtrl( sbSizer8->GetStaticBox(), wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	sbSizer8->Add( m_dPEnd, 0, wxALL|wxEXPAND, 5 );


	fgSizer5->Add( sbSizer8, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer30;
	fgSizer30 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer30->SetFlexibleDirection( wxBOTH );
	fgSizer30->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Display Spanning") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer291;
	fgSizer291 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer291->SetFlexibleDirection( wxBOTH );
	fgSizer291->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_sCycloneDaySpan = new wxSpinCtrl( sbSizer11->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 365, 30 );
	fgSizer291->Add( m_sCycloneDaySpan, 0, wxALL, 5 );

	m_staticText30 = new wxStaticText( sbSizer11->GetStaticBox(), wxID_ANY, _("days"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer291->Add( m_staticText30, 0, wxALL, 5 );


	sbSizer11->Add( fgSizer291, 1, wxEXPAND, 5 );


	fgSizer30->Add( sbSizer11, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer28;
	fgSizer28 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer28->SetFlexibleDirection( wxBOTH );
	fgSizer28->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText4 = new wxStaticText( m_panel2, wxID_ANY, _("Min Windspeed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer28->Add( m_staticText4, 0, wxALL, 5 );

	m_sMinWindSpeed = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 200, 60 );
	fgSizer28->Add( m_sMinWindSpeed, 0, wxALL, 5 );


	fgSizer30->Add( fgSizer28, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText5 = new wxStaticText( m_panel2, wxID_ANY, _("Max Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer29->Add( m_staticText5, 0, wxALL, 5 );

	m_sMaxPressure = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1200, 1000 );
	fgSizer29->Add( m_sMaxPressure, 0, wxALL, 5 );


	fgSizer30->Add( fgSizer29, 1, wxEXPAND, 5 );


	fgSizer5->Add( fgSizer30, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("El Nino Period") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbElNino = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("El Nino"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbElNino->SetValue(true);
	fgSizer10->Add( m_cbElNino, 0, wxALL, 5 );

	m_cbLaNina = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("La Nina"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbLaNina->SetValue(true);
	fgSizer10->Add( m_cbLaNina, 0, wxALL, 5 );

	m_cbNeutral = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("Neutral"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbNeutral->SetValue(true);
	fgSizer10->Add( m_cbNeutral, 0, wxALL, 5 );

	m_cbNotAvailable = new wxCheckBox( sbSizer4->GetStaticBox(), wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_cbNotAvailable, 0, wxALL, 5 );


	sbSizer4->Add( fgSizer10, 1, wxEXPAND, 5 );


	fgSizer5->Add( sbSizer4, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Storm Types") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbTropical = new wxCheckBox( sbSizer5->GetStaticBox(), wxID_ANY, _("Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbTropical->SetValue(true);
	fgSizer11->Add( m_cbTropical, 0, wxALL, 5 );

	m_cbSubTropical = new wxCheckBox( sbSizer5->GetStaticBox(), wxID_ANY, _("Sub Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSubTropical->SetValue(true);
	fgSizer11->Add( m_cbSubTropical, 0, wxALL, 5 );

	m_cbExtraTropical = new wxCheckBox( sbSizer5->GetStaticBox(), wxID_ANY, _("ExtraTropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbExtraTropical->SetValue(true);
	fgSizer11->Add( m_cbExtraTropical, 0, wxALL, 5 );

	m_cbRemanent = new wxCheckBox( sbSizer5->GetStaticBox(), wxID_ANY, _("Remanent"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_cbRemanent, 0, wxALL, 5 );


	sbSizer5->Add( fgSizer11, 1, wxEXPAND, 5 );


	fgSizer5->Add( sbSizer5, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Sustained Wind Key (knots)") ), wxVERTICAL );

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 0, 6, 0, 0 );

	m_panel51 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("20") );
	gSizer1->Add( m_panel51, 1, wxEXPAND | wxALL, 5 );

	m_staticText21 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("20"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	gSizer1->Add( m_staticText21, 0, wxALL, 5 );

	m_panel511 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("40") );
	gSizer1->Add( m_panel511, 1, wxEXPAND | wxALL, 5 );

	m_staticText22 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("40"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	gSizer1->Add( m_staticText22, 0, wxALL, 5 );

	m_panel5111 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("60") );
	gSizer1->Add( m_panel5111, 1, wxEXPAND | wxALL, 5 );

	m_staticText221 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("60"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText221->Wrap( -1 );
	gSizer1->Add( m_staticText221, 0, wxALL, 5 );

	m_panel51111 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("80") );
	gSizer1->Add( m_panel51111, 1, wxEXPAND | wxALL, 5 );

	m_staticText2211 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("80"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2211->Wrap( -1 );
	gSizer1->Add( m_staticText2211, 0, wxALL, 5 );

	m_panel511111 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("100") );
	gSizer1->Add( m_panel511111, 1, wxEXPAND | wxALL, 5 );

	m_staticText22111 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("100"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22111->Wrap( -1 );
	gSizer1->Add( m_staticText22111, 0, wxALL, 5 );

	m_panel5111111 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("120") );
	gSizer1->Add( m_panel5111111, 1, wxEXPAND | wxALL, 5 );

	m_staticText221111 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("120"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText221111->Wrap( -1 );
	gSizer1->Add( m_staticText221111, 0, wxALL, 5 );

	m_panel51111111 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("140") );
	gSizer1->Add( m_panel51111111, 1, wxEXPAND | wxALL, 5 );

	m_staticText2211111 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("140"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2211111->Wrap( -1 );
	gSizer1->Add( m_staticText2211111, 0, wxALL, 5 );

	m_panel511111111 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("160") );
	gSizer1->Add( m_panel511111111, 1, wxEXPAND | wxALL, 5 );

	m_staticText22111111 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("160"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22111111->Wrap( -1 );
	gSizer1->Add( m_staticText22111111, 0, wxALL, 5 );

	m_panel5111111111 = new wxPanel( sbSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("200") );
	gSizer1->Add( m_panel5111111111, 1, wxEXPAND | wxALL, 5 );

	m_staticText221111111 = new wxStaticText( sbSizer10->GetStaticBox(), wxID_ANY, _("200"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText221111111->Wrap( -1 );
	gSizer1->Add( m_staticText221111111, 0, wxALL, 5 );


	sbSizer10->Add( gSizer1, 1, wxEXPAND, 5 );


	fgSizer5->Add( sbSizer10, 1, wxEXPAND, 5 );


	fgSizer26->Add( fgSizer5, 1, wxEXPAND, 5 );

	m_staticText20 = new wxStaticText( m_panel2, wxID_ANY, _("Direction arrows are spaced every 6 hours on storm track"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	fgSizer26->Add( m_staticText20, 0, wxALL, 5 );


	m_panel2->SetSizer( fgSizer26 );
	m_panel2->Layout();
	fgSizer26->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, _("Cyclones"), false );
	m_panel4 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer171;
	fgSizer171 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer171->AddGrowableCol( 0 );
	fgSizer171->AddGrowableRow( 1 );
	fgSizer171->SetFlexibleDirection( wxBOTH );
	fgSizer171->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer161;
	fgSizer161 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer161->AddGrowableCol( 1 );
	fgSizer161->SetFlexibleDirection( wxBOTH );
	fgSizer161->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText141 = new wxStaticText( m_panel4, wxID_ANY, _("Climatology Plugin Version"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	fgSizer161->Add( m_staticText141, 0, wxALL, 5 );

	m_stVersion = new wxStaticText( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stVersion->Wrap( -1 );
	fgSizer161->Add( m_stVersion, 0, wxALL, 5 );

	m_staticText16 = new wxStaticText( m_panel4, wxID_ANY, _("Climatology Data Directory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	fgSizer161->Add( m_staticText16, 0, wxALL, 5 );

	m_tDataDirectory = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer161->Add( m_tDataDirectory, 0, wxALL|wxEXPAND, 5 );


	fgSizer171->Add( fgSizer161, 1, wxEXPAND, 5 );

	m_htmlInformation = new wxHtmlWindow( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	fgSizer171->Add( m_htmlInformation, 0, wxALL|wxEXPAND, 5 );


	m_panel4->SetSizer( fgSizer171 );
	m_panel4->Layout();
	fgSizer171->Fit( m_panel4 );
	m_notebook1->AddPage( m_panel4, _("About"), false );

	fgSizer3->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer22->AddGrowableCol( 1 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bAboutAuthor = new wxButton( this, wxID_ANY, _("About Author"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_bAboutAuthor, 0, wxALL, 5 );

	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_bClose, 0, wxALIGN_RIGHT|wxALL, 5 );


	fgSizer3->Add( fgSizer22, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer3 );
	this->Layout();
	fgSizer3->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_notebook1->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( ClimatologyConfigDialogBase::OnPageChanged ), NULL, this );
	m_cDataType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDataTypeChoice ), NULL, this );
	m_cDataUnits->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbEnabled->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnEnabled ), NULL, this );
	m_cbOverlayMap->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateOverlayConfig ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_cbOverlayInterpolation->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbIsoBars->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sIsoBarSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpinIsobar ), NULL, this );
	m_cIsoBarStep->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateIsobar ), NULL, this );
	m_cbNumbers->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sNumbersSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_cbDirectionArrowsEnable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsBarbs->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsLength->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsWidth->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_cpDirectionArrows->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ClimatologyConfigDialogBase::OnUpdateColor ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsSize->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sDirectionArrowsSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_cbWindAtlasEnable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasSize->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sWindAtlasSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_dPStart->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesDate ), NULL, this );
	m_dPEnd->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesDate ), NULL, this );
	m_sCycloneDaySpan->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sMinWindSpeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesSpin ), NULL, this );
	m_sMaxPressure->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesSpin ), NULL, this );
	m_cbElNino->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbLaNina->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbNeutral->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbNotAvailable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbSubTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbExtraTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbRemanent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_panel51->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel511->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel5111->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel51111->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel511111->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel5111111->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel51111111->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel511111111->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel5111111111->Connect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_bAboutAuthor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnAboutAuthor ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnClose ), NULL, this );
}

ClimatologyConfigDialogBase::~ClimatologyConfigDialogBase()
{
	// Disconnect Events
	m_notebook1->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( ClimatologyConfigDialogBase::OnPageChanged ), NULL, this );
	m_cDataType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDataTypeChoice ), NULL, this );
	m_cDataUnits->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbEnabled->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnEnabled ), NULL, this );
	m_cbOverlayMap->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateOverlayConfig ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_cbOverlayInterpolation->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbIsoBars->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sIsoBarSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpinIsobar ), NULL, this );
	m_cIsoBarStep->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateIsobar ), NULL, this );
	m_cbNumbers->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sNumbersSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_cbDirectionArrowsEnable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsBarbs->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsLength->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsWidth->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_cpDirectionArrows->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ClimatologyConfigDialogBase::OnUpdateColor ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sDirectionArrowsSize->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sDirectionArrowsSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_cbWindAtlasEnable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasSize->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sWindAtlasSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdateScroll ), NULL, this );
	m_dPStart->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesDate ), NULL, this );
	m_dPEnd->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesDate ), NULL, this );
	m_sCycloneDaySpan->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateSpin ), NULL, this );
	m_sMinWindSpeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesSpin ), NULL, this );
	m_sMaxPressure->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclonesSpin ), NULL, this );
	m_cbElNino->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbLaNina->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbNeutral->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbNotAvailable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbSubTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbExtraTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_cbRemanent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdateCyclones ), NULL, this );
	m_panel51->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel511->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel5111->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel51111->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel511111->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel5111111->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel51111111->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel511111111->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_panel5111111111->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ClimatologyConfigDialogBase::OnPaintKey ), NULL, this );
	m_bAboutAuthor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnAboutAuthor ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnClose ), NULL, this );

}
