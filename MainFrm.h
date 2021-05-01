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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_MAINFRM_H__0BC0FB9E_F0B7_486A_A939_5894B9E590A9__INCLUDED_)
#define AFX_MAINFRM_H__0BC0FB9E_F0B7_486A_A939_5894B9E590A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const char appName[] = "OpenPad";
const char className[] = "OpenPadClass";
const int blockSize = 128 * 1024;

class CMainFrame : public CFrameWnd
{

public:
	CMainFrame();
	void ReadFile();
	void WriteFile();
	void SetWindowTitle();

private:
	HWND hwndScintilla;

protected:
	void CMainFrame::DoDataExchange(CDataExchange* pDX);
	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0xff, 0xff, 0xff), int size = -1, const char* face = nullptr);
	LRESULT SendEditor(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0) {
		return ::SendMessage(hwndScintilla, Msg, wParam, lParam);
	}
	// Process notification from scintilla control
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
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
	void Print(bool bShowPrintDialog);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar	m_wndStatusBar;
	CToolBar	m_wndToolBar;
	CEdit		m_editBox;
	CButton		m_buttonOK, m_checkBox;
	BOOL		btnOkEnabled = FALSE;
	BOOL		textChanged = FALSE;
	CString		m_strPathname;

	int line_number = 0;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditAll();
	afx_msg void OnFilePrint();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileNew();
	afx_msg void OnFileSaveas();
	afx_msg void OnEnChangeEdit();
	afx_msg void OnButtonOK();
	afx_msg void OnUpdateButtonOK(CCmdUI* pCmdUI);
	afx_msg void OnCheckBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0BC0FB9E_F0B7_486A_A939_5894B9E590A9__INCLUDED_)

