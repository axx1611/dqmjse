
#pragma once

//////////////////////////////////////////////////////////////////////////

class CSelectSkillDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectSkillDlg)

public:

	explicit CSelectSkillDlg(int skill = 0x00, CWnd *pParent = NULL);

	int GetSelSkill() const { return m_nSelSkill; }

protected:

	CListBox	m_lstSkill;

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnLbnSelchangeListItemList();
	afx_msg void OnLbnDblclkListItemList();

	DECLARE_MESSAGE_MAP()

private:

	int			m_nSelSkill;

	void RefreshEnable();

};

//////////////////////////////////////////////////////////////////////////
