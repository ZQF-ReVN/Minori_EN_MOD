#pragma once
#include <cstdint>

struct MemStream
{
	uint32_t uiType;
	uint32_t uiSize;
	uint32_t uiBuffer;
	uint32_t uiUn0_0;
};

struct StdString
{
	uint8_t aStr[16];
	uint32_t uiCharCount;
	uint32_t uiBufferSize;
	uint32_t uiUn0_0;
};

struct Pack_Entry
{
	uint32_t uiUn0_0;
	uint32_t* pPrevEntry;
	uint32_t* pNextEntry;
	uint32_t uiUn1_0;
	uint8_t* pcResName;
	uint32_t uiUn2_0;
	uint32_t uiUn2_1;
	uint32_t uiUn2_2;
	uint32_t uiCharCount;
	uint32_t uiFlag;
	uint32_t uiUn3_0;
};

struct Res_Entry
{
	uint32_t* pUnEntry;
	uint32_t* pPrevEntry;
	uint32_t* pNextEntry;
	uint32_t uiReUn0_0;
	StdString msRes;
	uint32_t uiReUn1_0;
	uint32_t uiReUn1_2;
	uint32_t uiResSize;
	uint32_t uiResOffset;
	uint32_t uiReUn2_0;
	uint32_t uiReUn2_1;
	uint32_t uiResSize_Back;
	uint32_t uiResSize_Align;
	uint32_t uiReUn3_0;
	uint32_t uiReUn3_1;
	uint32_t uiReUn3_2;
};

struct VTable
{
	uint32_t fn_Un0_0;
	uint32_t fn_OpenPack;
	uint32_t fn_apr_shm_remove;
	uint32_t fn_Un1_0;
	uint32_t fn_LoadResToMem;
	uint32_t fn_Un2_0;
	uint32_t fn_GetPngData;
	uint32_t fn_SetPackFilePointer;
	uint32_t fn_Un3_0;
	uint32_t fn_Un3_1;
	uint32_t fn_Un3_2;
	uint32_t fn_Un3_3;
	uint32_t fn_Un3_4;
};

struct Pack_Index
{
	uint32_t uiUn0_0;
	Res_Entry* pResEntry;
	uint32_t uiEntryCount;
	uint32_t uiUn1_0;
	uint32_t uiUn1_1;
	uint32_t uihFile;      //似乎没啥用
};

struct Pack_Read
{
	VTable* pVtable;
	uint32_t aUn1[0x27];
	StdString msUn0[2];
	uint32_t aUn0[0x8];
	Pack_Index iPackIndex;
	uint32_t aUn2[0x9];
	uint32_t uiReUn4;
	uint32_t uiUn2_1;           // 指向某个buffer
	uint32_t uiUn2_2;
	uint32_t uiUn2_3;
	uint32_t uiUn2_4;
	uint32_t uiUn2_5;
	uint32_t aUn3[0x9];
	uint32_t uiResSize_0;
	uint32_t uiUn4_0;
	uint32_t uiResOffset_0;       //
	uint32_t uiReUn5;
	uint32_t uiResOffset_1;  //fn_SetPackFilePointer 中把uiResOffset拷贝过来
	uint32_t uihFile;           // 当前封包对象的文件句柄
	uint32_t uiUn5_0;
	uint32_t uiUn5_1;
	uint32_t uiReUn6;
	uint32_t uiResSize_1;
	StdString msPackName;
	StdString msResName;
	uint32_t uiUn6_0;
	uint32_t uiUn6_1;
	uint32_t uiUn6_2;            // 指向一个buffer
	uint32_t uiUn6_3;
	MemStream* pMemStream;
	uint32_t uiResBuffer;
	uint32_t uiResSize_2;
	uint32_t pBlowFishKey_0;
	uint32_t pBlowFishKey_1;
	uint32_t uiUn8_0;
	uint32_t uiUn8_1;
	StdString msGameName;
	uint32_t uiUn9_0;
	uint32_t uiUn9_1;
	uint32_t uiUn9_2;
	uint32_t uiUn9_3;
	uint32_t uiUn9_4;
	uint32_t uiUn9_5;
};

typedef char (__thiscall* pMinoriLoadRes)(Pack_Read* pPackRead, uint8_t* pResName);
static pMinoriLoadRes sg_fn_MinoriLoadRes = (pMinoriLoadRes)0x00522AC0;

typedef void* (__thiscall* pMinoriMemAlloc)(uint32_t nSize, MemStream* pMemStream, uint32_t nType);
static pMinoriMemAlloc sg_fn_MinoriMemAlloc = (pMinoriMemAlloc)0x004398B0;