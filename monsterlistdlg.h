
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterListDlg)

public:

	CMonsterListDlg(CWnd *pParent = NULL);

protected:

	CComboBox		m_cmbSex;
	CComboBox		m_cmbRace;
	CComboBox		m_cmbRank;
	CComboBox		m_cmbType;
	CComboBox		m_cmbStrategy;
	CComboBox		m_cmbSource;
	CComboBox		m_cmbSkillset[3];
	CComboBox		m_cmbAbility[6];
	CComboBox		m_cmbGuard[27];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
