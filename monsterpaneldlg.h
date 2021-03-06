
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterPanelDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterPanelDlg)

public:

	explicit CMonsterPanelDlg(CWnd *pParent = NULL);

	enum FILTER
	{
		FILTER_ALL,
		FILTER_SLIME,
		FILTER_DRAGON,
		FILTER_NATURE,
		FILTER_BEAST,
		FILTER_MATERIAL,
		FILTER_DEMON,
		FILTER_ZOMBIE,
		FILTER_INCARNI,
	};

	BOOL IsEncounterChecked(int race) const;
	BOOL IsKillChecked(int race) const;
	BOOL IsObtainChecked(int race) const;
	int GetTypeByFilter(FILTER filter) const;
	BOOL SetFilter(FILTER filter);
	void OpenAll();

protected:

	int		m_enconterCheck[DQMJ_MONSTERLIST_LEN];
	int		m_killCheck[DQMJ_MONSTERLIST_LEN];
	int		m_obtainCheck[DQMJ_MONSTERLIST_LEN];
	int		m_allCheck[DQMJ_MONSTERLIST_LEN];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	template<int idx>
	afx_msg void OnBnClickedCheckAll() { CheckAll(m_indexToRace[idx]); }
	template<int idx>
	afx_msg void OnBnClickedAny() { CheckAny(m_indexToRace[idx]); }

	DECLARE_MESSAGE_MAP()

private:

	int		m_nCtrlBorder;
	int		m_nCtrlHeight;
	int		m_nCtrlSpacing;
	FILTER	m_eFilter;
	int		m_raceToIndex[DQMJ_MONSTERLIST_LEN];
	int		m_indexToRace[DQMJ_MONSTERLIST_LEN];

	void RefreshFilter();
	void CheckAny(int race);
	void CheckAll(int race);

};

//////////////////////////////////////////////////////////////////////////
