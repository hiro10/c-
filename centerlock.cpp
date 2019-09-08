//=============================================================================
//
// 中央のカーソル処理 [CenterLock.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "centerlock.h"
#include "input.h"
#include "ace.h"
#include "boss.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexCenterLock( int no );
void SetTextureCenterLock( int no, int cntPattern );
void SetVertexCenterLock( int no );

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureCenterLock = NULL;		// テクスチャへのポリゴン

CenterLock				CenterLockWk[CENTERROCK_MAX];		//構造体

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitCenterLock(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CenterLock *centerLock = CenterLockWk;			//　中央のカーソルのポインターを初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,					// デバイスのポインタ
		TEXTURE_GAME_CENTERROCK,						// ファイルの名前
		&D3DTextureCenterLock);						// 読み込むメモリのポインタ

	// 中央のカーソルの初期化処理
	for (int i = 0; i < CENTERROCK_MAX; i++, centerLock++)
	{
		centerLock->use = true;													// 使用判定
		centerLock->pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 0);	// 座標データを初期化
		centerLock->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 回転データを初期化
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_CENTERROCK_SIZE_X, TEXTURE_CENTERROCK_SIZE_Y);
		centerLock->Radius = D3DXVec2Length(&temp);								// 半径を初期化
		centerLock->BaseAngle = atan2f(TEXTURE_CENTERROCK_SIZE_Y, TEXTURE_CENTERROCK_SIZE_X);	// 角度を初期化
		centerLock->Texture = D3DTextureCenterLock;								// テクスチャ情報
		MakeVertexCenterLock(i);													// 頂点情報の作成
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCenterLock(void)
{
	if (D3DTextureCenterLock != NULL)
	{	// テクスチャの開放
		D3DTextureCenterLock->Release();
		D3DTextureCenterLock = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCenterLock(void)
{
	CenterLock *centerLock = CenterLockWk;	// 中央のカーソルのポインターを初期化

	// ボスの取得
	Boss*boss;
	boss = GetBoss();
	
	for (int i = 0; i < CENTERROCK_MAX; i++, centerLock++)
	{
		if (centerLock->use == true)			// 使用している状態なら更新する
		{

			SetVertexCenterLock(i);				// 移動後の座標で頂点を設定
		}
		else if (GetMode() == MODE_BOSS && boss->hp <= 0)
		{
			centerLock->use = false;
		}
		else
		{
			centerLock->use = true;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCenterLock(void)
{
	// カメラの取得
	Camera *Cwk;
	Cwk = GetCamerawk();

	// ロックオントリガーが使用中の時のみ描画
	if (Cwk->LockOnTrigger == true)
	{
		LPDIRECT3DDEVICE9 Device = GetDevice();
		CenterLock *centerLock = CenterLockWk;

		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				// αデスティネーションカラーの指定
		Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z比較なし

		Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

		// 頂点フォーマットの設定
		Device->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < CENTERROCK_MAX; i++, centerLock++)
		{// 使用している状態なら描画する
			if (centerLock->use == true)
			{
				// テクスチャの設定
				Device->SetTexture(0, centerLock->Texture);

				// ポリゴンの描画
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_CENTERROCK, centerLock->vertexWk, sizeof(VERTEX_2D));
			}
		}

		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
		Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z比較あり

		Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexCenterLock( int no )
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// 中央のカーソルのポインターを初期化
	CenterLock *centerLock = &CenterLockWk[no];

	// 頂点座標の設定
	SetVertexCenterLock( no );

	// rhwの設定
	centerLock->vertexWk[0].rhw =
	centerLock->vertexWk[1].rhw =
	centerLock->vertexWk[2].rhw =
	centerLock->vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	centerLock->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	centerLock->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	centerLock->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	centerLock->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	centerLock->vertexWk[0].tex = D3DXVECTOR2(0.0f,0.0f);
	centerLock->vertexWk[1].tex = D3DXVECTOR2(1.0f,0.0f);
	centerLock->vertexWk[2].tex = D3DXVECTOR2(0.0f,1.0f);
	centerLock->vertexWk[3].tex = D3DXVECTOR2(1.0f,1.0f);

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureCenterLock( int no, int cntPattern )
{
	CenterLock *centerLock = &CenterLockWk[no];			// エネミーのポインターを初期化

	// テクスチャ座標の設定
	int x = cntPattern ;
	int y = cntPattern ;
	float sizeX = 1.0f;
	float sizeY = 1.0f ;
	
	centerLock->vertexWk[0].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY );
	centerLock->vertexWk[1].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY );
	centerLock->vertexWk[2].tex = D3DXVECTOR2( (float)( x ) * sizeX, (float)( y ) * sizeY + sizeY );
	centerLock->vertexWk[3].tex = D3DXVECTOR2( (float)( x ) * sizeX + sizeX, (float)( y ) * sizeY + sizeY );
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexCenterLock( int no )
{
	// 中央のカーソルのポインターを初期化
	CenterLock *centerLock = &CenterLockWk[no];
	
	// 頂点座標の設定
	centerLock->vertexWk[0].vtx.x = centerLock->pos.x - cosf(centerLock->BaseAngle + centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[0].vtx.y = centerLock->pos.y - sinf(centerLock->BaseAngle + centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[0].vtx.z = 0.0f;

	centerLock->vertexWk[1].vtx.x = centerLock->pos.x + cosf(centerLock->BaseAngle - centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[1].vtx.y = centerLock->pos.y - sinf(centerLock->BaseAngle - centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[1].vtx.z = 0.0f;

	centerLock->vertexWk[2].vtx.x = centerLock->pos.x - cosf(centerLock->BaseAngle - centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[2].vtx.y = centerLock->pos.y + sinf(centerLock->BaseAngle - centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[2].vtx.z = 0.0f;

	centerLock->vertexWk[3].vtx.x = centerLock->pos.x + cosf(centerLock->BaseAngle + centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[3].vtx.y = centerLock->pos.y + sinf(centerLock->BaseAngle + centerLock->rot.z) * centerLock->Radius;
	centerLock->vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 中央のカーソル取得関数
//=============================================================================
CenterLock *GetCenterLock(int no)
{
	return(&CenterLockWk[no]);
}

//=============================================================================
// 使用判定用関数
//=============================================================================
bool *GetUseCenterLock(void)
{
	return (&CenterLockWk->use);
}



