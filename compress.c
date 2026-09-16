#include "compress.h"

/************************************************************************/

/* No$GBAのRLU圧縮データを解凍しながら読み込む */
static BOOL rlu_read(HANDLE file, UINT data_size, UINT raw_size, LPBYTE raw_data);

/* 生データをNo$GBAのRLU圧縮して書き込む */
static UINT rlu_write(HANDLE file, UINT raw_size, LPCBYTE raw_data);

/* No$GBAのRLU圧縮コピーデータ処理 */
static UINT rlu_copy(HANDLE file, UINT len, LPCBYTE data);

/* No$GBAのRLU圧縮重複データ処理 */
static UINT rlu_repeat(HANDLE file, UINT len, BYTE data);

/************************************************************************/

/* No$GBAのRLU圧縮デートを解凍する */
LPBYTE uncompress_rlu_data(HANDLE file, UINT data_size, LPUINT raw_size)
{
	UINT32 size;
	DWORD read_size;
	LPBYTE raw_data;

	if ((data_size < sizeof(size)) || (raw_size == NULL))
		return NULL;

	if (!ReadFile(file, &size, sizeof(size), &read_size, NULL) || (read_size != sizeof(size)))
		return NULL;

	*raw_size = size;
	data_size -= sizeof(size);

	raw_data = HeapAlloc(GetProcessHeap(), 0, size);
	if (raw_data == NULL)
		return NULL;

	if (rlu_read(file, data_size, size, raw_data))
		return raw_data;

	HeapFree(GetProcessHeap(), 0, raw_data);
	return NULL;
}

/* 生デートをNo$GBAのRLUで圧縮する */
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

/* No$GBAのLZ圧縮デートを解凍する */
LPBYTE uncompress_lz_data(HANDLE file, UINT data_size, LPUINT raw_size)
{
	// TODO:
	return NULL;
}

/* 生デートをNo$GBAのLZで圧縮する */
UINT compress_lz_data(HANDLE file, LPCBYTE raw_data, UINT raw_size)
{
	// TODO:
	return 0;
}

/************************************************************************/

/* No$GBAのRLU圧縮データを解凍しながら読み込む */
static BOOL rlu_read(HANDLE file, UINT data_size, UINT raw_size, LPBYTE raw_data)
{
	UINT offset;
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
			if (offset + len > raw_size)
				len = raw_size - offset;

			FillMemory(raw_data + offset, len, HIBYTE(word));
			offset += len;
			continue;
		}

		if (LOBYTE(word) < 0x80)
		{
			len = LOBYTE(word);
			if (len == 0)
				return FALSE;

			if (offset + len > raw_size)
				len = raw_size - offset;

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

		if (offset + len > raw_size)
			len = raw_size - offset;

		FillMemory(raw_data + offset, len, HIBYTE(word));
		offset += len;
	}

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
