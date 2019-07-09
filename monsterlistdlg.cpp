
#include "stdafx.h"
#include "dqmjse.h"
#include "monsterlistdlg.h"
#include "monsterinfodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMonsterListDlg, CDialog)

BEGIN_MESSAGE_MAP(CMonsterListDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_UP, OnBnClickedButtonMonsterUp)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_TOP, OnBnClickedButtonMonsterTop)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_DOWN, OnBnClickedButtonMonsterDown)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_BOTTOM, OnBnClickedButtonMonsterBottom)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ADD, OnBnClickedButtonMonsterAdd)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_MODIFY, OnBnClickedButtonMonsterModify)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_REMOVE, OnBnClickedButtonMonsterRemove)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_COPY, OnBnClickedButtonMonsterCopy)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MONSTER_LIST, OnNMDblclkListMonsterList)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_MONSTER_LIST, OnLvnItemchangingListMonsterList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CMonsterListDlg::CMonsterListDlg(BOOL select_mode, CWnd *pParent) : CDialog(IDD_DQMJSE_MONSTER_LIST, pParent)
{
	m_bSelectMode = select_mode;
	m_bDisablePartyMember = FALSE;
	m_bDisableStandby = FALSE;
	m_nSelMonster = LB_ERR;
	m_nPartyIndex = -1;
	m_nStandbyIndex = -1;
}

void CMonsterListDlg::ConfigSelectMode(int sel, BOOL disable_party_member, BOOL disable_standby)
{
	ASSERT(m_bSelectMode);

	m_nSelMonster = sel;
	m_bDisablePartyMember = disable_party_member;
	m_bDisableStandby = disable_standby;
}

//////////////////////////////////////////////////////////////////////////

void CMonsterListDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_LIST_MONSTER_LIST, m_lstMonster);

	CDialog::DoDataExchange(pDX);
}

BOOL CMonsterListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstMonster.SetExtendedStyle(m_lstMonster.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	VERIFY(m_lstMonster.InsertColumn(COLUMN_NO, _T("No."), LVCFMT_LEFT, 28) == COLUMN_NO);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_PARTY, _T("PT"), LVCFMT_LEFT, 25) == COLUMN_PARTY);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_STANDBY, _T("ST"), LVCFMT_LEFT, 25) == COLUMN_STANDBY);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_NAME, _T("名前"), LVCFMT_LEFT, 64) == COLUMN_NAME);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_SEX, _T("性別"), LVCFMT_CENTER, 34) == COLUMN_SEX);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_RACE, _T("種族"), LVCFMT_LEFT, 88) == COLUMN_RACE);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_LEVEL, _T("レベル"), LVCFMT_LEFT, 42) == COLUMN_LEVEL);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_RANK, _T("RANK"), LVCFMT_CENTER, 42) == COLUMN_RANK);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_TYPE, _T("系統"), LVCFMT_LEFT, 62) == COLUMN_TYPE);
	VERIFY(m_lstMonster.InsertColumn(COLUMN_WEAPON, _T("装備"), LVCFMT_CENTER, 90) == COLUMN_WEAPON);

	m_lstMonster.SetRedraw(FALSE);
	UpdateMonsterList();
	m_lstMonster.SetRedraw(TRUE);

	if (m_bSelectMode)
	{
		if (GetCurSel() < 0)
			m_nSelMonster = LB_ERR;

		GetDlgItem(IDC_BUTTON_MONSTER_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_TOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_DOWN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_BOTTOM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_MODIFY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_REMOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_COPY)->ShowWindow(SW_HIDE);

		CRect rect1, rect2;
		GetDlgItem(IDC_BUTTON_MONSTER_UP)->GetWindowRect(rect1);
		GetDlgItem(IDC_BUTTON_MONSTER_DOWN)->GetWindowRect(rect2);

		CRect rect;
		GetWindowRect(rect);
		rect.bottom -= rect2.top - rect1.top;
		MoveWindow(rect);
	}
	else
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

		RefreshMonsterSel(LB_ERR);
	}

	return TRUE;
}

void CMonsterListDlg::OnOK()
{
	if (!m_bSelectMode)
		return;

	int sel = GetCurSel();
	if (!CanSelect(sel))
		return;

	m_nSelMonster = sel;
	m_nPartyIndex = (SHORT)LOWORD(m_lstMonster.GetItemData(sel));
	m_nStandbyIndex = (SHORT)HIWORD(m_lstMonster.GetItemData(sel));

	CDialog::OnOK();
}

void CMonsterListDlg::OnBnClickedButtonMonsterUp()
{
	if (m_bSelectMode)
		return;

	int cur_sel = GetCurSel();
	if (cur_sel <= 0)
		return;

	MoveMonster(cur_sel, cur_sel - 1);
}

void CMonsterListDlg::OnBnClickedButtonMonsterTop()
{
	if (m_bSelectMode)
		return;

	int cur_sel = GetCurSel();
	if (cur_sel <= 0)
		return;

	MoveMonster(cur_sel, 0);
}

