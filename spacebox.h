//=============================================================================
//
// skyboxの処理 [space.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _SPACE_H_
#define _SPACE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SPACE		(6)			
#define	SPACE_TEXTURE_FILENAME		"data/TEXTURE/space-3d-screenshot.png"	// 読み込むテクスチャファイル名
#define	SPACE_NUM_VERTEX			(4)
#define	SPACE_NUM_INDEX				(4)
#define	SPACE_NUM_POLYGIN			(2)
#define	SPACE_SIZE					(VIEW_FAR_Z/2.8f)						// skyboxの大きさ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSpace();
void UninitSpace(void);
void UpdateSpace(void);
void DrawSpace(void);

#endif
