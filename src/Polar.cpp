#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define PI 3.14159265

#include <wx/dir.h>
#include <wx/filefn.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/wfstream.h> 
#include <wx/txtstrm.h> 
#include <wx/math.h>
#include <wx/stdpaths.h>
#include <wx/progdlg.h>
#include <wx/gdicmn.h>

#include "nmea0183/nmea0183.h"
#include "polar_pi.h"
#include "PolarDialog.h"
#include "Polar.h"
#include "Options.h"

#include <map>

Polar::Polar(PolarDialog* parent)
{
	dlg = parent;

	windColour[0] = wxTheColourDatabase->Find(_T("YELLOW"));
	windColour[1] = wxTheColourDatabase->Find(_T("CORAL"));
	windColour[2] = wxTheColourDatabase->Find(_T("CYAN"));
	windColour[3] = wxTheColourDatabase->Find(_T("LIGHT BLUE"));
	windColour[4] = wxTheColourDatabase->Find(_T("CORNFLOWER BLUE"));
	windColour[5] = wxTheColourDatabase->Find(_T("GREEN"));
	windColour[6] = wxTheColourDatabase->Find(_T("BROWN"));
	windColour[7] = wxTheColourDatabase->Find(_T("RED"));
	windColour[8] = wxTheColourDatabase->Find(_T("VIOLET RED"));
	windColour[9] = wxTheColourDatabase->Find(_T("VIOLET"));

	input_NMEA = false;
	engineRunning = false;
	dtRPM = wxDateTime::Now();

    data_source_index = 1;       //NMEA input
	clear_speeds_directions();   // clear input variables

	filterDlg = new FilterDlg(dlg,this);

	dlg->m_TopSpeed->SetSelection(0);   // all wind speeds

	display_speed = wxAtoi(dlg->m_TopSpeed->GetString(0));
	display_inc_deg = 10 ;
    max_boat_speed = 30;       // hull speed
    set_drift_calc = 0;

	for(int i = 5; i < 41; i += 5)      // wind speed choices for display
		dlg->m_displaywind_speed->Append(wxString::Format(_T("%i knots"),i));

	dlg->m_displaywind_speed->Append(_T("Max. only"));
	dlg->m_displaywind_speed->SetSelection(0);

	dlg->m_panelPolar->Layout();

	wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
#ifdef __WXMSW__
	wxString stdPath  = std_path.GetConfigDir();
#endif
#ifdef __WXGTK__
	wxString stdPath  = std_path.GetUserDataDir();
#endif
#ifdef __WXOSX__
	wxString stdPath  = std_path.GetUserConfigDir();   // should be ~/Library/Preferences	
#endif

	wxString basePath = stdPath   + wxFileName::GetPathSeparator();
	basePath += _T("plugins") + wxString(wxFileName::GetPathSeparator());
	basePath += _T("logbook") + wxString(wxFileName::GetPathSeparator());
	basePath += _T("data") + wxString(wxFileName::GetPathSeparator());
	logbookDataPath  = basePath;

	clear_Master_pol();
//    sails.Clear();
//	filterSails = false;
}

Polar::~Polar(void)
{
	input_NMEA = false;
	delete filterDlg;
}

void Polar::load_POL_datum_str(wxString boat_speed, int i_wdirs, int i_wspds)
{                                           // TODO Look to calculate averages
	boat_speed.Replace(_T(","),_T("."));
	double speed = wxAtof(boat_speed);
	if(speed > 0.0)
	{
		Master_pol[i_wspds].wdirMax[i_wdirs] = speed;
		Master_pol[i_wspds].count[i_wdirs] = 1;
	}
	else
	{ // entry deleted????????????????????????????????????????????????????
		Master_pol[i_wspds].wdirMax[i_wdirs] = -1;
		Master_pol[i_wspds].count[i_wdirs] = 0;
	}

	dlg->m_panelPolar->Refresh();
}

void Polar::set_Dlg_for_DataSource(int data_source_index)        // Data Source
{
    int sash = dlg->m_splitter1->GetSize().GetWidth() - dlg->m_gridEdit->GetSize().GetWidth();

    if( dlg->m_Mode->GetSelection() == 0)    // New or continuing process               
    {
        dlg->m_gridEdit->ClearGrid();
		clear_Master_pol();
        dlg->m_panelPolar->Refresh();
    }

	switch(data_source_index)
	{
	case 0:	      // logbook   TODO Load something?????
		if(!dlg->m_splitter1->IsSplit()){
			dlg->m_splitter1->SplitVertically(dlg->m_panelPolar,dlg->m_panel_Grid,sash);
			dlg->m_splitter1->SetSashPosition(400);
        }

        dlg->m_buttonFilterPolar->Show();
		dlg->m_sourceSpeed->Enable();
        dlg->m_button_SavePOL->Show();
        dlg->m_button_LoadPOL->Hide();
		dlg->m_toggleBtnRecord->Hide();

		if(dlg->NMEA_timer->IsRunning())
			dlg->NMEA_timer->Stop();
		input_NMEA = false;	

        load_Logbook();
		break;
	case 1:	       //NMEA                                     // NMEA 
        if(!dlg->m_splitter1->IsSplit())
		{
			dlg->m_splitter1->SplitVertically(dlg->m_panelPolar,dlg->m_panel_Grid,sash);
			dlg->m_splitter1->SetSashPosition(400);
		}
        dlg->m_buttonFilterPolar->Show();
		dlg->m_sourceSpeed->Enable();
        dlg->m_button_SavePOL->Show();
        dlg->m_button_LoadPOL->Show();
        dlg->m_toggleBtnRecord->Show();

        dlg->NMEA_timer->Start(1000);
		dlg->timerRPM->Start(5000);
		
        input_NMEA = true;
        break;

	case 2:        //VDR                                     // VDR
		if(!dlg->m_splitter1->IsSplit())
		{
			dlg->m_splitter1->SplitVertically(dlg->m_panelPolar,dlg->m_panel_Grid,sash);
			dlg->m_splitter1->SetSashPosition(400);
		}
        dlg->m_buttonFilterPolar->Show();
		dlg->m_sourceSpeed->Enable();
        dlg->m_button_SavePOL->Show();
        dlg->m_button_LoadPOL->Show();
		dlg->m_toggleBtnRecord->Hide();

        if(dlg->NMEA_timer->IsRunning())
		dlg->NMEA_timer->Stop();

		loadVDR();

		break;
	case 3:	                                            //Manual Edit
		if(!dlg->m_splitter1->IsSplit())
		{
			dlg->m_splitter1->SplitVertically(dlg->m_panelPolar,dlg->m_panel_Grid,sash);
			dlg->m_splitter1->SetSashPosition(400);
		}
        dlg->m_buttonFilterPolar->Hide();
		dlg->m_sourceSpeed->Disable();
        dlg->m_button_SavePOL->Show();
        dlg->m_button_LoadPOL->Show();
		dlg->m_toggleBtnRecord->Hide();
		
		if(dlg->NMEA_timer->IsRunning())
			dlg->NMEA_timer->Stop();

		input_NMEA = false;
		break;

    case 4:                                             // STE
		if(!dlg->m_splitter1->IsSplit())
		{
			dlg->m_splitter1->SplitVertically(dlg->m_panelPolar,dlg->m_panel_Grid,sash);
			dlg->m_splitter1->SetSashPosition(400);
		}

        dlg->m_buttonFilterPolar->Show();
        dlg->m_sourceSpeed->Enable();
        dlg->m_button_SavePOL->Show();
        dlg->m_button_LoadPOL->Show();
		dlg->m_toggleBtnRecord->Hide();

        if(dlg->NMEA_timer->IsRunning())
			dlg->NMEA_timer->Stop();

        input_NMEA = false;
		loadSTE();
		break;
	}
}

