
#include "stdafx.h"
#include "dqmjse.h"
#include "monsterlistdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMonsterListDlg, CDialog)

BEGIN_MESSAGE_MAP(CMonsterListDlg, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CMonsterListDlg::CMonsterListDlg(CWnd *pParent) : CDialog(IDD_DQMJSE_MONSTER_LIST, pParent)
{

}

//////////////////////////////////////////////////////////////////////////

void CMonsterListDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_LIST_MONSTER_LIST, m_lstMonster);
	::DDX_Control(pDX, IDC_LIST_SKILL_LIST, m_lstSkill);
	::DDX_Control(pDX, IDC_COMBO_SEX, m_cmbSex);
	::DDX_Control(pDX, IDC_COMBO_RACE, m_cmbRace);
	::DDX_Control(pDX, IDC_COMBO_RANK, m_cmbRank);
	::DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	::DDX_Control(pDX, IDC_COMBO_STRATEGY, m_cmbStrategy);
	::DDX_Control(pDX, IDC_COMBO_SOURCE, m_cmbSource);
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
	::DDX_Control(pDX, IDC_COMBO_FARTHER_RACE, m_cmbParentsRace[0][0]);
	::DDX_Control(pDX, IDC_COMBO_FARTHER_FARTHER_RACE, m_cmbParentsRace[0][1]);
	::DDX_Control(pDX, IDC_COMBO_FARTHER_MOTHER_RACE, m_cmbParentsRace[0][2]);
	::DDX_Control(pDX, IDC_COMBO_MOTHER_RACE, m_cmbParentsRace[1][0]);
	::DDX_Control(pDX, IDC_COMBO_MOTHER_FARTHER_RACE, m_cmbParentsRace[1][1]);
	::DDX_Control(pDX, IDC_COMBO_MOTHER_MOTHER_RACE, m_cmbParentsRace[1][2]);

	CDialog::DoDataExchange(pDX);
}

