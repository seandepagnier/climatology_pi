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
	sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Month") ), wxVERTICAL );
	
	wxString m_cMonthChoices[] = { wxT("January"), wxT("Febuary"), wxT("March"), wxT("April"), wxT("May"), wxT("June"), wxT("July"), wxT("August"), wxT("September"), wxT("October"), wxT("November"), wxT("December"), wxT("All") };
	int m_cMonthNChoices = sizeof( m_cMonthChoices ) / sizeof( wxString );
	m_cMonth = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cMonthNChoices, m_cMonthChoices, 0 );
	m_cMonth->SetSelection( 0 );
	sbSizer21->Add( m_cMonth, 0, wxALL|wxEXPAND, 5 );
	
	m_sMonth = new wxSlider( this, wxID_ANY, 0, 0, 12, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizer21->Add( m_sMonth, 0, wxEXPAND, 5 );
	
	
	fgSizer1->Add( sbSizer21, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Climatology Data") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbWind = new wxCheckBox( this, wxID_ANY, wxT("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbWind->Enable( false );
	
	fgSizer4->Add( m_cbWind, 0, wxALL, 5 );
	
	m_tWind = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tWind, 0, wxALL, 5 );
	
	m_cbCurrents = new wxCheckBox( this, wxID_ANY, wxT("Currents"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCurrents->Enable( false );
	
	fgSizer4->Add( m_cbCurrents, 0, wxALL, 5 );
	
	m_tCurrent = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tCurrent, 0, wxALL, 5 );
	
	m_cbPressure = new wxCheckBox( this, wxID_ANY, wxT("Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbPressure->Enable( false );
	
	fgSizer4->Add( m_cbPressure, 0, wxALL, 5 );
	
	m_tPressure = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tPressure, 0, wxALL, 5 );
	
	m_cbSeaTemperature = new wxCheckBox( this, wxID_ANY, wxT("Sea Temperature"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSeaTemperature->Enable( false );
	
	fgSizer4->Add( m_cbSeaTemperature, 0, wxALL, 5 );
	
	m_tSeaTemperature = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_tSeaTemperature, 0, wxALL, 5 );
	
	m_cbCyclones = new wxCheckBox( this, wxID_ANY, wxT("Cyclones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCyclones->Enable( false );
	
	fgSizer4->Add( m_cbCyclones, 0, wxALL, 5 );
	
	
	sbSizer2->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	
	fgSizer1->Add( sbSizer2, 1, wxFIXED_MINSIZE, 5 );
	
	m_bConfig = new wxButton( this, wxID_ANY, wxT("Config"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bConfig, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cMonth->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_cbCurrents->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbPressure->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbSeaTemperature->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCyclones->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_bConfig->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnConfig ), NULL, this );
}

ClimatologyDialogBase::~ClimatologyDialogBase()
{
	// Disconnect Events
	m_cMonth->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ClimatologyDialogBase::OnMonth ), NULL, this );
	m_cbCurrents->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbPressure->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbSeaTemperature->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_cbCyclones->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnUpdate ), NULL, this );
	m_bConfig->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ClimatologyDialogBase::OnConfig ), NULL, this );
	
}

