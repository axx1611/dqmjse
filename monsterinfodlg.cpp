
#include "stdafx.h"
#include "dqmjse.h"
#include "monsterinfodlg.h"
#include "selectweapondlg.h"
#include "selectskilldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

const int NEW_MONSTER_RACE								= 0x01;

const int MAXLEVEL_LIST[]								= { 50, 75, 99 };

const int IDC_CHECK_SKILLSET_NONE[]						= { IDC_CHECK_SKILLSET_NONE1, IDC_CHECK_SKILLSET_NONE2, IDC_CHECK_SKILLSET_NONE3 };
const int IDC_LABEL_SKILLSET_PT[]						= { IDC_LABEL_SKILLSET_PT1, IDC_LABEL_SKILLSET_PT2, IDC_LABEL_SKILLSET_PT3 };
const int IDC_EDIT_SKILLSET_PT[]						= { IDC_EDIT_SKILLSET_PT1, IDC_EDIT_SKILLSET_PT2, IDC_EDIT_SKILLSET_PT3 };
const int IDC_SPIN_SKILLSET_PT[]						= { IDC_SPIN_SKILLSET_PT1, IDC_SPIN_SKILLSET_PT2, IDC_SPIN_SKILLSET_PT3 };
const int IDC_BUTTON_SKILLSET_MAX[]						= { IDC_BUTTON_SKILLSET_MAX1, IDC_BUTTON_SKILLSET_MAX2, IDC_BUTTON_SKILLSET_MAX3 };
const int IDC_COMBO_SKILLSET[]							= { IDC_COMBO_SKILLSET1, IDC_COMBO_SKILLSET2, IDC_COMBO_SKILLSET3 };

const int IDC_CHECK_ABILITY_NONE[]						= { IDC_CHECK_ABILITY_NONE1, IDC_CHECK_ABILITY_NONE2, IDC_CHECK_ABILITY_NONE3, IDC_CHECK_ABILITY_NONE4, IDC_CHECK_ABILITY_NONE5, IDC_CHECK_ABILITY_NONE6 };
const int IDC_COMBO_ABILITY[]							= { IDC_COMBO_ABILITY1, IDC_COMBO_ABILITY2, IDC_COMBO_ABILITY3, IDC_COMBO_ABILITY4, IDC_COMBO_ABILITY5, IDC_COMBO_ABILITY6 };

const int IDC_GUARD_NAME[]								= { IDC_GUARD_NAME1, IDC_GUARD_NAME2, IDC_GUARD_NAME3, IDC_GUARD_NAME4, IDC_GUARD_NAME5, IDC_GUARD_NAME6, IDC_GUARD_NAME7, IDC_GUARD_NAME8, IDC_GUARD_NAME9, IDC_GUARD_NAME10, IDC_GUARD_NAME11, IDC_GUARD_NAME12, IDC_GUARD_NAME13, IDC_GUARD_NAME14, IDC_GUARD_NAME15, IDC_GUARD_NAME16, IDC_GUARD_NAME17, IDC_GUARD_NAME18, IDC_GUARD_NAME19, IDC_GUARD_NAME20, IDC_GUARD_NAME21, IDC_GUARD_NAME22, IDC_GUARD_NAME23, IDC_GUARD_NAME24, IDC_GUARD_NAME25, IDC_GUARD_NAME26, IDC_GUARD_NAME27 };
const int IDC_COMBO_GUARD_LEVEL[]						= { IDC_COMBO_GUARD_LEVEL1, IDC_COMBO_GUARD_LEVEL2, IDC_COMBO_GUARD_LEVEL3, IDC_COMBO_GUARD_LEVEL4, IDC_COMBO_GUARD_LEVEL5, IDC_COMBO_GUARD_LEVEL6, IDC_COMBO_GUARD_LEVEL7, IDC_COMBO_GUARD_LEVEL8, IDC_COMBO_GUARD_LEVEL9, IDC_COMBO_GUARD_LEVEL10, IDC_COMBO_GUARD_LEVEL11, IDC_COMBO_GUARD_LEVEL12, IDC_COMBO_GUARD_LEVEL13, IDC_COMBO_GUARD_LEVEL14, IDC_COMBO_GUARD_LEVEL15, IDC_COMBO_GUARD_LEVEL16, IDC_COMBO_GUARD_LEVEL17, IDC_COMBO_GUARD_LEVEL18, IDC_COMBO_GUARD_LEVEL19, IDC_COMBO_GUARD_LEVEL20, IDC_COMBO_GUARD_LEVEL21, IDC_COMBO_GUARD_LEVEL22, IDC_COMBO_GUARD_LEVEL23, IDC_COMBO_GUARD_LEVEL24, IDC_COMBO_GUARD_LEVEL25, IDC_COMBO_GUARD_LEVEL26, IDC_COMBO_GUARD_LEVEL27 };

const int IDC_EDIT_PARENT_NAME[]						= { IDC_EDIT_FATHER_NAME, IDC_EDIT_MOTHER_NAME };
const int IDC_EDIT_PARENT_MASTER[]						= { IDC_EDIT_FATHER_MASTER, IDC_EDIT_MOTHER_MASTER };
const int IDC_BUTTON_PARENT_MASTER_ME[]					= { IDC_BUTTON_FATHER_MASTER_ME, IDC_BUTTON_MOTHER_MASTER_ME };
const int IDC_BUTTON_PARENT_MASTER_UNKNOWN[]			= { IDC_BUTTON_FATHER_MASTER_UNKNOWN, IDC_BUTTON_MOTHER_MASTER_UNKNOWN };
const int IDC_CHECK_PARENT_NONE[]						= { IDC_CHECK_FATHER_NONE, IDC_CHECK_MOTHER_NONE };
const int IDC_COMBO_PARENT_RACE[]						= { IDC_COMBO_FATHER_RACE, IDC_COMBO_MOTHER_RACE };

const int IDC_CHECK_GRANDPARENT_NONE[][DQMJ_PARENT_NUM]	= { { IDC_CHECK_FATHER_FATHER_NONE, IDC_CHECK_FATHER_MOTHER_NONE }, { IDC_CHECK_MOTHER_FATHER_NONE, IDC_CHECK_MOTHER_MOTHER_NONE } };
const int IDC_COMBO_GRANDPARENT_RACE[][DQMJ_PARENT_NUM]	= { { IDC_COMBO_FATHER_FATHER_RACE, IDC_COMBO_FATHER_MOTHER_RACE }, { IDC_COMBO_MOTHER_FATHER_RACE, IDC_COMBO_MOTHER_MOTHER_RACE } };

//////////////////////////////////////////////////////////////////////////

CIntList CMonsterInfoDlg::m_raceIDList;
CIntList CMonsterInfoDlg::m_skillsetIDList;
CIntList CMonsterInfoDlg::m_abilityIDList;
CIntToIntMap CMonsterInfoDlg::m_raceToCBIndex;
CIntToIntMap CMonsterInfoDlg::m_skillsetToCBIndex;
CIntToIntMap CMonsterInfoDlg::m_abilityToCBIndex;

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMonsterInfoDlg, CDialog)

