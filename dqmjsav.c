
#include <malloc.h>
#include "dqmjsav.h"
#include "dqmjsav.inl"
#include "compress.h"

/************************************************************************/

#define DESMUME_IDENTIFIER			"|-DESMUME SAVE-|"

#define NOCASHBGA_IDENTIFIER		"NocashGbaBackupMediaSavDataFile\x1A"
#define NOCASHBGA_UNCOMPRESSED		0								/* No$GBA圧縮方法用：未圧縮 */
#define NOCASHBGA_COMPRESSED_RLU	1								/* No$GBA圧縮方法用：Fast/RLU */
#define NOCASHBGA_COMPRESSED_LZ		2								/* No$GBA圧縮方法用：Good/LZ */

#define SAVE_SIZE_MIN				0xA760							/* セーブデータの最小サイズ */
#define INTERRUPT_OFFSET			0xA700							/* 中断データのオフセット */
#define SAVE_IDENTIFIER				0x004D5144						/* 'DQM\x00' */
#define STATUS_DEAD					0x80							/* 死亡フラグ */
#define STATUS_POISON				0x01							/* 毒フラグ */

#define MINMAX(v, a, b)				(max(min((v), (b)), (a)))
#define BETWEEN(v, a, b)			(((v) >= (a)) && ((v) < (b)))
#define IS_INTR(handle)				(((struct save_info *)(handle))->interrupt_flag)
#define GET_FMT(handle)				((struct save_fmt *)(IS_INTR(handle) ? (((struct save_info *)(handle))->raw_data + INTERRUPT_OFFSET) : ((struct save_info *)(handle))->raw_data))
#define GET_HEADER(handle)			(&GET_FMT(handle)->header)
#define GET_BODY(handle)			(&GET_FMT(handle)->body)
#define EMPTY_NAME(name)			do { ZeroMemory(&(name), sizeof(name)); *((LPBYTE)&(name)) = 0xFF; } while (FALSE)
#define COPY_NAME(dest, src)		do { ZeroMemory(&(dest), sizeof(dest)); CopyMemory(&(dest), &(src), min(sizeof(dest), sizeof(src))); } while (FALSE)
#define GET_BIT(buff, i)			(((buff)[(i) >> 3] >> ((i) & 7)) & 1)
#define SET_BIT(buff, i)			((VOID)((buff)[(i) >> 3] |= 1 << ((i) & 7)))
#define CLR_BIT(buff, i)			((VOID)((buff)[(i) >> 3] &= ~(1 << ((i) & 7))))
#define GET_HALF(buff, i)			(((i) & 1) ? ((buff)[(i) >> 1] >> 4) : ((buff)[(i) >> 1] & 0x0F))
#define SET_HALF(buff, i, v)		((VOID)((buff)[(i) >> 1] = ((i) & 1) ? (((buff)[(i) >> 1] & 0x0F) | ((v) << 4)) : (((buff)[(i) >> 1] & 0xF0) | ((v) & 0x0F))))

/************************************************************************/

#pragma pack(1)

/* モンスターデータフォーマット */
struct monster_fmt
{
	UINT8						name[11];						/* 0000: 名前 */
	UINT8						unused1;						/* 000B: 未使用、通常は0 */
	UINT16						race;							/* 000C: 種別 */
	UINT8						rank;							/* 000E: ランク */
	UINT8						type;							/* 000F: 系統 */
	UINT32						sex;							/* 0010: 性別 */
	UINT8						combine;						/* 0014: 配合回数 */
	UINT8						rom_data1[4];					/* 0015: 不明、ROMデータその１ */
	UINT8						rom_data2[24];					/* 0019: 不明、ROMデータその２ */
	UINT8						unused2;						/* 0031: 未使用、通常は0 */
	UINT8						base_guard[31];					/* 0032: 既定ガード */
	UINT8						actual_guard[31];				/* 0051: 実際ガード */
	UINT8						source;							/* 0070: 入手ルート */
	UINT8						status;							/* 0071: 異常状態マスク */
	UINT8						level;							/* 0072: レベル */
	UINT8						max_level;						/* 0073: レベル成長限界 */
	UINT16						base_hp;						/* 0074: 基礎最大HP */
	UINT16						base_mp;						/* 0076: 基礎最大MP */
	UINT16						base_atk;						/* 0078: 基礎攻撃力 */
	UINT16						base_def;						/* 007A: 基礎守備力 */
	UINT16						base_agi;						/* 007C: 基礎素早さ */
	UINT16						base_int;						/* 007E: 基礎賢さ */
	UINT16						current_hp;						/* 0080: 現在残りHP */
	UINT16						actual_hp;						/* 0082: 実際最大HP */
	UINT16						current_mp;						/* 0084: 現在残りMP */
	UINT16						actual_mp;						/* 0086: 実際最大MP */
	UINT16						actual_atk;						/* 0088: 実際攻撃力 */
	UINT16						actual_def;						/* 008A: 実際守備力 */
	UINT16						actual_agi;						/* 008C: 実際素早さ */
	UINT16						actual_int;						/* 008E: 実際賢さ */
	UINT32						exp;							/* 0090: 経験値 */
	UINT32						strategy;						/* 0094: 作戦 */
	UINT8						growth_type;					/* 0098: 爆発成長タイプ */
	UINT8						growth_level[2];				/* 0099: 爆発成長レベル範囲 */
	UINT32						ability_mask;					/* 009B: 特性マスク */
	UINT8						weapon;							/* 009F: 装備している武器 */
	UINT8						skillset[3];					/* 00A0: スキル */
	UINT8						assigned_sp[3];					/* 00A3: 各スキルに振り分けたポイント */
	UINT8						skill_active_num[3];			/* 00A6: 各スキルの開放特技数 */
	UINT8						unused3;						/* 00A9: 未使用、通常は0 */
	UINT16						remaining_sp;					/* 00AA: 残りスキルポイント */
	UINT8						skill_list[30];					/* 00AC: 習得特技リスト */
	UINT8						ability_list[40];				/* 00CA: 特性リスト */
	UINT8						cheat;							/* 00F2: 不正フラグ */
	UINT8						level_bak;						/* 00F3: レベル */
	UINT32						checksum;						/* 00F4: チェックサム */
	UINT8						master[11];						/* 00F8: マスターの名前 */
	UINT8						unused4;						/* 0103: 未使用、通常は0 */
	UINT16						father_race;					/* 0104: 父親の種別 */
	UINT8						father_name[11];				/* 0106: 父親の名前 */
	UINT8						father_master[11];				/* 0111: 父親のマスターの名前 */
	UINT16						mother_race;					/* 011C: 母親の種別 */
	UINT8						mother_name[11];				/* 011E: 母親の名前 */
	UINT8						mother_master[11];				/* 0129: 母親のマスターの名前 */
	UINT16						father_father_race;				/* 0134: 祖父の種別 */
	UINT8						father_father_name[11];			/* 0136: 祖父の名前の為に保留、未使用 */
	UINT8						father_father_master[11];		/* 0141: 祖父のマスターの名前の為に保留、未使用 */
	UINT16						father_mother_race;				/* 014C: 祖母の種別 */
	UINT8						father_mother_name[11];			/* 014E: 祖母の名前の為に保留、未使用 */
	UINT8						father_mother_master[11];		/* 0159: 祖母のマスターの名前の為に保留、未使用 */
	UINT16						mother_father_race;				/* 0164: 外祖父の種別 */
	UINT8						mother_father_name[11];			/* 0166: 外祖父の名前の為に保留、未使用 */
	UINT8						mother_father_master[11];		/* 0171: 外祖父のマスターの名前の為に保留、未使用 */
	UINT16						mother_mother_race;				/* 017C: 外祖母の種別 */
	UINT8						mother_mother_name[11];			/* 017E: 外祖母の名前の為に保留、未使用 */
	UINT8						mother_mother_master[11];		/* 0189: 外祖母のマスターの名前の為に保留、未使用 */
};

/* セーブデータフォーマットヘッダ */
struct save_fmt_header
{
	UINT32						identifier;						/* 0000: 常時'DQM\x00' */
	UINT8						version[8];						/* 0004: バージョン情報？ */
	UINT32						checksum;						/* 000C: チェックサム */
	UINT8						unknown1[28];
	UINT32						play_time;						/* 002C: プレイ時間（ロード画面表示用） */
	UINT8						party_member_num;				/* 0030: パーティモンスター数（ロード画面表示用） */
	UINT8						place_id;						/* 0031: 場所（ロード画面表示用） */
	UINT8						player_name[12];				/* 0032: 主人公の名前（ロード画面表示用） */
	UINT8						party_member_name[3][12];		/* 003E: パーティモンスターの名前（ロード画面表示用） */
	UINT16						party_member_race[3];			/* 0062: パーティモンスターの種別（ロード画面表示用） */
	UINT8						party_member_level[3];			/* 0068: パーティモンスターのレベル（ロード画面表示用） */
	UINT8						story_progress;					/* 006B: ストーリーの進み具合 */
	UINT8						unknown2[4];
};

/* セーブデータフォーマットボディ */
struct save_fmt_body
{
	UINT8						unknown1[264];					/* 0070: チェックサム計算開始位置 */
	UINT8						player_name[12];				/* 0178: 主人公の名前 */
	UINT32						gold;							/* 0184: 所持金 */
	UINT32						deposit;						/* 0188: 預金額 */
	UINT8						baggage[16];					/* 018C: 手荷物 */
	UINT8						inventory[155];					/* 019C: 袋内道具 */
	UINT8						unknown3[105];
	UINT32						play_time;						/* 02A0: プレイ時間 */
	UINT8						party_member_num;				/* 02A4: パーティモンスター数 */
	UINT8						monster_num;					/* 02A5: 所持モンスター数 */
	UINT8						party_member_idx[3];			/* 02A6: パーティモンスターの番号 */
	UINT8						encounter_library[45];			/* 02A9: 遭遇ライブラリ */
	UINT8						kill_library[45];				/* 02D6: 討伐ライブラリ */
	UINT8						obtain_library[45];				/* 0303: 入手ライブラリ */
	UINT8						skill_library[121];				/* 0330: スキルライブラリ */
	UINT8						unknown4[10];
	UINT8						player_skill;					/* 03B3: 主人公特技 */
	UINT8						unknown5[20];
	UINT32						scout_times;					/* 03C8: スカウト成功数 */
	UINT8						unknown6[8];
	UINT32						victory_times;					/* 03D4: 戦闘勝利数 */
	UINT32						combine_times;					/* 03D8: 配合回数 */
	UINT8						unknown7[32];
	UINT32						bookstore_onsale[6];			/* 03FC: スキルブックストア販売中ブック */
	UINT8						unknown8[500];
	UINT32						bookstore_inited;				/* 0608: スキルブックストア初期化完了フラグ */
	UINT16						standby_num;					/* 060C: スタンバイモンスター数 */
	UINT8						standby_idx[3];					/* 060E: パーティモンスターの番号 */
	UINT8						unknown9[75];
	struct monster_fmt			monster_list[100];				/* 065C: 所持モンスター情報 */
	struct monster_fmt			saint_beast;					/* A42C: 神獣取得時のモンスター情報 */
	UINT8						unknown10[132];
};

