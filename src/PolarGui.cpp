///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PolarGui.h"
#include "Polar.h"
///////////////////////////////////////////////////////////////////////////

PolarDlg::PolarDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer( wxVERTICAL );

//********************* Overhead Banner *********************************	
	bSizer_Banner = new wxBoxSizer( wxHORIZONTAL );
	
	txtAveragingMode = new wxStaticText( this, wxID_ANY, _("Data mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtAveragingMode->Wrap( -1 );
	bSizer_Banner->Add( txtAveragingMode, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxString m_choicesMode[] = { _("New"), _("Add")};
	int m_numberMode = sizeof( m_choicesMode ) / sizeof( wxString );
	m_Mode = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_numberMode, m_choicesMode, 0 );
	m_Mode->SetSelection(0 );
	m_Mode->SetToolTip( _("New polar or add to current one") );
	bSizer_Banner->Add( m_Mode, 0, wxALL, 5 );

	m_staticSourceData = new wxStaticText( this, wxID_ANY, _("Data Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticSourceData->Wrap( -1 );
	bSizer_Banner->Add( m_staticSourceData, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxString m_choiceSourceData[] = { _("Data from Logbook file"), _("Data from NMEA port"), _("Data from VDR file"), _("Manual Data Entry "),  _("Data from STE file"), };
	int m_numberDataChoices = sizeof( m_choiceSourceData ) / sizeof( wxString );
	m_SourceData = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_numberDataChoices, m_choiceSourceData, 0 );
	m_SourceData->SetSelection(1 );
	bSizer_Banner->Add( m_SourceData, 0, wxALL, 5 );
    
    m_staticTextTopSpeed = new wxStaticText( this, wxID_ANY, _("Top Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTopSpeed->Wrap( -1 );
	bSizer_Banner->Add( m_staticTextTopSpeed, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxString m_choicesTopSpeed[] = {_T("10"), _T("15"), _T("20"), _T("25") };
	int m_numberTopSpeedChoices = sizeof( m_choicesTopSpeed ) / sizeof( wxString );
	m_TopSpeed = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_numberTopSpeedChoices, m_choicesTopSpeed, 0 );
	m_TopSpeed->SetSelection( 0 );
	m_TopSpeed->SetToolTip( _("Set diagramm to x knots") );	
	bSizer_Banner->Add( m_TopSpeed, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticTextwind_dirInterval = new wxStaticText( this, wxID_ANY, _("Wind:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextwind_dirInterval->Wrap( -1 );
	bSizer_Banner->Add( m_staticTextwind_dirInterval, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxString m_choices_wind_dirs[] = { _T("True"), _T("Relative") };
	int m_number_wind_dirChoices = sizeof( m_choices_wind_dirs ) / sizeof( wxString );
	m_RelorTrue_Wind = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_number_wind_dirChoices, m_choices_wind_dirs, 0 );
	m_RelorTrue_Wind->SetSelection( 1 );
	m_RelorTrue_Wind->SetToolTip( _("True needs Boat.SOG input. ") );	
	bSizer_Banner->Add( m_RelorTrue_Wind, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText148 = new wxStaticText( this, wxID_ANY, _("Wind:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText148->Wrap( -1 );
	bSizer_Banner->Add( m_staticText148, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxString m_choices_displaywind_speed[] = { _("All") };
	int m_displaywind_speedNChoices = sizeof( m_choices_displaywind_speed ) / sizeof( wxString );
	m_displaywind_speed = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_displaywind_speedNChoices, m_choices_displaywind_speed, 0 );
	m_displaywind_speed->SetSelection( 0 );
	bSizer_Banner->Add( m_displaywind_speed, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_staticText146 = new wxStaticText( this, wxID_ANY, _("Speed:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText146->Wrap( -1 );
	bSizer_Banner->Add( m_staticText146, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );
	
	wxString m_choices_sourceSpeed[] = { _("Boat.SOG"), _("Boat.STW") };
	int m_sourceSpeedNChoices = sizeof( m_choices_sourceSpeed ) / sizeof( wxString );
	m_sourceSpeed = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_sourceSpeedNChoices, m_choices_sourceSpeed, 0 );
	m_sourceSpeed->SetSelection( 0 );
	m_sourceSpeed->SetToolTip( _(" Boat.STW - Speed through water\nBoat.SOG - Speed over ground.") );	
	bSizer_Banner->Add( m_sourceSpeed, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	m_buttonFilterPolar = new wxButton( this, wxID_ANY, _("Filter"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer_Banner->Add( m_buttonFilterPolar, 0, wxALL, 5 );
	
	bSizer48->Add( bSizer_Banner, 0, 0, 5 );

//**************** Graph and Grid ****************************************************
	m_staticline45 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer48->Add( m_staticline45, 0, wxEXPAND | wxALL, 0 );
	
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_3DBORDER|wxSP_3DSASH|wxSP_LIVE_UPDATE|wxSP_NO_XP_THEME );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( PolarDlg::m_splitter1OnIdle ), NULL, this );
	m_splitter1->SetMinimumPaneSize( 10 );
	
	m_panelPolar = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelPolar->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	m_panelPolar->SetMinSize( wxSize( 10,-1 ) );
	
	m_panel_Grid = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel_Grid->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
	
	wxBoxSizer* bSizer_Data_input;
	bSizer_Data_input = new wxBoxSizer( wxVERTICAL );
	
	m_gridEdit = new wxGrid( m_panel_Grid, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gridEdit->CreateGrid( 32, 10 );
	m_gridEdit->EnableEditing( true );
	m_gridEdit->EnableGridLines( true );
	m_gridEdit->EnableDragGridSize( false );
	m_gridEdit->SetMargins( 0, 0 );
	
	// Columns
	m_gridEdit->SetColSize( 0, 40 );
	m_gridEdit->SetColSize( 1, 40 );
	m_gridEdit->SetColSize( 2, 40 );
	m_gridEdit->SetColSize( 3, 40 );
	m_gridEdit->SetColSize( 4, 40 );
	m_gridEdit->SetColSize( 5, 40 );
	m_gridEdit->SetColSize( 6, 40 );
	m_gridEdit->SetColSize( 7, 40 );
	m_gridEdit->SetColSize( 8, 40 );
	m_gridEdit->SetColSize( 9, 40 );
	m_gridEdit->EnableDragColMove( false );
	m_gridEdit->EnableDragColSize( true );
	m_gridEdit->SetColLabelSize( 30 );
	m_gridEdit->SetColLabelValue( 0, _T("5") );
	m_gridEdit->SetColLabelValue( 1, _T("10") );
	m_gridEdit->SetColLabelValue( 2, _T("15") );
	m_gridEdit->SetColLabelValue( 3, _T("20") );
	m_gridEdit->SetColLabelValue( 4, _T("25") );
	m_gridEdit->SetColLabelValue( 5, _T("30") );
	m_gridEdit->SetColLabelValue( 6, _T("35") );
	m_gridEdit->SetColLabelValue( 7, _T("40") );
	m_gridEdit->SetColLabelValue( 8, _T("45") );
	m_gridEdit->SetColLabelValue( 9, _T("50") );
	m_gridEdit->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gridEdit->EnableDragRowSize( true );
	m_gridEdit->SetRowLabelSize( 50 );
	m_gridEdit->SetRowLabelValue( 0, _T("25\xB0") );
	m_gridEdit->SetRowLabelValue( 1, _T("30\xB0") );
	m_gridEdit->SetRowLabelValue( 2, _T("35\xB0") );
	m_gridEdit->SetRowLabelValue( 3, _T("40\xB0") );
	m_gridEdit->SetRowLabelValue( 4, _T("45\xB0") );
	m_gridEdit->SetRowLabelValue( 5, _T("50\xB0") );
	m_gridEdit->SetRowLabelValue( 6, _T("55\xB0") );
	m_gridEdit->SetRowLabelValue( 7, _T("60\xB0") );
	m_gridEdit->SetRowLabelValue( 8, _T("65\xB0") );
	m_gridEdit->SetRowLabelValue( 9, _T("70\xB0") );
	m_gridEdit->SetRowLabelValue( 10, _T("75\xB0") );
	m_gridEdit->SetRowLabelValue( 11, _T("80\xB0") );
	m_gridEdit->SetRowLabelValue( 12, _T("85\xB0") );
	m_gridEdit->SetRowLabelValue( 13, _T("90\xB0") );
	m_gridEdit->SetRowLabelValue( 14, _T("95\xB0") );
	m_gridEdit->SetRowLabelValue( 15, _T("100\xB0") );
	m_gridEdit->SetRowLabelValue( 16, _T("105\xB0") );
	m_gridEdit->SetRowLabelValue( 17, _T("110\xB0") );
	m_gridEdit->SetRowLabelValue( 18, _T("115\xB0") );
	m_gridEdit->SetRowLabelValue( 19, _T("120\xB0") );
	m_gridEdit->SetRowLabelValue( 20, _T("125\xB0") );
    m_gridEdit->SetRowLabelValue( 21, _T("130\xB0") );
	m_gridEdit->SetRowLabelValue( 22, _T("135\xB0") );
    m_gridEdit->SetRowLabelValue( 23, _T("140\xB0") );
	m_gridEdit->SetRowLabelValue( 24, _T("145\xB0") );
    m_gridEdit->SetRowLabelValue( 25, _T("150\xB0") );
	m_gridEdit->SetRowLabelValue( 26, _T("155\xB0") );
    m_gridEdit->SetRowLabelValue( 27, _T("160\xB0") );
	m_gridEdit->SetRowLabelValue( 28, _T("165\xB0") );
    m_gridEdit->SetRowLabelValue( 29, _T("170\xB0") );
	m_gridEdit->SetRowLabelValue( 30, _T("175\xB0") );
    m_gridEdit->SetRowLabelValue( 31, _T("180\xB0") );
    m_gridEdit->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gridEdit->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_CENTRE );
	bSizer_Data_input->Add( m_gridEdit, 0, wxALL|wxEXPAND, 0 );
	
	m_staticTextEngine = new wxStaticText( m_panel_Grid, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEngine->Wrap( -1 );
	bSizer_Data_input->Add( m_staticTextEngine, 0, wxALL|wxALIGN_LEFT, 5 );

	
    m_staticTextRecord = new wxTextCtrl( m_panel_Grid, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300,50), wxTE_MULTILINE);
	bSizer_Data_input->Add( m_staticTextRecord, 0, wxALL|wxALIGN_LEFT, 5 );
  /*
    m_staticTextFieldData = new wxTextCtrl( m_panel_Grid, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(600,100), wxTE_MULTILINE);
	bSizer_Data_input->Add( m_staticTextFieldData, 0, wxALL|wxALIGN_LEFT, 5 );
*/
 //   bSizer_Data_input->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerActions;
	bSizerActions = new wxBoxSizer( wxHORIZONTAL );

	m_staticActions = new wxStaticText( m_panel_Grid, wxID_ANY, _("File Action"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticActions->Wrap( -1 );
	bSizerActions->Add( m_staticActions, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_button_SavePOL = new wxButton( m_panel_Grid, wxID_ANY, _("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerActions->Add( m_button_SavePOL, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_button_LoadPOL = new wxButton( m_panel_Grid, wxID_ANY, _("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerActions->Add( m_button_LoadPOL, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
    m_buttonClearData = new wxButton( m_panel_Grid, wxID_ANY, _("Clear Data"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerActions->Add( m_buttonClearData, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_toggleBtnRecord = new wxToggleButton( m_panel_Grid, wxID_ANY, _("Start Recording"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerActions->Add( m_toggleBtnRecord, 0, wxALL, 5 );	
	
	bSizer_Data_input->Add( bSizerActions, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	m_panel_Grid->SetSizer( bSizer_Data_input );
	m_panel_Grid->Layout();
	bSizer_Data_input->Fit( m_panel_Grid );
	m_splitter1->SplitVertically( m_panelPolar, m_panel_Grid, 10 );
	bSizer48->Add( m_splitter1, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer48, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( PolarDlg::OnSizePolarDlg ) );
	m_Mode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceSource ), NULL, this );
    m_SourceData->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceSource ), NULL, this );
	m_TopSpeed->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoicePolardisplay_speed ), NULL, this );
	m_RelorTrue_Wind->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnRelorTrue_Wind ), NULL, this );
	m_displaywind_speed->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceWindPolar ), NULL, this );
	m_sourceSpeed->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceSourcePolar ), NULL, this );
	m_buttonFilterPolar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickFilterPolar ), NULL, this );

	m_button_SavePOL->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickSavePolar ), NULL, this );
	m_button_LoadPOL->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickLoad ), NULL, this );
	m_splitter1->Connect( wxEVT_SIZE, wxSizeEventHandler( PolarDlg::OnSizesplitter1 ), NULL, this );
	m_splitter1->Connect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( PolarDlg::OnSplitterSashPosChanged ), NULL, this );
	m_panelPolar->Connect( wxEVT_PAINT, wxPaintEventHandler( PolarDlg::OnPaintPolar ), NULL, this );
	m_gridEdit->Connect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( PolarDlg::OnGridCellChange ), NULL, this );
	m_buttonClearData->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickClearData ), NULL, this );
	m_toggleBtnRecord->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnToggleButtonRecord ), NULL, this );
}

PolarDlg::~PolarDlg()
{
	// Disconnect Events
//	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( PolarDlg::PolarDlgOnInitDialog ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PolarDlg::OnSizePolarDlg ) );
	m_Mode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceSource ), NULL, this );
	m_SourceData->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceSource ), NULL, this );
    m_TopSpeed->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoicePolardisplay_speed ), NULL, this );
	m_RelorTrue_Wind->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnRelorTrue_Wind ), NULL, this );
	m_displaywind_speed->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceWindPolar ), NULL, this );
	m_sourceSpeed->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PolarDlg::OnChoiceSourcePolar ), NULL, this );
	m_buttonFilterPolar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickFilterPolar ), NULL, this );

	m_button_SavePOL->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickSavePolar ), NULL, this );
	m_button_LoadPOL->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickLoad ), NULL, this );
	m_splitter1->Disconnect( wxEVT_SIZE, wxSizeEventHandler( PolarDlg::OnSizesplitter1 ), NULL, this );
	m_splitter1->Disconnect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( PolarDlg::OnSplitterSashPosChanged ), NULL, this );
	m_panelPolar->Disconnect( wxEVT_PAINT, wxPaintEventHandler( PolarDlg::OnPaintPolar ), NULL, this );
	m_gridEdit->Disconnect( wxEVT_GRID_CELL_CHANGE, wxGridEventHandler( PolarDlg::OnGridCellChange ), NULL, this );
	m_buttonClearData->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnButtonClickClearData ), NULL, this );
	m_toggleBtnRecord->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( PolarDlg::OnToggleButtonRecord ), NULL, this );
	
}

