//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "effect.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 Device);
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY);
void SetColorEffect(int nIdxEffect, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		D3DTextureEffect = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffEffect = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX				MtxWorldEffect;				// ���[���h�}�g���b�N�X
Effect1					effect[MAX_EFFECT];			// �G�t�F�N�g���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���̍쐬
	MakeVertexEffect(Device);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_EFFECT,			// �t�@�C���̖��O
								&D3DTextureEffect);		// �ǂݍ��ރ������[

	for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		effect[CntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect[CntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect[CntEffect].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		effect[CntEffect].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		effect[CntEffect].SizeX = EFFECT_SIZE_X;
		effect[CntEffect].SizeY = EFFECT_SIZE_Y;
		effect[CntEffect].Count = 0;
		effect[CntEffect].Pattern = 0;
		effect[CntEffect].Timer = 0;
		effect[CntEffect].use = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
{
	if(D3DTextureEffect != NULL)
	{// �e�N�X�`���̊J��
		D3DTextureEffect->Release();
		D3DTextureEffect = NULL;
	}

	if(D3DVtxBuffEffect != NULL)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffEffect->Release();
		D3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	for (int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		if (effect[CntEffect].use)
		{
			effect[CntEffect].pos.x += effect[CntEffect].move.x;
			effect[CntEffect].pos.z += effect[CntEffect].move.z;

			effect[CntEffect].col.a -= effect[CntEffect].DecAlpha;
			if (effect[CntEffect].col.a <= 0.0f)
			{
				effect[CntEffect].col.a = 0.0f;
			}
			SetColorEffect(CntEffect,
				D3DXCOLOR(effect[CntEffect].col.r, effect[CntEffect].col.b,
					effect[CntEffect].col.b, effect[CntEffect].col.a));

			effect[CntEffect].Timer--;
			if (effect[CntEffect].Timer <= 0)
			{
				effect[CntEffect].use = false;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXMATRIX MtxView,MtxScale,MtxTranslate;

	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z��r�Ȃ�

	Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		if(effect[CntEffect].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&MtxWorldEffect);

			// �r���[�}�g���b�N�X���擾
			MtxView = GetMtxView();

			MtxWorldEffect._11 = MtxView._11;
			MtxWorldEffect._12 = MtxView._21;
			MtxWorldEffect._13 = MtxView._31;
			MtxWorldEffect._21 = MtxView._12;
			MtxWorldEffect._22 = MtxView._22;
			MtxWorldEffect._23 = MtxView._32;
			MtxWorldEffect._31 = MtxView._13;
			MtxWorldEffect._32 = MtxView._23;
			MtxWorldEffect._33 = MtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&MtxScale, effect[CntEffect].scl.x, effect[CntEffect].scl.y, effect[CntEffect].scl.z);
			D3DXMatrixMultiply(&MtxWorldEffect, &MtxWorldEffect, &MtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&MtxTranslate, effect[CntEffect].pos.x, effect[CntEffect].pos.y, effect[CntEffect].pos.z);
			D3DXMatrixMultiply(&MtxWorldEffect, &MtxWorldEffect, &MtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			Device->SetTransform(D3DTS_WORLD, &MtxWorldEffect);

			Device->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			Device->SetStreamSource(0, D3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, D3DTextureEffect);

			// �|���S���̕`��
			Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (CntEffect * 4), NUM_POLYGON);

			Device->SetRenderState(D3DRS_LIGHTING, TRUE);
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

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 Device)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( Device->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffEffect,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffEffect->Lock(0, 0, (void**)&Vtx, 0);

		for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++, Vtx += 4)
		{
			// ���_���W�̐ݒ�
			Vtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			Vtx[1].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, -EFFECT_SIZE_Y / 2, 0.0f);
			Vtx[2].vtx = D3DXVECTOR3(-EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);
			Vtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X / 2, EFFECT_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			Vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			Vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
		D3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexEffect(int nIdxEffect, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffEffect->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxEffect * 4);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		Vtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		Vtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorEffect(int nIdxEffect, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffEffect->Lock(0, 0, (void**)&Vtx, 0);

		Vtx += (nIdxEffect * 4);

		// ���_���W�̐ݒ�
		Vtx[0].diffuse =
		Vtx[1].diffuse =
		Vtx[2].diffuse =
		Vtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffEffect->Unlock();
	}
}
//=============================================================================
// �G�t�F�N�g�̐ݒ�
//=============================================================================
int SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer)
{
	int nIdxEffect = -1;

	for(int CntEffect = 0; CntEffect < MAX_EFFECT; CntEffect++)
	{
		if(!effect[CntEffect].use)
		{
			effect[CntEffect].pos = pos;
			effect[CntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			effect[CntEffect].scl= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			effect[CntEffect].move = move;
			effect[CntEffect].col = col;
			effect[CntEffect].SizeX = fSizeX;
			effect[CntEffect].SizeY = fSizeY;
			effect[CntEffect].Timer = nTimer;
			effect[CntEffect].DecAlpha = col.a / nTimer;
			effect[CntEffect].use = true;

			// ���_���W�̐ݒ�
			SetVertexEffect(CntEffect, fSizeX, fSizeY);

			// ���_�J���[�̐ݒ�
			SetColorEffect(CntEffect,
							D3DXCOLOR(effect[CntEffect].col.r,effect[CntEffect].col.b,
										effect[CntEffect].col.b, effect[CntEffect].col.a));

			nIdxEffect = CntEffect;

			break;
		}
	}
	return nIdxEffect;
}
