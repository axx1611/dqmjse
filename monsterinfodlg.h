
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterInfoDlg)

public:

	explicit CMonsterInfoDlg(const DQMJ_SAVE_MONSTER_INFO *monsterinfo = NULL, BOOL in_party_or_standby = FALSE, CWnd *pParent = NULL);

	const DQMJ_SAVE_MONSTER_INFO &GetMonsterInfo() const { return m_monsterInfo; }

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
	int			m_nWeapon;
	int			m_nSkillPoint;
	int			m_nGrowthStartLevel;
	int			m_nGrowthEndLevel;
	int			m_nSkillsetPt[DQMJ_SKILLSET_MAX];
	CString		m_strName;
	CString		m_strMaster;
	CString		m_strParentName[DQMJ_PARENT_NUM];
	CString		m_strParentMaster[DQMJ_PARENT_NUM];
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
	afx_msg void OnBnClickedCheckFatherNone() { CheckParentNone(DQMJ_PARENT_FATHER); }
	afx_msg void OnBnClickedCheckMotherNone() { CheckParentNone(DQMJ_PARENT_MOTHER); }
	afx_msg void OnBnClickedCheckFatherFatherNone() { CheckGrandparentNone(DQMJ_PARENT_FATHER, DQMJ_PARENT_FATHER); }
	afx_msg void OnBnClickedCheckFatherMotherNone() { CheckGrandparentNone(DQMJ_PARENT_FATHER, DQMJ_PARENT_MOTHER); }
	afx_msg void OnBnClickedCheckMotherFatherNone() { CheckGrandparentNone(DQMJ_PARENT_MOTHER, DQMJ_PARENT_FATHER); }
	afx_msg void OnBnClickedCheckMotherMotherNone() { CheckGrandparentNone(DQMJ_PARENT_MOTHER, DQMJ_PARENT_MOTHER); }
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonRecover();
	afx_msg void OnBnClickedButtonStatusMax();
	afx_msg void OnBnClickedButtonStatusMaxForce();
	afx_msg void OnBnClickedButtonWeaponModify();
	afx_msg void OnBnClickedButtonWeaponRemove();
	afx_msg void OnBnClickedButtonGrowthRandom();
	afx_msg void OnBnClickedButtonSkillUp();
	afx_msg void OnBnClickedButtonSkillDown();
	afx_msg void OnBnClickedButtonSkillTop();
	afx_msg void OnBnClickedButtonSkillBottom();
	afx_msg void OnBnClickedButtonSkillAdd();
	afx_msg void OnBnClickedButtonSkillModify();
	afx_msg void OnBnClickedButtonSkillRemove();
	afx_msg void OnBnClickedButtonSkillSort();
	afx_msg void OnBnClickedButtonMasterMe() { SetMasterName(&m_masterName); }
	afx_msg void OnBnClickedButtonMasterUnknown() { SetMasterName(NULL); }
	afx_msg void OnBnClickedButtonFatherMasterMe() { SetParentMasterName(DQMJ_PARENT_FATHER, &m_masterName); }
	afx_msg void OnBnClickedButtonMotherMasterMe() { SetParentMasterName(DQMJ_PARENT_MOTHER, &m_masterName); }
	afx_msg void OnBnClickedButtonFatherMasterUnknown() { SetParentMasterName(DQMJ_PARENT_FATHER, NULL); }
	afx_msg void OnBnClickedButtonMotherMasterUnknown() { SetParentMasterName(DQMJ_PARENT_MOTHER, NULL); }
	afx_msg void OnLbnSelchangeListSkillList();
	afx_msg void OnCbnSelchangeComboRace();
	afx_msg void OnCbnSelchangeComboRank();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnCbnSelchangeComboMaxLevel();

	template<int idx>
	afx_msg void OnBnClickedCheckSkillsetNone() { CheckSkillsetNone(idx); }
	template<int idx>
	afx_msg void OnBnClickedCheckAbilityNone() { CheckAbilityNone(idx); }
	template<int idx>
	afx_msg void OnBnClickedButtonSkillsetMax() { SetSkillsetMax(idx); }

	DECLARE_MESSAGE_MAP()

private:

	BOOL					m_bInterrupt;
	BOOL					m_inPartyOrStandby;
	DQMJ_NAME				m_masterName;
	DQMJ_SAVE_MONSTER_INFO	m_monsterInfo;
	DQMJ_SAVE_MONSTER_INFO	m_backupInfo;

	static CIntList			m_raceIDList;
	static CIntList			m_skillsetIDList;
	static CIntList			m_abilityIDList;
	static CIntToIntMap		m_raceToCBIndex;
	static CIntToIntMap		m_skillsetToCBIndex;
	static CIntToIntMap		m_abilityToCBIndex;

	void MakeNewMonsterInfo();
	void ApplySaveData();
	void UpdateSaveData();
	void RefreshMonster();
	void RefreshWeapon();
	void RefreshMonsterInfo();
	void RefreshSkillSel(int sel, BOOL disable = FALSE);
	void UpdateRaceStatusSpinRange();
	void CheckSkillsetNone(int idx);
	void CheckAbilityNone(int idx);
	void CheckParentNone(int parent);
	void CheckGrandparentNone(int parent, int grandparent);
	void SetSkillsetMax(int idx);
	void SetMasterName(const DQMJ_NAME *name);
	void SetParentMasterName(int parent, const DQMJ_NAME *name);
	void UpdateSkillsetSel(int idx);
	void UpdateAbilitySel(int idx);
	int SelectSkill(int sel = LB_ERR);

	static void InitRaceCBIndex();
	static void InitSkillsetCBIndex();
	static void InitAbilityCBIndex();
	static void InitComboBox(CComboBox &combo, const int *list, int len);
	static void InitComboBox(CComboBox &combo, const CStringArray &sa);
	static void InitComboBox(CComboBox &combo, const CIntList &list, const CIntToStrMap &map);
	static int GetSafeCBIndex(CComboBox &combo, int idx);
	static int GetSafeCBIndex(CComboBox &combo, int id, const CIntToIntMap &map);

};

//////////////////////////////////////////////////////////////////////////
