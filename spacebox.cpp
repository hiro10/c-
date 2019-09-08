//=============================================================================
//
// spacebox�̏��� [spacebox.cpp]
// Author : �����@�C�O
//
//=============================================================================
#include "player.h"
#include "spacebox.h"
#include "camera.h"
#include "input.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 spaceTexture = NULL;		// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuff=NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
D3DXMATRIX MtxSpaceWorld;					// ���[���h�}�g���b�N�X

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
// ����������
//=============================================================================
HRESULT InitSpace()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();


	// �e�N�X�`���̓ǂݍ���
	if (!spaceTexture)
	{
		D3DXCreateTextureFromFile(Device,					// �f�o�C�X�ւ̃|�C���^
			SPACE_TEXTURE_FILENAME,							// �t�@�C���̖��O
			&spaceTexture);									// �ǂݍ��ރ������[
	}


	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(Device->CreateVertexBuffer(sizeof(VERTEX_3D) * SPACE_NUM_VERTEX*MAX_SPACE,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&D3DVtxBuff,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *Vtx;


		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuff->Lock(0, 0, (void**)&Vtx, 0);

		// ���_���W�̐ݒ�
		Vtx[0].vtx = vertexinfo[0];
		Vtx[1].vtx = vertexinfo[1];
		Vtx[2].vtx = vertexinfo[2];
		Vtx[3].vtx = vertexinfo[3];

		// �@���x�N�g���̐ݒ�
		Vtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		Vtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[0].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
		Vtx[1].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
		Vtx[2].tex = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
		Vtx[3].tex = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);

		// ���_���W�̐ݒ�
		Vtx[4].vtx = vertexinfo[5];
		Vtx[5].vtx = vertexinfo[4];
		Vtx[6].vtx = vertexinfo[7];
		Vtx[7].vtx = vertexinfo[6];

		// �@���x�N�g���̐ݒ�
		Vtx[4].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[5].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[6].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[7].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		Vtx[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[6].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[7].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[4].tex = D3DXVECTOR2(3.0f / 4.0f, 1.0f / 3.0f);
		Vtx[5].tex = D3DXVECTOR2(1.0f, 1.0f / 3.0f);
		Vtx[6].tex = D3DXVECTOR2(3.0f / 4.0f, 2.0f / 3.0f);
		Vtx[7].tex = D3DXVECTOR2(1.0f, 2.0f / 3.0f);


		// ���_���W�̐ݒ�
		Vtx[8].vtx = vertexinfo[4];
		Vtx[9].vtx = vertexinfo[0];
		Vtx[10].vtx = vertexinfo[6];
		Vtx[11].vtx = vertexinfo[2];

		// �@���x�N�g���̐ݒ�
		Vtx[8].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[9].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[10].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[11].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		Vtx[8].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[9].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[10].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[11].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[8].tex = D3DXVECTOR2(0.0f, 1.0f / 3.0f);
		Vtx[9].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
		Vtx[10].tex = D3DXVECTOR2(0.0f, 2.0f / 3.0f);
		Vtx[11].tex = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);


		// ���_���W�̐ݒ�
		Vtx[12].vtx = vertexinfo[1];
		Vtx[13].vtx = vertexinfo[5];
		Vtx[14].vtx = vertexinfo[3];
		Vtx[15].vtx = vertexinfo[7];

		// �@���x�N�g���̐ݒ�
		Vtx[12].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[13].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[14].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[15].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		Vtx[12].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[13].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[14].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[15].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[12].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);
		Vtx[13].tex = D3DXVECTOR2(3.0f / 4.0f, 1.0f / 3.0f);
		Vtx[14].tex = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);
		Vtx[15].tex = D3DXVECTOR2(3.0f / 4.0f, 2.0f / 3.0f);


		// ���_���W�̐ݒ�
		Vtx[16].vtx = vertexinfo[4];
		Vtx[17].vtx = vertexinfo[5];
		Vtx[18].vtx = vertexinfo[0];
		Vtx[19].vtx = vertexinfo[1];

		// �@���x�N�g���̐ݒ�
		Vtx[16].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[17].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[18].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[19].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		Vtx[16].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[17].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[18].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[19].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[16].tex = D3DXVECTOR2(1.0f / 4.0f, 0.0f);
		Vtx[17].tex = D3DXVECTOR2(2.0f / 4.0f, 0.0f);
		Vtx[18].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f / 3.0f);
		Vtx[19].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f / 3.0f);

		// ���_���W�̐ݒ�
		Vtx[20].vtx = vertexinfo[2];
		Vtx[21].vtx = vertexinfo[3];
		Vtx[22].vtx = vertexinfo[6];
		Vtx[23].vtx = vertexinfo[7];

		// �@���x�N�g���̐ݒ�
		Vtx[20].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[21].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[22].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		Vtx[23].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		Vtx[20].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[21].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[22].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Vtx[23].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		Vtx[20].tex = D3DXVECTOR2(1.0f / 4.0f, 2.0f / 3.0f);
		Vtx[21].tex = D3DXVECTOR2(2.0f / 4.0f, 2.0f / 3.0f);
		Vtx[22].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f);
		Vtx[23].tex = D3DXVECTOR2(2.0f / 4.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuff->Unlock();
	}
	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void UninitSpace(void)
{
	if (D3DVtxBuff)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuff->Release();
		D3DVtxBuff = NULL;
	}

	if (spaceTexture)
	{// �e�N�X�`���̊J��
		spaceTexture->Release();
		spaceTexture = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSpace(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSpace(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Player *player = GetPlayer();
	D3DXMATRIX MtxRot, MtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxSpaceWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&MtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&MtxSpaceWorld, &MtxSpaceWorld, &MtxRot);

	// �ړ��𔽉f�@��Ƀv���[���Ɠ����ʒu
	D3DXMatrixTranslation(&MtxTranslate, player->pos.x, player->pos.y, player->pos.z);
	D3DXMatrixMultiply(&MtxSpaceWorld, &MtxSpaceWorld, &MtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &MtxSpaceWorld);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	Device->SetStreamSource(0, D3DVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, spaceTexture);


	for (int i = 0; i < 6; i++)
	{
		// �|���S���̕`��
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

