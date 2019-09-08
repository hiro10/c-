//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGame(void);
HRESULT InitBossGame(void);
void UninitGame(void);
void UninitBossGame(void);
void UpdateGame(void);
void UpdateBossGame(void);
void DrawGame(void);
void DrawBossGame(void);

#endif
