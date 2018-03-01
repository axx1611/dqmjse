
#include "stdafx.h"
#include "dqmjse.h"
#include "monsterlistdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

const int CMonsterListDlg::MAXLEVEL_LIST[]		= { 50, 75, 99 };
const LPCTSTR CMonsterListDlg::SEX_LIST[]		= { _T("♂"), _T("♀"), _T("両性具有") };
const LPCTSTR CMonsterListDlg::RANK_LIST[]		= { _T("なし"), _T("F"), _T("E"), _T("D"), _T("C"), _T("B"), _T("A"), _T("S"), _T("SS"), _T("??") };
const LPCTSTR CMonsterListDlg::TYPE_LIST[]		= { _T("なし"), _T("スライム系"), _T("ドラゴン系"), _T("しぜん系"), _T("まじゅう系"), _T("ぶっしつ系"), _T("あくま系"), _T("ゾンビ系"), _T("しんじゅう系") };
const LPCTSTR CMonsterListDlg::STRATEGY_LIST[]	= { _T("ガンガンいこうぜ"), _T("いろいろやろうぜ"), _T("いのちだいじに"), _T("とくぎつかうな") };
const LPCTSTR CMonsterListDlg::SOURCE_LIST[]	= { _T("システム"), _T("ワイヤレス交換(?)"), _T("他国マスタースカウト"), _T("野良モンスタースカウト"), _T("配合"), _T("ゲスト") };
const LPCTSTR CMonsterListDlg::GROWTH_LIST[]	= { _T("ＨＰ"), _T("ＭＰ"), _T("攻撃力"), _T("守備力"), _T("素早さ"), _T("賢さ") };
const LPCTSTR CMonsterListDlg::GUARD_LIST[]		= { _T("弱い"), _T("ふつう"), _T("軽減"), _T("半減"), _T("激減"), _T("無効"), _T("反射"), _T("吸収") };

CMonsterListDlg::CIDStrList CMonsterListDlg::m_raceIDStrList;
CMonsterListDlg::CIDStrList CMonsterListDlg::m_skillsetIDStrList;
CMonsterListDlg::CIDStrList CMonsterListDlg::m_abilityIDStrList;
int CMonsterListDlg::m_raceToCBIndex[];
int CMonsterListDlg::m_skillsetToCBIndex[];
int CMonsterListDlg::m_abilityToCBIndex[];
DQMJ_SAVE_MONSTER_INFO CMonsterListDlg::m_emptyMonster;

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMonsterListDlg, CDialog)

BEGIN_MESSAGE_MAP(CMonsterListDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_CHECK_NORMALIZE, OnBnClickedCheckNormalize)
	ON_BN_CLICKED(IDC_CHECK_MODE_LOCK, OnBnClickedCheckModeLock)
	ON_BN_CLICKED(IDC_CHECK_RANK_AUTO, OnBnClickedCheckRankAuto)
	ON_BN_CLICKED(IDC_CHECK_TYPE_AUTO, OnBnClickedCheckTypeAuto)
	ON_BN_CLICKED(IDC_CHECK_DEAD, OnBnClickedCheckDead)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_UP, OnBnClickedButtonMonsterUp)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_TOP, OnBnClickedButtonMonsterTop)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_DOWN, OnBnClickedButtonMonsterDown)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_BOTTOM, OnBnClickedButtonMonsterBottom)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ADD, OnBnClickedButtonMonsterAdd)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_REMOVE, OnBnClickedButtonMonsterRemove)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_MAX, OnBnClickedButtonStatusMax)
	ON_BN_CLICKED(IDC_BUTTON_STATUS_MAX_FORCE, OnBnClickedButtonStatusMaxForce)
	ON_BN_CLICKED(IDC_BUTTON_WEAPON_MODIFY, OnBnClickedButtonWeaponModify)
	ON_BN_CLICKED(IDC_BUTTON_WEAPON_REMOVE, OnBnClickedButtonWeaponRemove)
	ON_BN_CLICKED(IDC_BUTTON_GROWTH_RANDOM, OnBnClickedButtonGrowthRandom)
	ON_BN_CLICKED(IDC_BUTTON_SKILLSET_MAX1, OnBnClickedButtonSkillsetMax1)
	ON_BN_CLICKED(IDC_BUTTON_SKILLSET_MAX2, OnBnClickedButtonSkillsetMax2)
	ON_BN_CLICKED(IDC_BUTTON_SKILLSET_MAX3, OnBnClickedButtonSkillsetMax3)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_UP, OnBnClickedButtonSkillUp)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_TOP, OnBnClickedButtonSkillTop)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_DOWN, OnBnClickedButtonSkillDown)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_BOTTOM, OnBnClickedButtonSkillBottom)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_ADD, OnBnClickedButtonSkillAdd)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_MODIFY, OnBnClickedButtonSkillModify)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_REMOVE, OnBnClickedButtonSkillRemove)
	ON_BN_CLICKED(IDC_BUTTON_SKILL_SORT, OnBnClickedButtonSkillSort)
	ON_LBN_SELCHANGE(IDC_LIST_SKILL_LIST, OnLbnSelchangeListSkillList)
	ON_CBN_SELCHANGE(IDC_COMBO_RACE, OnCbnSelchangeComboRace)
	ON_CBN_SELCHANGE(IDC_COMBO_RANK, OnCbnSelchangeComboRank)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
	ON_CBN_SELCHANGE(IDC_COMBO_MAX_LEVEL, OnCbnSelchangeComboMaxLevel)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_MONSTER_LIST, OnLvnItemchangingListMonsterList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CMonsterListDlg::CMonsterListDlg(BOOL interrupt, LPCTSTR master_name, CWnd *pParent) : CDialog(IDD_DQMJSE_MONSTER_LIST, pParent)
{
	m_bInterrupt = interrupt;

	CDqmjSEApp::GetStringName(m_masterName, master_name);

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
	m_nSkillPoint = 0;
	m_bSelectChanging = FALSE;
	m_strName = _T("");
	m_strMaster = _T("");
	m_strParentsName[0] = _T("");
	m_strParentsName[1] = _T("");
	m_strParentsMaster[0] = _T("");
	m_strParentsMaster[1] = _T("");

	::ZeroMemory(m_normalizeFlags, sizeof(m_normalizeFlags));
	::ZeroMemory(m_nGrowthLevel, sizeof(m_nGrowthLevel));
	::ZeroMemory(m_nSkillsetPt, sizeof(m_nSkillsetPt));
	::ZeroMemory(m_monsterInfo, sizeof(m_monsterInfo));

	StaticInitialize();
}

//////////////////////////////////////////////////////////////////////////

