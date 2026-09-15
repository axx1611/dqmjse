
#include "stdafx.h"
#include "dqmjse.h"
#include "dqmjsedlg.h"
#include "itemlistdlg.h"
#include "monsterlistdlg.h"
#include "monsterinfodlg.h"
#include "monsterlibrarydlg.h"
#include "skilllibrarydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

const int IDC_BRIEFING_NAME[]			= { IDC_BRIEFING_NAME1, IDC_BRIEFING_NAME2, IDC_BRIEFING_NAME3 };
const int IDC_BRIEFING_LEVEL[]			= { IDC_BRIEFING_LEVEL1, IDC_BRIEFING_LEVEL2, IDC_BRIEFING_LEVEL3 };
const int IDC_BRIEFING_RACE[]			= { IDC_BRIEFING_RACE1, IDC_BRIEFING_RACE2, IDC_BRIEFING_RACE3 };

const int IDC_PARTY_NAME[]				= { IDC_PARTY_NAME1, IDC_PARTY_NAME2, IDC_PARTY_NAME3 };
const int IDC_PARTY_LEVEL[]				= { IDC_PARTY_LEVEL1, IDC_PARTY_LEVEL2, IDC_PARTY_LEVEL3 };
const int IDC_PARTY_RACE[]				= { IDC_PARTY_RACE1, IDC_PARTY_RACE2, IDC_PARTY_RACE3 };
const int IDC_BUTTON_PARTY_EXCHANGE[]	= { IDC_BUTTON_PARTY_EXCHANGE1, IDC_BUTTON_PARTY_EXCHANGE2, IDC_BUTTON_PARTY_EXCHANGE3 };
const int IDC_BUTTON_PARTY_REMOVE[]		= { IDC_BUTTON_PARTY_REMOVE1, IDC_BUTTON_PARTY_REMOVE2, IDC_BUTTON_PARTY_REMOVE3 };
const int IDC_BUTTON_PARTY_EDIT[]		= { IDC_BUTTON_PARTY_EDIT1, IDC_BUTTON_PARTY_EDIT2, IDC_BUTTON_PARTY_EDIT3 };

const int IDC_STANDBY_NAME[]			= { IDC_STANDBY_NAME1, IDC_STANDBY_NAME2, IDC_STANDBY_NAME3 };
const int IDC_STANDBY_LEVEL[]			= { IDC_STANDBY_LEVEL1, IDC_STANDBY_LEVEL2, IDC_STANDBY_LEVEL3 };
const int IDC_STANDBY_RACE[]			= { IDC_STANDBY_RACE1, IDC_STANDBY_RACE2, IDC_STANDBY_RACE3 };
const int IDC_BUTTON_STANDBY_EXCHANGE[]	= { IDC_BUTTON_STANDBY_EXCHANGE1, IDC_BUTTON_STANDBY_EXCHANGE2, IDC_BUTTON_STANDBY_EXCHANGE3 };
const int IDC_BUTTON_STANDBY_REMOVE[]	= { IDC_BUTTON_STANDBY_REMOVE1, IDC_BUTTON_STANDBY_REMOVE2, IDC_BUTTON_STANDBY_REMOVE3 };
const int IDC_BUTTON_STANDBY_EDIT[]		= { IDC_BUTTON_STANDBY_EDIT1, IDC_BUTTON_STANDBY_EDIT2, IDC_BUTTON_STANDBY_EDIT3 };

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDqmjSEDlg, CDialog)

BEGIN_MESSAGE_MAP(CDqmjSEDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_EDIT_ITEM_LIST, OnEditItemList)
	ON_COMMAND(ID_EDIT_MONSTER_LIST, OnEditMonsterList)
	ON_COMMAND(ID_EDIT_MONSTER_LIBRARY, OnEditMonsterLibrary)
	ON_COMMAND(ID_EDIT_SKILL_LIBRARY, OnEditSkillLibrary)
	ON_BN_CLICKED(IDC_CHECK_SKILLBOOK_INITFLAG, OnBnClickedCheckSkillbookInitflag)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_EXCHANGE1, OnBnClickedButtonPartyExchange<0>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_EXCHANGE2, OnBnClickedButtonPartyExchange<1>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_EXCHANGE3, OnBnClickedButtonPartyExchange<2>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_EXCHANGE1, OnBnClickedButtonStandbyExchange<0>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_EXCHANGE2, OnBnClickedButtonStandbyExchange<1>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_EXCHANGE3, OnBnClickedButtonStandbyExchange<2>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_REMOVE1, OnBnClickedButtonPartyRemove<0>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_REMOVE2, OnBnClickedButtonPartyRemove<1>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_REMOVE3, OnBnClickedButtonPartyRemove<2>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_REMOVE1, OnBnClickedButtonStandbyRemove<0>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_REMOVE2, OnBnClickedButtonStandbyRemove<1>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_REMOVE3, OnBnClickedButtonStandbyRemove<2>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_EDIT1, OnBnClickedButtonPartyEdit<0>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_EDIT2, OnBnClickedButtonPartyEdit<1>)
	ON_BN_CLICKED(IDC_BUTTON_PARTY_EDIT3, OnBnClickedButtonPartyEdit<2>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_EDIT1, OnBnClickedButtonStandbyEdit<0>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_EDIT2, OnBnClickedButtonStandbyEdit<1>)
	ON_BN_CLICKED(IDC_BUTTON_STANDBY_EDIT3, OnBnClickedButtonStandbyEdit<2>)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CDqmjSEDlg::CDqmjSEDlg(CWnd *pParent) : CDialog(IDD_DQMJSE_DIALOG, pParent)
{
	m_nGold = 0;
	m_nDeposit = 0;
	m_nVictoryTimes = 0;
	m_nScoutTimes = 0;
	m_nCombineTimes = 0;
	m_nHour = 0;
	m_nMinute = 0;
	m_nSecond = 0;
	m_strPlayerName = _T("");

	m_hIcon = ::AfxGetApp()->LoadIcon(IDR_MAIN_MENU);
	m_hAccel = ::LoadAccelerators(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAIN_MENU));
}

