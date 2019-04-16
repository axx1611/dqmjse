
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include "dqmjsav.h"

//////////////////////////////////////////////////////////////////////////

typedef CList<int, int>						CIntList;
typedef CMap<int, int, int, int>			CMapIntToInt;
typedef CMap<int, int, CString, LPCTSTR>	CMapIntToStr;

//////////////////////////////////////////////////////////////////////////

class CDqmjSEApp : public CWinAppEx
{

public:

	CDqmjSEApp();

	HDQMJSAVE GetSaveHandle() const;
	const CStringArray &GetSexStringList() const;
	const CStringArray &GetRankStringList() const;
	const CStringArray &GetTypeStringList() const;
	const CStringArray &GetSkillsetStringList() const;
	const CStringArray &GetAbilityStringList() const;
	const CStringArray &GetStrategyStringList() const;
	const CStringArray &GetSourceStringList() const;
	const CStringArray &GetGrowthTypeStringList() const;
	const CStringArray &GetGuardTypeStringList() const;
	const CMapIntToStr &GetRaceStringMap() const;
	const CMapIntToStr &GetItemStringMap() const;

	int CalcDigit(int number) const;
	CString GetDecimalString(int num) const;
	CString GetDecimalString2(int num1, int num2) const;
	CString GetNameString(DQMJ_NAME name) const;
	CString GetSexString(int sex) const;
	CString GetRaceString(int race) const;
	CString GetRankString(int rank) const;
	CString GetTypeString(int type) const;
	CString GetItemString(int item) const;
	CString GetSkillsetString(int skillset) const;
	CString GetAbilityString(int ability) const;
	CString GetStrategyString(int strategy) const;
	CString GetSourceString(int source) const;
	CString GetGrowthTypeString(int growth) const;
	CString GetGuardTypeString(int guard) const;
	DQMJ_NAME &GetStringName(DQMJ_NAME &name, LPCTSTR str) const;

	BOOL OpenSave(LPCTSTR pszFilename);
	void CloseSave();

	virtual BOOL InitInstance();

private:

	HDQMJSAVE		m_hDqmjSave;
	CStringArray	m_sexList;
	CStringArray	m_rankList;
	CStringArray	m_typeList;
	CStringArray	m_skillsetList;
	CStringArray	m_abilityList;
	CStringArray	m_strategyList;
	CStringArray	m_sourceList;
	CStringArray	m_growthTypeList;
	CStringArray	m_guardTypeList;
	CMapIntToStr	m_raceMap;
	CMapIntToStr	m_itemMap;

	static void InitStringList(CStringArray &arr, const LPCTSTR *list, int len);
	static void InitStringList(CStringArray &arr, LPCTSTR (*func)(int), int len);
	static CString GetSafeString(const CStringArray &arr, int idx);
	static CString GetSafeString(const CMapIntToStr &map, int id);

};

//////////////////////////////////////////////////////////////////////////

extern CDqmjSEApp theApp;

//////////////////////////////////////////////////////////////////////////
