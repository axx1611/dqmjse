
#pragma once

//////////////////////////////////////////////////////////////////////////

class CInventoryPanelDlg : public CDialog
{
	DECLARE_DYNAMIC(CInventoryPanelDlg)

public:

	explicit CInventoryPanelDlg(CWnd *pParent = NULL);

	enum FILTER
	{
		FILTER_ALL,
		FILTER_USABLE,
		FILTER_NORMAL,
		FILTER_SPECIAL,
		FILTER_WEAPON,
		FILTER_SWORD,
		FILTER_SPEAR,
		FILTER_AXE,
		FILTER_HAMMER,
		FILTER_WHIP,
		FILTER_CLAW,
		FILTER_STAFF,
	};

	int GetItemNum(int item) const;
	void SetFilter(FILTER filter);

protected:

	int		m_itemNum[DQMJ_ITEMLIST_LEN];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()

private:

	int		m_nCtrlBorder;
	int		m_nCtrlHeight;
	int		m_nCtrlSpacing;
	FILTER	m_eFilter;
	int		m_itemToIndex[DQMJ_ITEMLIST_LEN];

	void RefreshFilter();

};

//////////////////////////////////////////////////////////////////////////
