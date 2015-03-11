/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Polar Plugin
 * Author:   David Cowell
 * Based on work done by David Register
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can repixels_knot_ratioribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/tokenzr.h>
#include <wx/fileconf.h>

#include "polar_pi.h"
#include "PolarDialog.h"
#include "Polar.h"
#include "icons.h"
#include "Options.h"
#include "wx/textctrl.h"

#include "jsonreader.h"
#include "jsonwriter.h"

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new polar_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------


polar_pi::polar_pi(void *ppimgr)
      :opencpn_plugin_112(ppimgr)
{
      // Create the PlugIn icons
      initialize_images();
}

polar_pi::~polar_pi(void)
{
      delete _img_Polar_pi;
      delete _img_Polar;
}

int polar_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-polar_pi") );

      // Set some default private member parameters
      m_Polar_dialog_x = 0;
      m_Polar_dialog_y = 0;
      m_Polar_dialog_sx = 400;
      m_Polar_dialog_sy = 900;
      m_pPolarDialog = NULL;
      m_bShowPolar = false;

/*
	  m_pLogbook_Sailing_Conditions = new Logbook_Sailing_Conditions;
	  m_pLogbook_Sailing_Conditions->meter = _T("m");
	  m_pLogbook_Sailing_Conditions->feet  = _T("ft");

	  m_pLogbook_Sailing_Conditions->abrSails.Clear();
	  m_pLogbook_Sailing_Conditions->sail_name.Clear();
*/
      ::wxDisplaySize(&m_display_width, &m_display_height);

      m_pconfig = GetOCPNConfigObject();

      LoadConfig();

      // Get a pointer to the opencpn display canvas, to use as a parent for the Polar dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    This PlugIn needs a toolbar icon, so request its insertion if enabled locally
    m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_Polar, _img_Polar, wxITEM_NORMAL,
            _("Polar"), _T(""), NULL,
            POLAR_TOOL_POSITION, 0, this);

//      wxMenuItem *pmi = new wxMenuItem(NULL, -1, _("PlugIn Item"));
//      int miid = AddCanvasContextMenuItem(pmi, (PlugInCallBackFunction )&s_ContextMenuCallback );
//      SetCanvasContextMenuItemViz(miid, true);

      return (
           WANTS_TOOLBAR_CALLBACK    |
           INSTALLS_TOOLBAR_TOOL     |
           WANTS_CONFIG              |
		   WANTS_NMEA_SENTENCES      |
		   WANTS_NMEA_EVENTS		 |
           WANTS_PREFERENCES	//	 |
//		   WANTS_PLUGIN_MESSAGING
            );
}

bool polar_pi::DeInit(void)
{
//      printf("polar_pi DeInit()\n");
      if(m_pPolarDialog)
            m_pPolarDialog->Close();

      return true;
}
/*
void polar_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{		
      if(message_id == _T("LOGBOOK_ENGINEBUTTON1"))  //TODO Check if functional
      {
		if(m_pPolarDialog)
			m_pPolarDialog->polar->setEngineStatus(message_body);
      }
}
*/
int polar_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int polar_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int polar_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int polar_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *polar_pi::GetPlugInBitmap()
{
      return _img_Polar_pi;
}

wxString polar_pi::GetCommonName()
{
      return _("Polar");
}


wxString polar_pi::GetShortDescription()
{
      return _("Polar PlugIn for OpenCPN");
}


wxString polar_pi::GetLongDescription()
{
      return _("Polar PlugIn for OpenCPN\n\
");

}


int polar_pi::GetToolbarToolCount(void)
{
      return 1;
}

//*************************************************************************************************************
//**********Preferences Panel ***************