BEGIN_MESSAGE_MAP(CMonsterInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_NORMALIZE, OnBnClickedCheckNormalize)
	ON_BN_CLICKED(IDC_CHECK_RANK_AUTO, OnBnClickedCheckRankAuto)
	ON_BN_CLICKED(IDC_CHECK_TYPE_AUTO, OnBnClickedCheckTypeAuto)
	ON_BN_CLICKED(IDC_CHECK_DEAD, OnBnClickedCheckDead)
	ON_BN_CLICKED(IDC_CHECK_SKILLSET_NONE1, OnBnClickedCheckSkillsetNone<0>)
	ON_BN_CLICKED(IDC_CHECK_SKILLSET_NONE2, OnBnClickedCheckSkillsetNone<1>)
	ON_BN_CLICKED(IDC_CHECK_SKILLSET_NONE3, OnBnClickedCheckSkillsetNone<2>)
	ON_BN_CLICKED(IDC_CHECK_ABILITY_NONE1, OnBnClickedCheckAbilityNone<0>)
	ON_BN_CLICKED(IDC_CHECK_ABILITY_NONE2, OnBnClickedCheckAbilityNone<1>)
	ON_BN_CLICKED(IDC_CHECK_ABILITY_NONE3, OnBnClickedCheckAbilityNone<2>)
	ON_BN_CLICKED(IDC_CHECK_ABILITY_NONE4, OnBnClickedCheckAbilityNone<3>)
	ON_BN_CLICKED(IDC_CHECK_ABILITY_NONE5, OnBnClickedCheckAbilityNone<4>)
	ON_BN_CLICKED(IDC_CHECK_ABILITY_NONE6, OnBnClickedCheckAbilityNone<5>)
	ON_BN_CLICKED(IDC_CHECK_FATHER_NONE, OnBnClickedCheckFatherNone)
	ON_BN_CLICKED(IDC_CHECK_MOTHER_NONE, OnBnClickedCheckMotherNone)
	ON_BN_CLICKED(IDC_CHECK_FATHER_FATHER_NONE, OnBnClickedCheckFatherFatherNone)
	ON_BN_CLICKED(IDC_CHECK_FATHER_MOTHER_NONE, OnBnClickedCheckFatherMotherNone)
	ON_BN_CLICKED(IDC_CHECK_MOTHER_FATHER_NONE, OnBnClickedCheckMotherFatherNone)
	ON_BN_CLICKED(IDC_CHECK_MOTHER_MOTHER_NONE, OnBnClickedCheckMotherMotherNone)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RECOVER, OnBnClickedButtonRecover)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_MAX, OnBnClickedButtonStatusMax)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_MAX_FORCE, OnBnClickedButtonStatusMaxForce)
	ON_BN_CLICKED(IDC_BUTTON_WEAPON_MODIFY, OnBnClickedButtonWeaponModify)
	ON_BN_CLICKED(IDC_BUTTON_WEAPON_REMOVE, OnBnClickedButtonWeaponRemove)
	ON_BN_CLICKED(IDC_BUTTON_GROWTH_RANDOM, OnBnClickedButtonGrowthRandom)
	ON_BN_CLICKED(IDC_BUTTON_SKILLSET_MAX1, OnBnClickedButtonSkillsetMax<0>)
	ON_BN_CLICKED(IDC_BUTTON_SKILLSET_MAX2, OnBnClickedButtonSkillsetMax<1>)
	ON_BN_CLICKED(IDC_BUTTON_SKILLSET_MAX3, OnBnClickedButtonSkillsetMax<2>)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_UP, OnBnClickedButtonSkillUp)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_DOWN, OnBnClickedButtonSkillDown)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_TOP, OnBnClickedButtonSkillTop)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_BOTTOM, OnBnClickedButtonSkillBottom)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_ADD, OnBnClickedButtonSkillAdd)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_MODIFY, OnBnClickedButtonSkillModify)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_REMOVE, OnBnClickedButtonSkillRemove)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_SORT, OnBnClickedButtonSkillSort)
	ON_BN_CLICKED(IDC_BUTTON_MASTER_ME, OnBnClickedButtonMasterMe)
	ON_BN_CLICKED(IDC_BUTTON_MASTER_UNKNOWN, OnBnClickedButtonMasterUnknown)
	ON_BN_CLICKED(IDC_BUTTON_FATHER_MASTER_ME, OnBnClickedButtonFatherMasterMe)
	ON_BN_CLICKED(IDC_BUTTON_MOTHER_MASTER_ME, OnBnClickedButtonMotherMasterMe)
	ON_BN_CLICKED(IDC_BUTTON_FATHER_MASTER_UNKNOWN, OnBnClickedButtonFatherMasterUnknown)
	ON_BN_CLICKED(IDC_BUTTON_MOTHER_MASTER_UNKNOWN, OnBnClickedButtonMotherMasterUnknown)
	ON_LBN_SELCHANGE(IDC_LIST_SKILL_LIST, OnLbnSelchangeListSkillList)
	ON_CBN_SELCHANGE(IDC_COMBO_RACE, OnCbnSelchangeComboRace)
	ON_CBN_SELCHANGE(IDC_COMBO_RANK, OnCbnSelchangeComboRank)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_MAX_LEVEL, OnCbnSelchangeComboMaxLevel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CMonsterInfoDlg::CMonsterInfoDlg(const DQMJ_SAVE_MONSTER_INFO *monsterinfo, BOOL in_party_or_standby, CWnd *pParent) : CDialog(IDD_DQMJSE_MONSTER_INFO, pParent)
{
	m_nLevel = 0;
	m_nCombine = 0;
	m_nBaseHp = 0;
	m_nBaseMp = 0;
	m_nBaseAtk = 0;
	m_nBaseDef = 0;
	m_nBaseAgi = 0;
	m_nBaseInt = 0;
	m_nAtk = 0;
	m_nDef = 0;
	m_nAgi = 0;
	m_nInt = 0;
	m_nCurHp = 0;
	m_nCurMp = 0;
	m_nMaxLevel = 0;
	m_nMaxHp = 0;
	m_nMaxMp = 0;
	m_nExp = 0;
	m_nWeapon = 0x00;
	m_nSkillPoint = 0;
	m_nGrowthStartLevel = 0;
	m_nGrowthEndLevel = 0;
	m_strName = _T("");
	m_strMaster = _T("");

	DQMJ_SAVE_BRIEFING briefing;
	VERIFY(::DQMJSaveQueryBriefing(theApp.GetSaveHandle(), &briefing));

	DQMJ_SAVE_PLAY_INFO playerinfo;
	VERIFY(::DQMJSaveQueryPlayInfo(theApp.GetSaveHandle(), &playerinfo));

	m_bInterrupt = briefing.interrupt;
	m_masterName = playerinfo.player_name;

	if (monsterinfo == NULL)
	{
		m_inPartyOrStandby = FALSE;
		MakeNewMonsterInfo();
	}
	else
	{
		m_inPartyOrStandby = in_party_or_standby;
		m_monsterInfo = *monsterinfo;
	}

	::ZeroMemory(m_nSkillsetPt, sizeof(m_nSkillsetPt));

	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
	{
		m_strParentName[i] = _T("");
		m_strParentMaster[i] = _T("");
	}

	static BOOL flag = FALSE;
	if (!flag)
	{
		InitRaceCBIndex();
		InitSkillsetCBIndex();
		InitAbilityCBIndex();
		flag = TRUE;
	}

	m_backupInfo = m_monsterInfo;
}

//////////////////////////////////////////////////////////////////////////

void CMonsterInfoDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Text(pDX, IDC_EDIT_LEVEL, m_nLevel);
	::DDX_Text(pDX, IDC_EDIT_COMBINE, m_nCombine);
	::DDX_Text(pDX, IDC_EDIT_BASE_HP, m_nBaseHp);
	::DDX_Text(pDX, IDC_EDIT_BASE_MP, m_nBaseMp);
	::DDX_Text(pDX, IDC_EDIT_BASE_ATK, m_nBaseAtk);
	::DDX_Text(pDX, IDC_EDIT_BASE_DEF, m_nBaseDef);
	::DDX_Text(pDX, IDC_EDIT_BASE_AGI, m_nBaseAgi);
	::DDX_Text(pDX, IDC_EDIT_BASE_INT, m_nBaseInt);
	::DDX_Text(pDX, IDC_EDIT_ATK, m_nAtk);
	::DDX_Text(pDX, IDC_EDIT_DEF, m_nDef);
	::DDX_Text(pDX, IDC_EDIT_AGI, m_nAgi);
	::DDX_Text(pDX, IDC_EDIT_INT, m_nInt);
	::DDX_Text(pDX, IDC_EDIT_CUR_HP, m_nCurHp);
	::DDX_Text(pDX, IDC_EDIT_CUR_MP, m_nCurMp);
	::DDX_Text(pDX, IDC_EDIT_MAX_LEVEL, m_nMaxLevel);
	::DDX_Text(pDX, IDC_EDIT_MAX_HP, m_nMaxHp);
	::DDX_Text(pDX, IDC_EDIT_MAX_MP, m_nMaxMp);
	::DDX_Text(pDX, IDC_EDIT_EXP, m_nExp);
	::DDX_Text(pDX, IDC_EDIT_SKILL_POINT, m_nSkillPoint);
	::DDX_Text(pDX, IDC_EDIT_GROWTH_START_LEVEL, m_nGrowthStartLevel);
	::DDX_Text(pDX, IDC_EDIT_GROWTH_END_LEVEL, m_nGrowthEndLevel);
	::DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	::DDX_Text(pDX, IDC_EDIT_MASTER, m_strMaster);

	::DDX_Control(pDX, IDC_LIST_SKILL_LIST, m_lstSkill);
	::DDX_Control(pDX, IDC_COMBO_SEX, m_cmbSex);
	::DDX_Control(pDX, IDC_COMBO_RACE, m_cmbRace);
	::DDX_Control(pDX, IDC_COMBO_RANK, m_cmbRank);
	::DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	::DDX_Control(pDX, IDC_COMBO_STRATEGY, m_cmbStrategy);
	::DDX_Control(pDX, IDC_COMBO_SOURCE, m_cmbSource);
	::DDX_Control(pDX, IDC_COMBO_MAX_LEVEL, m_cmbMaxLevel);
	::DDX_Control(pDX, IDC_COMBO_GROWTH_TYPE, m_cmbGrowthType);

	for (int i = 0; i < DQMJ_SKILLSET_MAX; i++)
		::DDX_Text(pDX, IDC_EDIT_SKILLSET_PT[i], m_nSkillsetPt[i]);
	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
		::DDX_Text(pDX, IDC_EDIT_PARENT_NAME[i], m_strParentName[i]);
	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
		::DDX_Text(pDX, IDC_EDIT_PARENT_MASTER[i], m_strParentMaster[i]);

	for (int i = 0; i < DQMJ_GUARD_MAX; i++)
		::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL[i], m_cmbGuard[i]);
	for (int i = 0; i < DQMJ_SKILLSET_MAX; i++)
		::DDX_Control(pDX, IDC_COMBO_SKILLSET[i], m_cmbSkillset[i]);
	for (int i = 0; i < DQMJ_ABILITY_MAX; i++)
		::DDX_Control(pDX, IDC_COMBO_ABILITY[i], m_cmbAbility[i]);
	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
		::DDX_Control(pDX, IDC_COMBO_PARENT_RACE[i], m_cmbParentRace[i]);

	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
	for (int j = 0; j < DQMJ_PARENT_NUM; j++)
		::DDX_Control(pDX, IDC_COMBO_GRANDPARENT_RACE[i][j], m_cmbGrandparentRace[i][j]);

	CDialog::DoDataExchange(pDX);
}

