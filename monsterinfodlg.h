
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterInfoDlg)

public:

	explicit CMonsterInfoDlg(const DQMJ_SAVE_MONSTER_INFO &monsterinfo, CWnd *pParent = NULL);

	const DQMJ_SAVE_MONSTER_INFO &GetMonsterInfo() const;

protected:

	int			m_nLevel;
	int			m_nCombine;
	int			m_nBaseHp;
	int			m_nBaseMp;
	int			m_nBaseAtk;
	int			m_nBaseDef;
	int			m_nBaseAgi;
	int			m_nBaseInt;
	int			m_nAtk;
	int			m_nDef;
	int			m_nAgi;
	int			m_nInt;
	int			m_nCurHp;
	int			m_nCurMp;
	int			m_nMaxLevel;
	int			m_nMaxHp;
	int			m_nMaxMp;
	int			m_nExp;
	int			m_nSkillPoint;
	int			m_nGrowthStartLevel;
	int			m_nGrowthEndLevel;
	int			m_nSkillsetPt[DQMJ_SKILLSET_MAX];
	CString		m_strName;
	CString		m_strMaster;
	CString		m_strParentName[DQMJ_PARENT_NUM];
	CString		m_strParentMaster[DQMJ_PARENT_NUM];
	CListCtrl	m_lstMonster;
	CListBox	m_lstSkill;
	CComboBox	m_cmbSex;
	CComboBox	m_cmbRace;
	CComboBox	m_cmbRank;
	CComboBox	m_cmbType;
	CComboBox	m_cmbStrategy;
	CComboBox	m_cmbSource;
	CComboBox	m_cmbMaxLevel;
	CComboBox	m_cmbGrowthType;
	CComboBox	m_cmbSkillset[DQMJ_SKILLSET_MAX];
	CComboBox	m_cmbAbility[DQMJ_ABILITY_MAX];
	CComboBox	m_cmbGuard[DQMJ_GUARD_MAX];
	CComboBox	m_cmbParentRace[DQMJ_PARENT_NUM];
	CComboBox	m_cmbGrandparentRace[DQMJ_PARENT_NUM][DQMJ_PARENT_NUM];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnBnClickedCheckNormalize();
	afx_msg void OnBnClickedCheckRankAuto();
	afx_msg void OnBnClickedCheckTypeAuto();
	afx_msg void OnBnClickedCheckDead();
	afx_msg void OnBnClickedCheckSkillsetNone1() { CheckSkillsetNone(0); }
	afx_msg void OnBnClickedCheckSkillsetNone2() { CheckSkillsetNone(1); }
	afx_msg void OnBnClickedCheckSkillsetNone3() { CheckSkillsetNone(2); }
	afx_msg void OnBnClickedCheckFatherNone() { CheckParentNone(DQMJ_PARENT_FATHER); }
	afx_msg void OnBnClickedCheckMotherNone() { CheckParentNone(DQMJ_PARENT_MOTHER); }
	afx_msg void OnBnClickedCheckFatherFatherNone() { CheckGrandparentNone(DQMJ_PARENT_FATHER, DQMJ_PARENT_FATHER); }
	afx_msg void OnBnClickedCheckFatherMotherNone() { CheckGrandparentNone(DQMJ_PARENT_FATHER, DQMJ_PARENT_MOTHER); }
	afx_msg void OnBnClickedCheckMotherFatherNone() { CheckGrandparentNone(DQMJ_PARENT_MOTHER, DQMJ_PARENT_FATHER); }
	afx_msg void OnBnClickedCheckMotherMotherNone() { CheckGrandparentNone(DQMJ_PARENT_MOTHER, DQMJ_PARENT_MOTHER); }
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
	afx_msg void OnBnClickedButtonGrowthRandom();
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
	afx_msg void OnBnClickedButtonMasterMe() { SetMasterName(m_masterName); }
	afx_msg void OnBnClickedButtonMasterUnknown() { SetMasterName(NULL); }
	afx_msg void OnBnClickedButtonFatherMasterMe() { SetParentMasterName(DQMJ_PARENT_FATHER, m_masterName); }
	afx_msg void OnBnClickedButtonMotherMasterMe() { SetParentMasterName(DQMJ_PARENT_MOTHER, m_masterName); }
	afx_msg void OnBnClickedButtonFatherMasterUnknown() { SetParentMasterName(DQMJ_PARENT_FATHER, NULL); }
	afx_msg void OnBnClickedButtonMotherMasterUnknown() { SetParentMasterName(DQMJ_PARENT_MOTHER, NULL); }
	afx_msg void OnLbnSelchangeListSkillList();
	afx_msg void OnCbnSelchangeComboRace();
	afx_msg void OnCbnSelchangeComboRank();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnCbnSelchangeComboMaxLevel();

	DECLARE_MESSAGE_MAP()

private:

	BOOL					m_bInterrupt;
	CString					m_masterName;
	DQMJ_SAVE_MONSTER_INFO	m_monsterInfo;
	DQMJ_SAVE_MONSTER_INFO	m_backupInfo;

	static CIntList			m_raceIDList;
	static CMapIntToInt		m_raceToCBIndex;

	void UpdateSaveData(int sel);
	void RefreshMonster(int sel);
	void RefreshMonsterInfo(int sel);
	void RefreshSkillSel(int sel, BOOL disable = FALSE);
	void UpdateStatusSpinRange(BOOL normalize);
	void CheckSkillsetNone(int idx);
	void CheckParentNone(int parent);
	void CheckGrandparentNone(int parent, int grandparent);
	void SetMasterName(LPCTSTR name);
	void SetParentMasterName(int parent, LPCTSTR name);

	static void InitCBIndex();
	static void InitComboBox(CComboBox &combo, const int *list, int len);
	static void InitComboBox(CComboBox &combo, const CStringArray &sa);
	static void InitComboBox(CComboBox &combo, const CIntList &list, const CMapIntToStr &map);
	static int GetSafeCBIndex(CComboBox &combo, int idx);
	static int GetSafeCBIndex(CComboBox &combo, int id, const CMapIntToInt &map);

};

//////////////////////////////////////////////////////////////////////////
