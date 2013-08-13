///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ClimatologyUI.h"

///////////////////////////////////////////////////////////////////////////

ClimatologyDialogBase::ClimatologyDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxVERTICAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxStaticBoxSizer* sbSizer21;
	sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Month") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer23;
	fgSizer23 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer23->AddGrowableCol( 0 );
	fgSizer23->SetFlexibleDirection( wxBOTH );
	fgSizer23->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxString m_cMonthChoices[] = { _("January"), _("Febuary"), _("March"), _("April"), _("May"), _("June"), _("July"), _("August"), _("September"), _("October"), _("November"), _("December"), _("All") };
	int m_cMonthNChoices = sizeof( m_cMonthChoices ) / sizeof( wxString );
	m_cMonth = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cMonthNChoices, m_cMonthChoices, 0 );
	m_cMonth->SetSelection( 0 );
	fgSizer23->Add( m_cMonth, 0, wxALL|wxEXPAND, 5 );
	
	m_cbAll = new wxCheckBox( this, wxID_ANY, _("All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer23->Add( m_cbAll, 0, wxALL, 5 );
	
	
	sbSizer21->Add( fgSizer23, 1, wxEXPAND, 5 );
	
	m_sMonth = new wxSlider( this, wxID_ANY, 0, 0, 17, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizer21->Add( m_sMonth, 0, wxEXPAND, 5 );
	
	
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
	
	m_cbWind = new wxCheckBox( this, wxID_ANY, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbWind->Enable( false );
	
	fgSizer15->Add( m_cbWind, 0, wxALL, 5 );
	
	m_tWind = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
	fgSizer15->Add( m_tWind, 0, wxALL, 5 );
	
	m_tWindDir = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( m_tWindDir, 0, wxALL, 5 );
	
	m_cbCurrent = new wxCheckBox( this, wxID_ANY, _("Current"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCurrent->Enable( false );
	
	fgSizer15->Add( m_cbCurrent, 0, wxALL, 5 );
	
	m_tCurrent = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 70,-1 ), 0 );
	fgSizer15->Add( m_tCurrent, 0, wxALL, 5 );
	
	m_tCurrentDir = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	fgSizer15->Add( m_tCurrentDir, 0, wxALL, 5 );
	
	
	fgSizer14->Add( fgSizer15, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbPressure = new wxCheckBox( this, wxID_ANY, _("Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbPressure->Enable( false );
	
	fgSizer4->Add( m_cbPressure, 0, wxALL, 5 );
	
	m_tPressure = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tPressure, 0, wxALL, 5 );
	
	m_cbSeaTemperature = new wxCheckBox( this, wxID_ANY, _("Sea Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSeaTemperature->Enable( false );
	
	fgSizer4->Add( m_cbSeaTemperature, 0, wxALL, 5 );
	
	m_tSeaTemperature = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tSeaTemperature, 0, wxALL, 5 );
	
	m_cbAirTemperature = new wxCheckBox( this, wxID_ANY, _("Air Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbAirTemperature->Enable( false );
	
	fgSizer4->Add( m_cbAirTemperature, 0, wxALL, 5 );
	
	m_tAirTemperature = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tAirTemperature, 0, wxALL, 5 );
	
	m_cbCloudCover = new wxCheckBox( this, wxID_ANY, _("Cloud Cover"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCloudCover->Enable( false );
	
	fgSizer4->Add( m_cbCloudCover, 0, wxALL, 5 );
	
	m_tCloudCover = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tCloudCover, 0, wxALL, 5 );
	
	m_cbPrecipitation = new wxCheckBox( this, wxID_ANY, _("Precipitation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbPrecipitation->Enable( false );
	
	fgSizer4->Add( m_cbPrecipitation, 0, wxALL, 5 );
	
	m_tPrecipitation = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tPrecipitation, 0, wxALL, 5 );
	
	m_cbRelativeHumidity = new wxCheckBox( this, wxID_ANY, _("Rel Humidity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbRelativeHumidity->Enable( false );
	
	fgSizer4->Add( m_cbRelativeHumidity, 0, wxALL, 5 );
	
	m_tRelativeHumidity = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tRelativeHumidity, 0, wxALL, 5 );
	
	m_cbSeaDepth = new wxCheckBox( this, wxID_ANY, _("Sea Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSeaDepth->Enable( false );
	
	fgSizer4->Add( m_cbSeaDepth, 0, wxALL, 5 );
	
	m_tSeaDepth = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tSeaDepth, 0, wxALL, 5 );
	
	m_cbCyclones = new wxCheckBox( this, wxID_ANY, _("Cyclones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCyclones->Enable( false );
	
	fgSizer4->Add( m_cbCyclones, 0, wxALL, 5 );
	
	m_bConfig = new wxButton( this, wxID_ANY, _("Config"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_bConfig, 0, wxALL, 5 );
	
	
	fgSizer14->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	
	sbSizer2->Add( fgSizer14, 1, wxEXPAND, 5 );
	
	
	fgSizer1->Add( sbSizer2, 1, wxFIXED_MINSIZE, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cMonth->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_cbAll->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnAll ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_cbWind->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCurrent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbPressure->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbSeaTemperature->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbAirTemperature->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCloudCover->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbPrecipitation->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbRelativeHumidity->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbSeaDepth->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCyclones->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_bConfig->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnConfig ), NULL, this );
}

ClimatologyDialogBase::~ClimatologyDialogBase()
{
	// Disconnect Events
	m_cMonth->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_cbAll->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnAll ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_cbWind->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCurrent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbPressure->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbSeaTemperature->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbAirTemperature->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCloudCover->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbPrecipitation->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbRelativeHumidity->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbSeaDepth->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCyclones->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
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
	
	wxArrayString m_cDataTypeChoices;
	m_cDataType = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cDataTypeChoices, 0 );
	m_cDataType->SetSelection( 0 );
	fgSizer18->Add( m_cDataType, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( m_panel5, wxID_ANY, _("Units"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer18->Add( m_staticText3, 0, wxALL, 5 );
	
	wxArrayString m_cDataUnitsChoices;
	m_cDataUnits = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cDataUnitsChoices, 0 );
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
	
	m_cbDirectionArrowsEnable = new wxCheckBox( m_panel5, wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_cbDirectionArrowsEnable, 0, wxALL, 5 );
	
	
	fgSizer16->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_rbDirectionArrowsBarbs = new wxRadioButton( m_panel5, wxID_ANY, _("Barbs"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_rbDirectionArrowsBarbs, 0, wxALL, 5 );
	
	m_rbDirectionArrowsLength = new wxRadioButton( m_panel5, wxID_ANY, _("Length"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer16->Add( m_rbDirectionArrowsLength, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( m_panel5, wxID_ANY, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer16->Add( m_staticText7, 0, wxALL, 5 );
	
	m_sDirectionArrowsWidth = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 5, 3 );
	fgSizer16->Add( m_sDirectionArrowsWidth, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( m_panel5, wxID_ANY, _("Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer16->Add( m_staticText8, 0, wxALL, 5 );
	
	m_cpDirectionArrows = new wxColourPickerCtrl( m_panel5, wxID_ANY, wxColour( 0, 221, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer16->Add( m_cpDirectionArrows, 0, wxALL, 5 );
	
	m_staticText9 = new wxStaticText( m_panel5, wxID_ANY, _("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer16->Add( m_staticText9, 0, wxALL, 5 );
	
	m_sDirectionArrowsOpacity = new wxSlider( m_panel5, wxID_ANY, 205, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer16->Add( m_sDirectionArrowsOpacity, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText11 = new wxStaticText( m_panel5, wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer16->Add( m_staticText11, 0, wxALL, 5 );
	
	m_sDirectionArrowsSize = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 30 );
	fgSizer16->Add( m_sDirectionArrowsSize, 0, wxALL, 5 );
	
	m_staticText10 = new wxStaticText( m_panel5, wxID_ANY, _("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer16->Add( m_staticText10, 0, wxALL, 5 );
	
	m_sDirectionArrowsSpacing = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1000, 100 );
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
	
	m_cbWindAtlasEnable = new wxCheckBox( m_panel41, wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer17->Add( m_cbWindAtlasEnable, 0, wxALL, 5 );
	
	
	fgSizer17->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText12 = new wxStaticText( m_panel41, wxID_ANY, _("Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer17->Add( m_staticText12, 0, wxALL, 5 );
	
	m_sWindAtlasSize = new wxSpinCtrl( m_panel41, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 5, 500, 60 );
	fgSizer17->Add( m_sWindAtlasSize, 0, wxALL, 5 );
	
	m_staticText13 = new wxStaticText( m_panel41, wxID_ANY, _("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	fgSizer17->Add( m_staticText13, 0, wxALL, 5 );
	
	m_sWindAtlasSpacing = new wxSpinCtrl( m_panel41, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, 80 );
	fgSizer17->Add( m_sWindAtlasSpacing, 0, wxALL, 5 );
	
	m_staticText14 = new wxStaticText( m_panel41, wxID_ANY, _("Opacity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer17->Add( m_staticText14, 0, wxALL, 5 );
	
	m_sWindAtlasOpacity = new wxSlider( m_panel41, wxID_ANY, 205, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer17->Add( m_sWindAtlasOpacity, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer24->Add( fgSizer17, 1, wxEXPAND, 5 );
	
	m_staticText19 = new wxStaticText( m_panel41, wxID_ANY, _("The wind atlas display shows the percentage of time wind is in each of eight directions as a length of the arrow, or in the case it is 35% or more a number is shown.\n\nThe barbs indicate wind speed, each barb being 5 knots.\n\nThe numbers in the circle, give percentage of calm (less than 3 knots) in blue or storm (above 47 knots) in red."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( 240 );
	fgSizer24->Add( m_staticText19, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer9->Add( fgSizer24, 1, wxEXPAND, 5 );
	
	
	fgSizer61->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	
	m_panel41->SetSizer( fgSizer61 );
	m_panel41->Layout();
	fgSizer61->Fit( m_panel41 );
	m_notebook1->AddPage( m_panel41, _("Wind"), false );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Start Date") ), wxVERTICAL );
	
	m_dPStart = new wxDatePickerCtrl( m_panel2, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_SHOWCENTURY );
	sbSizer7->Add( m_dPStart, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("End Date") ), wxVERTICAL );
	
	m_dPEnd = new wxDatePickerCtrl( m_panel2, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	sbSizer8->Add( m_dPEnd, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer8, 1, wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( m_panel2, wxID_ANY, _("Min Windspeed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer5->Add( m_staticText4, 0, wxALL, 5 );
	
	m_sMinWindSpeed = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 200, 60 );
	fgSizer5->Add( m_sMinWindSpeed, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( m_panel2, wxID_ANY, _("Max Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer5->Add( m_staticText5, 0, wxALL, 5 );
	
	m_sMaxPressure = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1200, 1000 );
	fgSizer5->Add( m_sMaxPressure, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Theatre") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbEastPacific = new wxCheckBox( m_panel2, wxID_ANY, _("East Pacific"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbEastPacific->SetValue(true); 
	fgSizer9->Add( m_cbEastPacific, 0, wxALL, 5 );
	
	m_cbWestPacific = new wxCheckBox( m_panel2, wxID_ANY, _("West Pacific"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbWestPacific->SetValue(true); 
	fgSizer9->Add( m_cbWestPacific, 0, wxALL, 5 );
	
	m_cbSouthPacific = new wxCheckBox( m_panel2, wxID_ANY, _("South Pacific"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSouthPacific->SetValue(true); 
	fgSizer9->Add( m_cbSouthPacific, 0, wxALL, 5 );
	
	m_cbAtlantic = new wxCheckBox( m_panel2, wxID_ANY, _("Atlantic"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbAtlantic->SetValue(true); 
	fgSizer9->Add( m_cbAtlantic, 0, wxALL, 5 );
	
	m_cbNorthIndian = new wxCheckBox( m_panel2, wxID_ANY, _("North Indian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbNorthIndian->SetValue(true); 
	fgSizer9->Add( m_cbNorthIndian, 0, wxALL, 5 );
	
	m_cbSouthIndian = new wxCheckBox( m_panel2, wxID_ANY, _("South Indian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSouthIndian->SetValue(true); 
	fgSizer9->Add( m_cbSouthIndian, 0, wxALL, 5 );
	
	
	sbSizer3->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("El Nino Period") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbElNino = new wxCheckBox( m_panel2, wxID_ANY, _("El Nino"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbElNino->SetValue(true); 
	fgSizer10->Add( m_cbElNino, 0, wxALL, 5 );
	
	m_cbLaNina = new wxCheckBox( m_panel2, wxID_ANY, _("La Nina"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbLaNina->SetValue(true); 
	fgSizer10->Add( m_cbLaNina, 0, wxALL, 5 );
	
	m_cbNeutral = new wxCheckBox( m_panel2, wxID_ANY, _("Neutral"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbNeutral->SetValue(true); 
	fgSizer10->Add( m_cbNeutral, 0, wxALL, 5 );
	
	m_cbNotAvailable = new wxCheckBox( m_panel2, wxID_ANY, _("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_cbNotAvailable, 0, wxALL, 5 );
	
	
	sbSizer4->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Storm Types") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbTropical = new wxCheckBox( m_panel2, wxID_ANY, _("Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbTropical->SetValue(true); 
	fgSizer11->Add( m_cbTropical, 0, wxALL, 5 );
	
	m_cbSubTropical = new wxCheckBox( m_panel2, wxID_ANY, _("Sub Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSubTropical->SetValue(true); 
	fgSizer11->Add( m_cbSubTropical, 0, wxALL, 5 );
	
	m_cbExtraTropical = new wxCheckBox( m_panel2, wxID_ANY, _("ExtraTropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbExtraTropical->SetValue(true); 
	fgSizer11->Add( m_cbExtraTropical, 0, wxALL, 5 );
	
	m_cbRemanent = new wxCheckBox( m_panel2, wxID_ANY, _("Remanent"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_cbRemanent, 0, wxALL, 5 );
	
	
	sbSizer5->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( fgSizer5 );
	m_panel2->Layout();
	fgSizer5->Fit( m_panel2 );
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
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bOnDonate = new wxButton( this, wxID_ANY, _("Donate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_bOnDonate, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer22->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer3->Add( fgSizer22, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_notebook1->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( ClimatologyConfigDialogBase::OnPageChanged ), NULL, this );
	m_cDataType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDataTypeChoice ), NULL, this );
	m_cDataUnits->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbEnabled->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnEnabled ), NULL, this );
	m_cbOverlayMap->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbIsoBars->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sIsoBarSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cIsoBarStep->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNumbers->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sNumbersSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbDirectionArrowsEnable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsBarbs->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsLength->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsWidth->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cpDirectionArrows->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsSize->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbWindAtlasEnable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasSize->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_dPStart->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_dPEnd->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sMinWindSpeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sMaxPressure->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbEastPacific->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbWestPacific->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbSouthPacific->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbAtlantic->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNorthIndian->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbSouthIndian->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbElNino->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbLaNina->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNeutral->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNotAvailable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbSubTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbExtraTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbRemanent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_bOnDonate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDonate ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnClose ), NULL, this );
}

ClimatologyConfigDialogBase::~ClimatologyConfigDialogBase()
{
	// Disconnect Events
	m_notebook1->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( ClimatologyConfigDialogBase::OnPageChanged ), NULL, this );
	m_cDataType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDataTypeChoice ), NULL, this );
	m_cDataUnits->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbEnabled->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnEnabled ), NULL, this );
	m_cbOverlayMap->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sOverlayTransparency->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbIsoBars->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sIsoBarSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cIsoBarStep->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNumbers->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sNumbersSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbDirectionArrowsEnable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsBarbs->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_rbDirectionArrowsLength->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsWidth->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cpDirectionArrows->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsOpacity->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsSize->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sDirectionArrowsSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbWindAtlasEnable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasSize->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sWindAtlasOpacity->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_dPStart->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_dPEnd->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sMinWindSpeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sMaxPressure->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbEastPacific->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbWestPacific->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbSouthPacific->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbAtlantic->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNorthIndian->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbSouthIndian->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbElNino->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbLaNina->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNeutral->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNotAvailable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbSubTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbExtraTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbRemanent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_bOnDonate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDonate ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnClose ), NULL, this );
	
}