BOOL CMonsterInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitComboBox(m_cmbSex, theApp.GetSexStringList());
	InitComboBox(m_cmbRace, m_raceIDList, theApp.GetRaceStringMap());
	InitComboBox(m_cmbRank, theApp.GetRankStringList());
	InitComboBox(m_cmbType, theApp.GetTypeStringList());
	InitComboBox(m_cmbStrategy, theApp.GetStrategyStringList());
	InitComboBox(m_cmbSource, theApp.GetSourceStringList());
	InitComboBox(m_cmbGrowthType, theApp.GetGrowthStringList());
	InitComboBox(m_cmbMaxLevel, MAXLEVEL_LIST, _countof(MAXLEVEL_LIST));

	for (int i = 0; i < DQMJ_SKILLSET_MAX; i++)
		InitComboBox(m_cmbSkillset[i], m_skillsetIDList, theApp.GetSkillsetStringMap());
	for (int i = 0; i < DQMJ_ABILITY_MAX; i++)
		InitComboBox(m_cmbAbility[i], m_abilityIDList, theApp.GetAbilityStringMap());
	for (int i = 0; i < DQMJ_GUARD_MAX; i++)
		InitComboBox(m_cmbGuard[i], theApp.GetGuardStringList());
	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
		InitComboBox(m_cmbParentRace[i], m_raceIDList, theApp.GetRaceStringMap());

	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
	for (int j = 0; j < DQMJ_PARENT_NUM; j++)
		InitComboBox(m_cmbGrandparentRace[i][j], m_raceIDList, theApp.GetRaceStringMap());

	for (int i = 0; i < DQMJ_GUARD_MAX; i++)
		SetDlgItemText(IDC_GUARD_NAME[i], ::DQMJSaveGetGuardName(i));

	int levelMaxDigit = theApp.CalcDigit(DQMJ_LEVEL_MAX);
	int statusMaxDigit = theApp.CalcDigit(DQMJ_STATUS_MAX);

	((CEdit *)GetDlgItem(IDC_EDIT_NAME))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_MASTER))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_COMBINE))->SetLimitText(theApp.CalcDigit(DQMJ_COMBINE_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_LEVEL))->SetLimitText(levelMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_MAX_LEVEL))->SetLimitText(levelMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_HP))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_MP))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_ATK))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_DEF))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_AGI))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_INT))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_ATK))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_DEF))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_AGI))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_INT))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_MAX_HP))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_MAX_MP))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_CUR_HP))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_CUR_MP))->SetLimitText(statusMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_SKILL_POINT))->SetLimitText(theApp.CalcDigit(DQMJ_SKILLPOINT_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_EXP))->SetLimitText(theApp.CalcDigit(INT_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_GROWTH_START_LEVEL))->SetLimitText(levelMaxDigit);
	((CEdit *)GetDlgItem(IDC_EDIT_GROWTH_END_LEVEL))->SetLimitText(levelMaxDigit);

	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_COMBINE))->SetRange32(0, DQMJ_COMBINE_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_LEVEL))->SetRange32(0, DQMJ_LEVEL_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_MAX_LEVEL))->SetRange32(0, DQMJ_LEVEL_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_ATK))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_DEF))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_AGI))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_INT))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_MAX_HP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_MAX_MP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_SKILL_POINT))->SetRange32(0, DQMJ_SKILLPOINT_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_EXP))->SetRange32(0, INT_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_GROWTH_START_LEVEL))->SetRange32(0, DQMJ_LEVEL_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_GROWTH_END_LEVEL))->SetRange32(0, DQMJ_LEVEL_MAX);

	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
		((CEdit *)GetDlgItem(IDC_EDIT_PARENT_NAME[i]))->SetLimitText(DQMJ_NAME_MAX);
	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
		((CEdit *)GetDlgItem(IDC_EDIT_PARENT_MASTER[i]))->SetLimitText(DQMJ_NAME_MAX);

	RefreshMonster();

	return TRUE;
}

void CMonsterInfoDlg::OnOK()
{
	ApplySaveData();
	CDialog::OnOK();
}

void CMonsterInfoDlg::OnCancel()
{
	ApplySaveData();
	CDialog::OnCancel();
}

void CMonsterInfoDlg::OnBnClickedCheckNormalize()
{
	UpdateSaveData();
	VERIFY(::DQMJSaveNormalizeMonster(m_bInterrupt, m_inPartyOrStandby, &m_masterName, &m_monsterInfo));
	RefreshMonster();
}

void CMonsterInfoDlg::OnBnClickedCheckRankAuto()
{
	DQMJ_SAVE_RACE_SETTING rs;
	if (::DQMJSaveGetRaceSetting(m_monsterInfo.race, &rs))
	{
		m_cmbRank.SetCurSel(GetSafeCBIndex(m_cmbRank, rs.rank));
		GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(FALSE);
	}
	else
	{
		m_cmbRank.SetCurSel(CB_ERR);
	}
}

void CMonsterInfoDlg::OnBnClickedCheckTypeAuto()
{
	DQMJ_SAVE_RACE_SETTING rs;
	if (::DQMJSaveGetRaceSetting(m_monsterInfo.race, &rs))
	{
		m_cmbType.SetCurSel(GetSafeCBIndex(m_cmbType, rs.type));
		GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(FALSE);
	}
	else
	{
		m_cmbType.SetCurSel(CB_ERR);
	}
}

void CMonsterInfoDlg::OnBnClickedCheckDead()
{
	if (IsDlgButtonChecked(IDC_CHECK_DEAD) == BST_CHECKED)
	{
		UpdateData(TRUE);
		m_nCurHp = 0;
		UpdateData(FALSE);

		if (IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED)
		{
			GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(FALSE);
		}
	}
	else
	{
		UpdateData(TRUE);
		m_nCurHp = max(m_nMaxHp, 1);
		UpdateData(FALSE);

		GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(TRUE);
	}
}

void CMonsterInfoDlg::OnBnClickedButtonApply()
{
	ApplySaveData();
	RefreshMonsterInfo();
}

void CMonsterInfoDlg::OnBnClickedButtonRecover()
{
	m_monsterInfo = m_backupInfo;

	CheckDlgButton(IDC_CHECK_NORMALIZE, BST_UNCHECKED);
	RefreshMonster();
}

