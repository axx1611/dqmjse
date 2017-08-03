
#include "stdafx.h"
#include "dqmjse.h"
#include "dqmjsedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

static CDqmjSEApp theApp;

//////////////////////////////////////////////////////////////////////////

CDqmjSEApp::CDqmjSEApp()
{
	m_hDqmjSave = NULL;
}

//////////////////////////////////////////////////////////////////////////

HDQMJSAVE CDqmjSEApp::GetSaveHandle() const
{
	return m_hDqmjSave;
}

BOOL CDqmjSEApp::OpenSave(LPCTSTR pszFilename)
{
	if (m_hDqmjSave != NULL)
		return FALSE;

	m_hDqmjSave = ::DQMJSaveOpenFile(pszFilename);
	if (m_hDqmjSave == NULL)
		return FALSE;

	return TRUE;
}

void CDqmjSEApp::CloseSave()
{
	::DQMJSaveCloseFile(m_hDqmjSave);
	m_hDqmjSave = NULL;
}

BOOL CDqmjSEApp::InitInstance()
{
	INITCOMMONCONTROLSEX initctrls;
	initctrls.dwSize = sizeof(initctrls);
	initctrls.dwICC = ICC_WIN95_CLASSES;
	::InitCommonControlsEx(&initctrls);

	CWinAppEx::InitInstance();

	CDqmjSEDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
