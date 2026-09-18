
#pragma once

#include <tchar.h>
#include <windows.h>

DECLARE_HANDLE(HDQMJSAVE);

/************************************************************************/

#define DQMJ_MONSTER_MAX				100
#define DQMJ_MEMBER_MAX					3
#define DQMJ_GOLD_MAX					9999999
#define DQMJ_ITEM_MAX					99
#define DQMJ_RECORD_MAX					99999
#define DQMJ_LEVEL_MAX					99
#define DQMJ_SKILLSET_MAX				3
#define DQMJ_SKILL_MAX					30
#define DQMJ_ABILITY_MAX				6
#define DQMJ_NAME_MAX					12
#define DQMJ_COMBINE_MAX				99
#define DQMJ_STATUS_MAX					999
#define DQMJ_SKILLPOINT_MAX				999
#define DQMJ_BAGGAGE_MAX				16
#define DQMJ_GUARD_MAX					27

#define DQMJ_MONSTERLIST_LEN			0x0159
#define DQMJ_ITEMLIST_LEN				0x009B
#define DQMJ_SKILLSETLIST_LEN			0x00C1
#define DQMJ_SKILLLIST_LEN				0x00C6
#define DQMJ_ABILITYLIST_LEN			0x001C

#define DQMJ_GROWTH_LEVEL_MIN			15
#define DQMJ_GROWTH_LEVEL_MAX			75
#define DQMJ_GROWTH_RANGE_MIN			5
#define DQMJ_GROWTH_RANGE_MAX			10

#define DQMJ_SKILL_COMPLETED			10
#define DQMJ_SKILL_LOCKED				(-1)

#define DQMJ_AUTO_RANK					(-1)
#define DQMJ_AUTO_TYPE					(-1)
#define DQMJ_AUTO_MAX_LEVEL				(-1)
#define DQMJ_AUTO_SKILL_ACTIVE_NUM		(-1)

#define DQMJ_RAND_SEX					(-1)
#define DQMJ_RAND_GROWTH_TYPE			(-1)
#define DQMJ_RAND_GROWTH_LEVEL			(-1)

#define DQMJ_NAME_DEFAULT_AS_BYTECODE	0

/************************************************************************/

/* 性別 */
enum
{
	DQMJ_SEX_MALE,															/* ♂ */
	DQMJ_SEX_FEMALE,														/* ♀ */
	DQMJ_SEX_BISEXUAL,														/* 両性具有 */
	DQMJ_SEX_NUM,
};

/* モンスターランク */
enum
{
	DQMJ_RANK_NONE,															/* なし */
	DQMJ_RANK_F,															/* ランクF */
	DQMJ_RANK_E,															/* ランクE */
	DQMJ_RANK_D,															/* ランクD */
	DQMJ_RANK_C,															/* ランクC */
	DQMJ_RANK_B,															/* ランクB */
	DQMJ_RANK_A,															/* ランクA */
	DQMJ_RANK_S,															/* ランクS */
	DQMJ_RANK_SS,															/* ランクSS */
	DQMJ_RANK_UNKNOWN,														/* ランク?? */
	DQMJ_RANK_NUM,
};

/* モンスター系統 */
enum
{
	DQMJ_TYPE_NONE,															/* なし */
	DQMJ_TYPE_SLIME,														/* スライム系 */
	DQMJ_TYPE_DRAGON,														/* ドラゴン系 */
	DQMJ_TYPE_NATURE,														/* しぜん系 */
	DQMJ_TYPE_BEAST,														/* まじゅう系 */
	DQMJ_TYPE_MATERIAL,														/* ぶっしつ系 */
	DQMJ_TYPE_DEMON,														/* あくま系 */
	DQMJ_TYPE_ZOMBIE,														/* ゾンビ系 */
	DQMJ_TYPE_INCARNI,														/* しんじゅう系 */
	DQMJ_TYPE_NUM,
};

/* 道具タイプ */
enum
{
	DQMJ_ITEM_NORMAL,														/* 一般道具 */
	DQMJ_ITEM_USABLE,														/* 使用可能 */
	DQMJ_ITEM_SPECIAL,														/* 特殊道具 */
	DQMJ_ITEM_SWORD,														/* 剣 */
	DQMJ_ITEM_SPEAR,														/* やり */
	DQMJ_ITEM_AXE,															/* オノ */
	DQMJ_ITEM_HAMMER,														/* ハンマー */
	DQMJ_ITEM_WHIP,															/* ムチ */
	DQMJ_ITEM_CLAW,															/* ツメ */
	DQMJ_ITEM_STAFF,														/* つえ */
	DQMJ_ITEM_NUM,
};

