//=============================================================================
//
// レーダー処理 [Radar.cpp]
// Author : 西村任弘
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "rader.h"
#include "enemy.h"
#include "ace.h"
#include "boss.h"
#include "player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexRadar(void);
void SetTextureRadar(int pno, int cntPattern);	// 
void SetTextureMaker(int pno, int cntPattern);
void SetVertexRadar(int pno);					// 頂点の計算処理
void SetVertexMaker(int pno);
void SetPoint(D3DXVECTOR3 pos, int rno, int type);
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
float GetDistance(D3DXVECTOR3 speed);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LPDIRECT3DTEXTURE9		D3DTextureRadar = NULL;		// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9		D3DTexturePoint = NULL;		// テクスチャへのポインタ

Rader							radar[RADAR_MAX];
Maker							maker[POINT_MAX];

float							g_fRadiusRadar;					// ポリゴンの半径
float							g_fBaseAngleRadar;					// ポリゴンの角度

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRadar()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Rader *radar = NULL;
	Maker *maker = NULL;


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(Device,		// デバイスのポインタ
		TEXTURE_RADAR_NORMAL,				// ファイルの名前
		&D3DTextureRadar);					// 読み込むメモリのポインタ

	D3DXCreateTextureFromFile(Device,		// デバイスのポインタ
		TEXTURE_POINT_NORMAL,				// ファイルの名前
		&D3DTexturePoint);					// 読み込むメモリのポインタ


	//レーダーの部分
	for (int i = 0; i < RADAR_MAX; i++)
	{
		radar = GetRadar(i);
		radar->s_polygon.s_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		radar->s_polygon.s_pos = D3DXVECTOR3(RADAR_POS_X, RADAR_POS_Y, 0.0f);
		radar->s_polygon.s_CountAnim = 0;
		radar->s_polygon.s_PatternAnim = 0;
		radar->s_bUse = true;

		radar->s_polygon.s_size.x = RADAR_SIZE_X;
		radar->s_polygon.s_size.y = RADAR_SIZE_Y;

		D3DXVECTOR2 temp = D3DXVECTOR2(RADAR_SIZE_X, RADAR_SIZE_Y);
		radar->s_polygon.s_Radius = D3DXVec2Length(&temp);					// エネミーの半径を初期化
		radar->s_polygon.s_Angle = atan2f(RADAR_SIZE_Y, RADAR_SIZE_X);	// エネミーの角度を初期化

		radar->s_polygon.s_texture = D3DTextureRadar;					// テクスチャ情報
	}

	// マーカー部分
	for (int i = 0; i < POINT_MAX; i++)
	{
		maker = GetMaker(i);
		maker->s_polygon.s_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		maker->s_polygon.s_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		maker->s_polygon.s_CountAnim = 0;
		maker->s_polygon.s_PatternAnim = 0;
		maker->s_bUse = false;

		maker->s_polygon.s_size.x = POINT_SIZE_X;
		maker->s_polygon.s_size.y = POINT_SIZE_Y;


		D3DXVECTOR2 temp = D3DXVECTOR2(POINT_SIZE_X, POINT_SIZE_Y);
		maker->s_polygon.s_Radius = D3DXVec2Length(&temp);					// エネミーの半径を初期化
		maker->s_polygon.s_Angle = atan2f(POINT_SIZE_Y, POINT_SIZE_X);	// エネミーの角度を初期化

		maker->s_polygon.s_texture = D3DTexturePoint;					// テクスチャ情報
	}

	// 頂点情報の作成
	MakeVertexRadar();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRadar(void)
{
	if (D3DTextureRadar != NULL)
	{	// テクスチャの開放
		D3DTextureRadar->Release();
		D3DTextureRadar = NULL;
	}

	if (D3DTexturePoint != NULL)
	{	// テクスチャの開放
		D3DTexturePoint->Release();
		D3DTexturePoint = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRadar(void)
{
	Rader *radar = GetRadar(0);
	Player* player = GetPlayer();
	Boss*boss = GetBoss();
	Ace*ace = GetAce();
	Enemy* enemy = GetEnemy(0);

	for (int i = 0; i < RADAR_MAX; i++)
	{
		// アニメーション
		radar->s_polygon.s_CountAnim++;
		// アニメーションWaitチェック
		if ((radar->s_polygon.s_CountAnim % TIME_ANIMATION_RADAR) == 0)
		{
			// パターンの切り替え
			radar->s_polygon.s_PatternAnim = (radar->s_polygon.s_PatternAnim + 1) % ANIM_PATTERN_RADAR;

			// テクスチャ座標を設定
			SetTextureRadar(i, radar->s_polygon.s_PatternAnim);
		}
		SetVertexRadar(i);	// 頂点の計算処理

	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		enemy = GetEnemy(i);
		if (enemy->use == true)
		{
			if (GetDistance(player->pos, enemy->pos) <= RADAR_DISTANCE)
			{
				D3DXVECTOR3 posInRadar = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 dis = enemy->pos - player->pos;
				float angle = atan2f(dis.x, dis.z);
				float distance = GetDistance(D3DXVECTOR3(dis.x, 0.0f, dis.z));
				posInRadar.x = distance * sinf(angle - player->rot.y);
				posInRadar.z = distance * cosf(angle - player->rot.y);
				SetPoint(posInRadar, 0, ENEMYTYPE_MAX);
			}
		}
	}

	// aceがtrueなら
	if (ace->use == true)
	{
		if (GetDistance(player->pos, ace->pos) <= RADAR_DISTANCE)
		{
			D3DXVECTOR3 posInRadar = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 dis = ace->pos - player->pos;
			float angle = atan2f(dis.x, dis.z);
			float distance = GetDistance(D3DXVECTOR3(dis.x, 0.0f, dis.z));
			posInRadar.x = distance * sinf(angle - player->rot.y);
			posInRadar.z = distance * cosf(angle - player->rot.y);
			SetPoint(posInRadar, 0, ENEMYTYPE_MAX);
		}
	}

	// bossがtrueなら
	if (boss->use == true)
	{
		if (GetDistance(player->pos, boss->pos) <= RADAR_DISTANCE)
		{
			D3DXVECTOR3 posInRadar = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 dis = boss->pos - player->pos;
			float angle = atan2f(dis.x, dis.z);
			float distance = GetDistance(D3DXVECTOR3(dis.x, 0.0f, dis.z));
			posInRadar.x = distance * sinf(angle - player->rot.y);
			posInRadar.z = distance * cosf(angle - player->rot.y);
			SetPoint(posInRadar, 0, ENEMYTYPE_MAX);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRadar(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Rader *radar = NULL;
	Maker *maker = NULL;

	for (int i = 0; i < RADAR_MAX; i++)
	{
		// レーダーの取得
		radar = GetRadar(i);

		// レーダーがtrueなら
		if (radar->s_bUse)
		{
			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, radar->s_polygon.s_texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, radar->s_polygon.s_vertexWk, sizeof(VERTEX_2D));
		}
	}

	for (int i = 0; i < POINT_MAX; i++)
	{
		// マーカーの取得
		maker = GetMaker(i);

		if (maker->s_bUse == true)
		{
			// 頂点フォーマットの設定
			Device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			Device->SetTexture(0, maker->s_polygon.s_texture);

			// ポリゴンの描画
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, maker->s_polygon.s_vertexWk, sizeof(VERTEX_2D));

			// マーカーを未使用に
			maker->s_bUse = false;
		}

	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexRadar(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Rader *radar = NULL;
	Maker *maker = NULL;

	for (int i = 0; i < RADAR_MAX; i++)
	{
		// レーダーの取得
		radar = GetRadar(i);
		
		// 頂点座標の設定	
		SetVertexRadar(i);

		// rhwの設定
		radar->s_polygon.s_vertexWk[0].rhw =
			radar->s_polygon.s_vertexWk[1].rhw =
			radar->s_polygon.s_vertexWk[2].rhw =
			radar->s_polygon.s_vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		radar->s_polygon.s_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		radar->s_polygon.s_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		radar->s_polygon.s_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		radar->s_polygon.s_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		SetTextureRadar(i, radar->s_polygon.s_PatternAnim);
	}

	for (int i = 0; i < POINT_MAX; i++)
	{
		maker = GetMaker(i);
		// 頂点の計算処理

		// 頂点座標の設定	
		SetVertexMaker(i);

		// rhwの設定
		maker->s_polygon.s_vertexWk[0].rhw =
			maker->s_polygon.s_vertexWk[1].rhw =
			maker->s_polygon.s_vertexWk[2].rhw =
			maker->s_polygon.s_vertexWk[3].rhw = 1.0f;

		// 反射光の設定
		maker->s_polygon.s_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		maker->s_polygon.s_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		maker->s_polygon.s_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		maker->s_polygon.s_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		SetTextureMaker(i, maker->s_polygon.s_PatternAnim);
	}

	return S_OK;
}

//=============================================================================
// テクスチャ座標の設定（レーダー部分）
//=============================================================================
void SetTextureRadar(int pno, int cntPattern)
{
	// テクスチャ座標の設定
	Rader *radar = GetRadar(pno);

	int x = cntPattern % TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y;

	radar->s_polygon.s_vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	radar->s_polygon.s_vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	radar->s_polygon.s_vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	radar->s_polygon.s_vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// テクスチャ座標の設定（マーカー部分）
//=============================================================================
void SetTextureMaker(int pno, int cntPattern)
{
	// テクスチャ座標の設定
	Maker *maker = GetMaker(pno);

	int x = cntPattern % TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y;

	maker->s_polygon.s_vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	maker->s_polygon.s_vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	maker->s_polygon.s_vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	maker->s_polygon.s_vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// 頂点座標の設定（レーダー部分）
//=============================================================================
void SetVertexRadar(int pno)
{
	Rader *radar = GetRadar(pno);

	// 頂点座標の設定
	radar->s_polygon.s_vertexWk[0].vtx.x = radar->s_polygon.s_pos.x - cosf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[0].vtx.y = radar->s_polygon.s_pos.y - sinf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[0].vtx.z = 0.0f;

	radar->s_polygon.s_vertexWk[1].vtx.x = radar->s_polygon.s_pos.x + cosf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[1].vtx.y = radar->s_polygon.s_pos.y - sinf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[1].vtx.z = 0.0f;

	radar->s_polygon.s_vertexWk[2].vtx.x = radar->s_polygon.s_pos.x - cosf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[2].vtx.y = radar->s_polygon.s_pos.y + sinf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[2].vtx.z = 0.0f;

	radar->s_polygon.s_vertexWk[3].vtx.x = radar->s_polygon.s_pos.x + cosf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[3].vtx.y = radar->s_polygon.s_pos.y + sinf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// 頂点座標の設定（マーカー部分）
//=============================================================================
void SetVertexMaker(int pno)
{
	Maker *maker = GetMaker(pno);

	// 頂点座標の設定
	maker->s_polygon.s_vertexWk[0].vtx.x = maker->s_polygon.s_pos.x - cosf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[0].vtx.y = maker->s_polygon.s_pos.y - sinf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[0].vtx.z = 0.0f;

	maker->s_polygon.s_vertexWk[1].vtx.x = maker->s_polygon.s_pos.x + cosf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[1].vtx.y = maker->s_polygon.s_pos.y - sinf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[1].vtx.z = 0.0f;

	maker->s_polygon.s_vertexWk[2].vtx.x = maker->s_polygon.s_pos.x - cosf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[2].vtx.y = maker->s_polygon.s_pos.y + sinf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[2].vtx.z = 0.0f;

	maker->s_polygon.s_vertexWk[3].vtx.x = maker->s_polygon.s_pos.x + cosf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[3].vtx.y = maker->s_polygon.s_pos.y + sinf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// SetPoint処理
//=============================================================================
void SetPoint(D3DXVECTOR3 pos, int rno, int type)
{
	// マーカーの取得
	Maker* maker = GetMaker(0);

	// レーダーの取得
	Rader* radar = GetRadar(rno);

	// 表示できる最大値分繰り返す
	for (int i = 0; i < POINT_MAX; i++)
	{
		maker = GetMaker(i);

		// マーカーが未使用なら
		if (maker->s_bUse == false)
		{
			maker->s_polygon.s_pos.x = radar->s_polygon.s_pos.x - pos.x*RADAR_SIZE_X / RADAR_DISTANCE;
			maker->s_polygon.s_pos.y = radar->s_polygon.s_pos.y + pos.z*RADAR_SIZE_Y / RADAR_DISTANCE;
			if (type == ENEMYTYPE_MAX)
			{
				// 反射光の設定
				maker->s_polygon.s_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_Radius = POINT_SIZE_MIDDLE;

			}

			// マーカーを使用に
			maker->s_bUse = true;

			// 頂点の計算処理
			SetVertexMaker(i);
			return;
		}
	}
}

//=============================================================================
// 二つの位置の距離を取得
// pos1: 目標位置1 pos2: 目標位置2
//=============================================================================
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z);
	return D3DXVec3Length(&temp);
}

//=============================================================================
// 距離を取得
// speed: 三つの軸の距離
//=============================================================================
float GetDistance(D3DXVECTOR3 speed)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(speed.x, speed.y, speed.z);
	return D3DXVec3Length(&temp);
}

//=============================================================================
// レーダーの取得
//=============================================================================

Rader* GetRadar(int pno)
{
	return &radar[pno];
}

//=============================================================================
// レーダーに表示するマーカーを取得
//=============================================================================
Maker* GetMaker(int pno)
{
	return &maker[pno];
}
