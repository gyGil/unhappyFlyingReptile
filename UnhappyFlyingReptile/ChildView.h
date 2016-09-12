
// ChildView.h : interface of the CChildView class
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
#include "MyPainting.h"
#pragma once

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();
// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	MyPainting* mp;		// the class for drawing screen
	bool isStart;		// indicate whether start or not the game
	bool isShootReptile;	// indicate whether reptile is shooted or not
	bool isReptileFlying;	// indicate whether reptile is flying or not
	int waitingCountForDying;	// this count is increased from drop on ground to the end of dying
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);	// set the cursor type
};