/* セーブデータフォーマット */
struct save_fmt
{
	struct save_fmt_header		header;							/* 0000: ヘッダ */
	struct save_fmt_body		body;							/* 0070: ボディ（チェックサム計算する部分） */
	UINT8						tail[188];						/* A644: テール */
};

/* No$GBAバッテリーファイルヘッダ */
struct nocashgba_header
{
	UINT8						identifier[32];					/* 0000: 常時'NocashGbaBackupMediaSavDataFile\x1A' */
	UINT32						version;						/* 0020: No$GBAバージョン */
	UINT8						day;							/* 0024: 保存時刻情報（日） */
	UINT8						month;							/* 0025: 保存時刻情報（月） */
	UINT16						year;							/* 0026: 保存時刻情報（年） */
	UINT8						hour;							/* 0028: 保存時刻情報（時間） */
	UINT8						minute;							/* 0029: 保存時刻情報（分） */
	UINT16						unknown1;
	UINT32						unknown2;
	UINT8						reserved[16];					/* 0030: 常時ゼロ */
	UINT32						sram;							/* 0040: 常時'SRAM' */
	UINT32						compression;					/* 0044: 圧縮方法 */
	UINT32						data_size;						/* 0048: 格納データサイズ（圧縮の場合は圧縮後のみ） */
};

/* DeSmuMEバッテリーファイルフッタ */
struct desmume_footer
{
	UINT32						actually_written_size;			/* 0000: 実際データサイズ */
	UINT32						padded_size;					/* 0004: パディング後のデータサイズ */
	UINT32						save_type;						/* 0008: 未使用 */
	UINT32						address_size;					/* 000C: アドレスバスサイズ */
	UINT32						save_size;						/* 0010: 未使用 */
	UINT32						version_number;					/* 0014: 常時0 */
	UINT8						identifier[16];					/* 0018: 常時'|-DESMUME SAVE-|' */
};

/* セーブファイルフォーマット用拡張情報 */
union extend_info
{
	struct nocashgba_info
	{
		struct nocashgba_header	header;							/* No$GBAバッテリーファイルヘッダ */
		LPBYTE					footer;							/* No$GBAバッテリーファイルフッタデータバッファ */
		UINT					footer_size;					/* No$GBAバッテリーファイルフッタデータサイズ */
	} nocashgba_info;
	struct desmume_info
	{
		struct desmume_footer	footer;							/* DeSmuMEバッテリーファイルフッタ */
		LPBYTE					dummy;							/* DeSmuMEダミーデータバッファ */
		UINT					dummy_size;						/* DeSmuMEダミーデータサイズ */
	} desmume_info;
};

#pragma pack()

/* 処理用セーブ情報 */
struct save_info
{
	int							format;							/* セーブファイルのフォーマット */
	LPBYTE						raw_data;						/* 生のセーブデータ */
	UINT						raw_size;						/* 生データのサイズ */
	BOOL						interrupt_flag;					/* 中断データの有り無し */
	union extend_info			extend_data;					/* フォーマット拡張情報 */
};

/************************************************************************/

/* 「ふめい」マスターの名前 */
CONST static DQMJ_NAME UNKNOWN_MASTER = { { 0x45, 0x4B, 0x26, 0xFF, 0x5B, 0x8E, 0x7F, 0xFF, 0x45, 0x4B, 0x26 } };

/************************************************************************/

/* 指定の固有情報を探す */
#define lookup_place_info(place)		((CONST struct name_info *)lookup_info_base((place), sizeof(struct name_info), PLACE_LIST, _countof(PLACE_LIST), -1))
#define lookup_guard_info(guard)		((CONST struct name_info *)lookup_info_base((guard), sizeof(struct name_info), GUARD_LIST, _countof(GUARD_LIST), -1))
#define lookup_ability_info(ability)	((CONST struct ability_info *)lookup_info_base((ability), sizeof(struct ability_info), ABILITY_LIST, _countof(ABILITY_LIST), 0x00))
#define lookup_skillset_info(skillset)	((CONST struct skillset_info *)lookup_info_base((skillset), sizeof(struct skillset_info), SKILLSET_LIST, _countof(SKILLSET_LIST), 0x00))
#define lookup_skill_info(skill)		((CONST struct skill_info *)lookup_info_base((skill), sizeof(struct skill_info), SKILL_LIST, _countof(SKILL_LIST), 0x00))
#define lookup_item_info(item)			((CONST struct item_info *)lookup_info_base((item), sizeof(struct item_info), ITEM_LIST, _countof(ITEM_LIST), 0x00))
#define lookup_race_info(race)			((CONST struct race_info *)lookup_info_base((race), sizeof(struct race_info), RACE_LIST, _countof(RACE_LIST), 0x00))

/* モンスター性別をランダムで作成する */
#define gen_monster_sex(type)			(((rand() % 17) == 0) ? DQMJ_SEX_BISEXUAL : (rand() % 2))

/* 爆発成長タイプをランダムで作成する */
#define gen_growth_type()				(rand() % DQMJ_GROWTH_NUM)

/* 爆発成長開始レベルをランダムで作成する */
#define gen_growth_start_level()		(DQMJ_GROWTH_LEVEL_MIN + rand() % (DQMJ_GROWTH_LEVEL_MAX - DQMJ_GROWTH_LEVEL_MIN + 1))

/* 爆発成長終了レベルをランダムで作成する */
#define gen_growth_end_level(start)		((start) + (DQMJ_GROWTH_RANGE_MIN - 1) + (rand() % (DQMJ_GROWTH_RANGE_MAX - DQMJ_GROWTH_RANGE_MIN + 1)))

/* セーブデータのチェックサムを算出 */
static UINT32 calc_save_checksum(struct save_fmt *sav);

/* モンスターデータのチェックサムを算出 */
static UINT32 calc_monster_checksum(struct monster_fmt *monster);

/* セーブ用タイムスタンプを時分秒に変換 */
static VOID timestamp_to_hms(UINT32 timestamp, LPINT hour, LPINT minute, LPINT second);

/* 時分秒をセーブ用タイムスタンプに変換 */
static UINT32 hms_to_timestamp(int hour, int minute, int second, UINT32 timestamp);

/* 指定の固有情報を探す */
static LPCVOID lookup_info_base(int id, size_t size, LPCVOID head, int num, int def);

/* 指定のガードを1段階上げる */
static VOID upgrade_guard(LPINT guard_info, int guard);

/* 指定のスキルを習得する */
static int learn_skillset(LPINT skillset_list, int skillset);

/* 指定の特性を開放する */
static VOID unlock_ability(LPINT ability_list, int ability);

/* 指定の特技を開放する */
static VOID unlock_skill(LPINT skill_list, int skill);

/* 特技リストの順番ソート */
static VOID sort_skill_list(LPINT skill_list, LPINT order);

/* 指定するアイテムは装備可能かを判断する */
static CONST struct item_info *check_equippable(int equipment, CONST struct race_info *ri, CONST int *ability_list);

/* 処理用セーブ情報内部で使われてるメモリーを解放 */
static VOID free_save_info(struct save_info *sav);

/* 特定モンスターはパーティーメンバーか、もしくはスタンバイかを判定 */
static BOOL check_monster_in_party_or_standby(int monster_idx, CONST struct save_fmt_body *body);

/* 全てのパーティーメンバーが削除された場合デフォルトのパーティメンバーを作成 */
static void set_default_party_member(struct save_fmt_header *header, struct save_fmt_body *body);

/* 特定モンスター情報を強制変更 */
static VOID modify_monster_info_force(struct monster_fmt *monster, CONST DQMJ_SAVE_MONSTER_INFO *in);

/* モンスター情報を全て合法値範囲内にノーマライズする */
static BOOL normalize_monster_by_handle(HDQMJSAVE handle, int monster_idx, DQMJ_SAVE_MONSTER_INFO *inout);

/* 特定モンスター情報を全てクリア */
static VOID clear_monster_info(struct monster_fmt *monster);

/* 開いたセーブファイルの読み込み処理 */
static BOOL read_save_file(HANDLE file, struct save_info *sav);

/* 新規セーブファイルの書き込み処理 */
static BOOL write_save_file(HANDLE file, struct save_info *sav, BOOL as_raw);

/* 未圧縮データを直接読み込み */
static BOOL read_uncompressed_data(HANDLE file, UINT size, struct save_info *sav);

/************************************************************************/

/* 名前データをUnicode文字列に変換する */
UINT DQMJSaveNameToString(LPWSTR str, UINT len, CONST DQMJ_NAME *name, WCHAR def)
{
	int mode;
	UINT i, j;
	WCHAR ch;

	static WCHAR lookup_table[256][3];
	static BOOL init_flag = FALSE;

	if (!name)
		return 0;

	if (!init_flag)
	{
		for (i = 0; i < _countof(lookup_table); i++)
		for (j = 0; j < _countof(lookup_table[i]); j++)
			lookup_table[i][j] = L'?';

		for (i = 0; i < _countof(NAME_LIST); i++)
		{
			if (NAME_LIST[i].str[0] == L'{')
				continue;

			for (j = 0; j < (UINT)lstrlenW(NAME_LIST[i].str); j++)
				lookup_table[NAME_LIST[i].id][j] = NAME_LIST[i].str[j];
		}

		init_flag = TRUE;
	}

	for (i = 0, j = 0, mode = 0; i < DQMJ_NAME_MAX; i++)
	{
		if (j >= len)
			return j;

		if (name->byte_code[i] == 0xFF)
			break;

		if (name->byte_code[i] == 0x92)
		{
			mode = 1;
			continue;
		}

		if (name->byte_code[i] == 0x93)
		{
			mode = 2;
			continue;
		}

		if (str != NULL)
		{
			ch = lookup_table[name->byte_code[i]][mode];
			if (ch == L'?')
				str[j] = (def == DQMJ_NAME_DEFAULT_AS_BYTECODE) ? name->byte_code[i] : def;
			else
				str[j] = ch;
		}

		mode = 0;
		j++;
	}

	if (str != NULL)
		str[j] = L'\0';

	return ++j;
}

