
#include "stdafx.h"
#include "dqmjse.h"
#include "selectskilldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSelectSkillDlg, CDialog)

BEGIN_MESSAGE_MAP(CSelectSkillDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_SKILL_LIST, OnLbnSelchangeListItemList)
	ON_LBN_DBLCLK(IDC_LIST_SKILL_LIST, OnLbnDblclkListItemList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CSelectSkillDlg::CSelectSkillDlg(int skill, CWnd *pParent) : CDialog(IDD_DQMJSE_SELECT_SKILL, pParent)
{
	m_nSelSkill = skill;
}

//////////////////////////////////////////////////////////////////////////

void CSelectSkillDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_LIST_SKILL_LIST, m_lstSkill);

	CDialog::DoDataExchange(pDX);
}

BOOL CSelectSkillDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstSkill.SetRedraw(FALSE);

	for (int i = 0; i < DQMJ_SKILLLIST_LEN; i++)
	{
		LPCTSTR name = ::DQMJSaveGetSkillName(i);
		if (name == NULL)
			continue;

		int idx = m_lstSkill.AddString(name);
		ASSERT(idx >= 0);

		m_lstSkill.SetItemData(idx, i);
		if (i == m_nSelSkill)
			m_lstSkill.SetCurSel(idx);
	}

	m_lstSkill.SetRedraw(TRUE);

	if ((m_nSelSkill != 0x00) && (m_lstSkill.GetCurSel() < 0))
		m_nSelSkill = 0x00;

	RefreshEnable();

	return TRUE;
}

void CSelectSkillDlg::OnOK()
{
	int sel = m_lstSkill.GetCurSel();
	if (sel < 0)
		return;

	m_nSelSkill = m_lstSkill.GetItemData(sel);
	CDialog::OnOK();
}

void CSelectSkillDlg::OnLbnSelchangeListItemList()
{
	RefreshEnable();
}

void CSelectSkillDlg::OnLbnDblclkListItemList()
{
	OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CSelectSkillDlg::RefreshEnable()
{
	GetDlgItem(IDOK)->EnableWindow(m_lstSkill.GetCurSel() >= 0);
}

//////////////////////////////////////////////////////////////////////////
