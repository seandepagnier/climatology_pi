///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "TrendsUI.h"

///////////////////////////////////////////////////////////////////////////

TrendsDialogBase::TrendsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 1, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxVERTICAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxStaticBoxSizer* sbSizer21;
	sbSizer21 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Month") ), wxVERTICAL );
	
	wxString m_cMonthChoices[] = { wxT("January"), wxT("Febuary"), wxT("March"), wxT("April"), wxT("May"), wxT("June"), wxT("July"), wxT("August"), wxT("September"), wxT("October"), wxT("November"), wxT("December") };
	int m_cMonthNChoices = sizeof( m_cMonthChoices ) / sizeof( wxString );
	m_cMonth = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cMonthNChoices, m_cMonthChoices, 0 );
	m_cMonth->SetSelection( 0 );
	sbSizer21->Add( m_cMonth, 0, wxALL|wxEXPAND, 5 );
	
	m_sMonth = new wxSlider( this, wxID_ANY, 0, 0, 11, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	sbSizer21->Add( m_sMonth, 0, wxEXPAND, 5 );
	
	
	fgSizer1->Add( sbSizer21, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Trends Data") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBox13 = new wxCheckBox( this, wxID_ANY, wxT("Wind Pilot Charts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox13->Enable( false );
	
	fgSizer4->Add( m_checkBox13, 0, wxALL, 5 );
	
	m_cbOceanCurrents = new wxCheckBox( this, wxID_ANY, wxT("Ocean Currents"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbOceanCurrents->Enable( false );
	
	fgSizer4->Add( m_cbOceanCurrents, 0, wxALL, 5 );
	
	m_cbPressure = new wxCheckBox( this, wxID_ANY, wxT("Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbPressure->Enable( false );
	
	fgSizer4->Add( m_cbPressure, 0, wxALL, 5 );
	
	m_cbSwellPlots = new wxCheckBox( this, wxID_ANY, wxT("Swell Plots"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSwellPlots->Enable( false );
	
	fgSizer4->Add( m_cbSwellPlots, 0, wxALL, 5 );
	
	m_cbCyclones = new wxCheckBox( this, wxID_ANY, wxT("Cyclones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbCyclones->Enable( false );
	
	fgSizer4->Add( m_cbCyclones, 0, wxALL, 5 );
	
	m_cbITCZ = new wxCheckBox( this, wxID_ANY, wxT("ITCZ (doldrum)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbITCZ->Enable( false );
	
	fgSizer4->Add( m_cbITCZ, 0, wxALL, 5 );
	
	
	sbSizer2->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	
	fgSizer1->Add( sbSizer2, 1, wxFIXED_MINSIZE, 5 );
	
	m_bConfig = new wxButton( this, wxID_ANY, wxT("Config"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_bConfig, 0, wxALL, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	fgSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cMonth->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_bConfig->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TrendsDialogBase::OnConfig ), NULL, this );
}

TrendsDialogBase::~TrendsDialogBase()
{
	// Disconnect Events
	m_cMonth->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_sMonth->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TrendsDialogBase::OnMonth ), NULL, this );
	m_bConfig->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TrendsDialogBase::OnConfig ), NULL, this );
	
}

TrendsConfigDialogBase::TrendsConfigDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->AddGrowableRow( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_auinotebook1 = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE );
	m_panel1 = new wxPanel( m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_auinotebook1->AddPage( m_panel1, wxT("Wind Atlas"), false, wxNullBitmap );
	m_panel2 = new wxPanel( m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableRow( 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbTropical = new wxCheckBox( m_panel2, wxID_ANY, wxT("Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbTropical->SetValue(true); 
	fgSizer5->Add( m_cbTropical, 0, wxALL, 5 );
	
	m_cbSubTropical = new wxCheckBox( m_panel2, wxID_ANY, wxT("Sub Tropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbSubTropical->SetValue(true); 
	fgSizer5->Add( m_cbSubTropical, 0, wxALL, 5 );
	
	m_cbExtraTropical = new wxCheckBox( m_panel2, wxID_ANY, wxT("ExtraTropical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbExtraTropical->SetValue(true); 
	fgSizer5->Add( m_cbExtraTropical, 0, wxALL, 5 );
	
	m_cbRemanent = new wxCheckBox( m_panel2, wxID_ANY, wxT("Remanent"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( m_cbRemanent, 0, wxALL, 5 );
	
	m_staticText1 = new wxStaticText( m_panel2, wxID_ANY, wxT("Start Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer5->Add( m_staticText1, 0, wxALL, 5 );
	
	m_dPStart = new wxDatePickerCtrl( m_panel2, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	fgSizer5->Add( m_dPStart, 0, wxALL, 5 );
	
	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, wxT("End Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer5->Add( m_staticText2, 0, wxALL, 5 );
	
	m_datePicker2 = new wxDatePickerCtrl( m_panel2, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	fgSizer5->Add( m_datePicker2, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( m_panel2, wxID_ANY, wxT("Min Windspeed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer5->Add( m_staticText4, 0, wxALL, 5 );
	
	m_spinCtrl1 = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 200, 0 );
	fgSizer5->Add( m_spinCtrl1, 0, wxALL, 5 );
	
	m_staticText5 = new wxStaticText( m_panel2, wxID_ANY, wxT("Max Pressure"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer5->Add( m_staticText5, 0, wxALL, 5 );
	
	m_spinCtrl2 = new wxSpinCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1200, 0 );
	fgSizer5->Add( m_spinCtrl2, 0, wxALL, 5 );
	
	
	fgSizer4->Add( fgSizer5, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( fgSizer4 );
	m_panel2->Layout();
	fgSizer4->Fit( m_panel2 );
	m_auinotebook1->AddPage( m_panel2, wxT("Cyclones"), true, wxNullBitmap );
	
	fgSizer3->Add( m_auinotebook1, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cbTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSubTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbExtraTropical->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbRemanent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_dPStart->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_datePicker2->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_spinCtrl1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_spinCtrl2->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
}

TrendsConfigDialogBase::~TrendsConfigDialogBase()
{
	// Disconnect Events
	m_cbTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbSubTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbExtraTropical->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_cbRemanent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_dPStart->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_datePicker2->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_spinCtrl1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	m_spinCtrl2->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TrendsConfigDialogBase::OnCycloneConfig ), NULL, this );
	
}
