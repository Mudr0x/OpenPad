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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OpenPad.h"
#include "MainFrm.h"
#include "Scintilla.h"
#include "SciLexer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* g_cppKeyWords =
_T("and and_eq asm auto bitand bitor bool break ")
_T("case catch char class compl const const_cast continue ")
_T("default delete do double dynamic_cast else enum explicit export extern false float for ")
_T("friend goto if inline int long mutable namespace new not not_eq ")
_T("operator or or_eq private protected public ")
_T("register reinterpret_cast return short signed sizeof static static_cast struct switch ")
_T("template this throw true try typedef typeid typename union unsigned using ")
_T("virtual void volatile wchar_t while xor xor_eq ");  // to be adapted for oppl language


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_ALL, OnEditAll)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_EN_CHANGE(ID_EDITBOX, &CMainFrame::OnEnChangeEdit)
	ON_UPDATE_COMMAND_UI(ID_BUTTONOK, OnUpdateButtonOK)
	ON_BN_CLICKED(ID_BUTTONOK, OnButtonOK)
	ON_BN_CLICKED(ID_CHECKBOX, OnCheckBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CFrameWnd::DoDataExchange(pDX);
	DDX_Control(pDX, ID_EDITBOX, m_editBox);
	DDX_Control(pDX, ID_BUTTONOK, m_buttonOK);
	DDX_Control(pDX, ID_CHECKBOX, m_checkBox);
}


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_strPathname = "";
}

CMainFrame::~CMainFrame()
{

}

/*			// For future purpose of future version of Scintilla perhaps
			// to bold margin number of caret line
			// for the moment SCI_MARGINSETSTYLE is only allowed for text margin
BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;
	// does notification come from my scintilla window?
	if (phDR != NULL && phDR->hwndFrom == hwndScintilla && phDR->code == SCN_UPDATEUI)
	{
		SCNotification *pMsg =	(SCNotification*)lParam;
		int curLine = SendEditor(SCI_LINEFROMPOSITION, SendEditor(SCI_GETCURRENTPOS));
		SendEditor(SCI_MARGINSETSTYLE, curLine, SC_WEIGHT_SEMIBOLD);
		return TRUE; // we processed the message
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}
*/

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect hwndRect;
	GetWindowRect(hwndRect);
	// create a Scintilla window to occupy the client area of the frame
	hwndScintilla = CreateWindow("Scintilla", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL,
		0, 25, hwndRect.Width() - 15, hwndRect.Height() - 100, this->GetSafeHwnd(), NULL, (HINSTANCE)GetWindowLong(this->GetSafeHwnd(), GWL_HINSTANCE), NULL);
	if (!hwndScintilla)
	{
		TRACE0("Failed to create Scintilla window\n");
		return -1;
	}

	// Set extended style: 3D look - ie. sunken edge
	::SetWindowLong(hwndScintilla, GWL_EXSTYLE, WS_EX_CLIENTEDGE);



	// Create the toolbar /////

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Toolbar customization for line scroll
	CRect rect;
	int nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_EDITBOX);
	m_wndToolBar.SetButtonInfo(nIndex, ID_EDITBOX, TBBS_BUTTON, 205);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.left = rect.left + 10;
	rect.right = rect.left + 205;
	if (!m_editBox.Create(WS_VISIBLE | WS_BORDER| WS_TABSTOP, rect, &m_wndToolBar, ID_EDITBOX))
	{
		TRACE(_T("Failed to create edit-box\n"));
		return FALSE;
	}

	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_BUTTONOK);
	m_wndToolBar.SetButtonInfo(nIndex, ID_BUTTONOK, TBBS_BUTTON, 205);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.left = rect.left + 200;
	rect.right = rect.left + 30;
	if (!m_buttonOK.Create("OK", WS_VISIBLE | WS_TABSTOP, rect, &m_wndToolBar, ID_BUTTONOK))
	{
		TRACE(_T("Failed to create button\n"));
		return FALSE;
	}

	nIndex = m_wndToolBar.GetToolBarCtrl().CommandToIndex(ID_CHECKBOX);
	m_wndToolBar.SetButtonInfo(nIndex, ID_CHECKBOX, TBBS_CHECKBOX, 205);
	m_wndToolBar.GetToolBarCtrl().GetItemRect(nIndex, &rect);
	rect.top = 1;
	rect.left = rect.left + 215;
	rect.right = rect.left + 100;
	if (!m_checkBox.Create("Auto-scroll", WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP, rect, &m_wndToolBar, ID_CHECKBOX))
	{
		TRACE(_T("Failed to create checkbox\n"));
		return FALSE;
	}
	m_checkBox.SetCheck(TRUE);

