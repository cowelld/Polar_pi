#ifndef _POLAR_H_
#define _POLAR_H_

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/dynarray.h>
#include <map>

#include "nmea0183.h"
#include "Options.h"
using namespace std;

WX_DEFINE_ARRAY_DOUBLE(int, ArrayOfDouble);

#define WINDDIR 32      // 16 five degree bins

class PolarDialog;
class polar_pi;
class FilterDlg;
class Options;

class NMEA0183;

class Polar
{
	enum PolColours {
	AQUAMARINE,
	BLUE,
	CYAN,
	YELLOW,
	YELLOWGREEN,
	GREEN,
	BROWN,
	RED,
	VIOLETRED,
	VIOLET
	};

public:
	Polar(PolarDialog* parent);
	~Polar(void);

	struct pol
	{
		double	wdir[WINDDIR];
		int		scount[WINDDIR];            // used to count key pairs??
		std::multimap<int,double> winddir; // key:wind direction & value:wind speed
		double wdirMax[WINDDIR];
		double wdirAve[WINDDIR];
		double wdirTotal[WINDDIR];
		int		count[WINDDIR];
    }Master_pol[10];               // i_wspd,j_wdir

    struct STE_point
    {
    wxString UTS,           // "yyyymmdd hh:mm:ss Z"
            Lat,            // deg.decimal
            Lon,
            SOG,
            COGT,
            COGM,
            Variation,
            Depth,
            CrseWind,
            RelWind,
            TrueWind,
            TWSpd,
            RWSpd,
            SpdParWind,
            BtHDG,
            BtWatSpd,
            WPLat,
            WPLon,
            WPRteCrse,
            XTE,
            BrngWP,
            pixels_knot_ratioWP,
            VMG,
            Waypoint;     
    } m_Point;

    struct wind{
        double TWA;
        double RWA;
        double TWS;
        double RWS;
        double CTWS;
        double CTWA; // corrected for tide/currents
        } Wind;

    struct boat{
        double SOG;
        double COG;
        double STW;
        double HDG;
    } Boat;


	wxDC*			dc;
	wxColour		windColour[12];
	polar_pi*		plugin;

	double			display_speed;
	int				display_inc_deg;
    int             data_source_index;

    wxString        wind_display_selection;
	double			wind_dir;
	double			wind_speed;
    int             j_wdir,i_wspd;
	wxString		wind_ref;

    double          boat_speed;
    double          max_boat_speed[10];
    int             set_drift_calc;
    double          current_set;
    double          current_drift;
    double          m_COG;
    double          m_HDG;

	bool			input_NMEA;
//	bool			engineRunning;
//	wxDateTime		dtRPM;

//    double          hull_speed;
//    double          boat_LWL;


	std::multimap<int,double>::iterator it;


	void Render_Polar();
	void createDiagram(wxDC& dc);
	void createSpeedBullets();
    void set_grid_indexes();
               
	void set_Dlg_for_DataSource(int sel);
	void load_POL_datum_str(wxString s, int row, int col);
	void parse_NMEA(wxString sentence);
    void check_grid_increments();
    bool validate_data(bool rel);

    void showFilterDlg();    
    bool filter_data();
	void insert_data_to_grid();
    void insert_data_to_Masterpol();
//	void setEngineStatus(wxString str);

    void clear_speeds_directions();

	void clear_Master_pol();
    void save_POL_file();
    void load_POL_file();

    void load_Logbook();
    void loadVDR();
    void loadSTE();
    void load_STE_point();
    void parse_STE_record(wxString m_instr);




private:
	PolarDialog*	dlg;
	FilterDlg*		filterDlg;

	NMEA0183       	m_NMEA0183;

	wxString		logbookDataPath;
	wxSize			max_dimension;
    wxPoint         center;
	int				radius;
	double			image_pixel_height[24];                 // display height in pixels
	double			pixels_knot_ratio;
    wxString        filter_message;


//	void calculateData();
};

static double deg2rad(double deg);
static double rad2deg(double rad);
double VTW(double VAW, double BAW, double SOG);
double BTW(double VAW, double BAW, double SOG);
double VAW(double VTW, double BTW, double SOG);
double BAW(double VTW, double BTW,double SOG);

#endif


// virtual void FunctionOnInitDialog( wxInitDialogEvent& event );


///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 30 2011)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __FILTERDLG_H__
#define __FILTERDLG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/panel.h>
#include <wx/string.h>
#include <wx/font.h>
#include <wx/Colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/dialog.h>


///////////////////////////////////////////////////////////////////////////////
/// Class FilterDlg
///////////////////////////////////////////////////////////////////////////////
class FilterDlg : public wxDialog 
{
	private:
		wxStaticText* m_staticTextUnitdisplay_speed;
		wxStaticText* m_staticTextUnitHeight;
		wxFlexGridSizer* fgSizer_FilterSails;
		wxButton* m_buttonSailsReset;
		wxStaticText* m_staticTextKnot;

		PolarDialog*	pfilt_dlg;      // Set to parent PolarDialog
		Polar*			pfilt_polar;    // Set to main polar

	protected:
		wxNotebook* m_Filter_notebook;
		wxPanel* boat_speed_range_panel;
		wxPanel* sails_panel;
		wxStaticText* m_staticText152;
		wxPanel* sea_state_panel;
		wxStaticText* m_staticText153;
		wxStaticText* m_staticText154;
		wxStdDialogButtonSizer* m_sdbNotebook;
		wxButton* m_sdbNotebookOK;
		wxButton* m_sdbNotebookCancel;
		
		// Virtual event handlers, overide them in your derived class
		void OnOKButtonClick( wxCommandEvent& event );
		void OnChoiceSpeedAvgMode( wxCommandEvent& event );
	    void OnSetDriftSelection( wxCommandEvent& event );
	public:
	    wxBoxSizer* bSizer_FilterRange;
        wxBoxSizer* bSizerMode;
        wxBoxSizer* bSizer_FilterSails;
		wxBoxSizer* bSizer52;
		wxBoxSizer* bSizer53;
        wxStaticText* txtAveragingMode;

        wxChoice* BoatSpeedFilter;
		wxChoice* boat_speed_range_pct;

        wxChoice* SetDriftFilter;
        wxStaticText* txtinput_Set_value;
        wxTextCtrl* input_Set_value;
        wxStaticText* txtinput_Drift_value;
        wxTextCtrl* input_Drift_value;
        wxString   current_set;
        wxString   current_drift;

        void OnButtonClickResetSails( wxCommandEvent& event );

		bool filterSails;
		wxCheckBox* checkboxSails[14];
        wxArrayString	Sails;
        wxArrayString	sail_name;

		wxChoice* checkboxWaveFrom[14];
		wxChoice* checkboxWaveTo[14];
		wxArrayString m_choiceChoicesFrom[10];
		wxArrayString m_choiceChoicesTo[10];

		void init();
		FilterDlg( PolarDialog* parent, Polar* polar,wxWindowID id = wxID_ANY, const wxString& title = _("Polar Data Filter"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 451,470 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~FilterDlg();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CollectDlg
///////////////////////////////////////////////////////////////////////////////
class CollectDlg : public wxDialog
{
	private:
	
	protected:
		wxStaticText* m_staticText9;
	
	public:		
		CollectDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 338,74 ), long style = 0 ); 
		~CollectDlg();

		wxStaticText* m_staticTextFile;
		wxGauge* m_gauge1;	
};

#endif 
