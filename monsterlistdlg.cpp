
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
	ON_BN_CLICKED(IDC_CHECK_GAME_ORDER, OnBnClickedCheckGameOrder)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_UP, OnBnClickedButtonMonsterUp)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_DOWN, OnBnClickedButtonMonsterDown)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_TOP, OnBnClickedButtonMonsterTop)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_BOTTOM, OnBnClickedButtonMonsterBottom)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ADD, OnBnClickedButtonMonsterAdd)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_MODIFY, OnBnClickedButtonMonsterModify)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_REMOVE, OnBnClickedButtonMonsterRemove)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_COPY, OnBnClickedButtonMonsterCopy)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MONSTER_LIST, OnNMDblclkListMonsterList)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_MONSTER_LIST, OnLvnItemchangingListMonsterList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CMonsterListDlg::CMonsterListDlg(CWnd *pParent) : CDialog(IDD_DQMJSE_MONSTER_LIST, pParent)
{
	m_bSelectMode = FALSE;
	m_bDisablePartyMember = FALSE;
	m_bDisableStandby = FALSE;
	m_nSelMonster = -1;
	m_nPartyIndex = -1;
	m_nStandbyIndex = -1;
}

void CMonsterListDlg::ConfigSelectMode(int monster_idx, BOOL disable_party_member, BOOL disable_standby)
{
	m_bSelectMode = TRUE;
	m_nSelMonster = monster_idx;
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

	if (m_bSelectMode)
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_MONSTER_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_TOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_DOWN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_BOTTOM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_MODIFY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_REMOVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MONSTER_COPY)->ShowWindow(SW_HIDE);

		CheckDlgButton(IDC_CHECK_GAME_ORDER, BST_CHECKED);
	}
	else
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

		CheckDlgButton(IDC_CHECK_GAME_ORDER, BST_UNCHECKED);
	}

	m_lstMonster.SetRedraw(FALSE);
	UpdateMonsterList();
	m_lstMonster.SetRedraw(TRUE);

	int sel = GetCurSel();
	if (sel < 0)
		m_nSelMonster = -1;

	RefreshMonsterSel(sel);

	return TRUE;
}

void CMonsterListDlg::OnOK()
{
	if (!m_bSelectMode)
		return;

	int sel = GetCurSel();
	if (!CanSelect(sel))
		return;

	MONSTER_INDEXES indexes = GetMonsterIndexes(sel);
	m_nSelMonster = indexes.monster_idx;
	m_nPartyIndex = indexes.party_member_idx;
	m_nStandbyIndex = indexes.standby_idx;

	CDialog::OnOK();
}

void CMonsterListDlg::OnBnClickedCheckGameOrder()
{
	RedrawMonsterList(GetCurSel());
}

void CMonsterListDlg::OnBnClickedButtonMonsterUp()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED))
		return;

	int sel = GetCurSel();
	if (sel <= 0)
		return;

	MoveMonster(sel, sel - 1);
}

void CMonsterListDlg::OnBnClickedButtonMonsterDown()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED))
		return;

	int sel = GetCurSel();
	if ((sel < 0) || (sel >= m_lstMonster.GetItemCount() - 1))
		return;

	MoveMonster(sel, sel + 1);
}

void CMonsterListDlg::OnBnClickedButtonMonsterTop()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED))
		return;

	int sel = GetCurSel();
	if (sel <= 0)
		return;

	MoveMonster(sel, 0);
}

void CMonsterListDlg::OnBnClickedButtonMonsterBottom()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED))
		return;

	int sel = GetCurSel();
	int bottom_idx = m_lstMonster.GetItemCount() - 1;
	if ((sel < 0) || (sel >= bottom_idx))
		return;

	MoveMonster(sel, bottom_idx);
}

void CMonsterListDlg::OnBnClickedButtonMonsterAdd()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED) || (m_lstMonster.GetItemCount() >= DQMJ_MONSTER_MAX))
		return;

	AddMonster();
}

void CMonsterListDlg::OnBnClickedButtonMonsterModify()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED))
		return;

	int sel = GetCurSel();
	if (sel < 0)
		return;

	ModifyMonster(sel);
}

void CMonsterListDlg::OnBnClickedButtonMonsterRemove()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED) || (m_lstMonster.GetItemCount() <= 1))
		return;

	int sel = GetCurSel();
	if (sel < 0)
		return;

	RemoveMonster(sel);
}