//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);

	// End of toolbar creation


	// Create the status bar /////

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
			sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Set Charactistics of the Status Bar
	UINT nID, nStyle;
	int cxWidth;

	m_wndStatusBar.GetPaneInfo(0, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(0, nID, SBPS_NORMAL | SBPS_STRETCH, cxWidth);

	// End of status bar creation /////


	// Global set up of Scintilla control /////

	// Set up caret line highlighting
	SendEditor(SCI_SETCARETLINEVISIBLE, TRUE);
	SendEditor(SCI_SETCARETLINEVISIBLEALWAYS, TRUE);
	//	SendEditor(SCI_SETCARETLINEBACK, RGB(204, 204, 255));
	//	SendEditor(SCI_SETCARETLINEBACKALPHA, 99);

	// Set up line number margin
	SendEditor(SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
	int margin_size = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM) "_9999");
	SendEditor(SCI_SETMARGINWIDTHN, 0, margin_size);

	// Set up the Lexer
	SendEditor(SCI_SETLEXER, SCLEX_CPP);
	SendEditor(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(g_cppKeyWords));

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 12, "Lucida Sans");
	SendEditor(SCI_STYLESETWEIGHT, STYLE_DEFAULT, SC_WEIGHT_SEMIBOLD);
	SendEditor(SCI_STYLECLEARALL);	// Copies global style to all others
	SendEditor(SCI_STYLESETWEIGHT, STYLE_LINENUMBER, SC_WEIGHT_NORMAL);
	SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
	SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
	SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
	SendEditor(SCI_STYLESETBOLD, SCE_C_WORD, TRUE);
	SetAStyle(SCE_C_STRING, RGB(0x80, 0x80, 0));  // override 0x80, 0, 0x80
	SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
	SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
	SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0, 0x80));  // override 0x80, 0x80, 0

	// Set up event mask
//	SendEditor(SCI_SETMODEVENTMASK, SCN_UPDATEUI);  // For future purpose of future version of Scintilla perhaps
													// to bold margin number of caret line
													// for the moment SCI_MARGINSETSTYLE is only allowed for text margin

	// End of set up of Scintilla control /////



	// Support Drag and Drop
	DragAcceptFiles();


	SetWindowTitle();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0, 0, 0, LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)));
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG}

void CMainFrame::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face)
{
	SendEditor(SCI_STYLESETFORE, style, fore);
	SendEditor(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendEditor(SCI_STYLESETSIZE, style, size);
	if (face)
		SendEditor(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* pWnd)
{
	// forward focus to the Scintilla window
//	::SetFocus(hwndScintilla);

	// or prefere to focus on line number input field
	m_editBox.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the Scintilla have first crack at the command
//	if (GetControlContainer()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnEditPaste()
{
	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_editBox.GetSafeHwnd())
		m_editBox.Paste();
	else
		SendEditor(SCI_PASTE);
}

void CMainFrame::OnEditCut()
{
	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_editBox.GetSafeHwnd())
		m_editBox.Cut();
	else
	SendEditor(WM_CUT);
}

void CMainFrame::OnEditCopy()
{
	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_editBox.GetSafeHwnd())
		m_editBox.Copy();
	else
	SendEditor(SCI_COPY);
}

void CMainFrame::OnEditUndo()
{
	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_editBox.GetSafeHwnd())
		m_editBox.Undo();
	else
	SendEditor(SCI_UNDO);
}

