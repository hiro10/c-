//=============================================================================
//
// ���C������ [main.cpp]
// Author : �����@�C�O
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
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"MOONSHOT"			// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
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
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			D3D = NULL;			// Direct3D �I�u�W�F�N�g
LPDIRECT3DDEVICE9	D3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
LPD3DXFONT			D3DXFont = NULL;		// �t�H���g�ւ̃|�C���^
int					CountFPS;			// FPS�J�E���^
#endif
MODE				Mode = MODE_TITLE;	// ���[�h

//=============================================================================
// ���C���֐�
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
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
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

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, Hwnd, true)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	ExecLastTime = 
	FPSLastTime = timeGetTime();
	CurrentTime =
	FrameCount = 0;

	
	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
	
	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (Msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
		else
		{
			CurrentTime = timeGetTime();
			if ((CurrentTime - FPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
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

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				FrameCount++;
			}
		}
	
	}
		// �E�B���h�E�N���X�̓o�^������
		UnregisterClass(CLASS_NAME, wcex.hInstance);

		// �I������
		Uninit();

		// ����\��߂�
		timeEndPeriod(1);				
	
	return (int)Msg.wParam;
}

//=============================================================================
// �v���V�[�W��
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
		case VK_ESCAPE:					// [ESC]�L�[�������ꂽ
			DestroyWindow(Hwnd);		// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(Hwnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND Hwnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(D3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory( &d3dpp, sizeof(d3dpp) );						// ���[�N���[���N���A
	d3dpp.BackBufferCount			= 1;
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									Hwnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &D3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										Hwnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &D3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if(FAILED(D3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											Hwnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &D3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
    D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�p�����[�^�̐ݒ�
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����
	
#ifdef _DEBUG
	// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(D3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &D3DXFont);
#endif
	// ���͏����̏�����
	InitInput(hInstance, Hwnd);

	// �t�F�[�h�̏�����
	InitFade();

	// �T�E���h�̏�����
	InitSound(Hwnd);

	// �ŏ��̓^�C�g����ʂ�
	SetMode(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	if(D3DXFont != NULL)
	{// ���\���p�t�H���g�̊J��
		D3DXFont->Release();
		D3DXFont = NULL;
	}
#endif

	if(D3DDevice != NULL)
	{// �f�o�C�X�̊J��
		D3DDevice->Release();
		D3DDevice = NULL;
	}

	if(D3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		D3D->Release();
		D3D = NULL;
	}

	// �t�F�[�h�̏I������
	UninitFade();

	// �T�E���h�̏I������
	UninitSound();

	// ���͏����̏I������
	UninitInput();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͍X�V
	UpdateInput();

	switch(Mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̍X�V
		UpdateTitle();
		break;

	case MODE_GAME:			// �Q�[����ʂ̍X�V(ACE��j
		UpdateGame();
		break;

	case MODE_BOSS:			// �Q�[����ʂ̍X�V(BOSS��j
		UpdateBossGame();
		break;

	case MODE_RESULT:		// ���U���g��ʂ̍X�V
		UpdateResult();
		break;
	}

	// �t�F�[�h�����̍X�V
	UpdateFade();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	D3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�t�H�O�̐ݒ�
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

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(D3DDevice->BeginScene()))
	{
		switch(Mode)
		{
		case MODE_TITLE:		// �^�C�g����ʂ̕`��
			DrawTitle();
			break;

		case MODE_GAME:			// �Q�[����ʂ̕`��(ACE��)
			DrawGame();
			break;

		case MODE_BOSS:			// �Q�[����ʂ̕`��(BOSS��)
			DrawBossGame();
			break;

		case MODE_RESULT:		// ���U���g��ʂ̕`��
			DrawResult();
			break;
		}

		// �t�F�[�h�`��
		DrawFade();

#ifdef _DEBUG
		// �f�o�b�O�\��
		DrawFPS();
#endif

		// Direct3D�ɂ��`��̏I��
		D3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	D3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return D3DDevice;
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(MODE mode)
{
	Mode = mode;

	switch(Mode)
	{
	case MODE_TITLE:
		// ���U���g��ʂ̏I������
		UninitResult();

		// �^�C�g����ʂ̏�����
		InitTitle();

		break;

	case MODE_GAME:
		// �^�C�g����ʂ̏I������
		UninitTitle();

		// �Q�[����ʂ̏�����
		InitGame();

		break;

	case MODE_BOSS:
		// �^�C�g����ʂ̏I������
		UninitGame();

		// �Q�[����ʂ̏�����
		InitBossGame();

		break;

	case MODE_RESULT:
		// �Q�[����ʂ̏I������
		UninitBossGame();

		// ���U���g��ʂ̏�����
		InitResult();

		break;
	}
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
MODE GetMode(void)
{
	return Mode;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O�\��
//=============================================================================
void DrawFPS(void)
{
	RECT Rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char Str[256];

	wsprintf(Str, "FPS:%d\n", CountFPS);

	// �e�L�X�g�`��
	D3DXFont->DrawText(NULL, Str, -1, &Rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif