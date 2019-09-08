//=============================================================================
//
// �������� [explosion.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "explosion.h"
#include "input.h"
#include "camera.h"
#include "boss.h"
#include "sound.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 Device);
void SetVertexExplosion(int nIdxBullet, float fSizeX, float fSizeY);
void SetColorExplosion(int nIdxExplosion, D3DXCOLOR col);
void SetTextureExplosion(int nIdxExplosion, int nPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffExplosion = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				MtxWorldExplosion;			// ���[���h�}�g���b�N�X
EXPLOSION				Explosion[MAX_EXPLOSION];	// �������[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexExplosion(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,						// �f�o�C�X�ւ̃|�C���^
								TEXTURE_EXPLOSION,			// �t�@�C���̖��O
								&D3DTextureExplosion);	// �ǂݍ��ރ������[

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		Explosion[CntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Explosion[CntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Explosion[CntExplosion].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Explosion[CntExplosion].SizeX = EXPLOSION_SIZE_X;
		Explosion[CntExplosion].SizeY = EXPLOSION_SIZE_Y;
		Explosion[CntExplosion].Counter = 0;
		Explosion[CntExplosion].Pattern = 0;
		Explosion[CntExplosion].Type = EXPLOSIONTYPE_BULLET_Player;
		Explosion[CntExplosion].use = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosion(void)
{
	if(D3DTextureExplosion != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureExplosion->Release();
		D3DTextureExplosion = NULL;
	}

	if(D3DVtxBuffExplosion != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffExplosion->Release();
		D3DVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateExplosion(void)
{
	
	// �{�X�̎擾
	Boss *boss;
	boss = GetBoss();

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		if(Explosion[CntExplosion].use)
		{
			Explosion[CntExplosion].Counter--;
			if((Explosion[CntExplosion].Counter % 4) == 0)
			{
				//�p�^�[���J�E���g��i�߂�
				Explosion[CntExplosion].Pattern++;

				if(Explosion[CntExplosion].Pattern >= 8)
				{
					//���g�p��
					Explosion[CntExplosion].use = false;
				}
				else
				{
					// �e�N�X�`�����W�̐ݒ�
					SetTextureExplosion(CntExplosion, Explosion[CntExplosion].Pattern);
				}
			}

			// ���_���W�̐ݒ�
			Explosion[CntExplosion].SizeX += 0.50f;
			Explosion[CntExplosion].SizeY += 0.50f;
			SetVertexExplosion(CntExplosion, Explosion[CntExplosion].SizeX, Explosion[CntExplosion].SizeY);

			if(Explosion[CntExplosion].Pattern >= 2)
			{
				// ���_�J���[�̐ݒ�
				Explosion[CntExplosion].col.a -= 0.02f;
				if(Explosion[CntExplosion].col.a < 0.0f)
				{
					Explosion[CntExplosion].col.a = 0.0f;
				}
				SetColorExplosion(CntExplosion, Explosion[CntExplosion].col);
			}
		}
		// �G���f�B���O���o�p�ɖ��g�p��
		if (GetMode()==MODE_BOSS&&boss->use == false)
		{
			// ���g�p��
			Explosion->use = false;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX mtxView,mtxScale,mtxTranslate;

	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z��r�Ȃ�

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		if(Explosion[CntExplosion].use)
		{

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&MtxWorldExplosion);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			MtxWorldExplosion._11 = mtxView._11;
			MtxWorldExplosion._12 = mtxView._21;
			MtxWorldExplosion._13 = mtxView._31;
			MtxWorldExplosion._21 = mtxView._12;
			MtxWorldExplosion._22 = mtxView._22;
			MtxWorldExplosion._23 = mtxView._32;
			MtxWorldExplosion._31 = mtxView._13;
			MtxWorldExplosion._32 = mtxView._23;
			MtxWorldExplosion._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, Explosion[CntExplosion].scale.x, Explosion[CntExplosion].scale.y, Explosion[CntExplosion].scale.z);
			D3DXMatrixMultiply(&MtxWorldExplosion, &MtxWorldExplosion, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, Explosion[CntExplosion].pos.x, Explosion[CntExplosion].pos.y, Explosion[CntExplosion].pos.z);
			D3DXMatrixMultiply(&MtxWorldExplosion, &MtxWorldExplosion, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &MtxWorldExplosion);

			Device->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureExplosion);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntExplosion * 4), NUM_POLYGON);

			Device->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// Z��r����
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EXPLOSION,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffExplosion,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-EXPLOSION_SIZE_X / 2, -EXPLOSION_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(EXPLOSION_SIZE_X / 2, -EXPLOSION_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-EXPLOSION_SIZE_X / 2, EXPLOSION_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(EXPLOSION_SIZE_X / 2, EXPLOSION_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[1].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[2].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[3].vtx = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			Vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			Vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			Vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			Vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffExplosion->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexExplosion(int nIdxExplosion, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxExplosion * 4);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorExplosion(int nIdxExplosion, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxExplosion * 4);

		// ���_���W�̐ݒ�
		Vtx[0].diffuse =
		Vtx[1].diffuse =
		Vtx[2].diffuse =
		Vtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureExplosion(int IdxExplosion, int Pattern)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffExplosion->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (IdxExplosion * 4);

		// ���_���W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(Pattern * 0.125f, 0.0f);
		Vtx[1].tex = D3DXVECTOR2((Pattern + 1) * 0.125f, 0.0f);
		Vtx[2].tex = D3DXVECTOR2(Pattern * 0.125f, 1.0f);
		Vtx[3].tex = D3DXVECTOR2((Pattern + 1) * 0.125f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffExplosion->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetExplosion(D3DXVECTOR3 pos, float SizeX, float SizeY, int Type)
{
	int IdxExplosion = -1;

	for(int CntExplosion = 0; CntExplosion < MAX_EXPLOSION; CntExplosion++)
	{
		if(!Explosion[CntExplosion].use)
		{
			Explosion[CntExplosion].pos = pos;
			Explosion[CntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Explosion[CntExplosion].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			Explosion[CntExplosion].SizeX = SizeX;
			Explosion[CntExplosion].SizeY = SizeY;
			Explosion[CntExplosion].Counter = 0;
			Explosion[CntExplosion].Pattern = 0;
			Explosion[CntExplosion].Type = Type;
			Explosion[CntExplosion].use = true;

			// ���_���W�̐ݒ�
			SetVertexExplosion(CntExplosion, SizeX, SizeY);

			if(Explosion[CntExplosion].Type == EXPLOSIONTYPE_BULLET_Player)
			{
				Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); 
			}
			else if(Explosion[CntExplosion].Type == EXPLOSIONTYPE_BULLET_ENEMY)
			{
				Explosion[CntExplosion].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f); 
			}
			SetColorExplosion(CntExplosion, Explosion[CntExplosion].col);

			// �e�N�X�`�����W�̐ݒ�
			SetTextureExplosion(CntExplosion, 0);

			IdxExplosion = CntExplosion;

			break;
		}
	}

	return IdxExplosion;
}