void CMainFrame::OnEditRedo()
{
	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_editBox.GetSafeHwnd()) {
//		m_editBox.Redo();					// to be implemented cause noway in normal way
//		m_editBox.SendMessage(WM_REDO);		// redo doesn't exist natively
	}
	else
		SendEditor(SCI_REDO);
}

void CMainFrame::OnEditAll()
{
	HWND hFocused = GetFocus()->GetSafeHwnd();
	if (hFocused == m_editBox.GetSafeHwnd())
		m_editBox.SetSel(0, -1);
	else
		SendEditor(SCI_SELECTALL);
}

void CMainFrame::OnFilePrint()
{
	Print(true);
}

void CMainFrame::Print(bool bShowPrintDialog)
{
	CPrintDialog printDialog(false);

	if (bShowPrintDialog)
	{
		if (printDialog.DoModal() == IDCANCEL)
			return; // User pressed cancel, don't print.
	}
	else
	{
		printDialog.GetDefaults();
	}

	HDC hPrinterDC = printDialog.GetPrinterDC();

	// This code basically taken from MS KB article Q129860

	Sci_RangeToFormat fr;
	int	  nHorizRes = GetDeviceCaps(hPrinterDC, HORZRES);
	int	  nVertRes = GetDeviceCaps(hPrinterDC, VERTRES);
	int     nLogPixelsX = GetDeviceCaps(hPrinterDC, LOGPIXELSX);
	int     nLogPixelsY = GetDeviceCaps(hPrinterDC, LOGPIXELSY);
	LONG	      lTextLength;   // Length of document.
	LONG	      lTextPrinted;  // Amount of document printed.

   // Ensure the printer DC is in MM_TEXT mode.
	SetMapMode(hPrinterDC, MM_TEXT);

	// Rendering to the same DC we are measuring.
	ZeroMemory(&fr, sizeof(fr));
	fr.hdc = fr.hdcTarget = hPrinterDC;

	// Set up the page.
	fr.rcPage.left = fr.rcPage.top = 0;
	fr.rcPage.right = (nHorizRes / nLogPixelsX) * 1440;
	fr.rcPage.bottom = (nVertRes / nLogPixelsY) * 1440;

	// Set up 0" margins all around.
	fr.rc.left = fr.rcPage.left;//+ 1440;  // 1440 TWIPS = 1 inch.
	fr.rc.top = fr.rcPage.top;//+ 1440;
	fr.rc.right = fr.rcPage.right;//- 1440;
	fr.rc.bottom = fr.rcPage.bottom;//- 1440;

   // Default the range of text to print as the entire document.
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;
	SendEditor(SCI_FORMATRANGE, true, reinterpret_cast<LPARAM>(&fr));

	// Set up the print job (standard printing stuff here).
	DOCINFO di;
	ZeroMemory(&di, sizeof(di));
	di.cbSize = sizeof(DOCINFO);

	di.lpszDocName = m_strPathname;

	// Do not print to file.
	di.lpszOutput = NULL;


	// Start the document.
	StartDoc(hPrinterDC, &di);

	// Find out real size of document in characters.
	lTextLength = SendEditor(SCI_GETTEXTLENGTH);

	do
	{
		// Start the page.
		StartPage(hPrinterDC);

		// Print as much text as can fit on a page. The return value is
		 // the index of the first character on the next page. Using TRUE
		 // for the wParam parameter causes the text to be printed.

		lTextPrinted = SendEditor(SCI_FORMATRANGE, true, reinterpret_cast<LPARAM>(&fr));
		SendEditor(EM_DISPLAYBAND, reinterpret_cast<LPARAM>(&fr.rc));

		// Print last page.
		EndPage(hPrinterDC);

		// If there is more text to print, adjust the range of characters
		// to start printing at the first character of the next page.
		if (lTextPrinted < lTextLength)
		{
			fr.chrg.cpMin = lTextPrinted;
			fr.chrg.cpMax = -1;
		}
	} while (lTextPrinted < lTextLength);

	// Tell the control to release cached information.
	SendEditor(SCI_FORMATRANGE, false, NULL);

	EndDoc(hPrinterDC);

	DeleteDC(hPrinterDC);

}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	CString FileName;

	::DragQueryFile(hDropInfo, 0, FileName.GetBufferSetLength(_MAX_PATH), _MAX_PATH);
	FileName.ReleaseBuffer();
	::DragFinish(hDropInfo);

	m_strPathname = FileName;

	SetWindowTitle();
	ReadFile();
}