void CMonsterInfoDlg::OnBnClickedButtonStatusMax()
{
	DQMJ_SAVE_RACE_SETTING rs;
	if (!::DQMJSaveGetRaceSetting(m_monsterInfo.race, &rs))
		return;

	UpdateSaveData();

	DQMJ_SAVE_MONSTER_INFO monsterinfo = m_monsterInfo;
	monsterinfo.combine = DQMJ_COMBINE_MAX;
	monsterinfo.level = DQMJ_LEVEL_MAX;
	monsterinfo.max_level = DQMJ_LEVEL_MAX;
	monsterinfo.remaining_sp = DQMJ_SKILLPOINT_MAX;
	monsterinfo.base_hp = rs.limit_hp;
	monsterinfo.base_mp = rs.limit_mp;
	monsterinfo.base_atk = rs.limit_atk;
	monsterinfo.base_def = rs.limit_def;
	monsterinfo.base_agi = rs.limit_agi;
	monsterinfo.base_int = rs.limit_int;

	VERIFY(::DQMJSaveNormalizeMonster(m_bInterrupt, m_inPartyOrStandby, &m_masterName, &monsterinfo));

	m_monsterInfo.combine = monsterinfo.combine;
	m_monsterInfo.level = monsterinfo.level;
	m_monsterInfo.max_level = monsterinfo.max_level;
	m_monsterInfo.remaining_sp = monsterinfo.remaining_sp;
	m_monsterInfo.base_hp = monsterinfo.base_hp;
	m_monsterInfo.base_mp = monsterinfo.base_mp;
	m_monsterInfo.base_atk = monsterinfo.base_atk;
	m_monsterInfo.base_def = monsterinfo.base_def;
	m_monsterInfo.base_agi = monsterinfo.base_agi;
	m_monsterInfo.base_int = monsterinfo.base_int;
	m_monsterInfo.actual_hp = monsterinfo.actual_hp;
	m_monsterInfo.actual_mp = monsterinfo.actual_mp;
	m_monsterInfo.actual_atk = monsterinfo.actual_atk;
	m_monsterInfo.actual_def = monsterinfo.actual_def;
	m_monsterInfo.actual_agi = monsterinfo.actual_agi;
	m_monsterInfo.actual_int = monsterinfo.actual_int;
	m_monsterInfo.current_hp = monsterinfo.current_hp;
	m_monsterInfo.current_mp = monsterinfo.current_mp;

	RefreshMonsterInfo();
}

void CMonsterInfoDlg::OnBnClickedButtonStatusMaxForce()
{
	UpdateData(TRUE);

	m_nCombine = DQMJ_COMBINE_MAX;
	m_nLevel = DQMJ_LEVEL_MAX;
	m_nMaxLevel = DQMJ_LEVEL_MAX;
	m_nSkillPoint = DQMJ_SKILLPOINT_MAX;

	m_nBaseHp = DQMJ_STATUS_MAX;
	m_nBaseMp = DQMJ_STATUS_MAX;
	m_nBaseAtk = DQMJ_STATUS_MAX;
	m_nBaseDef = DQMJ_STATUS_MAX;
	m_nBaseAgi = DQMJ_STATUS_MAX;
	m_nBaseInt = DQMJ_STATUS_MAX;

	m_nAtk = DQMJ_STATUS_MAX;
	m_nDef = DQMJ_STATUS_MAX;
	m_nAgi = DQMJ_STATUS_MAX;
	m_nInt = DQMJ_STATUS_MAX;

	m_nCurHp = DQMJ_STATUS_MAX;
	m_nCurMp = DQMJ_STATUS_MAX;
	m_nMaxHp = DQMJ_STATUS_MAX;
	m_nMaxMp = DQMJ_STATUS_MAX;

	UpdateData(FALSE);
}

void CMonsterInfoDlg::OnBnClickedButtonWeaponModify()
{
	UpdateSaveData();

	CSelectWeaponDlg dlg(m_monsterInfo, IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED, this);
	if (dlg.DoModal() != IDOK)
		return;

	m_nWeapon = dlg.GetSelWeapon();
	RefreshWeapon();
}

void CMonsterInfoDlg::OnBnClickedButtonWeaponRemove()
{
	m_nWeapon = 0x00;
	RefreshWeapon();
}

void CMonsterInfoDlg::OnBnClickedButtonGrowthRandom()
{
	UpdateData(TRUE);

	DQMJ_SAVE_MONSTER_INFO monsterinfo = m_monsterInfo;
	monsterinfo.growth_type = DQMJ_RAND_GROWTH_TYPE;
	monsterinfo.growth_start_level = DQMJ_RAND_GROWTH_LEVEL;
	monsterinfo.growth_end_level = DQMJ_RAND_GROWTH_LEVEL;

	VERIFY(::DQMJSaveNormalizeMonster(m_bInterrupt, m_inPartyOrStandby, &m_masterName, &monsterinfo));

	m_cmbGrowthType.SetCurSel(monsterinfo.growth_type);
	m_nGrowthStartLevel = monsterinfo.growth_start_level;
	m_nGrowthEndLevel = monsterinfo.growth_end_level;

	UpdateData(FALSE);
}

void CMonsterInfoDlg::OnBnClickedButtonSkillUp()
{
	if (m_lstSkill.GetSelCount() != 1)
		return;

	int sel;
	VERIFY(m_lstSkill.GetSelItems(1, &sel) == 1);
	if (sel == 0)
		return;

	CString text;
	m_lstSkill.GetText(sel, text);
	int skill = m_lstSkill.GetItemData(sel);
	m_lstSkill.DeleteString(sel);

	sel--;
	VERIFY(m_lstSkill.InsertString(sel, text) == sel);
	m_lstSkill.SetItemData(sel, skill);

	m_lstSkill.SetSel(sel);
	RefreshSkillSel(sel);
}

void CMonsterInfoDlg::OnBnClickedButtonSkillDown()
{
	if (m_lstSkill.GetSelCount() != 1)
		return;

	int sel;
	VERIFY(m_lstSkill.GetSelItems(1, &sel) == 1);
	if (sel >= m_lstSkill.GetCount() - 1)
		return;

	CString text;
	m_lstSkill.GetText(sel, text);
	int skill = m_lstSkill.GetItemData(sel);
	m_lstSkill.DeleteString(sel);

	sel++;
	VERIFY(m_lstSkill.InsertString(sel, text) == sel);
	m_lstSkill.SetItemData(sel, skill);

	m_lstSkill.SetSel(sel);
	RefreshSkillSel(sel);
}

void CMonsterInfoDlg::OnBnClickedButtonSkillTop()
{
	if (m_lstSkill.GetSelCount() != 1)
		return;

	int sel;
	if ((m_lstSkill.GetSelItems(1, &sel) != 1) || (sel == 0))
		return;

	CString text;
	m_lstSkill.GetText(sel, text);
	int skill = m_lstSkill.GetItemData(sel);
	m_lstSkill.DeleteString(sel);

	sel = 0;
	VERIFY(m_lstSkill.InsertString(sel, text) == sel);
	m_lstSkill.SetItemData(sel, skill);

	m_lstSkill.SetSel(sel);
	RefreshSkillSel(sel);
}

void CMonsterInfoDlg::OnBnClickedButtonSkillBottom()
{
	if (m_lstSkill.GetSelCount() != 1)
		return;

	int sel;
	if ((m_lstSkill.GetSelItems(1, &sel) != 1) || (sel >= m_lstSkill.GetCount() - 1))
		return;

	CString text;
	m_lstSkill.GetText(sel, text);
	int skill = m_lstSkill.GetItemData(sel);
	m_lstSkill.DeleteString(sel);

	sel = m_lstSkill.GetCount();
	VERIFY(m_lstSkill.InsertString(sel, text) == sel);
	m_lstSkill.SetItemData(sel, skill);

	m_lstSkill.SetSel(sel);
	RefreshSkillSel(sel);
}

void CMonsterInfoDlg::OnBnClickedButtonSkillAdd()
{
	if (m_lstSkill.GetCount() >= DQMJ_SKILL_MAX)
		return;

	RefreshSkillSel(SelectSkill());
}

void CMonsterInfoDlg::OnBnClickedButtonSkillModify()
{
	if (m_lstSkill.GetSelCount() != 1)
		return;

	int sel;
	if (m_lstSkill.GetSelItems(1, &sel) != 1)
		return;

	RefreshSkillSel(SelectSkill(sel));
}

void CMonsterInfoDlg::OnBnClickedButtonSkillRemove()
{
	if (m_lstSkill.GetSelCount() < 0)
		return;

	for (int i = DQMJ_SKILL_MAX - 1; i >= 0; i--)
	{
		if (m_lstSkill.GetSel(i) > 0)
			m_lstSkill.DeleteString(i);
	}

	SetDlgItemText(IDC_SKILL_NUM, theApp.GetDecimalString(m_lstSkill.GetCount(), DQMJ_SKILL_MAX));
	RefreshSkillSel(LB_ERR);
}

void CMonsterInfoDlg::OnBnClickedButtonSkillSort()
{
	if (m_lstSkill.GetCount() <= 0)
		return;

	int skill[DQMJ_SKILL_MAX];
	::ZeroMemory(skill, sizeof(skill));

	int len = m_lstSkill.GetCount();
	for (int i = 0; i < len; i++)
		skill[i] = m_lstSkill.GetItemData(i);

	std::sort(skill, skill + len);
	m_lstSkill.ResetContent();

	for (int i = 0; i < len; i++)
	{
		int idx = m_lstSkill.AddString(theApp.GetSkillString(skill[i]));
		ASSERT(idx >= 0);
		m_lstSkill.SetItemData(idx, skill[i]);
	}

	RefreshSkillSel(LB_ERR);
}

