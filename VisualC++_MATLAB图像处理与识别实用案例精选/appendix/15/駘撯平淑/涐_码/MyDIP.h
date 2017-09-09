// MyDIP.h : main header file for the MYDIP application
//

#if !defined(AFX_MYDIP_H__CCB27B8B_4ABF_4C6F_8CC6_EF382EE0C903__INCLUDED_)
#define AFX_MYDIP_H__CCB27B8B_4ABF_4C6F_8CC6_EF382EE0C903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyDIPApp:
// See MyDIP.cpp for the implementation of this class
//

class CMyDIPApp : public CWinApp
{
public:
	CMyDIPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDIPApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyDIPApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIP_H__CCB27B8B_4ABF_4C6F_8CC6_EF382EE0C903__INCLUDED_)
