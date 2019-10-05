//=============================================================================
//
// レーダー処理 [radar.h]
// Author :西村任弘 
//
//=============================================================================
#ifndef _RADAR_H_
#define _RADAR_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_RADAR_NORMAL		"data/TEXTURE/radar.png"		// 読み込むテクスチャファイル名
#define	TEXTURE_POINT_NORMAL		"data/TEXTURE/bullet.png"		// 読み込むテクスチャファイル名

#define TIME_ANIMATION_RADAR		(4)
#define TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X	(1)
#define TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y	(1)
#define ANIM_PATTERN_RADAR						(TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X*TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y)
#define	RADAR_POS_X								(140.0f)						// レーダーの表示位置
#define	RADAR_POS_Y								(130.0f)						// レーダーの表示位置
#define	RADAR_SIZE_X							(80)													// レーダーの幅
#define	RADAR_SIZE_Y							(80)													// レーダーの高さ
#define	POINT_SIZE_X							(2)														// 敵表示の大きさ
#define	POINT_SIZE_Y							(2)														// 敵表示の大きさ
#define	POINT_SIZE_UP							(9)														// 敵表示の大きさ
#define	POINT_SIZE_MISSILE						(3)														// 敵表示の大きさ
#define	POINT_SIZE_MIDDLE						(6)														// 敵表示の大きさ
#define	POINT_SIZE_DOWN							(3)														// 敵表示の大きさ
#define	POINT_HEIGHT							(30.0f)													// 敵表示の範囲
#define	RADAR_DISTANCE							(3000.0f)												// 敵表示の最大距離
#define RADAR_MAX								(1)														// レーダーの数
#define POINT_MAX								(1024)													// 表示できる最大値


//*****************************************************************************
// クラス定義
//*****************************************************************************

// レーダー
class Rader
{
public:
	PROJECT2D	s_polygon;
	bool		s_bUse;
};

// マーカー
class Maker
{
public:
	PROJECT2D	s_polygon;
	bool		s_bUse;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRadar(void);
void UninitRadar(void);
void UpdateRadar(void);
Rader* GetRadar(int Ano);
Maker* GetMaker(int Ano);
void DrawRadar();

#endif
#pragma once