void CMonsterListDlg::DoDataExchange(CDataExchange *pDX)
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
	::DDX_Text(pDX, IDC_EDIT_GROWTH_LEVEL_START, m_nGrowthLevel[0]);
	::DDX_Text(pDX, IDC_EDIT_GROWTH_LEVEL_END, m_nGrowthLevel[1]);
	::DDX_Text(pDX, IDC_EDIT_SKILLSET_PT1, m_nSkillsetPt[0]);
	::DDX_Text(pDX, IDC_EDIT_SKILLSET_PT2, m_nSkillsetPt[1]);
	::DDX_Text(pDX, IDC_EDIT_SKILLSET_PT3, m_nSkillsetPt[2]);
	::DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	::DDX_Text(pDX, IDC_EDIT_MASTER, m_strMaster);
	::DDX_Text(pDX, IDC_EDIT_FARTHER_NAME, m_strParentsName[0]);
	::DDX_Text(pDX, IDC_EDIT_FARTHER_MASTER, m_strParentsMaster[0]);
	::DDX_Text(pDX, IDC_EDIT_MOTHER_NAME, m_strParentsName[1]);
	::DDX_Text(pDX, IDC_EDIT_MOTHER_MASTER, m_strParentsMaster[1]);

	::DDX_Control(pDX, IDC_LIST_MONSTER_LIST, m_lstMonster);
	::DDX_Control(pDX, IDC_LIST_SKILL_LIST, m_lstSkill);
	::DDX_Control(pDX, IDC_COMBO_SEX, m_cmbSex);
	::DDX_Control(pDX, IDC_COMBO_RACE, m_cmbRace);
	::DDX_Control(pDX, IDC_COMBO_RANK, m_cmbRank);
	::DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	::DDX_Control(pDX, IDC_COMBO_STRATEGY, m_cmbStrategy);
	::DDX_Control(pDX, IDC_COMBO_SOURCE, m_cmbSource);
	::DDX_Control(pDX, IDC_COMBO_MAX_LEVEL, m_cmbMaxLevel);
	::DDX_Control(pDX, IDC_COMBO_GROWTH_TYPE, m_cmbGrowthType);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL1, m_cmbGuard[0]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL2, m_cmbGuard[1]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL3, m_cmbGuard[2]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL4, m_cmbGuard[3]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL5, m_cmbGuard[4]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL6, m_cmbGuard[5]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL7, m_cmbGuard[6]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL8, m_cmbGuard[7]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL9, m_cmbGuard[8]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL10, m_cmbGuard[9]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL11, m_cmbGuard[10]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL12, m_cmbGuard[11]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL13, m_cmbGuard[12]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL14, m_cmbGuard[13]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL15, m_cmbGuard[14]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL16, m_cmbGuard[15]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL17, m_cmbGuard[16]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL18, m_cmbGuard[17]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL19, m_cmbGuard[18]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL20, m_cmbGuard[19]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL21, m_cmbGuard[20]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL22, m_cmbGuard[21]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL23, m_cmbGuard[22]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL24, m_cmbGuard[23]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL25, m_cmbGuard[24]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL26, m_cmbGuard[25]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL27, m_cmbGuard[26]);
	::DDX_Control(pDX, IDC_COMBO_SKILLSET1, m_cmbSkillset[0]);
	::DDX_Control(pDX, IDC_COMBO_SKILLSET2, m_cmbSkillset[1]);
	::DDX_Control(pDX, IDC_COMBO_SKILLSET3, m_cmbSkillset[2]);
	::DDX_Control(pDX, IDC_COMBO_ABILITY1, m_cmbAbility[0]);
	::DDX_Control(pDX, IDC_COMBO_ABILITY2, m_cmbAbility[1]);
	::DDX_Control(pDX, IDC_COMBO_ABILITY3, m_cmbAbility[2]);
	::DDX_Control(pDX, IDC_COMBO_ABILITY4, m_cmbAbility[3]);
	::DDX_Control(pDX, IDC_COMBO_ABILITY5, m_cmbAbility[4]);
	::DDX_Control(pDX, IDC_COMBO_ABILITY6, m_cmbAbility[5]);
	::DDX_Control(pDX, IDC_COMBO_FARTHER_RACE, m_cmbParentsRace[0]);
	::DDX_Control(pDX, IDC_COMBO_FARTHER_FARTHER_RACE, m_cmbGrandparentsRace[0][0]);
	::DDX_Control(pDX, IDC_COMBO_FARTHER_MOTHER_RACE, m_cmbGrandparentsRace[0][1]);
	::DDX_Control(pDX, IDC_COMBO_MOTHER_RACE, m_cmbParentsRace[1]);
	::DDX_Control(pDX, IDC_COMBO_MOTHER_FARTHER_RACE, m_cmbGrandparentsRace[1][0]);
	::DDX_Control(pDX, IDC_COMBO_MOTHER_MOTHER_RACE, m_cmbGrandparentsRace[1][1]);

	CDialog::DoDataExchange(pDX);
}

