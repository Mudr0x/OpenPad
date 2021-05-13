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

// MainFrm.h : interface of the CSciCtrl class
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(SCICTRL)
#define SCICTRL

#define STR_SCINTILLAWND _T("Scintilla")


class CSciCtrl : public CWnd
{
// Construction / Destruction
public:
	CSciCtrl();
	virtual ~CSciCtrl();

// Operations
public:
	void Init();
	void SizeMargin();
	char * GetBuffer();
	void ClearAll();
	void AddText(const char * words);
	void SetText(const char * words);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSciCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT SendEditor(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0) {
		return ::SendMessage(this->GetSafeHwnd(), Msg, wParam, lParam);
	}

protected:
	void CSciCtrl::DoDataExchange(CDataExchange* pDX);
	void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0xff, 0xff, 0xff), int size = -1, const char* face = nullptr);
	// Process notification from scintilla control
	DECLARE_DYNAMIC(CSciCtrl)

	CWnd		m_wndScintilla;
	CString		m_strPathname;
	CStatic		m_gotoLabel, m_findLabel;
	CEdit		m_gotoEdit, m_findEdit;
	CButton		m_gotoOK, m_autoCheckBox;
	CButton		m_findNext, m_findPrev;
	BOOL		canOpenFile = TRUE;
	BOOL		gotoBtnEnabled = FALSE;
	BOOL		findNextBtnEnabled = FALSE;

	int line_number = 0;


	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#endif


