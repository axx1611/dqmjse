
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

CString CDqmjSEApp::GetNameString(const DQMJ_NAME &name) const
{
	CStringW str;

	UINT len = ::DQMJSaveNameToString(NULL, UINT_MAX, &name, L'?');
	VERIFY(::DQMJSaveNameToString(str.GetBuffer(len), len, &name, L'?') == len);
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

CString CDqmjSEApp::GetSkillString(int skill) const
{
	if (skill == 0x00)
		return _T("なし");

	return GetSafeString(m_skillMap, skill);
}

DQMJ_NAME CDqmjSEApp::GetStringName(LPCTSTR str) const
{
	DQMJ_NAME name;
	::DQMJSaveStringToName(&name, CStringW(str));

	return name;
}

BOOL CDqmjSEApp::OpenFile(LPCTSTR pszFilePath)
{
	if (m_hDqmjSave != NULL)
		return FALSE;

	CString path = GetFullPath(pszFilePath);
	if (path.IsEmpty())
		return FALSE;

	m_hDqmjSave = ::DQMJSaveOpenFile(path);
	if (m_hDqmjSave == NULL)
		return FALSE;

	m_filePath = path;
	return TRUE;
}

BOOL CDqmjSEApp::SaveFile()
{
	if ((m_hDqmjSave == NULL) || m_filePath.IsEmpty())
		return FALSE;

	return ::DQMJSaveSaveToFile(m_hDqmjSave, m_filePath, FALSE);
}

BOOL CDqmjSEApp::SaveAsFile(LPCTSTR pszFilePath, BOOL asRaw)
{
	CString path = GetFullPath(pszFilePath);
	if (path.IsEmpty())
		return FALSE;

	if (!::DQMJSaveSaveToFile(m_hDqmjSave, path, asRaw))
		return FALSE;

	m_filePath = path;
	return TRUE;
}

void CDqmjSEApp::CloseFile()
{
	::DQMJSaveCloseFile(m_hDqmjSave);
	m_hDqmjSave = NULL;
	m_filePath.Empty();
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
	InitStringList(m_strategyList, STRATEGY_LIST, DQMJ_STRATEGY_NUM);
	InitStringList(m_sourceList, SOURCE_LIST, DQMJ_SOURCE_NUM);
	InitStringList(m_growthList, GROWTH_LIST, DQMJ_GROWTH_NUM);
	InitStringList(m_guardList, GUARD_LIST, DQMJ_GUARD_NUM);

	for (int i = 0; i < DQMJ_MONSTERLIST_LEN; i++)
	{
		DQMJ_SAVE_RACE_SETTING rs;
		if (!::DQMJSaveGetRaceSetting(i, &rs))
			continue;

		m_raceMap.SetAt(i, rs.name);
		m_typeRaceList[rs.type].AddTail(i);
	}

	for (int i = 0; i < DQMJ_ITEMLIST_LEN; i++)
	{
		DQMJ_SAVE_ITEM_SETTING is;
		if (!::DQMJSaveGetItemSetting(i, &is))
			continue;

		m_itemMap.SetAt(i, is.name);

		if (is.weapon)
			m_weaponList.AddTail(i);
	}

	for (int i = 0; i < DQMJ_SKILLSETLIST_LEN; i++)
	{
		DQMJ_SAVE_SKILLSET_SETTING sss;
		if (!::DQMJSaveGetSkillsetSetting(i, &sss))
			continue;

		m_skillsetMap.SetAt(i, sss.name);
	}

	for (int i = 0; i < DQMJ_ABILITYLIST_LEN; i++)
	{
		LPCTSTR name = ::DQMJSaveGetAbilityName(i);
		if (name != NULL)
			m_abilityMap.SetAt(i, name);
	}

	for (int i = 0; i < DQMJ_SKILLLIST_LEN; i++)
	{
		LPCTSTR name = ::DQMJSaveGetSkillName(i);
		if (name != NULL)
			m_skillMap.SetAt(i, name);
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

CString CDqmjSEApp::GetFullPath(LPCTSTR path)
{
	DWORD size = ::GetFullPathName(path, 0, NULL, NULL);
	if (size == 0)
		return _T("");

	size++;

	CString sstr;
	if (::GetFullPathName(path, size, sstr.GetBuffer(size), NULL) == 0)
		return _T("");

	sstr.ReleaseBuffer();
	size = ::GetLongPathName(sstr, NULL, 0);

	if (size == 0)
		return sstr;

	size++;

	CString lstr;
	if (::GetLongPathName(sstr, lstr.GetBuffer(size), size) == 0)
		return sstr;

	lstr.ReleaseBuffer();
	return lstr;
}

//////////////////////////////////////////////////////////////////////////