void CMonsterInfoDlg::OnLbnSelchangeListSkillList()
{
	RefreshSkillSel(m_lstSkill.GetCurSel());
}

void CMonsterInfoDlg::OnCbnSelchangeComboRace()
{
	ApplySaveData();
	UpdateRaceStatusSpinRange();
	RefreshMonsterInfo();
}

void CMonsterInfoDlg::OnCbnSelchangeComboRank()
{
	UpdateSaveData();
	RefreshMonsterInfo();
}

void CMonsterInfoDlg::OnCbnSelchangeComboType()
{
	UpdateSaveData();
	RefreshMonsterInfo();
}

void CMonsterInfoDlg::OnCbnSelchangeComboMaxLevel()
{
	ASSERT(IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED);

	UpdateData(TRUE);
	m_nMaxLevel = MAXLEVEL_LIST[m_cmbMaxLevel.GetCurSel()];
	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////

void CMonsterInfoDlg::MakeNewMonsterInfo()
{
	::ZeroMemory(&m_monsterInfo, sizeof(m_monsterInfo));

	m_monsterInfo.master = m_masterName;
	m_monsterInfo.race = NEW_MONSTER_RACE;
	m_monsterInfo.sex = DQMJ_RAND_SEX;
	m_monsterInfo.rank = DQMJ_AUTO_RANK;
	m_monsterInfo.type = DQMJ_AUTO_TYPE;
	m_monsterInfo.strategy = -1;
	m_monsterInfo.source = -1;
	m_monsterInfo.growth_type = DQMJ_RAND_GROWTH_TYPE;
	m_monsterInfo.growth_start_level = DQMJ_RAND_GROWTH_LEVEL;
	m_monsterInfo.growth_end_level = DQMJ_RAND_GROWTH_LEVEL;

	DQMJ_SAVE_RACE_SETTING rs;
	VERIFY(::DQMJSaveGetRaceSetting(NEW_MONSTER_RACE, &rs));
	CString name = rs.name;
	m_monsterInfo.name = theApp.GetStringName(name.Left(2));

	VERIFY(::DQMJSaveNormalizeMonster(m_bInterrupt, m_inPartyOrStandby, &m_masterName, &m_monsterInfo));
}

void CMonsterInfoDlg::ApplySaveData()
{
	UpdateSaveData();

	if (IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED)
		VERIFY(::DQMJSaveNormalizeMonster(m_bInterrupt, m_inPartyOrStandby, &m_masterName, &m_monsterInfo));
}

void CMonsterInfoDlg::UpdateSaveData()
{
	UpdateData(TRUE);

	m_monsterInfo.cheat = IsDlgButtonChecked(IDC_CHECK_CHEAT) == BST_CHECKED;
	m_monsterInfo.dead = IsDlgButtonChecked(IDC_CHECK_DEAD) == BST_CHECKED;
	m_monsterInfo.poison = IsDlgButtonChecked(IDC_CHECK_POISON) == BST_CHECKED;
	m_monsterInfo.name = theApp.GetStringName(m_strName);
	m_monsterInfo.master = theApp.GetStringName(m_strMaster);
	m_monsterInfo.race = m_cmbRace.GetItemData(m_cmbRace.GetCurSel());
	m_monsterInfo.sex = m_cmbSex.GetCurSel();
	m_monsterInfo.rank = m_cmbRank.GetCurSel();
	m_monsterInfo.type = m_cmbType.GetCurSel();
	m_monsterInfo.combine = m_nCombine;
	m_monsterInfo.source = m_cmbSource.GetCurSel();
	m_monsterInfo.level = m_nLevel;
	m_monsterInfo.max_level = m_nMaxLevel;
	m_monsterInfo.base_hp = m_nBaseHp;
	m_monsterInfo.base_mp = m_nBaseMp;
	m_monsterInfo.base_atk = m_nBaseAtk;
	m_monsterInfo.base_def = m_nBaseDef;
	m_monsterInfo.base_agi = m_nBaseAgi;
	m_monsterInfo.base_int = m_nBaseInt;
	m_monsterInfo.current_hp = m_nCurHp;
	m_monsterInfo.current_mp = m_nCurMp;
	m_monsterInfo.actual_hp = m_nMaxHp;
	m_monsterInfo.actual_mp = m_nMaxMp;
	m_monsterInfo.actual_atk = m_nAtk;
	m_monsterInfo.actual_def = m_nDef;
	m_monsterInfo.actual_agi = m_nAgi;
	m_monsterInfo.actual_int = m_nInt;
	m_monsterInfo.remaining_sp = m_nSkillPoint;
	m_monsterInfo.exp = m_nExp;
	m_monsterInfo.strategy = m_cmbStrategy.GetCurSel();
	m_monsterInfo.growth_type = m_cmbGrowthType.GetCurSel();
	m_monsterInfo.growth_start_level = m_nGrowthStartLevel;
	m_monsterInfo.growth_end_level = m_nGrowthEndLevel;
	m_monsterInfo.weapon = m_nWeapon;

	for (int i = 0, j = 0, k = DQMJ_SKILLSET_MAX; i < DQMJ_SKILLSET_MAX; i++)
	{
		if ((IsDlgButtonChecked(IDC_CHECK_SKILLSET_NONE[i]) == BST_CHECKED) || (m_cmbSkillset[i].GetCurSel() < 0))
		{
			k--;
			m_monsterInfo.skillset[k] = 0x00;
			m_monsterInfo.assigned_sp[k] = 0;
			m_monsterInfo.skill_active_num[j] = 0;
		}
		else
		{
			m_monsterInfo.skillset[j] = m_cmbSkillset[i].GetItemData(m_cmbSkillset[i].GetCurSel());
			m_monsterInfo.assigned_sp[j] = m_nSkillsetPt[i];
			m_monsterInfo.skill_active_num[j] = ::DQMJSaveCalcSkillActiveCount(m_monsterInfo.skillset[j], m_monsterInfo.assigned_sp[j]);
			j++;
		}
	}

	for (int i = 0; i < DQMJ_GUARD_MAX; i++)
	{
		if (m_cmbGuard[i].GetCurSel() < 0)
			m_monsterInfo.guard_info[i] = DQMJ_GUARD_NORMAL;
		else
			m_monsterInfo.guard_info[i] = m_cmbGuard[i].GetCurSel();
	}

	::ZeroMemory(m_monsterInfo.skill_list, sizeof(m_monsterInfo.skill_list));
	for (int i = 0; i < m_lstSkill.GetCount(); i++)
		m_monsterInfo.skill_list[i] = m_lstSkill.GetItemData(i);

	for (int i = 0, j = 0, k = DQMJ_ABILITY_MAX; i < DQMJ_ABILITY_MAX; i++)
	{
		if ((IsDlgButtonChecked(IDC_CHECK_ABILITY_NONE[i]) == BST_CHECKED) || (m_cmbAbility[i].GetCurSel() < 0))
 			m_monsterInfo.ability_list[--k] = 0x00;
 		else
 			m_monsterInfo.ability_list[j++] = m_cmbAbility[i].GetItemData(m_cmbAbility[i].GetCurSel());
	}

	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
	{
		if ((IsDlgButtonChecked(IDC_CHECK_PARENT_NONE[i]) == BST_CHECKED) || (m_cmbParentRace[i].GetCurSel() < 0))
		{
			m_monsterInfo.parent_race[i] = 0x00;
			::ZeroMemory(&m_monsterInfo.parent_name[i], sizeof(m_monsterInfo.parent_name[i]));
			::ZeroMemory(&m_monsterInfo.parent_master[i], sizeof(m_monsterInfo.parent_master[i]));
		}
		else
		{
			m_monsterInfo.parent_race[i] = m_cmbParentRace[i].GetItemData(m_cmbParentRace[i].GetCurSel());
			m_monsterInfo.parent_name[i] = theApp.GetStringName(m_strParentName[i]);
			m_monsterInfo.parent_master[i] = theApp.GetStringName(m_strParentMaster[i]);
		}

		for (int j = 0; j < DQMJ_PARENT_NUM; j++)
		{
			if ((IsDlgButtonChecked(IDC_CHECK_PARENT_NONE[i]) == BST_CHECKED) || (IsDlgButtonChecked(IDC_CHECK_GRANDPARENT_NONE[i][j]) == BST_CHECKED) || (m_cmbGrandparentRace[i][j].GetCurSel() < 0))
				m_monsterInfo.grandparent_race[i][j] = 0x00;
			else
				m_monsterInfo.grandparent_race[i][j] = m_cmbGrandparentRace[i][j].GetItemData(m_cmbGrandparentRace[i][j].GetCurSel());
		}
	}
}

void CMonsterInfoDlg::RefreshMonster()
{
	BOOL normalize = IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED;
	BOOL freeModeOnly = !normalize;
	BOOL interruptOnly = !normalize || (m_bInterrupt && m_inPartyOrStandby);
	BOOL partyOrStandbyOnly = !normalize || m_inPartyOrStandby;

	m_cmbMaxLevel.ShowWindow(normalize ? SW_SHOWNOACTIVATE : SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_LEVEL)->ShowWindow(normalize ? SW_HIDE : SW_SHOWNOACTIVATE);
	GetDlgItem(IDC_SPIN_MAX_LEVEL)->ShowWindow(normalize ? SW_HIDE : SW_SHOWNOACTIVATE);

	m_lstSkill.EnableWindow(freeModeOnly);

	for (int i = 0; i < DQMJ_GUARD_MAX; i++)
		m_cmbGuard[i].EnableWindow(freeModeOnly);

	GetDlgItem(IDC_EDIT_MASTER)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_BUTTON_MASTER_ME)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_BUTTON_MASTER_UNKNOWN)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_CHEAT)->EnableWindow(freeModeOnly);

	GetDlgItem(IDC_EDIT_MAX_HP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_MAX_MP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_ATK)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_DEF)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_AGI)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_INT)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_EDIT_CUR_MP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_SPIN_MAX_HP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_MAX_MP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_ATK)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_DEF)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_AGI)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_INT)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_SPIN_CUR_MP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_BUTTON_STATUS_MAX_FORCE)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_DEAD)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_CHECK_POISON)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_BUTTON_WEAPON_MODIFY)->EnableWindow(partyOrStandbyOnly);
	GetDlgItem(IDC_BUTTON_WEAPON_REMOVE)->EnableWindow(partyOrStandbyOnly);

	for (int i = 0; i < DQMJ_ABILITY_MAX; i++)
		GetDlgItem(IDC_CHECK_ABILITY_NONE[i])->EnableWindow(freeModeOnly);

	UpdateRaceStatusSpinRange();
	RefreshMonsterInfo();

	RefreshSkillSel(LB_ERR, normalize);
}