BOOL CMonsterListDlg::OnInitDialog()
{
	static const int guard_name_id[] =
	{
		IDC_GUARD_NAME1, IDC_GUARD_NAME2, IDC_GUARD_NAME3, IDC_GUARD_NAME4, IDC_GUARD_NAME5, IDC_GUARD_NAME6, IDC_GUARD_NAME7, IDC_GUARD_NAME8, IDC_GUARD_NAME9,
		IDC_GUARD_NAME10, IDC_GUARD_NAME11, IDC_GUARD_NAME12, IDC_GUARD_NAME13, IDC_GUARD_NAME14, IDC_GUARD_NAME15, IDC_GUARD_NAME16, IDC_GUARD_NAME17, IDC_GUARD_NAME18,
		IDC_GUARD_NAME19, IDC_GUARD_NAME20, IDC_GUARD_NAME21, IDC_GUARD_NAME22, IDC_GUARD_NAME23, IDC_GUARD_NAME24, IDC_GUARD_NAME25, IDC_GUARD_NAME26, IDC_GUARD_NAME27, 
	};

	CDialog::OnInitDialog();

	VERIFY(m_cmbSex.AddString(_T("♂")) == DQMJ_SEX_MALE);
	VERIFY(m_cmbSex.AddString(_T("♀")) == DQMJ_SEX_FEMALE);
	VERIFY(m_cmbSex.AddString(_T("両性具有")) == DQMJ_SEX_BISEXUAL);

	VERIFY(m_cmbRank.AddString(_T("なし")) == DQMJ_RANK_NONE);
	VERIFY(m_cmbRank.AddString(_T("F")) == DQMJ_RANK_F);
	VERIFY(m_cmbRank.AddString(_T("E")) == DQMJ_RANK_E);
	VERIFY(m_cmbRank.AddString(_T("D")) == DQMJ_RANK_D);
	VERIFY(m_cmbRank.AddString(_T("C")) == DQMJ_RANK_C);
	VERIFY(m_cmbRank.AddString(_T("B")) == DQMJ_RANK_B);
	VERIFY(m_cmbRank.AddString(_T("A")) == DQMJ_RANK_A);
	VERIFY(m_cmbRank.AddString(_T("S")) == DQMJ_RANK_S);
	VERIFY(m_cmbRank.AddString(_T("SS")) == DQMJ_RANK_SS);
	VERIFY(m_cmbRank.AddString(_T("??")) == DQMJ_RANK_UNKNOWN);

	VERIFY(m_cmbType.AddString(_T("なし")) == DQMJ_TYPE_NONE);
	VERIFY(m_cmbType.AddString(_T("スライム系")) == DQMJ_TYPE_SLIME);
	VERIFY(m_cmbType.AddString(_T("ドラゴン系")) == DQMJ_TYPE_DRAGON);
	VERIFY(m_cmbType.AddString(_T("しぜん系")) == DQMJ_TYPE_NATURE);
	VERIFY(m_cmbType.AddString(_T("まじゅう系")) == DQMJ_TYPE_BEAST);
	VERIFY(m_cmbType.AddString(_T("ぶっしつ系")) == DQMJ_TYPE_MATERIAL);
	VERIFY(m_cmbType.AddString(_T("あくま系")) == DQMJ_TYPE_DEMON);
	VERIFY(m_cmbType.AddString(_T("ゾンビ系")) == DQMJ_TYPE_ZOMBIE);
	VERIFY(m_cmbType.AddString(_T("しんじゅう系")) == DQMJ_TYPE_INCARNI);

	VERIFY(m_cmbSource.AddString(_T("システム")) == DQMJ_SOURCE_SYSTEM);
	VERIFY(m_cmbSource.AddString(_T("ワイヤレス交換(?)")) == DQMJ_SOURCE_EXCHANGE);
	VERIFY(m_cmbSource.AddString(_T("他国マスタースカウト")) == DQMJ_SOURCE_MASTER_SCOUT);
	VERIFY(m_cmbSource.AddString(_T("野良モンスタースカウト")) == DQMJ_SOURCE_FEILD_SCOUT);
	VERIFY(m_cmbSource.AddString(_T("配合")) == DQMJ_SOURCE_COMBINE);
	VERIFY(m_cmbSource.AddString(_T("ゲスト")) == DQMJ_SOURCE_GUEST);

	VERIFY(m_cmbStrategy.AddString(_T("ガンガンいこうぜ")) == DQMJ_STRATEGY_GO_ALL_OUT);
	VERIFY(m_cmbStrategy.AddString(_T("いろいろやろうぜ")) == DQMJ_STRATEGY_TRY_OUT);
	VERIFY(m_cmbStrategy.AddString(_T("いのちだいじに")) == DQMJ_STRATEGY_TAKE_CARE_LIFE);
	VERIFY(m_cmbStrategy.AddString(_T("とくぎつかうな")) == DQMJ_STRATEGY_NO_SKILL);

	VERIFY(m_cmbGrowthType.AddString(_T("ＨＰ")) == DQMJ_GROWTH_HP);
	VERIFY(m_cmbGrowthType.AddString(_T("ＭＰ")) == DQMJ_GROWTH_MP);
	VERIFY(m_cmbGrowthType.AddString(_T("こうげき力")) == DQMJ_GROWTH_ATK);
	VERIFY(m_cmbGrowthType.AddString(_T("しゅび力")) == DQMJ_GROWTH_DEF);
	VERIFY(m_cmbGrowthType.AddString(_T("すばやさ")) == DQMJ_GROWTH_AGI);
	VERIFY(m_cmbGrowthType.AddString(_T("かしこさ")) == DQMJ_GROWTH_INT);

	for (int i = 0; i < _countof(m_cmbGuard); i++)
	{
		SetDlgItemText(guard_name_id[i], ::DQMJSaveGetGuardName(i));
		VERIFY(m_cmbGuard[i].AddString(_T("弱い")) == DQMJ_GUARD_WEAK);
		VERIFY(m_cmbGuard[i].AddString(_T("ふつう")) == DQMJ_GUARD_NORMAL);
		VERIFY(m_cmbGuard[i].AddString(_T("軽減")) == DQMJ_GUARD_STRONG);
		VERIFY(m_cmbGuard[i].AddString(_T("半減")) == DQMJ_GUARD_HALF);
		VERIFY(m_cmbGuard[i].AddString(_T("激減")) == DQMJ_GUARD_VERY_STRONG);
		VERIFY(m_cmbGuard[i].AddString(_T("無効")) == DQMJ_GUARD_INVALID);
		VERIFY(m_cmbGuard[i].AddString(_T("反射")) == DQMJ_GUARD_REFLECT);
		VERIFY(m_cmbGuard[i].AddString(_T("吸収")) == DQMJ_GUARD_ABSORB);
	}

	for (int i = 0; i < _countof(m_cmbSkillset); i++)
	{
		for (int j = 0; j < DQMJ_SKILLSETLIST_LEN; j++)
		{
			LPCTSTR str = ::DQMJSaveGetSkillsetName(j);
			if (str != NULL)
				VERIFY(m_cmbSkillset[i].SetItemData(m_cmbSkillset[i].AddString(str), j) != CB_ERR);
		}
	}

	for (int i = 0; i < _countof(m_cmbAbility); i++)
	{
		for (int j = 0; j < DQMJ_ABILITYLIST_LEN; j++)
		{
			LPCTSTR str = ::DQMJSaveGetAbilityName(j);
			if (str != NULL)
				VERIFY(m_cmbAbility[i].SetItemData(m_cmbAbility[i].AddString(str), j) != CB_ERR);
		}
	}

	InitRaceComboBox(m_cmbRace);

	for (int i = 0; i < _countof(m_cmbParentsRace); i++)
	for (int j = 0; j < _countof(m_cmbParentsRace[i]); j++)
		InitRaceComboBox(m_cmbParentsRace[i][j]);

	EnableAll(FALSE);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CMonsterListDlg::EnableAll(BOOL enable)
{
	if (!enable)
	{
		m_cmbSex.SetCurSel(CB_ERR);
		m_cmbRank.SetCurSel(CB_ERR);
		m_cmbType.SetCurSel(CB_ERR);
		m_cmbSource.SetCurSel(CB_ERR);
		m_cmbStrategy.SetCurSel(CB_ERR);
		m_cmbType.SetCurSel(CB_ERR);
		m_cmbRace.SetCurSel(CB_ERR);

		for (int i = 0; i < _countof(m_cmbGuard); i++)
			m_cmbGuard[i].SetCurSel(CB_ERR);
		for (int i = 0; i < _countof(m_cmbSkillset); i++)
			m_cmbSkillset[i].SetCurSel(CB_ERR);
		for (int i = 0; i < _countof(m_cmbAbility); i++)
			m_cmbAbility[i].SetCurSel(CB_ERR);

		for (int i = 0; i < _countof(m_cmbParentsRace); i++)
		for (int j = 0; j < _countof(m_cmbParentsRace[i]); j++)
			m_cmbParentsRace[i][j].SetCurSel(CB_ERR);
	}

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
	GetDlgItem(IDC_EDIT_MASTER)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_MASTER_ME)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_MASTER_UNKNOWN)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_RANK_AUTO)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_TYPE_AUTO)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_CHEAT)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_SEX)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_RACE)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_RANK)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_STRATEGY)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_SOURCE)->EnableWindow(enable);

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
	GetDlgItem(IDC_EDIT_MAX_HP)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_MAX_MP)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_ATK)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_DEF)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_AGI)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_INT)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_CUR_HP)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_CUR_MP)->EnableWindow(enable);
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
	GetDlgItem(IDC_SPIN_MAX_HP)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_MAX_MP)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_ATK)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_DEF)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_AGI)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_INT)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_CUR_HP)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_CUR_MP)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_SKILL_POINT)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_EXP)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_STATUS_MAX)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_STATUS_MAX_FORCE)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_DEAD)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_POISON)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_WEAPON)->EnableWindow(enable);
	GetDlgItem(IDC_WEAPON_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_WEAPON_MODIFY)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_WEAPON_REMOVE)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_GROWTH)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_GROWTH_TYPE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_GROWTH_LEVEL)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_GROWTH_LEVEL_RANGE)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_GROWTH_LEVEL_START)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_GROWTH_LEVEL_END)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_GROWTH_LEVEL_START)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_GROWTH_LEVEL_END)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_GROWTH_TYPE_RANDOM)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_GROWTH_LEVEL_RANDOM)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_GROWTH_TYPE)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_SKILLSET)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_SKILLSET_PT1)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_SKILLSET_PT2)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_SKILLSET_PT3)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_SKILLSET_PT1)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_SKILLSET_PT2)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_SKILLSET_PT3)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_SKILLSET_PT1)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_SKILLSET_PT2)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_SKILLSET_PT3)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_SKILLSET_MAX1)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_SKILLSET_MAX2)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_SKILLSET_MAX3)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLSET_NONE1)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLSET_NONE2)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLSET_NONE3)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_SKILLSET1)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_SKILLSET2)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_SKILLSET3)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_ABILITY)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_ABILITY_NONE1)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_ABILITY_NONE2)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_ABILITY_NONE3)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_ABILITY_NONE4)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_ABILITY_NONE5)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_ABILITY_NONE6)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_ABILITY1)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_ABILITY2)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_ABILITY3)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_ABILITY4)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_ABILITY5)->EnableWindow(enable);
	GetDlgItem(IDC_COMBO_ABILITY6)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_SKILL)->EnableWindow(enable);
}

//////////////////////////////////////////////////////////////////////////

void CMonsterListDlg::InitRaceComboBox(CComboBox &combo)
{
	static BOOL flag = FALSE;
	static CList<DQMJ_SAVE_RACE_SETTING> list[DQMJ_TYPE_NUM];

	if (!flag)
	{
		for (int i = 0; i < DQMJ_MONSTERLIST_LEN; i++)
		{
			DQMJ_SAVE_RACE_SETTING rs;
			if (::DQMJSaveGetRaceSetting(i, &rs))
				list[rs.type].AddTail(rs);
		}

		flag = TRUE;
	}

	for (int i = 0; i < DQMJ_TYPE_NUM; i++)
	{
		POSITION pos = list[i].GetHeadPosition();
		while (pos != NULL)
		{
			const DQMJ_SAVE_RACE_SETTING &rs = list[i].GetNext(pos);
			VERIFY(combo.SetItemData(combo.AddString(rs.name), rs.race) != CB_ERR);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