//////////////////////////////////////////////////////////////////////////

void CDqmjSEDlg::DoDataExchange(CDataExchange *pDX)
{
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_NAME, m_strPlayerName);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_GOLD, m_nGold);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_DEPOSIT, m_nDeposit);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_VICTORYTIMES, m_nVictoryTimes);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_SCOUTTIMES, m_nScoutTimes);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_COMBINETIMES, m_nCombineTimes);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_HOUR, m_nHour);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_MINUTE, m_nMinute);
	::DDX_Text(pDX, IDC_EDIT_PLAYINFO_SECOND, m_nSecond);

	::DDV_MinMaxInt(pDX, m_nGold, 0, DQMJ_GOLD_MAX);
	::DDV_MinMaxInt(pDX, m_nDeposit, 0, DQMJ_GOLD_MAX);
	::DDV_MinMaxInt(pDX, m_nVictoryTimes, 0, DQMJ_RECORD_MAX);
	::DDV_MinMaxInt(pDX, m_nScoutTimes, 0, DQMJ_RECORD_MAX);
	::DDV_MinMaxInt(pDX, m_nCombineTimes, 0, DQMJ_RECORD_MAX);
	::DDV_MinMaxInt(pDX, m_nHour, 0, 99999);
	::DDV_MinMaxInt(pDX, m_nMinute, 0, 59);
	::DDV_MinMaxInt(pDX, m_nSecond, 0, 59);

	CDialog::DoDataExchange(pDX);
}

BOOL CDqmjSEDlg::PreTranslateMessage(MSG *pMsg)
{
	if ((m_hAccel != NULL) && ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDqmjSEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_NAME))->SetLimitText(DQMJ_NAME_MAX);
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_GOLD))->SetLimitText(theApp.CalcDigit(DQMJ_GOLD_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_DEPOSIT))->SetLimitText(theApp.CalcDigit(DQMJ_GOLD_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_VICTORYTIMES))->SetLimitText(theApp.CalcDigit(DQMJ_RECORD_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_SCOUTTIMES))->SetLimitText(theApp.CalcDigit(DQMJ_RECORD_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_COMBINETIMES))->SetLimitText(theApp.CalcDigit(DQMJ_RECORD_MAX));
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_HOUR))->SetLimitText(5);
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_MINUTE))->SetLimitText(2);
	((CEdit *)GetDlgItem(IDC_EDIT_PLAYINFO_SECOND))->SetLimitText(2);

	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_GOLD))->SetRange32(0, DQMJ_GOLD_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_DEPOSIT))->SetRange32(0, DQMJ_GOLD_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_VICTORYTIMES))->SetRange32(0, DQMJ_RECORD_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_SCOUTTIMES))->SetRange32(0, DQMJ_RECORD_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_COMBINETIMES))->SetRange32(0, DQMJ_RECORD_MAX);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_HOUR))->SetRange32(0, 99999);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_MINUTE))->SetRange32(0, 59);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_PLAYINFO_SECOND))->SetRange32(0, 59);

	EnableAll(FALSE);

	return TRUE;
}

void CDqmjSEDlg::OnOK()
{
	UpdateData(TRUE);
}

void CDqmjSEDlg::OnCancel()
{
	if ((theApp.GetSaveHandle() != NULL) && (MessageBox(_T("未保存の変更データは全て失いますが、それでも終了しますか？"), _T("注意"), MB_OKCANCEL | MB_ICONWARNING) != IDOK))
		return;

	CDialog::OnCancel();
}

void CDqmjSEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = ::GetSystemMetrics(SM_CXICON);
		int cyIcon = ::GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDqmjSEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDqmjSEDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR path[MAX_PATH] = _T("");
	::DragQueryFile(hDropInfo, 0, path, MAX_PATH);
	::DragFinish(hDropInfo);

	CloseFile();

	if (!OpenFile(path))
		CloseFile();

	CDialog::OnDropFiles(hDropInfo);
}