void Polar::load_Logbook()
{ /*
//	::SendPluginMessage(wxString(_T("Save Polar Logbook.txt")),wxString(_T("")));

	wxArrayString files;												
	wxDir dir;
	dir.GetAllFiles(logbookDataPath,&files,_T("*logbook*.txt"),wxDIR_FILES);				// collect all logbook-files
	bool nextline = false;

	clear_Master_pol();

	wxProgressDialog progressRead( _("Please wait."), _T(""),
		100 , dlg,  wxFRAME_NO_TASKBAR | wxPD_AUTO_HIDE );
	progressRead.Fit();  

	for(unsigned int i = 0; i < files.Count(); i++) 
	{
		wxFileName fn(files[i]);
		progressRead.Update(((i+1)/files.GetCount())*100,_("Collecting historical data: ")+fn.GetFullName());
		progressRead.Fit();
		::wxSafeYield();


		wxFileInputStream stream( files[i] );					// go through all the files
		wxTextInputStream in(stream);	
		wxString wdirstr,wspdstr;

        wxString input_line;
		wxStringTokenizer tkz(input_line,_T("\t"),wxTOKEN_RET_EMPTY);	// tab delimination

        while(!stream.Eof())
		{
			input_line = in.ReadLine();
			if(stream.Eof()) break;

			int token_count = 0;

			bool rel = false;
			nextline = false;
            clear_speeds_directions();

//********** Parse Data lines *******************

			while(tkz.HasMoreTokens())
			{
				wxString m_str = tkz.GetNextToken();
				switch(token_count)
				{
				case 7: // Sign "S"
					if(m_str.Trim() != _T("S")) { nextline = true; break; }		// only at sea , no harbour,mooring etc., else read next line
					break;

				case 14: // Boat.SOG												// get Boat.SOG from GPS, if speed=0 get next line
					m_str.Replace(_T(","),_T("."));
					Boat.SOG = wxAtof(m_str);
					if(Boat.SOG == 0.0) { nextline = true; break; }	
					break;

				case 15: // Boat.STW												// get Boat.STW from Speedo, if speed=0 get next line
					m_str.Replace(_T(","),_T("."));
					Boat.STW = wxAtof(m_str);
					if(Boat.STW == 0.0) { nextline = true; break; }
					break;

				case 19: // WindDirection
					if(m_str.Trim().IsEmpty()) 
					{ wind_dir = -1; nextline = true; break; }			// if winddirection is empty, get next line

					m_str.Replace(_T(","),_T("."));
					wind_dir = wxAtoi(m_str);
                    if(wind_dir > 180 && wind_dir < 361)
                        wind_dir = 360 - wind_dir;       // only map one side of boat
					if(m_str.Contains(_T("R")))
						rel = true;
					break;

				case 20: // wind_speed					// add wind_speed in struct p[winddirection]
					{
						m_str.Replace(_T(","),_T("."));
						double wspd = wxAtof(m_str);
						if(wspd == 0.0) { nextline = true; break; }

						if(m_str.Contains(_T("m/s")))
							wind_speed = wxRound((wspd*3600)/1852);
						else if(m_str.Contains(_T("km/h")))
							wind_speed = wxRound(wspd/1.852);
						else
							wind_speed = wspd;

					}
					break;

				case 28: // Engine#1 Hours	 Don't use when Engine is on
					if(!m_str.Trim().Contains(_T("00:00")))
					{ boat_speed = 0; nextline = true; }	
					break;
/*
				case 32: // Sails Configuration Filtering
					if(filterSails)
					{
						if(m_str.Trim(false).IsEmpty())
						{ nextline = true; break; }
						m_str.Replace(_T("\n"),_T(","));
						wxStringTokenizer tkz(m_str,_T(","));
						if(tkz.CountTokens() != sails.GetCount())
						  { nextline = true; break; }
						else
						{
							unsigned int ok = 0;
							while(tkz.HasMoreTokens())
							{
								wxString t = tkz.GetNextToken().Trim(false);
								t = t.Trim();
								for(unsigned int i = 0; i < sails.GetCount(); i++)
								{
									if(t == sails.Item(i).Trim())
									{
										ok++;
										break;
									}
								}
							}				
							if(ok == 0 || ok != sails.Count())
							{ nextline = true; break; }
						}
					}
					else
						nextline = false;
					break;

				case 40: // Engine#2 Hours
					if(!m_str.Trim().Contains(_T("00:00")))						// Don't use when Engine in on
					{ boat_speed = 0; nextline = true; }
					break;
				}
				if(nextline) { break; }			// true = read next line
				token_count++;
			}

//********* Validate data and put in graph and DB

            if(validate_data (rel) && !nextline)
			{
                insert_data_to_Masterpol();
                insert_data_to_grid();
                clear_speeds_directions();
			}
		}
	}

//****************************************************************************
//************ Collect BOAT Speeds within Speed  Range (Percent) for averaging
    if(filterDlg->BoatSpeedFilter->GetSelection())
	{
		for(int i_wspds = 0; i_wspds < 10; i_wspds++)
		{
			for(int i_wdirs = 0; i_wdirs  < WINDDIR; i_wdirs++)
			{
				for (it = Master_pol[i_wspds].winddir.begin(); it != Master_pol[i_wspds].winddir.end(); ++it)
				{
					int wind_dir = (*it).first;
					double wind_spd = (*it).second;

					if((wind_dir == i_wdirs) )
					{
						double percent = ((wind_spd - Master_pol[i_wspds].wdirMax[i_wdirs])/
                            Master_pol[i_wspds].wdirMax[i_wdirs])*100;
						//	wxMessageBox(wxString::Format(_T("%.2f %.2f %.2f proz = %.2f"),spd,Master_pol[i].wdirMax[n],spd-Master_pol[i].wdirMax[n],percent));

						if(percent >= -wxAtof(filterDlg->boat_speed_range_pct->GetString(filterDlg->boat_speed_range_pct->GetSelection())))
						{
							Master_pol[i_wspds].count[i_wdirs]++;
							Master_pol[i_wspds].wdirTotal[i_wdirs] += wind_spd;
							/*wxMessageBox(wxString::Format(_T("%.2f %.2f %.2f proz = %.2f\n%i %.2f"),spd,Master_pol[i].wdirMax[n],Master_pol[i].wdirMax[n]-spd,((Master_pol[i].wdirMax[n]-spd)/spd)*100,
							Master_pol[i].count[n],Master_pol[i].wdirTotal[n]));

						}
					}
				}
			}
		}
	}

//****************************************************************************
//************ Do WIND Speed Averageing

	if(filterDlg->BoatSpeedFilter->GetSelection() > 0)
	{
		for(int i_wspds = 0; i_wspds < 10; i_wspds++)
		{
			for(int i_wdirs = 0; i_wdirs < WINDDIR; i_wdirs++)
			{
				for (it = Master_pol[i_wspds].winddir.begin(); it!=Master_pol[i_wspds].winddir.end(); ++it)
				{
					int wind_dir = (*it).first;
					double wind_spd = (*it).second;

					if((wind_dir == i_wdirs) )
					{
						Master_pol[i_wspds].count[i_wdirs]++;
						Master_pol[i_wspds].wdirTotal[i_wdirs] += wind_spd;
						/*wxMessageBox(wxString::Format(_T("%.2f %.2f %.2f proz = %.2f\n%i %.2f"),spd,Master_pol[i].wdirMax[n],Master_pol[i].wdirMax[n]-spd,((Master_pol[i].wdirMax[n]-spd)/spd)*100,
						Master_pol[i].count[n],Master_pol[i].wdirTotal[n]));

					}
				}
			}
		}
    }

	    for(int i_wspds = 0; i_wspds < 10; i_wspds++)
	    {
		    for(int i_wdirs = 0; i_wdirs < WINDDIR; i_wdirs++)
		    {
			    if(Master_pol[i_wspds].count[i_wdirs] > 0)
			    {
				    Master_pol[i_wspds].wdirAve[i_wdirs] = Master_pol[i_wspds].wdirTotal[i_wdirs] /
                        Master_pol[i_wspds].count[i_wdirs];
			    }
		    }

	    }


//***********************************************************************************

	dlg->m_panelPolar->Refresh();
	dlg->m_button_SavePOL->Enable();
    */
}

