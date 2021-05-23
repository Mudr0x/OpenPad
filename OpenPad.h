//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

// OpenPad.h : main header file for the OpenPad application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "SingleInstance.h"


const char appID[] = _T("OpenPad.OpenPadApp.v_1_2");
const char className[] = _T("OpenPadClass");
const char guid[] = _T("84626DA8-50F0-404C-984D-9AF9277B6A7D");
const char regKey[] = _T("OpenPad\\OpenPadApp");


// COpenPadApp:
// See OpenPad.cpp for the implementation of this class
//

class COpenPadApp : public CWinApp, public CSingleInstance
{
public:
	COpenPadApp() noexcept;

protected:

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void WakeUp(LPCTSTR aCommandLine) const;

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenPadApp theApp;