void CDqmjSEDlg::OnFileOpen()
{
	CFileDialog cfd(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T(""), this);
	if (cfd.DoModal() != IDOK)
		return;

	CloseFile();

	if (!OpenFile(cfd.GetPathName()))
		CloseFile();
}

void CDqmjSEDlg::OnFileClose()
{
	CloseFile();
}

void CDqmjSEDlg::OnFileSave()
{
	if (MessageBox(_T("データ破損の恐れがありますので、予めバックアップすることをお勧めします。\n\n保存処理を進みますか？"), _T("注意"), MB_YESNO | MB_ICONWARNING) != IDYES)
		return;

	if (theApp.SaveFile())
		MessageBox(_T("保存完了しました。"), _T("通知"), MB_OK | MB_ICONINFORMATION);
	else
		MessageBox(_T("保存失敗しました！"), _T("エラー"), MB_OK | MB_ICONERROR);
}

void CDqmjSEDlg::OnFileSaveAs()
{
	CString filter;
	switch (::DQMJSaveQueryFileFormat(theApp.GetSaveHandle()))
	{
	case DQMJ_FORMAT_RAW:
		filter = _T("生バイナリ配列 (*.sav)|*.sav||");
		break;
	case DQMJ_FORMAT_DESMUME:
		filter = _T("DeSmuMEバッテリーデータ (*.dsv)|*.dsv|生バイナリ配列 (*.sav)|*.sav||");
		break;
	case DQMJ_FORMAT_NOCASHBGA:
		filter = _T("No$GBAバッテリーデータ (*.sav)|*.sav|生バイナリ配列 (*.sav)|*.sav||");
		break;
	default:
		return;
	}

	CString name = theApp.GetFilePath();
	int i = name.ReverseFind(_T('\\'));
	if (i >= 0)
		name = name.Mid(i + 1);

	CFileDialog dlg(FALSE, NULL, name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this);
	if (dlg.DoModal() != IDOK)
		return;

	if (dlg.GetPathName().CompareNoCase(theApp.GetFilePath()) == 0)
	{
		if (MessageBox(_T("データ破損の恐れがありますので、予めバックアップすることをお勧めします。\n\n保存処理を進みますか？"), _T("注意"), MB_YESNO | MB_ICONWARNING) != IDYES)
			return;
	}

	if (theApp.SaveAsFile(dlg.GetPathName(), dlg.GetOFN().nFilterIndex != 1))
		MessageBox(_T("保存完了しました。"), _T("通知"), MB_OK | MB_ICONINFORMATION);
	else
		MessageBox(_T("保存失敗しました！"), _T("エラー"), MB_OK | MB_ICONERROR);
}

void CDqmjSEDlg::OnEditItemList()
{
	UpdateSaveData();

	CItemListDlg dlg(this);
	dlg.DoModal();
}

void CDqmjSEDlg::OnEditMonsterList()
{
	UpdateSaveData();

	CMonsterListDlg dlg(this);
	dlg.DoModal();

	VERIFY(RefreshBriefing());
	VERIFY(RefreshRanchInfo());
}

void CDqmjSEDlg::OnEditMonsterLibrary()
{
	UpdateSaveData();

	CMonsterLibraryDlg dlg(this);
	dlg.DoModal();
}

void CDqmjSEDlg::OnEditSkillLibrary()
{
	UpdateSaveData();

	CSkillLibraryDlg dlg(this);
	dlg.DoModal();
}

void CDqmjSEDlg::OnBnClickedCheckSkillbookInitflag()
{
	BOOL enable = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_INITFLAG) == BST_CHECKED;

	GetDlgItem(IDC_CHECK_SKILLBOOK_WARRIOR)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLBOOK_MAGE)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLBOOK_CLERIC)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLBOOK_FIGHTER)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLBOOK_SAGE)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLBOOK_THIEF)->EnableWindow(enable);

	if (!enable)
	{
		CheckDlgButton(IDC_CHECK_SKILLBOOK_WARRIOR, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_MAGE, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_CLERIC, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_FIGHTER, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_SAGE, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_THIEF, BST_UNCHECKED);
	}
}

//////////////////////////////////////////////////////////////////////////

BOOL CDqmjSEDlg::OpenFile(LPCTSTR pszFilename)
{
	if (!theApp.OpenFile(pszFilename))
		return FALSE;

	if (!RefreshBriefing())
		return FALSE;

	if (!RefreshPlayInfo())
		return FALSE;

	if (!RefreshBookInfo())
		return FALSE;

	if (!RefreshRanchInfo())
		return FALSE;

	EnableAll(TRUE);
	return TRUE;
}

void CDqmjSEDlg::CloseFile()
{
	theApp.CloseFile();
	EnableAll(FALSE);
}

