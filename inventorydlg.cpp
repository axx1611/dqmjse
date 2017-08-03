
#include "stdafx.h"
#include "dqmjse.h"
#include "inventorydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CInventoryDlg, CDialog)

BEGIN_MESSAGE_MAP(CInventoryDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CInventoryDlg::CInventoryDlg(CWnd *pParent) : CDialog(IDD_DQMJSE_INVENTORY, pParent)
{
	m_nCtrlBorder = 0;
	m_nCtrlHeight = 0;
	m_nCtrlSpacing = 0;
	m_eFilter = FILTER_ALL;
	::ZeroMemory(m_itemNum, sizeof(m_itemNum));
	::ZeroMemory(m_itemCtrl, sizeof(m_itemCtrl));
}

//////////////////////////////////////////////////////////////////////////

void CInventoryDlg::SetFilter(FILTER filter)
{
	if (m_eFilter == filter)
		return;

	m_eFilter = filter;
	UpdateData(TRUE);
	RefreshFilter();
}

//////////////////////////////////////////////////////////////////////////

void CInventoryDlg::DoDataExchange(CDataExchange *pDX)
{
	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
	{
		if (m_itemCtrl[i][1] == 0)
			continue;

		::DDX_Text(pDX, m_itemCtrl[i][1], m_itemNum[i]);
		::DDV_MinMaxInt(pDX, m_itemNum[i], 0, DQMJ_ITEM_MAX);
	}

	CDialog::DoDataExchange(pDX);
}

BOOL CInventoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect1, rect2;
	GetDlgItem(IDC_EDIT_INVENTORY_ITEM1)->GetWindowRect(rect1);
	ScreenToClient(rect1);
	m_nCtrlBorder = rect1.top;
	GetDlgItem(IDC_EDIT_INVENTORY_ITEM1)->GetWindowRect(rect1);
	GetDlgItem(IDC_EDIT_INVENTORY_ITEM4)->GetWindowRect(rect2);
	m_nCtrlHeight = rect1.Height();
	m_nCtrlSpacing = rect2.top - rect1.top - m_nCtrlHeight;

	DQMJ_SAVE_ITEM_INFO iteminfo;
	VERIFY(::DQMJSaveQueryItemInfo(((CDqmjSEApp *)::AfxGetApp())->GetSaveHandle(), &iteminfo));

	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
		m_itemNum[i] = iteminfo.inventory[i];

	RefreshFilter();
	return TRUE;
}

void CInventoryDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE;
	si.nPage = cy;
	SetScrollInfo(SB_VERT, &si, FALSE);
}

void CInventoryDlg::OnSetFocus(CWnd *pOldWnd)
{
//	CDialog::OnSetFocus(pOldWnd);
}

void CInventoryDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	if (pScrollBar == NULL)
	{
		SCROLLINFO si;
		GetScrollInfo(SB_VERT, &si, SIF_ALL);

		int pos = si.nPos;
		switch (nSBCode)
		{
		case SB_TOP:
			pos = si.nMin;
			break;
		case SB_BOTTOM:
			pos = si.nMax - si.nPage;
			break;
		case SB_LINEUP:
			pos -= 22;
			break;
		case SB_LINEDOWN:
			pos += 22;
			break;
		case SB_PAGEUP:
			pos -= si.nPage;
			break;
		case SB_PAGEDOWN:
			pos += si.nPage;
			break;
		case SB_THUMBTRACK:
			pos = si.nTrackPos;
			break;
		}

		if (pos >= si.nMax - (int)si.nPage)
			pos = si.nMax - si.nPage;
		if (pos < si.nMin)
			pos = si.nMin;

		if (pos != si.nPos)
		{
			ScrollWindow(0, si.nPos - pos);
			SetScrollPos(SB_VERT, pos);
		}

		SetFocus();
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CInventoryDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}

void CInventoryDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	CDialog::OnRButtonDown(nFlags, point);
}

