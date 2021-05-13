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

// OpenPadDoc.cpp : implementation of the COpenPadDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenPad.h"
#endif

#include "OpenPadDoc.h"
#include "OpenPadView.h"

#include "Scintilla.h"
#include "SciLexer.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// COpenPadDoc

IMPLEMENT_DYNCREATE(COpenPadDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenPadDoc, CDocument)
END_MESSAGE_MAP()


// COpenPadDoc construction/destruction

COpenPadDoc::COpenPadDoc() noexcept
{
	// TODO: add one-time construction code here

}

COpenPadDoc::~COpenPadDoc()
{
}


BOOL COpenPadDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	p_docSciCtrl->ClearAll();
//	SetModifiedFlag(FALSE);
	return TRUE;
}

BOOL COpenPadDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	p_docSciCtrl->SizeMargin();
//	SetModifiedFlag(FALSE);
	return TRUE;
}


// COpenPadDoc serialization

void COpenPadDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		char *textbuffer = p_docSciCtrl->GetBuffer();
		ar.Write(textbuffer, strlen(textbuffer));
	}
	else
	{
		// TODO: add loading code here
		p_docSciCtrl->ClearAll();

		char buff[1000000];
		while (1) {
			UINT nr = ar.Read(buff, 1000000);
			if (nr < 1000000) *(buff + nr) = '\0';
			p_docSciCtrl->AddText(buff);
			if (nr < 1000000) break;
		}

		// Set up a caret slop for ideal target scrolling on middle displaying lines on screen
		p_docSciCtrl->SendEditor(SCI_SETYCARETPOLICY, CARET_SLOP, p_docSciCtrl->SendEditor(SCI_LINESONSCREEN) / 2);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void COpenPadDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void COpenPadDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COpenPadDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COpenPadDoc diagnostics

#ifdef _DEBUG
void COpenPadDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenPadDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COpenPadDoc commands
