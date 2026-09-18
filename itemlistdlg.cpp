
#include "stdafx.h"
#include "dqmjse.h"
#include "itemlistdlg.h"
#include "selectusabledlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CItemListDlg, CDialog)

BEGIN_MESSAGE_MAP(CItemListDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_UP, OnBnClickedButtonBaggageUp)
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_DOWN, OnBnClickedButtonBaggageDown)
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_TOP, OnBnClickedButtonBaggageTop)
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_BOTTOM, OnBnClickedButtonBaggageBottom)
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_ADD, OnBnClickedButtonBaggageAdd)
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_MODIFY, OnBnClickedButtonBaggageModify)
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_REMOVE, OnBnClickedButtonBaggageRemove)
	ON_BN_CLICKED(IDC_BUTTON_BAGGAGE_SORT, OnBnClickedButtonBaggageSort)
	ON_LBN_SELCHANGE(IDC_LIST_BAGGAGE_LIST, OnLbnSelchangeListBaggageList)
	ON_LBN_DBLCLK(IDC_LIST_BAGGAGE_LIST, OnLbnDblclkListBaggageList)
	ON_CBN_SELCHANGE(IDC_COMBO_INVENTORY_FILTER, OnCbnSelchangeComboInventoryFilter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CItemListDlg::CItemListDlg(CWnd *pParent) : CDialog(IDD_DQMJSE_ITEM_LIST, pParent)
{

}

//////////////////////////////////////////////////////////////////////////

void CItemListDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Control(pDX, IDC_LIST_BAGGAGE_LIST, m_lstBaggage);
	::DDX_Control(pDX, IDC_COMBO_INVENTORY_FILTER, m_cmbFilter);

	CDialog::DoDataExchange(pDX);
}

BOOL CItemListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbFilter.SetRedraw(FALSE);
	VERIFY(m_cmbFilter.AddString(_T("すべて")) == CInventoryPanelDlg::FILTER_ALL);
	VERIFY(m_cmbFilter.AddString(_T("使用可能")) == CInventoryPanelDlg::FILTER_USABLE);
	VERIFY(m_cmbFilter.AddString(_T("一般道具")) == CInventoryPanelDlg::FILTER_NORMAL);
	VERIFY(m_cmbFilter.AddString(_T("特殊道具")) == CInventoryPanelDlg::FILTER_SPECIAL);
	VERIFY(m_cmbFilter.AddString(_T("武器")) == CInventoryPanelDlg::FILTER_WEAPON);
	VERIFY(m_cmbFilter.AddString(_T("剣")) == CInventoryPanelDlg::FILTER_SWORD);
	VERIFY(m_cmbFilter.AddString(_T("やり")) == CInventoryPanelDlg::FILTER_SPEAR);
	VERIFY(m_cmbFilter.AddString(_T("オノ")) == CInventoryPanelDlg::FILTER_AXE);
	VERIFY(m_cmbFilter.AddString(_T("ハンマー")) == CInventoryPanelDlg::FILTER_HAMMER);
	VERIFY(m_cmbFilter.AddString(_T("ムチ")) == CInventoryPanelDlg::FILTER_WHIP);
	VERIFY(m_cmbFilter.AddString(_T("ツメ")) == CInventoryPanelDlg::FILTER_CLAW);
	VERIFY(m_cmbFilter.AddString(_T("つえ")) == CInventoryPanelDlg::FILTER_STAFF);
	m_cmbFilter.SetRedraw(TRUE);
	m_cmbFilter.SetCurSel(CInventoryPanelDlg::FILTER_ALL);

	CRect rect;
	GetDlgItem(IDC_STATIC_RECT)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_dlgPanel.Create(IDD_DQMJSE_INVENTORY_PANEL, this);
	m_dlgPanel.MoveWindow(&rect, FALSE);
	m_dlgPanel.ShowWindow(SW_SHOW);

	DQMJ_SAVE_ITEM_INFO iteminfo;
	VERIFY(::DQMJSaveQueryItemInfo(theApp.GetSaveHandle(), &iteminfo));

	for (int i = 0; i < DQMJ_BAGGAGE_MAX; i++)
	{
		if (iteminfo.baggage[i] != 0x00)
			VERIFY(m_lstBaggage.SetItemData(m_lstBaggage.AddString(theApp.GetItemString(iteminfo.baggage[i])), iteminfo.baggage[i]) != LB_ERR);
	}

	SetDlgItemText(IDC_BAGGAGE_NUM, theApp.GetDecimalString(m_lstBaggage.GetCount(), DQMJ_BAGGAGE_MAX));
	RefreshBaggageSel(LB_ERR);

	return TRUE;
}