BOOL CMonsterListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckDlgButton(IDC_CHECK_MODE_LOCK, BST_CHECKED);

	DQMJ_SAVE_PLAY_INFO playerinfo;
	VERIFY(::DQMJSaveQueryPlayInfo(CDqmjSEApp::GetSaveHandle(), &playerinfo));

	VERIFY(::DQMJSaveQueryRanchInfo(CDqmjSEApp::GetSaveHandle(), &m_ranchInfo));
	SetDlgItemText(IDC_MONSTER_NUM, GetMonsterNumString(m_ranchInfo.monster_num));

	m_lstMonster.SetExtendedStyle(m_lstMonster.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	VERIFY(m_lstMonster.InsertColumn(COLUMN_NO, _T("No."), LVCFMT_LEFT, 20) == COLUMN_NO);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_NAME, _T("名前"), LVCFMT_LEFT, 64) == COLUMN_NAME);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_PLACE, _T("場所"), LVCFMT_CENTER, 28) == COLUMN_PLACE);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_SEX, _T("性別"), LVCFMT_CENTER, 28) == COLUMN_SEX);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_RACE, _T("種族"), LVCFMT_LEFT, 88) == COLUMN_RACE);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_RANK, _T("RANK"), LVCFMT_CENTER, 28) == COLUMN_RANK);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_TYPE, _T("系統"), LVCFMT_LEFT, 64) == COLUMN_TYPE);

	for (int i = 0; i < m_ranchInfo.monster_num; i++)
	{
		VERIFY(m_lstMonster.InsertItem(i, _T("")) == i);
		VERIFY(::DQMJSaveQueryMonsterInfo(CDqmjSEApp::GetSaveHandle(), i, &m_monsterInfo[i]));

		m_lstMonster.SetItemText(i, COLUMN_NO, CDqmjSEApp::GetDecimalString(i + 1));
		m_lstMonster.SetItemText(i, COLUMN_NAME, CDqmjSEApp::GetNameString(m_monsterInfo[i].name));
		m_lstMonster.SetItemText(i, COLUMN_SEX, GetSafeCBString(m_monsterInfo[i].sex, SEX_LIST, _countof(SEX_LIST)));
		m_lstMonster.SetItemText(i, COLUMN_RACE, GetRaceString(m_monsterInfo[i].race));
		m_lstMonster.SetItemText(i, COLUMN_RANK, GetSafeCBString(m_monsterInfo[i].rank, RANK_LIST, _countof(RANK_LIST)));
		m_lstMonster.SetItemText(i, COLUMN_TYPE, GetSafeCBString(m_monsterInfo[i].type, TYPE_LIST, _countof(TYPE_LIST)));
	}

	for (int i = 0; i < DQMJ_MEMBER_MAX; i++)
	{
		m_lstMonster.SetItemText(m_ranchInfo.party_member_idx[i], COLUMN_PLACE, _T("パーティー"));
		m_lstMonster.SetItemText(m_ranchInfo.standby_idx[i], COLUMN_PLACE, _T("スタンバイ"));
	}

	InitComboBox(m_cmbSex, SEX_LIST, _countof(SEX_LIST));
	InitComboBox(m_cmbRace, m_raceIDStrList);
	InitComboBox(m_cmbRank, RANK_LIST, _countof(RANK_LIST));
	InitComboBox(m_cmbType, TYPE_LIST, _countof(TYPE_LIST));
	InitComboBox(m_cmbStrategy, STRATEGY_LIST, _countof(STRATEGY_LIST));
	InitComboBox(m_cmbSource, SOURCE_LIST, _countof(SOURCE_LIST));
	InitComboBox(m_cmbGrowthType, GROWTH_LIST, _countof(GROWTH_LIST));
	InitComboBox(m_cmbMaxLevel, MAXLEVEL_LIST, _countof(MAXLEVEL_LIST));
	InitComboBox(m_cmbParentsRace[0], m_raceIDStrList);
	InitComboBox(m_cmbParentsRace[1], m_raceIDStrList);
	InitComboBox(m_cmbGrandparentsRace[0][0], m_raceIDStrList);
	InitComboBox(m_cmbGrandparentsRace[0][1], m_raceIDStrList);
	InitComboBox(m_cmbGrandparentsRace[1][0], m_raceIDStrList);
	InitComboBox(m_cmbGrandparentsRace[1][1], m_raceIDStrList);

	for (int i = 0; i < _countof(m_cmbSkillset); i++)
		InitComboBox(m_cmbSkillset[i], m_skillsetIDStrList);
	for (int i = 0; i < _countof(m_cmbAbility); i++)
		InitComboBox(m_cmbAbility[i], m_abilityIDStrList);
	for (int i = 0; i < _countof(m_cmbGuard); i++)
		InitComboBox(m_cmbGuard[i], GUARD_LIST, _countof(GUARD_LIST));

	SetDlgItemText(IDC_GUARD_NAME1, ::DQMJSaveGetGuardName(0));
	SetDlgItemText(IDC_GUARD_NAME2, ::DQMJSaveGetGuardName(1));
	SetDlgItemText(IDC_GUARD_NAME3, ::DQMJSaveGetGuardName(2));
	SetDlgItemText(IDC_GUARD_NAME4, ::DQMJSaveGetGuardName(3));
	SetDlgItemText(IDC_GUARD_NAME5, ::DQMJSaveGetGuardName(4));
	SetDlgItemText(IDC_GUARD_NAME6, ::DQMJSaveGetGuardName(5));
	SetDlgItemText(IDC_GUARD_NAME7, ::DQMJSaveGetGuardName(6));
	SetDlgItemText(IDC_GUARD_NAME8, ::DQMJSaveGetGuardName(7));
	SetDlgItemText(IDC_GUARD_NAME9, ::DQMJSaveGetGuardName(8));
	SetDlgItemText(IDC_GUARD_NAME10, ::DQMJSaveGetGuardName(9));
	SetDlgItemText(IDC_GUARD_NAME11, ::DQMJSaveGetGuardName(10));
	SetDlgItemText(IDC_GUARD_NAME12, ::DQMJSaveGetGuardName(11));
	SetDlgItemText(IDC_GUARD_NAME13, ::DQMJSaveGetGuardName(12));
	SetDlgItemText(IDC_GUARD_NAME14, ::DQMJSaveGetGuardName(13));
	SetDlgItemText(IDC_GUARD_NAME15, ::DQMJSaveGetGuardName(14));
	SetDlgItemText(IDC_GUARD_NAME16, ::DQMJSaveGetGuardName(15));
	SetDlgItemText(IDC_GUARD_NAME17, ::DQMJSaveGetGuardName(16));
	SetDlgItemText(IDC_GUARD_NAME18, ::DQMJSaveGetGuardName(17));
	SetDlgItemText(IDC_GUARD_NAME19, ::DQMJSaveGetGuardName(18));
	SetDlgItemText(IDC_GUARD_NAME20, ::DQMJSaveGetGuardName(19));
	SetDlgItemText(IDC_GUARD_NAME21, ::DQMJSaveGetGuardName(20));
	SetDlgItemText(IDC_GUARD_NAME22, ::DQMJSaveGetGuardName(21));
	SetDlgItemText(IDC_GUARD_NAME23, ::DQMJSaveGetGuardName(22));
	SetDlgItemText(IDC_GUARD_NAME24, ::DQMJSaveGetGuardName(23));
	SetDlgItemText(IDC_GUARD_NAME25, ::DQMJSaveGetGuardName(24));
	SetDlgItemText(IDC_GUARD_NAME26, ::DQMJSaveGetGuardName(25));
	SetDlgItemText(IDC_GUARD_NAME27, ::DQMJSaveGetGuardName(26));

	((CEdit *)GetDlgItem(IDC_EDIT_NAME))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_MASTER))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_FARTHER_NAME))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_FARTHER_MASTER))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_MOTHER_NAME))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_MOTHER_MASTER))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_COMBINE))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_COMBINE_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_LEVEL))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_LEVEL_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_MAX_LEVEL))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_LEVEL_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_HP))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_MP))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_ATK))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_DEF))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_AGI))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_BASE_INT))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_ATK))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_DEF))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_AGI))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_INT))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_MAX_HP))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_MAX_MP))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_CUR_HP))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_CUR_MP))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_STATUS_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_SKILL_POINT))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_SKILLPOINT_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_EXP))->SetLimitText(CDqmjSEApp::CalcDigit(INT_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_GROWTH_LEVEL_START))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_LEVEL_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_GROWTH_LEVEL_END))->SetLimitText(CDqmjSEApp::CalcDigit(DQMJ_LEVEL_MAX));

	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_COMBINE))->SetRange32(0, DQMJ_COMBINE_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_LEVEL))->SetRange32(0, DQMJ_LEVEL_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_MAX_LEVEL))->SetRange32(0, DQMJ_LEVEL_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_HP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_MP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_ATK))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_DEF))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_AGI))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_BASE_INT))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_ATK))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_DEF))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_AGI))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_INT))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_MAX_HP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_MAX_MP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CUR_HP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_CUR_MP))->SetRange32(0, DQMJ_STATUS_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_SKILL_POINT))->SetRange32(0, DQMJ_SKILLPOINT_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_EXP))->SetRange32(0, INT_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_GROWTH_LEVEL_START))->SetRange32(0, DQMJ_LEVEL_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_GROWTH_LEVEL_END))->SetRange32(0, DQMJ_LEVEL_MAX);

	RefreshMonster(-1);
	return TRUE;
}

void CMonsterListDlg::OnOK()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	UpdateSaveData(cur_sel);
	RefreshMonsterInfo(cur_sel);
}

void CMonsterListDlg::OnCancel()
{
	// TODO:

	CDialog::OnCancel();
}

void CMonsterListDlg::OnBnClickedButtonCopy()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonPaste()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedCheckNormalize()
{
	int cur_sel = GetCurSel();
	int start = cur_sel;
	int count = (cur_sel >= 0) ? 1 : 0;

	if (IsDlgButtonChecked(IDC_CHECK_MODE_LOCK) == BST_CHECKED)
	{
		start = 0;
		count = _countof(m_monsterInfo);
	}

	for (int i = start; i < start + count; i++)
	{
		if (IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED)
		{
			m_normalizeFlags[i] = TRUE;
			::DQMJSaveNormalizeMonster(m_bInterrupt, IsInPartyOrStandby(i), m_masterName, &m_monsterInfo[i], &m_monsterInfo[i]);
		}
		else
		{
			m_normalizeFlags[i] = FALSE;
		}
	}

	RefreshMonster(cur_sel);
}