/* 主人公特技番号 */
enum
{
	DQMJ_PLAYERSKILL_ZOOM,													/* ルーラ */
	DQMJ_PLAYERSKILL_EVAC,													/* リレミト */
	DQMJ_PLAYERSKILL_WHISTLE,												/* くちぶえ */
	DQMJ_PLAYERSKILL_STEALTH,												/* ステルス */
	DQMJ_PLAYERSKILL_NUM,
};

/* スキルブック・ストア用スキルブック番号 */
enum
{
	DQMJ_SKILLBOOK_WARRIOR,													/* せんしブック */
	DQMJ_SKILLBOOK_MAGE,													/* まほうつかいブック */
	DQMJ_SKILLBOOK_CLERIC,													/* そうりょブック */
	DQMJ_SKILLBOOK_FIGHTER,													/* ぶどうかブック */
	DQMJ_SKILLBOOK_SAGE,													/* けんじゃブック */
	DQMJ_SKILLBOOK_THIEF,													/* とうぞくブック */
	DQMJ_SKILLBOOK_NUM,
};

/* ガードレベル */
enum
{
	DQMJ_GUARD_WEAK,														/* 弱い */
	DQMJ_GUARD_NORMAL,														/* ふつう */
	DQMJ_GUARD_STRONG,														/* 軽減 */
	DQMJ_GUARD_HALF,														/* 半減 */
	DQMJ_GUARD_VERY_STRONG,													/* 激減 */
	DQMJ_GUARD_INVALID,														/* 無効 */
	DQMJ_GUARD_COUNTER,														/* 反射 */
	DQMJ_GUARD_ABSORB,														/* 吸収 */
	DQMJ_GUARD_NUM,
};

/* モンスター入手ソース */
enum
{
	DQMJ_SOURCE_SYSTEM,														/* システム */
	DQMJ_SOURCE_EXCHANGE,													/* ワイヤレス交換？ */
	DQMJ_SOURCE_MASTER_SCOUT,												/* 他国マスタースカウト */
	DQMJ_SOURCE_FEILD_SCOUT,												/* 野良モンスタースカウト */
	DQMJ_SOURCE_COMBINE,													/* 配合 */
	DQMJ_SOURCE_GUEST,														/* ゲスト */
	DQMJ_SOURCE_NUM,
};

/* モンスター作戦タイプ */
enum
{
	DQMJ_STRATEGY_GO_ALL_OUT,												/* ガンガンいこうぜ */
	DQMJ_STRATEGY_TRY_OUT,													/* いろいろやろうぜ */
	DQMJ_STRATEGY_TAKE_CARE_LIFE,											/* いのちだいじに */
	DQMJ_STRATEGY_NO_SKILL,													/* とくぎつかうな */
	DQMJ_STRATEGY_NUM,
};

/* 爆発成長タイプ */
enum
{
	DQMJ_GROWTH_HP,															/* HP */
	DQMJ_GROWTH_MP,															/* MP */
	DQMJ_GROWTH_ATK,														/* こうげき力 */
	DQMJ_GROWTH_DEF,														/* しゅび力 */
	DQMJ_GROWTH_AGI,														/* すばやさ */
	DQMJ_GROWTH_INT,														/* かしこさ */
	DQMJ_GROWTH_NUM,
};

/* 親の性別 */
enum
{
	DQMJ_PARENT_FATHER,														/* 父親 */
	DQMJ_PARENT_MOTHER,														/* 母親 */
	DQMJ_PARENT_NUM,
};

/* セーブデータファイル形式 */
enum
{
	DQMJ_FORMAT_INVALID,													/* 無効 */
	DQMJ_FORMAT_RAW,														/* 生バイナリ配列 */
	DQMJ_FORMAT_DESMUME,													/* DeSmuMEバッテリーデータ */
	DQMJ_FORMAT_NOCASHBGA,													/* No$GBAバッテリーデータ */
};

/* 名前情報 */
typedef struct
{
	BYTE				byte_code[DQMJ_NAME_MAX];
} DQMJ_NAME;