/* Unicode文字列を名前データに変換する */
UINT DQMJSaveStringToName(DQMJ_NAME *name, LPCWSTR str)
{
	UINT i, j, k;
	BOOL loop;

	static WCHAR lookup_table[3][256];
	static BOOL init_flag = FALSE;

	if (!init_flag)
	{
		ZeroMemory(lookup_table, sizeof(lookup_table));

		for (i = 0; i < _countof(NAME_LIST); i++)
		{
			if (NAME_LIST[i].str[0] == L'{')
				continue;

			for (j = 0; j < (UINT)lstrlenW(NAME_LIST[i].str); j++)
				lookup_table[j][NAME_LIST[i].id] = NAME_LIST[i].str[j];
		}

		init_flag = TRUE;
	}

	if (name != NULL)
		EMPTY_NAME(*name);

	for (k = 0; *str != L'\0'; str++)
	{
		if ((*str & 0xFF00) == 0)
		{
			if (name != NULL)
				name->byte_code[k] = *str & 0xFF;

			k++;
		}
		else
		{
			loop = TRUE;

			for (i = 0; loop && (i < _countof(lookup_table)); i++)
			for (j = 0; loop && (j < _countof(lookup_table[i])); j++)
			{
				if (*str != lookup_table[i][j])
					continue;

				if (i == 1)
				{
					if (name != NULL)
						name->byte_code[k] = 0x92;
					k++;
				}
				else if (i == 2)
				{
					if (name != NULL)
						name->byte_code[k] = 0x93;
					k++;
				}

				if (k >= DQMJ_NAME_MAX)
					return k;

				if (name != NULL)
					name->byte_code[k] = j;

				k++;
				loop = FALSE;
			}
		}

		if (k >= DQMJ_NAME_MAX)
			return k;
	}

	if (k >= DQMJ_NAME_MAX)
		return k;

	if (name != NULL)
		name->byte_code[k] = 0xFF;

	return ++k;
}

/* モンスター種別設定情報を取得 */
BOOL DQMJSaveGetRaceSetting(int race, DQMJ_SAVE_RACE_SETTING *out)
{
	int i;
	CONST struct race_info *ri;

	if (out == NULL)
		return FALSE;

	ri = lookup_race_info(race);
	if (ri->race == 0x00)
		return FALSE;

	out->race = ri->race;
	out->rank = ri->rank;
	out->type = ri->type;
	out->name = ri->name;
	out->limit_hp = ri->limit_hp;
	out->limit_mp = ri->limit_mp;
	out->limit_atk = ri->limit_atk;
	out->limit_def = ri->limit_def;
	out->limit_agi = ri->limit_agi;
	out->limit_int = ri->limit_int;

	for (i = 0; i < DQMJ_GUARD_MAX; i++)
		out->guard[i] = GET_HALF(ri->base_guard, i);
	for (i = 0; i < DQMJ_ABILITY_MAX; i++)
		out->ability[i] = ri->base_ability[i];
	for (i = 0; i < DQMJ_SKILLSET_MAX; i++)
		out->skillset[i] = ri->base_skillset[i];
	
	return TRUE;
}

/* 道具IDから道具設定情報を取得 */
BOOL DQMJSaveGetItemSetting(int item, DQMJ_SAVE_ITEM_SETTING *out)
{
	CONST struct item_info *ii;

	if (out == NULL)
		return FALSE;

	ii = lookup_item_info(item);
	if (ii->item == 0x00)
		return FALSE;

	out->item = ii->item;
	out->type = 0;
	out->weapon = (ii->type & ITEM_WEAPON) ? TRUE : FALSE;
	out->name = ii->name;

	if (ii->type & ITEM_USABLE)
		out->type = DQMJ_ITEM_USABLE;
	else if (ii->type & ITEM_SPECIAL)
		out->type = DQMJ_ITEM_SPECIAL;
	else if (ii->type & ITEM_SWORD)
		out->type = DQMJ_ITEM_SWORD;
	else if (ii->type & ITEM_SPEAR)
		out->type = DQMJ_ITEM_SPEAR;
	else if (ii->type & ITEM_AXE)
		out->type = DQMJ_ITEM_AXE;
	else if (ii->type & ITEM_HAMMER)
		out->type = DQMJ_ITEM_HAMMER;
	else if (ii->type & ITEM_WHIP)
		out->type = DQMJ_ITEM_WHIP;
	else if (ii->type & ITEM_CLAW)
		out->type = DQMJ_ITEM_CLAW;
	else if (ii->type & ITEM_STAFF)
		out->type = DQMJ_ITEM_STAFF;
	else
		out->type = DQMJ_ITEM_NORMAL;

	return TRUE;
}

/* スキルIDからスキル情報を取得 */
BOOL DQMJSaveGetSkillsetSetting(int skillset, DQMJ_SAVE_SKILLSET_SETTING *out)
{
	int i;
	CONST struct skillset_info *ssi;

	if (out == NULL)
		return FALSE;

	ssi = lookup_skillset_info(skillset);
	if (ssi->skillset == 0x00)
		return FALSE;

	out->skillset = ssi->skillset;
	out->name = ssi->name;
	out->param = ssi->param;
	out->count = 0;
	out->max_point = 0;

	for (i = 0; i < _countof(ssi->point); i++)
	{
		if (ssi->point[i] == 0)
			break;

		out->count++;
		out->max_point = ssi->point[i];
	}

	return TRUE;
}

/* 場所IDから場所名を取得 */
LPCTSTR DQMJSaveGetPlaceName(int place)
{
	CONST struct name_info *ni;

	ni = lookup_place_info(place);
	if (ni == NULL)
		return NULL;

	return ni->name;
}

/* ガードIDからガード名を取得 */
LPCTSTR DQMJSaveGetGuardName(int guard)
{
	CONST struct name_info *ni;

	ni = lookup_guard_info(guard);
	if (ni == NULL)
		return NULL;

	return ni->name;
}

/* 特技IDから特技名を取得 */
LPCTSTR DQMJSaveGetSkillName(int skill)
{
	CONST struct skill_info *si;

	si = lookup_skill_info(skill);
	if (si->skill == 0x00)
		return NULL;

	return si->name;
}

/* 特性IDから特性名を取得 */
LPCTSTR DQMJSaveGetAbilityName(int ability)
{
	CONST struct ability_info *ai;

	ai = lookup_ability_info(ability);
	if (ai->ability == 0x00)
		return NULL;

	return ai->name;
}

/* 指定するアイテムは装備可能かを判断する */
BOOL DQMJSaveCheckEquippable(int race, int equipment, CONST int *ability_list)
{
	CONST struct race_info *ri;

	if (ability_list == NULL)
		return FALSE;

	ri = lookup_race_info(race);
	if (ri->race == 0x00)
		return FALSE;

	if (check_equippable(equipment, ri, ability_list) == NULL)
		return FALSE;

	return TRUE;
}

/* モンスタースキルに振り分けたスキルポイントから該当スキルの開放特技数を取得 */
int DQMJSaveCalcSkillActiveCount(int skillset, int skill_point)
{
	int i, n;
	CONST struct skillset_info *ssi;

	ssi = lookup_skillset_info(skillset);
	if (ssi == NULL)
		return 0;

	n = 0;

	for (i = 0; i < _countof(ssi->point); i++)
	{
		if (ssi->point[i] == 0)
			break;

		if (skill_point >= ssi->point[i])
			n++;
	}

	return n;
}