void Polar::loadVDR()
{
    wxString m_infilename;

	wxFileDialog fdlg(dlg,_("Select a VDR-File"),_T(""));
	if(fdlg.ShowModal() == wxID_CANCEL) return;

	wxFileInputStream input(fdlg.GetPath());
	wxTextInputStream in(input);

    m_infilename = _T("VDR File: ") + fdlg.GetPath() + _T("\r\n");
    dlg->m_staticTextRecord->AppendText(m_infilename);

	wxProgressDialog progressRead( _("Please wait."), _T(""),
		100, dlg,  wxFRAME_NO_TASKBAR | wxPD_AUTO_HIDE | wxPD_CAN_ABORT );
	progressRead.Fit(); 

	wxString s = wxEmptyString;
	long long ii = input.GetLength();

	do{
		in >> s;
		if(input.Eof()) break;

        long long p = input.TellI();
		int o = ((double)p/(double)ii)*100;
		if(!progressRead.Update(o))
			break;

		parse_NMEA(s + _T("\x0D\x0A"));     // this only gives one datum per input line

        if (validate_data(true))             // assume this is relative wind
		{
            if (filter_data())
            {
                insert_data_to_Masterpol();
		        insert_data_to_grid();
                clear_speeds_directions();
            }
        }
		s = wxEmptyString;

	}while(true);

	dlg->m_panelPolar->Refresh();
}

void Polar::loadSTE()
{
    wxString filetypext = _("*.trt");
    wxString m_infilename;
	wxFileDialog fdlg(dlg,_("Select a STE File (.trt)"),_T(""), _T(""), filetypext, wxFD_OPEN|wxFD_FILE_MUST_EXIST );
	if(fdlg.ShowModal() == wxID_CANCEL) return;

	wxFileInputStream input(fdlg.GetPath());
	wxTextInputStream in(input);
    m_infilename = _T("STE File: ") + fdlg.GetPath() + _T("\r\n");
    dlg->m_staticTextRecord->AppendText(m_infilename);

	wxProgressDialog progressRead( _("Please wait."), _T(""),
		100, dlg,  wxFRAME_NO_TASKBAR | wxPD_AUTO_HIDE | wxPD_CAN_ABORT );
	progressRead.Fit(); 


	wxString s = wxEmptyString;
    long long ii = input.GetLength();

	do{
		in >> s;
		if(input.Eof()) break;

        long long p = input.TellI();
		int o = ((double)p/(double)ii)*100;
		if(!progressRead.Update(o))
			break;

        parse_STE_record(s);
        load_STE_point();                       // loads all speeds and directions

        if (validate_data(true))                    // loads boat_speed from Boat.SOG or Boat.STW
        {
            if (filter_data())
            {
                insert_data_to_Masterpol();
		        insert_data_to_grid();
                clear_speeds_directions();
            }
        }
		s = wxEmptyString;
	//	::wxYield();
	}while(true);

	dlg->m_panelPolar->Refresh();
	dlg->m_button_SavePOL->Enable();
}

bool Polar::validate_data(bool rel)     // relative wind measurement source
{
    bool data_OK = true;

    if(dlg->m_sourceSpeed->GetSelection() == 0)  {   // 0-Boat.SOG 1-Boat.STW
        switch (set_drift_calc)
        {
        case 0:                     // 0- no correction (default)
            if (Boat.SOG > 2){
                boat_speed = Boat.SOG;
                break;
            }
            else data_OK = false;
            break;

        case 1:                     // 1- correct with measured NMEA set drift
            if(Boat.HDG >= 0 && Boat.STW > 2)
            {
                if(Boat.COG >= 0 && Boat.SOG > 2)
                {
                }
            }
            break;
        case 2:                     // 2 = correct with given set drift
            if (!filterDlg->input_Set_value->IsEmpty() &&
                    !filterDlg->input_Drift_value->IsEmpty())
                {
                    break;
                }
            else  data_OK = false;
            break;
        }
    }
    else if (Boat.STW > 2){
        boat_speed = Boat.STW;
        }
        else data_OK = false;    

//************ Boat_speed good look at wind speed ********************
    if ( data_OK && boat_speed < max_boat_speed)
    {
        if( Wind.RWS > 2.0 && Wind.RWA > 0 )
        {
            Wind.TWA = rad2deg(BTW(Wind.RWS, deg2rad(Wind.RWA), boat_speed));
            Wind.TWS = VTW(Wind.RWS, deg2rad(Wind.RWA), boat_speed);
        }
    
        else if (Wind.TWS > 2.0 && Wind.TWA > 0)
        {
            Wind.RWS = VAW(Wind.TWS,deg2rad( Wind.TWA), boat_speed);
            Wind.RWA = rad2deg(BAW(Wind.TWS, deg2rad(Wind.TWA), boat_speed));
        }
           
        if (dlg->m_RelorTrue_Wind->GetSelection() == 0) {       // True wind
            wind_dir = Wind.TWA;
            wind_speed = Wind.TWS;
        }
        else {
            wind_dir = Wind.RWA;
            wind_speed = Wind.RWS;
        }

        if(wind_speed > 2.0){
            if (wind_dir < 0) wind_dir += 360;
//                wind_dir = (int) wind_dir % 360;      TODO
            if (wind_dir > 180)
                wind_dir = 360 - wind_dir;
            // Wind.TWA and Wind.RWA are mapped to 0-180 degrees
        }
        else data_OK = false;        // too low wind speed
    }
    else data_OK = false;           // boat speed out of range

    if (data_OK)
    {
        if(wind_dir > 25)
        {
            j_wdir = wxRound((wind_dir-25)/5); // 25->180 --> 0->31
            i_wspd = wxRound(wind_speed/5);
        }
    }
    return data_OK;
}

bool Polar:: filter_data()
{
    bool data_OK = false;
    if (filterDlg->BoatSpeedFilter->GetSelection() == 0)        // 0 = max value 1 = Average
    {
        double cell_value = wxAtof(dlg->m_gridEdit->GetCellValue(j_wdir,i_wspd));
            if( boat_speed > cell_value)
            {
                data_OK = true;        // use boat_speed value
            } 
    } else
    {
        if (boat_speed > Master_pol[i_wspd].wdirMax[j_wdir])
        {
            Master_pol[i_wspd].wdirMax[j_wdir] = boat_speed; // TODO Check effect of not zeroing sum & count
        }

        double lower_limit = (1 - .05 *
            (filterDlg->boat_speed_range_pct->GetSelection() + 1)) *
            Master_pol[i_wspd].wdirMax[j_wdir];

        if (boat_speed > lower_limit)
        {
                Master_pol[i_wspd].count[j_wdir] += 1;
                Master_pol[i_wspd].wdirTotal[j_wdir] += boat_speed;
                boat_speed = Master_pol[i_wspd].wdirTotal[j_wdir] /
                    Master_pol[i_wspd].count[j_wdir];
                Master_pol[i_wspd].wdirAve[j_wdir] = boat_speed;
                data_OK = true;
        }
    }
    return data_OK;
}

void Polar::insert_data_to_grid()
{
	if((j_wdir >= 0 && j_wdir < 32) && (i_wspd >= 0 && i_wspd < 10))
	{
        dlg->m_gridEdit->SetCellValue(j_wdir,i_wspd,wxString::Format(_T("%.2f"),boat_speed));
    }
}

void Polar::insert_data_to_Masterpol()
{
    Master_pol[i_wspd].winddir.insert(std::pair<int,double>(j_wdir,wind_speed));
    if (filterDlg->BoatSpeedFilter->GetSelection() == 0)
	    Master_pol[i_wspd].wdirMax[j_wdir] = boat_speed;
}

void Polar::createDiagram(wxDC& dc)
{
	max_dimension = dlg->m_panelPolar->GetSize();
	center.x = max_dimension.x / 2 - 190;
	center.y = max_dimension.y /2;
	image_pixel_height[0] = max_dimension.y /2 -40 ;           // current height for denumerator
	pixels_knot_ratio = image_pixel_height[0] / display_speed;
	for(int i = 0; i < display_speed; i++)
		image_pixel_height[i] = wxRound(pixels_knot_ratio*(i+1));
}