void CMonsterListDlg::OnBnClickedCheckModeLock()
{
	if (!IsDlgButtonChecked(IDC_CHECK_MODE_LOCK))
	{
		if (GetCurSel() < 0)
			GetDlgItem(IDC_CHECK_NORMALIZE)->EnableWindow(FALSE);

		return;
	}

	for (int i = 0; i < _countof(m_monsterInfo); i++)
	{
		if (IsDlgButtonChecked(IDC_CHECK_NORMALIZE) == BST_CHECKED)
		{
			m_normalizeFlags[i] = TRUE;
			::DQMJSaveNormalizeMonster(m_bInterrupt, IsInPartyOrStandby(i), m_masterName, &m_monsterInfo[i], &m_monsterInfo[i]);
		}
		else
		{
			m_normalizeFlags[i] = FALSE;
		}
	}

	GetDlgItem(IDC_CHECK_NORMALIZE)->EnableWindow(TRUE);
}

void CMonsterListDlg::OnBnClickedCheckRankAuto()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	DQMJ_SAVE_RACE_SETTING rs;
	if (::DQMJSaveGetRaceSetting(m_monsterInfo[cur_sel].race, &rs))
	{
		m_cmbRank.SetCurSel(GetSafeCBIndex(rs.rank, _countof(RANK_LIST)));
		GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(FALSE);
	}
	else
	{
		m_cmbRank.SetCurSel(CB_ERR);
	}
}

void CMonsterListDlg::OnBnClickedCheckTypeAuto()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	DQMJ_SAVE_RACE_SETTING rs;
	if (::DQMJSaveGetRaceSetting(m_monsterInfo[cur_sel].race, &rs))
	{
		m_cmbType.SetCurSel(GetSafeCBIndex(rs.type, _countof(TYPE_LIST)));
		GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(FALSE);
	}
	else
	{
		m_cmbType.SetCurSel(CB_ERR);
	}
}

void CMonsterListDlg::OnBnClickedCheckDead()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	if (!m_normalizeFlags[cur_sel])
		return;

	if (IsDlgButtonChecked(IDC_CHECK_DEAD) == BST_CHECKED)
	{
		m_nCurHp = 0;
		UpdateData(FALSE);

		GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(FALSE);
	}
	else
	{
		m_nCurHp = max(m_monsterInfo[cur_sel].current_hp, 1);
		UpdateData(FALSE);

		GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(TRUE);
	}
}

void CMonsterListDlg::OnBnClickedButtonMonsterUp()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonMonsterTop()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonMonsterDown()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonMonsterBottom()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonMonsterAdd()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonMonsterRemove()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonStatusMax()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonStatusMaxForce()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonWeaponModify()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonWeaponRemove()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonGrowthRandom()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	m_cmbGrowthType.SetCurSel(CB_ERR);
	SetDlgItemInt(IDC_EDIT_GROWTH_LEVEL_START, DQMJ_RAND_GROWTH_LEVEL);
	SetDlgItemInt(IDC_EDIT_GROWTH_LEVEL_END, DQMJ_RAND_GROWTH_LEVEL);

	UpdateSaveData(cur_sel);
	RefreshMonsterInfo(cur_sel);
}

void CMonsterListDlg::OnBnClickedButtonSkillsetMax1()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillsetMax2()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillsetMax3()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillUp()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillTop()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillDown()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillBottom()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillAdd()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillModify()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillRemove()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnBnClickedButtonSkillSort()
{
	// TODO: Add your control notification handler code here
}

void CMonsterListDlg::OnLbnSelchangeListSkillList()
{
	RefreshSkillSel(m_lstSkill.GetCurSel());
}

void CMonsterListDlg::OnCbnSelchangeComboRace()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	UpdateSaveData(cur_sel);
	RefreshMonsterInfo(cur_sel);
}

void CMonsterListDlg::OnCbnSelchangeComboRank()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	UpdateSaveData(cur_sel);
	RefreshMonsterInfo(cur_sel);
}

void CMonsterListDlg::OnCbnSelchangeComboType()
{
	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	UpdateSaveData(cur_sel);
	RefreshMonsterInfo(cur_sel);
}

void CMonsterListDlg::OnCbnSelchangeComboMaxLevel()
{
	UpdateData(TRUE);
	m_nMaxLevel = MAXLEVEL_LIST[m_cmbMaxLevel.GetCurSel()];
	UpdateData(FALSE);
}

void CMonsterListDlg::OnLvnItemchangingListMonsterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if ((pNMLV->uOldState & LVIS_FOCUSED) && !(pNMLV->uNewState & LVIS_FOCUSED))
		m_bSelectChanging = TRUE;

	if (!((pNMLV->uOldState ^ pNMLV->uNewState) & LVIS_SELECTED))
		return;

	if (pNMLV->uOldState & LVIS_SELECTED)
		UpdateSaveData(pNMLV->iItem);

	if (pNMLV->uNewState & LVIS_SELECTED)
		m_bSelectChanging = FALSE;

	if (m_bSelectChanging)
	{
		m_bSelectChanging = FALSE;
		return;
	}

	int cur_sel = (pNMLV->uNewState & LVIS_SELECTED) ? pNMLV->iItem : -1;
	if (cur_sel >= 0)
		CheckDlgButton(IDC_CHECK_NORMALIZE, m_normalizeFlags[cur_sel] ? BST_CHECKED : BST_UNCHECKED);
	else if (IsDlgButtonChecked(IDC_CHECK_MODE_LOCK) != BST_CHECKED)
		CheckDlgButton(IDC_CHECK_NORMALIZE, BST_UNCHECKED);

	RefreshMonster(cur_sel);
}

//////////////////////////////////////////////////////////////////////////

int CMonsterListDlg::GetCurSel()
{
	if (m_lstMonster.GetSelectedCount() <= 0)
		return -1;

	return m_lstMonster.GetSelectionMark();
}

