
#include "stdafx.h"
#include "dqmjse.h"
#include "monsterlibrarydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMonsterLibraryDlg, CDialog)

BEGIN_MESSAGE_MAP(CMonsterLibraryDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CHECKALL, OnBnClickedButtonCheckall)
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER_FILTER, OnCbnSelchangeComboMonsterFilter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CMonsterLibraryDlg::CMonsterLibraryDlg(CWnd *pParent) : CDialog(IDD_DQMJSE_MONSTER_LIBRARY, pParent)
{

}

//////////////////////////////////////////////////////////////////////////

void CMonsterLibraryDlg::Update()
{
	RefreshCollectNum();
}

//////////////////////////////////////////////////////////////////////////

void CMonsterLibraryDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_COMBO_MONSTER_FILTER, m_cmbFilter);

	CDialog::DoDataExchange(pDX);
}

BOOL CMonsterLibraryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbFilter.SetRedraw(FALSE);
	VERIFY(m_cmbFilter.AddString(_T("すべて")) == CMonsterPanelDlg::FILTER_ALL);
	VERIFY(m_cmbFilter.AddString(_T("スライムけい")) == CMonsterPanelDlg::FILTER_SLIME);
	VERIFY(m_cmbFilter.AddString(_T("ドラゴンけい")) == CMonsterPanelDlg::FILTER_DRAGON);
	VERIFY(m_cmbFilter.AddString(_T("しぜんけい")) == CMonsterPanelDlg::FILTER_NATURE);
	VERIFY(m_cmbFilter.AddString(_T("まじゅうけい")) == CMonsterPanelDlg::FILTER_BEAST);
	VERIFY(m_cmbFilter.AddString(_T("ぶっしつけい")) == CMonsterPanelDlg::FILTER_MATERIAL);
	VERIFY(m_cmbFilter.AddString(_T("あくまけい")) == CMonsterPanelDlg::FILTER_DEMON);
	VERIFY(m_cmbFilter.AddString(_T("ゾンビけい")) == CMonsterPanelDlg::FILTER_ZOMBIE);
	VERIFY(m_cmbFilter.AddString(_T("しんじゅうけい")) == CMonsterPanelDlg::FILTER_INCARNI);
	m_cmbFilter.SetRedraw(TRUE);
	m_cmbFilter.SetCurSel(CMonsterPanelDlg::FILTER_ALL);

	CRect rect;
	GetDlgItem(IDC_STATIC_RECT)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_dlgPanel.Create(IDD_DQMJSE_MONSTER_PANEL, this);
	m_dlgPanel.MoveWindow(&rect, FALSE);
	m_dlgPanel.ShowWindow(SW_SHOW);

	RefreshCollectNum();

	return TRUE;
}

void CMonsterLibraryDlg::OnOK()
{
	UpdateData(TRUE);
}

void CMonsterLibraryDlg::OnCancel()
{
	m_dlgPanel.UpdateData(TRUE);

	DQMJ_SAVE_LIBRARY_INFO libraryinfo;
	VERIFY(::DQMJSaveQueryLibraryInfo(theApp.GetSaveHandle(), &libraryinfo));

	for (int i = 0; i < DQMJ_MONSTERLIST_LEN; i++)
	{
		libraryinfo.encounter[i] = m_dlgPanel.IsEncounterChecked(i);
		libraryinfo.kill[i] = m_dlgPanel.IsKillChecked(i);
		libraryinfo.obtain[i] = m_dlgPanel.IsObtainChecked(i);
	}

	VERIFY(::DQMJSaveModifyLibraryInfo(theApp.GetSaveHandle(), &libraryinfo));

	CDialog::OnCancel();
}

void CMonsterLibraryDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}

void CMonsterLibraryDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnRButtonDown(nFlags, point);
}

void CMonsterLibraryDlg::OnBnClickedButtonCheckall()
{
	m_dlgPanel.OpenAll();
	RefreshCollectNum();
}

void CMonsterLibraryDlg::OnCbnSelchangeComboMonsterFilter()
{
	int sel = m_cmbFilter.GetCurSel();
	if (sel < 0)
		return;

	if (m_dlgPanel.SetFilter((CMonsterPanelDlg::FILTER)sel))
		RefreshCollectNum();
}

//////////////////////////////////////////////////////////////////////////

void CMonsterLibraryDlg::RefreshCollectNum()
{
	int sel = m_cmbFilter.GetCurSel();
	if (sel < 0)
		return;

	int total = 0;
	int cnt = 0;

	if (sel == CMonsterPanelDlg::FILTER_ALL)
	{
		const CIntToStrMap &map = theApp.GetRaceStringMap();
		total = map.GetCount();

		for (const CIntToStrMap::CPair *pair = map.PGetFirstAssoc(); pair; pair = map.PGetNextAssoc(pair))
		{
			if (m_dlgPanel.IsObtainChecked(pair->key))
				cnt++;
		}
	}
	else
	{
		const CIntList &list = theApp.GetTypeRaceList(m_dlgPanel.GetTypeByFilter((CMonsterPanelDlg::FILTER)sel));
		total = list.GetCount();

		POSITION pos = list.GetHeadPosition();
		while (pos)
		{
			if (m_dlgPanel.IsObtainChecked(list.GetNext(pos)))
				cnt++;
		}
	}

	SetDlgItemText(IDC_COLLECT_NUM, theApp.GetDecimalString(cnt, total));
}

//////////////////////////////////////////////////////////////////////////