void Polar::Render_Polar()
{
/*	if(data_source_index == 0)          // konnilogbook
	{
/*		dc->DrawText(_("Testing, work in progress..."), 40, 60); 

		dc->SetPen( wxPen( wxColour(0,0,0), 1 ) ); 
		dc->SetBrush( wxBrush(wxColour(255,0,0)) );	
		dc->DrawCircle( wxPoint(40,90), 5);
		dc->DrawText(_("less than 3 entries found"), 60, 80); 
		dc->SetBrush( wxBrush(wxColour(255,255,0)) );
		dc->DrawCircle( wxPoint(40,110), 5);
		dc->DrawText(_("less than 5 entries found"), 60, 100); 
		dc->SetBrush( wxBrush(wxColour(0,255,0)) );
		dc->DrawCircle( wxPoint(40,130), 5);
		dc->DrawText(_("more than 10 entries found"), 60, 120); 
		dc->SetBrush( wxBrush(wxColour(255,255,255)) );
	}
*/
//********** Draw Rings and speeds***********************
	int pos_xknt, pos_yknt,neg_yknt;
	for(int i = display_speed - 1; i >= 0; i--)
	{
		pos_xknt = wxRound(cos((0-90)*(PI/180.0))*image_pixel_height[i]+center.x);
		pos_yknt = wxRound(sin((0-90)*(PI/180.0))*image_pixel_height[i]+center.y);
		neg_yknt = wxRound(sin((0+90)*(PI/180.0))*image_pixel_height[i]+center.y);
 
        dc->DrawArc(center.x,center.y+image_pixel_height[i],center.x,center.y-image_pixel_height[i],center.x,center.y);
            
            if(display_speed < 20){
			    dc->DrawText(wxString::Format(_T("%i"),i+1),wxPoint(pos_xknt,pos_yknt-10));
                dc->DrawText(wxString::Format(_T("%i"),i+1),wxPoint(pos_xknt,neg_yknt-10));
            }
            else{
				if((display_speed == 20 && ((i+1) % 2) == 0) || (display_speed == 25 && ((i+1) % 3) == 0)){
				    dc->DrawText(wxString::Format(_T("%i"),i+1),wxPoint(pos_xknt,pos_yknt-10));
                    dc->DrawText(wxString::Format(_T("%i"),i+1),wxPoint(pos_xknt,neg_yknt-10));
                }
			}
	}
//********** Draw Spokes *********************************
	int angle = 20;
	for(angle; angle <= 180; angle += display_inc_deg)
	{
		int xt = wxRound(cos((angle - 90) * PI/180)*(image_pixel_height[(int)display_speed-1]+20)+center.x);
		int yt = wxRound(sin((angle - 90) * PI/180)*(image_pixel_height[(int)display_speed-1]+20)+center.y);

		dc->DrawLine(wxPoint(center.x,center.y),wxPoint(xt,yt));
		dc->DrawText(wxString::Format(_T("%i\xB0"),angle), wxPoint(xt,yt));
	}

//************ LWL Data ******************************************
    dc->DrawText(wxString::Format(_T("LWL: %3.1f  Max Speed: %3.1f"),boat_LWL,max_boat_speed), wxPoint(200,20));

//************ Filter effect *************************************
    wxString Filter = _("Maximum boat speeds");
    if ( filterDlg->BoatSpeedFilter->GetSelection() > 0)
    {
        double pct = 5 * (filterDlg->boat_speed_range_pct->GetSelection() + 1);
        Filter = _("Using ") + (wxString::Format(_T("%2.0f pct of Max (Ave)"),pct));
    }
    dc->DrawText(Filter, wxPoint(200,40));

	createSpeedBullets();
}

void Polar::createSpeedBullets()
{
    int selected_i_wspd =  dlg->m_displaywind_speed->GetSelection();       // initially 0
	
	int end_index;
	int xt, yt, bullet_point_count;
	wxPoint ptBullets[180];             // max of 180 degrees

	if(selected_i_wspd != 0) {
        selected_i_wspd -= 1;
        end_index = selected_i_wspd + 1;
    }
	else{
        selected_i_wspd = 0;                  // all speeds
        end_index = 10;
    }

	double speed_in_pixels;
	wxColour Colour,brush;
	wxPen init_pen = dc->GetPen();			// get actual Pen for restoring later

    for(int i_wspds = selected_i_wspd; i_wspds < end_index; i_wspds++)									// go thru all winddirection-structures depending on choiocebox degrees
	{
		bullet_point_count = 0;
		Colour = windColour[i_wspds];
        brush = windColour[i_wspds];

		for(int i_wdirs = 0; i_wdirs < WINDDIR; i_wdirs++)          // 0->31
		{
            double cell_value = wxAtof(dlg->m_gridEdit->GetCellValue(i_wdirs,i_wspds));
			if( cell_value > 0) 
                {
			        speed_in_pixels = cell_value * pixels_knot_ratio;

			        xt = wxRound(cos(((i_wdirs*5+25)- 90)*PI/180)*speed_in_pixels + center.x);		// calculate the point for the bullet
			        yt = wxRound(sin(((i_wdirs*5+25)- 90)*PI/180)*speed_in_pixels + center.y);

                    wxPoint pt(xt,yt);
				    ptBullets[bullet_point_count++] = pt;      // Add to display array
            }
		}

//************ Wind_sped Column, plot curve and bullets ********************

		if(bullet_point_count > 2)					//Draw curves, needs min. 3 points
		{
			dc->SetPen(wxPen(Colour,3));
			dc->DrawSpline(bullet_point_count,ptBullets);
		}
  	
		for(int i = 0; i < bullet_point_count; i++)     // draw the bullet
		{
			if(ptBullets[i].x != 0 && ptBullets[i].y != 0)
			{            
		    dc->SetBrush( brush );
			dc->SetPen(wxPen(wxColour(0,0,0),2));
			dc->DrawCircle(ptBullets[i],5);	
			ptBullets[i].x = ptBullets[i].y = 0;
			}
		} 
	}

	dc->SetPen(init_pen);
}


//*************** Support for POL files *************************************************

void Polar::clear_Master_pol()
{
	for(int i = 0; i < 10; i++)
	{
		for(int n = 0; n < WINDDIR; n++)
		{
			Master_pol[i].wdir[n] = 0;
			Master_pol[i].count[n] = 0;
			Master_pol[i].scount[n] = 0;
			Master_pol[i].wdirMax[n] = 0;
			Master_pol[i].wdirTotal[n] = 0;
            Master_pol[i].wdirAve[n] = 0;
			Master_pol[i].winddir.clear();
		}
	}
}