void CMonsterListDlg::UpdateSaveData(int sel)
{
	if (sel < 0)
		return;

	UpdateData(TRUE);

	DQMJ_SAVE_MONSTER_INFO &monster = m_monsterInfo[sel];

	monster.cheat = IsDlgButtonChecked(IDC_CHECK_CHEAT) == BST_CHECKED;
	monster.dead = IsDlgButtonChecked(IDC_CHECK_DEAD) == BST_CHECKED;
	monster.poison = IsDlgButtonChecked(IDC_CHECK_POISON) == BST_CHECKED;

	CDqmjSEApp::GetStringName(monster.name, m_strName);
	CDqmjSEApp::GetStringName(monster.master, m_strMaster);

	monster.race = m_cmbRace.GetItemData(m_cmbRace.GetCurSel());
	monster.sex = m_cmbSex.GetCurSel();
	monster.rank = m_cmbRank.GetCurSel();
	monster.type = m_cmbType.GetCurSel();
	monster.combine = m_nCombine;
	monster.source = m_cmbSource.GetCurSel();
	monster.level = m_nLevel;
	monster.max_level = m_nMaxLevel;
	monster.base_hp = m_nBaseHp;
	monster.base_mp = m_nBaseMp;
	monster.base_atk = m_nBaseAtk;
	monster.base_def = m_nBaseDef;
	monster.base_agi = m_nBaseAgi;
	monster.base_int = m_nBaseInt;
	monster.current_hp = m_nCurHp;
	monster.current_mp = m_nCurMp;
	monster.actual_hp = m_nMaxHp;
	monster.actual_mp = m_nMaxMp;
	monster.actual_atk = m_nAtk;
	monster.actual_def = m_nDef;
	monster.actual_agi = m_nAgi;
	monster.actual_int = m_nInt;
	monster.exp = m_nExp;
	monster.strategy = m_cmbStrategy.GetCurSel();
	monster.growth_type = m_cmbGrowthType.GetCurSel();
	monster.growth_level[0] = m_nGrowthLevel[0];
	monster.growth_level[1] = m_nGrowthLevel[1];
#if 0
	int					weapon;									/* 装備している武器 */
	int					skillset[DQMJ_SKILLSET_MAX];			/* モンスタースキル */
	int					assigned_sp[DQMJ_SKILLSET_MAX];			/* 各スキルに振り分けたポイント */
	int					skill_active_num[DQMJ_SKILLSET_MAX];	/* 各スキルの開放特技数（restrict時は無視される） */
	int					remaining_sp;							/* 残りスキルポイント */
	int					guard_info[DQMJ_GUARD_MAX];				/* ガード情報（restrict時は無視される） */
	int					skill_list[DQMJ_SKILL_MAX];				/* 習得特技リスト（restrict時は無視される） */
	int					ability_list[DQMJ_ABILITY_MAX];			/* 特性リスト（restrict時は無視される） */
	int					parents_race[2];						/* 両親の種別 */
	DQMJ_NAME			parents_name[2];						/* 両親の名前 */
	DQMJ_NAME			parents_master[2];						/* 両親の作成マスターの名前 */
	int					grandparents_race[2][2];	
#endif

	if (m_normalizeFlags[sel])
	{
		::DQMJSaveNormalizeMonster(m_bInterrupt, IsInPartyOrStandby(sel), m_masterName, &monster, &monster);
		return;
	}

	if ((monster.growth_type == DQMJ_RAND_GROWTH_TYPE) || (monster.growth_level[0] == DQMJ_RAND_GROWTH_LEVEL) || (monster.growth_level[1] == DQMJ_RAND_GROWTH_LEVEL))
	{
		DQMJ_SAVE_MONSTER_INFO m;
		::DQMJSaveNormalizeMonster(m_bInterrupt, IsInPartyOrStandby(sel), m_masterName, &monster, &m);
		monster.growth_type = m.growth_type;
		monster.growth_level[0] = m.growth_level[0];
		monster.growth_level[1] = m.growth_level[1];
	}
}

void CMonsterListDlg::RefreshMonster(int sel)
{
	BOOL enable = sel >= 0;
	BOOL normalize = enable ? m_normalizeFlags[sel] : IsDlgButtonChecked(IDC_CHECK_NORMALIZE);
	BOOL freeModeOnly = enable && !normalize;
	BOOL interruptOnly = enable && (!normalize || m_bInterrupt);

	m_cmbMaxLevel.ShowWindow(normalize ? SW_SHOWNOACTIVATE : SW_HIDE);
	GetDlgItem(IDC_EDIT_MAX_LEVEL)->ShowWindow(normalize ? SW_HIDE : SW_SHOWNOACTIVATE);
	GetDlgItem(IDC_SPIN_MAX_LEVEL)->ShowWindow(normalize ? SW_HIDE : SW_SHOWNOACTIVATE);

	m_cmbSex.EnableWindow(enable);
	m_cmbRace.EnableWindow(enable);
	m_cmbStrategy.EnableWindow(enable);
	m_cmbSource.EnableWindow(enable);
	m_cmbMaxLevel.EnableWindow(enable);

	m_lstSkill.EnableWindow(freeModeOnly);

	for (int i = 0; i < _countof(m_cmbGuard); i++)
		m_cmbGuard[i].EnableWindow(freeModeOnly);

	if (IsDlgButtonChecked(IDC_CHECK_MODE_LOCK) == BST_CHECKED)
		GetDlgItem(IDC_CHECK_NORMALIZE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_CHECK_NORMALIZE)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_INFORMATION)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_BASIC)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_SEX)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MASTER)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_RANK)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_TYPE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_STRATEGY)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_SOURCE)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_MASTER)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_BUTTON_MASTER_ME)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_BUTTON_MASTER_UNKNOWN)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_CHEAT)->EnableWindow(freeModeOnly);

	GetDlgItem(IDC_GROUP_STATUS)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_LEVEL)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_COMBINE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BASE_HP)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BASE_MP)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BASE_ATK)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BASE_DEF)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BASE_AGI)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BASE_INT)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MAX_LEVEL)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MAX_HP)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MAX_MP)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_ATK)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_DEF)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_AGI)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_INT)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_CUR_HP)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_CUR_MP)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_SKILL_POINT)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_EXP)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_LEVEL)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_COMBINE)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_BASE_HP)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_BASE_MP)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_BASE_ATK)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_BASE_DEF)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_BASE_AGI)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_BASE_INT)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_MAX_LEVEL)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_MAX_HP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_MAX_MP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_ATK)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_DEF)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_AGI)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_INT)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_EDIT_CUR_MP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_EDIT_SKILL_POINT)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_EXP)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_LEVEL)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_COMBINE)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_BASE_HP)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_BASE_MP)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_BASE_ATK)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_BASE_DEF)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_BASE_AGI)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_BASE_INT)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_MAX_LEVEL)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_MAX_HP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_MAX_MP)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_ATK)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_DEF)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_AGI)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_INT)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_SPIN_CUR_MP)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_SPIN_SKILL_POINT)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_EXP)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_STATUS_MAX)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_STATUS_MAX_FORCE)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_DEAD)->EnableWindow(interruptOnly);
	GetDlgItem(IDC_CHECK_POISON)->EnableWindow(freeModeOnly);

	GetDlgItem(IDC_GROUP_WEAPON)->EnableWindow(enable);
	GetDlgItem(IDC_WEAPON_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_WEAPON_MODIFY)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_GROWTH)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_GROWTH_TYPE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_GROWTH_LEVEL_START)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_GROWTH_LEVEL_END)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_GROWTH_RANDOM)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_SKILLSET)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLSET_NONE1)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLSET_NONE2)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLSET_NONE3)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_ABILITY)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_ABILITY_NONE1)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_ABILITY_NONE2)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_ABILITY_NONE3)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_ABILITY_NONE4)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_ABILITY_NONE5)->EnableWindow(freeModeOnly);
	GetDlgItem(IDC_CHECK_ABILITY_NONE6)->EnableWindow(freeModeOnly);

	GetDlgItem(IDC_GROUP_SKILL)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_SKILL_NUM)->EnableWindow(enable);
	GetDlgItem(IDC_SKILL_NUM)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_GUARD)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME1)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME2)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME3)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME4)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME5)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME6)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME7)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME8)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME9)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME10)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME11)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME12)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME13)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME14)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME15)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME16)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME17)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME18)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME19)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME20)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME21)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME22)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME23)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME24)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME25)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME26)->EnableWindow(enable);
	GetDlgItem(IDC_GUARD_NAME27)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_FARTHER)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_FARTHER_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_FARTHER_MASTER)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_FARTHER_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_FARTHER_FARTHER_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_FARTHER_MOTHER_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_FARTHER_NONE)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_MOTHER)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MOTHER_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MOTHER_MASTER)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MOTHER_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MOTHER_FARTHER_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_MOTHER_MOTHER_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_MOTHER_NONE)->EnableWindow(enable);

	RefreshMonsterSel(sel);

	if (sel < 0)
		RefreshMonsterInfo(m_emptyMonster, FALSE);
	else
		RefreshMonsterInfo(m_monsterInfo[sel], normalize);

	RefreshSkillSel(LB_ERR, !enable || normalize);
}

