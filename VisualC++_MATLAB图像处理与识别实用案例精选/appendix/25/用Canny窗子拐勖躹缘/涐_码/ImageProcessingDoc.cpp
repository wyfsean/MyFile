// ImageProcessingDoc.cpp : implementation of the CImageProcessingDoc class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "GlobalApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc

IMPLEMENT_DYNCREATE(CImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessingDoc, CDocument)
	//{{AFX_MSG_MAP(CImageProcessingDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_RELOAD, OnFileReload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc construction/destruction

CImageProcessingDoc::CImageProcessingDoc()
{
	// TODO: add one-time construction code here
	InitDocVariable();
}

CImageProcessingDoc::~CImageProcessingDoc()
{
	FreeDocVariable();
}

BOOL CImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc serialization

void CImageProcessingDoc::Serialize(CArchive& ar)
{
	m_pDibInit->Serialize(ar);

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc diagnostics

#ifdef _DEBUG
void CImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc commands

int CImageProcessingDoc::InitDocVariable()
{
	m_pDibInit = new CDib;

	// added by tanhc in 2002-7-30, used to test some function
	m_pDibTest = new CDib;

	m_nColorIndex = 0;

	//--------------------------------------------

	return 0;
}

int CImageProcessingDoc::FreeDocVariable()
{
	delete m_pDibInit;
	m_pDibInit = NULL;

	// added by tanhc in 2002-7-30, used to test some function
	delete m_pDibTest ;
	m_pDibTest = NULL ;

	//--------------------------------------------

	return 0;
}

void CImageProcessingDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CString strSaveFileType = "λͼ�ļ� (*.bmp;*.dib)|*.bmp; *.dib|All Files (*.*)|*.*||";
	CFileDialog FileDlg(FALSE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strSaveFileType);

	CFile fileOpen ;
	if( FileDlg.DoModal() == IDOK ) {
		if(!fileOpen.Open( FileDlg.GetPathName() , CFile::modeCreate|CFile::modeWrite )){
			AfxMessageBox("cannot create the file to save");
			return;
		}
		if( !m_pDibInit->Write( &fileOpen ) ){
			return;
		}
		fileOpen.Close();
	}
	
}

/*************************************************************************
 *
 * \�������ƣ�
 *   OnFileReload()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú�����ӦID_FILE_RELOAD�������Ϣ��������DIB
 *
 *************************************************************************
 */
void CImageProcessingDoc::OnFileReload() 
{
	// �жϵ�ǰͼ���Ƿ��Ѿ����Ķ�
	if (IsModified())
	{
		// ��ʾ�û��ò�������ʧ���е�ǰ���޸�
		if (MessageBox(NULL, "���´�ͼ�񽫶�ʧ���иĶ����Ƿ������", "ϵͳ��ʾ", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			// �û�ȡ��������ֱ�ӷ���
			return;
		}
	}
	CFile file;

	CString strPathName;
	CFileException fe;

	strPathName = GetPathName();

	// ���´��ļ�
	if( !file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ʧ��
		ReportSaveLoadException(strPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		// ����
		return;
	}

	// ���Ĺ��
	BeginWaitCursor();

	// ���¶�ȡͼ��
	if(!m_pDibInit->Read(&file)){
		// �ָ������״
 		EndWaitCursor();
 				
 		 		
 		// ����
 		return;
	}

	// ��ʼ���ͱ��ΪFALSE
 	SetModifiedFlag(FALSE);
 	
 	// ˢ��
 	UpdateAllViews(NULL);
  	
 	// �ָ������״
 	EndWaitCursor();
	
 	// ����
 	return;
}