void Polar::save_POL_file()
{
	double data;

	wxFileDialog saveFileDialog(dlg, _("Save POL File"), _T(""), _T("Polar"),
		_T("OCPN-Format(*.pol)|*.pol|QTVlm(*.pol)|*.pol|Maxsea(*.pol)|*.pol|CVS-Format(*csv)|*.csv"), 
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	int sel = saveFileDialog.GetFilterIndex();          //File type
	wxString saveFile = saveFileDialog.GetPath();
	
	if(sel == 3 && !saveFile.EndsWith(_T(".csv")))
	  saveFile += _T(".csv");
	else if(sel != 3 && !saveFile.EndsWith(_T(".pol")))
	  saveFile += _T(".pol");

	//***************Save Data from Master POL file (average or MAX) **********************************
    //Loading Data to buffer

	struct pol save_data[10];                           // POL buffer with ten wind speed rows
	for(int i = 0; i < 10; i++)	
		save_data[i] = Master_pol[i];                       // Copy data

	for(int i_wspds = 0; i_wspds < 10; i_wspds++)
	{
		for(int i_wdirs = 0; i_wdirs < WINDDIR; i_wdirs++)          // 0->31
		{
			save_data[i_wspds].wdirTotal[i_wdirs] += save_data[i_wspds].wdirTotal[WINDDIR-1-i_wdirs];    // Sum of values
			save_data[i_wspds].count[i_wdirs]     += save_data[i_wspds].count    [WINDDIR-1-i_wdirs];    // Count of values
			save_data[i_wspds].wdirMax[i_wdirs]   += save_data[i_wspds].wdirMax  [WINDDIR-1-i_wdirs];    //TODO Check out dirMax

			if(save_data[i_wspds].count[i_wdirs] >= 2 && save_data[i_wspds].count[WINDDIR-1-i_wdirs] > 0)    // NO!
				save_data[i_wspds].wdirTotal[i_wdirs] /= 2;

			if(save_data[i_wspds].wdirMax[i_wdirs] > 0 && save_data[i_wspds].wdirMax[WINDDIR-1-i_wdirs] > 0) // Averaging MAX doesn't make sense
				save_data[i_wspds].wdirMax[i_wdirs]	/= 2;

			save_data[i_wspds].wdirTotal [WINDDIR-1-i_wdirs] = 0;
			save_data[i_wspds].count[WINDDIR-1-i_wdirs] = 0;

			//if(save_data[i_wspds].count[i_wdirs] > 0)
			//save_data[i_wspds].count[i_wdirs] = 1;
		}
	}

	wxFileOutputStream output( saveFile);
	wxTextOutputStream polarFile(output);

	if(sel == 0)                            // Standard OCPN POL file
	{
		for(int i_wspds = 0; i_wspds < 10; i_wspds++)
		{
			polarFile << (i_wspds+1)*5 << _T(" ");                   // wind_speed -> first posit of line

			for(int i_wdirs = 0; i_wdirs < WINDDIR; i_wdirs++) //  0->31
			{
				data = -1;
				if (data_source_index == 0){                            // Logbook -> average
					data = save_data[i_wspds].wdirAve[i_wdirs];
                }
                else {
                    data = save_data[i_wspds].wdirMax[i_wdirs];          //NMEA,VDR, STE -> Max
				}

				polarFile << (i_wdirs+5)*5 << _T(" ");              // 20->
				if( data > 0.0)                                       // save_data[i_wspds].count[i_wdirs] > 0 &&
					polarFile << wxString::Format(_T("%.2f "),data);
				else
				polarFile << _T(" ");
				
			}
			polarFile << _T("\n");
		}
	}
	else if(sel == 1)                       // QTV format
	{
		polarFile << _T("TWA/TWS\t");
		for(int i = 0 ; i < 11; i++)
			polarFile << wxString::Format(_T("%i\t"),i*5);
		polarFile << _T("60\n");

		for(int i_wdirs = -1; i_wdirs < WINDDIR; i_wdirs++)
		{
			if(i_wdirs == -1)
			{
				for(int z = 0; z < 13; z++)
					polarFile << _T("0\t");
				polarFile << _T("\n");
				continue;
			}
			polarFile << 20+(i_wdirs*5) << _T("\t");
			for(int i_wspds = 0; i_wspds < 10; i_wspds++)
			{
				data = -1;
				switch(data_source_index)
				{
				case 0:
					data = save_data[i_wspds].wdirTotal[i_wdirs];
					break;
				case 1:
				case 2:
				case 3:
                case 4:
					data = save_data[i_wspds].wdirMax[i_wdirs];
					break;
				}
				if(i_wspds == 0)
					polarFile << _T("0\t");
				if(save_data[i_wspds].count[i_wdirs] > 0 && data >= 0.0)
					polarFile << wxString::Format(_T("%.2f\t"),data);
				else
					polarFile << _T("\t");

			}
			polarFile << _T("0\n");
		}
	}
	else if(sel == 2)                               //Maxsea format
	{
		polarFile << _T("TWA\t");
		for(int i = 0 ; i < 11; i++)
			polarFile << wxString::Format(_T("%i\t"),i*5);
		polarFile << _T("\n");

		for(int i_wdirs = 0; i_wdirs < WINDDIR; i_wdirs++)
		{
			polarFile << 25+(i_wdirs*5) << _T("\t");
			for(int i_wspds = 0; i_wspds < 10; i_wspds++)
			{
				data = -1;
				switch(data_source_index)
				{
				case 0:
					data = save_data[i_wspds].wdirTotal[i_wdirs];
					break;
				case 1:
				case 2:
				case 3:
                case 4:
					data = save_data[i_wspds].wdirMax[i_wdirs];
					break;
				}
				if(i_wspds == 0)
					polarFile << _T("0\t");
				if(save_data[i_wspds].count[i_wdirs] > 0 && data >= 0.0)
					polarFile << wxString::Format(_T("%.2f\t"),data);
				else
					polarFile << _T("\t");

			}
			polarFile << _T("\n");
		}
	}
	else                                        // CSV format
	{
	  wxString s = _T(",");	  
	  for(int col = 0; col < dlg->m_gridEdit->GetCols(); col++)
	      s <<  dlg->m_gridEdit->GetColLabelValue(col)+_T(",");
	  s.RemoveLast();
	  polarFile << s << _T("\n");
	  
	  for(int row = 0; row < dlg->m_gridEdit->GetRows(); row++)
	  { 
	    s = wxEmptyString;
	    s <<  dlg->m_gridEdit->GetRowLabelValue(row) << _T(",");
	    for(int col = 0; col < dlg->m_gridEdit->GetCols(); col++)
	      s << dlg->m_gridEdit->GetCellValue(row,col) << _T(",");
	    
	    s.RemoveLast();
	    polarFile << s << _T("\n");
	  }
	}
	output.Close();

	for(int i_wspds = 0; i_wspds < 10; i_wspds++)	
		Master_pol[i_wspds] = save_data[i_wspds];     // TODO Load updated back to display?
	dlg->m_panelPolar->Refresh();
}

void Polar::load_POL_file()
{
	wxFileDialog fdlg(dlg,_("Select a Polar-File"),_T(""));
	if(fdlg.ShowModal() == wxID_CANCEL) return;

	clear_Master_pol();
	dlg->m_gridEdit->ClearGrid();

	set_Dlg_for_DataSource(3);

	wxFileInputStream stream( fdlg.GetPath() );							
	wxTextInputStream in(stream);	
	wxString wdirstr,wsp;

	bool test_first = true;
	int file_type = -1;
    int file_line = -1;

    wxStringTokenizer tkz;              // Selected Parsing character

	while(!stream.Eof())
	{
		wxString str = in.ReadLine();
//		if(stream.Eof()) break;


		if(test_first)
		{
            if(str.GetChar(0) == '5') {                          // our POL (or csv)
				file_type = 0;
                wxStringTokenizer tk(str,_T(" "),wxTOKEN_RET_EMPTY);
				tkz = tk;
            }
			else if(str.Contains(_T("TWA/TWS"))) {               // specials
				file_type = 1;
                wxStringTokenizer tk(str,_T("\t"),wxTOKEN_RET_EMPTY);	
				tkz = tk;
            }
			else if (str.Contains(_T("TWA"))) {
				file_type = 2;
                wxStringTokenizer tk(str,_T("\t"),wxTOKEN_RET_EMPTY);	
				tkz = tk;
            }
			else
			{
				wxMessageBox(_T("Cannot load this file"));
				return;
			}

			test_first = false;
//			if( file_type != 0)
//				continue;
		}
/*
		wxString u = tkz.GetNextToken();                        // first field (wind speed)
		if(u == _T("0") && file_type == 1){
            file_line++;
            continue;
        }
*/		
        if(file_line == -1)
			file_line = 0;                                          // starting at file line 0 (col 0)

        tkz.SetString(str);
        int gridir = 0;

//***************************************************************************************************		
		if (file_type == 0)                                      // standard POL and csv
		{
			wxString s;
            int gridpd = file_line;
            double number;
            tkz.GetNextToken();                             // Wind Speed

			while(tkz.HasMoreTokens())
			{                         
				s = tkz.GetNextToken();
                s.ToDouble(&number);

				if(number < 20.){                            // Wind Angle
                        dlg->m_gridEdit->SetCellValue(gridir -1,gridpd,s);

                        tkz.GetNextToken();
                    }
                gridir++;
            }
		}

        else if(file_type == 1 || file_type == 2)                              // special format
		{
			int i_wspd = 0, token_count = 0; 
			wxString s;
			while(tkz.HasMoreTokens())
			{
				token_count++;
				if(token_count > 11) break;
				s = tkz.GetNextToken();
				if(s == _T("0") && (file_type == 1 || file_type == 2)) continue;
				dlg->m_gridEdit->SetCellValue(file_line,i_wspd,s);
				load_POL_datum_str(s,file_line,i_wspd++);
			}
		}
        file_line++;
	}

	dlg->m_panelPolar->Refresh();
}

//**************** Support routines for major functions ***********************************

void Polar::parse_NMEA(wxString sentence)
{
	wxDateTime dt;
	m_NMEA0183 << sentence;

	if(m_NMEA0183.PreParse())
	{
		if(m_NMEA0183.LastSentenceIDReceived == _T("RMC"))
		{
			if(m_NMEA0183.Parse())
				if(m_NMEA0183.Rmc.SpeedOverGroundKnots != 999.0)
					Boat.SOG = m_NMEA0183.Rmc.SpeedOverGroundKnots;
		}
		else if(m_NMEA0183.LastSentenceIDReceived == _T("VHW"))
		{
			if(m_NMEA0183.Parse())
                if(m_NMEA0183.Vhw.Knots != 999.0)
				    Boat.STW = m_NMEA0183.Vhw.Knots;
		}
		else if(m_NMEA0183.LastSentenceIDReceived == _T("MWV"))
		{
			if(m_NMEA0183.Parse())
			{
				wind_dir = m_NMEA0183.Mwv.windAngle;
				wind_ref = m_NMEA0183.Mwv.Reference; // "R" or "T"

				if(m_NMEA0183.Mwv.WindSpeedUnits == 'N')    // Nautical miles/ hr (knots)
					wind_speed = wxRound(m_NMEA0183.Mwv.WindSpeed);
				else if(m_NMEA0183.Mwv.WindSpeedUnits == 'M') // meters/sec
					wind_speed = wxRound((m_NMEA0183.Mwv.WindSpeed*3600)/1852);
				else if(m_NMEA0183.Mwv.WindSpeedUnits == 'K')  // Kilometers
					wind_speed = m_NMEA0183.Mwv.WindSpeed/1.852;

                if (wind_ref == _("R")) {
                    Wind.RWS = wind_speed;
                    Wind.RWA = wind_dir;
                }
                else if (wind_ref == _("T")){
                    Wind.TWS = wind_speed;
                    Wind.TWA = wind_dir;
                }
			}
		}
		else if(m_NMEA0183.LastSentenceIDReceived == _T("VWR"))
		{
			if(m_NMEA0183.Parse())
			{
				wind_dir = m_NMEA0183.Vwr.WindDirectionMagnitude;
				wind_ref = _T("R");

				if(m_NMEA0183.Vwr.WindSpeedKnots > 0)
					wind_speed = m_NMEA0183.Vwr.WindSpeedKnots;
				else if(m_NMEA0183.Vwr.WindSpeedms > 0)
					wind_speed = wxRound((m_NMEA0183.Vwr.WindSpeedms*3600)/1852);
				else if(m_NMEA0183.Vwr.WindSpeedKmh > 0)
					wind_speed = m_NMEA0183.Vwr.WindSpeedKmh / 1.852;
			}

		}
	}

//************** Engine data Parsing **********************************
		wxStringTokenizer tkz(sentence,_T(","));
		wxString sentenceInd = tkz.GetNextToken();

		if(sentenceInd == _T("$ERRPM"))
		{

			long Umin = 0;

			dtRPM = wxDateTime::Now();

			wxString NMEA_source = tkz.GetNextToken();
			wxString engineNr = tkz.GetNextToken();
			wxString speed = tkz.GetNextToken();
			wxString pitch = tkz.GetNextToken();

			speed.ToLong(&Umin);

			if(Umin != 0L)
			{
				if(NMEA_source == _T("E"))
				{
					if(!engineRunning)
					{
						engineRunning = true;
						setEngineStatus(_T("ON"));
					}
				}
			}
			else
			{
				if(engineRunning)
				{
					engineRunning = false;
					setEngineStatus(_T("OFF"));
				}
			}

		}
}

void Polar::load_STE_point()
{   
    clear_speeds_directions();
    double temp = m_Point.SOG.ToDouble(&Boat.SOG);
    temp = m_Point.COGT.ToDouble(&Boat.COG);
    temp = m_Point.BtHDG.ToDouble(&Boat.HDG);
    temp = m_Point.BtWatSpd.ToDouble(&Boat.STW);
    temp = m_Point.RWSpd.ToDouble(&Wind.RWS);
    temp = m_Point.TWSpd.ToDouble(&Wind.TWS);
    temp = m_Point.TrueWind.ToDouble(&Wind.TWA);
    temp = m_Point.RelWind.ToDouble(&Wind.RWA);

    Boat.HDG = Boat.HDG * 180 /PI;   // these are radians  (but not geo referenced)
    Boat.COG = Boat.COG * 180 /PI;
    Wind.RWA = Wind.RWA * 180 /PI;
    Wind.TWA = Wind.TWA * 180 /PI;
    wind_ref = _T("R");
}

void Polar::parse_STE_record(wxString m_instr)
{

    if( !m_instr.IsEmpty() )
    {
        wxStringTokenizer tokenizer(m_instr, _T(","));

        if( tokenizer.GetPosition() == 0 )
        {
            m_Point.UTS = tokenizer.GetNextToken();
            m_Point.Lat = tokenizer.GetNextToken();
            m_Point.Lon = tokenizer.GetNextToken();
            m_Point.SOG = tokenizer.GetNextToken();
            m_Point.COGT = tokenizer.GetNextToken();
            m_Point.COGM = tokenizer.GetNextToken();
            m_Point.Variation = tokenizer.GetNextToken();
            m_Point.Depth = tokenizer.GetNextToken();
            m_Point.CrseWind = tokenizer.GetNextToken();
            m_Point.RelWind = tokenizer.GetNextToken();
            m_Point.TrueWind = tokenizer.GetNextToken();
            m_Point.TWSpd = tokenizer.GetNextToken();
            m_Point.RWSpd = tokenizer.GetNextToken();
            m_Point.SpdParWind = tokenizer.GetNextToken();
            m_Point.BtHDG = tokenizer.GetNextToken();
            m_Point.BtWatSpd = tokenizer.GetNextToken();
        }
    }
}

void Polar::setEngineStatus(wxString str)
{
	if(data_source_index == 1 && str == _T("ON"))        // NMEA source with engine running
	{
		input_NMEA = false;
		dlg->m_toggleBtnRecord->Hide();
		dlg->m_staticTextEngine->SetLabel(_("Engine started / Recording is stopped"));
		dlg->m_panel_Grid->Layout();
	}
	else if(data_source_index == 1 && str == _T("OFF"))  // NMEA source with engine off
	{
		dlg->m_toggleBtnRecord->Show();
		dlg->m_staticTextEngine->SetLabel(_T(""));
		dlg->m_panel_Grid->Layout();
		input_NMEA = true;
	}
}

void Polar::clear_speeds_directions()
{
    Boat.STW = 0;
	Boat.SOG = 0;
    Boat.HDG = -1;
    Boat.COG = -1;

	wind_speed = 0;
	wind_dir = 0;
	wind_ref = wxEmptyString;

    Wind.TWA = 0;
    Wind.TWS = 0;
    Wind.RWA = 0;
    Wind.RWS = 0;
}

void Polar::showDlg()
{
	filterDlg->ShowModal();
}

////////////// Filter  Dialog ////////////////////////

FilterDlg::FilterDlg( PolarDialog* parent, Polar* polar, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	pfilt_dlg = parent;
	this->pfilt_polar = polar;
//	pfilt_Logbook_Sailing_Conditions = new Logbook_Sailing_Conditions();

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerNotebook;
	bSizerNotebook = new wxBoxSizer( wxVERTICAL );
	
	m_Filter_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

//*************** Wind Range - Boat Speed Treatment ******************************************
	boat_speed_range_panel = new wxPanel( m_Filter_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );   

    bSizer_FilterRange = new wxBoxSizer( wxVERTICAL);

    wxString m_choicesMode[] = { _("Use Maximum Boat Speed"), _("Use Average Boat Speed")};
	int m_numberMode = sizeof( m_choicesMode ) / sizeof( wxString );
	BoatSpeedFilter = new wxChoice( boat_speed_range_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_numberMode, m_choicesMode, 0 );
	BoatSpeedFilter->SetSelection(0 );
	BoatSpeedFilter->SetToolTip( _("Averaging based on boat speed selection between lower limit and max value") );
	bSizer_FilterRange->Add( BoatSpeedFilter, 0, wxALIGN_LEFT|wxALL, 5 );

//    bSizer_FilterRange->Add( 0, 40, 0, 0, 5 );

	bSizerMode = new wxBoxSizer( wxHORIZONTAL );

    txtAveragingMode = new wxStaticText( boat_speed_range_panel, wxID_ANY, _("Averaging Group Limits:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtAveragingMode->Wrap( -1 );
	bSizerMode->Add( txtAveragingMode, 0, wxLEFT, 5 );

	wxString boat_speed_range_pctChoices[] = { _T("5 %"), _T("10 %"), _T("15 %"), _T("20 %"), _T("25 %"), _T("50 %"), _T("75 %"), _T("ALL") };
	int number_boat_speed_range_pctChoices = sizeof( boat_speed_range_pctChoices ) / sizeof( wxString );
	boat_speed_range_pct = new wxChoice( boat_speed_range_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, number_boat_speed_range_pctChoices, boat_speed_range_pctChoices, 0 );
	boat_speed_range_pct->SetSelection( 0 );
    boat_speed_range_pct->SetToolTip( _("Example:\nMax-Speed = 2.5 kts\nSelected 10 % = 0.25 kts\nAveraging Range from 2.25 to 2.5 kts") );
	bSizerMode->Add( boat_speed_range_pct, 0, wxLEFT, 5 );
	boat_speed_range_pct->Disable();

	bSizer_FilterRange->Add( bSizerMode, 0, wxALIGN_CENTER|wxALL, 5 );

    wxString m_choicesSetDrift[] = {_("Use straight Boat.SOG/Boat.STW"), _("Use Known Set and Drift"), _("Use NMEA measured Set and Drift")};
	int m_numberSetDrift = sizeof( m_choicesSetDrift ) / sizeof( wxString );
	SetDriftFilter = new wxChoice( boat_speed_range_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_numberSetDrift, m_choicesSetDrift, 0 );
	SetDriftFilter->SetSelection(0);
	SetDriftFilter->SetToolTip( _("If HDG and Boat.STW values available calculate Boat.STW = Boat.SOG - tidal current") );
	bSizer_FilterRange->Add( SetDriftFilter, 0, wxALIGN_LEFT|wxALL, 5 );

    txtinput_Set_value = new wxStaticText( boat_speed_range_panel, wxID_ANY, _("Known Set value (degrees true):"), wxDefaultPosition, wxDefaultSize, 0 );
	txtinput_Set_value->Wrap( -1 );
    bSizer_FilterRange->Add( txtinput_Set_value, 0, wxALL|wxALIGN_LEFT, 5 );

    input_Set_value = new wxTextCtrl( boat_speed_range_panel, wxID_ANY);
	bSizer_FilterRange->Add( input_Set_value, 0, wxALL|wxALIGN_LEFT, 5 );

    txtinput_Drift_value = new wxStaticText( boat_speed_range_panel, wxID_ANY, _("Known Drift value (knots):"), wxDefaultPosition, wxDefaultSize, 0 );
	txtinput_Drift_value->Wrap( -1 );
    bSizer_FilterRange->Add( txtinput_Drift_value, 0, wxALL|wxALIGN_LEFT, 5 );

    input_Drift_value = new wxTextCtrl( boat_speed_range_panel, wxID_ANY);
	bSizer_FilterRange->Add( input_Drift_value, 0, wxALL|wxALIGN_LEFT, 5 );

	boat_speed_range_panel->SetSizer( bSizer_FilterRange );
	boat_speed_range_panel->Layout();
	bSizerMode->Fit( boat_speed_range_panel );
	m_Filter_notebook->AddPage( boat_speed_range_panel, _("Boat Speed Treatment"), true );

//***** Sails Sheet ************************************************************************************
	sails_panel = new wxPanel( m_Filter_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer_FilterSails = new wxBoxSizer( wxVERTICAL );
	
//	m_staticText152 = new wxStaticText( sails_panel, wxID_ANY, _("Work in irogress"), wxDefaultPosition, wxDefaultSize, 0 );
//	m_staticText152->Wrap( -1 );
//	bSizer_FilterSails->Add( m_staticText152, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
		
	bSizer_FilterSails->Add( 0, 40, 0, 0, 5 );
	
	fgSizer_FilterSails = new wxFlexGridSizer( 0, 1, 10, 5 );
	fgSizer_FilterSails->SetFlexibleDirection( wxBOTH );
	fgSizer_FilterSails->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	bSizer_FilterSails->Add( fgSizer_FilterSails, 0, wxALIGN_LEFT, 5 );
	
	
	sails_panel->SetSizer( bSizer_FilterSails );
	sails_panel->Layout();
	bSizer_FilterSails->Fit( sails_panel );
	m_Filter_notebook->AddPage( sails_panel, _("Sails"), false );

//****************************************************************************************
	sea_state_panel = new wxPanel( m_Filter_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer52 = new wxBoxSizer( wxVERTICAL );
	
//	m_staticText153 = new wxStaticText( sea_state_panel, wxID_ANY, _("Work in progess"), wxDefaultPosition, wxDefaultSize, 0 );
//	m_staticText153->Wrap( -1 );
//	bSizer52->Add( m_staticText153, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer_FilterLimit1;
	bSizer_FilterLimit1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText154 = new wxStaticText( sea_state_panel, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText154->Wrap( -1 );
	bSizer_FilterLimit1->Add( m_staticText154, 0, wxALL, 5 );
	
	
	bSizer52->Add( bSizer_FilterLimit1, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	bSizer52->Add( 0, 0, 0, 0, 5 );
	
	
	sea_state_panel->SetSizer( bSizer52 );
	sea_state_panel->Layout();
	bSizer52->Fit( sea_state_panel );
	m_Filter_notebook->AddPage( sea_state_panel, _("Sea State"), false );
	
	bSizerNotebook->Add( m_Filter_notebook, 1, wxEXPAND | wxALL, 5 );

	//
	m_sdbNotebook = new wxStdDialogButtonSizer();
	m_sdbNotebookOK = new wxButton( this, wxID_OK );
	m_sdbNotebook->AddButton( m_sdbNotebookOK );
	m_sdbNotebookCancel = new wxButton( this, wxID_CANCEL );
	m_sdbNotebook->AddButton( m_sdbNotebookCancel );
	m_sdbNotebook->Realize();
	
	bSizerNotebook->Add( m_sdbNotebook, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( bSizerNotebook );
	this->Layout();
	
	this->Centre( wxBOTH );	

 	// Connect Events
//	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( FilterDlg::init ) );
	m_sdbNotebookOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlg::OnOKButtonClick ), NULL, this );
    BoatSpeedFilter->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FilterDlg::OnChoiceSpeedAvgMode ), NULL, this );
    SetDriftFilter->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FilterDlg::OnSetDriftSelection ), NULL, this );

	init();
}

FilterDlg::~FilterDlg()
{
	// Disconnect Events
//	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( FilterDlg::PolarDlgOnInitDialog ) );
	m_sdbNotebookOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlg::OnOKButtonClick ), NULL, this );
	BoatSpeedFilter->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FilterDlg::OnChoiceSpeedAvgMode ), NULL, this );
    SetDriftFilter->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FilterDlg::OnSetDriftSelection ), NULL, this );
}
void FilterDlg::init()
{

    input_Set_value->Disable();
    input_Drift_value->Disable();

//******************* Sails Panel **********************************

    sail_name.Add(_("Jib"));
    sail_name.Add(_("Genoa 110"));
    sail_name.Add(_("Genoa 130"));
    sail_name.Add(_("Genoa 150"));
    sail_name.Add(_("Staysail"));
    sail_name.Add(_("Main"));
    sail_name.Add(_("Main - First Reef"));
    sail_name.Add(_("Main - Second Reef"));
    sail_name.Add(_("Try"));
    sail_name.Add(_("Mizzen"));
    sail_name.Add(_("Mizzen Storm"));
    sail_name.Add(_("Symetrical Spinnacker"));
    sail_name.Add(_("Asymetrical Spinnacker"));

	fgSizer_FilterSails->Clear(true);
	sails_panel->Layout();

	for(int i = 0; i < (int)sail_name.Count(); i++)
	{
		checkboxSails[i] = new wxCheckBox( sails_panel, wxID_ANY,sail_name.Item(i) , wxDefaultPosition, wxDefaultSize, 0 );
		checkboxSails[i]->SetValue( false );
		fgSizer_FilterSails->Add(  checkboxSails[i] ,0, 0, 5 );
	}

	    m_buttonSailsReset = new wxButton( sails_panel, wxID_ANY, _("Clear"), wxDefaultPosition , wxDefaultSize, 0 );
	    m_buttonSailsReset->SetToolTip(_("Clear all sail selections."));
	   	m_buttonSailsReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FilterDlg::OnButtonClickResetSails ), NULL, this );
	    fgSizer_FilterSails->Add( m_buttonSailsReset, 0, 0, 5 );