void CItemListDlg::OnOK()
{
	UpdateData(TRUE);
}

void CItemListDlg::OnCancel()
{
	m_dlgPanel.UpdateData(TRUE);

	DQMJ_SAVE_ITEM_INFO iteminfo;
	VERIFY(::DQMJSaveQueryItemInfo(theApp.GetSaveHandle(), &iteminfo));

	::ZeroMemory(iteminfo.baggage, sizeof(iteminfo.baggage));
	::ZeroMemory(iteminfo.inventory, sizeof(iteminfo.inventory));

	for (int i = 0; i < m_lstBaggage.GetCount(); i++)
		iteminfo.baggage[i] = m_lstBaggage.GetItemData(i);

	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
		iteminfo.inventory[i] = m_dlgPanel.GetItemNum(i);

	VERIFY(::DQMJSaveModifyItemInfo(theApp.GetSaveHandle(), &iteminfo));

	CDialog::OnCancel();
}

void CItemListDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}

void CItemListDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnRButtonDown(nFlags, point);
}

void CItemListDlg::OnBnClickedButtonBaggageUp()
{
	if (m_lstBaggage.GetSelCount() != 1)
		return;

	int sel;
	VERIFY(m_lstBaggage.GetSelItems(1, &sel) == 1);
	if (sel == 0)
		return;

	CString text;
	m_lstBaggage.GetText(sel, text);
	int item = m_lstBaggage.GetItemData(sel);
	m_lstBaggage.DeleteString(sel);

	sel--;
	VERIFY(m_lstBaggage.InsertString(sel, text) == sel);
	m_lstBaggage.SetItemData(sel, item);

	m_lstBaggage.SetSel(sel);
	RefreshBaggageSel(sel);
}

void CItemListDlg::OnBnClickedButtonBaggageDown()
{
	if (m_lstBaggage.GetSelCount() != 1)
		return;

	int sel;
	VERIFY(m_lstBaggage.GetSelItems(1, &sel) == 1);
	if (sel >= m_lstBaggage.GetCount() - 1)
		return;

	CString text;
	m_lstBaggage.GetText(sel, text);
	int item = m_lstBaggage.GetItemData(sel);
	m_lstBaggage.DeleteString(sel);

	sel++;
	VERIFY(m_lstBaggage.InsertString(sel, text) == sel);
	m_lstBaggage.SetItemData(sel, item);

	m_lstBaggage.SetSel(sel);
	RefreshBaggageSel(sel);
}

void CItemListDlg::OnBnClickedButtonBaggageTop()
{
	if (m_lstBaggage.GetSelCount() != 1)
		return;

	int sel;
	if ((m_lstBaggage.GetSelItems(1, &sel) != 1) || (sel == 0))
		return;

	CString text;
	m_lstBaggage.GetText(sel, text);
	int item = m_lstBaggage.GetItemData(sel);
	m_lstBaggage.DeleteString(sel);

	sel = 0;
	VERIFY(m_lstBaggage.InsertString(sel, text) == sel);
	m_lstBaggage.SetItemData(sel, item);

	m_lstBaggage.SetSel(sel);
	RefreshBaggageSel(sel);
}

void CItemListDlg::OnBnClickedButtonBaggageBottom()
{
	if (m_lstBaggage.GetSelCount() != 1)
		return;

	int sel;
	if ((m_lstBaggage.GetSelItems(1, &sel) != 1) || (sel >= m_lstBaggage.GetCount() - 1))
		return;

	CString text;
	m_lstBaggage.GetText(sel, text);
	int item = m_lstBaggage.GetItemData(sel);
	m_lstBaggage.DeleteString(sel);

	sel = m_lstBaggage.GetCount();
	VERIFY(m_lstBaggage.InsertString(sel, text) == sel);
	m_lstBaggage.SetItemData(sel, item);

	m_lstBaggage.SetSel(sel);
	RefreshBaggageSel(sel);
}

void CItemListDlg::OnBnClickedButtonBaggageAdd()
{
	if (m_lstBaggage.GetCount() >= DQMJ_BAGGAGE_MAX)
		return;

	RefreshBaggageSel(SelectUsable());
}

void CItemListDlg::OnBnClickedButtonBaggageModify()
{
	if (m_lstBaggage.GetSelCount() != 1)
		return;

	int sel;
	if (m_lstBaggage.GetSelItems(1, &sel) != 1)
		return;

	RefreshBaggageSel(SelectUsable(sel));
}

