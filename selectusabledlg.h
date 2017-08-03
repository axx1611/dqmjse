
#pragma once

//////////////////////////////////////////////////////////////////////////

class CSelectUsableDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectUsableDlg)

public:

	explicit CSelectUsableDlg(int item = 0x00, CWnd *pParent = NULL);

	int GetSelItem() const;

protected:

	int			m_nSelItem;
	CListBox	m_lstItemList;

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLbnSelchangeListItemList();
	afx_msg void OnLbnDblclkListItemList();

	DECLARE_MESSAGE_MAP()

private:

	void RefreshEnable();

};

//////////////////////////////////////////////////////////////////////////
