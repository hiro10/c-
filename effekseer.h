//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Kurosawa Yuta
//
//=============================================================================
#ifndef _EFFEKSEER_H_
#define _EFFEKSEER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_EFFEKSEER				(3000)							// エフェクト最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	int			ID;
	int			EffectType;
	bool		Use;		// 使用しているかどうか
} EFFEKSEER;

typedef struct
{
	EffekseerRendererDX9::Renderer		*Render;
	Effekseer::Manager					*Manager;
	Effekseer::Effect					**Effect;
	int									EffectNum;
}EFFECTCONTROLLER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEffekseer(int type);
void UninitEffekseer(void);
void UpdateEffekseer(void);
void DrawEffekseer(void);
void SetEffekseer(D3DXVECTOR3 pos, int effectType);
//EFFECT *GetEffekseer(int efno);
int SetEffekseer(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);
HRESULT SafeLoadEffekseer(Effekseer::Manager *Manager, Effekseer::Effect **Effect, const EFK_CHAR* SrcPath, const char* ErrorSrc);

#endif
