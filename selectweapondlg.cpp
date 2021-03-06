
#include "stdafx.h"
#include "dqmjse.h"
#include "selectweapondlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSelectWeaponDlg, CDialog)

BEGIN_MESSAGE_MAP(CSelectWeaponDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WEAPON_LIST, OnNMDblclkListWeaponList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WEAPON_LIST, OnLvnItemchangedListWeaponList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CSelectWeaponDlg::CSelectWeaponDlg(const DQMJ_SAVE_MONSTER_INFO &monsterinfo, BOOL normalize, CWnd *pParent) : CDialog(IDD_DQMJSE_SELECT_WEAPON, pParent)
{
	m_bNormalize = normalize;
	m_nWeapon = monsterinfo.weapon;
	m_nRace = monsterinfo.race;
	m_abilityList = monsterinfo.ability_list;
}

//////////////////////////////////////////////////////////////////////////

void CSelectWeaponDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_LIST_WEAPON_LIST, m_lstWeapon);

	CDialog::DoDataExchange(pDX);
}

BOOL CSelectWeaponDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstWeapon.SetExtendedStyle(m_lstWeapon.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	VERIFY(m_lstWeapon.InsertColumn(COLUMN_NAME, _T("名前"), LVCFMT_LEFT, 86) == COLUMN_NAME);
	VERIFY(m_lstWeapon.InsertColumn(COLUMN_TYPE, _T("種別"), LVCFMT_CENTER, 46) == COLUMN_TYPE);

	m_lstWeapon.SetRedraw(FALSE);

	int idx = 0;
	const CIntList &list = theApp.GetWeaponList();
	POSITION pos = list.GetHeadPosition();
	while (pos)
	{
		int item = list.GetNext(pos);

		DQMJ_SAVE_ITEM_SETTING is;
		if (!::DQMJSaveGetItemSetting(item, &is))
			continue;

		if (m_bNormalize && !::DQMJSaveCheckEquippable(m_nRace, item, m_abilityList))
			continue;

		VERIFY(m_lstWeapon.InsertItem(idx, _T("")) == idx);

		m_lstWeapon.SetItemData(idx, item);
		m_lstWeapon.SetItemText(idx, COLUMN_NAME, is.name);
		m_lstWeapon.SetItemText(idx, COLUMN_TYPE, GetWeaponTypeString(is.type));

		if (item == m_nWeapon)
		{
			m_lstWeapon.SetItemState(idx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_lstWeapon.SetSelectionMark(idx);
			m_lstWeapon.EnsureVisible(idx, FALSE);
		}

		idx++;
	}

	m_lstWeapon.SetRedraw(TRUE);

	int sel = GetCurSel();
	if (sel >= 0)
	{
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else
	{
		m_nWeapon = 0x00;
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}

	return TRUE;
}

void CSelectWeaponDlg::OnOK()
{
	int sel = GetCurSel();
	if (sel < 0)
		return;

	m_nWeapon = m_lstWeapon.GetItemData(sel);
	CDialog::OnOK();
}

void CSelectWeaponDlg::OnNMDblclkListWeaponList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	OnOK();
}

void CSelectWeaponDlg::OnLvnItemchangedListWeaponList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (!IsWindowVisible())
		return;

	GetDlgItem(IDOK)->EnableWindow(pNMLV->uNewState & LVIS_SELECTED);
}

//////////////////////////////////////////////////////////////////////////

int CSelectWeaponDlg::GetCurSel()
{
	int sel = m_lstWeapon.GetSelectionMark();
	if (!m_lstWeapon.GetItemState(sel, LVIS_SELECTED))
		return LB_ERR;

	return sel;
}

LPCTSTR CSelectWeaponDlg::GetWeaponTypeString(int type)
{
	switch (type)
	{
	case DQMJ_ITEM_SWORD:
		return _T("剣");
	case DQMJ_ITEM_SPEAR:
		return _T("やり");
	case DQMJ_ITEM_AXE:
		return _T("オノ");
	case DQMJ_ITEM_HAMMER:
		return _T("ハンマー");
	case DQMJ_ITEM_WHIP:
		return _T("ムチ");
	case DQMJ_ITEM_CLAW:
		return _T("ツメ");
	case DQMJ_ITEM_STAFF:
		return _T("つえ");
	}

	return _T("不明");
}

//////////////////////////////////////////////////////////////////////////
