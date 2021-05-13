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

// OpenPadView.h : interface of the COpenPadView class
//

#pragma once


#include "ScintillaControl.h"

#define STR_SCINTILLAWND _T("Scintilla")


class COpenPadView : public CView
{
protected: // create from serialization only
	COpenPadView() noexcept;

	void COpenPadView::DoDataExchange(CDataExchange* pDX);
	// Process notification from scintilla control
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	DECLARE_DYNCREATE(COpenPadView)

// Attributes
public:
	COpenPadDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~COpenPadView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	CSciCtrl*	p_viewSciCtrl;
	CStatic*	m_gotoLabel;
	CStatic*	m_findLabel;
	CEdit*		m_gotoEdit;
	CEdit*		m_findEdit;
	CButton*	m_gotoOK;
	CButton*	m_autoCheckBox;
	CButton*	m_findNext;
	CButton*	m_findPrev;
	BOOL		gotoBtnEnabled;
	BOOL		findNextBtnEnabled;
	BOOL		findPrevBtnEnabled;

	int line_number = 0;
	CString text_to_find = "";

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
//	afx_msg void OnFilePrint();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditAll();
	afx_msg void OnEnChangeGotoEdit();
	afx_msg void OnGotoOK();
	afx_msg void OnUpdateGotoOK(CCmdUI* pCmdUI);
	afx_msg void OnAutoCheckBox();
	afx_msg void OnEnChangeFindEdit();
	afx_msg void OnFindNext();
	afx_msg void OnUpdateFindNext(CCmdUI* pCmdUI);
	afx_msg void OnFindPrev();
	afx_msg void OnUpdateFindPrev(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAddmarker();
	afx_msg void OnUpdateOptionsAddmarker(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDeletemarker();
	afx_msg void OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI);
	afx_msg void OnOptionsFindNextmarker();
	afx_msg void OnOptionsFindPrevmarker();
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in OpenPadView.cpp
inline COpenPadDoc* COpenPadView::GetDocument() const
   { return reinterpret_cast<COpenPadDoc*>(m_pDocument); }
#endif

