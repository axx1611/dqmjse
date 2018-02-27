
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterListDlg)

public:

	CMonsterListDlg(BOOL interrupt, LPCTSTR master_name, CWnd *pParent = NULL);

protected:

	int						m_nLevel;
	int						m_nCombine;
	int						m_nBaseHp;
	int						m_nBaseMp;
	int						m_nBaseAtk;
	int						m_nBaseDef;
	int						m_nBaseAgi;
	int						m_nBaseInt;
	int						m_nAtk;
	int						m_nDef;
	int						m_nAgi;
	int						m_nInt;
	int						m_nCurHp;
	int						m_nCurMp;
	int						m_nMaxLevel;
	int						m_nMaxHp;
	int						m_nMaxMp;
	int						m_nExp;
	int						m_nSkillPoint;
	int						m_nGrowthLevel[2];
	int						m_nSkillsetPt[DQMJ_SKILLSET_MAX];
	BOOL					m_bInterrupt;
	BOOL					m_bSelectChanging;
	BOOL					m_normalizeFlags[DQMJ_MONSTER_MAX];
	CString					m_strName;
	CString					m_strMaster;
	CString					m_strParentsName[2];
	CString					m_strParentsMaster[2];
	CListCtrl				m_lstMonster;
	CListBox				m_lstSkill;
	CComboBox				m_cmbSex;
	CComboBox				m_cmbRace;
	CComboBox				m_cmbRank;
	CComboBox				m_cmbType;
	CComboBox				m_cmbStrategy;
	CComboBox				m_cmbSource;
	CComboBox				m_cmbMaxLevel;
	CComboBox				m_cmbGrowthType;
	CComboBox				m_cmbSkillset[DQMJ_SKILLSET_MAX];
	CComboBox				m_cmbAbility[DQMJ_ABILITY_MAX];
	CComboBox				m_cmbGuard[DQMJ_GUARD_MAX];
	CComboBox				m_cmbParentsRace[2];
	CComboBox				m_cmbGrandparentsRace[2][2];
	DQMJ_NAME				m_masterName;
	DQMJ_SAVE_MONSTER_INFO	m_editingInfo[DQMJ_MONSTER_MAX];
	DQMJ_SAVE_MONSTER_INFO	m_monsterInfo[DQMJ_MONSTER_MAX];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonPaste();
	afx_msg void OnBnClickedCheckNormalize();
	afx_msg void OnBnClickedCheckModeLock();
	afx_msg void OnBnClickedCheckRankAuto();
	afx_msg void OnBnClickedCheckTypeAuto();
	afx_msg void OnBnClickedCheckDead();
	afx_msg void OnBnClickedCheckGrowthTypeRandom();
	afx_msg void OnBnClickedCheckGrowthLevelRandom();
	afx_msg void OnBnClickedButtonMonsterUp();
	afx_msg void OnBnClickedButtonMonsterTop();
	afx_msg void OnBnClickedButtonMonsterDown();
	afx_msg void OnBnClickedButtonMonsterBottom();
	afx_msg void OnBnClickedButtonMonsterAdd();
	afx_msg void OnBnClickedButtonMonsterRemove();
	afx_msg void OnBnClickedButtonStatusMax();
	afx_msg void OnBnClickedButtonStatusMaxForce();
	afx_msg void OnBnClickedButtonWeaponModify();
	afx_msg void OnBnClickedButtonWeaponRemove();
	afx_msg void OnBnClickedButtonSkillsetMax1();
	afx_msg void OnBnClickedButtonSkillsetMax2();
	afx_msg void OnBnClickedButtonSkillsetMax3();
	afx_msg void OnBnClickedButtonSkillUp();
	afx_msg void OnBnClickedButtonSkillTop();
	afx_msg void OnBnClickedButtonSkillDown();
	afx_msg void OnBnClickedButtonSkillBottom();
	afx_msg void OnBnClickedButtonSkillAdd();
	afx_msg void OnBnClickedButtonSkillModify();
	afx_msg void OnBnClickedButtonSkillRemove();
	afx_msg void OnBnClickedButtonSkillSort();
	afx_msg void OnLbnSelchangeListSkillList();
	afx_msg void OnLvnItemchangingListMonsterList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:

	enum COLUMN
	{
		COLUMN_NO,
		COLUMN_NAME,
		COLUMN_PLACE,
		COLUMN_SEX,
		COLUMN_RACE,
		COLUMN_RANK,
		COLUMN_TYPE,
	};

	struct IDStrPair
	{
		int		id;
		LPCTSTR	str;
	};

	typedef CList<IDStrPair>	CIDStrList;

	int GetCurSel();
	void RefreshMonster(int sel);
	void RefreshMonsterSel(int sel);
	void RefreshMonsterInfo(DQMJ_SAVE_MONSTER_INFO &monster, BOOL normalize);
	void RefreshSkillSel(int sel, BOOL disable = FALSE);

	static const int MAXLEVEL_LIST[];
	static const LPCTSTR SEX_LIST[DQMJ_SEX_NUM];
	static const LPCTSTR RANK_LIST[DQMJ_RANK_NUM];
	static const LPCTSTR TYPE_LIST[DQMJ_TYPE_NUM];
	static const LPCTSTR STRATEGY_LIST[DQMJ_STRATEGY_NUM];
	static const LPCTSTR SOURCE_LIST[DQMJ_SOURCE_NUM];
	static const LPCTSTR GROWTH_LIST[DQMJ_GROWTH_NUM];
	static const LPCTSTR GUARD_LIST[DQMJ_GUARD_NUM];

	static CIDStrList m_raceIDStrList;
	static CIDStrList m_skillsetIDStrList;
	static CIDStrList m_abilityIDStrList;
	static int m_raceToCBIndex[DQMJ_MONSTERLIST_LEN];
	static int m_skillsetToCBIndex[DQMJ_SKILLSETLIST_LEN];
	static int m_abilityToCBIndex[DQMJ_ABILITYLIST_LEN];
	static DQMJ_SAVE_MONSTER_INFO m_emptyMonster;

	static void StaticInitialize();
	static void InitCBIndex();
	static void InitComboBox(CComboBox &combo, const int *list, int len);
	static void InitComboBox(CComboBox &combo, const LPCTSTR *list, int len);
	static void InitComboBox(CComboBox &combo, const CIDStrList &list);
	static int GetSafeCBIndex(int idx, int len);
	static int GetSafeCBIndex(int id, const int *list, int len);
	static LPCTSTR GetSafeCBString(int idx, const LPCTSTR *list, int len);
	static LPCTSTR GetRaceString(int race);
	static LPCTSTR GetEquipmentNameString(int equipment);
	static CString GetMonsterNumString(int num);
	static CString GetSkillNumString(int num);
	static BOOL IsInPartyOrStandby(int monster_idx);

};

//////////////////////////////////////////////////////////////////////////
