
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterListDlg)

public:

	explicit CMonsterListDlg(BOOL select_mode, CWnd *pParent = NULL);

	int GetSelMonster() const { return m_nSelMonster; }
	int GetPartyIndex() const { return m_nPartyIndex; }
	int GetStandbyIndex() const { return m_nStandbyIndex; }
	void ConfigSelectMode(int sel, BOOL disable_party_member, BOOL disable_standby);

protected:

	CListCtrl	m_lstMonster;

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnBnClickedButtonMonsterUp();
	afx_msg void OnBnClickedButtonMonsterTop();
	afx_msg void OnBnClickedButtonMonsterDown();
	afx_msg void OnBnClickedButtonMonsterBottom();
	afx_msg void OnBnClickedButtonMonsterAdd();
	afx_msg void OnBnClickedButtonMonsterModify();
	afx_msg void OnBnClickedButtonMonsterRemove();
	afx_msg void OnBnClickedButtonMonsterCopy();
	afx_msg void OnNMDblclkListMonsterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangingListMonsterList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:

	enum COLUMN
	{
		COLUMN_NO,
		COLUMN_PARTY,
		COLUMN_STANDBY,
		COLUMN_NAME,
		COLUMN_SEX,
		COLUMN_RACE,
		COLUMN_LEVEL,
		COLUMN_RANK,
		COLUMN_TYPE,
		COLUMN_WEAPON,
	};

	BOOL		m_bSelectMode;
	BOOL		m_bDisablePartyMember;
	BOOL		m_bDisableStandby;
	int			m_nSelMonster;
	int			m_nPartyIndex;
	int			m_nStandbyIndex;

	int GetCurSel();
	BOOL CanSelect(int sel);
	void UpdateMonsterList();
	void RedrawMonsterList(int sel);
	void RefreshMonsterSel(int sel);
	void AddMonster();
	void ModifyMonster(int sel);
	void RemoveMonster(int sel);
	void CopyMonster(int sel);
	void MoveMonster(int sel, int idx);

};

//////////////////////////////////////////////////////////////////////////