void CMonsterListDlg::OnBnClickedButtonMonsterDown()
{
	if (m_bSelectMode)
		return;

	int cur_sel = GetCurSel();
	if ((cur_sel < 0) || (cur_sel >= m_lstMonster.GetItemCount() - 1))
		return;

	MoveMonster(cur_sel, cur_sel + 1);
}

void CMonsterListDlg::OnBnClickedButtonMonsterBottom()
{
	if (m_bSelectMode)
		return;

	int cur_sel = GetCurSel();
	int bottom_idx = m_lstMonster.GetItemCount() - 1;
	if ((cur_sel < 0) || (cur_sel >= bottom_idx))
		return;

	MoveMonster(cur_sel, bottom_idx);
}

void CMonsterListDlg::OnBnClickedButtonMonsterAdd()
{
	if (m_bSelectMode || (m_lstMonster.GetItemCount() >= DQMJ_MONSTER_MAX))
		return;

	AddMonster();
}

void CMonsterListDlg::OnBnClickedButtonMonsterModify()
{
	if (m_bSelectMode)
		return;

	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	ModifyMonster(cur_sel);
}

void CMonsterListDlg::OnBnClickedButtonMonsterRemove()
{
	if (m_bSelectMode || (m_lstMonster.GetItemCount() <= 1))
		return;

	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	RemoveMonster(cur_sel);
}

void CMonsterListDlg::OnBnClickedButtonMonsterCopy()
{
	if (m_bSelectMode || (m_lstMonster.GetItemCount() >= DQMJ_MONSTER_MAX))
		return;

	int cur_sel = GetCurSel();
	if (cur_sel < 0)
		return;

	CopyMonster(cur_sel);
}

void CMonsterListDlg::OnNMDblclkListMonsterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	if (m_bSelectMode)
		OnOK();
	else
		ModifyMonster(pNMItemActivate->iItem);
}

void CMonsterListDlg::OnLvnItemchangingListMonsterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (m_bSelectMode)
		GetDlgItem(IDOK)->EnableWindow(CanSelect((pNMLV->uNewState & LVIS_SELECTED) ? pNMLV->iItem : LB_ERR));
	else
		RefreshMonsterSel((pNMLV->uNewState & LVIS_SELECTED) ? pNMLV->iItem : LB_ERR);
}

//////////////////////////////////////////////////////////////////////////

int CMonsterListDlg::GetCurSel()
{
	int sel = m_lstMonster.GetSelectionMark();
	if (!m_lstMonster.GetItemState(sel, LVIS_SELECTED))
		return LB_ERR;

	return sel;
}

BOOL CMonsterListDlg::CanSelect(int sel)
{
	ASSERT(m_bSelectMode);

	if (sel < 0)
		return FALSE;

	if (sel == m_nSelMonster)
		return TRUE;

	if (m_bDisablePartyMember && ((SHORT)LOWORD(m_lstMonster.GetItemData(sel)) >= 0))
		return FALSE;

	if (m_bDisableStandby && ((SHORT)HIWORD(m_lstMonster.GetItemData(sel)) >= 0))
		return FALSE;

	return TRUE;
}

void CMonsterListDlg::UpdateMonsterList()
{
	DQMJ_SAVE_RANCH_INFO ranchinfo;
	VERIFY(::DQMJSaveQueryRanchInfo(theApp.GetSaveHandle(), &ranchinfo));

	for (int i = 0; i < ranchinfo.monster_num; i++)
	{
		DQMJ_SAVE_MONSTER_INFO monsterinfo;
		VERIFY(m_lstMonster.InsertItem(i, _T("")) == i);
		VERIFY(::DQMJSaveQueryMonsterInfo(theApp.GetSaveHandle(), i, &monsterinfo));

		m_lstMonster.SetItemData(i, MAKELONG(-1, -1));
		m_lstMonster.SetItemText(i, COLUMN_NO, theApp.GetDecimalString(i + 1));
		m_lstMonster.SetItemText(i, COLUMN_NAME, theApp.GetNameString(monsterinfo.name));
		m_lstMonster.SetItemText(i, COLUMN_SEX, theApp.GetSexString(monsterinfo.sex));
		m_lstMonster.SetItemText(i, COLUMN_RACE, theApp.GetRaceString(monsterinfo.race));
		m_lstMonster.SetItemText(i, COLUMN_LEVEL, theApp.GetDecimalString(monsterinfo.level, monsterinfo.max_level));
		m_lstMonster.SetItemText(i, COLUMN_RANK, theApp.GetRankString(monsterinfo.rank));
		m_lstMonster.SetItemText(i, COLUMN_TYPE, theApp.GetTypeString(monsterinfo.type));
		m_lstMonster.SetItemText(i, COLUMN_WEAPON, theApp.GetItemString(monsterinfo.weapon));

		if (m_bSelectMode && (i == m_nSelMonster))
		{
			m_lstMonster.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_lstMonster.SetSelectionMark(i);
		}
	}

	for (int i = 0; i < ranchinfo.party_member_num; i++)
	{
		m_lstMonster.SetItemData(ranchinfo.party_member_idx[i], MAKELONG(i, -1));
		m_lstMonster.SetItemText(ranchinfo.party_member_idx[i], COLUMN_PARTY, theApp.GetDecimalString(i + 1));
	}

	for (int i = 0; i < ranchinfo.standby_num; i++)
	{
		m_lstMonster.SetItemData(ranchinfo.standby_idx[i], MAKELONG(-1, i));
		m_lstMonster.SetItemText(ranchinfo.standby_idx[i], COLUMN_STANDBY, theApp.GetDecimalString(i + 1));
	}

	SetDlgItemText(IDC_MONSTER_NUM, theApp.GetDecimalString(m_lstMonster.GetItemCount(), DQMJ_MONSTER_MAX));
}

