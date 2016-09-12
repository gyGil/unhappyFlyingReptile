
// UnhappyFlyingReptile.h : main header file for the UnhappyFlyingReptile application
//

/// \mainpage GAS. A2. Unhappy Flying Reptile.
///
/// \section intro Program Introduction
/// - Create backround images with chroma key.
/// - Make flying reptile on screen.
/// - Implement double buffering when window is resized.
/// - When user clicked on reptile, reptile is falling down to ground with proper sounds.
///
/// \section version Current version of this program
/// <ul>
/// <li>\author         GuenYoung Gil</li>
/// <li>\version        1.00.00</li>
/// <li>\date           2016.03.04</li>
/// <li>\copyright      GuenYoung Gil</li>
/// <ul>

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols



// CgasA1App:
// See gasA1.cpp for the implementation of this class
//

class CgasA1App : public CWinApp
{
public:
	CgasA1App();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CgasA1App theApp;