/*	
	    fgSizer_FilterSails->SetVGap(5);
	    fgSizer_FilterSails->SetHGap(10);
/*

//******************* Sea State Panel **********************************
	wxString d;
	switch(0)
	{
	case 0:	d = _("meter"); break;
	case 1: d = _("feet"); break;
	}

	for(int i = 0; i < 10; i++)
	{
		wxBoxSizer* bSizerWave = new wxBoxSizer( wxHORIZONTAL );

		wxStaticText* m_staticText = new wxStaticText( sea_state_panel, wxID_ANY, _("Wind:"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText->Wrap( -1 );
		bSizerWave->Add( m_staticText, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

		m_staticTextKnot = new wxStaticText( sea_state_panel, wxID_ANY, wxString::Format(_T("%i"),(i+1)*4), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticTextKnot->Wrap( -1 );
		bSizerWave->Add( m_staticTextKnot, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

		m_staticTextUnitdisplay_speed = new wxStaticText( sea_state_panel, wxID_ANY, _("kts"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticTextUnitdisplay_speed->Wrap( -1 );
		bSizerWave->Add( m_staticTextUnitdisplay_speed, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

		wxStaticLine* m_staticlinew = new wxStaticLine( sea_state_panel, wxID_ANY, wxDefaultPosition, wxSize( 1,-1 ), wxLI_HORIZONTAL );
		bSizerWave->Add( m_staticlinew, 0, wxEXPAND | wxALL, 5 );

		m_staticText = new wxStaticText( sea_state_panel, wxID_ANY, _("Wavehight from"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText->Wrap( -1 );
		bSizerWave->Add( m_staticText, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );


		m_choiceChoicesFrom[i].Add(_("any"));
		for(int n = 0; n < 10; n++)
		{ m_choiceChoicesFrom[i].Add(wxString::Format(_T("%.2f"),n*0.5)); }

		int m_choice40NChoices = sizeof( m_choiceChoicesFrom ) / sizeof( wxString );
		checkboxWaveFrom[i] = new wxChoice( sea_state_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceChoicesFrom[i], 0 );
		checkboxWaveFrom[i]->SetSelection( 0 );
		bSizerWave->Add( checkboxWaveFrom[i], 0, wxALL, 5 );

		m_staticText = new wxStaticText( sea_state_panel, wxID_ANY, _("to"), wxDefaultPosition, wxDefaultSize, 0 );
		m_staticText->Wrap( -1 );
		bSizerWave->Add( m_staticText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

		wxArrayString m_choiceChoicesTo[10];
		m_choiceChoicesTo[i].Add(_("any"));
		for(int n = 0; n < 10; n++)
		{ m_choiceChoicesTo[i].Add(wxString::Format(_T("%.2f"),(n+1)*0.5)); }
		checkboxWaveTo[i] = new wxChoice( sea_state_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceChoicesTo[i], 0 );
		checkboxWaveTo[i]->SetSelection( 0 );
		bSizerWave->Add( checkboxWaveTo[i], 0, wxALL, 5 );

		m_staticTextUnitHeight = new wxStaticText( sea_state_panel, wxID_ANY, d, wxDefaultPosition, wxDefaultSize, 0 );
		m_staticTextUnitHeight->Wrap( -1 );
		bSizerWave->Add( m_staticTextUnitHeight, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

		bSizer52->Add( bSizerWave, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	}
	sea_state_panel->SetSizer( bSizer52 );
	sea_state_panel->Layout();
	bSizer52->Fit( sea_state_panel );
*/
	sails_panel->Layout();
	this->Layout();

