// MyDIPDoc.h : interface of the CMyDIPDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDIPDOC_H__EAAB6882_287D_41DA_81A4_1E2D93804886__INCLUDED_)
#define AFX_MYDIPDOC_H__EAAB6882_287D_41DA_81A4_1E2D93804886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyDIPDoc : public CDocument
{
protected: // create from serialization only
	CMyDIPDoc();
	DECLARE_DYNCREATE(CMyDIPDoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }
	CSize m_sizeDoc;
	HDIB m_hDIB;
	CPalette* m_palDIB;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDIPDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyDIPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyDIPDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL OnSaveDocument(LPCTSTR lpszPathName);
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	BOOL InitDIBData();
	BOOL ReadImgFile(CString sName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIPDOC_H__EAAB6882_287D_41DA_81A4_1E2D93804886__INCLUDED_)
