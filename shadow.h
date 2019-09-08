//=============================================================================
//
// �e���� [shadow.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SHADOW_SIZE_X		(50.0f)							// �e�̕�
#define	SHADOW_SIZE_Z		(50.0f)							// �e�̍���

#define	MAX_SHADOW			(1280)							// �e�ő吔

//*****************************************************************************
// �\����(�N���X)��`
//*****************************************************************************
class  SHADOW
{
public:
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	bool use;				// �g�p���Ă��邩�ǂ���
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, float SizeX, float SizeY, float SizeZ);
void DeleteShadow(int IdxShadow);
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos);
void SetVertexShadow(int IdxShadow,int s, float SizeX,float SizeY, float SizeZ);
void SetColorShadow(int IdxShadow, D3DXCOLOR col);

#endif
