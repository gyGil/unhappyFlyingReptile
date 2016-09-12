
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "UnhappyFlyingReptile.h"
#include "ChildView.h"
//#include "MyPainting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView


CChildView::CChildView()
{
	isStart = false;		// game is star
	isShootReptile = false;
	isReptileFlying = true;
	waitingCountForDying = 0;

}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
ON_WM_CREATE()
ON_WM_TIMER()
ON_WM_LBUTTONDOWN()
//ON_WM_MOUSEACTIVATE()
//ON_WM_MOVE()
//ON_WM_SETFOCUS()
ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS ,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	return TRUE;
}

/// <summary>
/// \details
/// - draw screen when paint message sent
///
/// \return <b>void</b>
/// </summary>
void CChildView::OnPaint() 
{
	// Device contexts allow device-independent drawing in Windows. 
	// It can be used to draw to the screen, to the printer, or to a metafile.
	CPaintDC dc(this); 

	// retrieves the coordinates of a window's client and get width and height
	RECT rectScreen;
	GetClientRect(&rectScreen);
	int x = rectScreen.top;
	int y = rectScreen.left;
	int width = rectScreen.right - rectScreen.left;
	int height = rectScreen.bottom - rectScreen.top;

	// on the first drawing, instantiate my object to allocate resource and drawing
	if (!isStart)
	{
		mp = new MyPainting(width,height);
		isStart = true;
	}

	// when window size is changed, change all window size related factors(members)
	mp->UpdateMembersWhenChageWinSize(width, height);	

	// creates a bitmap compatible with the device that is associated with the specified device context.
	HDC hdcMem = ::CreateCompatibleDC(dc);	

	// current state of the device context by copying state information. 
	// It can be used to restore the device context by calling RestoreDC. 
	const int nMemDC = ::SaveDC(hdcMem);	
																	
	HBITMAP hBitmap = ::CreateCompatibleBitmap(dc, width, height);	// HBITMAP: A handle to a bitmap.
	
	// selects an object into the specified device context (DC). 
	// The new object replaces the previous object of the same type
	::SelectObject(hdcMem, hBitmap);	

	// Make backgrounds and slingshot
	if (this->isShootReptile)
		mp->DrawBackGround(&hdcMem, x, y, width, height, true);	// Full screen effect
	else
		mp->DrawBackGround(&hdcMem, x, y, width, height, false);

	mp->DrawSlingShot(&hdcMem, width, height,1);
	mp->DrawSlingShot(&hdcMem, width, height, 2);

	// hit reptile at the first time
	if (this->isShootReptile)
	{
		mp->DyingReptile(&hdcMem, width, height, true);
		mp->SetScore(mp->GetScore() + MyPainting::SCORE_SHOT_REPTILE);
		this->isShootReptile = !this->isShootReptile;
	}
	else if (!this->isReptileFlying)	// reptile is not flying(is killed)
	{
		// when reptile hit to ground
		if (!mp->DyingReptile(&hdcMem, width, height, false))
		{
			// when reptile finish to dying action
			if (this->waitingCountForDying > 20)
			{
				this->waitingCountForDying = 0;
				this->isReptileFlying = true;
				mp->DrawReptile(&hdcMem, width, height, true);
				mp->RespowneBoxes(width, height);
				mp->SetHasBoxCollision(false);
			}
			++waitingCountForDying;
		}
	}
	else    // reptile is flying
	{
		mp->DrawReptile(&hdcMem, width, height, false);
	}	
	mp->CollisionHandle(width,height);
	mp->DrawBoxes(&hdcMem);
	mp->DrawScore(&hdcMem);

	// free resource for drawing
	mp->EndDrawing(width, height);

	// copy to screen from memory screen(double buffering)
	::BitBlt(dc, x, y, width, height, hdcMem, x, y, SRCCOPY);

	// release resource
	::RestoreDC(hdcMem, nMemDC);
	::DeleteObject(hBitmap);
	::DeleteDC(hdcMem);	
}

/// <summary>
/// \details
/// - prevent to draw with white paint when screen resized
/// </summary>
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
}

/// <summary>
/// \details
/// - set the timer
/// </summary>
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(1, 20, NULL);

	//ShowCursor(false);
	return 0;
}

/// <summary>
/// \details
/// - timer is hitted, force to redraw
/// </summary>
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnTimer(nIDEvent);
	Invalidate();
}

/// <summary>
/// \details
/// - If left button on mouse is clicked, check that position whether is clicked reptile or not.
///   If it is clicked, start falling animation with sound.
/// </summary>
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	// reference from https://msdn.microsoft.com/en-us/library/windows/desktop/dd743680(v=vs.85).aspx
	// play shooting sound when reptile is flying
	if (this->isReptileFlying)
	{
		PlaySound(L"res\\shoot.wav", NULL, SND_ASYNC);	

		// if user hit reptile, make sound to be hitted
		this->isShootReptile = mp->IsClickedReptile(point.x, point.y);
		if (this->isShootReptile)
		{
			PlaySound(L"res\\hit.wav", NULL, SND_ASYNC );
			this->isReptileFlying = false;
		}			
	}
}

/// <summary>
/// \details
/// - Change cursor appearance
/// </summary>
BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	
	// chage the cursor shape
	HCURSOR hCursor = LoadCursorFromFile(L"res\\cursor.cur");
	SetCursor(hCursor);

	return true;
}