void polar_pi::ShowPreferencesDialog( wxWindow* parent )
{
    wxDialog *dialog = new wxDialog( parent, wxID_ANY, _("Polar Preferences"),
        wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE );

    int border_size = 4;

    wxBoxSizer* Display_Preferencs_panel = new wxBoxSizer(wxVERTICAL);
    dialog->SetSizer(Display_Preferencs_panel);

    //  Polar toolbox icon checkbox
    wxStaticBox* StaticBoxSizer = new wxStaticBox(dialog, wxID_ANY, _("Polar Configuration"));
    wxStaticBoxSizer* StaticBoxSizerPolar = new wxStaticBoxSizer(StaticBoxSizer, wxVERTICAL);
    Display_Preferencs_panel->Add(StaticBoxSizerPolar, 0, wxGROW|wxALL, border_size);
    
    pStatic_Boat_LWL = new wxStaticText(dialog, wxID_ANY, _("Boat Water Line Length"));
    StaticBoxSizerPolar->Add(pStatic_Boat_LWL, 1, wxALIGN_LEFT | wxALL, 2);

    m_pBoat_LWL = new wxTextCtrl( dialog, wxID_ANY);
	StaticBoxSizerPolar->Add( m_pBoat_LWL, 0, wxALL|wxALIGN_LEFT, 5 );
    m_pBoat_LWL->SetValue(m_boatLWL);

    pStatic_Wind_Inc = new wxStaticText(dialog, wxID_ANY, _("Wind Speed Increment"));
    StaticBoxSizerPolar->Add(pStatic_Wind_Inc, 1, wxALIGN_LEFT | wxALL, 2);

    m_tWind_Inc = new wxTextCtrl( dialog, wxID_ANY);
	StaticBoxSizerPolar->Add( m_tWind_Inc, 0, wxALL|wxALIGN_LEFT, 5 );
    Wind_Speed_inc = wxString::Format(_("%i"), Wind_Speed_increment);
    m_tWind_Inc->SetValue(Wind_Speed_inc);

    m_pPlaning = new wxCheckBox( dialog, -1, _("Planing boat."));
    StaticBoxSizerPolar->Add(m_pPlaning, 1, wxALIGN_LEFT|wxALL, border_size);
    m_pPlaning->SetValue(m_bPlaning);

    m_pSpeed_limit = new wxCheckBox( dialog, -1, _("Use lower speed limit of 70 percent"));
    StaticBoxSizerPolar->Add(m_pSpeed_limit, 1, wxALIGN_LEFT|wxALL, border_size);
    m_pPlaning->SetValue(m_bSpeed_limit);

    m_pPolarUseMS = new wxCheckBox( dialog, -1, _("Show meters/sec for Wind Speed"));
    StaticBoxSizerPolar->Add(m_pPolarUseMS, 1, wxALIGN_LEFT|wxALL, border_size);

    m_pPolarUseMS->SetValue(m_bPolarUseMS);


    wxStdDialogButtonSizer* DialogButtonSizer = dialog->CreateStdDialogButtonSizer(wxOK|wxCANCEL);
    Display_Preferencs_panel->Add(DialogButtonSizer, 0, wxALIGN_RIGHT|wxALL, 5);

    dialog->Fit();

    if(dialog->ShowModal() == wxID_OK)  // Use this instead of Event driven routines.
      {
            if(m_boatLWL != m_pBoat_LWL->GetValue()){
                m_boatLWL = m_pBoat_LWL->GetValue();
                m_boatLWL.ToDouble(&m_boat_LWL);
                m_hull_speed = 1.43*sqrt(m_boat_LWL);
            }

            if(Wind_Speed_inc != m_tWind_Inc->GetValue()){
                Wind_Speed_inc = m_tWind_Inc->GetValue();
            }
            Wind_Speed_inc.ToLong(&Wind_Speed_increment);
            

            if (m_bPlaning != m_pPlaning->GetValue()){
                  m_bPlaning = m_pPlaning->GetValue();
            }

            if (m_bSpeed_limit != m_pSpeed_limit->GetValue()){
                  m_bSpeed_limit = m_pSpeed_limit->GetValue();
            }

            if(m_bPolarUseMS != m_pPolarUseMS->GetValue()) // If change of WS units kill dialog and restart.
            {
                  m_bPolarUseMS = m_pPolarUseMS->GetValue();

                  if(m_pPolarDialog)
                  {
                    m_pPolarDialog->Destroy();

                    m_pPolarDialog = new PolarDialog( m_parent_window, this);
					m_pPolarDialog->SetSize(m_Polar_dialog_sx, m_Polar_dialog_sy);
                    m_pPolarDialog->Show();                        // Show modeless, so it stays on the screen
                    SetToolbarItemState( m_leftclick_tool_id, true );
                  }
                  else
                    SetToolbarItemState( m_leftclick_tool_id, false );

            }
            SaveConfig();
      }
}