void CMonsterInfoDlg::RefreshWeapon()
{
	SetDlgItemText(IDC_WEAPON_NAME, theApp.GetItemString(m_nWeapon));
	GetDlgItem(IDC_BUTTON_WEAPON_REMOVE)->EnableWindow(m_nWeapon != 0x00);
}

void CMonsterInfoDlg::RefreshMonsterInfo()
{
	m_nLevel = m_monsterInfo.level;
	m_nCombine = m_monsterInfo.combine;
	m_nBaseHp = m_monsterInfo.base_hp;
	m_nBaseMp = m_monsterInfo.base_mp;
	m_nBaseAtk = m_monsterInfo.base_atk;
	m_nBaseDef = m_monsterInfo.base_def;
	m_nBaseAgi = m_monsterInfo.base_agi;
	m_nBaseInt = m_monsterInfo.base_int;
	m_nAtk = m_monsterInfo.actual_atk;
	m_nDef = m_monsterInfo.actual_def;
	m_nAgi = m_monsterInfo.actual_agi;
	m_nInt = m_monsterInfo.actual_int;
	m_nMaxLevel = m_monsterInfo.max_level;
	m_nMaxHp = m_monsterInfo.actual_hp;
	m_nMaxMp = m_monsterInfo.actual_mp;
	m_nCurHp = m_monsterInfo.current_hp;
	m_nCurMp = m_monsterInfo.current_mp;
	m_nExp = m_monsterInfo.exp;
	m_nWeapon = m_monsterInfo.weapon;
	m_nSkillPoint = m_monsterInfo.remaining_sp;
	m_strName = theApp.GetNameString(m_monsterInfo.name);
	m_strMaster = theApp.GetNameString(m_monsterInfo.master);

	CheckDlgButton(IDC_CHECK_CHEAT, m_monsterInfo.cheat ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_DEAD, m_monsterInfo.dead ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_POISON, m_monsterInfo.poison ? BST_CHECKED : BST_UNCHECKED);

	RefreshWeapon();

	m_cmbSex.SetCurSel(GetSafeCBIndex(m_cmbSex, m_monsterInfo.sex));
	m_cmbRace.SetCurSel(GetSafeCBIndex(m_cmbRace, m_monsterInfo.race, m_raceToCBIndex));
	m_cmbStrategy.SetCurSel(GetSafeCBIndex(m_cmbStrategy, m_monsterInfo.strategy));
	m_cmbSource.SetCurSel(GetSafeCBIndex(m_cmbSource, m_monsterInfo.source));
	m_cmbMaxLevel.SetCurSel(CB_ERR);
	m_lstSkill.ResetContent();

	BOOL normalize = IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED;
	BOOL freeModeOnly = !normalize;

	DQMJ_SAVE_RACE_SETTING rs;
	if (::DQMJSaveGetRaceSetting(m_monsterInfo.race, &rs))
	{
		GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(freeModeOnly);
		GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(freeModeOnly);

		m_cmbRank.EnableWindow(freeModeOnly);
		m_cmbType.EnableWindow(freeModeOnly);
	}
	else
	{
		rs.rank = DQMJ_RANK_NONE;
		rs.type = DQMJ_TYPE_NONE;

		GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(FALSE);

		m_cmbRank.EnableWindow(FALSE);
		m_cmbType.EnableWindow(FALSE);
	}

	if (normalize && m_monsterInfo.dead)
	{
		GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(FALSE);
	}

	if (m_monsterInfo.rank == DQMJ_AUTO_RANK)
	{
		CheckDlgButton(IDC_CHECK_RANK_AUTO, BST_CHECKED);
		GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(FALSE);

		m_cmbRank.SetCurSel(GetSafeCBIndex(m_cmbRank, rs.rank));
	}
	else
	{
		if (m_monsterInfo.rank == rs.rank)
		{
			CheckDlgButton(IDC_CHECK_RANK_AUTO, BST_CHECKED);
			GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(FALSE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_RANK_AUTO, BST_UNCHECKED);
		}

		m_cmbRank.SetCurSel(GetSafeCBIndex(m_cmbRank, m_monsterInfo.rank));
	}

	if (m_monsterInfo.type == DQMJ_AUTO_TYPE)
	{
		CheckDlgButton(IDC_CHECK_TYPE_AUTO, BST_CHECKED);
		GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(FALSE);

		m_cmbType.SetCurSel(GetSafeCBIndex(m_cmbType, rs.type));
	}
	else
	{
		if (m_monsterInfo.type == rs.type)
		{
			CheckDlgButton(IDC_CHECK_TYPE_AUTO, BST_CHECKED);
			GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(FALSE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_TYPE_AUTO, BST_UNCHECKED);
		}

		m_cmbType.SetCurSel(GetSafeCBIndex(m_cmbType, m_monsterInfo.type));
	}

	if (m_monsterInfo.growth_type == DQMJ_RAND_GROWTH_TYPE)
	{
		m_cmbGrowthType.EnableWindow(FALSE);
		m_cmbGrowthType.SetCurSel(CB_ERR);
	}
	else
	{
		m_cmbGrowthType.EnableWindow(TRUE);
		m_cmbGrowthType.SetCurSel(GetSafeCBIndex(m_cmbGrowthType, m_monsterInfo.growth_type));
	}

	if ((m_monsterInfo.growth_start_level == DQMJ_RAND_GROWTH_LEVEL) || (m_monsterInfo.growth_end_level == DQMJ_RAND_GROWTH_LEVEL))
	{
		m_nGrowthStartLevel = 0;
		m_nGrowthEndLevel = 0;

		GetDlgItem(IDC_EDIT_GROWTH_START_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GROWTH_END_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_GROWTH_START_LEVEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_GROWTH_END_LEVEL)->EnableWindow(FALSE);
	}
	else
	{
		m_nGrowthStartLevel = m_monsterInfo.growth_start_level;
		m_nGrowthEndLevel = m_monsterInfo.growth_end_level;

		GetDlgItem(IDC_EDIT_GROWTH_START_LEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GROWTH_END_LEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_GROWTH_START_LEVEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_GROWTH_END_LEVEL)->EnableWindow(TRUE);
	}

	for (int i = 0; i < DQMJ_GUARD_MAX; i++)
		m_cmbGuard[i].SetCurSel(GetSafeCBIndex(m_cmbGuard[i], m_monsterInfo.guard_info[i]));

	for (int i = 0; i < _countof(MAXLEVEL_LIST); i++)
	{
		if (MAXLEVEL_LIST[i] == m_monsterInfo.max_level)
		{
			m_cmbMaxLevel.SetCurSel(i);
			break;
		}
	}

	for (int i = 0; i < DQMJ_SKILLSET_MAX; i++)
	{
		m_nSkillsetPt[i] = m_monsterInfo.assigned_sp[i];

		int sel = CB_ERR;
		if (m_monsterInfo.skillset[i] != 0x00)
			sel = GetSafeCBIndex(m_cmbSkillset[i], m_monsterInfo.skillset[i], m_skillsetToCBIndex);

		if (sel < 0)
		{
			CheckDlgButton(IDC_CHECK_SKILLSET_NONE[i], BST_CHECKED);

			GetDlgItem(IDC_LABEL_SKILLSET_PT[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SKILLSET_PT[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_SKILLSET_PT[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SKILLSET_MAX[i])->EnableWindow(FALSE);

			m_cmbSkillset[i].EnableWindow(FALSE);
			m_cmbSkillset[i].SetCurSel(CB_ERR);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_SKILLSET_NONE[i], BST_UNCHECKED);

			GetDlgItem(IDC_LABEL_SKILLSET_PT[i])->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SKILLSET_PT[i])->EnableWindow(TRUE);
			GetDlgItem(IDC_SPIN_SKILLSET_PT[i])->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SKILLSET_MAX[i])->EnableWindow(TRUE);

			m_cmbSkillset[i].EnableWindow(TRUE);
			m_cmbSkillset[i].SetCurSel(sel);
		}
	}

	for (int i = 0; i < DQMJ_ABILITY_MAX; i++)
	{
		int sel = CB_ERR;
		if (m_monsterInfo.ability_list[i] != 0x00)
			sel = GetSafeCBIndex(m_cmbAbility[i], m_monsterInfo.ability_list[i], m_abilityToCBIndex);

		if (sel < 0)
		{
			CheckDlgButton(IDC_CHECK_ABILITY_NONE[i], BST_CHECKED);

			m_cmbAbility[i].EnableWindow(FALSE);
			m_cmbAbility[i].SetCurSel(CB_ERR);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_ABILITY_NONE[i], BST_UNCHECKED);

			m_cmbAbility[i].EnableWindow(freeModeOnly);
			m_cmbAbility[i].SetCurSel(sel);
		}
	}

	for (int i = 0; i < DQMJ_SKILL_MAX; i++)
	{
		LPCTSTR str = ::DQMJSaveGetSkillName(m_monsterInfo.skill_list[i]);
		if (str != NULL)
			VERIFY(m_lstSkill.SetItemData(m_lstSkill.AddString(str), m_monsterInfo.skill_list[i]) != LB_ERR);
	}

	SetDlgItemText(IDC_SKILL_NUM, theApp.GetDecimalString(m_lstSkill.GetCount(), DQMJ_SKILL_MAX));

	for (int i = 0; i < DQMJ_PARENT_NUM; i++)
	{
		BOOL enable = m_monsterInfo.parent_race[i] != 0x00;

		CheckDlgButton(IDC_CHECK_PARENT_NONE[i], enable ? BST_UNCHECKED : BST_CHECKED);

		if (enable)
		{
			m_strParentName[i] = theApp.GetNameString(m_monsterInfo.parent_name[i]);
			m_strParentMaster[i] = theApp.GetNameString(m_monsterInfo.parent_master[i]);
			m_cmbParentRace[i].SetCurSel(GetSafeCBIndex(m_cmbParentRace[i], m_monsterInfo.parent_race[i], m_raceToCBIndex));
		}
		else
		{
			m_strParentName[i] = _T("");
			m_strParentMaster[i] = _T("");
			m_cmbParentRace[i].SetCurSel(CB_ERR);
		}

		GetDlgItem(IDC_EDIT_PARENT_NAME[i])->EnableWindow(enable);
		GetDlgItem(IDC_EDIT_PARENT_MASTER[i])->EnableWindow(enable);
		GetDlgItem(IDC_BUTTON_PARENT_MASTER_ME[i])->EnableWindow(enable);
		GetDlgItem(IDC_BUTTON_PARENT_MASTER_UNKNOWN[i])->EnableWindow(enable);

		m_cmbParentRace[i].EnableWindow(enable);

		for (int j = 0; j < DQMJ_PARENT_NUM; j++)
		{
			GetDlgItem(IDC_CHECK_GRANDPARENT_NONE[i][j])->EnableWindow(enable);

			if (m_monsterInfo.grandparent_race[i][j] == 0x00)
			{
				CheckDlgButton(IDC_CHECK_GRANDPARENT_NONE[i][j], BST_CHECKED);
				m_cmbGrandparentRace[i][j].EnableWindow(FALSE);
				m_cmbGrandparentRace[i][j].SetCurSel(CB_ERR);
			}
			else
			{
				CheckDlgButton(IDC_CHECK_GRANDPARENT_NONE[i][j], BST_UNCHECKED);
				m_cmbGrandparentRace[i][j].EnableWindow(enable);
				m_cmbGrandparentRace[i][j].SetCurSel(GetSafeCBIndex(m_cmbGrandparentRace[i][j], m_monsterInfo.grandparent_race[i][j], m_raceToCBIndex));
			}
		}
	}

	UpdateData(FALSE);
}

