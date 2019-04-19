
#include "stdafx.h"
#include "dqmjse.h"
#include "dqmjsedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

const LPCTSTR SEX_LIST[]		= { _T("♂"), _T("♀"), _T("♂♀") };
const LPCTSTR RANK_LIST[]		= { _T("なし"), _T("F"), _T("E"), _T("D"), _T("C"), _T("B"), _T("A"), _T("S"), _T("SS"), _T("??") };
const LPCTSTR TYPE_LIST[]		= { _T("なし"), _T("スライム系"), _T("ドラゴン系"), _T("しぜん系"), _T("まじゅう系"), _T("ぶっしつ系"), _T("あくま系"), _T("ゾンビ系"), _T("しんじゅう系") };
const LPCTSTR STRATEGY_LIST[]	= { _T("ガンガンいこうぜ"), _T("いろいろやろうぜ"), _T("いのちだいじに"), _T("とくぎつかうな") };
const LPCTSTR SOURCE_LIST[]		= { _T("システム"), _T("ワイヤレス交換(?)"), _T("他国マスタースカウト"), _T("野良モンスタースカウト"), _T("配合"), _T("ゲスト") };
const LPCTSTR GROWTH_LIST[]		= { _T("ＨＰ"), _T("ＭＰ"), _T("攻撃力"), _T("守備力"), _T("素早さ"), _T("賢さ") };
const LPCTSTR GUARD_LIST[]		= { _T("弱い"), _T("ふつう"), _T("軽減"), _T("半減"), _T("激減"), _T("無効"), _T("反射"), _T("吸収") };

CDqmjSEApp theApp;

//////////////////////////////////////////////////////////////////////////

CDqmjSEApp::CDqmjSEApp()
{
	m_hDqmjSave = NULL;
}

//////////////////////////////////////////////////////////////////////////

int CDqmjSEApp::CalcDigit(int number) const
{
	int n = 0;

	while (number != 0)
	{
		n++;
		number /= 10;
	}

	return (n == 0) ? 1 : n;
}

CString CDqmjSEApp::GetDecimalString(int num) const
{
	CString str;
	str.Format(_T("%d"), num);

	return str;
}

CString CDqmjSEApp::GetDecimalString(int num1, int num2) const
{
	CString str;
	str.Format(_T("%d/%d"), num1, num2);

	return str;
}

CString CDqmjSEApp::GetNameString(DQMJ_NAME name) const
{
	CStringW str;

	UINT len = ::DQMJSaveNameToString(NULL, UINT_MAX, name, L'?');
	VERIFY(::DQMJSaveNameToString(str.GetBuffer(len), len, name, L'?') == len);
	str.ReleaseBuffer();

	return CString(str);
}

CString CDqmjSEApp::GetSexString(int sex) const
{
	return GetSafeString(m_sexList, sex);
}

CString CDqmjSEApp::GetRaceString(int race) const
{
	return GetSafeString(m_raceMap, race);
}

CString CDqmjSEApp::GetRankString(int rank) const
{
	return GetSafeString(m_rankList, rank);
}

CString CDqmjSEApp::GetTypeString(int type) const
{
	return GetSafeString(m_typeList, type);
}

CString CDqmjSEApp::GetItemString(int item) const
{
	if (item == 0x00)
		return _T("なし");

	return GetSafeString(m_itemMap, item);
}

DQMJ_NAME &CDqmjSEApp::GetStringName(DQMJ_NAME &name, LPCTSTR str) const
{
	::DQMJSaveStringToName(name, CStringW(str));
	return name;
}

BOOL CDqmjSEApp::OpenSave(LPCTSTR pszFilename)
{
	if (m_hDqmjSave != NULL)
		return FALSE;

	m_hDqmjSave = ::DQMJSaveOpenFile(pszFilename);
	if (m_hDqmjSave == NULL)
		return FALSE;

	return TRUE;
}

void CDqmjSEApp::CloseSave()
{
	::DQMJSaveCloseFile(m_hDqmjSave);
	m_hDqmjSave = NULL;
}

BOOL CDqmjSEApp::InitInstance()
{
	INITCOMMONCONTROLSEX initctrls;
	initctrls.dwSize = sizeof(initctrls);
	initctrls.dwICC = ICC_WIN95_CLASSES;
	::InitCommonControlsEx(&initctrls);

	CWinAppEx::InitInstance();

	InitStringList(m_sexList, SEX_LIST, DQMJ_SEX_NUM);
	InitStringList(m_rankList, RANK_LIST, DQMJ_RANK_NUM);
	InitStringList(m_typeList, TYPE_LIST, DQMJ_TYPE_NUM);
	InitStringList(m_skillsetList, ::DQMJSaveGetSkillsetName, DQMJ_SKILLSETLIST_LEN);
	InitStringList(m_abilityList, ::DQMJSaveGetAbilityName, DQMJ_ABILITYLIST_LEN);
	InitStringList(m_strategyList, STRATEGY_LIST, DQMJ_STRATEGY_NUM);
	InitStringList(m_sourceList, SOURCE_LIST, DQMJ_SOURCE_NUM);
	InitStringList(m_growthList, GROWTH_LIST, DQMJ_GROWTH_NUM);
	InitStringList(m_guardList, GUARD_LIST, DQMJ_GUARD_NUM);

	for (int i = 0; i < DQMJ_MONSTERLIST_LEN; i++)
	{
		DQMJ_SAVE_RACE_SETTING rs;
		if (::DQMJSaveGetRaceSetting(i, &rs))
			m_raceMap.SetAt(i, rs.name);
	}

	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
	{
		DQMJ_SAVE_ITEM_SETTING is;
		if (::DQMJSaveGetItemSetting(i, &is))
			m_itemMap.SetAt(i, is.name);
	}

	CDqmjSEDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

void CDqmjSEApp::InitStringList(CStringArray &arr, const LPCTSTR *list, int len)
{
	arr.SetSize(len);

	for (int i = 0; i < len; i++)
		arr.SetAt(i, list[i]);
}

void CDqmjSEApp::InitStringList(CStringArray &arr, LPCTSTR (*func)(int), int len)
{
	arr.SetSize(len);

	for (int i = 0; i < len; i++)
		arr.SetAt(i, func(i));
}

CString CDqmjSEApp::GetSafeString(const CStringArray &arr, int idx)
{
	if ((idx >= 0) && (idx < arr.GetCount()))
		return arr.GetAt(idx);

	return _T("不明");
}

CString CDqmjSEApp::GetSafeString(const CIntToStrMap &map, int id)
{
	CString str;
	if (map.Lookup(id, str))
		return str;

	return _T("不明");
}

//////////////////////////////////////////////////////////////////////////
