#include "PolarDialog.h"
#include "polar_pi.h"
#include "Polar.h"

#include <math.h>  
#define PI 3.14159265

PolarDialog::PolarDialog( wxWindow* parent, polar_pi* pol )
:
PolarDlg( parent )
{
	plugin = pol;
	polar = new Polar(this);

	NMEA_timer = new wxTimer (this,500);
	this->Connect( NMEA_timer->GetId(),wxEVT_TIMER, wxTimerEventHandler( PolarDialog::OnNMEAtimer ),NULL,this);
	NMEA_timeout = 5;

//	timerRPM = new wxTimer (this,501);
//	this->Connect( timerRPM->GetId(),wxEVT_TIMER, wxTimerEventHandler( PolarDialog::OnTimerRPM ),NULL,this);
}

PolarDialog::~PolarDialog(void)
{
	NMEA_timer->Stop();
 //   timerRPM->Stop();
	this->Disconnect( 500 ,wxEVT_TIMER, wxTimerEventHandler( PolarDialog::OnNMEAtimer ),NULL,this);
//	this->Disconnect( 501 ,wxEVT_TIMER, wxTimerEventHandler( PolarDialog::OnTimerRPM ),NULL,this);
//	delete timerRPM;
	delete NMEA_timer;
}

void PolarDialog::OnToggleButtonRecord( wxCommandEvent& event )
{
	setToggleButtonRecord(event.IsChecked());
}

void PolarDialog::setToggleButtonRecord(bool status)
{
	const static wxString startstop[2] = {_("Start"),_("Stop")};
	polar->input_NMEA = status;
	this->m_toggleBtnRecord->SetLabel(_("Record ")+startstop[status]);
	if(!status)
		NMEA_timer->Stop();
	else
		NMEA_timer->Start(1000);
}

void PolarDialog::OnButtonClickClearData(wxCommandEvent& event)
{
	m_gridEdit->ClearGrid();
	polar->clear_Master_pol();
	m_panelPolar->Refresh();
}
/*
void PolarDialog::OnTimerRPM(wxTimerEvent& event)
{
	if(polar->data_source_index == 1 && polar->engineRunning)
	{
		wxTimeSpan sp = wxDateTime::Now().Subtract(polar->dtRPM);
		if(sp.GetSeconds() > 5)
		{
			polar->engineRunning = false;
			polar->setEngineStatus(_T("OFF"));
		}
	}
}
*/
void PolarDialog::OnNMEAtimer(wxTimerEvent& event)          // 1 second event (NMEA only)
{
	event.Skip();
	NMEA_timer->Stop();                         
	NMEA_timeout--;

	if(NMEA_timeout == 0)                             // Take snapshot of data every 5 secs
	{
		wxString Current_Data = wxString::Format(_T("Boat.SOG:%.2f   Boat.STW:%.2f   Wind.RWS:%.2f   Wind.RWA:%.2f"), polar->Boat.SOG,
            polar->Boat.STW, polar->Wind.RWS, polar->Wind.RWA);
//			m_staticTextEngine->SetLabel(Current_Data);

        if (polar->validate_data(true)){
            polar->insert_data_to_grid();
            polar->insert_data_to_Masterpol();
			m_panelPolar->Refresh();
            polar->clear_speeds_directions();
        }
        NMEA_timeout = 5;				// five seconds between reads
	}
	NMEA_timer->Start(1000);
}
/*
void PolarDialog::PolarDlgOnInitDialog( wxInitDialogEvent& event )
{
//	m_splitter1->Unsplit();
	m_gridEdit->GetGridCornerLabelWindow()->SetFocus();
	m_gridEdit->GetGridCornerLabelWindow()->SetForegroundColour(wxColour(0,0,0));
	m_gridEdit->GetGridCornerLabelWindow()->SetLabel(wxString((_("TWA\TWS"))));
	m_gridEdit->GetGridCornerLabelWindow()->Refresh();

    event.Skip();
}
*/
void PolarDialog::OnPaintPolar( wxPaintEvent& event )
{
	wxPaintDC dc(m_panelPolar);
	polar->dc = &dc;
	polar->createDiagram(dc);
    polar->Render_Polar();
}

