
#pragma once

//////////////////////////////////////////////////////////////////////////

class CSelectWeaponDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectWeaponDlg)

public:

	explicit CSelectWeaponDlg(const DQMJ_SAVE_MONSTER_INFO &monsterinfo, BOOL normalize, CWnd *pParent = NULL);

	int GetSelWeapon() const { return m_nWeapon; }

protected:

	CListCtrl	m_lstWeapon;

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnNMDblclkListWeaponList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListWeaponList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:

	enum COLUMN
	{
		COLUMN_NAME,
		COLUMN_TYPE,
		COLUMN_OWNER,
	};

	BOOL		m_bNormalize;
	int			m_nWeapon;
	int			m_nRace;
	const int*	m_abilityList;

	int GetCurSel();
	LPCTSTR GetWeaponTypeString(int type);

};

//////////////////////////////////////////////////////////////////////////