/* モンスター情報を全て合法値範囲内にノーマライズする */
BOOL DQMJSaveNormalizeMonster(BOOL interrupt, BOOL in_party_or_standby, CONST DQMJ_NAME *player_name, DQMJ_SAVE_MONSTER_INFO *inout)
{
	int i, j, max_sp;
	DQMJ_SAVE_MONSTER_INFO mi;
	CONST struct race_info *ri;
	CONST struct item_info *ii;
	CONST struct skillset_info *ssi;

	if (inout == NULL)
		return FALSE;

	ri = lookup_race_info(inout->race);
	if (ri->race == 0x00)
		return FALSE;

	mi = *inout;
	ZeroMemory(inout, sizeof(*inout));

	inout->cheat = FALSE;
	inout->dead = (interrupt && in_party_or_standby && mi.dead) ? TRUE : FALSE;
	inout->poison = FALSE;
	inout->race = ri->race;
	inout->sex = (ri->type == DQMJ_TYPE_INCARNI) ? DQMJ_SEX_BISEXUAL : (BETWEEN(mi.sex, 0, DQMJ_SEX_NUM) ? mi.sex : gen_monster_sex(ri->type));
	inout->rank = ri->rank;
	inout->type = ri->type;
	inout->combine = MINMAX(mi.combine, 0, DQMJ_COMBINE_MAX);
	inout->source = (!BETWEEN(mi.source, 0, DQMJ_SOURCE_NUM) || ((ri->type == DQMJ_TYPE_INCARNI) && ((mi.combine > 0) || ((mi.source != DQMJ_SOURCE_SYSTEM) && (mi.source != DQMJ_SOURCE_COMBINE))))) ? DQMJ_SOURCE_COMBINE : mi.source;
	inout->base_hp = MINMAX(mi.base_hp, 1, ri->limit_hp);
	inout->base_mp = MINMAX(mi.base_mp, 0, ri->limit_mp);
	inout->base_atk = MINMAX(mi.base_atk, 0, ri->limit_atk);
	inout->base_def = MINMAX(mi.base_def, 0, ri->limit_def);
	inout->base_agi = MINMAX(mi.base_agi, 0, ri->limit_agi);
	inout->base_int = MINMAX(mi.base_int, 0, ri->limit_int);
	inout->actual_hp = inout->base_hp;
	inout->actual_mp = inout->base_mp;
	inout->actual_atk = inout->base_atk;
	inout->actual_def = inout->base_def;
	inout->actual_agi = inout->base_agi;
	inout->actual_int = inout->base_int;
	inout->exp = max(mi.exp, 0);
	inout->strategy = BETWEEN(mi.strategy & 0xFF, 0, DQMJ_STRATEGY_NUM) ? (mi.strategy & 0xFF) : DQMJ_STRATEGY_GO_ALL_OUT;
	inout->remaining_sp = MINMAX(mi.remaining_sp, 0, DQMJ_SKILLPOINT_MAX);

	if (inout->combine < 5)
		inout->max_level = 50;
	else if (((inout->combine < 10) || (mi.max_level < 99)) && (mi.level <= 75))
		inout->max_level = 75;
	else
		inout->max_level = 99;

	inout->level = MINMAX(mi.level, 1, inout->max_level);

	if (BETWEEN(mi.growth_type, 0, DQMJ_GROWTH_NUM))
		inout->growth_type = mi.growth_type;
	else
		inout->growth_type = gen_growth_type();

	if ((mi.growth_start_level == DQMJ_RAND_GROWTH_LEVEL) || (mi.growth_end_level == DQMJ_RAND_GROWTH_LEVEL) || !BETWEEN(mi.growth_start_level, DQMJ_GROWTH_LEVEL_MIN, DQMJ_GROWTH_LEVEL_MAX + 1))
	{
		inout->growth_start_level = gen_growth_start_level();
		inout->growth_end_level = gen_growth_end_level(inout->growth_start_level);
	}
	else
	{
		inout->growth_start_level = mi.growth_start_level;

		if (BETWEEN(mi.growth_end_level - mi.growth_start_level + 1, DQMJ_GROWTH_RANGE_MIN, DQMJ_GROWTH_RANGE_MAX + 1))
			inout->growth_end_level = mi.growth_end_level;
		else
			inout->growth_end_level = gen_growth_end_level(mi.growth_start_level);
	}

	for (i = 0; i < DQMJ_GUARD_MAX; i++)
		inout->guard_info[i] = GET_HALF(ri->base_guard, i);

	for (i = 0; i < DQMJ_ABILITY_MAX; i++)
		inout->ability_list[i] = ri->base_ability[i];

	for (i = 0; i < DQMJ_SKILLSET_MAX; i++)
	{
		if (lookup_skillset_info(mi.skillset[i])->skillset == 0x00)
			continue;

		j = learn_skillset(inout->skillset, mi.skillset[i]);
		if (j < 0)
			break;

		inout->assigned_sp[j] += mi.assigned_sp[i];
	}

	for (i = 0; i < DQMJ_SKILLSET_MAX; i++)
	{
		ssi = lookup_skillset_info(inout->skillset[i]);
		if (ssi->skillset == 0x00)
			break;

		for (j = 0, max_sp = 0; j < _countof(ssi->point); j++)
		{
			if (ssi->point[j] == 0)
				break;
			if (max_sp < ssi->point[j])
				max_sp = ssi->point[j];
		}

		inout->assigned_sp[i] = MINMAX(inout->assigned_sp[i], 0, max_sp);
		inout->skill_active_num[i] = DQMJSaveCalcSkillActiveCount(ssi->skillset, inout->assigned_sp[i]);

		for (j = 0; j < inout->skill_active_num[i]; j++)
		{
			switch (ssi->type[j])
			{
			case UNLOCK_STATUS_HP:
				inout->actual_hp += ssi->value[j];
				break;
			case UNLOCK_STATUS_MP:
				inout->actual_mp += ssi->value[j];
				break;
			case UNLOCK_STATUS_ATK:
				inout->actual_atk += ssi->value[j];
				break;
			case UNLOCK_STATUS_DEF:
				inout->actual_def += ssi->value[j];
				break;
			case UNLOCK_STATUS_AGI:
				inout->actual_agi += ssi->value[j];
				break;
			case UNLOCK_STATUS_INT:
				inout->actual_int += ssi->value[j];
				break;
			case UNLOCK_GUARD:
				upgrade_guard(inout->guard_info, ssi->value[j]);
				break;
			case UNLOCK_ABILITY:
				unlock_ability(inout->ability_list, ssi->value[j]);
				break;
			case UNLOCK_SKILL:
				unlock_skill(inout->skill_list, ssi->value[j]);
				break;
			}
		}
	}

	sort_skill_list(inout->skill_list, (LPINT)mi.skill_list);

	if (in_party_or_standby)
	{
		ii = check_equippable(mi.weapon, ri, inout->ability_list);
		if (ii != NULL)
		{
			inout->weapon = ii->item;
			inout->actual_atk += ii->add_atk;
			inout->actual_def += ii->add_def;
			inout->actual_agi += ii->add_agi;
			inout->actual_int += ii->add_int;
		}
	}

	if (interrupt && in_party_or_standby)
	{
		inout->current_hp = inout->dead ? 0 : MINMAX(mi.current_hp, 1, inout->actual_hp);
		inout->current_mp = MINMAX(mi.current_mp, 0, inout->actual_mp);
	}
	else
	{
		inout->current_hp = inout->actual_hp;
		inout->current_mp = inout->actual_mp;
	}

	inout->name = mi.name;
	inout->master = mi.master;

	if (player_name && (inout->master.byte_code[0] == 0xFF))
		inout->master = *player_name;

	for (i = 0; i < DQMJ_PARENT_NUM; i++)
	{
		inout->parent_race[i] = lookup_race_info(mi.parent_race[i])->race;
		inout->parent_name[i] = mi.parent_name[i];
		inout->parent_master[i] = mi.parent_master[i];
	}

	if ((inout->parent_race[DQMJ_PARENT_FATHER] == 0x00) || (inout->parent_race[DQMJ_PARENT_MOTHER] == 0x00))
	{
		EMPTY_NAME(inout->parent_name[DQMJ_PARENT_FATHER]);
		EMPTY_NAME(inout->parent_name[DQMJ_PARENT_MOTHER]);

		inout->parent_race[DQMJ_PARENT_FATHER] = 0x00;
		inout->parent_race[DQMJ_PARENT_MOTHER] = 0x00;

		return TRUE;
	}

	for (i = 0; i < DQMJ_PARENT_NUM; i++)
	{
		if (inout->parent_master[i].byte_code[0] == 0xFF)
			inout->parent_master[i] = UNKNOWN_MASTER;

		inout->grandparent_race[i][DQMJ_PARENT_FATHER] = lookup_race_info(mi.grandparent_race[i][DQMJ_PARENT_FATHER])->race;
		inout->grandparent_race[i][DQMJ_PARENT_MOTHER] = lookup_race_info(mi.grandparent_race[i][DQMJ_PARENT_MOTHER])->race;

		if ((inout->grandparent_race[i][DQMJ_PARENT_FATHER] == 0x00) || (inout->grandparent_race[i][DQMJ_PARENT_MOTHER] == 0x00))
		{
			inout->grandparent_race[i][DQMJ_PARENT_FATHER] = 0x00;
			inout->grandparent_race[i][DQMJ_PARENT_MOTHER] = 0x00;
		}
	}

	return TRUE;
}

/* セーブファイルを開く */
HDQMJSAVE DQMJSaveOpenFile(LPCTSTR file_path)
{
	BOOL ok;
	HANDLE file;
	struct save_info sav;
	HDQMJSAVE handle;

	if ((file_path == NULL) || (*file_path == TEXT('\0')))
		return NULL;

	file = CreateFile(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return NULL;

	ZeroMemory(&sav, sizeof(sav));

	ok = read_save_file(file, &sav);
	CloseHandle(file);

	if (ok)
	{
		handle = HeapAlloc(GetProcessHeap(), 0, sizeof(sav));
		if (handle != NULL)
		{
			CopyMemory(handle, &sav, sizeof(sav));
			return handle;
		}
	}

	free_save_info(&sav);
	return NULL;
}

/* セーブファイルを閉じる */
BOOL DQMJSaveCloseFile(HDQMJSAVE handle)
{
	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
		return FALSE;

	free_save_info((struct save_info *)handle);
	HeapFree(GetProcessHeap(), 0, handle);
	return TRUE;
}

/* 名前を付けてセーブファイルを保存 */
BOOL DQMJSaveSaveToFile(HDQMJSAVE handle, LPCTSTR file_path, BOOL as_raw)
{
	BOOL ok;
	HANDLE file;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
		return FALSE;

	if ((file_path == NULL) || (*file_path == TEXT('\0')))
		return FALSE;

	file = CreateFile(file_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return FALSE;

	ok = write_save_file(file, (struct save_info *)handle, as_raw);
	CloseHandle(file);

	return ok;
}

/* セーブデータファイルフォーマットを取得 */
int DQMJSaveQueryFileFormat(HDQMJSAVE handle)
{
	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
		return DQMJ_FORMAT_INVALID;

	return ((struct save_info *)handle)->format;
}

/* セーブファイルから一括（ロード画面表示用）情報を取得 */
BOOL DQMJSaveQueryBriefing(HDQMJSAVE handle, DQMJ_SAVE_BRIEFING *out)
{
	int i;
	struct save_fmt_header *header;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (out == NULL))
		return FALSE;

	header = GET_HEADER(handle);
	out->interrupt = IS_INTR(handle);
	out->place_id = header->place_id;
	out->party_member_num = header->party_member_num;
	out->story_progress = max(header->story_progress / 5, 10);
	COPY_NAME(out->player_name, header->player_name);
	timestamp_to_hms(header->play_time, &out->play_time_hour, &out->play_time_min, &out->play_time_sec);

	for (i = 0; i < DQMJ_MEMBER_MAX; i++)
	{
		out->party_member_race[i] = header->party_member_race[i];
		out->party_member_level[i] = header->party_member_level[i];
		COPY_NAME(out->party_member_name[i], header->party_member_name[i]);
	}

	return TRUE;
}

/* セーブファイルからプレイ情報を取得 */
BOOL DQMJSaveQueryPlayInfo(HDQMJSAVE handle, DQMJ_SAVE_PLAY_INFO *out)
{
	int i;
	struct save_fmt_body *body;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (out == NULL))
		return FALSE;

	body = GET_BODY(handle);
	out->gold = body->gold;
	out->deposit = body->deposit;
	out->victory_times = body->victory_times;
	out->scout_times = body->scout_times;
	out->combine_times = body->combine_times;
	COPY_NAME(out->player_name, body->player_name);
	timestamp_to_hms(body->play_time, &out->play_time_hour, &out->play_time_min, &out->play_time_sec);

	for (i = 0; i < DQMJ_PLAYERSKILL_NUM; i++)
		out->player_skill[i] = (body->player_skill & (1 << i)) ? TRUE : FALSE;

	return TRUE;
}

/* セーブファイルから道具情報を取得 */
BOOL DQMJSaveQueryItemInfo(HDQMJSAVE handle, DQMJ_SAVE_ITEM_INFO *out)
{
	int i;
	struct save_fmt_body *body;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (out == NULL))
		return FALSE;

	body = GET_BODY(handle);
	out->bookstore_inited = body->bookstore_inited ? TRUE : FALSE;

	for (i = 0; i < DQMJ_BAGGAGE_MAX; i++)
		out->baggage[i] = body->baggage[i];
	for (i = 0; i < DQMJ_ITEMLIST_LEN; i++)
		out->inventory[i] = body->inventory[i];
	for (i = 0; i < DQMJ_SKILLBOOK_NUM; i++)
		out->bookstore_onsale[i] = body->bookstore_onsale[i] ? TRUE : FALSE;

	return TRUE;
}

/* セーブファイルからライブラリ情報を取得 */
BOOL DQMJSaveQueryLibraryInfo(HDQMJSAVE handle, DQMJ_SAVE_LIBRARY_INFO *out)
{
	int i;
	struct save_fmt_body *body;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (out == NULL))
		return FALSE;

	body = GET_BODY(handle);

	for (i = 0; i < DQMJ_MONSTERLIST_LEN; i++)
	{
		out->encounter[i] = GET_BIT(body->encounter_library, i);
		out->kill[i] = GET_BIT(body->kill_library, i);
		out->obtain[i] = GET_BIT(body->obtain_library, i);
	}

	for (i = 0; i < DQMJ_SKILLSETLIST_LEN; i++)
	{
		if (GET_BIT(body->skill_library, i) == 0)
			out->skill[i] = DQMJ_SKILL_LOCKED;
		else
			out->skill[i] = min(GET_HALF(body->skill_library + 0x18, i), DQMJ_SKILL_COMPLETED);
	}

	return TRUE;
}

