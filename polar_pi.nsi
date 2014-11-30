; Script generated by the HM NIS Edit Script Wizard.
!include nsDialogs.nsh
!include LogicLib.nsh

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "polar_pi"
!define PRODUCT_VERSION "0.5"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"


XPStyle on
RequestExecutionLevel admin
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"

OutFile "Polar_pi-Setup.exe"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Czech.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\French.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\German.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Spanish.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Danish.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Polish.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Dutch.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\Turkish.nlf"
; License data
; Not exactly translated, but it shows what's needed
/*LicenseLangString myLicenseData ${LANG_ENGLISH} ""
LicenseLangString myLicenseData ${LANG_DUTCH} ""
LicenseLangString myLicenseData ${LANG_FRENCH} ""
LicenseLangString myLicenseData ${LANG_GERMAN} ""
LicenseLangString myLicenseData ${LANG_KOREAN} ""
LicenseLangString myLicenseData ${LANG_RUSSIAN} ""
LicenseLangString myLicenseData ${LANG_SPANISH} ""
LicenseLangString myLicenseData ${LANG_SWEDISH} ""
LicenseLangString myLicenseData ${LANG_TRADCHINESE} ""
LicenseLangString myLicenseData ${LANG_SIMPCHINESE} ""
LicenseLangString myLicenseData ${LANG_SLOVAK} ""

LicenseData $(myLicenseData)
*/

;Name $(Name)

