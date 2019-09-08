//=============================================================================
//
// spaceboxの処理 [spacebox.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "player.h"
#include "spacebox.h"
#include "camera.h"
#include "input.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 spaceTexture = NULL;		// テクスチャ読み込み場所
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff=NULL;	// 頂点バッファインターフェースへのポインタ
D3DXMATRIX MtxSpaceWorld;					// ワールドマトリックス

D3DXVECTOR3	vertexinfo[8] = 
{
	D3DXVECTOR3(-SPACE_SIZE, SPACE_SIZE, SPACE_SIZE),
	D3DXVECTOR3(SPACE_SIZE, SPACE_SIZE, SPACE_SIZE),
	D3DXVECTOR3(-SPACE_SIZE,-SPACE_SIZE, SPACE_SIZE),
	D3DXVECTOR3(SPACE_SIZE,-SPACE_SIZE, SPACE_SIZE),
	D3DXVECTOR3(-SPACE_SIZE, SPACE_SIZE,-SPACE_SIZE),
	D3DXVECTOR3(SPACE_SIZE, SPACE_SIZE,-SPACE_SIZE),
	D3DXVECTOR3(-SPACE_SIZE,-SPACE_SIZE,-SPACE_SIZE),
	D3DXVECTOR3(SPACE_SIZE,-SPACE_SIZE,-SPACE_SIZE),
};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSpace()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();


	// テクスチャの読み込み
	if (!spaceTexture)
	{
		D3DXCreateTextureFromFile(Device,					// デバイスへのポインタ
			SPACE_TEXTURE_FILENAME,							// ファイルの名前
			&spaceTexture);									// 読み込むメモリー
	}


	// オブジェクトの頂点バッファを生成
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * SPACE_NUM_VERTEX*MAX_SPACE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,					// 頂点バッファの使用法　
		FVF_VERTEX_3D,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&D3DVtxBuff,						// 頂点バッファインターフェースへのポインタ
		NULL)))
	{
		return E_FAIL;
	}


	{//頂点バッファの中身を埋める
		VERTEX_3D *Vtx;


		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3DVtxBuff->Lock(0, 0, (void**)&Vtx, 0);

		// 頂点座標の設定
		Vtx[0].vtx = vertexinfo[0];
		Vtx[1].vtx = vertexinfo[1];
		Vtx[2].vtx = vertexinfo[2];
		Vtx[3].vtx = vertexinfo[3];

		// 法線ベクトルの設定
		Vtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// 反射光の設定
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[0].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
		Vtx[1].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
		Vtx[2].tex = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
		Vtx[3].tex = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);

		// 頂点座標の設定
		Vtx[4].vtx = vertexinfo[5];
		Vtx[5].vtx = vertexinfo[4];
		Vtx[6].vtx = vertexinfo[7];
		Vtx[7].vtx = vertexinfo[6];

		// 法線ベクトルの設定
		Vtx[4].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[5].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[6].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[7].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// 反射光の設定
		Vtx[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[6].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[7].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[4].tex = D3DXVECTOR2(3.0f / 4.0f, 1.0f / 3.0f);
		Vtx[5].tex = D3DXVECTOR2(1.0f, 1.0f / 3.0f);
		Vtx[6].tex = D3DXVECTOR2(3.0f / 4.0f, 2.0f / 3.0f);
		Vtx[7].tex = D3DXVECTOR2(1.0f, 2.0f / 3.0f);


		// 頂点座標の設定
		Vtx[8].vtx = vertexinfo[4];
		Vtx[9].vtx = vertexinfo[0];
		Vtx[10].vtx = vertexinfo[6];
		Vtx[11].vtx = vertexinfo[2];

		// 法線ベクトルの設定
		Vtx[8].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[9].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[10].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[11].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// 反射光の設定
		Vtx[8].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[9].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[10].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[11].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[8].tex = D3DXVECTOR2(0.0f, 1.0f / 3.0f);
		Vtx[9].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
		Vtx[10].tex = D3DXVECTOR2(0.0f, 2.0f / 3.0f);
		Vtx[11].tex = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);


		// 頂点座標の設定
		Vtx[12].vtx = vertexinfo[1];
		Vtx[13].vtx = vertexinfo[5];
		Vtx[14].vtx = vertexinfo[3];
		Vtx[15].vtx = vertexinfo[7];

		// 法線ベクトルの設定
		Vtx[12].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[13].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[14].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[15].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// 反射光の設定
		Vtx[12].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[13].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[15].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[12].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
		Vtx[13].tex = D3DXVECTOR2(3.0f / 4.0f, 1.0f / 3.0f);
		Vtx[14].tex = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);
		Vtx[15].tex = D3DXVECTOR2(3.0f / 4.0f, 2.0f / 3.0f);


		// 頂点座標の設定
		Vtx[16].vtx = vertexinfo[4];
		Vtx[17].vtx = vertexinfo[5];
		Vtx[18].vtx = vertexinfo[0];
		Vtx[19].vtx = vertexinfo[1];

		// 法線ベクトルの設定
		Vtx[16].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[17].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[18].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[19].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// 反射光の設定
		Vtx[16].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[18].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[19].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[16].tex = D3DXVECTOR2(1.0f / 4.0f, 0.0f);
		Vtx[17].tex = D3DXVECTOR2(2.0f / 4.0f, 0.0f);
		Vtx[18].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
		Vtx[19].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);

		// 頂点座標の設定
		Vtx[20].vtx = vertexinfo[2];
		Vtx[21].vtx = vertexinfo[3];
		Vtx[22].vtx = vertexinfo[6];
		Vtx[23].vtx = vertexinfo[7];

		// 法線ベクトルの設定
		Vtx[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// 反射光の設定
		Vtx[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[21].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[22].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		Vtx[20].tex = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
		Vtx[21].tex = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);
		Vtx[22].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f);
		Vtx[23].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f);

		// 頂点データをアンロックする
		D3DVtxBuff->Unlock();
	}
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitSpace(void)
{
	if (D3DVtxBuff)
	{// 頂点バッファの開放
		D3DVtxBuff->Release();
		D3DVtxBuff = NULL;
	}

	if (spaceTexture)
	{// テクスチャの開放
		spaceTexture->Release();
		spaceTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSpace(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSpace(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Player *player = GetPlayer();
	D3DXMATRIX MtxRot, MtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&MtxSpaceWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&MtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&MtxSpaceWorld, &MtxSpaceWorld, &MtxRot);

	// 移動を反映　常にプレーヤと同じ位置
	D3DXMatrixTranslation(&MtxTranslate, player->pos.x, player->pos.y, player->pos.z);
	D3DXMatrixMultiply(&MtxSpaceWorld, &MtxSpaceWorld, &MtxTranslate);

	// ワールドマトリックスの設定
	Device->SetTransform(D3DTS_WORLD, &MtxSpaceWorld);

	// 頂点バッファをレンダリングパイプラインに設定
	Device->SetStreamSource(0, D3DVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	Device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	Device->SetTexture(0, spaceTexture);


	for (int i = 0; i < 6; i++)
	{
		// ポリゴンの描画
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