BOOL CDqmjSEDlg::RefreshBriefing()
{
	DQMJ_SAVE_BRIEFING briefing;
	if (!::DQMJSaveQueryBriefing(theApp.GetSaveHandle(), &briefing))
		return FALSE;

	SetDlgItemText(IDC_BRIEFING_INTERRUPT, briefing.interrupt ? _T("はい") : _T("いいえ"));
	SetDlgItemText(IDC_BRIEFING_PLACE, GetPlaceString(briefing.place_id));
	SetDlgItemText(IDC_BRIEFING_PLAYERNAME, theApp.GetNameString(briefing.player_name));
	SetDlgItemText(IDC_BRIEFING_STORYPROGRESS, GetStoryString(briefing.story_progress));
	SetDlgItemText(IDC_BRIEFING_PLAYTIME, GetTimeString(briefing.play_time_hour, briefing.play_time_min, briefing.play_time_sec));

	for (int i = 0; i < DQMJ_MEMBER_MAX; i++)
	{
		if (i >= briefing.party_member_num)
		{
			SetDlgItemText(IDC_BRIEFING_NAME[i], _T("-----"));
			SetDlgItemText(IDC_BRIEFING_LEVEL[i], _T("--"));
			SetDlgItemText(IDC_BRIEFING_RACE[i], _T("--"));
		}
		else
		{
			SetDlgItemText(IDC_BRIEFING_NAME[i], theApp.GetNameString(briefing.party_member_name[i]));
			SetDlgItemText(IDC_BRIEFING_LEVEL[i], GetLevelString(briefing.party_member_level[i]));
			SetDlgItemText(IDC_BRIEFING_RACE[i], GetRaceString(briefing.party_member_race[i]));
		}
	}

	return TRUE;
}

BOOL CDqmjSEDlg::RefreshPlayInfo()
{
	DQMJ_SAVE_PLAY_INFO playinfo;
	if (!::DQMJSaveQueryPlayInfo(theApp.GetSaveHandle(), &playinfo))
		return FALSE;

	m_strPlayerName = theApp.GetNameString(playinfo.player_name);
	m_nGold = playinfo.gold;
	m_nDeposit = playinfo.deposit;
	m_nVictoryTimes = playinfo.victory_times;
	m_nScoutTimes = playinfo.scout_times;
	m_nCombineTimes = playinfo.combine_times;
	m_nHour = playinfo.play_time_hour;
	m_nMinute = playinfo.play_time_min;
	m_nSecond = playinfo.play_time_sec;

	UpdateData(FALSE);

	CheckDlgButton(IDC_CHECK_PLAYERSKILL_ZOOM, playinfo.player_skill[DQMJ_PLAYERSKILL_ZOOM] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_PLAYERSKILL_EVAC, playinfo.player_skill[DQMJ_PLAYERSKILL_EVAC] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_PLAYERSKILL_WHISTLE, playinfo.player_skill[DQMJ_PLAYERSKILL_WHISTLE] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_PLAYERSKILL_STEALTH, playinfo.player_skill[DQMJ_PLAYERSKILL_STEALTH] ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;
}

BOOL CDqmjSEDlg::RefreshBookInfo()
{
	DQMJ_SAVE_ITEM_INFO iteminfo;
	if (!::DQMJSaveQueryItemInfo(theApp.GetSaveHandle(), &iteminfo))
		return FALSE;

	GetDlgItem(IDC_CHECK_SKILLBOOK_WARRIOR)->EnableWindow(iteminfo.bookstore_inited);
	GetDlgItem(IDC_CHECK_SKILLBOOK_MAGE)->EnableWindow(iteminfo.bookstore_inited);
	GetDlgItem(IDC_CHECK_SKILLBOOK_CLERIC)->EnableWindow(iteminfo.bookstore_inited);
	GetDlgItem(IDC_CHECK_SKILLBOOK_FIGHTER)->EnableWindow(iteminfo.bookstore_inited);
	GetDlgItem(IDC_CHECK_SKILLBOOK_SAGE)->EnableWindow(iteminfo.bookstore_inited);
	GetDlgItem(IDC_CHECK_SKILLBOOK_THIEF)->EnableWindow(iteminfo.bookstore_inited);

	CheckDlgButton(IDC_CHECK_SKILLBOOK_INITFLAG, iteminfo.bookstore_inited ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_SKILLBOOK_WARRIOR, iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_WARRIOR] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_SKILLBOOK_MAGE, iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_MAGE] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_SKILLBOOK_CLERIC, iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_CLERIC] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_SKILLBOOK_FIGHTER, iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_FIGHTER] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_SKILLBOOK_SAGE, iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_SAGE] ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK_SKILLBOOK_THIEF, iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_THIEF] ? BST_CHECKED : BST_UNCHECKED);

	return TRUE;
}

