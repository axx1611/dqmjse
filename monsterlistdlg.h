
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterListDlg)

public:

	explicit CMonsterListDlg(CWnd *pParent = NULL);

	int GetSelMonster() const { return m_nSelMonster; }
	int GetPartyIndex() const { return m_nPartyIndex; }
	int GetStandbyIndex() const { return m_nStandbyIndex; }
	void ConfigSelectMode(int monster_idx, BOOL disable_party_member, BOOL disable_standby);

protected:

	CListCtrl	m_lstMonster;

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnBnClickedCheckGameOrder();
	afx_msg void OnBnClickedButtonMonsterUp();
	afx_msg void OnBnClickedButtonMonsterDown();
	afx_msg void OnBnClickedButtonMonsterTop();
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

	struct MONSTER_INDEXES
	{
		int				monster_idx:16;
		int				party_member_idx:8;
		int				standby_idx:8;
	};

	union ITEM_DATA
	{
		DWORD_PTR		data;
		MONSTER_INDEXES	indexes;
	};

	BOOL				m_bSelectMode;
	BOOL				m_bDisablePartyMember;
	BOOL				m_bDisableStandby;
	int					m_nSelMonster;
	int					m_nPartyIndex;
	int					m_nStandbyIndex;

	int GetCurSel();
	BOOL CanSelect(int sel);
	int CheckMonsterInParty(int monster_idx, const DQMJ_SAVE_RANCH_INFO &ranchinfo);
	int CheckMonsterStandby(int monster_idx, const DQMJ_SAVE_RANCH_INFO &ranchinfo);
	MONSTER_INDEXES GetMonsterIndexes(int idx);
	void SetMonsterIndexes(int idx, int monster_idx, int party_member_idx, int standby_idx);
	void UpdateMonsterList();
	void RedrawMonsterList(int sel);
	void RefreshMonsterSel(int sel);
	void InsertMonsterItem(int monster_idx, const DQMJ_SAVE_RANCH_INFO &ranchinfo);
	void AddMonster();
	void ModifyMonster(int sel);
	void RemoveMonster(int sel);
	void CopyMonster(int sel);
	void MoveMonster(int sel, int idx);

};

//////////////////////////////////////////////////////////////////////////