/* モンスター種別設定情報 */
typedef struct
{
	int					race;												/* モンスター種別 */
	int					rank;												/* ランク */
	int					type;												/* 系統 */
	LPCTSTR				name;												/* モンスター名 */
	int					limit_hp;											/* HP成長限界 */
	int					limit_mp;											/* MP成長限界 */
	int					limit_atk;											/* 攻撃力成長限界 */
	int					limit_def;											/* 守備力成長限界 */
	int					limit_agi;											/* 素早さ成長限界 */
	int					limit_int;											/* 賢さ成長限界 */
	int					guard[DQMJ_GUARD_MAX];								/* 既定ガード */
	int					ability[DQMJ_ABILITY_MAX];							/* 既定特性 */
	int					skillset[DQMJ_SKILLSET_MAX];						/* 既定スキル */
} DQMJ_SAVE_RACE_SETTING;

/* 道具設定情報 */
typedef struct
{
	int					item;												/* 道具ID */
	int					type;												/* 道具タイプ */
	BOOL				weapon;												/* 武器フラグ */
	LPCTSTR				name;												/* 道具名 */
} DQMJ_SAVE_ITEM_SETTING;

/* スキル設定情報 */
typedef struct
{
	int					skillset;											/* スキルID */
	LPCTSTR				name;												/* スキル名 */
	BOOL				param;												/* パラメータスキルフラグ */
	int					count;												/* とくぎ／パラメータの数 */
	int					max_point;											/* すべての特技開放に必要なスキルポイント */
} DQMJ_SAVE_SKILLSET_SETTING;

/* セーブ情報一括（ロード画面表示用） */
typedef struct
{
	BOOL				interrupt;											/* 中断フラグ */
	DQMJ_NAME			player_name;										/* 主人公の名前 */
	int					place_id;											/* 場所ID */
	int					play_time_hour;										/* プレイ時間（時） */
	int					play_time_min;										/* プレイ時間（分） */
	int					play_time_sec;										/* プレイ時間（秒） */
	int					party_member_num;									/* パーティモンスター数 */
	DQMJ_NAME			party_member_name[DQMJ_MEMBER_MAX];					/* パーティモンスターの名前 */
	int					party_member_race[DQMJ_MEMBER_MAX];					/* パーティモンスターの種別 */
	int					party_member_level[DQMJ_MEMBER_MAX];				/* パーティモンスターのレベル */
	int					story_progress;										/* マデュライト入手数 */
} DQMJ_SAVE_BRIEFING;

/* プレイ情報 */
typedef struct
{
	DQMJ_NAME			player_name;										/* 主人公の名前 */
	int					gold;												/* 所持金 */
	int					deposit;											/* 預金額 */
	int					play_time_hour;										/* プレイ時間（時） */
	int					play_time_min;										/* プレイ時間（分） */
	int					play_time_sec;										/* プレイ時間（秒） */
	int					victory_times;										/* 戦闘勝利数 */
	int					scout_times;										/* スカウト成功数 */
	int					combine_times;										/* 配合回数 */
	BOOL				player_skill[DQMJ_PLAYERSKILL_NUM];					/* 主人公特技開放状態 */
} DQMJ_SAVE_PLAY_INFO;

/* 道具関連情報 */
typedef struct
{
	int					baggage[DQMJ_BAGGAGE_MAX];							/* 手荷物情報 */
	int					inventory[DQMJ_ITEMLIST_LEN];						/* 袋の道具情報 */
	BOOL				bookstore_inited;									/* スキルブック・ストア初期化完了フラグ */
	BOOL				bookstore_onsale[DQMJ_SKILLBOOK_NUM];				/* スキルブック・ストア発売中ブック */
} DQMJ_SAVE_ITEM_INFO;

/* ライブラリ関連情報 */
typedef struct
{
	BOOL				encounter[DQMJ_MONSTERLIST_LEN];					/* 遭遇ライブラリ情報 */
	BOOL				kill[DQMJ_MONSTERLIST_LEN];							/* 討伐ライブラリ情報 */
	BOOL				obtain[DQMJ_MONSTERLIST_LEN];						/* 入手ライブラリ情報 */
	int					skill[DQMJ_SKILLSETLIST_LEN];						/* スキルライブラリ情報 */
} DQMJ_SAVE_LIBRARY_INFO;

/* 牧場関連情報 */
typedef struct
{
	int					monster_num;										/* 所持モンスター数 */
	int					party_member_num;									/* パーティモンスター数 */
	int					party_member_idx[DQMJ_MEMBER_MAX];					/* パーティモンスターの番号 */
	int					standby_num;										/* スタンバイモンスター数 */
	int					standby_idx[DQMJ_MEMBER_MAX];						/* スタンバイモンスターの番号 */
} DQMJ_SAVE_RANCH_INFO;