BOOL CDqmjSEDlg::RefreshRanchInfo()
{
	DQMJ_SAVE_RANCH_INFO ranchinfo;
	if (!::DQMJSaveQueryRanchInfo(theApp.GetSaveHandle(), &ranchinfo))
		return FALSE;

	SetDlgItemText(IDC_PLAYINFO_MONSTER_NUM, GetMonsterNumString(ranchinfo.monster_num));

	for (int i = 0; i < DQMJ_MEMBER_MAX; i++)
	{
		if (i >= ranchinfo.party_member_num)
		{
			SetDlgItemText(IDC_PARTY_NAME[i], _T("-----"));
			SetDlgItemText(IDC_PARTY_LEVEL[i], _T("--"));
			SetDlgItemText(IDC_PARTY_RACE[i], _T("--"));
			GetDlgItem(IDC_BUTTON_PARTY_EXCHANGE[i])->EnableWindow(i == ranchinfo.party_member_num);
			GetDlgItem(IDC_BUTTON_PARTY_REMOVE[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PARTY_EDIT[i])->EnableWindow(FALSE);
		}
		else
		{
			DQMJ_SAVE_MONSTER_INFO monster;
			if (!::DQMJSaveQueryMonsterInfo(theApp.GetSaveHandle(), ranchinfo.party_member_idx[i], &monster))
			{
				SetDlgItemText(IDC_PARTY_NAME[i], _T("<broken>"));
				SetDlgItemText(IDC_PARTY_LEVEL[i], GetLevelString(0));
				SetDlgItemText(IDC_PARTY_RACE[i], GetRaceString(0x00));
				GetDlgItem(IDC_BUTTON_PARTY_EDIT[i])->EnableWindow(FALSE);
			}
			else
			{
				SetDlgItemText(IDC_PARTY_NAME[i], theApp.GetNameString(monster.name));
				SetDlgItemText(IDC_PARTY_LEVEL[i], GetLevelString(monster.level));
				SetDlgItemText(IDC_PARTY_RACE[i], GetRaceString(monster.race));
				GetDlgItem(IDC_BUTTON_PARTY_EDIT[i])->EnableWindow(TRUE);
			}

			GetDlgItem(IDC_BUTTON_PARTY_EXCHANGE[i])->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_PARTY_REMOVE[i])->EnableWindow(ranchinfo.party_member_num > 1);
		}

		if (i >= ranchinfo.standby_num)
		{
			SetDlgItemText(IDC_STANDBY_NAME[i], _T("-----"));
			SetDlgItemText(IDC_STANDBY_LEVEL[i], _T("--"));
			SetDlgItemText(IDC_STANDBY_RACE[i], _T("--"));
			GetDlgItem(IDC_BUTTON_STANDBY_EXCHANGE[i])->EnableWindow(i == ranchinfo.standby_num);
			GetDlgItem(IDC_BUTTON_STANDBY_REMOVE[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STANDBY_EDIT[i])->EnableWindow(FALSE);
		}
		else
		{
			DQMJ_SAVE_MONSTER_INFO monster;
			if (!::DQMJSaveQueryMonsterInfo(theApp.GetSaveHandle(), ranchinfo.standby_idx[i], &monster))
			{
				SetDlgItemText(IDC_STANDBY_NAME[i], _T("<broken>"));
				SetDlgItemText(IDC_STANDBY_LEVEL[i], GetLevelString(0));
				SetDlgItemText(IDC_STANDBY_RACE[i], GetRaceString(0x00));
				GetDlgItem(IDC_BUTTON_STANDBY_EDIT[i])->EnableWindow(FALSE);
			}
			else
			{
				SetDlgItemText(IDC_STANDBY_NAME[i], theApp.GetNameString(monster.name));
				SetDlgItemText(IDC_STANDBY_LEVEL[i], GetLevelString(monster.level));
				SetDlgItemText(IDC_STANDBY_RACE[i], GetRaceString(monster.race));
				GetDlgItem(IDC_BUTTON_STANDBY_EDIT[i])->EnableWindow(TRUE);
			}

			GetDlgItem(IDC_BUTTON_STANDBY_EXCHANGE[i])->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_STANDBY_REMOVE[i])->EnableWindow(TRUE);
		}
	}

	return TRUE;
}

