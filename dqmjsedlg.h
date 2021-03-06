
#pragma once

//////////////////////////////////////////////////////////////////////////

class CDqmjSEDlg : public CDialog
{
	DECLARE_DYNAMIC(CDqmjSEDlg)

public:

	explicit CDqmjSEDlg(CWnd *pParent = NULL);

protected:

	int			m_nGold;
	int			m_nDeposit;
	int			m_nVictoryTimes;
	int			m_nScoutTimes;
	int			m_nCombineTimes;
	int			m_nHour;
	int			m_nMinute;
	int			m_nSecond;
	HICON		m_hIcon;
	HACCEL		m_hAccel;
	CString		m_strPlayerName;

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditItemList();
	afx_msg void OnEditMonsterList();
	afx_msg void OnEditMonsterLibrary();
	afx_msg void OnEditSkillLibrary();
	afx_msg void OnBnClickedCheckSkillbookInitflag();

	template<int idx>
	afx_msg void OnBnClickedButtonPartyExchange() { ExchangeMember(TRUE, idx); }
	template<int idx>
	afx_msg void OnBnClickedButtonStandbyExchange() { ExchangeMember(FALSE, idx); }
	template<int idx>
	afx_msg void OnBnClickedButtonPartyRemove() { RemoveMember(TRUE, idx); }
	template<int idx>
	afx_msg void OnBnClickedButtonStandbyRemove() { RemoveMember(FALSE, idx); }
	template<int idx>
	afx_msg void OnBnClickedButtonPartyEdit() { EditMember(TRUE, idx); }
	template<int idx>
	afx_msg void OnBnClickedButtonStandbyEdit() { EditMember(FALSE, idx); }

	DECLARE_MESSAGE_MAP()

private:

	BOOL OpenFile(LPCTSTR pszFilename);
	void CloseFile();
	BOOL RefreshBriefing();
	BOOL RefreshPlayInfo();
	BOOL RefreshBookInfo();
	BOOL RefreshRanchInfo();
	void EnableAll(BOOL enable);
	void UpdateSaveData();
	void ExchangeMember(BOOL party_or_standby, int idx);
	void RemoveMember(BOOL party_or_standby, int idx);
	void EditMember(BOOL party_or_standby, int idx);

	static CString GetStoryString(int story);
	static CString GetPlaceString(int place);
	static CString GetTimeString(int hour, int minute, int second);
	static CString GetLevelString(int level);
	static CString GetRaceString(int race);
	static CString GetMonsterNumString(int monster_num);

};

//////////////////////////////////////////////////////////////////////////