void CMonsterInfoDlg::RefreshSkillSel(int sel, BOOL disable)
{
	int item_count = m_lstSkill.GetCount();
	int sel_count = m_lstSkill.GetSelCount();

	GetDlgItem(IDC_BUTTON_SKILL_UP)->EnableWindow(!disable && (sel_count == 1) && (sel > 0));
	GetDlgItem(IDC_BUTTON_SKILL_TOP)->EnableWindow(!disable && (sel_count == 1) && (sel > 0));
	GetDlgItem(IDC_BUTTON_SKILL_DOWN)->EnableWindow(!disable && (sel_count == 1) && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_SKILL_BOTTOM)->EnableWindow(!disable && (sel_count == 1) && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_SKILL_ADD)->EnableWindow(!disable && (item_count < DQMJ_SKILL_MAX));
	GetDlgItem(IDC_BUTTON_SKILL_MODIFY)->EnableWindow(!disable && (sel_count == 1) && (sel >= 0));
	GetDlgItem(IDC_BUTTON_SKILL_REMOVE)->EnableWindow(!disable && (sel >= 0));
	GetDlgItem(IDC_BUTTON_SKILL_SORT)->EnableWindow(!disable && (item_count > 0));
}

void CMonsterInfoDlg::UpdateRaceStatusSpinRange()
{
	if (IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED)
	{
		DQMJ_SAVE_RACE_SETTING rs;
		VERIFY(::DQMJSaveGetRaceSetting(m_monsterInfo.race, &rs));

		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_HP))->SetRange32(0, rs.limit_hp);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_MP))->SetRange32(0, rs.limit_mp);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_ATK))->SetRange32(0, rs.limit_atk);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_DEF))->SetRange32(0, rs.limit_def);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_AGI))->SetRange32(0, rs.limit_agi);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_INT))->SetRange32(0, rs.limit_int);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CUR_HP))->SetRange32(1, m_monsterInfo.actual_hp);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CUR_MP))->SetRange32(0, m_monsterInfo.actual_mp);

		DQMJ_SAVE_MONSTER_INFO monsterinfo = m_monsterInfo;
		VERIFY(::DQMJSaveNormalizeMonster(m_bInterrupt, m_inPartyOrStandby, &m_masterName, &monsterinfo));

		m_monsterInfo.base_hp = monsterinfo.base_hp;
		m_monsterInfo.base_mp = monsterinfo.base_mp;
		m_monsterInfo.base_atk = monsterinfo.base_atk;
		m_monsterInfo.base_def = monsterinfo.base_def;
		m_monsterInfo.base_agi = monsterinfo.base_agi;
		m_monsterInfo.base_int = monsterinfo.base_int;
		m_monsterInfo.actual_hp = monsterinfo.actual_hp;
		m_monsterInfo.actual_mp = monsterinfo.actual_mp;
		m_monsterInfo.actual_atk = monsterinfo.actual_atk;
		m_monsterInfo.actual_def = monsterinfo.actual_def;
		m_monsterInfo.actual_agi = monsterinfo.actual_agi;
		m_monsterInfo.actual_int = monsterinfo.actual_int;
		m_monsterInfo.current_hp = monsterinfo.current_hp;
		m_monsterInfo.current_mp = monsterinfo.current_mp;
	}
	else
	{
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_HP))->SetRange32(0, DQMJ_STATUS_MAX);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_MP))->SetRange32(0, DQMJ_STATUS_MAX);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_ATK))->SetRange32(0, DQMJ_STATUS_MAX);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_DEF))->SetRange32(0, DQMJ_STATUS_MAX);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_AGI))->SetRange32(0, DQMJ_STATUS_MAX);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_INT))->SetRange32(0, DQMJ_STATUS_MAX);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CUR_HP))->SetRange32(0, DQMJ_STATUS_MAX);
		((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CUR_MP))->SetRange32(0, DQMJ_STATUS_MAX);
	}
}

