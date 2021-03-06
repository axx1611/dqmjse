
#include "stdafx.h"
#include "dqmjse.h"
#include "selectusabledlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSelectUsableDlg, CDialog)

BEGIN_MESSAGE_MAP(CSelectUsableDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_ITEM_LIST, OnLbnSelchangeListItemList)
	ON_LBN_DBLCLK(IDC_LIST_ITEM_LIST, OnLbnDblclkListItemList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CSelectUsableDlg::CSelectUsableDlg(int item, CWnd *pParent) : CDialog(IDD_DQMJSE_SELECT_USABLE, pParent)
{
	m_nSelItem = item;
}

//////////////////////////////////////////////////////////////////////////

void CSelectUsableDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_LIST_ITEM_LIST, m_lstItem);

	CDialog::DoDataExchange(pDX);
}

BOOL CSelectUsableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstItem.SetRedraw(FALSE);

	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
	{
		DQMJ_SAVE_ITEM_SETTING is;
		if (!::DQMJSaveGetItemSetting(i, &is))
			continue;

		if (is.type != DQMJ_ITEM_USABLE)
			continue;

		int idx = m_lstItem.AddString(is.name);
		ASSERT(idx >= 0);

		m_lstItem.SetItemData(idx, i);
		if (i == m_nSelItem)
			m_lstItem.SetCurSel(idx);
	}

	m_lstItem.SetRedraw(TRUE);

	if ((m_nSelItem != 0x00) && (m_lstItem.GetCurSel() < 0))
		m_nSelItem = 0x00;

	RefreshEnable();

	return TRUE;
}

void CSelectUsableDlg::OnOK()
{
	int sel = m_lstItem.GetCurSel();
	if (sel < 0)
		return;

	m_nSelItem = m_lstItem.GetItemData(sel);
	CDialog::OnOK();
}

void CSelectUsableDlg::OnLbnSelchangeListItemList()
{
	RefreshEnable();
}

void CSelectUsableDlg::OnLbnDblclkListItemList()
{
	OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CSelectUsableDlg::RefreshEnable()
{
	GetDlgItem(IDOK)->EnableWindow(m_lstItem.GetCurSel() >= 0);
}

//////////////////////////////////////////////////////////////////////////
