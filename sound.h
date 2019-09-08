//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,			// BGM0
	SOUND_LABEL_BGM001,				// BGM1
	SOUND_LABEL_SE_SHOT,			// 弾発射音
	SOUND_LABEL_SE_EXPLOSION,		// 爆発音1
	SOUND_LABEL_SE_EXPLOSION2,		// 爆発音2
	SOUND_LABEL_SE_ENTER,			// エンターオン
	SOUND_LABEL_SE_FALSE,			// ミッション失敗音
	SOUND_LABEL_SE_TRUE,			// ミッション成功音
	SOUND_LABEL_SE_ROCK,			// ロック音
	SOUND_LABEL_SE_CANCEL,			// キャンセル音
	SOUND_LABEL_SE_ENEMYEXPLOSION,	// エネミーの爆発音

	SOUND_LABEL_MAX,
} SOUND_LABEL;

enum
{	// 再生用フラグ
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
