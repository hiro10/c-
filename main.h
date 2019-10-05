//=============================================================================
//
// メイン処理 [main.h]
// Author : 西村　任弘
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_


#define _CRT_SECURE_NO_WARNINGS			// scanf のwarning防止

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include <time.h>
#include <tchar.h>
#include <stdio.h>
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerSoundXAudio2.h>

#include "d3dx9.h"


#define DIRECTINPUT_VERSION (0x0800)	// 警告対策
#include "dinput.h"
#include "xaudio2.h"		// XAudio2関連のヘッダー

#if _DEBUG

#endif



//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(1280)				// ウインドウの幅
#define SCREEN_HEIGHT	(960)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数

#define SAFE_RELEASE(ptr)		{ if(ptr) { (ptr)->Release(); (ptr) = NULL; } }


// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
class VERTEX_2D
{
public:
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
};

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
class VERTEX_3D
{
public:
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
};

typedef struct
{
	VERTEX_2D				s_vertexWk[NUM_VERTEX];			// 頂点情報格納ワーク
	D3DXVECTOR3				s_pos;							// ポリゴンの座標
	D3DXVECTOR3				s_rot;							// ポリゴンの回転量
	int						s_CountAnim;					// アニメーションカウント
	int						s_PatternAnim;					// アニメーションパターンナンバー
	D3DXVECTOR3				s_size;							// 見た目の大きさ
	float					s_Radius;						// ポリゴンの半径
	float					s_Angle;						// ポリゴンの角度


	LPDIRECT3DTEXTURE9		s_texture;						// テクスチャ情報

}PROJECT2D;

//*************************************
// モードの種類
//*************************************
typedef enum
{
	MODE_TITLE = 0,			// タイトル画面
	MODE_GAME,			// ゲーム画面
	MODE_BOSS,
	MODE_RESULT,			// リザルト画面
	MODE_MAX
} MODE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif