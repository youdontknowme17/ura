#include "stdafx.h"
#include <assert.h>
#include "ListCtrlEx.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT( )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx construction/destruction

CListCtrlEx::CListCtrlEx()
{
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_clrHText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrHBkgnd = ::GetSysColor(COLOR_HIGHLIGHT);
	m_clrPercent = RGB(0, 0, 255); // ::GetSysColor(COLOR_HOTLIGHT);
	m_clrHPercent = ::GetSysColor(COLOR_BTNFACE);
}

CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int iSavedDC = pDC->SaveDC();             // Save DC state
        
	int iItem = lpDrawItemStruct->itemID;

	CRect rcBound;
	GetItemRect(iItem, rcBound, LVIR_BOUNDS);

	LVITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE | LVIF_INDENT | LVIF_PARAM;
	lvi.iItem = iItem;
	lvi.iSubItem = 0;	
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	bool bHighlight = (
		(lvi.state & LVIS_DROPHILITED) ||
		((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS)))
		);

	pDC->FillSolidRect(&rcBound, (bHighlight)? m_clrHBkgnd:m_clrBkgnd);

	// Draw labels for remaining columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
	for (int nColumn = 0; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcBound.right = rcBound.left + lvc.cx;
		CRect rcItem(rcBound);
		CString sItem = GetItemText(iItem, nColumn);
		
		rcItem.DeflateRect(2, 1);
		if (sItem.Right(1) == '%')
		{
			pDC->Rectangle(rcItem);
			rcItem.DeflateRect(1, 1);
			CRect rcLeft, rcRight;
			rcLeft = rcRight = rcItem;
			rcLeft.right = rcLeft.left + MulDiv(atoi(sItem), rcItem.Width(), 100);
			rcRight.left = rcLeft.right;
			if (bHighlight)
			{
				pDC->FillSolidRect(rcLeft, m_clrHPercent);
				pDC->FillSolidRect(rcRight, m_clrBkgnd);
				pDC->SetTextColor(m_clrText);
				pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
			}
			else
			{
				pDC->FillSolidRect(rcLeft, m_clrPercent);
				pDC->FillSolidRect(rcRight, m_clrHPercent);
				CRgn rgn;
				rgn.CreateRectRgnIndirect(rcLeft);
				pDC->SelectClipRgn(&rgn);
				pDC->SetTextColor(m_clrBkgnd);
				pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);

				rgn.SetRectRgn(rcRight);
				pDC->SelectClipRgn(&rgn);
				pDC->SetTextColor(m_clrText);
				pDC->DrawText(sItem, rcItem, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
				pDC->SelectClipRgn(NULL);
			}
		}
		else
		{
			if (nColumn == 0)
			{					
				m_ImageList.Draw(pDC,	0, rcItem.TopLeft(), ILD_NORMAL);				
				pDC->SetTextColor((bHighlight) ? m_clrHText:m_clrText);
				rcItem.MoveToX(20);
				pDC->DrawText(sItem, rcItem, DT_NOCLIP | DT_LEFT | DT_VCENTER);				
			}
			else
			{			
				pDC->SetTextColor((bHighlight) ? m_clrHText:m_clrText);
				pDC->DrawText(sItem, rcItem, DT_NOCLIP | DT_LEFT | DT_VCENTER);
			}
		}
		rcBound.left = rcBound.right;
	}

	pDC->RestoreDC(iSavedDC);                 // Restore DC.
}

void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 16;
}

void CListCtrlEx::Init(UINT nBitmapID)
{
	m_ImageList.Create(nBitmapID, 16, 1, RGB(255, 255, 255)); // Image List ����
	// WM_WINDOWPOSCHANGED�� �Ϻη� �߻����Ѽ� �θ� �����찡
	// WM_MEASUREITEM �޼����� �������� �Ѵ�.
	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
}
