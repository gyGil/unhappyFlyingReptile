
// MainFrm.h : interface of the CMainFrame class
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
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CChildView    m_wndView;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