void CDqmjSEDlg::EnableAll(BOOL enable)
{
	if (!enable)
	{
		m_strPlayerName = _T("");
		m_nGold = 0;
		m_nDeposit = 0;
		m_nVictoryTimes = 0;
		m_nScoutTimes = 0;
		m_nCombineTimes = 0;
		m_nHour = 0;
		m_nMinute = 0;
		m_nSecond = 0;

		UpdateData(FALSE);

		SetDlgItemText(IDC_BRIEFING_INTERRUPT, _T("--"));
		SetDlgItemText(IDC_BRIEFING_PLACE, _T("--"));
		SetDlgItemText(IDC_BRIEFING_PLAYERNAME, _T("-----"));
		SetDlgItemText(IDC_BRIEFING_STORYPROGRESS, GetStoryString(0));
		SetDlgItemText(IDC_BRIEFING_PLAYTIME, _T("--:--:--"));

		SetDlgItemText(IDC_PLAYINFO_MONSTER_NUM, GetMonsterNumString(0));

		CheckDlgButton(IDC_CHECK_PLAYERSKILL_ZOOM, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_PLAYERSKILL_EVAC, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_PLAYERSKILL_WHISTLE, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_PLAYERSKILL_STEALTH, BST_UNCHECKED);

		CheckDlgButton(IDC_CHECK_SKILLBOOK_INITFLAG, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_WARRIOR, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_MAGE, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_CLERIC, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_FIGHTER, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_SAGE, BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_SKILLBOOK_THIEF, BST_UNCHECKED);

		GetDlgItem(IDC_CHECK_SKILLBOOK_WARRIOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SKILLBOOK_MAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SKILLBOOK_CLERIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SKILLBOOK_FIGHTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SKILLBOOK_SAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SKILLBOOK_THIEF)->EnableWindow(FALSE);

		for (int i = 0; i < DQMJ_MEMBER_MAX; i++)
		{
			SetDlgItemText(IDC_BRIEFING_NAME[i], _T("-----"));
			SetDlgItemText(IDC_BRIEFING_LEVEL[i], _T("--"));
			SetDlgItemText(IDC_BRIEFING_RACE[i], _T("--"));

			SetDlgItemText(IDC_PARTY_NAME[i], _T("-----"));
			SetDlgItemText(IDC_PARTY_LEVEL[i], _T("--"));
			SetDlgItemText(IDC_PARTY_RACE[i], _T("--"));

			GetDlgItem(IDC_BUTTON_PARTY_EXCHANGE[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PARTY_REMOVE[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PARTY_EDIT[i])->EnableWindow(FALSE);

			SetDlgItemText(IDC_STANDBY_NAME[i], _T("-----"));
			SetDlgItemText(IDC_STANDBY_LEVEL[i], _T("--"));
			SetDlgItemText(IDC_STANDBY_RACE[i], _T("--"));

			GetDlgItem(IDC_BUTTON_STANDBY_EXCHANGE[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STANDBY_REMOVE[i])->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STANDBY_EDIT[i])->EnableWindow(FALSE);
		}
	}

	GetMenu()->EnableMenuItem(ID_FILE_SAVE, enable ? MF_ENABLED : MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_FILE_SAVE_AS, enable ? MF_ENABLED : MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_FILE_CLOSE, enable ? MF_ENABLED : MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_EDIT_ITEM_LIST, enable ? MF_ENABLED : MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_EDIT_MONSTER_LIST, enable ? MF_ENABLED : MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_EDIT_MONSTER_LIBRARY, enable ? MF_ENABLED : MF_GRAYED);
	GetMenu()->EnableMenuItem(ID_EDIT_SKILL_LIBRARY, enable ? MF_ENABLED : MF_GRAYED);

	GetDlgItem(IDC_GROUP_BRIEFING)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BRIEFING_INTERRUPT)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BRIEFING_PLAYERNAME)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BRIEFING_STORYPROGRESS)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BRIEFING_PLACE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_BRIEFING_PLAYTIME)->EnableWindow(enable);
	GetDlgItem(IDC_BRIEFING_INTERRUPT)->EnableWindow(enable);
	GetDlgItem(IDC_BRIEFING_PLAYERNAME)->EnableWindow(enable);
	GetDlgItem(IDC_BRIEFING_STORYPROGRESS)->EnableWindow(enable);
	GetDlgItem(IDC_BRIEFING_PLACE)->EnableWindow(enable);
	GetDlgItem(IDC_BRIEFING_PLAYTIME)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_PLAYINFO)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_GOLD)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_DEPOSIT)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_VICTORYTIMES)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_SCOUTTIMES)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_COMBINETIMES)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_TIME)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_HOUR)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_MINUTE)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_SECOND)->EnableWindow(enable);
	GetDlgItem(IDC_LABEL_PLAYINFO_MONSTER_NUM)->EnableWindow(enable);
	GetDlgItem(IDC_PLAYINFO_MONSTER_NUM)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_NAME)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_GOLD)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_DEPOSIT)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_VICTORYTIMES)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_SCOUTTIMES)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_COMBINETIMES)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_HOUR)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_MINUTE)->EnableWindow(enable);
	GetDlgItem(IDC_EDIT_PLAYINFO_SECOND)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_GOLD)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_DEPOSIT)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_VICTORYTIMES)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_SCOUTTIMES)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_COMBINETIMES)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_HOUR)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_MINUTE)->EnableWindow(enable);
	GetDlgItem(IDC_SPIN_PLAYINFO_SECOND)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_PLAYERSKILL)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_PLAYERSKILL_ZOOM)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_PLAYERSKILL_EVAC)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_PLAYERSKILL_WHISTLE)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_PLAYERSKILL_STEALTH)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_PARTY)->EnableWindow(enable);
	GetDlgItem(IDC_GROUP_STANDBY)->EnableWindow(enable);

	GetDlgItem(IDC_GROUP_SKILLBOOK)->EnableWindow(enable);
	GetDlgItem(IDC_CHECK_SKILLBOOK_INITFLAG)->EnableWindow(enable);

	for (int i = 0; i < DQMJ_MEMBER_MAX; i++)
	{
		GetDlgItem(IDC_BRIEFING_NAME[i])->EnableWindow(enable);
		GetDlgItem(IDC_BRIEFING_LEVEL[i])->EnableWindow(enable);
		GetDlgItem(IDC_BRIEFING_RACE[i])->EnableWindow(enable);

		GetDlgItem(IDC_PARTY_NAME[i])->EnableWindow(enable);
		GetDlgItem(IDC_PARTY_LEVEL[i])->EnableWindow(enable);
		GetDlgItem(IDC_PARTY_RACE[i])->EnableWindow(enable);

		GetDlgItem(IDC_STANDBY_NAME[i])->EnableWindow(enable);
		GetDlgItem(IDC_STANDBY_LEVEL[i])->EnableWindow(enable);
		GetDlgItem(IDC_STANDBY_RACE[i])->EnableWindow(enable);
	}
}

