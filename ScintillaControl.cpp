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

// ScintillaControl.cpp : implementation of the CSciCtrl class
//

#include "stdafx.h"

#include "ScintillaControl.h"
#include "Scintilla.h"
#include "SciLexer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR* g_opplKeyWords =
_T("and and_eq asm auto bitand bitor bool break ")
_T("case catch char class compl const const_cast continue ")
_T("default delete do double dynamic_cast else enum explicit export extern false float for ")
_T("friend goto if inline int long mutable namespace new not not_eq ")
_T("operator or or_eq private protected public ")
_T("register reinterpret_cast return short signed sizeof static static_cast struct switch ")
_T("template this throw true try typedef typeid typename union unsigned using ")
_T("virtual void volatile wchar_t while xor xor_eq ");  // to be adapted for oppl language


// CSciCtrl

IMPLEMENT_DYNAMIC(CSciCtrl, CWnd)

BEGIN_MESSAGE_MAP(CSciCtrl, CWnd)

END_MESSAGE_MAP()



void CSciCtrl::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	//	DDX_Control(pDX, ID_GOTOEDIT, m_gotoEdit);
	//	DDX_Control(pDX, ID_GOTOOK, m_gotoOK);
	//	DDX_Control(pDX, ID_AUTO_CHECKBOX, m_autoCheckBox);
	//	DDX_Control(pDX, ID_FINDNEXT, m_findNext);
}


// CSciCtrl construction/destruction

CSciCtrl::CSciCtrl()
{
	HINSTANCE hmod = LoadLibrary("SciLexer.DLL");
	if (hmod==NULL)
	{
		MessageBox( 
		"The Scintilla DLL could not be loaded.", 
		"Error loading Scintilla", 
		MB_OK | MB_ICONERROR);
	} 
}

CSciCtrl::~CSciCtrl()
{
}


void CSciCtrl::Init()
{
	// Global set up of Scintilla control /////
	
	// Set up line number margin
	SizeMargin();

	// Set up caret line highlighting
	SendMessage(SCI_SETCARETLINEVISIBLE, TRUE);
	SendMessage(SCI_SETCARETLINEVISIBLEALWAYS, TRUE);
	//	SendMessage(SCI_SETCARETLINEBACK, RGB(204, 204, 255));
	//	SendMessage(SCI_SETCARETLINEBACKALPHA, 99);

	// Set up markers
	SendMessage(SCI_MARKERDEFINE, 0, SC_MARK_BOOKMARK);
	SendMessage(SCI_MARKERSETFORE, 0, RGB(255, 0, 0));
	SendMessage(SCI_MARKERSETBACK, 0, RGB(0, 0, 139));

	// Set up the Lexer
	SendMessage(SCI_SETLEXER, SCLEX_CPP);
	SendMessage(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(g_opplKeyWords));

	// Set up the global default style. These attributes are used wherever no explicit choices are made.
	SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 12, "Lucida Sans");
	SendMessage(SCI_STYLESETWEIGHT, STYLE_DEFAULT, SC_WEIGHT_SEMIBOLD);
	SendMessage(SCI_STYLECLEARALL);	// Copies global style to all others
	SendMessage(SCI_STYLESETWEIGHT, STYLE_LINENUMBER, SC_WEIGHT_NORMAL);
	SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
	SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
	SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
	SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
	SendMessage(SCI_STYLESETBOLD, SCE_C_WORD, TRUE);
	SetAStyle(SCE_C_STRING, RGB(0x80, 0x80, 0));  // override 0x80, 0, 0x80
	SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
	SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
	SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0, 0x80));  // override 0x80, 0x80, 0

	// Set up event mask
//	SendMessage(SCI_SETMODEVENTMASK, SCN_UPDATEUI);  // For future purpose of future version of Scintilla perhaps
													// to bold margin number of caret line
													// for the moment SCI_MARGINSETSTYLE is only allowed for text margin

//	SendMessage(SCI_SETMODEVENTMASK, SCN_MODIFIED);

	// End of set up of Scintilla control /////

}

void CSciCtrl::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face)
{
	SendMessage(SCI_STYLESETFORE, style, fore);
	SendMessage(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendMessage(SCI_STYLESETSIZE, style, size);
	if (face)
		SendMessage(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

void CSciCtrl::SizeMargin()
{
	// Set up margin number width /////

	// get width of character '9' in pixels
	int margin_size = SendMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, (long)_T("9"));
	// try to get number of lines in control
	LRESULT lLines = SendMessage(SCI_GETLINECOUNT);

	int nChars = 1;
	while (lLines > 0)
	{
		lLines = lLines / 10;
		nChars++;
	}
	nChars++; // allow an extra char for safety
	SendMessage(SCI_SETMARGINWIDTHN, 0, nChars * margin_size);

	// End Set up of margin number width /////
}

void CSciCtrl::ClearAll()
{
	//Init the Scintilla control
	SendMessage(SCI_CLEARALL);
	SendMessage(EM_EMPTYUNDOBUFFER);
	SendMessage(SCI_SETSAVEPOINT);
	SizeMargin();
}

char * CSciCtrl::GetBuffer()
{
	int buflen = SendMessage(SCI_GETLENGTH) + 1;
	char *textbuffer = new char[buflen];
	SendMessage(SCI_GETTEXT, buflen, (long)textbuffer);
	return textbuffer;
}

void CSciCtrl::SetText(const char * text)
{
	if (text != NULL)
		SendMessage(SCI_SETTEXT, 0, (long)text);
}

void CSciCtrl::AddText(const char * txt)
{
	if (txt!=NULL)
		SendMessage(SCI_ADDTEXT,strlen(txt),(long)txt);
}