ClimatologyConfigDialogBase::ClimatologyConfigDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel5 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_cDataTypeChoices;
	m_cDataType = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cDataTypeChoices, 0 );
	m_cDataType->SetSelection( 0 );
	fgSizer7->Add( m_cDataType, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( m_panel5, wxID_ANY, wxT("Units"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer7->Add( m_staticText3, 0, wxALL, 5 );
	
	wxArrayString m_cDataUnitsChoices;
	m_cDataUnits = new wxChoice( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cDataUnitsChoices, 0 );
	m_cDataUnits->SetSelection( 0 );
	fgSizer7->Add( m_cDataUnits, 0, wxALL, 5 );
	
	m_cbIsoBars = new wxCheckBox( m_panel5, wxID_ANY, wxT("Iso Bars"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_cbIsoBars, 0, wxALL, 5 );
	
	m_staticText41 = new wxStaticText( m_panel5, wxID_ANY, wxT("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer7->Add( m_staticText41, 0, wxALL, 5 );
	
	m_sIsoBarSpacing = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000, 1 );
	fgSizer7->Add( m_sIsoBarSpacing, 0, wxALL, 5 );
	
	m_cbOverlayMap = new wxCheckBox( m_panel5, wxID_ANY, wxT("OverlayMap"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_cbOverlayMap, 0, wxALL, 5 );
	
	
	fgSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_cbNumbers = new wxCheckBox( m_panel5, wxID_ANY, wxT("Numbers"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7->Add( m_cbNumbers, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( m_panel5, wxID_ANY, wxT("Spacing"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer7->Add( m_staticText6, 0, wxALL, 5 );
	
	m_sNumbersSpacing = new wxSpinCtrl( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, 50 );
	fgSizer7->Add( m_sNumbersSpacing, 0, wxALL, 5 );
	
	
	m_panel5->SetSizer( fgSizer7 );
	m_panel5->Layout();
	fgSizer7->Fit( m_panel5 );
	m_notebook1->AddPage( m_panel5, wxT("Standard Displays"), true );
	m_panel41 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer61->AddGrowableCol( 1 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText7 = new wxStaticText( m_panel41, wxID_ANY, wxT("Data Directory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer61->Add( m_staticText7, 0, wxALL, 5 );
	
	m_dirWindData = new wxDirPickerCtrl( m_panel41, wxID_ANY, wxEmptyString, wxT("Folder with wind data"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer61->Add( m_dirWindData, 0, wxALL|wxEXPAND, 5 );
	
	m_cbWindNumbers = new wxCheckBox( m_panel41, wxID_ANY, wxT("Pilot Charts"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer61->Add( m_cbWindNumbers, 0, wxALL, 5 );
	
	
	m_panel41->SetSizer( fgSizer61 );
	m_panel41->Layout();
	fgSizer61->Fit( m_panel41 );
	m_notebook1->AddPage( m_panel41, wxT("Wind"), false );
	m_panel2 = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("Start Date") ), wxVERTICAL );
	
	m_dPStart = new wxDatePickerCtrl( m_panel2, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_SHOWCENTURY );
	sbSizer7->Add( m_dPStart, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer7, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("End Date") ), wxVERTICAL );
	
	m_dPEnd = new wxDatePickerCtrl( m_panel2, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	sbSizer8->Add( m_dPEnd, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer8, 1, wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( m_panel2, wxID_ANY, wxT("Min Windspeed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer5->Add( m_staticText4, 0, wxALL, 5 );
	
	m_sMinWindSpeed = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 200, 60 );
	fgSizer5->Add( m_sMinWindSpeed, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( m_panel2, wxID_ANY, wxT("Max Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer5->Add( m_staticText5, 0, wxALL, 5 );
	
	m_sMaxPressure = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1200, 1000 );
	fgSizer5->Add( m_sMaxPressure, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("Theatre") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbEastPacific = new wxCheckBox( m_panel2, wxID_ANY, wxT("East Pacific"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbEastPacific->SetValue(true); 
	fgSizer9->Add( m_cbEastPacific, 0, wxALL, 5 );
	
	m_cbWestPacific = new wxCheckBox( m_panel2, wxID_ANY, wxT("West Pacific"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbWestPacific->SetValue(true); 
	fgSizer9->Add( m_cbWestPacific, 0, wxALL, 5 );
	
	m_cbSouthPacific = new wxCheckBox( m_panel2, wxID_ANY, wxT("South Pacific"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSouthPacific->SetValue(true); 
	fgSizer9->Add( m_cbSouthPacific, 0, wxALL, 5 );
	
	m_cbAtlantic = new wxCheckBox( m_panel2, wxID_ANY, wxT("Atlantic"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbAtlantic->SetValue(true); 
	fgSizer9->Add( m_cbAtlantic, 0, wxALL, 5 );
	
	m_cbNorthIndian = new wxCheckBox( m_panel2, wxID_ANY, wxT("North Indian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbNorthIndian->SetValue(true); 
	fgSizer9->Add( m_cbNorthIndian, 0, wxALL, 5 );
	
	m_cbSouthIndian = new wxCheckBox( m_panel2, wxID_ANY, wxT("South Indian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSouthIndian->SetValue(true); 
	fgSizer9->Add( m_cbSouthIndian, 0, wxALL, 5 );
	
	
	sbSizer3->Add( fgSizer9, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("El Nino Period") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbElNino = new wxCheckBox( m_panel2, wxID_ANY, wxT("El Nino"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbElNino->SetValue(true); 
	fgSizer10->Add( m_cbElNino, 0, wxALL, 5 );
	
	m_cbLaNina = new wxCheckBox( m_panel2, wxID_ANY, wxT("La Nina"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbLaNina->SetValue(true); 
	fgSizer10->Add( m_cbLaNina, 0, wxALL, 5 );
	
	m_cbNeutral = new wxCheckBox( m_panel2, wxID_ANY, wxT("Neutral"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbNeutral->SetValue(true); 
	fgSizer10->Add( m_cbNeutral, 0, wxALL, 5 );
	
	m_cbNotAvailable = new wxCheckBox( m_panel2, wxID_ANY, wxT("N/A"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_cbNotAvailable, 0, wxALL, 5 );
	
	
	sbSizer4->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer4, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("Storm Types") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbTropical = new wxCheckBox( m_panel2, wxID_ANY, wxT("Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbTropical->SetValue(true); 
	fgSizer11->Add( m_cbTropical, 0, wxALL, 5 );
	
	m_cbSubTropical = new wxCheckBox( m_panel2, wxID_ANY, wxT("Sub Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSubTropical->SetValue(true); 
	fgSizer11->Add( m_cbSubTropical, 0, wxALL, 5 );
	
	m_cbExtraTropical = new wxCheckBox( m_panel2, wxID_ANY, wxT("ExtraTropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbExtraTropical->SetValue(true); 
	fgSizer11->Add( m_cbExtraTropical, 0, wxALL, 5 );
	
	m_cbRemanent = new wxCheckBox( m_panel2, wxID_ANY, wxT("Remanent"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer11->Add( m_cbRemanent, 0, wxALL, 5 );
	
	
	sbSizer5->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	
	fgSizer5->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( fgSizer5 );
	m_panel2->Layout();
	fgSizer5->Fit( m_panel2 );
	m_notebook1->AddPage( m_panel2, wxT("Cyclones"), false );
	
	fgSizer3->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1->Realize();
	
	fgSizer3->Add( m_sdbSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cDataType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDataTypeChoice ), NULL, this );
	m_cDataUnits->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbIsoBars->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sIsoBarSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbOverlayMap->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNumbers->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sNumbersSpacing->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_dPStart->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_dPEnd->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_sMinWindSpeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_sMaxPressure->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbEastPacific->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbWestPacific->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSouthPacific->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbAtlantic->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbNorthIndian->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSouthIndian->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbElNino->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbLaNina->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbNeutral->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbNotAvailable->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSubTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbExtraTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbRemanent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
}

ClimatologyConfigDialogBase::~ClimatologyConfigDialogBase()
{
	// Disconnect Events
	m_cDataType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnDataTypeChoice ), NULL, this );
	m_cDataUnits->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbIsoBars->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sIsoBarSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbOverlayMap->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_cbNumbers->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_sNumbersSpacing->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnUpdate ), NULL, this );
	m_dPStart->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_dPEnd->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_sMinWindSpeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_sMaxPressure->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbEastPacific->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbWestPacific->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSouthPacific->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbAtlantic->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbNorthIndian->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSouthIndian->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbElNino->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbLaNina->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbNeutral->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbNotAvailable->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSubTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbExtraTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbRemanent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ClimatologyConfigDialogBase::OnCycloneConfig ), NULL, this );
	
}
