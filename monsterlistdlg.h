
#pragma once

//////////////////////////////////////////////////////////////////////////

class CMonsterListDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterListDlg)

public:

	CMonsterListDlg(CWnd *pParent = NULL);

protected:

	CListBox		m_lstMonster;
	CListBox		m_lstSkill;
	CComboBox		m_cmbSex;
	CComboBox		m_cmbRace;
	CComboBox		m_cmbRank;
	CComboBox		m_cmbType;
	CComboBox		m_cmbStrategy;
	CComboBox		m_cmbSource;
	CComboBox		m_cmbGrowthType;
	CComboBox		m_cmbSkillset[3];
	CComboBox		m_cmbAbility[6];
	CComboBox		m_cmbGuard[27];
	CComboBox		m_cmbParentsRace[2][3];

	virtual void DoDataExchange(CDataExchange *pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:

	void EnableAll(BOOL enable);

	static void InitRaceComboBox(CComboBox &combo);

};

//////////////////////////////////////////////////////////////////////////
