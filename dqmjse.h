
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "dqmjsav.h"

//////////////////////////////////////////////////////////////////////////

class CDqmjSEApp : public CWinAppEx
{

public:

	CDqmjSEApp();

	HDQMJSAVE GetSaveHandle() const;
	BOOL OpenSave(LPCTSTR pszFilename);
	void CloseSave();

	virtual BOOL InitInstance();

private:

	HDQMJSAVE	m_hDqmjSave;

};

//////////////////////////////////////////////////////////////////////////