void CItemListDlg::OnBnClickedButtonBaggageRemove()
{
	if (m_lstBaggage.GetSelCount() < 0)
		return;

	for (int i = DQMJ_BAGGAGE_MAX - 1; i >= 0; i--)
	{
		if (m_lstBaggage.GetSel(i) > 0)
			m_lstBaggage.DeleteString(i);
	}

	SetDlgItemText(IDC_BAGGAGE_NUM, theApp.GetDecimalString(m_lstBaggage.GetCount(), DQMJ_BAGGAGE_MAX));
	RefreshBaggageSel(LB_ERR);
}

void CItemListDlg::OnBnClickedButtonBaggageSort()
{
	if (m_lstBaggage.GetCount() <= 0)
		return;

	int item[DQMJ_BAGGAGE_MAX];
	::ZeroMemory(item, sizeof(item));

	int len = m_lstBaggage.GetCount();
	for (int i = 0; i < len; i++)
		item[i] = m_lstBaggage.GetItemData(i);

	std::sort(item, item + len);
	m_lstBaggage.ResetContent();

	for (int i = 0; i < len; i++)
	{
		int idx = m_lstBaggage.AddString(theApp.GetItemString(item[i]));
		ASSERT(idx >= 0);
		m_lstBaggage.SetItemData(idx, item[i]);
	}

	RefreshBaggageSel(LB_ERR);
}

void CItemListDlg::OnLbnSelchangeListBaggageList()
{
	int sel;
	if (m_lstBaggage.GetSelItems(1, &sel) <= 0)
		sel = LB_ERR;

	RefreshBaggageSel(sel);
}

void CItemListDlg::OnLbnDblclkListBaggageList()
{
	if (m_lstBaggage.GetSelCount() != 1)
		return;

	int sel;
	if (m_lstBaggage.GetSelItems(1, &sel) != 1)
		return;

	if (sel != m_lstBaggage.GetAnchorIndex())
		return;

	RefreshBaggageSel(SelectUsable(sel));
}

void CItemListDlg::OnCbnSelchangeComboInventoryFilter()
{
	int sel = m_cmbFilter.GetCurSel();
	if (sel >= 0)
		m_dlgPanel.SetFilter((CInventoryPanelDlg::FILTER)sel);
}

//////////////////////////////////////////////////////////////////////////

void CItemListDlg::RefreshBaggageSel(int sel)
{
	int item_count = m_lstBaggage.GetCount();
	int sel_count = m_lstBaggage.GetSelCount();

	GetDlgItem(IDC_BUTTON_BAGGAGE_UP)->EnableWindow((sel_count == 1) && (sel > 0));
	GetDlgItem(IDC_BUTTON_BAGGAGE_TOP)->EnableWindow((sel_count == 1) && (sel > 0));
	GetDlgItem(IDC_BUTTON_BAGGAGE_DOWN)->EnableWindow((sel_count == 1) && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_BAGGAGE_BOTTOM)->EnableWindow((sel_count == 1) && (sel >= 0) && (sel < item_count - 1));
	GetDlgItem(IDC_BUTTON_BAGGAGE_ADD)->EnableWindow(item_count < DQMJ_BAGGAGE_MAX);
	GetDlgItem(IDC_BUTTON_BAGGAGE_MODIFY)->EnableWindow((sel_count == 1) && (sel >= 0));
	GetDlgItem(IDC_BUTTON_BAGGAGE_REMOVE)->EnableWindow(sel_count > 0);
	GetDlgItem(IDC_BUTTON_BAGGAGE_SORT)->EnableWindow(item_count > 0);
}

int CItemListDlg::SelectUsable(int sel)
{
	CSelectUsableDlg dlg((sel < 0) ? 0x00 : m_lstBaggage.GetItemData(sel), this);
	if (dlg.DoModal() != IDOK)
		return sel;

	int item = dlg.GetSelItem();
	if (item == 0x00)
		return sel;

	for (int i = 0; i < m_lstBaggage.GetCount(); i++)
		m_lstBaggage.SetSel(i, FALSE);

	if (sel < 0)
	{
		sel = m_lstBaggage.AddString(theApp.GetItemString(item));
		ASSERT(sel >= 0);
		m_lstBaggage.SetItemData(sel, item);
		m_lstBaggage.SetSel(sel);
		SetDlgItemText(IDC_BAGGAGE_NUM, theApp.GetDecimalString(m_lstBaggage.GetCount(), DQMJ_BAGGAGE_MAX));
	}
	else
	{
		m_lstBaggage.DeleteString(sel);
		m_lstBaggage.InsertString(sel, theApp.GetItemString(item));
		m_lstBaggage.SetItemData(sel, item);
		m_lstBaggage.SetSel(sel);
	}

	return sel;
}

//////////////////////////////////////////////////////////////////////////