void CMonsterListDlg::OnBnClickedButtonMonsterCopy()
{
	if (m_bSelectMode || (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED) || (m_lstMonster.GetItemCount() >= DQMJ_MONSTER_MAX))
		return;

	int sel = GetCurSel();
	if (sel < 0)
		return;

	CopyMonster(sel);
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

	if (!IsWindowVisible())
		return;

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

	MONSTER_INDEXES indexes = GetMonsterIndexes(sel);

	if (m_nSelMonster == indexes.monster_idx)
		return TRUE;

	if (m_bDisablePartyMember && (indexes.party_member_idx >= 0))
		return FALSE;

	if (m_bDisableStandby && (indexes.standby_idx >= 0))
		return FALSE;

	return TRUE;
}

int CMonsterListDlg::CheckMonsterInParty(int monster_idx, const DQMJ_SAVE_RANCH_INFO &ranchinfo)
{
	for (int i = 0; i < ranchinfo.party_member_num; i++)
	{
		if (monster_idx == ranchinfo.party_member_idx[i])
			return i;
	}

	return -1;
}

int CMonsterListDlg::CheckMonsterStandby(int monster_idx, const DQMJ_SAVE_RANCH_INFO &ranchinfo)
{
	for (int i = 0; i < ranchinfo.standby_num; i++)
	{
		if (monster_idx == ranchinfo.standby_idx[i])
			return i;
	}

	return -1;
}

CMonsterListDlg::MONSTER_INDEXES CMonsterListDlg::GetMonsterIndexes(int idx)
{
	if ((idx < 0) || (idx >= m_lstMonster.GetItemCount()))
	{
		MONSTER_INDEXES indexes;
		indexes.monster_idx = -1;
		indexes.party_member_idx = -1;
		indexes.standby_idx = -1;
		return indexes;
	}

	ITEM_DATA itemdata;
	itemdata.data = m_lstMonster.GetItemData(idx);
	return itemdata.indexes;
}

void CMonsterListDlg::SetMonsterIndexes(int idx, int monster_idx, int party_member_idx, int standby_idx)
{
	ASSERT(idx >= 0);

	ITEM_DATA itemdata;
	itemdata.data = 0;
	itemdata.indexes.monster_idx = monster_idx;
	itemdata.indexes.party_member_idx = party_member_idx;
	itemdata.indexes.standby_idx = standby_idx;

	m_lstMonster.SetItemData(idx, itemdata.data);
}

void CMonsterListDlg::UpdateMonsterList()
{
	DQMJ_SAVE_RANCH_INFO ranchinfo;
	VERIFY(::DQMJSaveQueryRanchInfo(theApp.GetSaveHandle(), &ranchinfo));

	if (IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED)
	{
		for (int i = 0; i < ranchinfo.party_member_num; i++)
			InsertMonsterItem(ranchinfo.party_member_idx[i], ranchinfo);

		for (int i = 0; i < ranchinfo.monster_num; i++)
		{
			if (CheckMonsterInParty(i, ranchinfo) < 0)
				InsertMonsterItem(i, ranchinfo);
		}
	}
	else
	{
		for (int i = 0; i < ranchinfo.monster_num; i++)
			InsertMonsterItem(i, ranchinfo);
	}

	SetDlgItemText(IDC_MONSTER_NUM, theApp.GetDecimalString(m_lstMonster.GetItemCount(), DQMJ_MONSTER_MAX));
}

void CMonsterListDlg::RedrawMonsterList(int sel)
{
	m_nSelMonster = GetMonsterIndexes(sel).monster_idx;

	m_lstMonster.SetRedraw(FALSE);
	m_lstMonster.DeleteAllItems();
	UpdateMonsterList();
	m_lstMonster.SetRedraw(TRUE);

	m_lstMonster.SetFocus();
	RefreshMonsterSel(GetCurSel());
}

void CMonsterListDlg::RefreshMonsterSel(int sel)
{
	if (m_bSelectMode)
		return;

	int item_count = m_lstMonster.GetItemCount();
	BOOL game_order = IsDlgButtonChecked(IDC_CHECK_GAME_ORDER) == BST_CHECKED;

	GetDlgItem(IDC_BUTTON_MONSTER_UP)->EnableWindow(!game_order && (sel > 0));
	GetDlgItem(IDC_BUTTON_MONSTER_TOP)->EnableWindow(!game_order && (sel > 0));
	GetDlgItem(IDC_BUTTON_MONSTER_DOWN)->EnableWindow(!game_order && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_MONSTER_BOTTOM)->EnableWindow(!game_order && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_MONSTER_ADD)->EnableWindow(!game_order && (item_count < DQMJ_MONSTER_MAX));
	GetDlgItem(IDC_BUTTON_MONSTER_MODIFY)->EnableWindow(sel >= 0);
	GetDlgItem(IDC_BUTTON_MONSTER_REMOVE)->EnableWindow(!game_order && (sel >= 0) && (item_count > 1));
	GetDlgItem(IDC_BUTTON_MONSTER_COPY)->EnableWindow(!game_order && (sel >= 0) && (item_count < DQMJ_MONSTER_MAX));
}

