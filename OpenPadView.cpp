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

// OpenPadView.cpp : implementation of the COpenPadView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenPad.h"
#endif

#include "OpenPadDoc.h"
#include "OpenPadView.h"
#include "MainFrm.h"

#include "Scintilla.h"
#include "SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// COpenPadView

IMPLEMENT_DYNCREATE(COpenPadView, CView)

BEGIN_MESSAGE_MAP(COpenPadView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
//	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_ALL, OnEditAll)

	ON_EN_CHANGE(ID_GOTOEDIT, OnEnChangeGotoEdit)
	ON_BN_CLICKED(ID_GOTOOK, OnGotoOK)
	ON_UPDATE_COMMAND_UI(ID_GOTOOK, OnUpdateGotoOK)
	ON_BN_CLICKED(ID_AUTO_CHECKBOX, OnAutoCheckBox)
	ON_EN_CHANGE(ID_FINDEDIT, OnEnChangeFindEdit)
	ON_BN_CLICKED(ID_FINDNEXT, OnFindNext)
	ON_UPDATE_COMMAND_UI(ID_FINDNEXT, OnUpdateFindNext)
	ON_BN_CLICKED(ID_FINDPREV, OnFindPrev)
	ON_UPDATE_COMMAND_UI(ID_FINDPREV, OnUpdateFindPrev)
	
	ON_COMMAND(ID_OPTIONS_ADDMARKER, OnOptionsAddmarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDMARKER, OnUpdateOptionsAddmarker)
	ON_COMMAND(ID_OPTIONS_DELETEMARKER, OnOptionsDeletemarker)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETEMARKER, OnUpdateOptionsDeletemarker)
	ON_COMMAND(ID_OPTIONS_FIND_NEXTMARKER, OnOptionsFindNextmarker)
	ON_COMMAND(ID_OPTIONS_FIND_PREVMARKER, OnOptionsFindPrevmarker)
END_MESSAGE_MAP()


void COpenPadView::DoDataExchange(CDataExchange* pDX)
{
	CView::DoDataExchange(pDX);
	DDX_Control(pDX, ID_GOTOEDIT, *m_gotoEdit);
	DDX_Control(pDX, ID_GOTOOK, *m_gotoOK);
	DDX_Control(pDX, ID_AUTO_CHECKBOX, *m_autoCheckBox);
	DDX_Control(pDX, ID_FINDEDIT, *m_findEdit);
	DDX_Control(pDX, ID_FINDNEXT, *m_findNext);
	DDX_Control(pDX, ID_FINDPREV, *m_findPrev);
}


// COpenPadView construction/destruction

COpenPadView::COpenPadView() noexcept
{
	p_viewSciCtrl = NULL;
}

COpenPadView::~COpenPadView()
{
	delete p_viewSciCtrl;
}

BOOL COpenPadView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	/*
	BOOL bPreCreated = CView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL | WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
	*/
	return CView::PreCreateWindow(cs);
}

// COpenPadView drawing

void COpenPadView::OnDraw(CDC* /*pDC*/)
{
	COpenPadDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

BOOL COpenPadView::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the Scintilla have first crack at the command
	if (p_viewSciCtrl && IsWindow(p_viewSciCtrl->m_hWnd))
		if (p_viewSciCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL COpenPadView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;

	/*			// For future purpose of future version of Scintilla perhaps
				// to bold margin number of caret line
				// for the moment SCI_MARGINSETSTYLE is only allowed for text margin
	// does notification come from my scintilla window?
	if (phDR != NULL && phDR->hwndFrom == hwndScintilla && phDR->code == SCN_UPDATEUI)
	{
		SCNotification *pMsg =	(SCNotification*)lParam;
		int curLine = SendEditor(SCI_LINEFROMPOSITION, SendEditor(SCI_GETCURRENTPOS));
		SendEditor(SCI_MARGINSETSTYLE, curLine, SC_WEIGHT_SEMIBOLD);
		return TRUE; // we processed the message
	}
	*/

	if (phDR != NULL && phDR->hwndFrom == p_viewSciCtrl->m_hWnd && phDR->code == SCN_CHARADDED)
	{
		COpenPadDoc* pDoc = GetDocument();
			pDoc->SetModifiedFlag(TRUE);
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}

void COpenPadView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_gotoOK = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOOK);
	m_autoCheckBox = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_AUTO_CHECKBOX);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);
	m_findNext = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDNEXT);
	m_findPrev = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDPREV);

	m_autoCheckBox->SetCheck(TRUE);
	gotoBtnEnabled = FALSE;
	m_gotoOK->UpdateWindow();
	findNextBtnEnabled = FALSE;
	m_findNext->UpdateWindow();
	findPrevBtnEnabled = FALSE;
	m_findPrev->UpdateWindow();
}


