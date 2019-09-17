//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Kurosawa Yuta
//
//=============================================================================
#ifndef _EFFEKSEER_H_
#define _EFFEKSEER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_EFFEKSEER				(3000)							// �G�t�F�N�g�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	int			ID;
	int			EffectType;
	bool		Use;		// �g�p���Ă��邩�ǂ���
} EFFEKSEER;

typedef struct
{
	EffekseerRendererDX9::Renderer		*Render;
	Effekseer::Manager					*Manager;
	Effekseer::Effect					**Effect;
	int									EffectNum;
}EFFECTCONTROLLER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEffekseer(int type);
void UninitEffekseer(void);
void UpdateEffekseer(void);
void DrawEffekseer(void);
void SetEffekseer(D3DXVECTOR3 pos, int effectType);
//EFFECT *GetEffekseer(int efno);
int SetEffekseer(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nTimer);
HRESULT SafeLoadEffekseer(Effekseer::Manager *Manager, Effekseer::Effect **Effect, const EFK_CHAR* SrcPath, const char* ErrorSrc);

#endif