void CMonsterListDlg::InsertMonsterItem(int monster_idx, const DQMJ_SAVE_RANCH_INFO &ranchinfo)
{
	DQMJ_SAVE_MONSTER_INFO monsterinfo;
	int idx = m_lstMonster.InsertItem(m_lstMonster.GetItemCount(), _T(""));
	VERIFY(::DQMJSaveQueryMonsterInfo(theApp.GetSaveHandle(), monster_idx, &monsterinfo));

	m_lstMonster.SetItemText(idx, COLUMN_NO, theApp.GetDecimalString(monster_idx + 1));
	m_lstMonster.SetItemText(idx, COLUMN_NAME, theApp.GetNameString(monsterinfo.name));
	m_lstMonster.SetItemText(idx, COLUMN_SEX, theApp.GetSexString(monsterinfo.sex));
	m_lstMonster.SetItemText(idx, COLUMN_RACE, theApp.GetRaceString(monsterinfo.race));
	m_lstMonster.SetItemText(idx, COLUMN_LEVEL, theApp.GetDecimalString(monsterinfo.level, monsterinfo.max_level));
	m_lstMonster.SetItemText(idx, COLUMN_RANK, theApp.GetRankString(monsterinfo.rank));
	m_lstMonster.SetItemText(idx, COLUMN_TYPE, theApp.GetTypeString(monsterinfo.type));
	m_lstMonster.SetItemText(idx, COLUMN_WEAPON, theApp.GetItemString(monsterinfo.weapon));

	if (monster_idx == m_nSelMonster)
	{
		m_lstMonster.SetItemState(idx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_lstMonster.SetSelectionMark(idx);
		m_lstMonster.EnsureVisible(idx, FALSE);
	}

	int party_member_idx = CheckMonsterInParty(monster_idx, ranchinfo);
	if (party_member_idx >= 0)
	{
		SetMonsterIndexes(idx, monster_idx, party_member_idx, -1);
		m_lstMonster.SetItemText(idx, COLUMN_PARTY, theApp.GetDecimalString(party_member_idx + 1));
		return;
	}

	int standby_idx = CheckMonsterStandby(monster_idx, ranchinfo);
	if (standby_idx >= 0)
	{
		SetMonsterIndexes(idx, monster_idx, -1, standby_idx);
		m_lstMonster.SetItemText(idx, COLUMN_STANDBY, theApp.GetDecimalString(standby_idx + 1));
		return;
	}

	SetMonsterIndexes(idx, monster_idx, -1, -1);
}

void CMonsterListDlg::AddMonster()
{
	ASSERT(!m_bSelectMode);

	CMonsterInfoDlg dlg(NULL, FALSE, this);
	if (dlg.DoModal() != IDOK)
	{
		m_lstMonster.SetFocus();
		return;
	}

	VERIFY(::DQMJSaveNewMonster(theApp.GetSaveHandle(), &dlg.GetMonsterInfo(), FALSE));
	RedrawMonsterList(0);
}

void CMonsterListDlg::ModifyMonster(int sel)
{
	ASSERT(!m_bSelectMode);

	if (sel < 0)
		return;

	DQMJ_SAVE_MONSTER_INFO monsterinfo;
	VERIFY(::DQMJSaveQueryMonsterInfo(theApp.GetSaveHandle(), sel, &monsterinfo));

	MONSTER_INDEXES indexes = GetMonsterIndexes(sel);
	CMonsterInfoDlg dlg(&monsterinfo, (indexes.party_member_idx >= 0) || (indexes.standby_idx >= 0), this);
	if (dlg.DoModal() != IDOK)
	{
		m_lstMonster.SetFocus();
		return;
	}

	VERIFY(::DQMJSaveModifyMonster(theApp.GetSaveHandle(), sel, &dlg.GetMonsterInfo(), FALSE));
	RedrawMonsterList(sel);
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