/* セーブファイルから牧場情報を取得 */
BOOL DQMJSaveQueryRanchInfo(HDQMJSAVE handle, DQMJ_SAVE_RANCH_INFO *out)
{
	int i;
	struct save_fmt_body *body;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (out == NULL))
		return FALSE;

	body = GET_BODY(handle);
	out->monster_num = min(body->monster_num, DQMJ_MONSTER_MAX);
	out->party_member_num = min(body->party_member_num, DQMJ_MEMBER_MAX);
	out->standby_num = min(body->standby_num, DQMJ_MEMBER_MAX);

	for (i = 0; i < DQMJ_MEMBER_MAX; i++)
	{
		out->party_member_idx[i] = body->party_member_idx[i];
		out->standby_idx[i] = body->standby_idx[i];
	}

	return TRUE;
}

/* セーブファイルから特定モンスター情報を取得 */
BOOL DQMJSaveQueryMonsterInfo(HDQMJSAVE handle, int monster_idx, DQMJ_SAVE_MONSTER_INFO *out)
{
	int i;
	struct save_fmt_body *body;
	struct monster_fmt *monster;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (out == NULL))
		return FALSE;

	body = GET_BODY(handle);

	if (!BETWEEN(monster_idx, 0, DQMJ_MONSTER_MAX) || (monster_idx >= body->monster_num))
		return FALSE;

	monster = body->monster_list + monster_idx;

	out->cheat = monster->cheat ? TRUE : FALSE;
	out->dead = (monster->status & STATUS_DEAD) ? TRUE : FALSE;
	out->poison = (monster->status & STATUS_POISON) ? TRUE : FALSE;
	out->race = monster->race;
	out->rank = monster->rank;
	out->type = monster->type;
	out->sex = monster->sex;
	out->combine = monster->combine;
	out->source = monster->source;
	out->level = monster->level;
	out->max_level = monster->max_level;
	out->base_hp = monster->base_hp;
	out->base_mp = monster->base_mp;
	out->base_atk = monster->base_atk;
	out->base_def = monster->base_def;
	out->base_agi = monster->base_agi;
	out->base_int = monster->base_int;
	out->current_hp = monster->current_hp;
	out->current_mp = monster->current_mp;
	out->actual_hp = monster->actual_hp;
	out->actual_mp = monster->actual_mp;
	out->actual_atk = monster->actual_atk;
	out->actual_def = monster->actual_def;
	out->actual_agi = monster->actual_agi;
	out->actual_int = monster->actual_int;
	out->exp = monster->exp;
	out->strategy = monster->strategy;
	out->growth_type = monster->growth_type;
	out->growth_start_level = monster->growth_level[0];
	out->growth_end_level = monster->growth_level[1];
	out->weapon = monster->weapon;
	out->remaining_sp = monster->remaining_sp;
	out->parent_race[DQMJ_PARENT_FATHER] = monster->father_race;
	out->parent_race[DQMJ_PARENT_MOTHER] = monster->mother_race;
	out->grandparent_race[DQMJ_PARENT_FATHER][DQMJ_PARENT_FATHER] = monster->father_father_race;
	out->grandparent_race[DQMJ_PARENT_FATHER][DQMJ_PARENT_MOTHER] = monster->father_mother_race;
	out->grandparent_race[DQMJ_PARENT_MOTHER][DQMJ_PARENT_FATHER] = monster->mother_father_race;
	out->grandparent_race[DQMJ_PARENT_MOTHER][DQMJ_PARENT_MOTHER] = monster->mother_mother_race;

	COPY_NAME(out->name, monster->name);
	COPY_NAME(out->master, monster->master);
	COPY_NAME(out->parent_name[DQMJ_PARENT_FATHER], monster->father_name);
	COPY_NAME(out->parent_name[DQMJ_PARENT_MOTHER], monster->mother_name);
	COPY_NAME(out->parent_master[DQMJ_PARENT_FATHER], monster->father_master);
	COPY_NAME(out->parent_master[DQMJ_PARENT_MOTHER], monster->mother_master);

	for (i = 0; i < DQMJ_SKILLSET_MAX; i++)
	{
		out->skillset[i] = monster->skillset[i];
		out->assigned_sp[i] = monster->assigned_sp[i];
	}

	for (i = 0; i < DQMJ_GUARD_MAX; i++)
		out->guard_info[i] = monster->actual_guard[i];
	for (i = 0; i < DQMJ_SKILL_MAX; i++)
		out->skill_list[i] = monster->skill_list[i];
	for (i = 0; i < DQMJ_ABILITY_MAX; i++)
		out->ability_list[i] = monster->ability_list[i];

	return TRUE;
}

/* セーブファイルのプレイ情報を変更 */
BOOL DQMJSaveModifyPlayInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_PLAY_INFO *in)
{
	int i;
	struct save_fmt_body *body;
	struct save_fmt_header *header;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (in == NULL))
		return FALSE;

	body = GET_BODY(handle);
	body->gold = MINMAX(in->gold, 0, DQMJ_GOLD_MAX);
	body->deposit = MINMAX(in->deposit, 0, DQMJ_GOLD_MAX);
	body->scout_times = MINMAX(in->scout_times, 0, DQMJ_RECORD_MAX);
	body->victory_times = MINMAX(in->victory_times, 0, DQMJ_RECORD_MAX);
	body->combine_times = MINMAX(in->combine_times, 0, DQMJ_RECORD_MAX);
	body->player_skill = 0;
	COPY_NAME(body->player_name, in->player_name);
	body->play_time = hms_to_timestamp(in->play_time_hour, in->play_time_min, in->play_time_sec, body->play_time);

	for (i = 0; i < DQMJ_PLAYERSKILL_NUM; i++)
	{
		if (in->player_skill[i])
			SET_BIT(&body->player_skill, i);
	}

	header = GET_HEADER(handle);
	COPY_NAME(header->player_name, body->player_name);
	header->play_time = body->play_time;

	return TRUE;
}

/* セーブファイルの道具情報を変更 */
BOOL DQMJSaveModifyItemInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_ITEM_INFO *in)
{
	int i, j;
	struct save_fmt_body *body;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (in == NULL))
		return FALSE;

	body = GET_BODY(handle);
	body->bookstore_inited = in->bookstore_inited ? 1 : 0;
	ZeroMemory(body->baggage, sizeof(body->baggage));

	for (i = 0, j = 0; i < DQMJ_BAGGAGE_MAX; i++)
	{
		if (lookup_item_info(in->baggage[i])->item != 0x00)
			body->baggage[j++] = in->baggage[i];
	}

	for (i = 0; i < DQMJ_ITEMLIST_LEN; i++)
		body->inventory[i] = MINMAX(in->inventory[i], 0, DQMJ_ITEM_MAX);
	for (i = 0; i < DQMJ_SKILLBOOK_NUM; i++)
		body->bookstore_onsale[i] = in->bookstore_onsale[i] ? 1 : 0;

	return TRUE;
}

/* セーブファイルのライブラリ情報を変更 */
BOOL DQMJSaveModifyLibraryInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_LIBRARY_INFO *in)
{
	int i, n;
	struct save_fmt_body *body;
	CONST struct skillset_info *ssi;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (in == NULL))
		return FALSE;

	body = GET_BODY(handle);
	ZeroMemory(body->encounter_library, sizeof(body->encounter_library));
	ZeroMemory(body->kill_library, sizeof(body->kill_library));
	ZeroMemory(body->obtain_library, sizeof(body->obtain_library));
	ZeroMemory(body->skill_library, sizeof(body->skill_library));

	for (i = 0; i < DQMJ_MONSTERLIST_LEN; i++)
	{
		if (in->encounter[i])
			SET_BIT(body->encounter_library, i);
		if (in->kill[i])
			SET_BIT(body->kill_library, i);
		if (in->obtain[i])
			SET_BIT(body->obtain_library, i);
	}

	for (i = 0; i < DQMJ_SKILLSETLIST_LEN; i++)
	{
		if (in->skill[i] <= DQMJ_SKILL_LOCKED)
			continue;

		ssi = lookup_skillset_info(i);
		for (n = 0; n < _countof(ssi->point); n++)
		{
			if (ssi->point[n] == 0)
				break;
		}

		SET_BIT(body->skill_library, i);
		SET_HALF(body->skill_library + 0x18, i, (in->skill[i] < n) ? in->skill[i] : DQMJ_SKILL_COMPLETED);
	}

	return TRUE;
}

/* セーブファイルの牧場情報を変更 */
BOOL DQMJSaveModifyRanchInfo(HDQMJSAVE handle, CONST DQMJ_SAVE_RANCH_INFO *in)
{
	int i, j, n, idx;
	struct save_fmt_body *body;
	struct save_fmt_header *header;
	struct monster_fmt *monster;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (in == NULL))
		return FALSE;

	body = GET_BODY(handle);
	body->monster_num = MINMAX(in->monster_num, 0, DQMJ_MONSTER_MAX);
	body->party_member_num = 0;
	body->standby_num = 0;
	ZeroMemory(body->party_member_idx, sizeof(body->party_member_idx));
	ZeroMemory(body->standby_idx, sizeof(body->standby_idx));

	n = MINMAX(in->party_member_num, 0, DQMJ_MEMBER_MAX);
	for (i = 0; i < n; i++)
	{
		idx = in->party_member_idx[i];
		if ((idx < 0) || (idx >= body->monster_num))
			continue;

		for (j = 0; j < body->party_member_num; j++)
		{
			if (idx == body->party_member_idx[j])
				break;
		}

		if (j >= body->party_member_num)
			body->party_member_idx[body->party_member_num++] = idx;
	}

	n = MINMAX(in->standby_num, 0, DQMJ_MEMBER_MAX);
	for (i = 0; i < n; i++)
	{
		idx = in->standby_idx[i];
		if ((idx < 0) || (idx >= body->monster_num))
			continue;

		for (j = 0; j < body->standby_num; j++)
		{
			if (idx == body->standby_idx[j])
				break;
		}

		if (j < body->standby_num)
			continue;

		for (j = 0; j < body->party_member_num; j++)
		{
			if (idx == body->party_member_idx[j])
				break;
		}

		if (j >= body->party_member_num)
			body->standby_idx[body->standby_num++] = idx;
	}

	header = GET_HEADER(handle);
	header->party_member_num = body->party_member_num;
	ZeroMemory(header->party_member_name, sizeof(header->party_member_name));
	ZeroMemory(header->party_member_race, sizeof(header->party_member_race));
	ZeroMemory(header->party_member_level, sizeof(header->party_member_level));

	for (i = 0; i < header->party_member_num; i++)
	{
		monster = body->monster_list + body->party_member_idx[i];
		header->party_member_race[i] = monster->race;
		header->party_member_level[i] = monster->level;
		COPY_NAME(header->party_member_name[i], monster->name);
	}

	return TRUE;
}