void CDqmjSEDlg::UpdateSaveData()
{
	UpdateData(TRUE);

	DQMJ_SAVE_PLAY_INFO playinfo;
	VERIFY(::DQMJSaveQueryPlayInfo(theApp.GetSaveHandle(), &playinfo));
	DQMJ_SAVE_ITEM_INFO iteminfo;
	VERIFY(::DQMJSaveQueryItemInfo(theApp.GetSaveHandle(), &iteminfo));

	playinfo.player_name = theApp.GetStringName(m_strPlayerName);
	playinfo.gold = m_nGold;
	playinfo.deposit = m_nDeposit;
	playinfo.victory_times = m_nVictoryTimes;
	playinfo.scout_times = m_nScoutTimes;
	playinfo.combine_times = m_nCombineTimes;
	playinfo.play_time_hour = m_nHour;
	playinfo.play_time_min = m_nMinute;
	playinfo.play_time_sec = m_nSecond;

	playinfo.player_skill[DQMJ_PLAYERSKILL_ZOOM] = IsDlgButtonChecked(IDC_CHECK_PLAYERSKILL_ZOOM) == BST_CHECKED;
	playinfo.player_skill[DQMJ_PLAYERSKILL_EVAC] = IsDlgButtonChecked(IDC_CHECK_PLAYERSKILL_EVAC) == BST_CHECKED;
	playinfo.player_skill[DQMJ_PLAYERSKILL_WHISTLE] = IsDlgButtonChecked(IDC_CHECK_PLAYERSKILL_WHISTLE) == BST_CHECKED;
	playinfo.player_skill[DQMJ_PLAYERSKILL_STEALTH] = IsDlgButtonChecked(IDC_CHECK_PLAYERSKILL_STEALTH) == BST_CHECKED;

	iteminfo.bookstore_inited = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_INITFLAG) == BST_CHECKED;

	iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_WARRIOR] = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_WARRIOR) == BST_CHECKED;
	iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_MAGE] = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_MAGE) == BST_CHECKED;
	iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_CLERIC] = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_CLERIC) == BST_CHECKED;
	iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_FIGHTER] = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_FIGHTER) == BST_CHECKED;
	iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_SAGE] = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_SAGE) == BST_CHECKED;
	iteminfo.bookstore_onsale[DQMJ_SKILLBOOK_THIEF] = IsDlgButtonChecked(IDC_CHECK_SKILLBOOK_THIEF) == BST_CHECKED;

	VERIFY(::DQMJSaveModifyItemInfo(theApp.GetSaveHandle(), &iteminfo));
	VERIFY(::DQMJSaveModifyPlayInfo(theApp.GetSaveHandle(), &playinfo));
}

void CDqmjSEDlg::ExchangeMember(BOOL party_or_standby, int idx)
{
	ASSERT((idx >= 0) && (idx < DQMJ_MEMBER_MAX));

	DQMJ_SAVE_RANCH_INFO ranchinfo;
	if (!::DQMJSaveQueryRanchInfo(theApp.GetSaveHandle(), &ranchinfo))
		return;

	int monster_idx = LB_ERR;
	if (party_or_standby)
	{
		if (idx < ranchinfo.party_member_num)
			monster_idx = ranchinfo.party_member_idx[idx];
	}
	else
	{
		if (idx < ranchinfo.standby_num)
			monster_idx = ranchinfo.standby_idx[idx];
	}

	BOOL disable_party_member = FALSE;
	BOOL disable_standby = FALSE;
	if (monster_idx < 0)
	{
		if (party_or_standby || (ranchinfo.party_member_num <= 1))
			disable_party_member = TRUE;
		if (!party_or_standby)
			disable_standby = TRUE;
	}

	CMonsterListDlg dlg(this);
	dlg.ConfigSelectMode(monster_idx, disable_party_member, disable_standby);
	if (dlg.DoModal() != IDOK)
		return;

	int exchange_idx = dlg.GetSelMonster();
	if ((exchange_idx <= LB_ERR) || (exchange_idx == monster_idx))
		return;

	if (party_or_standby)
		ranchinfo.party_member_idx[idx] = exchange_idx;
	else
		ranchinfo.standby_idx[idx] = exchange_idx;

	int pt_idx = dlg.GetPartyIndex();
	int st_idx = dlg.GetStandbyIndex();

	if (monster_idx >= 0)
	{
		if (pt_idx >= 0)
			ranchinfo.party_member_idx[pt_idx] = monster_idx;
		else if (st_idx >= 0)
			ranchinfo.standby_idx[st_idx] = monster_idx;
	}
	else
	{
		if (party_or_standby)
			ranchinfo.party_member_num++;
		else
			ranchinfo.standby_num++;

		if (pt_idx >= 0)
		{
			ranchinfo.party_member_num--;
			for (int i = pt_idx; i < ranchinfo.party_member_num; i++)
				ranchinfo.party_member_idx[i] = ranchinfo.party_member_idx[i + 1];
		}
		else if (st_idx >= 0)
		{
			ranchinfo.standby_num--;
			for (int i = st_idx; i < ranchinfo.standby_num; i++)
				ranchinfo.standby_idx[i] = ranchinfo.standby_idx[i + 1];
		}
	}

	VERIFY(::DQMJSaveModifyRanchInfo(theApp.GetSaveHandle(), &ranchinfo));
	VERIFY(RefreshRanchInfo());

	if (party_or_standby || (pt_idx >= 0))
		VERIFY(RefreshBriefing());
}

