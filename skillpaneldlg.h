
#pragma once

//////////////////////////////////////////////////////////////////////////

class CSkillPanelDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkillPanelDlg)

public:

	explicit CSkillPanelDlg(CWnd *pParent = NULL);

	enum FILTER
	{
		FILTER_ALL,
		FILTER_SKILL,
		FILTER_PARAMETER,
	};

	int GetSkillsetUnlockNum(int skillset) const;
	BOOL SetFilter(FILTER filter);
	void OpenAll();

protected:

	int		m_unlockCheck[DQMJ_SKILLSETLIST_LEN];
	int		m_unlockCount[DQMJ_SKILLSETLIST_LEN];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	template<int idx>
	afx_msg void OnBnClickedCheckUnlockSkill() { CheckUnlock(m_indexToSkillset[idx]); }
	template<int idx>
	afx_msg void OnBnClickedButtonAllSkill() { OpenAllSkill(m_indexToSkillset[idx]); }

	DECLARE_MESSAGE_MAP()

private:

	int		m_nCtrlBorder;
	int		m_nCtrlHeight;
	int		m_nCtrlSpacing;
	FILTER	m_eFilter;
	int		m_skillCount[DQMJ_SKILLSETLIST_LEN];
	int		m_skillsetToIndex[DQMJ_SKILLSETLIST_LEN];
	int		m_indexToSkillset[DQMJ_SKILLSETLIST_LEN];

	void RefreshFilter();
	void UpdateSkillset(int skillset);
	void CheckUnlock(int skillset);
	void OpenAllSkill(int skillset);

};

//////////////////////////////////////////////////////////////////////////
