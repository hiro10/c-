//=============================================================================
//
// 影処理 [shadow.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	SHADOW_SIZE_X		(50.0f)							// 弾の幅
#define	SHADOW_SIZE_Z		(50.0f)							// 弾の高さ

#define	MAX_SHADOW			(1280)							// 影最大数

//*****************************************************************************
// 構造体(クラス)定義
//*****************************************************************************
class  SHADOW
{
public:
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	bool use;				// 使用しているかどうか
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, float SizeX, float SizeY, float SizeZ);
void DeleteShadow(int IdxShadow);
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos);
void SetVertexShadow(int IdxShadow,int s, float SizeX,float SizeY, float SizeZ);
void SetColorShadow(int IdxShadow, D3DXCOLOR col);

#endif
