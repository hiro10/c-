//=============================================================================
//
// ���C������ [light.cpp]
// Author : �����C�O
//
//=============================================================================
#include "light.h"
D3DLIGHT9			Light[NUM_LIGHT];	// ���C�g���

//=============================================================================
// ���C�g�̏���������
//=============================================================================
void InitLight(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXVECTOR3 VecDir;

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[0], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[0].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[0].Diffuse = D3DXCOLOR(100.0f, 100.0f, 200.0f, 100.0f);

	// ����
	Light[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	VecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[0].Direction, &VecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	Device->SetLight(0, &Light[0]);

	// ���C�g�̐ݒ�
	Device->LightEnable(0, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[1], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[1].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ����
	Light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	VecDir = D3DXVECTOR3(-0.20f, 10.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[1].Direction, &VecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	Device->SetLight(1, &Light[1]);

	// ���C�g�̐ݒ�
	Device->LightEnable(1, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[2], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[2].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[2].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// ����
	Light[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ���C�g�̕����̐ݒ�
	VecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[2].Direction, &VecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	Device->SetLight(2, &Light[2]);

	// ���C�g�̐ݒ�
	Device->LightEnable(2, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[3], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[3].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[3].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// ����
	Light[3].Ambient = D3DXCOLOR(900.3f, 200.3f, 0.3f, 1.0f);

	// ���C�g�̕����̐ݒ�
	VecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[2].Direction, &VecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	Device->SetLight(3, &Light[3]);

	// ���C�g�̐ݒ�
	Device->LightEnable(3, TRUE);

	// ���C�e�B���O���[�h
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);


}
//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}
//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{

}
//=============================================================================
// ���C�g�̕`�揈��
//=============================================================================
void DrawLight(void)
{

}