void CMainFrame::ReadFile()
{
	CString fullPath = m_strPathname;

	// Convert full filename characters from "\" to "\\"
	fullPath.Replace(_T("\\"), _T("\\\\"));

	// The file from which to load the contents of the Scintilla control.
	FILE *fp = fopen(fullPath, "rb");

	if (fp) {
		char data[blockSize];
		int lenFile = fread(data, 1, sizeof(data), fp);
		while (lenFile > 0) {
			SendEditor(SCI_ADDTEXT, lenFile,
				reinterpret_cast<LPARAM>(static_cast<char *>(data)));
			lenFile = fread(data, 1, sizeof(data), fp);
		}
		fclose(fp);
	}
	else {
		char msg[MAX_PATH + 100];
		strcpy(msg, "Could not open file \"");
		strcat(msg, fullPath);
		strcat(msg, "\".");
		::MessageBox(this->GetSafeHwnd(), msg, appName, MB_OK);
	}
	SendEditor(SCI_SETUNDOCOLLECTION, 1);
	::SetFocus(hwndScintilla);
	SendEditor(EM_EMPTYUNDOBUFFER);
	SendEditor(SCI_SETSAVEPOINT);
	//SendEditor(SCI_GOTOPOS, 0);


	// Set up margin number width /////

	// get width of character '9' in pixels
	int margin_size = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, (long)_T("9"));
	// try to get number of lines in control
	LRESULT lLines = SendEditor(SCI_GETLINECOUNT);

	int nChars = 1;
	while (lLines > 0)
	{
		lLines = lLines / 10;
		nChars++;
	}
	nChars++; // allow an extra char for safety
	SendEditor(SCI_SETMARGINWIDTHN, 0, nChars * margin_size);

	// End Set up of margin number width /////
	
}

void CMainFrame::WriteFile()
{
	CString fullPath = m_strPathname;

	// Convert full filename characters from "\" to "\\"
	fullPath.Replace(_T("\\"), _T("\\\\"));

	// The file from which to save the contents of the Scintilla control.
	FILE *fp = fopen(fullPath, "wb");
	if (fp) {
		char data[blockSize + 1];
		int lengthDoc = SendEditor(SCI_GETLENGTH);
		for (int i = 0; i < lengthDoc; i += blockSize) {
			int grabSize = lengthDoc - i;
			if (grabSize > blockSize)
				grabSize = blockSize;
			TEXTRANGE tr;
			tr.chrg.cpMin = i;
			tr.chrg.cpMax = i + grabSize;
			tr.lpstrText = data;
			SendEditor(EM_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
			fwrite(data, grabSize, 1, fp);
		}
		fclose(fp);
		SendEditor(SCI_SETSAVEPOINT);
	}
	else {
		char msg[MAX_PATH + 100];
		strcpy(msg, "Could not save file \"");
		strcat(msg, fullPath);
		strcat(msg, "\".");
		::MessageBox(this->GetSafeHwnd(), msg, appName, MB_OK);
	}
}

void CMainFrame::OnFileOpen()
{
	OnFileNew();
	SendEditor(SCI_CANCEL);
	SendEditor(SCI_SETUNDOCOLLECTION, 0);

	// szFilters is a text string that includes two file name filters:
	// "*.ohf, *.ohd, *.oppl, *.txt" for "OH Profile Files" and "*.*' for "All Files."
	char szFilters[] = "OH Profile (*.ohf;*.ohfd;*.oppl;*.txt)|*.ohf;*.ohfd;*.oppl;*.txt|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, "", "",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		m_strPathname = fileDlg.GetPathName();

		//Change the window's title to the opened file's title.
		SetWindowTitle();
		ReadFile();
	}
}

void CMainFrame::OnFileSave()
{
	if (m_strPathname == "") OnFileSaveas();
	else WriteFile();
}

