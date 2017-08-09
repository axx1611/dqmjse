
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterListDlg)

public:

	CMonsterListDlg(CWnd *pParent = NULL);

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
	BOOL					m_initializeFlags[DQMJ_MONSTER_MAX];
	BOOL					m_normalizeFlags[DQMJ_MONSTER_MAX];
	CString					m_strName;
	CString					m_strMaster;
	CString					m_strFartherName;
	CString					m_strFartherMaster;
	CString					m_strMotherName;
	CString					m_strMotherMaster;
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
	CComboBox				m_cmbGuard[DQMJ_GUARD_NUM];
	CComboBox				m_cmbFartherRace[3];
	CComboBox				m_cmbMotherRace[3];
	DQMJ_NAME				m_masterName;
	DQMJ_SAVE_MONSTER_INFO	m_monsterInfo[DQMJ_MONSTER_MAX];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedCheckNormalize();
	afx_msg void OnBnClickedButtonMonsterUp();
	afx_msg void OnBnClickedButtonMonsterTop();
	afx_msg void OnBnClickedButtonMonsterDown();
	afx_msg void OnBnClickedButtonMonsterBottom();
	afx_msg void OnBnClickedButtonMonsterAdd();
	afx_msg void OnBnClickedButtonMonsterRemove();
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

	void RefreshEnable(int cur_sel);
	void RefreshMonster(DQMJ_SAVE_MONSTER_INFO &monster);
	void EnableAll(BOOL enable);
	void NormalizeUpdated(BOOL enable);

	static void InitRaceComboBox(CComboBox &combo);
	static void InitSkillsetComboBox(CComboBox &combo);
	static void InitAbilityComboBox(CComboBox &combo);
	static CString GetMonsterNumString(int num);
	static CString GetSkillNumString(int num);
	static LPCTSTR GetSexString(int sex);
	static LPCTSTR GetRaceString(int race);
	static LPCTSTR GetRankString(int rank);
	static LPCTSTR GetTypeString(int type);
	static LPCTSTR GetEquipmentNameString(int equipment);

};

//////////////////////////////////////////////////////////////////////////