void PolarDialog::OnSizePolar( wxSizeEvent& event )
{
	m_panelPolar->Fit();
	m_panelPolar->Refresh();
}

void PolarDialog::OnSizePolarDlg( wxSizeEvent& event )
{
	Layout();
	Refresh();
//	m_panelPolar->Fit();
//	m_panelPolar->Refresh();
}


void PolarDialog::OnChoicePolardisplay_speed( wxCommandEvent& event )
{
	polar->display_speed = wxAtof(event.GetString());
	m_panelPolar->Refresh();
}

void PolarDialog::OnRelorTrue_Wind( wxCommandEvent& event )
{
	if(m_RelorTrue_Wind->GetSelection()== 0){
        polar->wind_display_selection = _("T");
    }
    else {
        polar->wind_display_selection = _("R");
    }

	m_panelPolar->Refresh();
}

void PolarDialog::OnChoiceWindPolar( wxCommandEvent& event )
{
	m_panelPolar->Refresh();
}

void PolarDialog::OnButtonClickFilterPolar( wxCommandEvent& event )
{
	polar->showFilterDlg();
}

void PolarDialog::OnChoiceSourcePolar( wxCommandEvent& event )
{
	polar->data_source_index = event.GetSelection();
}

void PolarDialog::OnSplitterSashPosChanged( wxSplitterEvent& event )
{
	m_panelPolar->Refresh();
	m_splitter1->GetWindow2()->Update();
}

void PolarDialog::OnChoiceMode( wxCommandEvent& event )
{
	
}

void PolarDialog::OnChoiceSource( wxCommandEvent& event )
{
	static int sel = -1;
/*
	if(event.GetSelection() == 0 && plugin->m_pLogbook_Sailing_Conditions->abrSails.Count() == 0)
	{
		wxMessageBox(_("The plugin logbookkonni isn't installed\n\nIt's needed to collect wind/speed-data from all available logbooks\n\nThe create-button is hidden and the mode isn't useable"));
		m_SourceData->SetSelection((sel==-1)?sel = 1:sel);
		return;
	}
*/
	sel = event.GetSelection();
	polar->set_Dlg_for_DataSource(sel);
}

void PolarDialog::OnGridCellChange( wxGridEvent& event )
{
	wxString cell_str = m_gridEdit->GetCellValue(event.GetRow(),event.GetCol());

	if(!cell_str.IsEmpty())
	{
		if(cell_str.Contains(_T(",")))
		{
			cell_str.Replace(_T(","),_T("."));
		}
		cell_str = wxString::Format(_T("%.2f"), wxAtof(cell_str));
        polar->load_POL_datum_str(cell_str, event.GetRow(),event.GetCol());

	}
    else
        polar->load_POL_datum_str(_("0.0"), event.GetRow(),event.GetCol());

	event.Skip();
}

void PolarDialog::OnGridLabelSelect( wxGridEvent& event)
{
    int Col = event.GetCol();
    if (Col > 0)
    {
        m_displaywind_speed->SetSelection(Col+1);
    }
    else {
        int Row = event.GetRow();
        if (Row > 0)
        {
        }
    }
    m_panelPolar->Refresh();
}

void PolarDialog::OnSizesplitter1( wxSizeEvent& event )
{
	int pos = this->m_splitter1->GetSashPosition();
	this->m_splitter1->SetSashPosition(pos+1);
	this->m_splitter1->SetSashPosition(pos);
}

void PolarDialog::OnButtonClickLoad( wxCommandEvent& event )
{
    m_button_SavePOL->Disable();
	polar->load_POL_file();
}

void PolarDialog::OnButtonClickSavePolar( wxCommandEvent& event )
{

	polar->save_POL_file();
}