//	this->m_Filter_notebook->RemovePage(2);
//	if(Logbook_Sailing_Conditions->abrSails.Count() == 0)
//		this->m_Filter_notebook->RemovePage(1);

}

void FilterDlg::OnOKButtonClick( wxCommandEvent& event )
{
    if(!input_Set_value->IsEmpty()) 
        current_set = input_Set_value->GetValue(); 

    if(!input_Drift_value->IsEmpty()) 
        current_drift = input_Drift_value->GetValue();
/*
	if(Sails.Count() > 0)
		Sails.Clear();


	filterSails = false;

	for(int i =  0; i < 14; i++)
		if(checkboxSails[i]->IsChecked())
		{
			Sails.Add(sail_name.Item(i));
			filterSails = true;
		}
*/
		this->Hide();
}

void FilterDlg::OnChoiceSpeedAvgMode( wxCommandEvent& event )
{
    if (BoatSpeedFilter->GetSelection() == 1)
    {
        boat_speed_range_pct->Enable();
    }
}

void FilterDlg::OnSetDriftSelection( wxCommandEvent& event )
{
    pfilt_dlg->polar->set_drift_calc = SetDriftFilter->GetSelection();

    if (SetDriftFilter->GetSelection() == 2){
        input_Set_value->Enable();
        input_Drift_value->Enable();
    }
    else {
        input_Set_value->Disable();
        input_Drift_value->Disable();
    }
}

