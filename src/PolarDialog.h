#ifndef __PolarDialog__
#define __PolarDialog__

/**
@file
Subclass of PolarDialog, which is generated by wxFormBuilder.
*/

#include "PolarGui.h"
#include "nmea0183/nmea0183.h"
#include <wx/timer.h>

//// end generated include
class polar_pi;
class Polar;
/** Implementing PolarDialog */
class PolarDialog : public PolarDlg
{
	public:
		// Handlers for PolarDlg events.
		void PolarDlgOnInitDialog( wxInitDialogEvent& event );
		void OnPaintPolar( wxPaintEvent& event );
		void OnSizePolar( wxSizeEvent& event );
		void OnChoicePolardisplay_speed( wxCommandEvent& event );
		void OnRelorTrue_Wind( wxCommandEvent& event );
		void OnChoiceWindPolar( wxCommandEvent& event );
		void OnChoiceSourcePolar( wxCommandEvent& event );
		void OnButtonClickFilterPolar( wxCommandEvent& event );

		void OnButtonClickSavePolar( wxCommandEvent& event );
		void OnButtonClickLoad( wxCommandEvent& event );
		void OnSplitterSashPosChanged( wxSplitterEvent& event );
		void OnChoiceMode( wxCommandEvent& event );
        void OnChoiceSource( wxCommandEvent& event );
		void OnGridCellChange( wxGridEvent& event );
        void OnGridLabelSelect( wxGridEvent& event);
		void OnNMEAtimer(wxTimerEvent& event);
//		void OnTimerRPM(wxTimerEvent& event);
		void OnSizesplitter1( wxSizeEvent& event );
		void OnToggleButtonRecord( wxCommandEvent& event );
		void OnButtonClickClearData( wxCommandEvent& event );
		void OnSizePolarDlg( wxSizeEvent& event );

		polar_pi*	plugin;
		Polar*		polar;

		wxTimer*	NMEA_timer;
		int			NMEA_timeout;
//		wxTimer*	timerRPM;

	public:
		/** Constructor */
		PolarDialog( wxWindow* parent, polar_pi* );
		~PolarDialog();

		void setToggleButtonRecord(bool status);
	//// end generated class members

	private:
	
};

#endif // __PolarDialog__