/* モンスター保存情報 */
typedef struct
{
	BOOL				cheat;												/* 不正フラグ（restrict時は無視される） */
	BOOL				dead;												/* 死亡フラグ */
	BOOL				poison;												/* 毒フラグ */
	DQMJ_NAME			name;												/* 名前 */
	DQMJ_NAME			master;												/* 作成マスターの名前 */
	int					race;												/* 種別 */
	int					sex;												/* 性別 */
	int					rank;												/* ランク（restrict時は無視される） */
	int					type;												/* 系統（restrict時は無視される） */
	int					combine;											/* 配合回数 */
	int					source;												/* 入手ルート */
	int					level;												/* レベル */
	int					max_level;											/* レベル成長限界 */
	int					base_hp;											/* 基礎最大HP */
	int					base_mp;											/* 基礎最大MP */
	int					base_atk;											/* 基礎攻撃力 */
	int					base_def;											/* 基礎守備力 */
	int					base_agi;											/* 基礎素早さ */
	int					base_int;											/* 基礎賢さ */
	int					current_hp;											/* 現在残りHP */
	int					current_mp;											/* 現在残りMP */
	int					actual_hp;											/* 実際最大HP（restrict時は無視される） */
	int					actual_mp;											/* 実際最大MP（restrict時は無視される） */
	int					actual_atk;											/* 実際攻撃力（restrict時は無視される） */
	int					actual_def;											/* 実際守備力（restrict時は無視される） */
	int					actual_agi;											/* 実際素早さ（restrict時は無視される） */
	int					actual_int;											/* 実際賢さ（restrict時は無視される） */
	int					exp;												/* 経験値 */
	int					strategy;											/* 作戦 */
	int					growth_type;										/* 爆発成長タイプ */
	int					growth_start_level;									/* 爆発成長開始レベル */
	int					growth_end_level;									/* 爆発成長終了レベル */
	int					weapon;												/* 装備している武器 */
	int					skillset[DQMJ_SKILLSET_MAX];						/* モンスタースキル */
	int					assigned_sp[DQMJ_SKILLSET_MAX];						/* 各スキルに振り分けたポイント */
	int					skill_active_num[DQMJ_SKILLSET_MAX];				/* 各スキルの開放特技数（restrict時は無視される） */
	int					remaining_sp;										/* 残りスキルポイント */
	int					guard_info[DQMJ_GUARD_MAX];							/* ガード情報（restrict時は無視される） */
	int					skill_list[DQMJ_SKILL_MAX];							/* 習得特技リスト（restrict時は無視される） */
	int					ability_list[DQMJ_ABILITY_MAX];						/* 特性リスト（restrict時は無視される） */
	int					parent_race[DQMJ_PARENT_NUM];						/* 両親の種別 */
	DQMJ_NAME			parent_name[DQMJ_PARENT_NUM];						/* 両親の名前 */
	DQMJ_NAME			parent_master[DQMJ_PARENT_NUM];						/* 両親の作成マスターの名前 */
	int					grandparent_race[DQMJ_PARENT_NUM][DQMJ_PARENT_NUM];	/* 祖父母・外祖父母の種別 */
} DQMJ_SAVE_MONSTER_INFO;

/************************************************************************/

/* 名前データをUnicode文字列に変換する */
EXTERN_C UINT DQMJSaveNameToString(LPWSTR str, UINT len, CONST DQMJ_NAME *name, WCHAR def);

/* Unicode文字列を名前データに変換する */
EXTERN_C UINT DQMJSaveStringToName(DQMJ_NAME *name, LPCWSTR str);

/* モンスター種別設定情報を取得 */
EXTERN_C BOOL DQMJSaveGetRaceSetting(int race, DQMJ_SAVE_RACE_SETTING *out);

/* 道具IDから道具設定情報を取得 */
EXTERN_C BOOL DQMJSaveGetItemSetting(int item, DQMJ_SAVE_ITEM_SETTING *out);

/* スキルIDからスキル情報を取得 */
EXTERN_C BOOL DQMJSaveGetSkillsetSetting(int skillset, DQMJ_SAVE_SKILLSET_SETTING *out);

/* 場所IDから場所名を取得 */
EXTERN_C LPCTSTR DQMJSaveGetPlaceName(int place);

/* ガードIDからガード名を取得 */
EXTERN_C LPCTSTR DQMJSaveGetGuardName(int guard);