; Set one text for all languages (simply don't use a LangString)
CompletedText "Polar completed"

var hwnd
var Group1Radio1
var Group1Radio2

#InstallDir "$PROGRAMFILES\OpenCPN"
InstallDir ""
Page custom nsDialogsPage
Page directory
Page instfiles
Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
;LicenseText "Falls Sie alle Bedingungen des Abkommens akzeptieren, klicken Sie auf 'Annehmen'. Sie m�ssen die Lizenzvereinbarungen anerkennen um $(^Name) zu installieren."
;LicenseData "C:\Dokumente und Einstellungen\Konni.MICKY-6B710455C\Eigene Dateien\temp\Readme.txt"
LicenseForceSelection checkbox
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
	;Language selection dialog
	Push ""
	Push ${LANG_ENGLISH}
	Push English
	Push ${LANG_DUTCH}
	Push Dutch
	Push ${LANG_FRENCH}
	Push French
	Push ${LANG_GERMAN}
	Push German
	Push ${LANG_POLISH}
	Push Polish
	Push ${LANG_TURKISH}
	Push Turkish
	Push ${LANG_SPANISH}
	Push Spanish
;	Push ${LANG_SWEDISH}
;	Push Swedish
	Push ${LANG_DANISH}
	Push Danish
;	Push ${LANG_TRADCHINESE}
;	Push "Traditional Chinese"
;	Push ${LANG_SIMPCHINESE}
;	Push "Simplified Chinese"
	Push ${LANG_CZECH}
	Push Czech

	Push A ; A means auto count languages
	       ; for the auto count to work the first empty push (Push "") must remain

	LangDLL::LangDialog "Installer Language" "Please select the language of the installer"

	Pop $LANGUAGE
	StrCmp $LANGUAGE "cancel" 0 +2
	Abort
	

FunctionEnd

Function nsDialogsPage

        StrCpy $INSTDIR "$PROGRAMFILES\OpenCPN"
        
	nsDialogs::Create 1018
	Pop $0

	GetFunctionAddress $0 OnBack
	nsDialogs::OnBack $0

	${NSD_CreateRadioButton} 60u 20% 40% 6% "Normal Installation"
		Pop $Group1Radio1
		${NSD_AddStyle} $Group1Radio1 ${WS_GROUP}
		${NSD_OnClick} $Group1Radio1 RadioClick
		${NSD_Check} $Group1Radio1
		
	${NSD_CreateLabel} 75u 25% 75% 6% "$PROGRAMFILES\OpenCPN"
	Pop $0
	
	${NSD_CreateRadioButton} 60u 40% 40% 6% "Parallel Installation"
		Pop $Group1Radio2
		${NSD_OnClick} $Group1Radio2 RadioClick

	${NSD_CreateLabel} 75u 45% 75% 6% "$PROGRAMFILES\OpenCPN 3.3.604"
	Pop $0
	
	nsDialogs::Show

FunctionEnd

Function RadioClick
	Pop $hwnd
	${If} $hwnd == $Group1Radio1
	    StrCpy $INSTDIR "$PROGRAMFILES\OpenCPN"
	${ElseIf} $hwnd == $Group1Radio2
	    StrCpy $INSTDIR "$PROGRAMFILES\OpenCPN 3.3.604"
	${EndIf}
FunctionEnd

Function OnBack

FunctionEnd

Section "DLL" SEC01
  LogSet On
  SetOutPath "$INSTDIR\plugins"
  SetOverwrite on
  File "C:\Program Files (x86)\OpenCPN\plugins\polar_pi.dll"
SectionEnd

Section "Languages" SEC02
/*   SetOutPath "$INSTDIR\share\locale\cs\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\cs\LC_MESSAGES\opencpn-polar_pi"
   */
   SetOutPath "$INSTDIR\share\locale\de\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\de\LC_MESSAGES\opencpn-polar_pi.mo"
   SetOutPath "$INSTDIR\share\locale\fr\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\fr\LC_MESSAGES\opencpn-polar_pi.mo"
/*
   SetOutPath "$INSTDIR\share\locale\es\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\es\LC_MESSAGES\opencpn-polar_pi.mo"
   SetOutPath "$INSTDIR\share\locale\da\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\da\LC_MESSAGES\opencpn-polar_pi.mo"
   SetOutPath "$INSTDIR\share\locale\pl\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\pl\LC_MESSAGES\opencpn-polar_pi.mo"
   SetOutPath "$INSTDIR\share\locale\nl\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\nl\LC_MESSAGES\opencpn-polar_pi.mo"
   SetOutPath "$INSTDIR\share\locale\tr_TR\LC_MESSAGES\"
   File "C:\Program Files (x86)\OpenCPN\share\locale\tr_TR\LC_MESSAGES\opencpn-polar_pi.mo"
   */
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\plugins\uninst_polar_pi.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\plugins\uninst_polar_pi.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfull uninstalled."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Please close OpenCPN now, if it's running for a clean uninstall,$\nbut don't close this window." IDYES +2
  Abort
FunctionEnd

Function un.RmLanguageFiles
 Exch $R0 ; exclude file
 Exch
 Exch $R1 ; route dir
 Push $R2
 Push $R3

  FindFirst $R3 $R2 "$R1\*.*"
  IfErrors Exit

  Top:
;   StrCmp $R2 "." Next
   StrCmp $R2 ".." Next
;   StrCmp $R2 $R0 Next
   IfFileExists "$R1\$R2\LC_MESSAGES\opencpn-polar_pi.mo"  Del

   #Goto Exit ;uncomment this to stop it being recursive (delete only one file)

   Del:
;    detailprint "$R1\$R2\LC_MESSAGES\opencpn-polar_pi.mo"
    Delete "$R1\$R2\LC_MESSAGES\opencpn-polar_pi.mo"
   Next:
    ClearErrors
    FindNext $R3 $R2
    IfErrors Exit
   Goto Top

  Exit:
  FindClose $R3

 Pop $R3
 Pop $R2
 Pop $R1
 Pop $R0
FunctionEnd

Section Uninstall
 /* SetShellVarContext all
  RMDir /r $APPDATA\opencpn\plugins\logbook
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/OverviewGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/ServiceGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/RepairsGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/BuyPartsGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/NavGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/WeatherGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/MotorGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/CrewGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/WakeGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/Logbook/EquipGridColWidth
  DeleteINISec $APPDATA\opencpn\opencpn.ini PlugIns/LogbookKonni
*/
  Push "$PROGRAMFILES\OpenCPN\share\locale"
  Push "file to exclude"
  Call un.RmLanguageFiles
;  Delete "$PROGRAMFILES\opencpn-polar_pi.mo"

  Delete $INSTDIR\polar_pi.dll
  Delete $INSTDIR\uninst_polar_pi.exe


  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  SetAutoClose false

SectionEnd