void CDqmjSEDlg::RemoveMember(BOOL party_or_standby, int idx)
{
	ASSERT((idx >= 0) && (idx < DQMJ_MEMBER_MAX));

	DQMJ_SAVE_RANCH_INFO ranchinfo;
	if (!::DQMJSaveQueryRanchInfo(theApp.GetSaveHandle(), &ranchinfo))
		return;

	if (party_or_standby)
	{
		if ((idx >= ranchinfo.party_member_num) || (ranchinfo.party_member_num <= 1))
			return;

		for (int i = idx; i < ranchinfo.party_member_num - 1; i++)
			ranchinfo.party_member_idx[i] = ranchinfo.party_member_idx[i + 1];

		ranchinfo.party_member_num--;
	}
	else
	{
		if (idx >= ranchinfo.standby_num)
			return;

		for (int i = idx; i < ranchinfo.standby_num - 1; i++)
			ranchinfo.standby_idx[i] = ranchinfo.standby_idx[i + 1];

		ranchinfo.standby_num--;
	}

	VERIFY(::DQMJSaveModifyRanchInfo(theApp.GetSaveHandle(), &ranchinfo));
	VERIFY(RefreshRanchInfo());

	if (party_or_standby)
		VERIFY(RefreshBriefing());
}

void CDqmjSEDlg::EditMember(BOOL party_or_standby, int idx)
{
	ASSERT((idx >= 0) && (idx < DQMJ_MEMBER_MAX));

	DQMJ_SAVE_RANCH_INFO ranchinfo;
	if (!::DQMJSaveQueryRanchInfo(theApp.GetSaveHandle(), &ranchinfo))
		return;

	int monster_idx = LB_ERR;
	if (party_or_standby)
	{
		if (idx >= ranchinfo.party_member_num)
			return;

		monster_idx = ranchinfo.party_member_idx[idx];
	}
	else
	{
		if (idx >= ranchinfo.standby_num)
			return;

		monster_idx = ranchinfo.standby_idx[idx];
	}

	DQMJ_SAVE_MONSTER_INFO monsterinfo;
	if (!::DQMJSaveQueryMonsterInfo(theApp.GetSaveHandle(), monster_idx, &monsterinfo))
		return;

	CMonsterInfoDlg dlg(&monsterinfo, TRUE, this);
	if (dlg.DoModal() != IDOK)
		return;

	VERIFY(::DQMJSaveModifyMonster(theApp.GetSaveHandle(), monster_idx, &dlg.GetMonsterInfo(), FALSE));
	VERIFY(RefreshRanchInfo());
}

//////////////////////////////////////////////////////////////////////////

CString CDqmjSEDlg::GetStoryString(int story)
{
	WCHAR str[] = L"☆☆☆☆☆☆☆☆☆☆";
	for (int i = 0; i < min(story, 10); i++)
		str[i] = L'★';

	return CString(str);
}

CString CDqmjSEDlg::GetPlaceString(int place)
{
	LPCTSTR str = ::DQMJSaveGetPlaceName(place);
	return (str == NULL) ? _T("不明") : str;
}

CString CDqmjSEDlg::GetTimeString(int hour, int minute, int second)
{
	CString str;
	str.Format(_T("%d:%02d:%02d"), hour, minute, second);

	return str;
}

CString CDqmjSEDlg::GetLevelString(int level)
{
	CString str;
	str.Format(_T("Lv.%d"), level);

	return str;
}

CString CDqmjSEDlg::GetRaceString(int race)
{
	CString str;
	str.Format(_T("[%s]"), theApp.GetRaceString(race));

	return str;
}

CString CDqmjSEDlg::GetMonsterNumString(int monster_num)
{
	CString str;
	str.Format(_T("%d 匹"), monster_num);

	return str;
}

//////////////////////////////////////////////////////////////////////////
