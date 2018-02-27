
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

HDQMJSAVE CDqmjSEApp::GetSaveHandle()
{
	return theApp.m_hDqmjSave;
}

int CDqmjSEApp::CalcDigit(int number)
{
	int n = 0;
	UINT absvale = (number >= 0) ? number : -number;

	while (number != 0)
	{
		n++;
		number /= 10;
	}

	return (n == 0) ? 1 : n;
}

CString CDqmjSEApp::GetDecimalString(int number)
{
	CString str;
	str.Format(_T("%d"), number);

	return str;
}

CString CDqmjSEApp::GetNameString(DQMJ_NAME name)
{
	CStringW str;

	UINT len = ::DQMJSaveNameToString(NULL, UINT_MAX, name, L'?');
	VERIFY(::DQMJSaveNameToString(str.GetBuffer(len), len, name, L'?') == len);
	str.ReleaseBuffer();

	return str;
}

//////////////////////////////////////////////////////////////////////////
