
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

void CMonsterListDlg::DoDataExchange(CDataExchange* pDX)
{
	::DDX_Control(pDX, IDC_COMBO_SEX, m_cmbSex);
	::DDX_Control(pDX, IDC_COMBO_RACE, m_cmbRace);
	::DDX_Control(pDX, IDC_COMBO_RANK, m_cmbRank);
	::DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	::DDX_Control(pDX, IDC_COMBO_STRATEGY, m_cmbStrategy);
	::DDX_Control(pDX, IDC_COMBO_SOURCE, m_cmbSource);
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

	CDialog::DoDataExchange(pDX);
}

BOOL CMonsterListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbSource.AddString(_T("野良モンスタースカウト"));
	m_cmbSource.SetCurSel(0);
	m_cmbGuard[0].AddString(_T("ふつう"));
	m_cmbType.AddString(_T("しんじゅう系"));
	m_cmbSex.AddString(_T("両性具有"));
	m_cmbSex.SetCurSel(0);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