void CMonsterListDlg::RefreshMonsterSel(int sel)
{
	int item_count = m_lstMonster.GetItemCount();

	GetDlgItem(IDC_BUTTON_MONSTER_UP)->EnableWindow(sel > 0);
	GetDlgItem(IDC_BUTTON_MONSTER_TOP)->EnableWindow(sel > 0);
	GetDlgItem(IDC_BUTTON_MONSTER_DOWN)->EnableWindow((sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_MONSTER_BOTTOM)->EnableWindow((sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_MONSTER_ADD)->EnableWindow(item_count < DQMJ_MONSTER_MAX);
	GetDlgItem(IDC_BUTTON_MONSTER_REMOVE)->EnableWindow(sel >= 0);
}

void CMonsterListDlg::RefreshMonsterInfo(int sel)
{
	if ((sel >= 0) && (sel < DQMJ_MONSTER_MAX))
		RefreshMonsterInfo(m_monsterInfo[sel], m_normalizeFlags[sel]);
}

void CMonsterListDlg::RefreshMonsterInfo(DQMJ_SAVE_MONSTER_INFO &monster, BOOL normalize)
{
	static const int skillset_ctrl_id[][5] =
	{
		{ IDC_CHECK_SKILLSET_NONE1, IDC_LABEL_SKILLSET_PT1, IDC_EDIT_SKILLSET_PT1, IDC_SPIN_SKILLSET_PT1, IDC_BUTTON_SKILLSET_MAX1 },
		{ IDC_CHECK_SKILLSET_NONE2, IDC_LABEL_SKILLSET_PT2, IDC_EDIT_SKILLSET_PT2, IDC_SPIN_SKILLSET_PT2, IDC_BUTTON_SKILLSET_MAX2 },
		{ IDC_CHECK_SKILLSET_NONE3, IDC_LABEL_SKILLSET_PT3, IDC_EDIT_SKILLSET_PT3, IDC_SPIN_SKILLSET_PT3, IDC_BUTTON_SKILLSET_MAX3 },
	};

	static const int ability_none_id[] =
	{
		IDC_CHECK_ABILITY_NONE1, IDC_CHECK_ABILITY_NONE2, IDC_CHECK_ABILITY_NONE3, IDC_CHECK_ABILITY_NONE4, IDC_CHECK_ABILITY_NONE5, IDC_CHECK_ABILITY_NONE6,
	};

	static const int parents_ctrl_id[][7] =
	{
		{ IDC_CHECK_FARTHER_NONE, IDC_CHECK_FARTHER_FARTHER_NONE, IDC_CHECK_FARTHER_MOTHER_NONE, IDC_EDIT_FARTHER_NAME, IDC_EDIT_FARTHER_MASTER, IDC_BUTTON_FARTHER_MASTER_ME, IDC_BUTTON_FARTHER_MASTER_UNKNOWN },
		{ IDC_CHECK_MOTHER_NONE, IDC_CHECK_MOTHER_FARTHER_NONE, IDC_CHECK_MOTHER_MOTHER_NONE, IDC_EDIT_MOTHER_NAME, IDC_EDIT_MOTHER_MASTER, IDC_BUTTON_MOTHER_MASTER_ME, IDC_BUTTON_MOTHER_MASTER_UNKNOWN },
	};

	m_nLevel = monster.level;
	m_nCombine = monster.combine;
	m_nBaseHp = monster.base_hp;
	m_nBaseMp = monster.base_mp;
	m_nBaseAtk = monster.base_atk;
	m_nBaseDef = monster.base_def;
	m_nBaseAgi = monster.base_agi;
	m_nBaseInt = monster.base_int;
	m_nAtk = monster.actual_atk;
	m_nDef = monster.actual_def;
	m_nAgi = monster.actual_agi;
	m_nInt = monster.actual_int;
	m_nMaxLevel = monster.max_level;
	m_nMaxHp = monster.actual_hp;
	m_nMaxMp = monster.actual_mp;
	m_nCurHp = monster.current_hp;
	m_nCurMp = monster.current_mp;
	m_nExp = monster.exp;
	m_nSkillPoint = monster.remaining_sp;

	m_strName = CDqmjSEApp::GetNameString(monster.name);
	m_strMaster = CDqmjSEApp::GetNameString(monster.master);

	CheckDlgButton(IDC_CHECK_CHEAT, monster.cheat ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_DEAD, monster.dead ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_POISON, monster.poison ? BST_CHECKED : BST_UNCHECKED);

	SetDlgItemText(IDC_WEAPON_NAME, GetEquipmentNameString(monster.weapon));
	GetDlgItem(IDC_BUTTON_WEAPON_REMOVE)->EnableWindow(monster.weapon != 0x00);

	m_cmbSex.SetCurSel(GetSafeCBIndex(monster.sex, _countof(SEX_LIST)));
	m_cmbRace.SetCurSel(GetSafeCBIndex(monster.race, m_raceToCBIndex, _countof(m_raceToCBIndex)));
	m_cmbStrategy.SetCurSel(GetSafeCBIndex(monster.strategy, _countof(STRATEGY_LIST)));
	m_cmbSource.SetCurSel(GetSafeCBIndex(monster.source, _countof(SOURCE_LIST)));
	m_cmbMaxLevel.SetCurSel(CB_ERR);
	m_lstSkill.ResetContent();

	DQMJ_SAVE_RACE_SETTING rs;
	if (::DQMJSaveGetRaceSetting(monster.race, &rs))
	{
		GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(!normalize);
		GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(!normalize);

		m_cmbRank.EnableWindow(!normalize);
		m_cmbType.EnableWindow(!normalize);
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

	if (normalize && monster.dead)
	{
		GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CUR_MP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_CUR_MP)->EnableWindow(FALSE);
	}

	if (monster.rank == DQMJ_AUTO_RANK)
	{
		CheckDlgButton(IDC_CHECK_RANK_AUTO, BST_CHECKED);
		GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(FALSE);

		m_cmbRank.SetCurSel(GetSafeCBIndex(rs.rank, _countof(RANK_LIST)));
	}
	else
	{
		if (monster.rank == rs.rank)
		{
			CheckDlgButton(IDC_CHECK_RANK_AUTO, BST_CHECKED);
			GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(FALSE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_RANK_AUTO, BST_UNCHECKED);
		}

		m_cmbRank.SetCurSel(GetSafeCBIndex(monster.rank, _countof(RANK_LIST)));
	}

	if (monster.type == DQMJ_AUTO_TYPE)
	{
		CheckDlgButton(IDC_CHECK_TYPE_AUTO, BST_CHECKED);
		GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(FALSE);

		m_cmbType.SetCurSel(GetSafeCBIndex(rs.type, _countof(TYPE_LIST)));
	}
	else
	{
		if (monster.type == rs.type)
		{
			CheckDlgButton(IDC_CHECK_TYPE_AUTO, BST_CHECKED);
			GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(FALSE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_TYPE_AUTO, BST_UNCHECKED);
		}

		m_cmbType.SetCurSel(GetSafeCBIndex(monster.type, _countof(TYPE_LIST)));
	}

	if (monster.growth_type == DQMJ_RAND_GROWTH_TYPE)
	{
		m_cmbGrowthType.EnableWindow(FALSE);
		m_cmbGrowthType.SetCurSel(CB_ERR);
	}
	else
	{
		m_cmbGrowthType.EnableWindow(TRUE);
		m_cmbGrowthType.SetCurSel(GetSafeCBIndex(monster.growth_type, _countof(GROWTH_LIST)));
	}

	if ((monster.growth_level[0] == DQMJ_RAND_GROWTH_LEVEL) || (monster.growth_level[1] == DQMJ_RAND_GROWTH_LEVEL))
	{
		m_nGrowthLevel[0] = 0;
		m_nGrowthLevel[1] = 0;

		GetDlgItem(IDC_EDIT_GROWTH_LEVEL_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GROWTH_LEVEL_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_GROWTH_LEVEL_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_GROWTH_LEVEL_END)->EnableWindow(FALSE);
	}
	else
	{
		m_nGrowthLevel[0] = monster.growth_level[0];
		m_nGrowthLevel[1] = monster.growth_level[1];

		GetDlgItem(IDC_EDIT_GROWTH_LEVEL_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GROWTH_LEVEL_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_GROWTH_LEVEL_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_GROWTH_LEVEL_END)->EnableWindow(TRUE);
	}

	for (int i = 0; i < _countof(m_cmbGuard); i++)
		m_cmbGuard[i].SetCurSel(GetSafeCBIndex(monster.guard_info[i], _countof(GUARD_LIST)));

	for (int i = 0; i < _countof(MAXLEVEL_LIST); i++)
	{
		if (MAXLEVEL_LIST[i] == monster.max_level)
		{
			m_cmbMaxLevel.SetCurSel(i);
			break;
		}
	}

	for (int i = 0; i < _countof(m_cmbSkillset); i++)
	{
		m_nSkillsetPt[i] = monster.assigned_sp[i];

		if (monster.skillset[i] == 0x00)
		{
			CheckDlgButton(skillset_ctrl_id[i][0], BST_CHECKED);

			GetDlgItem(skillset_ctrl_id[i][1])->EnableWindow(FALSE);
			GetDlgItem(skillset_ctrl_id[i][2])->EnableWindow(FALSE);
			GetDlgItem(skillset_ctrl_id[i][3])->EnableWindow(FALSE);
			GetDlgItem(skillset_ctrl_id[i][4])->EnableWindow(FALSE);

			m_cmbSkillset[i].EnableWindow(FALSE);
			m_cmbSkillset[i].SetCurSel(CB_ERR);
		}
		else
		{
			CheckDlgButton(skillset_ctrl_id[i][0], BST_UNCHECKED);

			GetDlgItem(skillset_ctrl_id[i][1])->EnableWindow(TRUE);
			GetDlgItem(skillset_ctrl_id[i][2])->EnableWindow(TRUE);
			GetDlgItem(skillset_ctrl_id[i][3])->EnableWindow(TRUE);
			GetDlgItem(skillset_ctrl_id[i][4])->EnableWindow(TRUE);

			m_cmbSkillset[i].EnableWindow(TRUE);
			m_cmbSkillset[i].SetCurSel(GetSafeCBIndex(monster.skillset[i], m_skillsetToCBIndex, _countof(m_skillsetToCBIndex)));
		}
	}

	for (int i = 0; i < _countof(m_cmbAbility); i++)
	{
		if (monster.ability_list[i] == 0x00)
		{
			CheckDlgButton(ability_none_id[i], BST_CHECKED);

			m_cmbAbility[i].EnableWindow(FALSE);
			m_cmbAbility[i].SetCurSel(CB_ERR);
		}
		else
		{
			CheckDlgButton(ability_none_id[i], BST_UNCHECKED);

			m_cmbAbility[i].EnableWindow(!normalize);
			m_cmbAbility[i].SetCurSel(GetSafeCBIndex(monster.ability_list[i], m_abilityToCBIndex, _countof(m_abilityToCBIndex)));
		}
	}

	for (int i = 0; i < DQMJ_SKILL_MAX; i++)
	{
		LPCTSTR str = ::DQMJSaveGetSkillName(monster.skill_list[i]);
		if (str != NULL)
			VERIFY(m_lstSkill.SetItemData(m_lstSkill.AddString(str), monster.skill_list[i]) != LB_ERR);
	}

	SetDlgItemText(IDC_SKILL_NUM, GetSkillNumString(m_lstSkill.GetCount()));

	for (int i = 0; i < _countof(m_cmbParentsRace); i++)
	{
		if (monster.parents_race[i] == 0x00)
		{
			m_strParentsName[i] = _T("");
			m_strParentsMaster[i] = _T("");

			CheckDlgButton(parents_ctrl_id[i][0], BST_CHECKED);
			CheckDlgButton(parents_ctrl_id[i][1], BST_CHECKED);
			CheckDlgButton(parents_ctrl_id[i][2], BST_CHECKED);

			GetDlgItem(parents_ctrl_id[i][1])->EnableWindow(FALSE);
			GetDlgItem(parents_ctrl_id[i][2])->EnableWindow(FALSE);
			GetDlgItem(parents_ctrl_id[i][3])->EnableWindow(FALSE);
			GetDlgItem(parents_ctrl_id[i][4])->EnableWindow(FALSE);
			GetDlgItem(parents_ctrl_id[i][5])->EnableWindow(FALSE);
			GetDlgItem(parents_ctrl_id[i][6])->EnableWindow(FALSE);

			m_cmbParentsRace[i].EnableWindow(FALSE);
			m_cmbParentsRace[i].SetCurSel(CB_ERR);
			m_cmbGrandparentsRace[i][0].EnableWindow(FALSE);
			m_cmbGrandparentsRace[i][0].SetCurSel(CB_ERR);
			m_cmbGrandparentsRace[i][1].EnableWindow(FALSE);
			m_cmbGrandparentsRace[i][1].SetCurSel(CB_ERR);
		}
		else
		{
			m_strParentsName[i] = CDqmjSEApp::GetNameString(monster.parents_name[i]);
			m_strParentsMaster[i] = CDqmjSEApp::GetNameString(monster.parents_master[i]);

			CheckDlgButton(parents_ctrl_id[i][0], BST_UNCHECKED);

			GetDlgItem(parents_ctrl_id[i][1])->EnableWindow(TRUE);
			GetDlgItem(parents_ctrl_id[i][2])->EnableWindow(TRUE);
			GetDlgItem(parents_ctrl_id[i][3])->EnableWindow(TRUE);
			GetDlgItem(parents_ctrl_id[i][4])->EnableWindow(TRUE);
			GetDlgItem(parents_ctrl_id[i][5])->EnableWindow(TRUE);
			GetDlgItem(parents_ctrl_id[i][6])->EnableWindow(TRUE);

			m_cmbParentsRace[i].EnableWindow(TRUE);
			m_cmbParentsRace[i].SetCurSel(GetSafeCBIndex(monster.parents_race[i], m_raceToCBIndex, _countof(m_raceToCBIndex)));

			if (monster.grandparents_race[i][0] == 0x00)
			{
				CheckDlgButton(parents_ctrl_id[i][1], BST_CHECKED);
				m_cmbGrandparentsRace[i][0].EnableWindow(FALSE);
				m_cmbGrandparentsRace[i][0].SetCurSel(CB_ERR);
			}
			else
			{
				CheckDlgButton(parents_ctrl_id[i][1], BST_UNCHECKED);
				m_cmbGrandparentsRace[i][0].EnableWindow(TRUE);
				m_cmbGrandparentsRace[i][0].SetCurSel(GetSafeCBIndex(monster.grandparents_race[i][0], m_raceToCBIndex, _countof(m_raceToCBIndex)));
			}

			if (monster.grandparents_race[i][1] == 0x00)
			{
				CheckDlgButton(parents_ctrl_id[i][2], BST_CHECKED);
				m_cmbGrandparentsRace[i][1].EnableWindow(FALSE);
				m_cmbGrandparentsRace[i][1].SetCurSel(CB_ERR);
			}
			else
			{
				CheckDlgButton(parents_ctrl_id[i][2], BST_UNCHECKED);
				m_cmbGrandparentsRace[i][1].EnableWindow(TRUE);
				m_cmbGrandparentsRace[i][1].SetCurSel(GetSafeCBIndex(monster.grandparents_race[i][1], m_raceToCBIndex, _countof(m_raceToCBIndex)));
			}
		}
	}

	UpdateData(FALSE);
}

void CMonsterListDlg::RefreshSkillSel(int sel, BOOL disable)
{
	int item_count = m_lstSkill.GetCount();

	GetDlgItem(IDC_BUTTON_SKILL_UP)->EnableWindow(!disable && (sel > 0));
	GetDlgItem(IDC_BUTTON_SKILL_TOP)->EnableWindow(!disable && (sel > 0));
	GetDlgItem(IDC_BUTTON_SKILL_DOWN)->EnableWindow(!disable && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_SKILL_BOTTOM)->EnableWindow(!disable && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_SKILL_ADD)->EnableWindow(!disable && (item_count < DQMJ_SKILL_MAX));
	GetDlgItem(IDC_BUTTON_SKILL_MODIFY)->EnableWindow(!disable && (sel >= 0));
	GetDlgItem(IDC_BUTTON_SKILL_REMOVE)->EnableWindow(!disable && (sel >= 0));
	GetDlgItem(IDC_BUTTON_SKILL_SORT)->EnableWindow(!disable && (item_count > 0));
}

BOOL CMonsterListDlg::IsInPartyOrStandby(int monster_idx)
{
	for (int i = 0; i < DQMJ_MEMBER_MAX; i++)
	{
		if (monster_idx == m_ranchInfo.party_member_idx[i])
			return TRUE;

		if (monster_idx == m_ranchInfo.standby_idx[i])
			return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void CMonsterListDlg::StaticInitialize()
{
	static BOOL flag = FALSE;
	if (flag)
		return;

	flag = TRUE;

	m_emptyMonster.name[0] = 0xFF;
	m_emptyMonster.master[0] = 0xFF;
	m_emptyMonster.sex = -1;
	m_emptyMonster.rank = DQMJ_AUTO_RANK;
	m_emptyMonster.type = DQMJ_AUTO_TYPE;
	m_emptyMonster.strategy = -1;
	m_emptyMonster.source = -1;
	m_emptyMonster.growth_type = DQMJ_RAND_GROWTH_TYPE;
	m_emptyMonster.growth_level[0] = DQMJ_RAND_GROWTH_LEVEL;
	m_emptyMonster.growth_level[1] = DQMJ_RAND_GROWTH_LEVEL;

	for (int i = 0; i < _countof(m_emptyMonster.guard_info); i++)
		m_emptyMonster.guard_info[i] = -1;

	InitCBIndex();
}

void CMonsterListDlg::InitCBIndex()
{
	for (int i = 0; i < _countof(m_raceToCBIndex); i++)
		m_raceToCBIndex[i] = CB_ERR;
	for (int i = 0; i < _countof(m_skillsetToCBIndex); i++)
		m_skillsetToCBIndex[i] = CB_ERR;
	for (int i = 0; i < _countof(m_abilityToCBIndex); i++)
		m_abilityToCBIndex[i] = CB_ERR;

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
			IDStrPair pair = { rs.race, rs.name };
			m_raceToCBIndex[pair.id] = m_raceIDStrList.GetCount();
			m_raceIDStrList.AddTail(pair);
		}
	}

	for (int i = 0; i < DQMJ_SKILLSETLIST_LEN; i++)
	{
		LPCTSTR str = ::DQMJSaveGetSkillsetName(i);
		if (str != NULL)
		{
			IDStrPair pair = { i, str };
			m_skillsetToCBIndex[pair.id] = m_skillsetIDStrList.GetCount();
			m_skillsetIDStrList.AddTail(pair);
		}
	}

	for (int i = 0; i < DQMJ_ABILITYLIST_LEN; i++)
	{
		LPCTSTR str = ::DQMJSaveGetAbilityName(i);
		if (str != NULL)
		{
			IDStrPair pair = { i, str };
			m_abilityToCBIndex[pair.id] = m_abilityIDStrList.GetCount();
			m_abilityIDStrList.AddTail(pair);
		}
	}
}

void CMonsterListDlg::InitComboBox(CComboBox &combo, const int *list, int len)
{
	combo.SetRedraw(FALSE);

	for (int i = 0; i < len; i++)
		VERIFY(combo.AddString(CDqmjSEApp::GetDecimalString(list[i])) == i);

	combo.SetRedraw(TRUE);
}

void CMonsterListDlg::InitComboBox(CComboBox &combo, const LPCTSTR *list, int len)
{
	combo.SetRedraw(FALSE);

	for (int i = 0; i < len; i++)
		VERIFY(combo.AddString(list[i]) == i);

	combo.SetRedraw(TRUE);
}

void CMonsterListDlg::InitComboBox(CComboBox &combo, const CIDStrList &list)
{
	combo.SetRedraw(FALSE);

	POSITION pos = list.GetHeadPosition();
	while (pos != NULL)
	{
		const IDStrPair &pair = list.GetNext(pos);
		VERIFY(combo.SetItemData(combo.AddString(pair.str), pair.id) != CB_ERR);
	}

	combo.SetRedraw(TRUE);
}

int CMonsterListDlg::GetSafeCBIndex(int idx, int len)
{
	return ((idx >= 0) && (idx < len)) ? idx : CB_ERR;
}

int CMonsterListDlg::GetSafeCBIndex(int idx, const int *list, int len)
{
	return ((idx >= 0) && (idx < len)) ? list[idx] : CB_ERR;
}

LPCTSTR CMonsterListDlg::GetSafeCBString(int idx, const LPCTSTR *list, int len)
{
	return ((idx >= 0) && (idx < len)) ? list[idx] : _T("不明");
}

LPCTSTR CMonsterListDlg::GetRaceString(int race)
{
	DQMJ_SAVE_RACE_SETTING rs;
	if (::DQMJSaveGetRaceSetting(race, &rs))
		return rs.name;

	return _T("不明");
}

LPCTSTR CMonsterListDlg::GetEquipmentNameString(int equipment)
{
	if (equipment == 0x00)
		return _T("なし");

	DQMJ_SAVE_ITEM_SETTING is;
	if (::DQMJSaveGetItemSetting(equipment, &is))
		return is.name;

	return _T("不明");
}

CString CMonsterListDlg::GetMonsterNumString(int num)
{
	CString str;
	str.Format(_T("%d/%d"), num, DQMJ_MONSTER_MAX);

	return str;
}

CString CMonsterListDlg::GetSkillNumString(int num)
{
	CString str;
	str.Format(_T("%d/%d"), num, DQMJ_SKILL_MAX);

	return str;
}

//////////////////////////////////////////////////////////////////////////
