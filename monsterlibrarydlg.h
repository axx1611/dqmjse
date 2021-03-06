
#pragma once

#include "monsterpaneldlg.h"

//////////////////////////////////////////////////////////////////////////

class CMonsterLibraryDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterLibraryDlg)

public:

	explicit CMonsterLibraryDlg(CWnd *pParent = NULL);

	void Update();

protected:

	CComboBox			m_cmbFilter;
	CMonsterPanelDlg	m_dlgPanel;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonCheckall();
	afx_msg void OnCbnSelchangeComboMonsterFilter();

	DECLARE_MESSAGE_MAP()

private:

	void RefreshCollectNum();

};

//////////////////////////////////////////////////////////////////////////
