
#pragma once

#include "skillpaneldlg.h"

//////////////////////////////////////////////////////////////////////////

class CSkillLibraryDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkillLibraryDlg)

public:

	explicit CSkillLibraryDlg(CWnd *pParent = NULL);

	void Update();

protected:

	CComboBox			m_cmbFilter;
	CSkillPanelDlg		m_dlgPanel;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonCheckall();
	afx_msg void OnCbnSelchangeComboSkillFilter();

	DECLARE_MESSAGE_MAP()

private:

	void RefreshCollectNum();

};

//////////////////////////////////////////////////////////////////////////
