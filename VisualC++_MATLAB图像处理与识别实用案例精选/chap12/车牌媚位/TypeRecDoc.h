 // TypeRecDoc.h : interface of the CTypeRecDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TYPERECDOC_H__0BE554A7_D13B_4F68_8F6A_D2FA6AA37EF7__INCLUDED_)
#define AFX_TYPERECDOC_H__0BE554A7_D13B_4F68_8F6A_D2FA6AA37EF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dibapi.h"
#include "edgecontour.h"
#include "TemplateTrans.h"
#include "suanfa1.h"

class CTypeRecDoc : public CDocument
{
protected: // create from serialization only
	CTypeRecDoc();
	DECLARE_DYNCREATE(CTypeRecDoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeRecDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReplaceHDIB(HDIB hDIB);
	COLORREF m_refColorBKG;
	void InitDIBData();
	virtual ~CTypeRecDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
	//{{AFX_MSG(CTypeRecDoc)
	afx_msg void OnFileReopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPERECDOC_H__0BE554A7_D13B_4F68_8F6A_D2FA6AA37EF7__INCLUDED_)