//************************************************************************************************************************
//********* Start of Plug-in *******************

void polar_pi::OnToolbarToolCallback(int id)
{
//      printf("polar_pi ToolCallBack()\n");
//     ::wxBell();

      // Qualify the Polar dialog position
            bool b_reset_pos = false;


#ifdef __WXMSW__
        //  Support MultiMonitor setups which an allow negative window positions.
        //  If the requested window does not intersect any installed monitor,
        //  then default to simple primary monitor positioning.
            RECT frame_title_rect;
            frame_title_rect.left =   m_Polar_dialog_x;
            frame_title_rect.top =    m_Polar_dialog_y;
            frame_title_rect.right =  m_Polar_dialog_x + m_Polar_dialog_sx;
            frame_title_rect.bottom = m_Polar_dialog_y + 30;


            if(NULL == MonitorFromRect(&frame_title_rect, MONITOR_DEFAULTTONULL))
                  b_reset_pos = true;
#else
       //    Make sure drag bar (title bar) of window on Client Area of screen, with a little slop...
            wxRect window_title_rect;                    // conservative estimate
            window_title_rect.x = m_Polar_dialog_x;
            window_title_rect.y = m_Polar_dialog_y;
            window_title_rect.width = m_Polar_dialog_sx;
            window_title_rect.height = 30;

            wxRect ClientRect = wxGetClientDisplayRect();
            ClientRect.Deflate(60, 60);      // Prevent the new window from being too close to the edge
            if(!ClientRect.Intersects(window_title_rect))
                  b_reset_pos = true;

#endif

            if(b_reset_pos)
            {
                  m_Polar_dialog_x = 20;
                  m_Polar_dialog_y = 100;
                  m_Polar_dialog_sx = 400;
                  m_Polar_dialog_sy = 900;
            }

      //Toggle Polar display
      m_bShowPolar = !m_bShowPolar;

      // show the Polar dialog
	  if(NULL == m_pPolarDialog)
	  {
		  m_pPolarDialog = new PolarDialog(m_parent_window, this);
//		  if(m_pLogbook_Sailing_Conditions->sail_name.Count() == 0)                           // no logbook data
		  {
			  m_pPolarDialog->SetSize(m_Polar_dialog_sx, m_Polar_dialog_sy);
			  m_pPolarDialog->m_SourceData->SetSelection(1); // to NMEA
			  m_pPolarDialog->polar->set_Dlg_for_DataSource(1); // to NMEA
              m_hull_speed = 1.43*sqrt(m_boat_LWL);       // hull speed
		  }

          m_pPolarDialog->Hide();                        
      }

      //    Toggle dialog?
      if(m_bShowPolar)
            m_pPolarDialog->Show();
      else
            m_pPolarDialog->Hide();


      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowPolar );


}

void polar_pi::OnPolarDialogClose()
{
      SetToolbarItemState( m_leftclick_tool_id, false );

      if(m_pPolarDialog){
	  		m_pPolarDialog->GetSize(&m_Polar_dialog_sx, &m_Polar_dialog_sy);
            m_pPolarDialog->Hide();
	  }
      m_bShowPolar = false;

      SaveConfig();
}

void polar_pi::SetNMEASentence(wxString &sentence)
{
	if(m_pPolarDialog && m_pPolarDialog->polar->input_NMEA)
      {
		  m_pPolarDialog->polar->parse_NMEA(sentence);
      }
}

void polar_pi::SetPositionFix(PlugIn_Position_Fix &pfix)
{
}