void CMainFrame::OnFileNew()
{
	//Init the Scintilla control
	SendEditor(SCI_CLEARALL);
	SendEditor(EM_EMPTYUNDOBUFFER);
	SendEditor(SCI_SETSAVEPOINT);

	// Reset line number margin
	int margin_size = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM) "_999");
	SendEditor(SCI_SETMARGINWIDTHN, 0, margin_size);


	m_strPathname = "";

	SetWindowTitle();
}

void CMainFrame::OnFileSaveas()
{
	// szFilters is a text string that includes two file name filters:
	// "*.ohf, *.ohd, *.oppl, *.txt" for "OH Profile Files" and "*.*" for "All Files."
	char szFilters[] = "OH Profile (*.ohf;*.ohfd;*.oppl;*.txt)|*.ohf;*.ohfd;*.oppl;*.txt|All Files (*.*)|*.*||";

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(FALSE, "", m_strPathname.Mid(m_strPathname.ReverseFind('\\') + 1),
		OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, szFilters, this);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		m_strPathname = fileDlg.GetPathName();

		//Change the window's title to include the saved file's title.
		SetWindowTitle();

		WriteFile();
	}
}

void CMainFrame::SetWindowTitle()
{
	CString Title;

	if (m_strPathname == "") Title = "OpenPad - Untitled";
	else Title = "OpenPad - " + m_strPathname;
	SetWindowText(Title);
}


void CMainFrame::OnEnChangeEdit()
{
	CString s;
	m_editBox.GetWindowText(s);
	LPCSTR c = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`¬!\"£$%^&*()-_=+[]{};:'@#~\|,./<>?€";
	int j = s.FindOneOf(c);
	if (j > -1)
	{
		m_editBox.SetWindowText("");
//		m_editBox.SetFocus();
		MessageBox("Invalid line number", "Error", MB_OK);
		return;
	}	else
			{
				char * p;
				line_number = strtol(s, &p, 10);
				if (*p != 0) {
					m_editBox.SetWindowText("");
					//		m_editBox.SetFocus();
					MessageBox("Invalid line number", "Error", MB_OK);
					return;
				}
				if (line_number == 0) {
					btnOkEnabled = FALSE;
					m_buttonOK.UpdateWindow();
					return;
				}
			}

	if (m_checkBox.GetCheck())
	{
		btnOkEnabled = FALSE;
		m_buttonOK.UpdateWindow();
		line_number = line_number - 1;

		// Line scroll
		SendEditor(SCI_LINESCROLL, 0, line_number - (SendEditor(SCI_LINESONSCREEN) / 2));
		// go to line
		SendEditor(SCI_GOTOLINE, line_number);

		// select line
	//	int start_pos = SendEditor(SCI_POSITIONFROMLINE, 1150 - 1);
	//	int end_pos = SendEditor(SCI_GETLINEENDPOSITION, 1150 - 1);
	//	SendEditor(SCI_SETSEL, start_pos, end_pos);
	}

	else {
		line_number = line_number - 1;
		btnOkEnabled = TRUE;
		m_buttonOK.UpdateWindow();
	}
}

void CMainFrame::OnButtonOK()
{
	if (btnOkEnabled) {
		// Line scroll
		SendEditor(SCI_LINESCROLL, 0, line_number - (SendEditor(SCI_LINESONSCREEN) / 2));
		// go to line
		SendEditor(SCI_GOTOLINE, line_number);
	}		
}

void CMainFrame::OnUpdateButtonOK(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(btnOkEnabled);
}

void CMainFrame::OnCheckBox()
{
	if (line_number > 0) {
		if (btnOkEnabled) {
			btnOkEnabled = FALSE;
			m_buttonOK.UpdateWindow();
			// Line scroll
			SendEditor(SCI_LINESCROLL, 0, line_number - (SendEditor(SCI_LINESONSCREEN) / 2));
			// go to line
			SendEditor(SCI_GOTOLINE, line_number);
		}
		else {
			btnOkEnabled = TRUE;
			m_buttonOK.UpdateWindow();
		}
	}
}