void FilterDlg::OnButtonClickResetSails( wxCommandEvent& event )
{
	for(int i = 0; i < (int)sail_name.Count(); i++)
    {
        checkboxSails[i]->SetValue( false );
    }
}

static double deg2rad(double deg)
{
    return ((90 - deg * PI / 180.0));
}

static double rad2deg(double rad)
{
    return (int(rad * 180.0 / PI + 90 + 360) % 360);
}

double VTW(double VAW, double BAW, double SOG)
{
    double VTW_value = sqrt(pow((VAW * sin(BAW)),2) + pow((VAW * cos(BAW)- SOG),2));
    return VTW_value;
}

double BTW(double VAW, double BAW, double SOG)
{
    double BTW_value = atan((VAW * sin(BAW))/(VAW * cos(BAW)-SOG));
    return BTW_value;
}

double VAW(double VTW, double BTW, double SOG)
{
    double VAW_value = sqrt(pow((VTW * sin(BTW)),2) + pow((VTW * cos(BTW)+ SOG), 2));
    return VAW_value;
}

double BAW(double VTW, double BTW,double SOG)
{
    double BAW_value = atan((VTW * sin(BTW))/(VTW * cos(BTW)+ SOG));
    return BAW_value;
}
/*
///////////////////////////// Collect Dialog ///////////////////////////////////

::CollectDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bNotebook;
	bNotebook = new wxBoxSizer( wxHORIZONTAL );

	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Collecting Data from: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bNotebook->Add( m_staticText9, 0, wxALL, 5 );

	m_staticTextFile = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFile->Wrap( -1 );
	bNotebook->Add( m_staticTextFile, 0, wxALL, 5 );


	bSizer10->Add( bNotebook, 0, 0, 5 );

	m_gauge1 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxSize( 300,-1 ), wxGA_HORIZONTAL );
	m_gauge1->SetValue( 0 ); 
	bSizer10->Add( m_gauge1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	this->SetSizer( bSizer10 );
	this->Layout();

	this->Centre( wxBOTH );
	this->Centre( wxBOTH );
}

CollectDlg::~CollectDlg()
{
}
*/

