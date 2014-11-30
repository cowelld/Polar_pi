#ifndef _LOGOPTIONS_H_
#define _LOGOPTIONS_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/dynarray.h> 

WX_DEFINE_ARRAY_INT(int, ArrayOfGridColWidth);

class Sailing_Conditions
{
public:
	Sailing_Conditions(void){};
	~Sailing_Conditions(void){};

	wxString meter;
	wxString feet;

	// Logbook Grid Weather
	wxString	baro;
	wxString    temperature;
	wxString	windkts;
	wxString	windmeter;
	wxString	windkmh;

	// Sails
	wxArrayString	abrSails;
	wxArrayString	sailsName;
	bool			bSailIsChecked[14];
	int				rowGap;
	int				colGap;

};
#endif