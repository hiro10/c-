//=============================================================================
//
// 中央のロックオンの処理 [center_rock.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _CENTERROCK_H_
#define _CENTERROCK_H_

//******************************************************************************
// マクロ定義
//******************************************************************************
#define	NUM_CENTERROCK				(2)			// ポリゴン数

#define TEXTURE_GAME_CENTERROCK		("data/TEXTURE/ロックオン」.png")	// 画像
#define TEXTURE_CENTERROCK_SIZE_X	(100)		// テクスチャサイズ
#define TEXTURE_CENTERROCK_SIZE_Y	(100/2)		// 同上

#define TEXTURE_PATTERN_DIVIDE_X_CENTERROCK	(1)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y_CENTERROCK	(1)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM_CENTERROCK			(TEXTURE_PATTERN_DIVIDE_X_CENTERROCK*TEXTURE_PATTERN_DIVIDE_Y_CENTERROCK)	// アニメーションパターン数
#define TIME_ANIMATION_CENTERROCK			(1)	// アニメーションの切り替わるカウント

#define CENTERROCK_MAX						(1) // 最大数

//*****************************************************************************
// 構造体(クラス)定義
//*****************************************************************************

class CenterLock
{
public:
	bool			use;						// true:使用  false:未使用
	D3DXVECTOR3		pos;						// ポリゴンの移動量
	D3DXVECTOR3		rot;						// ポリゴンの回転量
	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント
	
	LPDIRECT3DTEXTURE9	Texture;				// テクスチャ情報
	VERTEX_2D		vertexWk[NUM_VERTEX];		// 頂点情報格納ワーク

	float			Radius;						// カーソルの半径
	float			BaseAngle;					// カーソルの角度

} ;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCenterLock(void);
void UninitCenterLock(void);
void UpdateCenterLock(void);
void DrawCenterLock(void);
bool *GetUseCenterLock(void);
CenterLock *GetCenterLock(int no);


#endif
