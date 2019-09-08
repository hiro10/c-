//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 西村　任弘
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"MOONSHOT"			// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND Hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND Hwnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
#ifdef _DEBUG
void DrawFPS(void);
#endif

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			D3D = NULL;			// Direct3D オブジェクト
LPDIRECT3DDEVICE9	D3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
LPD3DXFONT			D3DXFont = NULL;		// フォントへのポインタ
int					CountFPS;			// FPSカウンタ
#endif
MODE				Mode = MODE_TITLE;	// モード

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	
	UNREFERENCED_PARAMETER(lpCmdLine);		

	DWORD ExecLastTime;
	DWORD FPSLastTime;
	DWORD CurrentTime;
	DWORD FrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND Hwnd;
	MSG Msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	Hwnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, Hwnd, true)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	ExecLastTime = 
	FPSLastTime = timeGetTime();
	CurrentTime =
	FrameCount = 0;

	
	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
	
	// メッセージループ
	while (1)
	{
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
		else
		{
			CurrentTime = timeGetTime();
			if ((CurrentTime - FPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				CountFPS = FrameCount * 1000 / (CurrentTime - FPSLastTime);
#endif
				FPSLastTime = CurrentTime;
				FrameCount = 0;
			}

			if ((CurrentTime - ExecLastTime) >= (1000 / 60))
			{
				ExecLastTime = CurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				FrameCount++;
			}
		}
	
	}
		// ウィンドウクラスの登録を解除
		UnregisterClass(CLASS_NAME, wcex.hInstance);

		// 終了処理
		Uninit();

		// 分解能を戻す
		timeEndPeriod(1);				
	
	return (int)Msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND Hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:					// [ESC]キーが押された
			DestroyWindow(Hwnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(Hwnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND Hwnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(D3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory( &d3dpp, sizeof(d3dpp) );						// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									Hwnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &D3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										Hwnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &D3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											Hwnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &D3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
    D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートパラメータの設定
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数
	
#ifdef _DEBUG
	// 情報表示用フォントを設定
	D3DXCreateFont(D3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &D3DXFont);
#endif
	// 入力処理の初期化
	InitInput(hInstance, Hwnd);

	// フェードの初期化
	InitFade();

	// サウンドの初期化
	InitSound(Hwnd);

	// 最初はタイトル画面に
	SetMode(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	if(D3DXFont != NULL)
	{// 情報表示用フォントの開放
		D3DXFont->Release();
		D3DXFont = NULL;
	}
#endif

	if(D3DDevice != NULL)
	{// デバイスの開放
		D3DDevice->Release();
		D3DDevice = NULL;
	}

	if(D3D != NULL)
	{// Direct3Dオブジェクトの開放
		D3D->Release();
		D3D = NULL;
	}

	// フェードの終了処理
	UninitFade();

	// サウンドの終了処理
	UninitSound();

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力更新
	UpdateInput();

	switch(Mode)
	{
	case MODE_TITLE:		// タイトル画面の更新
		UpdateTitle();
		break;

	case MODE_GAME:			// ゲーム画面の更新(ACE戦）
		UpdateGame();
		break;

	case MODE_BOSS:			// ゲーム画面の更新(BOSS戦）
		UpdateBossGame();
		break;

	case MODE_RESULT:		// リザルト画面の更新
		UpdateResult();
		break;
	}

	// フェード処理の更新
	UpdateFade();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	D3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//フォグの設定
	if (Mode==MODE_GAME)
	{
		
		FLOAT StartPos = 10;
		FLOAT EndPos = 10000;

		D3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		D3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0x0, 0x0f, 0x00, 0x0f));
		D3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		D3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		D3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));
		D3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));
	}

	else
	{
		D3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}

	// Direct3Dによる描画の開始
	if(SUCCEEDED(D3DDevice->BeginScene()))
	{
		switch(Mode)
		{
		case MODE_TITLE:		// タイトル画面の描画
			DrawTitle();
			break;

		case MODE_GAME:			// ゲーム画面の描画(ACE戦)
			DrawGame();
			break;

		case MODE_BOSS:			// ゲーム画面の描画(BOSS戦)
			DrawBossGame();
			break;

		case MODE_RESULT:		// リザルト画面の描画
			DrawResult();
			break;
		}

		// フェード描画
		DrawFade();

#ifdef _DEBUG
		// デバッグ表示
		DrawFPS();
#endif

		// Direct3Dによる描画の終了
		D3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	D3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return D3DDevice;
}

//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
	Mode = mode;

	switch(Mode)
	{
	case MODE_TITLE:
		// リザルト画面の終了処理
		UninitResult();

		// タイトル画面の初期化
		InitTitle();

		break;

	case MODE_GAME:
		// タイトル画面の終了処理
		UninitTitle();

		// ゲーム画面の初期化
		InitGame();

		break;

	case MODE_BOSS:
		// タイトル画面の終了処理
		UninitGame();

		// ゲーム画面の初期化
		InitBossGame();

		break;

	case MODE_RESULT:
		// ゲーム画面の終了処理
		UninitBossGame();

		// リザルト画面の初期化
		InitResult();

		break;
	}
}

//=============================================================================
// モードの取得
//=============================================================================
MODE GetMode(void)
{
	return Mode;
}

#ifdef _DEBUG
//=============================================================================
// デバッグ表示
//=============================================================================
void DrawFPS(void)
{
	RECT Rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char Str[256];

	wsprintf(Str, "FPS:%d\n", CountFPS);

	// テキスト描画
	D3DXFont->DrawText(NULL, Str, -1, &Rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif