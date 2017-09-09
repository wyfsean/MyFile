// TypeRec.h : main header file for the TYPEREC application
//

#if !defined(AFX_TYPEREC_H__4C97764C_FFAB_466D_8C38_11A21F2CB7D8__INCLUDED_)
#define AFX_TYPEREC_H__4C97764C_FFAB_466D_8C38_11A21F2CB7D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTypeRecApp:
// See TypeRec.cpp for the implementation of this class
//

class CTypeRecApp : public CWinApp
{
public:
	CTypeRecApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeRecApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTypeRecApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPEREC_H__4C97764C_FFAB_466D_8C38_11A21F2CB7D8__INCLUDED_)