// COpenPadView message handlers

int COpenPadView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if (p_viewSciCtrl == NULL)             // Have we already done this bit?
	{
		p_viewSciCtrl = new CSciCtrl;     // Create the CSciCtrl object
		if (!p_viewSciCtrl) return -1;


		int IDC_SCINTILLA = 10000;
		if (!p_viewSciCtrl->Create(STR_SCINTILLAWND, _T("OpenPad"),
			WS_CHILD | WS_VISIBLE,
			CRect(0, 0, 0, 0), this, (UINT)IDC_SCINTILLA))
		{
			TRACE0("Failed to create Scintilla window\n");
			return -1;
		};
		p_viewSciCtrl->ShowWindow(SW_SHOW);
		p_viewSciCtrl->Init();
		COpenPadDoc* pDoc = GetDocument();
		pDoc->p_docSciCtrl = p_viewSciCtrl;
	}

	return 0;
}

void COpenPadView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (p_viewSciCtrl->GetSafeHwnd())
		p_viewSciCtrl->MoveWindow(0, 0, cx, cy, TRUE);
	
}

void COpenPadView::OnSetFocus(CWnd* pWnd)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);

	// forward focus to the Scintilla control
//	::SetFocus(p_viewSciCtrl);

	// or prefere to focus on line number input field
	m_gotoEdit->SetFocus();
}

void COpenPadView::OnEditCopy()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);


	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_gotoEdit->GetSafeHwnd())
		m_gotoEdit->Copy();
	else if (hFocused == m_findEdit->GetSafeHwnd())
		m_findEdit->Copy();
	else
		p_viewSciCtrl->SendEditor(WM_COPY);
}

void COpenPadView::OnEditPaste()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);


	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_gotoEdit->GetSafeHwnd())
		m_gotoEdit->Paste();
	else if (hFocused == m_findEdit->GetSafeHwnd())
		m_findEdit->Paste();
	else
		p_viewSciCtrl->SendEditor(WM_PASTE);
}

void COpenPadView::OnEditCut()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);


	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_gotoEdit->GetSafeHwnd())
		m_gotoEdit->Cut();
	else if (hFocused == m_findEdit->GetSafeHwnd())
		m_findEdit->Cut();
	else
		p_viewSciCtrl->SendEditor(WM_CUT);
}

void COpenPadView::OnEditUndo()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);


	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_gotoEdit->GetSafeHwnd())
		m_gotoEdit->Undo();
	else if (hFocused == m_findEdit->GetSafeHwnd())
		m_findEdit->Undo();
	else
		p_viewSciCtrl->SendEditor(SCI_UNDO);
}

void COpenPadView::OnEditRedo()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);


	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_gotoEdit->GetSafeHwnd()) {
		//		m_gotoEdit.Redo();					// to be implemented cause noway in normal way
		//		m_gotoEdit->SendMessage(WM_REDO);	// redo doesn't exist natively
	}
	else if (hFocused == m_findEdit->GetSafeHwnd()) {
		//		m_findEdit->Redo();					// to be implemented cause noway in normal way
		//		m_findEdit->SendMessage(WM_REDO);	// redo doesn't exist natively
	}
	else
		p_viewSciCtrl->SendEditor(SCI_REDO);
}

