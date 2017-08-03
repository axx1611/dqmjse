
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
	::DDX_Control(pDX, IDC_COMBO_RACE, m_cmbRace);
	::DDX_Control(pDX, IDC_COMBO_RANK, m_cmbRank);
	::DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	::DDX_Control(pDX, IDC_COMBO_STRATEGY, m_cmbStrategy);
	::DDX_Control(pDX, IDC_COMBO_SOURCE, m_cmbSource);
	::DDX_Control(pDX, IDC_COMBO_GUARD_BATCH, m_cmbGuardBatch);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL1, m_cmbGuardLevel[0]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL2, m_cmbGuardLevel[1]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL3, m_cmbGuardLevel[2]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL4, m_cmbGuardLevel[3]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL5, m_cmbGuardLevel[4]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL6, m_cmbGuardLevel[5]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL7, m_cmbGuardLevel[6]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL8, m_cmbGuardLevel[7]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL9, m_cmbGuardLevel[8]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL10, m_cmbGuardLevel[9]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL11, m_cmbGuardLevel[10]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL12, m_cmbGuardLevel[11]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL13, m_cmbGuardLevel[12]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL14, m_cmbGuardLevel[13]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL15, m_cmbGuardLevel[14]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL16, m_cmbGuardLevel[15]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL17, m_cmbGuardLevel[16]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL18, m_cmbGuardLevel[17]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL19, m_cmbGuardLevel[18]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL20, m_cmbGuardLevel[19]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL21, m_cmbGuardLevel[20]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL22, m_cmbGuardLevel[21]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL23, m_cmbGuardLevel[22]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL24, m_cmbGuardLevel[23]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL25, m_cmbGuardLevel[24]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL26, m_cmbGuardLevel[25]);
	::DDX_Control(pDX, IDC_COMBO_GUARD_LEVEL27, m_cmbGuardLevel[26]);
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

	m_cmbSource.AddString(_T("Ò°Á¼¥â¥ó¥¹¥¿©`¥¹¥«¥¦¥È"));
	m_cmbSource.SetCurSel(0);
	m_cmbGuardLevel[0].AddString(_T("¤Õ¤Ä¤¦"));
	m_cmbType.AddString(_T("¤·¤ó¤¸¤å¤¦Ïµ"));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
