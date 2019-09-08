//=============================================================================
//
// �����̃J�[�\������ [CenterLock.cpp]
// Author : �����@�C�O
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
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexCenterLock( int no );
void SetTextureCenterLock( int no, int cntPattern );
void SetVertexCenterLock( int no );

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureCenterLock = NULL;		// �e�N�X�`���ւ̃|���S��

CenterLock				CenterLockWk[CENTERROCK_MAX];		//�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitCenterLock(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CenterLock *centerLock = CenterLockWk;			//�@�����̃J�[�\���̃|�C���^�[��������

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�̃|�C���^
		TEXTURE_GAME_CENTERROCK,						// �t�@�C���̖��O
		&D3DTextureCenterLock);						// �ǂݍ��ރ������̃|�C���^

	// �����̃J�[�\���̏���������
	for (int i = 0; i < CENTERROCK_MAX; i++, centerLock++)
	{
		centerLock->use = true;													// �g�p����
		centerLock->pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 , 0);	// ���W�f�[�^��������
		centerLock->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// ��]�f�[�^��������
		D3DXVECTOR2 temp = D3DXVECTOR2(TEXTURE_CENTERROCK_SIZE_X, TEXTURE_CENTERROCK_SIZE_Y);
		centerLock->Radius = D3DXVec2Length(&temp);								// ���a��������
		centerLock->BaseAngle = atan2f(TEXTURE_CENTERROCK_SIZE_Y, TEXTURE_CENTERROCK_SIZE_X);	// �p�x��������
		centerLock->Texture = D3DTextureCenterLock;								// �e�N�X�`�����
		MakeVertexCenterLock(i);													// ���_���̍쐬
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCenterLock(void)
{
	if (D3DTextureCenterLock != NULL)
	{	// �e�N�X�`���̊J��
		D3DTextureCenterLock->Release();
		D3DTextureCenterLock = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCenterLock(void)
{
	CenterLock *centerLock = CenterLockWk;	// �����̃J�[�\���̃|�C���^�[��������

	// �{�X�̎擾
	Boss*boss;
	boss = GetBoss();
	
	for (int i = 0; i < CENTERROCK_MAX; i++, centerLock++)
	{
		if (centerLock->use == true)			// �g�p���Ă����ԂȂ�X�V����
		{

			SetVertexCenterLock(i);				// �ړ���̍��W�Œ��_��ݒ�
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
// �`�揈��
//=============================================================================
void DrawCenterLock(void)
{
	// �J�����̎擾
	Camera *Cwk;
	Cwk = GetCamerawk();

	// ���b�N�I���g���K�[���g�p���̎��̂ݕ`��
	if (Cwk->LockOnTrigger == true)
	{
		LPDIRECT3DDEVICE9 Device = GetDevice();
		CenterLock *centerLock = CenterLockWk;

		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);				// ���f�X�e�B�l�[�V�����J���[�̎w��
		Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);				// Z��r�Ȃ�

		Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

		// ���_�t�H�[�}�b�g�̐ݒ�
		Device->SetFVF(FVF_VERTEX_2D);

		for (int i = 0; i < CENTERROCK_MAX; i++, centerLock++)
		{// �g�p���Ă����ԂȂ�`�悷��
			if (centerLock->use == true)
			{
				// �e�N�X�`���̐ݒ�
				Device->SetTexture(0, centerLock->Texture);

				// �|���S���̕`��
				Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_CENTERROCK, centerLock->vertexWk, sizeof(VERTEX_2D));
			}
		}

		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
		Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z��r����

		Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexCenterLock( int no )
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// �����̃J�[�\���̃|�C���^�[��������
	CenterLock *centerLock = &CenterLockWk[no];

	// ���_���W�̐ݒ�
	SetVertexCenterLock( no );

	// rhw�̐ݒ�
	centerLock->vertexWk[0].rhw =
	centerLock->vertexWk[1].rhw =
	centerLock->vertexWk[2].rhw =
	centerLock->vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	centerLock->vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	centerLock->vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	centerLock->vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	centerLock->vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	centerLock->vertexWk[0].tex = D3DXVECTOR2(0.0f,0.0f);
	centerLock->vertexWk[1].tex = D3DXVECTOR2(1.0f,0.0f);
	centerLock->vertexWk[2].tex = D3DXVECTOR2(0.0f,1.0f);
	centerLock->vertexWk[3].tex = D3DXVECTOR2(1.0f,1.0f);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureCenterLock( int no, int cntPattern )
{
	CenterLock *centerLock = &CenterLockWk[no];			// �G�l�~�[�̃|�C���^�[��������

	// �e�N�X�`�����W�̐ݒ�
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
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexCenterLock( int no )
{
	// �����̃J�[�\���̃|�C���^�[��������
	CenterLock *centerLock = &CenterLockWk[no];
	
	// ���_���W�̐ݒ�
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
// �����̃J�[�\���擾�֐�
//=============================================================================
CenterLock *GetCenterLock(int no)
{
	return(&CenterLockWk[no]);
}

//=============================================================================
// �g�p����p�֐�
//=============================================================================
bool *GetUseCenterLock(void)
{
	return (&CenterLockWk->use);
}



