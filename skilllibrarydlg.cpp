
#include "stdafx.h"
#include "dqmjse.h"
#include "skilllibrarydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSkillLibraryDlg, CDialog)

BEGIN_MESSAGE_MAP(CSkillLibraryDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CHECKALL, OnBnClickedButtonCheckall)
	ON_CBN_SELCHANGE(IDC_COMBO_SKILL_FILTER, OnCbnSelchangeComboSkillFilter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CSkillLibraryDlg::CSkillLibraryDlg(CWnd *pParent) : CDialog(IDD_DQMJSE_SKILL_LIBRARY, pParent)
{

}

//////////////////////////////////////////////////////////////////////////

void CSkillLibraryDlg::Update()
{
	RefreshCollectNum();
}

//////////////////////////////////////////////////////////////////////////

void CSkillLibraryDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_COMBO_SKILL_FILTER, m_cmbFilter);

	CDialog::DoDataExchange(pDX);
}

BOOL CSkillLibraryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbFilter.SetRedraw(FALSE);
	VERIFY(m_cmbFilter.AddString(_T("すべて")) == CSkillPanelDlg::FILTER_ALL);
	VERIFY(m_cmbFilter.AddString(_T("とくぎのスキル")) == CSkillPanelDlg::FILTER_SKILL);
	VERIFY(m_cmbFilter.AddString(_T("パラメータスキル")) == CSkillPanelDlg::FILTER_PARAMETER);
	m_cmbFilter.SetRedraw(TRUE);
	m_cmbFilter.SetCurSel(CSkillPanelDlg::FILTER_ALL);

	CRect rect;
	GetDlgItem(IDC_STATIC_RECT)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_dlgPanel.Create(IDD_DQMJSE_SKILL_PANEL, this);
	m_dlgPanel.MoveWindow(&rect, FALSE);
	m_dlgPanel.ShowWindow(SW_SHOW);

	RefreshCollectNum();

	return TRUE;
}

void CSkillLibraryDlg::OnOK()
{
	UpdateData(TRUE);
}

void CSkillLibraryDlg::OnCancel()
{
	m_dlgPanel.UpdateData(TRUE);

	DQMJ_SAVE_LIBRARY_INFO libraryinfo;
	VERIFY(::DQMJSaveQueryLibraryInfo(theApp.GetSaveHandle(), &libraryinfo));

	for (int i = 0; i < DQMJ_SKILLSETLIST_LEN; i++)
		libraryinfo.skill[i] = m_dlgPanel.GetSkillsetUnlockNum(i);

	VERIFY(::DQMJSaveModifyLibraryInfo(theApp.GetSaveHandle(), &libraryinfo));

	CDialog::OnCancel();
}

void CSkillLibraryDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}

void CSkillLibraryDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnRButtonDown(nFlags, point);
}

void CSkillLibraryDlg::OnBnClickedButtonCheckall()
{
	m_dlgPanel.OpenAll();
	RefreshCollectNum();
}

void CSkillLibraryDlg::OnCbnSelchangeComboSkillFilter()
{
	int sel = m_cmbFilter.GetCurSel();
	if (sel < 0)
		return;

	if (m_dlgPanel.SetFilter((CSkillPanelDlg::FILTER)sel))
		RefreshCollectNum();
}

//////////////////////////////////////////////////////////////////////////

void CSkillLibraryDlg::RefreshCollectNum()
{
	int sel = m_cmbFilter.GetCurSel();
	if (sel < 0)
		return;

	int total = 0;
	int cnt = 0;

	for (int i = 0; i < DQMJ_SKILLSETLIST_LEN; i++)
	{
		DQMJ_SAVE_SKILLSET_SETTING sss;
		if (!::DQMJSaveGetSkillsetSetting(i, &sss))
			continue;

		switch (sel)
		{
		case CSkillPanelDlg::FILTER_SKILL:
			if (sss.param)
				continue;
			break;
		case CSkillPanelDlg::FILTER_PARAMETER:
			if (!sss.param)
				continue;
			break;
		}

		total++;

		if (m_dlgPanel.GetSkillsetUnlockNum(i) != DQMJ_SKILL_LOCKED)
			cnt++;
	}

	SetDlgItemText(IDC_COLLECT_NUM, theApp.GetDecimalString(cnt, total));
}

//////////////////////////////////////////////////////////////////////////
