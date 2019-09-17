//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Kurosawa yuta
//
//=============================================================================
#include "effekseer.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
#include "player.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
::Effekseer::Vector3D D3DXVecToVec3D(D3DXVECTOR3 vec);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
EFFEKSEER					Effect[MAX_EFFEKSEER];			// 弾ワーク
EFFECTCONTROLLER		EffectCtrl;

static ::Effekseer::Vector3D			g_position;


//エフェクト構造体
static const EFK_CHAR* EffectFileName[] =
{
	(const EFK_CHAR*)L"data/EFFECT/sandaga.efk",
	(const EFK_CHAR*)L"data/EFFECT/ballet.efk",
	(const EFK_CHAR*)L"data/EFFECT/ballet.efk",
	(const EFK_CHAR*)L"data/EFFECT/sandaga.efk",

};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffekseer(int type)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	int Effect_No = 0;

	for (Effect_No = 0; Effect_No < MAX_EFFEKSEER; Effect_No++)
	{
		Effect[Effect_No].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		Effect[Effect_No].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Effect[Effect_No].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Effect[Effect_No].ID = -1;
		Effect[Effect_No].EffectType = -1;
		Effect[Effect_No].Use = false;
	}

	if (type == 0)
	{
		EffectCtrl.Manager = NULL;
		EffectCtrl.Render = NULL;
		EffectCtrl.Effect = NULL;
		EffectCtrl.EffectNum = sizeof(EffectFileName) / sizeof(const EFK_CHAR*);

		// 描画用インスタンスの生成
		EffectCtrl.Render = ::EffekseerRendererDX9::Renderer::Create(Device, 30000);

		// エフェクト管理用インスタンスの生成
		EffectCtrl.Manager = ::Effekseer::Manager::Create(30000);

		// 描画用インスタンスから描画機能を設定
		EffectCtrl.Manager->SetSpriteRenderer(EffectCtrl.Render->CreateSpriteRenderer());
		EffectCtrl.Manager->SetRibbonRenderer(EffectCtrl.Render->CreateRibbonRenderer());
		EffectCtrl.Manager->SetRingRenderer(EffectCtrl.Render->CreateRingRenderer());
		EffectCtrl.Manager->SetTrackRenderer(EffectCtrl.Render->CreateTrackRenderer());
		EffectCtrl.Manager->SetModelRenderer(EffectCtrl.Render->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		EffectCtrl.Manager->SetTextureLoader(EffectCtrl.Render->CreateTextureLoader());
		EffectCtrl.Manager->SetModelLoader(EffectCtrl.Render->CreateModelLoader());


		// メモリを確保
		EffectCtrl.Effect = (Effekseer::Effect**)calloc(EffectCtrl.EffectNum, sizeof(Effekseer::Effect*));
		if (EffectCtrl.Effect == NULL)
		{
			MessageBox(0, "Alloc Effect Memory Failed！", "Error", 0);
			return E_FAIL;
		}


		// エフェクトの読込
		for (int i = 0; i < EffectCtrl.EffectNum; i++)
		{
			if (FAILED(SafeLoadEffekseer(EffectCtrl.Manager, &EffectCtrl.Effect[i], EffectFileName[i], "Effect")))
			{
				return E_FAIL;
			}
		}
	}

	// 全エフェクト停止
	EffectCtrl.Manager->StopAllEffects();


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffekseer(void)
{
	// エフェクトの停止
	EffectCtrl.Manager->StopAllEffects();

	// エフェクトの破棄
	//SafeFree(EffectCtrl.Effect);
	for (int i = 0; i < EffectCtrl.EffectNum; i++)
	{
		ES_SAFE_RELEASE(EffectCtrl.Effect[i]);
	}

	// 先にエフェクト管理用インスタンスを破棄
	EffectCtrl.Manager->Destroy();

	// 次に描画用インスタンスを破棄
	EffectCtrl.Render->Destroy();

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffekseer(void)
{
	static int Count = 0;
	int Effect_No = 0;
	int EffectID = 0;
	Player *player = GetPlayer();
	// ボスの取得
	Boss *boss;
	boss = GetBoss();

	for (Effect_No = 0; Effect_No < MAX_EFFEKSEER; Effect_No++)
	{
		if (Effect[Effect_No].Use == true)
		{
			EffectID = Effect[Effect_No].ID;

			switch (Effect[Effect_No].EffectType)
			{
			case 0:
			case 1:
				EffectCtrl.Manager->SetLocation(EffectID, D3DXVecToVec3D(Effect[Effect_No].pos));
				break;

			case 2:
				EffectCtrl.Manager->SetLocation(EffectID, D3DXVecToVec3D(boss->pos));
				break;

			case 3:
				EffectCtrl.Manager->SetLocation(EffectID, D3DXVecToVec3D(boss->pos));
				break;

			}
		}
	}

	

	// エフェクトの更新処理を行う
	EffectCtrl.Manager->Update();

	return;
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawEffekseer(void)
{
	// カメラの取得
	Camera		*Cwk = GetCamerawk();

	D3DXVECTOR3 cameraEye = Cwk->PosCameraP;
	D3DXVECTOR3 cameraAt = Cwk->PosCameraR;
	D3DXVECTOR3 cameraUp = Cwk->PosCameraU;

	// 投影行列を設定
	EffectCtrl.Render->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z));

	// カメラ行列を設定
	EffectCtrl.Render->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(D3DXVecToVec3D(cameraEye),
			D3DXVecToVec3D(cameraAt), D3DXVecToVec3D(cameraUp)));

	// エフェクトの描画開始処理を行う。
	EffectCtrl.Render->BeginRendering();

	// エフェクトの描画を行う。
	EffectCtrl.Manager->Draw();

	// エフェクトの描画終了処理を行う。
	EffectCtrl.Render->EndRendering();

	return;

}

