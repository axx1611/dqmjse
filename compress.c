
#include <limits.h>
#include "compress.h"

/************************************************************************/

/* ビットストリームアクセス用 */
struct bit_stream
{
	HANDLE	file;				/* ビットストリームを読み込むファイルハンドル */
	UINT	stream_size;		/* ビットストリームのデータサイズ */
	UINT	stream_offset;		/* ビットストリーム内の読み込み位置（バイト） */
	UINT	bit_count;			/* 現在のワードに残っているビット数 */
	UINT32	word;				/* 現在読み込んでいるビットワード */
};

#pragma pack(1)

/* No$GBAのLZ圧縮データヘッダ */
struct lz_header
{
	BYTE	type;				/* 0000: データ種別（現在は未使用） */
	UINT32	bit_size;			/* 0001: ビットストリームのデータサイズ */
	UINT32	raw_size;			/* 0005: 解凍後のデータサイズ */
	UINT16	dist_size;			/* 0009: 距離テーブルのデータサイズ */
};

#pragma pack()

/************************************************************************/

/* No$GBAのRLU圧縮/解凍用関数 */
static BOOL rlu_read(HANDLE file, UINT data_size, UINT raw_size, LPBYTE raw_data);
static UINT rlu_write(HANDLE file, UINT raw_size, LPCBYTE raw_data);
static UINT rlu_copy(HANDLE file, UINT len, LPCBYTE data);
static UINT rlu_repeat(HANDLE file, UINT len, BYTE data);

/* No$GBAのLZ圧縮/解凍用関数 */
static BOOL lz_uncompress(HANDLE file, DWORD bit_off, UINT bit_size, LPBYTE dist_tab, UINT dist_size, LPBYTE len_tab, UINT len_size, LPBYTE raw_data, UINT raw_size);
static BOOL lz_read_bits(struct bit_stream *stream, UINT count, LPUINT value);
static BOOL lz_read_symbol(struct bit_stream *stream, LPCBYTE table, UINT table_size, LPUINT value);

/************************************************************************/

/* No$GBAのRLU圧縮データを解凍する */
LPBYTE uncompress_rlu_data(HANDLE file, UINT data_size, LPUINT raw_size)
{
	UINT32 size;
	DWORD read_size;
	LPBYTE raw_data;

	if ((data_size < sizeof(size)) || (raw_size == NULL))
		return NULL;

	if (!ReadFile(file, &size, sizeof(size), &read_size, NULL) || (read_size != sizeof(size)))
		return NULL;

	data_size -= sizeof(size);

	raw_data = HeapAlloc(GetProcessHeap(), 0, size);
	if (raw_data == NULL)
		return NULL;

	if (!rlu_read(file, data_size, size, raw_data))
	{
		HeapFree(GetProcessHeap(), 0, raw_data);
		return NULL;
	}

	*raw_size = size;
	return raw_data;
}

/* 生データをNo$GBAのRLUで圧縮する */
UINT compress_rlu_data(HANDLE file, LPCBYTE raw_data, UINT raw_size)
{
	UINT32 size;
	UINT data_size;
	DWORD wrt_size;

	if ((raw_data == NULL) || (raw_size == 0))
		return 0;

	size = raw_size;
	if (!WriteFile(file, &size, sizeof(size), &wrt_size, NULL) || (wrt_size != sizeof(size)))
		return 0;

	data_size = rlu_write(file, size, raw_data);
	if (data_size == 0)
		return 0;

	return sizeof(size) + data_size;
}