/* セーブファイルの特定モンスター情報を変更 */
BOOL DQMJSaveModifyMonster(HDQMJSAVE handle, int monster_idx, CONST DQMJ_SAVE_MONSTER_INFO *in, BOOL normalize)
{
	int i;
	DQMJ_SAVE_MONSTER_INFO mi;
	struct save_fmt_body *body;
	struct save_fmt_header *header;
	struct monster_fmt *monster;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (in == NULL))
		return FALSE;

	body = GET_BODY(handle);

	if (!BETWEEN(monster_idx, 0, min(body->monster_num, DQMJ_MONSTER_MAX)))
		return FALSE;

	monster = body->monster_list + monster_idx;

	if (normalize)
	{
		mi = *in;

		if (!normalize_monster_by_handle(handle, monster_idx, &mi))
			return FALSE;

		monster->ability_mask = 0;
		monster->unused1 = 0;
		monster->unused2 = 0;
		monster->unused3 = 0;
		monster->unused4 = 0;

		ZeroMemory(monster->skillset, sizeof(monster->skillset));
		ZeroMemory(monster->assigned_sp, sizeof(monster->assigned_sp));
		ZeroMemory(monster->skill_active_num, sizeof(monster->skill_active_num));
		ZeroMemory(monster->actual_guard, sizeof(monster->actual_guard));
		ZeroMemory(monster->skill_list, sizeof(monster->skill_list));
		ZeroMemory(monster->ability_list, sizeof(monster->ability_list));

		in = &mi;
	}

	modify_monster_info_force(monster, in);

	header = GET_HEADER(handle);

	for (i = 0; i < (int)min(body->party_member_num, _countof(body->party_member_idx)); i++)
	{
		if (body->party_member_idx[i] != monster_idx)
			continue;

		header->party_member_race[i] = monster->race;
		header->party_member_level[i] = monster->level;
		COPY_NAME(header->party_member_name[i], monster->name);
		break;
	}

	return TRUE;
}

/* セーブファイルに新しいモンスターを作成して先頭に追加 */
BOOL DQMJSaveNewMonster(HDQMJSAVE handle, CONST DQMJ_SAVE_MONSTER_INFO *in, BOOL normalize)
{
	int i;
	DQMJ_SAVE_MONSTER_INFO mi;
	struct save_fmt_body *body;
	struct monster_fmt monster;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (in == NULL))
		return FALSE;

	body = GET_BODY(handle);

	if (body->monster_num >= DQMJ_MONSTER_MAX)
		return FALSE;

	clear_monster_info(&monster);

	if (normalize)
	{
		mi = *in;

		if (!normalize_monster_by_handle(handle, -1, &mi))
			return FALSE;

		in = &mi;
	}

	for (i = body->monster_num; i > 0; i--)
		body->monster_list[i] = body->monster_list[i - 1];

	modify_monster_info_force(&monster, in);
	body->monster_list[0] = monster;
	body->monster_num++;

	for (i = 0; i < (int)min(body->party_member_num, _countof(body->party_member_idx)); i++)
		body->party_member_idx[i]++;

	for (i = 0; i < (int)min(body->standby_num, _countof(body->standby_idx)); i++)
		body->standby_idx[i]++;

	return TRUE;
}

/* セーブファイルの特定モンスターの位置を移動 */
BOOL DQMJSaveMoveMonster(HDQMJSAVE handle, int monster_idx, int moveto_idx)
{
	int i, j;
	struct save_fmt_body *body;
	struct monster_fmt monster;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
		return FALSE;

	body = GET_BODY(handle);

	if (!BETWEEN(monster_idx, 0, min(body->monster_num, DQMJ_MONSTER_MAX)) || !BETWEEN(moveto_idx, 0, min(body->monster_num, DQMJ_MONSTER_MAX)))
		return FALSE;

	if (monster_idx == moveto_idx)
		return TRUE;

	if (monster_idx < moveto_idx)
	{
		monster = body->monster_list[monster_idx];

		for (i = monster_idx, j = i + 1; j <= moveto_idx; i++, j++)
			body->monster_list[i] = body->monster_list[j];

		body->monster_list[moveto_idx] = monster;

		for (i = 0; i < (int)min(body->party_member_num, _countof(body->party_member_idx)); i++)
		{
			if (body->party_member_idx[i] == monster_idx)
				body->party_member_idx[i] = moveto_idx;
			else if ((body->party_member_idx[i] > monster_idx) && (body->party_member_idx[i] <= moveto_idx))
				body->party_member_idx[i]--;
		}

		for (i = 0; i < (int)min(body->standby_num, _countof(body->standby_idx)); i++)
		{
			if (body->standby_idx[i] == monster_idx)
				body->standby_idx[i] = moveto_idx;
			else if ((body->standby_idx[i] > monster_idx) && (body->standby_idx[i] <= moveto_idx))
				body->standby_idx[i]--;
		}
	}
	else
	{
		monster = body->monster_list[monster_idx];

		for (i = monster_idx, j = i - 1; j >= moveto_idx; i--, j--)
			body->monster_list[i] = body->monster_list[j];

		body->monster_list[moveto_idx] = monster;

		for (i = 0; i < (int)min(body->party_member_num, _countof(body->party_member_idx)); i++)
		{
			if (body->party_member_idx[i] == monster_idx)
				body->party_member_idx[i] = moveto_idx;
			else if ((body->party_member_idx[i] >= moveto_idx) && (body->party_member_idx[i] < monster_idx))
				body->party_member_idx[i]++;
		}

		for (i = 0; i < (int)min(body->standby_num, _countof(body->standby_idx)); i++)
		{
			if (body->standby_idx[i] == monster_idx)
				body->standby_idx[i] = moveto_idx;
			else if ((body->standby_idx[i] >= moveto_idx) && (body->standby_idx[i] < monster_idx))
				body->standby_idx[i]++;
		}
	}

	return TRUE;
}

/* セーブファイルの特定モンスターをコピーして先頭に追加 */
BOOL DQMJSaveCopyMonster(HDQMJSAVE handle, int monster_idx)
{
	int i;
	struct monster_fmt monster;
	struct save_fmt_body *body;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
		return FALSE;

	body = GET_BODY(handle);

	if ((body->monster_num >= DQMJ_MONSTER_MAX) || !BETWEEN(monster_idx, 0, body->monster_num))
		return FALSE;

	monster = body->monster_list[monster_idx];

	for (i = body->monster_num; i > 0; i--)
		body->monster_list[i] = body->monster_list[i - 1];

	body->monster_list[0] = monster;
	body->monster_num++;

	for (i = 0; i < (int)min(body->party_member_num, _countof(body->party_member_idx)); i++)
		body->party_member_idx[i]++;

	for (i = 0; i < (int)min(body->standby_num, _countof(body->standby_idx)); i++)
		body->standby_idx[i]++;

	return TRUE;
}

/* セーブファイルの特定モンスターを削除 */
BOOL DQMJSaveRemoveMonster(HDQMJSAVE handle, int monster_idx)
{
	int i, j, n;
	struct save_fmt_body *body;
	struct save_fmt_header *header;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE))
		return FALSE;

	body = GET_BODY(handle);

	n = min(body->monster_num, DQMJ_MONSTER_MAX);
	if ((n <= 1) || !BETWEEN(monster_idx, 0, n))
		return FALSE;

	for (i = monster_idx, j = i + 1; j < n; i++, j++)
		body->monster_list[i] = body->monster_list[j];

	body->monster_num = n - 1;
	clear_monster_info(body->monster_list + body->monster_num);

	header = GET_HEADER(handle);

	n = min(body->standby_num, _countof(body->standby_idx));
	for (i = 0; i < n; i++)
	{
		if (body->standby_idx[i] != monster_idx)
			continue;

		for (j = i + 1; j < n; i++, j++)
			body->standby_idx[i] = body->standby_idx[j];

		body->standby_num = --n;
		break;
	}

	for (i = 0; i < body->standby_num; i++)
	{
		if (body->standby_idx[i] > monster_idx)
			body->standby_idx[i]--;
	}

	n = min(body->party_member_num, _countof(body->party_member_idx));
	for (i = 0; i < n; i++)
	{
		if (body->party_member_idx[i] != monster_idx)
			continue;

		for (j = i + 1; j < n; i++, j++)
		{
			body->party_member_idx[i] = body->party_member_idx[j];
			header->party_member_race[i] = header->party_member_race[j];
			header->party_member_level[i] = header->party_member_level[j];
			COPY_NAME(header->party_member_name[i], header->party_member_name[j]);
		}

		if (--n > 0)
		{
			body->party_member_num = n;
			header->party_member_num = n;
			break;
		}

		set_default_party_member(header, body);
		return TRUE;
	}

	for (i = 0; i < body->party_member_num; i++)
	{
		if (body->party_member_idx[i] > monster_idx)
			body->party_member_idx[i]--;
	}

	return TRUE;
}

/************************************************************************/

/* セーブデータのチェックサムを算出 */
static UINT32 calc_save_checksum(struct save_fmt *sav)
{
	UINT32 checksum;
	PUINT32 ptr;

	checksum = 0;
	ptr = (PUINT32)&sav->body;

	while ((PUINT8)ptr < sav->tail)
		checksum += *ptr++;

	return checksum;
}

/* モンスターデータのチェックサムを算出 */
static UINT32 calc_monster_checksum(struct monster_fmt *monster)
{
	int i;
	UINT32 checksum;
	
	checksum = 0;
	checksum += monster->race;
	checksum += monster->sex;
	checksum += monster->combine;
	checksum += monster->level;
	checksum += monster->max_level;
	checksum += monster->base_hp;
	checksum += monster->base_mp;
	checksum += monster->base_atk;
	checksum += monster->base_def;
	checksum += monster->base_agi;
	checksum += monster->base_int;
	checksum += monster->actual_hp;
	checksum += monster->actual_mp;
	checksum += monster->actual_atk;
	checksum += monster->actual_def;
	checksum += monster->actual_agi;
	checksum += monster->actual_int;
	checksum += monster->weapon;
	checksum += monster->remaining_sp;

	for (i = 0; i < _countof(monster->rom_data1); i++)
		checksum += monster->rom_data1[i];
	for (i = 0; i < _countof(monster->rom_data2); i++)
		checksum += monster->rom_data2[i];
	for (i = 0; i < _countof(monster->base_guard); i++)
		checksum += monster->base_guard[i];
	for (i = 0; i < _countof(monster->actual_guard); i++)
		checksum += monster->actual_guard[i];
	for (i = 0; i < _countof(monster->skillset); i++)
		checksum += monster->skillset[i];
	for (i = 0; i < _countof(monster->assigned_sp); i++)
		checksum += monster->assigned_sp[i];
	for (i = 0; i < _countof(monster->skill_active_num); i++)
		checksum += monster->skill_active_num[i];
	for (i = 0; i < _countof(monster->skill_list); i++)
		checksum += monster->skill_list[i];
	for (i = 0; i < _countof(monster->ability_list); i++)
		checksum += monster->ability_list[i];

	return checksum;
}

