
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

int CSelectUsableDlg::GetSelItem() const
{
	return m_nSelItem;
}

//////////////////////////////////////////////////////////////////////////

void CSelectUsableDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_LIST_ITEM_LIST, m_lstItemList);

	CDialog::DoDataExchange(pDX);
}

BOOL CSelectUsableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
	{
		DQMJ_SAVE_ITEM_SETTING is;
		if (!::DQMJSaveGetItemSetting(i, &is))
			continue;

		if (is.type != DQMJ_ITEM_USABLE)
			continue;

		int index = m_lstItemList.AddString(is.name);
		ASSERT(index >= 0);

		m_lstItemList.SetItemData(index, i);
		if (i == m_nSelItem)
			m_lstItemList.SetCurSel(index);
	}

	if ((m_nSelItem != 0x00) && (m_lstItemList.GetCurSel() < 0))
		m_nSelItem = 0x00;

	RefreshEnable();

	return TRUE;
}

void CSelectUsableDlg::OnOK()
{
	int sel = m_lstItemList.GetCurSel();
	if (sel < 0)
		return;

	m_nSelItem = m_lstItemList.GetItemData(sel);
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
	GetDlgItem(IDOK)->EnableWindow(m_lstItemList.GetCurSel() >= 0);
}

//////////////////////////////////////////////////////////////////////////
