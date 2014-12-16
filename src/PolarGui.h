///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __POLARGUI_H__
#define __POLARGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/Colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/tglbtn.h>
#include <wx/splitter.h>
#include <wx/dialog.h>
#include <wx/gauge.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PolarDlg
///////////////////////////////////////////////////////////////////////////////
class PolarDlg : public wxDialog
{
	private:
	
	public:
        wxStaticText* txtAveragingMode;
        wxChoice* m_Mode;
		wxStaticText* m_staticSourceData;
		wxStaticText* m_staticTextTopSpeed;
		wxChoice* m_TopSpeed;
		wxStaticText* m_staticTextwind_dirInterval;
		wxChoice* m_RelorTrue_Wind;
		wxStaticText* m_staticText148;
		wxChoice* m_displaywind_speed;

		wxStaticText* m_staticText146;
		wxChoice* m_sourceSpeed;
		wxStaticText* m_staticText_Current;
		wxChoice* m_CurrentSpeed;
		wxButton* m_buttonFilterPolar;

        wxStaticText* m_staticActions;
		wxButton* m_button_SavePOL;
		wxButton* m_button_LoadPOL;
		wxStaticLine* m_staticline45;
		wxButton* m_buttonClearData;
		wxToggleButton* m_toggleBtnRecord;
		
		// Virtual event handlers, overide them in your derived class
//		virtual void PolarDlgOnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnSizePolarDlg( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnChoiceMode( wxCommandEvent& event ) { event.Skip(); }
        virtual void OnChoiceSource( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoicePolardisplay_speed( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRelorTrue_Wind( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceWindPolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceSourcePolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickFilterPolar( wxCommandEvent& event ) { event.Skip(); }

		virtual void OnButtonClickSavePolar( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickLoad( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSizesplitter1( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnSplitterSashPosChanged( wxSplitterEvent& event ) { event.Skip(); }
		virtual void OnPaintPolar( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnGridCellChange( wxGridEvent& event ) { event.Skip(); }
		virtual void OnButtonClickClearData( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToggleButtonRecord( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxBoxSizer* bSizer_Banner;
		wxChoice* m_SourceData;
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panelPolar;
		wxPanel* m_panel_Grid;
		wxGrid* m_gridEdit;
//		wxStaticText* m_staticTextEngine;
        wxTextCtrl* m_staticTextRecord;
//        wxTextCtrl* m_staticTextFieldData;
		
		PolarDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Polar Diagram"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 968,602 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER );
		~PolarDlg();
		
		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 400 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( PolarDlg::m_splitter1OnIdle ), NULL, this );
		}
	
};

#endif //__POLARGUI_H__
