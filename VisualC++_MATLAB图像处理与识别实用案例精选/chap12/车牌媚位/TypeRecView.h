// TypeRecView.h : interface of the CTypeRecView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYPERECVIEW_H__CEDA43C9_F712_492D_8032_DA4EBBB299AB__INCLUDED_)
#define AFX_TYPERECVIEW_H__CEDA43C9_F712_492D_8032_DA4EBBB299AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTypeRecView : public CView
{
protected: // create from serialization only
	CTypeRecView();
	DECLARE_DYNCREATE(CTypeRecView)

// Attributes
public:
	CTypeRecDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeRecView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_ipzLeft,m_ipzRight,m_ipzBottom,m_ipzTop;
	void Search_Directory(char *szFilename);
	virtual ~CTypeRecView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTypeRecView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFILE256ToGray();
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam); 
	afx_msg void OnFILE24ToGray();
	afx_msg void OnTempSubrect();
	afx_msg void OnEditCopy();
	afx_msg void OnTempPalette();
	afx_msg void OnTempGray();
	afx_msg void OnTempError();
	afx_msg void OnTest11();
	afx_msg void OnTest12();
	afx_msg void OnTest13();
	afx_msg void OnTest14();
	afx_msg void OnTest15();
	afx_msg void OnEditPaste();
	afx_msg void OnTest145();
	afx_msg void OnTest16();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TypeRecView.cpp
inline CTypeRecDoc* CTypeRecView::GetDocument()
   { return (CTypeRecDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPERECVIEW_H__CEDA43C9_F712_492D_8032_DA4EBBB299AB__INCLUDED_)