BOOL CInventoryDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (((nFlags & (MK_CONTROL | MK_SHIFT)) == 0) && (zDelta != 0))
	{
		SCROLLINFO si;
		GetScrollInfo(SB_VERT, &si, SIF_ALL);

		int pos = si.nPos - zDelta / 3;

		if (pos >= si.nMax - (int)si.nPage)
			pos = si.nMax - si.nPage;
		if (pos < si.nMin)
			pos = si.nMin;

		if (pos != si.nPos)
		{
			ScrollWindow(0, si.nPos - pos);
			SetScrollPos(SB_VERT, pos);
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

//////////////////////////////////////////////////////////////////////////

void CInventoryDlg::RefreshFilter()
{
#define M(i)	{ IDC_INVENTORY_ITEM##i, IDC_EDIT_INVENTORY_ITEM##i, IDC_SPIN_INVENTORY_ITEM##i }
	static int ctrl_id[][3] =
	{
		M(1), M(2), M(3), M(4), M(5), M(6), M(7), M(8), M(9), M(10), M(11), M(12), M(13), M(14), M(15), M(16), M(17), M(18), M(19), M(20),
		M(21), M(22), M(23), M(24), M(25), M(26), M(27), M(28), M(29), M(30), M(31), M(32), M(33), M(34), M(35), M(36), M(37), M(38), M(39),
		M(40), M(41), M(42), M(43), M(44), M(45), M(46), M(47), M(48), M(49), M(50), M(51), M(52), M(53), M(54), M(55), M(56), M(57), M(58),
		M(59), M(60), M(61), M(62), M(63), M(64), M(65), M(66), M(67), M(68), M(69), M(70), M(71), M(72), M(73), M(74), M(75), M(76), M(77),
		M(78), M(79), M(80), M(81), M(82), M(83), M(84), M(85), M(86), M(87), M(88), M(89), M(90), M(91), M(92), M(93), M(94), M(95), M(96),
		M(97), M(98), M(99), M(100), M(101), M(102), M(103), M(104), M(105), M(106), M(107), M(108), M(109), M(110), M(111), M(112), M(113),
		M(114), M(115), M(116), M(117), M(118), M(119), M(120), M(121), M(122), M(123), M(124), M(125), M(126),
	};
#undef M

	int num = 0;
	::ZeroMemory(m_itemCtrl, sizeof(m_itemCtrl));

	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
	{
		DQMJ_SAVE_ITEM_SETTING is;
		if (!::DQMJSaveGetItemSetting(i, &is))
			continue;

		switch (m_eFilter)
		{
		case FILTER_ALL:
			break;
		case FILTER_USABLE:
			if (is.type != DQMJ_ITEM_USABLE)
				continue;
			break;
		case FILTER_NORMAL:
			if (is.type != DQMJ_ITEM_NORMAL)
				continue;
			break;
		case FILTER_SPECIAL:
			if (is.type != DQMJ_ITEM_SPECIAL)
				continue;
			break;
		case FILTER_EQUIPMENT:
			if (!is.equipment)
				continue;
			break;
		case FILTER_SWORD:
			if (is.type != DQMJ_ITEM_SWORD)
				continue;
			break;
		case FILTER_SPEAR:
			if (is.type != DQMJ_ITEM_SPEAR)
				continue;
			break;
		case FILTER_AXE:
			if (is.type != DQMJ_ITEM_AXE)
				continue;
			break;
		case FILTER_HAMMER:
			if (is.type != DQMJ_ITEM_HAMMER)
				continue;
			break;
		case FILTER_WHIP:
			if (is.type != DQMJ_ITEM_WHIP)
				continue;
			break;
		case FILTER_CLAW:
			if (is.type != DQMJ_ITEM_CLAW)
				continue;
			break;
		case FILTER_STAFF:
			if (is.type != DQMJ_ITEM_STAFF)
				continue;
			break;
		default:
			continue;
		}

		m_itemCtrl[i][0] = ctrl_id[num][0];
		m_itemCtrl[i][1] = ctrl_id[num][1];
		m_itemCtrl[i][2] = ctrl_id[num][2];
		num++;

		GetDlgItem(m_itemCtrl[i][0])->ShowWindow(SW_SHOWNOACTIVATE);
		GetDlgItem(m_itemCtrl[i][1])->ShowWindow(SW_SHOWNOACTIVATE);
		GetDlgItem(m_itemCtrl[i][2])->ShowWindow(SW_SHOWNOACTIVATE);

		SetDlgItemText(m_itemCtrl[i][0], is.name);
		SetDlgItemInt(m_itemCtrl[i][1], m_itemNum[i]);
		((CEdit *)GetDlgItem(m_itemCtrl[i][1]))->SetLimitText(2);
		((CSpinButtonCtrl *)GetDlgItem(m_itemCtrl[i][2]))->SetRange32(0, DQMJ_ITEM_MAX);
	}

	for (int i = num; i < _countof(ctrl_id); i++)
	{
		GetDlgItem(ctrl_id[i][0])->ShowWindow(SW_HIDE);
		GetDlgItem(ctrl_id[i][1])->ShowWindow(SW_HIDE);
		GetDlgItem(ctrl_id[i][2])->ShowWindow(SW_HIDE);
	}

	ScrollWindow(0, GetScrollPos(SB_VERT));

	int row = (num + 2) / 3;
	SetScrollRange(SB_VERT, 0, row * m_nCtrlHeight + (row - 1) * m_nCtrlSpacing + m_nCtrlBorder * 2, FALSE);
	SetScrollPos(SB_VERT, 0, TRUE);
}

//////////////////////////////////////////////////////////////////////////