/* 特技IDから特技名を取得 */
EXTERN_C LPCTSTR DQMJSaveGetSkillName(int skill);

/* 特性IDから特性名を取得 */
EXTERN_C LPCTSTR DQMJSaveGetAbilityName(int ability);

/* 指定したアイテムが装備可能かどうかを判断する */
EXTERN_C BOOL DQMJSaveCheckEquippable(int race, int equipment, CONST int *ability_list);

/* モンスタースキルに振り分けたスキルポイントから該当スキルの開放特技数を取得 */
EXTERN_C int DQMJSaveCalcSkillActiveCount(int skillset, int skill_point);

/* モンスター情報をすべて合法範囲内に正規化する */
EXTERN_C BOOL DQMJSaveNormalizeMonster(BOOL interrupt, BOOL in_party_or_standby, CONST DQMJ_NAME *player_name, DQMJ_SAVE_MONSTER_INFO *inout);

/* セーブファイルを開く */
EXTERN_C HDQMJSAVE DQMJSaveOpenFile(LPCTSTR file_path);

/* セーブファイルを閉じる */
EXTERN_C BOOL DQMJSaveCloseFile(HDQMJSAVE handle);

/* 名前を付けてセーブファイルを保存する */
EXTERN_C BOOL DQMJSaveSaveToFile(HDQMJSAVE handle, LPCTSTR file_path, BOOL as_raw);

/* セーブファイルの形式を取得する */
EXTERN_C int DQMJSaveQueryFileFormat(HDQMJSAVE handle);

/* セーブファイルからロード画面表示用の一括情報を取得する */
EXTERN_C BOOL DQMJSaveQueryBriefing(HDQMJSAVE handle, DQMJ_SAVE_BRIEFING *out);

/* セーブファイルからプレイ情報を取得 */
EXTERN_C BOOL DQMJSaveQueryPlayInfo(HDQMJSAVE handle, DQMJ_SAVE_PLAY_INFO *out);

/* セーブファイルから道具情報を取得 */
EXTERN_C BOOL DQMJSaveQueryItemInfo(HDQMJSAVE handle, DQMJ_SAVE_ITEM_INFO *out);

/* セーブファイルからライブラリ情報を取得 */
EXTERN_C BOOL DQMJSaveQueryLibraryInfo(HDQMJSAVE handle, DQMJ_SAVE_LIBRARY_INFO *out);

/* セーブファイルから牧場情報を取得 */
EXTERN_C BOOL DQMJSaveQueryRanchInfo(HDQMJSAVE handle, DQMJ_SAVE_RANCH_INFO *out);

/* セーブファイルから指定したモンスター情報を取得する */
EXTERN_C BOOL DQMJSaveQueryMonsterInfo(HDQMJSAVE handle, int monster_idx, DQMJ_SAVE_MONSTER_INFO *out);

/* セーブファイルのプレイ情報を変更 */
EXTERN_C BOOL DQMJSaveModifyPlayInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_PLAY_INFO *in);

/* セーブファイルの道具情報を変更 */
EXTERN_C BOOL DQMJSaveModifyItemInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_ITEM_INFO *in);

/* セーブファイルのライブラリ情報を変更 */
EXTERN_C BOOL DQMJSaveModifyLibraryInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_LIBRARY_INFO *in);

/* セーブファイルの牧場情報を変更 */
EXTERN_C BOOL DQMJSaveModifyRanchInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_RANCH_INFO *in);

/* セーブファイルの指定したモンスター情報を変更する */
EXTERN_C BOOL DQMJSaveModifyMonster(HDQMJSAVE handle, int monster_idx, CONST DQMJ_SAVE_MONSTER_INFO *in, BOOL normalize);

/* セーブファイルに新しいモンスターを作成して先頭に追加 */
EXTERN_C BOOL DQMJSaveNewMonster(HDQMJSAVE handle, CONST DQMJ_SAVE_MONSTER_INFO *in, BOOL normalize);

/* セーブファイル内の指定したモンスターの位置を移動する */
EXTERN_C BOOL DQMJSaveMoveMonster(HDQMJSAVE handle, int monster_idx, int moveto_idx);

/* セーブファイル内の指定したモンスターをコピーして先頭に追加する */
EXTERN_C BOOL DQMJSaveCopyMonster(HDQMJSAVE handle, int monster_idx);

/* セーブファイル内の指定したモンスターを削除する */
EXTERN_C BOOL DQMJSaveRemoveMonster(HDQMJSAVE handle, int monster_idx);

/************************************************************************/