/* No$GBAのLZ圧縮データを解凍する */
LPBYTE uncompress_lz_data(HANDLE file, UINT data_size, LPUINT raw_size)
{
	BOOL ok;
	UINT len_size;
	DWORD offset, read_size;
	LPBYTE dist_tab, len_tab, raw_data;
	struct lz_header header;

	if ((data_size < sizeof(header)) || (raw_size == NULL))
		return NULL;

	offset = SetFilePointer(file, 0, NULL, FILE_CURRENT);
	if (offset == INVALID_SET_FILE_POINTER)
		return NULL;

	if (!ReadFile(file, &header, sizeof(header), &read_size, NULL) || (read_size != sizeof(header)))
		return NULL;

	if ((header.raw_size == 0) || (header.dist_size == 0))
		return NULL;

	if ((header.bit_size > data_size - sizeof(header)) || (header.dist_size > data_size - sizeof(header) - header.bit_size))
		return NULL;

	len_size = data_size - sizeof(header) - header.bit_size - header.dist_size;
	if (len_size == 0)
		return NULL;

	if (SetFilePointer(file, header.bit_size, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
		return NULL;

	dist_tab = HeapAlloc(GetProcessHeap(), 0, header.dist_size);
	len_tab = HeapAlloc(GetProcessHeap(), 0, len_size);
	raw_data = HeapAlloc(GetProcessHeap(), 0, header.raw_size);

	ok = lz_uncompress(file, offset + sizeof(header), header.bit_size, dist_tab, header.dist_size, len_tab, len_size, raw_data, header.raw_size);

	HeapFree(GetProcessHeap(), 0, dist_tab);
	HeapFree(GetProcessHeap(), 0, len_tab);

	if (!ok || SetFilePointer(file, offset + data_size, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		HeapFree(GetProcessHeap(), 0, raw_data);
		return NULL;
	}

	*raw_size = header.raw_size;
	return raw_data;
}

/************************************************************************/

/* No$GBAのRLU圧縮データを解凍しながら読み込む */
static BOOL rlu_read(HANDLE file, UINT data_size, UINT raw_size, LPBYTE raw_data)
{
	UINT offset;
	UINT8 byte;
	UINT16 word, len;
	DWORD read_size;

	offset = 0;

	while (offset < raw_size)
	{
		if (data_size < sizeof(word))
			return FALSE;

		if (!ReadFile(file, &word, sizeof(word), &read_size, NULL) || (read_size != sizeof(word)))
			return FALSE;

		data_size -= sizeof(word);

		if (LOBYTE(word) > 0x80)
		{
			len = LOBYTE(word) - 0x80;
			if (len > raw_size - offset)
				return FALSE;

			FillMemory(raw_data + offset, len, HIBYTE(word));
			offset += len;
			continue;
		}

		if (LOBYTE(word) < 0x80)
		{
			len = LOBYTE(word);
			if (len == 0)
				return FALSE;

			if (len > raw_size - offset)
				return FALSE;

			raw_data[offset++] = HIBYTE(word);
			len--;
			if (len == 0)
				continue;

			if (data_size < len)
				return FALSE;

			if (!ReadFile(file, raw_data + offset, len, &read_size, NULL) || (read_size != len))
				return FALSE;

			data_size -= len;
			offset += len;
			continue;
		}

		if (data_size < sizeof(len))
			return FALSE;

		if (!ReadFile(file, &len, sizeof(len), &read_size, NULL) || (read_size != sizeof(len)))
			return FALSE;

		data_size -= sizeof(len);

		if (len > raw_size - offset)
			return FALSE;

		FillMemory(raw_data + offset, len, HIBYTE(word));
		offset += len;
	}

	if (data_size != sizeof(byte))
		return FALSE;

	if (!ReadFile(file, &byte, sizeof(byte), &read_size, NULL) || (read_size != sizeof(byte)))
		return FALSE;

	if (byte != 0)
		return FALSE;

	return TRUE;
}

/* 生データをNo$GBAのRLU圧縮して書き込む */
static UINT rlu_write(HANDLE file, UINT raw_size, LPCBYTE raw_data)
{
	UINT offset, data_size, copy_len, repeat_len;
	UINT8 byte;
	DWORD wrt_size;

	data_size = 0;
	byte = raw_data[0];
	offset = 1;
	copy_len = 1;
	repeat_len = 1;

	do
	{
		if (raw_data[offset] == byte)
		{
			offset++;
			copy_len++;
			repeat_len++;

			if (repeat_len > 2)
			{
				if (copy_len > repeat_len)
				{
					wrt_size = rlu_copy(file, copy_len - repeat_len, raw_data + offset - copy_len);
					if (wrt_size == 0)
						return 0;

					data_size += wrt_size;
				}

				copy_len = 0;
			}
		}
		else
		{
			if (repeat_len > 2)
			{
				wrt_size = rlu_repeat(file, repeat_len, byte);
				if (wrt_size == 0)
					return 0;

				data_size += wrt_size;
			}

			byte = raw_data[offset++];
			copy_len++;
			repeat_len = 1;
		}
	}
	while (offset < raw_size);

	if (repeat_len > 2)
	{
		wrt_size = rlu_repeat(file, repeat_len, byte);
		if (wrt_size == 0)
			return 0;
	}
	else
	{
		wrt_size = rlu_copy(file, copy_len, raw_data + offset - copy_len);
		if (wrt_size == 0)
			return 0;
	}

	data_size += wrt_size;
	byte = 0;

	if (!WriteFile(file, &byte, sizeof(byte), &wrt_size, NULL) || (wrt_size != sizeof(byte)))
		return 0;

	return data_size + sizeof(byte);
}

/* No$GBAのRLU圧縮コピーデータ処理 */
static UINT rlu_copy(HANDLE file, UINT len, LPCBYTE data)
{
	UINT data_size;
	UINT8 byte;
	DWORD wrt_size;

	data_size = 0;

	while (len != 0)
	{
		byte = (len < 0x80) ? len : 0x7F;
		len -= byte;

		if (!WriteFile(file, &byte, sizeof(byte), &wrt_size, NULL) || (wrt_size != sizeof(byte)))
			return 0;
		if (!WriteFile(file, data, byte, &wrt_size, NULL) || (wrt_size != byte))
			return 0;

		data += byte;
		data_size += sizeof(byte) + byte;
	}

	return data_size;
}

/* No$GBAのRLU圧縮重複データ処理 */
static UINT rlu_repeat(HANDLE file, UINT len, BYTE data)
{
	UINT data_size;
	UINT8 byte;
	UINT16 word;
	DWORD wrt_size;

	data_size = 0;

	while (len != 0)
	{
		if (len < 0x80)
		{
			byte = len | 0x80;

			if (!WriteFile(file, &byte, sizeof(byte), &wrt_size, NULL) || (wrt_size != sizeof(byte)))
				return 0;
			if (!WriteFile(file, &data, sizeof(data), &wrt_size, NULL) || (wrt_size != sizeof(data)))
				return 0;

			data_size += sizeof(byte) + sizeof(data);
			break;
		}

		byte = 0x80;
		word = (len < 0x10000) ? len : 0xFFFF;
		len -= word;

		if (!WriteFile(file, &byte, sizeof(byte), &wrt_size, NULL) || (wrt_size != sizeof(byte)))
			return 0;
		if (!WriteFile(file, &data, sizeof(data), &wrt_size, NULL) || (wrt_size != sizeof(data)))
			return 0;
		if (!WriteFile(file, &word, sizeof(word), &wrt_size, NULL) || (wrt_size != sizeof(word)))
			return 0;

		data_size += sizeof(byte) + sizeof(data) + sizeof(word);
	}

	return data_size;
}

/************************************************************************/

/* No$GBAのLZ圧縮データを解凍しながら読み込む */
static BOOL lz_uncompress(HANDLE file, DWORD bit_off, UINT bit_size, LPBYTE dist_tab, UINT dist_size, LPBYTE len_tab, UINT len_size, LPBYTE raw_data, UINT raw_size)
{
	UINT offset, flag, dist, len, i;
	DWORD read_size;
	struct bit_stream stream;

	if ((dist_tab == NULL) || (len_tab == NULL) || (raw_data == NULL))
		return FALSE;

	if (!ReadFile(file, dist_tab, dist_size, &read_size, NULL) || (read_size != dist_size))
		return FALSE;

	if (!ReadFile(file, len_tab, len_size, &read_size, NULL) || (read_size != len_size))
		return FALSE;

	if (SetFilePointer(file, bit_off, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return FALSE;

	stream.file = file;
	stream.stream_size = bit_size;
	stream.stream_offset = 0;
	stream.bit_count = 0;
	stream.word = 0;

	offset = 0;

	while (offset < raw_size)
	{
		if (!lz_read_bits(&stream, 1, &flag))
			return FALSE;

		if (flag == 0)
		{
			if (stream.stream_offset >= stream.stream_size)
				return FALSE;

			if (!ReadFile(file, raw_data + offset, sizeof(*raw_data), &read_size, NULL) || (read_size != sizeof(*raw_data)))
				return FALSE;

			stream.stream_offset++;
			offset++;
			continue;
		}

		if (!lz_read_symbol(&stream, dist_tab, dist_size, &dist))
			return FALSE;

		if (!lz_read_symbol(&stream, len_tab, len_size, &len))
			return FALSE;

		if ((dist == 0) || (dist > offset) || (len > raw_size - offset))
			return FALSE;

		for (i = 0; i < len; i++)
		{
			raw_data[offset] = raw_data[offset - dist];
			offset++;
		}
	}

	return TRUE;
}

/* No$GBAのLZ解凍用ビットストリームを読み込む */
static BOOL lz_read_bits(struct bit_stream *stream, UINT count, LPUINT value)
{
	DWORD read_size;

	if (count > 16)
		return FALSE;

	*value = 0;

	if (count == 0)
		return TRUE;

	if (stream->bit_count < count)
	{
		if (stream->bit_count != 0)
		{
			*value = stream->word >> (32 - stream->bit_count);
			count -= stream->bit_count;

			stream->word = 0;
			stream->bit_count = 0;
		}

		if ((stream->stream_offset >= stream->stream_size) || (stream->stream_size - stream->stream_offset < sizeof(UINT32)))
			return FALSE;

		if (!ReadFile(stream->file, &stream->word, sizeof(UINT32), &read_size, NULL) || (read_size != sizeof(UINT32)))
			return FALSE;

		stream->stream_offset += sizeof(UINT32);
		stream->bit_count = 32;
	}

	*value = (*value << count) | (stream->word >> (32 - count));

	stream->word <<= count;
	stream->bit_count -= count;

	return TRUE;
}

/* No$GBAのLZ範囲テーブルから値を1つ読み込む */
static BOOL lz_read_symbol(struct bit_stream *stream, LPCBYTE table, UINT table_size, LPUINT value)
{
	UINT offset, entry, count, extra, bit;

	offset = 0;

	while (offset < table_size)
	{
		entry = table[offset++];
		count = entry >> 1;

		if (entry & 1)
		{
			if ((count > 16) || (table_size - offset < sizeof(UINT)))
				return FALSE;

			*value = *(CONST UINT *)(table + offset);
			if (!lz_read_bits(stream, count, &extra) || (extra > UINT_MAX - *value))
				return FALSE;

			*value += extra;
			return TRUE;
		}

		if (!lz_read_bits(stream, 1, &bit))
			return FALSE;

		if (bit != 0)
		{
			if (count > table_size - offset)
				return FALSE;

			offset += count;
		}
	}

	return FALSE;
}

/************************************************************************/