/* セーブ用タイムスタンプを時分秒に変換 */
static VOID timestamp_to_hms(UINT32 timestamp, LPINT hour, LPINT minute, LPINT second)
{
	*hour = (timestamp & 0xFFFC0000) >> 18;
	*minute = (timestamp & 0x0003F000) >> 12;
	*second = (timestamp & 0x00000FC0) >> 6;
}

/* 時分秒をセーブ用タイムスタンプに変換 */
static UINT32 hms_to_timestamp(int hour, int minute, int second, UINT32 timestamp)
{
	return (hour << 18) | ((minute & 0x3F) << 12) | ((second & 0x3F) << 6) | (timestamp & 0x003F);
}

/* 指定の固有情報を探す */
static LPCVOID lookup_info_base(int id, size_t size, LPCVOID head, int num, int def)
{
	int a, b, i;
	CONST int *p;

	a = 0;
	b = num - 1;

	while (a <= b)
	{
		i = (a + b) / 2;
		p = (CONST int *)((LPCBYTE)head + size * i);

		if (*p < id)
			a = i + 1;
		else if (*p > id)
			b = i - 1;
		else
			return p;
	}

	if (def >= 0)
		return (LPCBYTE)head + size * def;

	return NULL;
}

/* 指定のガードを1段階上げる */
static VOID upgrade_guard(LPINT guard_info, int guard)
{
	if (guard_info[guard] < 0x05)		/* 無効まで */
		guard_info[guard]++;
}

/* 指定のスキルを習得する */
static int learn_skillset(LPINT skillset_list, int skillset)
{
	int i;
	CONST struct skillset_info *ssi;

	ssi = lookup_skillset_info(skillset);

	for (i = 0; i < DQMJ_SKILLSET_MAX; i++)
	{
		if (skillset_list[i] == 0x00)
			break;

		if (skillset_list[i] == skillset)
			return i;
	}

	if (i >= DQMJ_SKILLSET_MAX)
		return -1;

	skillset_list[i] = skillset;
	return i;
}

/* 指定の特性を開放する */
static VOID unlock_ability(LPINT ability_list, int ability)
{
	int i;
	CONST struct ability_info *ai;

	ai = lookup_ability_info(ability);

	for (i = 0; i < DQMJ_ABILITY_MAX; i++)
	{
		if (ability_list[i] == ability)
			return;

		if (lookup_ability_info(ability_list[i])->ability == 0x00)
		{
			ability_list[i] = ability;
			return;
		}

		if (ability_list[i] == ai->upper)
			return;

		if (ability_list[i] == ai->lower)
		{
			ability_list[i] = ability;
			return;
		}
	}
}

/* 指定の特技を開放する */
static VOID unlock_skill(LPINT skill_list, int skill)
{
	int i, j;
	CONST struct skill_info *si;

	si = lookup_skill_info(skill);

	for (i = 0; i < DQMJ_SKILL_MAX; i++)
	{
		if (skill_list[i] == skill)
			return;

		if (lookup_skill_info(skill_list[i])->skill == 0x00)
		{
			skill_list[i] = skill;
			return;
		}

		for (j = 0; j < _countof(si->upper); j++)
		{
			if (si->upper[j] == 0x00)
				break;

			if (skill_list[i] == si->upper[j])
				return;
		}

		for (j = 0; j < _countof(si->lower); j++)
		{
			if (si->lower[j] == 0x00)
				break;

			if (skill_list[i] == si->lower[j])
			{
				skill_list[i] = skill;
				return;
			}
		}
	}
}

/* 特技リストの順番ソート */
static VOID sort_skill_list(LPINT skill_list, LPINT order)
{
	int i, j, k;
	int list[DQMJ_SKILL_MAX];

	CopyMemory(list, skill_list, sizeof(list));

	k = 0;

	for (i = 0; i < DQMJ_SKILL_MAX; i++)
	{
		if (order[i] == 0x00)
			continue;

		for (j = 0; j < _countof(list); j++)
		{
			if (list[j] != order[i])
				continue;

			list[j] = 0x00;
			skill_list[k++] = order[i];
		}
	}

	for (i = 0; i < _countof(list); i++)
	{
		if (list[i] != 0x00)
			skill_list[k++] = list[i];
	}
}

/* 指定するアイテムは装備可能かを判断する */
static CONST struct item_info *check_equippable(int equipment, CONST struct race_info *ri, CONST int *ability_list)
{
	int i = 0;
	CONST struct item_info *ii;

	ii = lookup_item_info(equipment);
	if ((ii->item == 0x00) || ((ii->type & ITEM_WEAPON) == 0))
		return NULL;

	if ((ri->equip_mask & ii->type) != 0)
		return ii;

	for (i = 0; i < DQMJ_ABILITY_MAX; i++)
	{
		if (ability_list[i] == 0x1B)
			return ii;
	}

	return NULL;
}

/* 処理用セーブ情報内部で使われてるメモリーを解放 */
static VOID free_save_info(struct save_info *sav)
{
	switch (sav->format)
	{
	case DQMJ_FORMAT_DESMUME:
		HeapFree(GetProcessHeap(), 0, sav->extend_data.desmume_info.dummy);
		break;
	case DQMJ_FORMAT_NOCASHBGA:
		HeapFree(GetProcessHeap(), 0, sav->extend_data.nocashgba_info.footer);
		break;
	}

	HeapFree(GetProcessHeap(), 0, sav->raw_data);
}

/* 特定モンスターはパーティーメンバーか、もしくはスタンバイかを判定 */
static BOOL check_monster_in_party_or_standby(int monster_idx, CONST struct save_fmt_body *body)
{
	int i;

	for (i = 0; i < (int)min(body->party_member_num, _countof(body->party_member_idx)); i++)
	{
		if (body->party_member_idx[i] == monster_idx)
			return TRUE;
	}

	for (i = 0; i < (int)min(body->standby_num, _countof(body->standby_idx)); i++)
	{
		if (body->standby_idx[i] == monster_idx)
			return TRUE;
	}

	return FALSE;
}

/* 全てのパーティーメンバーが削除された場合デフォルトのパーティメンバーを作成 */
static void set_default_party_member(struct save_fmt_header *header, struct save_fmt_body *body)
{
	int idx, i, j;

	idx = body->monster_num - 1;

	body->party_member_num = 1;
	body->party_member_idx[0] = idx;

	header->party_member_num = 1;
	header->party_member_race[0] = body->monster_list[idx].race;
	header->party_member_level[0] = body->monster_list[idx].level;
	COPY_NAME(header->party_member_name[0], body->monster_list[idx].name);

	for (i = 0; i < body->standby_num; i++)
	{
		if (body->standby_idx[i] != idx)
			continue;

		for (j = i + 1; j < body->standby_num; j++)
			body->standby_idx[i] = body->standby_idx[j];

		body->standby_num--;
		break;
	}
}

/* 特定モンスター情報を強制変更 */
static VOID modify_monster_info_force(struct monster_fmt *monster, CONST DQMJ_SAVE_MONSTER_INFO *in)
{
	int i;
	CONST struct race_info *ri;
	CONST struct ability_info *ai;

	ri = lookup_race_info(in->race);

	monster->cheat = in->cheat ? 1 : 0;
	monster->race = in->race;
	monster->rank = (in->rank == DQMJ_AUTO_RANK) ? ri->rank : in->rank;
	monster->type = (in->type == DQMJ_AUTO_TYPE) ? ri->type : in->type;
	monster->sex = (in->sex == DQMJ_RAND_SEX) ? gen_monster_sex(monster->type) : in->sex;
	monster->combine = in->combine;
	monster->source = in->source;
	monster->status = in->dead ? (monster->status | STATUS_DEAD) : (monster->status &= ~STATUS_DEAD);
	monster->status = in->poison ? (monster->status | STATUS_POISON) : (monster->status &= ~STATUS_POISON);
	monster->level = in->level;
	monster->base_hp = in->base_hp;
	monster->base_mp = in->base_mp;
	monster->base_atk = in->base_atk;
	monster->base_def = in->base_def;
	monster->base_agi = in->base_agi;
	monster->base_int = in->base_int;
	monster->current_hp = in->current_hp;
	monster->current_mp = in->current_mp;
	monster->actual_hp = in->actual_hp;
	monster->actual_mp = in->actual_mp;
	monster->actual_atk = in->actual_atk;
	monster->actual_def = in->actual_def;
	monster->actual_agi = in->actual_agi;
	monster->actual_int = in->actual_int;
	monster->exp = in->exp;
	monster->strategy = in->strategy;
	monster->growth_type = (in->growth_type == DQMJ_RAND_GROWTH_TYPE) ? gen_growth_type() : in->growth_type;
	monster->ability_mask = 0;
	monster->weapon = in->weapon;
	monster->remaining_sp = in->remaining_sp;
	monster->level_bak = in->level;

	monster->father_race = in->parent_race[DQMJ_PARENT_FATHER];
	monster->mother_race = in->parent_race[DQMJ_PARENT_MOTHER];
	monster->father_father_race = in->grandparent_race[DQMJ_PARENT_FATHER][DQMJ_PARENT_FATHER];
	monster->father_mother_race = in->grandparent_race[DQMJ_PARENT_FATHER][DQMJ_PARENT_MOTHER];
	monster->mother_father_race = in->grandparent_race[DQMJ_PARENT_MOTHER][DQMJ_PARENT_FATHER];
	monster->mother_mother_race = in->grandparent_race[DQMJ_PARENT_MOTHER][DQMJ_PARENT_MOTHER];

	COPY_NAME(monster->name, in->name);
	COPY_NAME(monster->master, in->master);
	COPY_NAME(monster->father_name, in->parent_name[DQMJ_PARENT_FATHER]);
	COPY_NAME(monster->mother_name, in->parent_name[DQMJ_PARENT_MOTHER]);
	COPY_NAME(monster->father_master, in->parent_master[DQMJ_PARENT_FATHER]);
	COPY_NAME(monster->mother_master, in->parent_master[DQMJ_PARENT_MOTHER]);

	CopyMemory(monster->rom_data1, ri->rom_data1, sizeof(monster->rom_data1));
	CopyMemory(monster->rom_data2, ri->rom_data2, sizeof(monster->rom_data2));

	for (i = 0; i < _countof(monster->base_guard); i++)
		monster->base_guard[i] = GET_HALF(ri->base_guard, i);

	if (in->max_level == DQMJ_AUTO_MAX_LEVEL)
	{
		if (monster->combine < 5)
			monster->max_level = 50;
		else if (monster->combine < 10)
			monster->max_level = 75;
		else
			monster->max_level = 100;
	}
	else
	{
		monster->max_level = in->max_level;
	}

	if ((in->growth_start_level == DQMJ_RAND_GROWTH_LEVEL) || (in->growth_end_level == DQMJ_RAND_GROWTH_LEVEL))
	{
		monster->growth_level[0] = gen_growth_start_level();
		monster->growth_level[1] = gen_growth_end_level(monster->growth_level[0]);
	}
	else
	{
		monster->growth_level[0] = in->growth_start_level;
		monster->growth_level[1] = in->growth_end_level;
	}

	for (i = 0; i < DQMJ_SKILLSET_MAX; i++)
	{
		monster->skillset[i] = in->skillset[i];
		monster->assigned_sp[i] = in->assigned_sp[i];

		if (in->skill_active_num[i] == DQMJ_AUTO_SKILL_ACTIVE_NUM)
			monster->skill_active_num[i] = DQMJSaveCalcSkillActiveCount(monster->skillset[i], monster->assigned_sp[i]);
		else
			monster->skill_active_num[i] = in->skill_active_num[i];
	}

	for (i = 0; i < DQMJ_GUARD_MAX; i++)
		monster->actual_guard[i] = in->guard_info[i];
	for (i = 0; i < DQMJ_SKILL_MAX; i++)
		monster->skill_list[i] = in->skill_list[i];

	for (i = 0; i < DQMJ_ABILITY_MAX; i++)
	{
		monster->ability_list[i] = in->ability_list[i];
		ai = lookup_ability_info(in->ability_list[i]);
		if (ai->ability != 0x00)
			monster->ability_mask |= 1 << (ai->ability - 1);
	}

	monster->checksum = calc_monster_checksum(monster);
}

