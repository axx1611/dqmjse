
#pragma once

#include "inventorypaneldlg.h"

//////////////////////////////////////////////////////////////////////////

class CItemListDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemListDlg)

public:

	explicit CItemListDlg(CWnd *pParent = NULL);

protected:

	CListBox			m_lstBaggage;
	CComboBox			m_cmbFilter;
	CInventoryPanelDlg	m_dlgPanel;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonBaggageUp();
	afx_msg void OnBnClickedButtonBaggageDown();
	afx_msg void OnBnClickedButtonBaggageTop();
	afx_msg void OnBnClickedButtonBaggageBottom();
	afx_msg void OnBnClickedButtonBaggageAdd();
	afx_msg void OnBnClickedButtonBaggageModify();
	afx_msg void OnBnClickedButtonBaggageRemove();
	afx_msg void OnBnClickedButtonBaggageSort();
	afx_msg void OnLbnSelchangeListBaggageList();
	afx_msg void OnLbnDblclkListBaggageList();
	afx_msg void OnCbnSelchangeComboInventoryFilter();

	DECLARE_MESSAGE_MAP()

private:

	void RefreshBaggageSel(int sel);
	int SelectUsable(int sel = LB_ERR);

};

//////////////////////////////////////////////////////////////////////////
