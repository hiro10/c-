//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Kurosawa yuta
//
//=============================================================================
#include "effekseer.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
#include "player.h"


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
::Effekseer::Vector3D D3DXVecToVec3D(D3DXVECTOR3 vec);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
EFFEKSEER					Effect[MAX_EFFEKSEER];			// �e���[�N
EFFECTCONTROLLER		EffectCtrl;

static ::Effekseer::Vector3D			g_position;


//�G�t�F�N�g�\����
static const EFK_CHAR* EffectFileName[] =
{
	(const EFK_CHAR*)L"data/EFFECT/sandaga.efk",
	(const EFK_CHAR*)L"data/EFFECT/ballet.efk",
	(const EFK_CHAR*)L"data/EFFECT/ballet.efk",
	(const EFK_CHAR*)L"data/EFFECT/sandaga.efk",

};


//=============================================================================
// ����������
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

		// �`��p�C���X�^���X�̐���
		EffectCtrl.Render = ::EffekseerRendererDX9::Renderer::Create(Device, 30000);

		// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
		EffectCtrl.Manager = ::Effekseer::Manager::Create(30000);

		// �`��p�C���X�^���X����`��@�\��ݒ�
		EffectCtrl.Manager->SetSpriteRenderer(EffectCtrl.Render->CreateSpriteRenderer());
		EffectCtrl.Manager->SetRibbonRenderer(EffectCtrl.Render->CreateRibbonRenderer());
		EffectCtrl.Manager->SetRingRenderer(EffectCtrl.Render->CreateRingRenderer());
		EffectCtrl.Manager->SetTrackRenderer(EffectCtrl.Render->CreateTrackRenderer());
		EffectCtrl.Manager->SetModelRenderer(EffectCtrl.Render->CreateModelRenderer());

		// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		EffectCtrl.Manager->SetTextureLoader(EffectCtrl.Render->CreateTextureLoader());
		EffectCtrl.Manager->SetModelLoader(EffectCtrl.Render->CreateModelLoader());


		// ���������m��
		EffectCtrl.Effect = (Effekseer::Effect**)calloc(EffectCtrl.EffectNum, sizeof(Effekseer::Effect*));
		if (EffectCtrl.Effect == NULL)
		{
			MessageBox(0, "Alloc Effect Memory Failed�I", "Error", 0);
			return E_FAIL;
		}


		// �G�t�F�N�g�̓Ǎ�
		for (int i = 0; i < EffectCtrl.EffectNum; i++)
		{
			if (FAILED(SafeLoadEffekseer(EffectCtrl.Manager, &EffectCtrl.Effect[i], EffectFileName[i], "Effect")))
			{
				return E_FAIL;
			}
		}
	}

	// �S�G�t�F�N�g��~
	EffectCtrl.Manager->StopAllEffects();


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffekseer(void)
{
	// �G�t�F�N�g�̒�~
	EffectCtrl.Manager->StopAllEffects();

	// �G�t�F�N�g�̔j��
	//SafeFree(EffectCtrl.Effect);
	for (int i = 0; i < EffectCtrl.EffectNum; i++)
	{
		ES_SAFE_RELEASE(EffectCtrl.Effect[i]);
	}

	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	EffectCtrl.Manager->Destroy();

	// ���ɕ`��p�C���X�^���X��j��
	EffectCtrl.Render->Destroy();

	return;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffekseer(void)
{
	static int Count = 0;
	int Effect_No = 0;
	int EffectID = 0;
	Player *player = GetPlayer();
	// �{�X�̎擾
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

	

	// �G�t�F�N�g�̍X�V�������s��
	EffectCtrl.Manager->Update();

	return;
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffekseer(void)
{
	// �J�����̎擾
	Camera		*Cwk = GetCamerawk();

	D3DXVECTOR3 cameraEye = Cwk->PosCameraP;
	D3DXVECTOR3 cameraAt = Cwk->PosCameraR;
	D3DXVECTOR3 cameraUp = Cwk->PosCameraU;

	// ���e�s���ݒ�
	EffectCtrl.Render->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z));

	// �J�����s���ݒ�
	EffectCtrl.Render->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(D3DXVecToVec3D(cameraEye),
			D3DXVecToVec3D(cameraAt), D3DXVecToVec3D(cameraUp)));

	// �G�t�F�N�g�̕`��J�n�������s���B
	EffectCtrl.Render->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	EffectCtrl.Manager->Draw();

	// �G�t�F�N�g�̕`��I���������s���B
	EffectCtrl.Render->EndRendering();

	return;

}

//=============================================================================
// �G�t�F�N�g��ǂݍ���
//=============================================================================
HRESULT SafeLoadEffekseer(Effekseer::Manager *Manager, Effekseer::Effect **Effect, const EFK_CHAR* SrcPath, const char* ErrorSrc)
{
	char Message[256];
	// �G�t�F�N�g�t�@�C���̒��̃e�N�X�`���̐�
	int TextureNum = 0;

	// �G�t�F�N�g�t�@�C����ǂݍ���
	(*Effect) = Effekseer::Effect::Create(Manager, SrcPath);

	// ���s������
	if ((*Effect) == NULL)
	{
		sprintf(Message, "Load %s Effect Failed�I", ErrorSrc);

		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	// �G�t�F�N�g�t�@�C���̒��̃e�N�X�`���̐����擾����
	TextureNum = (*Effect)->GetColorImageCount();
	for (int i = 0; i < TextureNum; i++)
	{
		// �e�N�X�`���̃|�C���^���擾����
		Effekseer::TextureData *Texture = (*Effect)->GetColorImage(i);

		// �e�N�X�`����ǂݍ��ނ����s������
		if (Texture == NULL)
		{
			sprintf(Message, "Load %s Effect Texture Failed�I", ErrorSrc);

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