void CMonsterListDlg::RedrawMonsterList(int sel)
{
	ASSERT(!m_bSelectMode);

	m_lstMonster.SetRedraw(FALSE);
	m_lstMonster.DeleteAllItems();
	UpdateMonsterList();

	if (sel >= m_lstMonster.GetItemCount())
		sel = LB_ERR;
	else
		m_lstMonster.SetItemState(sel, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	m_lstMonster.SetSelectionMark(sel);
	m_lstMonster.SetRedraw(TRUE);

	m_lstMonster.SetFocus();
	RefreshMonsterSel(sel);
}

void CMonsterListDlg::RefreshMonsterSel(int sel)
{
	ASSERT(!m_bSelectMode);

	int item_count = m_lstMonster.GetItemCount();

	GetDlgItem(IDC_BUTTON_MONSTER_UP)->EnableWindow(sel > 0);
	GetDlgItem(IDC_BUTTON_MONSTER_TOP)->EnableWindow(sel > 0);
	GetDlgItem(IDC_BUTTON_MONSTER_DOWN)->EnableWindow((sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_MONSTER_BOTTOM)->EnableWindow((sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_MONSTER_ADD)->EnableWindow(item_count < DQMJ_MONSTER_MAX);
	GetDlgItem(IDC_BUTTON_MONSTER_MODIFY)->EnableWindow(sel >= 0);
	GetDlgItem(IDC_BUTTON_MONSTER_REMOVE)->EnableWindow((sel >= 0) && (item_count > 1));
	GetDlgItem(IDC_BUTTON_MONSTER_COPY)->EnableWindow((sel >= 0) && (item_count < DQMJ_MONSTER_MAX));
}

void CMonsterListDlg::AddMonster()
{
	ASSERT(!m_bSelectMode);

	DQMJ_SAVE_MONSTER_INFO monsterinfo;
	::ZeroMemory(&monsterinfo, sizeof(monsterinfo));

	CMonsterInfoDlg dlg(monsterinfo, FALSE, this);
	dlg.DoModal();
	monsterinfo = dlg.GetMonsterInfo();

	VERIFY(::DQMJSaveNewMonster(theApp.GetSaveHandle(), &monsterinfo, FALSE));
	RedrawMonsterList(0);
}

void CMonsterListDlg::ModifyMonster(int sel)
{
	ASSERT(!m_bSelectMode);

	if (sel < 0)
		return;

	DQMJ_SAVE_MONSTER_INFO monsterinfo;
	VERIFY(::DQMJSaveQueryMonsterInfo(theApp.GetSaveHandle(), sel, &monsterinfo));

	int party_idx = (SHORT)LOWORD(m_lstMonster.GetItemData(sel));
	int standby_idx = (SHORT)HIWORD(m_lstMonster.GetItemData(sel));

	CMonsterInfoDlg dlg(monsterinfo, (party_idx >= 0) || (standby_idx >= 0), this);
	dlg.DoModal();
	monsterinfo = dlg.GetMonsterInfo();

	m_lstMonster.SetFocus();
	RefreshMonsterSel(sel);
}

void CMonsterListDlg::RemoveMonster(int sel)
{
	ASSERT(!m_bSelectMode);

	if (sel < 0)
		return;

	VERIFY(::DQMJSaveRemoveMonster(theApp.GetSaveHandle(), sel));
	RedrawMonsterList(sel);
}

void CMonsterListDlg::CopyMonster(int sel)
{
	ASSERT(!m_bSelectMode);

	if (sel < 0)
		return;

	VERIFY(::DQMJSaveCopyMonster(theApp.GetSaveHandle(), sel));
	RedrawMonsterList(0);
}

void CMonsterListDlg::MoveMonster(int sel, int idx)
{
	ASSERT(!m_bSelectMode);

	if (sel < 0)
		return;

	VERIFY(::DQMJSaveMoveMonster(theApp.GetSaveHandle(), sel, idx));
	RedrawMonsterList(idx);
}

//////////////////////////////////////////////////////////////////////////