/* 特定モンスター情報を全てクリア */
static VOID clear_monster_info(struct monster_fmt *monster)
{
	ZeroMemory(monster, sizeof(struct monster_fmt));

	*monster->father_name = 0xFF;
	*monster->mother_name = 0xFF;
	*monster->father_father_name = 0xFF;
	*monster->father_mother_name = 0xFF;
	*monster->mother_father_name = 0xFF;
	*monster->mother_mother_name = 0xFF;

	FillMemory(monster->name, sizeof(monster->name), 0xFF);
	FillMemory(monster->master, sizeof(monster->master), 0xFF);
	FillMemory(monster->father_master, sizeof(monster->father_master), 0xFF);
	FillMemory(monster->mother_master, sizeof(monster->mother_master), 0xFF);
	FillMemory(monster->father_father_master, sizeof(monster->father_father_master), 0xFF);
	FillMemory(monster->father_mother_master, sizeof(monster->father_mother_master), 0xFF);
	FillMemory(monster->mother_father_master, sizeof(monster->mother_father_master), 0xFF);
	FillMemory(monster->mother_mother_master, sizeof(monster->mother_mother_master), 0xFF);
}

/* モンスター情報を全て合法値範囲内にノーマライズする */
BOOL normalize_monster_by_handle(HDQMJSAVE handle, int monster_idx, DQMJ_SAVE_MONSTER_INFO *inout)
{
	BOOL in_party_or_standby;
	CONST struct save_fmt_body *body;

	if ((handle == NULL) || (handle == INVALID_HANDLE_VALUE) || (inout == NULL))
		return FALSE;

	body = GET_BODY(handle);

	if (monster_idx >= 0)
	{
		if (monster_idx >= min(body->monster_num, DQMJ_MONSTER_MAX))
			return FALSE;

		in_party_or_standby = check_monster_in_party_or_standby(monster_idx, body);
	}
	else
	{
		in_party_or_standby = FALSE;
	}

	return DQMJSaveNormalizeMonster(IS_INTR(handle), in_party_or_standby, (CONST DQMJ_NAME *)&body->player_name, inout);
}

/* 開いたセーブファイルの読み込み処理 */
static BOOL read_save_file(HANDLE file, struct save_info *sav)
{
	DWORD file_size, data_size, read_size;
	struct save_fmt *fmt;
	struct desmume_info *desmume = &sav->extend_data.desmume_info;
	struct nocashgba_info *nocashgba = &sav->extend_data.nocashgba_info;

	file_size = GetFileSize(file, NULL);
	if (file_size == INVALID_FILE_SIZE)
		return FALSE;

	if (!ReadFile(file, &nocashgba->header, sizeof(nocashgba->header), &read_size, NULL) || (read_size != sizeof(nocashgba->header)))
		return FALSE;

	if (memcmp(nocashgba->header.identifier, NOCASHBGA_IDENTIFIER, sizeof(nocashgba->header.identifier)) == 0)
	{
		sav->format = DQMJ_FORMAT_NOCASHBGA;

		switch (nocashgba->header.compression)
		{
		case NOCASHBGA_UNCOMPRESSED:
			if (!read_uncompressed_data(file, nocashgba->header.data_size, sav))
				return FALSE;
			break;
		case NOCASHBGA_COMPRESSED_RLU:
			sav->raw_data = uncompress_rlu_data(file, nocashgba->header.data_size, &sav->raw_size);
			if (sav->raw_data == NULL)
				return FALSE;
			break;
		case NOCASHBGA_COMPRESSED_LZ:
			sav->raw_data = uncompress_lz_data(file, nocashgba->header.data_size, &sav->raw_size);
			if (sav->raw_data == NULL)
				return FALSE;
			break;
		default:
			return FALSE;
		}

		if (sav->raw_size < SAVE_SIZE_MIN)
			return FALSE;

		nocashgba->footer_size = file_size - sizeof(nocashgba->header) - nocashgba->header.data_size;
		if (nocashgba->footer_size != 0)
		{
			if (SetFilePointer(file, sizeof(nocashgba->header) + nocashgba->header.data_size, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
				return FALSE;

			nocashgba->footer = HeapAlloc(GetProcessHeap(), 0, nocashgba->footer_size);
			if (nocashgba->footer == NULL)
				return FALSE;

			if (!ReadFile(file, nocashgba->footer, nocashgba->footer_size, &read_size, NULL) || (read_size != nocashgba->footer_size))
				return FALSE;
		}

		data_size = sav->raw_size;
	}
	else
	{
		if (SetFilePointer(file, -(LONG)sizeof(desmume->footer), NULL, FILE_END) == INVALID_SET_FILE_POINTER)
			return FALSE;

		if (!ReadFile(file, &desmume->footer, sizeof(desmume->footer), &read_size, NULL) || (read_size != sizeof(desmume->footer)))
			return FALSE;

		if (memcmp(desmume->footer.identifier, DESMUME_IDENTIFIER, sizeof(desmume->footer.identifier)) == 0)
		{
			sav->format = DQMJ_FORMAT_DESMUME;
			data_size = desmume->footer.padded_size;

			desmume->dummy_size = file_size - data_size - sizeof(desmume->footer);
			if (desmume->dummy_size != 0)
			{
				desmume->dummy = HeapAlloc(GetProcessHeap(), 0, desmume->dummy_size);
				if (desmume->dummy == NULL)
					return FALSE;

				if (SetFilePointer(file, data_size, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
					return FALSE;

				if (!ReadFile(file, desmume->dummy, desmume->dummy_size, &read_size, NULL) || (read_size != desmume->dummy_size))
					return FALSE;
			}
		}
		else
		{
			sav->format = DQMJ_FORMAT_RAW;
			data_size = file_size;
		}

		if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			return FALSE;

		if (!read_uncompressed_data(file, data_size, sav))
			return FALSE;
	}

	fmt = (struct save_fmt *)(sav->raw_data + INTERRUPT_OFFSET);

	if (fmt->header.identifier == SAVE_IDENTIFIER)
	{
		if (data_size < INTERRUPT_OFFSET + sizeof(struct save_fmt))
			return FALSE;

		sav->interrupt_flag = TRUE;
	}
	else
	{
		fmt = (struct save_fmt *)sav->raw_data;
		if (fmt->header.identifier != SAVE_IDENTIFIER)
			return FALSE;

		sav->interrupt_flag = FALSE;
	}

	return TRUE;
}

/* 新規セーブファイルの書き込み処理 */
static BOOL write_save_file(HANDLE file, struct save_info *sav, BOOL as_raw)
{
	DWORD wrt_size;
	struct save_fmt *fmt;
	struct desmume_info *desmume = &sav->extend_data.desmume_info;
	struct nocashgba_info *nocashgba = &sav->extend_data.nocashgba_info;

	if (sav->raw_size < SAVE_SIZE_MIN)
		return FALSE;

	fmt = (struct save_fmt *)(sav->interrupt_flag ? (sav->raw_data + INTERRUPT_OFFSET) : sav->raw_data);
	fmt->header.checksum = calc_save_checksum(fmt);

	if (!as_raw && (sav->format == DQMJ_FORMAT_NOCASHBGA))
	{
		if (!WriteFile(file, &nocashgba->header, sizeof(nocashgba->header), &wrt_size, NULL) || (wrt_size != sizeof(nocashgba->header)))
			return FALSE;

		switch (nocashgba->header.compression)
		{
		case NOCASHBGA_COMPRESSED_RLU:
			nocashgba->header.data_size = compress_rlu_data(file, sav->raw_data, sav->raw_size);
			if (nocashgba->header.data_size == 0)
				return FALSE;
			break;
		case NOCASHBGA_COMPRESSED_LZ:
			nocashgba->header.data_size = compress_lz_data(file, sav->raw_data, sav->raw_size);
			if (nocashgba->header.data_size == 0)
				return FALSE;
			break;
		default:
			nocashgba->header.data_size = sav->raw_size;
			if (!WriteFile(file, sav->raw_data, sav->raw_size, &wrt_size, NULL) || (wrt_size != sav->raw_size))
				return FALSE;
			break;
		}

		if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			return FALSE;

		if (!WriteFile(file, &nocashgba->header, sizeof(nocashgba->header), &wrt_size, NULL) || (wrt_size != sizeof(nocashgba->header)))
			return FALSE;

		if (nocashgba->footer_size != 0)
		{
			if (SetFilePointer(file, nocashgba->header.data_size, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
				return FALSE;

			if (!WriteFile(file, nocashgba->footer, nocashgba->footer_size, &wrt_size, NULL) || (wrt_size != nocashgba->footer_size))
				return FALSE;
		}

		return TRUE;
	}

	if (!WriteFile(file, sav->raw_data, sav->raw_size, &wrt_size, NULL) || (wrt_size != sav->raw_size))
		return FALSE;

	if (!as_raw && (sav->format == DQMJ_FORMAT_DESMUME))
	{
		if (desmume->dummy_size != 0)
		{
			if (!WriteFile(file, desmume->dummy, desmume->dummy_size, &wrt_size, NULL) || (wrt_size != desmume->dummy_size))
				return FALSE;
		}

		if (!WriteFile(file, &desmume->footer, sizeof(desmume->footer), &wrt_size, NULL) || (wrt_size != sizeof(desmume->footer)))
			return FALSE;
	}

	return TRUE;
}

/* 未圧縮データを直接読み込み */
static BOOL read_uncompressed_data(HANDLE file, UINT size, struct save_info *sav)
{
	DWORD read_size;

	sav->raw_size = size;
	sav->raw_data = HeapAlloc(GetProcessHeap(), 0, size);
	if (sav->raw_data == NULL)
		return FALSE;

	if (!ReadFile(file, sav->raw_data, size, &read_size, NULL) || (read_size != size))
		return FALSE;

	return TRUE;
}

/************************************************************************/