bool polar_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/PlugIns/Polar" ) );
            pConf->Read ( _T( "PolarUseMS" ), &m_bPolarUseMS, 0 );
            pConf->Read ( _T( "PolarPlaningBoat" ), &m_bPlaning, 0 );
            pConf->Read ( _T( "PolarBoatLWL" ) ,&m_boatLWL, _T("0.0") );
            m_boatLWL.ToDouble(&m_boat_LWL);
            m_hull_speed = 1.43*sqrt(m_boat_LWL);
            pConf->Read ( _T( "PolarSpeedlimit" ), &m_bSpeed_limit, 0 );
            pConf->Read ( _T( "PolarWindDirInc" ), &Wind_Dir_inc, _("5") );
            Wind_Dir_increment = wxAtol(Wind_Dir_inc);
            pConf->Read ( _T( "PolarWindSpdInc" ), &Wind_Speed_inc, _("5") );
            Wind_Speed_increment = wxAtol(Wind_Speed_inc);
            pConf->Read ( _T( "PolarInitWindDir" ), &Init_dir, _("25") );
            initial_Dir= wxAtol(Init_dir);

            m_Polar_dialog_sx = pConf->Read ( _T ( "PolarDialogSizeX" ), 400L );
            m_Polar_dialog_sy = pConf->Read ( _T ( "PolarDialogSizeY" ), 800L );
            m_Polar_dialog_x =  pConf->Read ( _T ( "PolarDialogPosX" ), 20L );
            m_Polar_dialog_y =  pConf->Read ( _T ( "PolarDialogPosY" ), 100L );


            //pConf->SetPath ( _T ( "/PlugIns/Polar/Directories" ) );
 /*           pConf->Read ( _T ( "PolarDirectory" ), &m_Polar_dir );

			pConf->SetPath ( _T( "/PlugIns/Logbook" ) );
			wxString sails = wxEmptyString;
			pConf->Read ( _T ( "Sails" ), &sails);
			if(!sails.IsEmpty())
			{
				wxStringTokenizer tkz(sails,_T(","));
				if(wxString(sails.GetChar(0)).IsNumber())
				{
					m_pLogbook_Sailing_Conditions->rowGap = wxAtoi(tkz.GetNextToken());
					m_pLogbook_Sailing_Conditions->colGap = wxAtoi(tkz.GetNextToken());
				}
				
				for(int i = 0; i < 14; i++)
				{
					m_pLogbook_Sailing_Conditions->abrSails.Add(tkz.GetNextToken());
					m_pLogbook_Sailing_Conditions->sail_name.Add(tkz.GetNextToken());
					m_pLogbook_Sailing_Conditions->bSailIsChecked[i] = (wxAtoi(tkz.GetNextToken())?true:false);
				}
			}*/
            return true;
      }
      else
            return false;
}

bool polar_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T("/Plugins/Polar" ) );
            pConf->Write ( _T ( "PolarUseMS" ), m_bPolarUseMS );
            pConf->Write ( _T( "PolarPlaningBoat" ), m_bPlaning);
            pConf->Write ( _T( "PolarBoatLWL" ) , m_boatLWL);
            pConf->Write ( _T( "PolarSpeedlimit" ), m_bSpeed_limit );
            Wind_Dir_inc = wxString::Format(_("%i"), Wind_Dir_increment);
            pConf->Write ( _T("PolarWindDirInc" ), Wind_Dir_inc);
            Wind_Speed_inc = wxString::Format(_("%i"),Wind_Speed_increment);
            pConf->Write ( _T("PolarWindSpdInc" ), Wind_Speed_inc);
            Init_dir = wxString::Format(_("%i"), initial_Dir);
            pConf->Write ( _T( "PolarInitWindDir" ),Init_dir );
            
            pConf->Write ( _T ( "PolarDialogSizeX" ),  m_Polar_dialog_sx );
            pConf->Write ( _T ( "PolarDialogSizeY" ),  m_Polar_dialog_sy );
            pConf->Write ( _T ( "PolarDialogPosX" ),   m_Polar_dialog_x );
            pConf->Write ( _T ( "PolarDialogPosY" ),   m_Polar_dialog_y );

           // pConf->SetPath ( _T ( "/Directories" ) );
            pConf->Write ( _T ( "PolarDirectory" ), m_Polar_dir );

            return true;
      }
      else
            return false;
}
/*
void polar_pi::SetColourScheme(PIpdorScheme cs)
{

      DimeWindow(m_pPolarDialog);

}
*/