void COpenPadView::OnEditAll()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);


	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_gotoEdit->GetSafeHwnd())
		m_gotoEdit->SetSel(0, -1);
	else if (hFocused == m_findEdit->GetSafeHwnd())
		m_findEdit->SetSel(0, -1);
	else
		p_viewSciCtrl->SendEditor(SCI_SELECTALL);
}


void COpenPadView::OnEnChangeGotoEdit()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOEDIT);
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);
	m_gotoOK = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOOK);
	m_autoCheckBox = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_AUTO_CHECKBOX);

	CString s;
	m_gotoEdit->GetWindowText(s);
	LPCSTR c = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`¬!\"£$%^&*()-_=+[]{};:'@#~|,./<>?€";
	int j = s.FindOneOf(c);
	if (j > -1)
	{
		m_gotoEdit->SetWindowText("");
		//		m_gotoEdit.SetFocus();
		MessageBox("Invalid line number", "Error", MB_OK);
		return;
	}
	else
	{
		char * p;
		line_number = strtol(s, &p, 10);
		if (*p != 0) {
			m_gotoEdit->SetWindowText("");
			//		m_gotoEdit.SetFocus();
			MessageBox("Invalid line number", "Error", MB_OK);
			return;
		}
		if (line_number == 0) {
			gotoBtnEnabled = FALSE;
			m_gotoOK->UpdateWindow();
			return;
		}
	}

	if (m_autoCheckBox->GetCheck())
	{
		gotoBtnEnabled = FALSE;
		m_gotoOK->UpdateWindow();
		line_number = line_number - 1;

		// go to line
		p_viewSciCtrl->SendEditor(SCI_GOTOLINE, line_number);

		// select line
	//	int start_pos = SendEditor(SCI_POSITIONFROMLINE, line_number);
	//	int end_pos = SendEditor(SCI_GETLINEENDPOSITION, line_number);
	//	SendEditor(SCI_SETSEL, start_pos, end_pos);
	}

	else {
		line_number = line_number - 1;
		gotoBtnEnabled = TRUE;
		m_gotoOK->UpdateWindow();
	}
}

void COpenPadView::OnGotoOK()
{
	if (gotoBtnEnabled) {
		// go to line
		p_viewSciCtrl->SendEditor(SCI_GOTOLINE, line_number);
	}
}

void COpenPadView::OnUpdateGotoOK(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(gotoBtnEnabled);
}

void COpenPadView::OnAutoCheckBox()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_gotoOK = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_GOTOOK);

	if (line_number > 0) {
		if (gotoBtnEnabled) {
			gotoBtnEnabled = FALSE;
			m_gotoOK->UpdateWindow();
			// go to line
			p_viewSciCtrl->SendEditor(SCI_GOTOLINE, line_number);
		}
		else {
			gotoBtnEnabled = TRUE;
			m_gotoOK->UpdateWindow();
		}
	}
}

void COpenPadView::OnEnChangeFindEdit()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	m_findEdit = (CEdit*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDEDIT);
	m_findNext = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDNEXT);
	m_findPrev = (CButton*)pFrame->m_wndDlgBar.GetDlgItem(ID_FINDPREV);

	m_findEdit->GetWindowText(text_to_find);
	if (text_to_find == "")
	{
		findNextBtnEnabled = FALSE;
		m_findNext->UpdateWindow();
		findPrevBtnEnabled = FALSE;
		m_findPrev->UpdateWindow();
	}
	else {
		findNextBtnEnabled = TRUE;
		m_findNext->UpdateWindow();
		findPrevBtnEnabled = TRUE;
		m_findPrev->UpdateWindow();
	}
}

void COpenPadView::OnFindNext()
{
	Sci_TextToFind ft;
	ft.chrg.cpMin = static_cast<Sci_PositionCR>(p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS));
	ft.chrg.cpMax = static_cast<Sci_PositionCR>(p_viewSciCtrl->SendEditor(SCI_GETTEXTLENGTH));
	ft.lpstrText = text_to_find;
	p_viewSciCtrl->SendEditor(SCI_FINDTEXT, SCFIND_NONE, reinterpret_cast<LPARAM>(&ft));
	p_viewSciCtrl->SendEditor(SCI_SETSEL, ft.chrgText.cpMin, ft.chrgText.cpMax);
}

void COpenPadView::OnUpdateFindNext(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(findNextBtnEnabled);
}

void COpenPadView::OnFindPrev()
{
	Sci_TextToFind ft;
	ft.chrg.cpMin = static_cast<Sci_PositionCR>(p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS));
	ft.chrg.cpMax = static_cast<Sci_PositionCR>(p_viewSciCtrl->SendEditor(0));
	ft.lpstrText = text_to_find;
	p_viewSciCtrl->SendEditor(SCI_FINDTEXT, SCFIND_NONE, reinterpret_cast<LPARAM>(&ft));
	p_viewSciCtrl->SendEditor(SCI_SETSEL, ft.chrgText.cpMax, ft.chrgText.cpMin);
}

void COpenPadView::OnUpdateFindPrev(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(findPrevBtnEnabled);
}

void COpenPadView::OnOptionsAddmarker()
{
	const Sci_Position nPos = p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS);
	const int nLine = p_viewSciCtrl->SendEditor(SCI_LINEFROMPOSITION, nPos);
	p_viewSciCtrl->SendEditor(SCI_MARKERADD, nLine, 0);
}

void COpenPadView::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI)
{
	const Sci_Position nPos = p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS);
	const int nLine = p_viewSciCtrl->SendEditor(SCI_LINEFROMPOSITION, nPos);
	const int nBits = p_viewSciCtrl->SendEditor(SCI_MARKERGET, nLine);

	pCmdUI->Enable((nBits & 0x1) == 0);
}

void COpenPadView::OnOptionsDeletemarker()
{
	const Sci_Position nPos = p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS);
	const int nLine = p_viewSciCtrl->SendEditor(SCI_LINEFROMPOSITION, nPos);
	p_viewSciCtrl->SendEditor(SCI_MARKERDELETE, nLine, 0);
}

void COpenPadView::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI)
{
	const Sci_Position nPos = p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS);
	const int nLine = p_viewSciCtrl->SendEditor(SCI_LINEFROMPOSITION, nPos);
	const int nBits = p_viewSciCtrl->SendEditor(SCI_MARKERGET, nLine);

	pCmdUI->Enable(nBits & 0x1);
}

void COpenPadView::OnOptionsFindNextmarker()
{
	const Sci_Position nPos = p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS);
	const int nLine = p_viewSciCtrl->SendEditor(SCI_LINEFROMPOSITION, nPos);
	const int nFoundLine = p_viewSciCtrl->SendEditor(SCI_MARKERNEXT, nLine + 1, 0x1);
	if (nFoundLine >= 0)
		p_viewSciCtrl->SendEditor(SCI_GOTOLINE, nFoundLine);
	else
		MessageBeep(MB_ICONHAND);
}

void COpenPadView::OnOptionsFindPrevmarker()
{
	const Sci_Position nPos = p_viewSciCtrl->SendEditor(SCI_GETCURRENTPOS);
	const int nLine = p_viewSciCtrl->SendEditor(SCI_LINEFROMPOSITION, nPos);
	const int nFoundLine = p_viewSciCtrl->SendEditor(SCI_MARKERPREVIOUS, nLine - 1, 0x1);
	if (nFoundLine >= 0)
		p_viewSciCtrl->SendEditor(SCI_GOTOLINE, nFoundLine);
	else
		MessageBeep(MB_ICONHAND);
}



// COpenPadView printing

BOOL COpenPadView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CView preparation
	return CView::DoPreparePrinting(pInfo);
}

void COpenPadView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CView begin printing
	CView::OnBeginPrinting(pDC, pInfo);
}

void COpenPadView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CView end printing
	CView::OnEndPrinting(pDC, pInfo);
}


// COpenPadView diagnostics

#ifdef _DEBUG
void COpenPadView::AssertValid() const
{
	CView::AssertValid();
}

void COpenPadView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenPadDoc* COpenPadView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenPadDoc)));
	return (COpenPadDoc*)m_pDocument;
}
#endif //_DEBUG


