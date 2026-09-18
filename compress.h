
#pragma once

#include <windows.h>

/************************************************************************/

/* No$GBAのRLU圧縮データを解凍する */
EXTERN_C LPBYTE uncompress_rlu_data(HANDLE file, UINT data_size, LPUINT raw_size);

/* 生データをNo$GBAのRLUで圧縮する */
EXTERN_C UINT compress_rlu_data(HANDLE file, LPCBYTE raw_data, UINT raw_size);

/* No$GBAのLZ圧縮データを解凍する */
EXTERN_C LPBYTE uncompress_lz_data(HANDLE file, UINT data_size, LPUINT raw_size);

/************************************************************************/
