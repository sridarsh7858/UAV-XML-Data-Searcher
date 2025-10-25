
// XMLF.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CXMLFApp:
// See XMLF.cpp for the implementation of this class
//

class CXMLFApp : public CWinApp
{
public:
	CXMLFApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CXMLFApp theApp;