//=============================================================================
// エフェクトを読み込む
//=============================================================================
HRESULT SafeLoadEffekseer(Effekseer::Manager *Manager, Effekseer::Effect **Effect, const EFK_CHAR* SrcPath, const char* ErrorSrc)
{
	char Message[256];
	// エフェクトファイルの中のテクスチャの数
	int TextureNum = 0;

	// エフェクトファイルを読み込む
	(*Effect) = Effekseer::Effect::Create(Manager, SrcPath);

	// 失敗したら
	if ((*Effect) == NULL)
	{
		sprintf(Message, "Load %s Effect Failed！", ErrorSrc);

		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	// エフェクトファイルの中のテクスチャの数を取得する
	TextureNum = (*Effect)->GetColorImageCount();
	for (int i = 0; i < TextureNum; i++)
	{
		// テクスチャのポインタを取得する
		Effekseer::TextureData *Texture = (*Effect)->GetColorImage(i);

		// テクスチャを読み込むが失敗したら
		if (Texture == NULL)
		{
			sprintf(Message, "Load %s Effect Texture Failed！", ErrorSrc);

			MessageBox(0, Message, "Error", 0);
			return E_FAIL;
		}
	}

	return S_OK;
}

void SetEffekseer(D3DXVECTOR3 pos, int effectType)
{
	for (int effectNo = 0; effectNo < MAX_EFFEKSEER; effectNo++)
	{
		if (Effect[effectNo].Use == false)
		{
			Effect[effectNo].ID = EffectCtrl.Manager->Play(EffectCtrl.Effect[effectType], pos.x, pos.y, pos.x);
			Effect[effectNo].pos = pos;
			Effect[effectNo].EffectType =  effectType;
			Effect[effectNo].Use = true;
			break;
		}
	}
	return;
}


::Effekseer::Vector3D D3DXVecToVec3D(D3DXVECTOR3 vec)
{
	return ::Effekseer::Vector3D(vec.x, vec.y, vec.z);
}