void CMonsterInfoDlg::CheckSkillsetNone(int idx)
{
	ASSERT((idx >= 0) && (idx < DQMJ_SKILLSET_MAX));

	UpdateData(TRUE);

	if (IsDlgButtonChecked(IDC_CHECK_SKILLSET_NONE[idx]) == BST_CHECKED)
	{
		m_nSkillsetPt[idx] = 0;
		m_cmbSkillset[idx].SetCurSel(CB_ERR);
		m_cmbSkillset[idx].EnableWindow(FALSE);
	}
	else
	{
		m_cmbSkillset[idx].SetCurSel(0);
		m_cmbSkillset[idx].EnableWindow(TRUE);
	}

	GetDlgItem(IDC_LABEL_SKILLSET_PT[idx])->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SKILLSET_PT[idx])->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_SKILLSET_PT[idx])->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SKILLSET_MAX[idx])->EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CMonsterInfoDlg::CheckAbilityNone(int idx)
{
	ASSERT((idx >= 0) && (idx < DQMJ_ABILITY_MAX));

	UpdateData(TRUE);

	if (IsDlgButtonChecked(IDC_CHECK_ABILITY_NONE[idx]) == BST_CHECKED)
	{
		m_cmbAbility[idx].SetCurSel(CB_ERR);
		m_cmbAbility[idx].EnableWindow(FALSE);
	}
	else
	{
		m_cmbAbility[idx].SetCurSel(0);
		m_cmbAbility[idx].EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

void CMonsterInfoDlg::CheckParentNone(int parent)
{
	ASSERT((parent >= 0) && (parent < DQMJ_PARENT_NUM));

	if (IsDlgButtonChecked(IDC_CHECK_PARENT_NONE[parent]) != BST_CHECKED)
	{
		m_cmbParentRace[parent].SetCurSel(0);
		UpdateData(TRUE);
		m_strParentName[parent] = _T("ふめい");
		m_strParentMaster[parent] = _T("ふめい");
		UpdateData(FALSE);
	}

	UpdateSaveData();
	RefreshMonsterInfo();
}

void CMonsterInfoDlg::CheckGrandparentNone(int parent, int grandparent)
{
	ASSERT((parent >= 0) && (parent < DQMJ_PARENT_NUM) && (grandparent >= 0) && (grandparent < DQMJ_PARENT_NUM));

	if (IsDlgButtonChecked(IDC_CHECK_GRANDPARENT_NONE[parent][grandparent]) != BST_CHECKED)
		m_cmbGrandparentRace[parent][grandparent].SetCurSel(0);

	UpdateSaveData();
	RefreshMonsterInfo();
}

void CMonsterInfoDlg::SetSkillsetMax(int idx)
{
	ASSERT((idx >= 0) && (idx < DQMJ_SKILLSET_MAX));

	DQMJ_SAVE_SKILLSET_SETTING sss;
	if (!::DQMJSaveGetSkillsetSetting(m_cmbSkillset[idx].GetItemData(m_cmbSkillset[idx].GetCurSel()), &sss))
		return;

	UpdateData(TRUE);
	m_nSkillsetPt[idx] = sss.max_point;
	UpdateData(FALSE);
}

void CMonsterInfoDlg::SetMasterName(const DQMJ_NAME *name)
{
	UpdateData(TRUE);
	m_strMaster = (name == NULL) ? _T("ふめい") : theApp.GetNameString(*name);
	UpdateData(FALSE);
}

void CMonsterInfoDlg::SetParentMasterName(int parent, const DQMJ_NAME *name)
{
	UpdateData(TRUE);
	m_strParentMaster[parent] = (name == NULL) ? _T("ふめい") : theApp.GetNameString(*name);
	UpdateData(FALSE);
}

int CMonsterInfoDlg::SelectSkill(int sel)
{
	CSelectSkillDlg dlg((sel < 0) ? 0x00 : m_lstSkill.GetItemData(sel), this);
	if (dlg.DoModal() != IDOK)
		return sel;

	int skill = dlg.GetSelSkill();
	if (skill == 0x00)
		return sel;

	for (int i = 0; i < m_lstSkill.GetCount(); i++)
		m_lstSkill.SetSel(i, FALSE);

	if (sel < 0)
	{
		sel = m_lstSkill.AddString(theApp.GetSkillString(skill));
		ASSERT(sel >= 0);
		m_lstSkill.SetItemData(sel, skill);
		m_lstSkill.SetSel(sel);
		SetDlgItemText(IDC_SKILL_NUM, theApp.GetDecimalString(m_lstSkill.GetCount(), DQMJ_SKILL_MAX));
	}
	else
	{
		m_lstSkill.DeleteString(sel);
		m_lstSkill.InsertString(sel, theApp.GetSkillString(skill));
		m_lstSkill.SetItemData(sel, skill);
		m_lstSkill.SetSel(sel);
	}

	return sel;
}

//////////////////////////////////////////////////////////////////////////

void CMonsterInfoDlg::InitRaceCBIndex()
{
	ASSERT(m_raceIDList.IsEmpty() && m_raceToCBIndex.IsEmpty());

	CList<DQMJ_SAVE_RACE_SETTING> list[DQMJ_TYPE_NUM];

	for (int i = 0; i < DQMJ_MONSTERLIST_LEN; i++)
	{
		DQMJ_SAVE_RACE_SETTING rs;
		if (::DQMJSaveGetRaceSetting(i, &rs))
			list[rs.type].AddTail(rs);
	}

	for (int i = 0; i < DQMJ_TYPE_NUM; i++)
	{
		POSITION pos = list[i].GetHeadPosition();
		while (pos != NULL)
		{
			const DQMJ_SAVE_RACE_SETTING &rs = list[i].GetNext(pos);
			m_raceToCBIndex.SetAt(rs.race, m_raceIDList.GetCount());
			m_raceIDList.AddTail(rs.race);
		}
	}
}

void CMonsterInfoDlg::InitSkillsetCBIndex()
{
	ASSERT(m_skillsetIDList.IsEmpty() && m_skillsetToCBIndex.IsEmpty());

	for (int i = 0; i < DQMJ_SKILLSETLIST_LEN; i++)
	{
		DQMJ_SAVE_SKILLSET_SETTING sss;
		if (!::DQMJSaveGetSkillsetSetting(i, &sss))
			continue;

		m_skillsetToCBIndex.SetAt(i, m_skillsetIDList.GetCount());
		m_skillsetIDList.AddTail(i);
	}
}

void CMonsterInfoDlg::InitAbilityCBIndex()
{
	ASSERT(m_abilityIDList.IsEmpty() && m_abilityToCBIndex.IsEmpty());

	for (int i = 0; i < DQMJ_ABILITYLIST_LEN; i++)
	{
		if (!::DQMJSaveGetAbilityName(i))
			continue;

		m_abilityToCBIndex.SetAt(i, m_abilityIDList.GetCount());
		m_abilityIDList.AddTail(i);
	}
}

void CMonsterInfoDlg::InitComboBox(CComboBox &combo, const int *list, int len)
{
	combo.SetRedraw(FALSE);

	for (int i = 0; i < len; i++)
		VERIFY(combo.AddString(theApp.GetDecimalString(list[i])) == i);

	combo.SetRedraw(TRUE);
}

void CMonsterInfoDlg::InitComboBox(CComboBox &combo, const CStringArray &sa)
{
	combo.SetRedraw(FALSE);

	for (int i = 0; i < sa.GetCount(); i++)
		VERIFY(combo.AddString(sa.GetAt(i)) == i);

	combo.SetRedraw(TRUE);
}

void CMonsterInfoDlg::InitComboBox(CComboBox &combo, const CIntList &list, const CIntToStrMap &map)
{
	combo.SetRedraw(FALSE);

	POSITION pos = list.GetHeadPosition();
	while (pos != NULL)
	{
		int id = list.GetNext(pos);
		CString str;
		if (map.Lookup(id, str))
			VERIFY(combo.SetItemData(combo.AddString(str), id) != CB_ERR);
	}

	combo.SetRedraw(TRUE);
}

int CMonsterInfoDlg::GetSafeCBIndex(CComboBox &combo, int idx)
{
	return ((idx >= 0) && (idx < combo.GetCount())) ? idx : CB_ERR;
}

int CMonsterInfoDlg::GetSafeCBIndex(CComboBox &combo, int id, const CIntToIntMap &map)
{
	int idx;
	if (!map.Lookup(id, idx))
		return CB_ERR;

	return GetSafeCBIndex(combo, idx);
}

//////////////////////////////////////////////////////////////////////////
