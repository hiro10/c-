//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,			// BGM0
	SOUND_LABEL_BGM001,				// BGM1
	SOUND_LABEL_SE_SHOT,			// �e���ˉ�
	SOUND_LABEL_SE_EXPLOSION,		// ������1
	SOUND_LABEL_SE_EXPLOSION2,		// ������2
	SOUND_LABEL_SE_ENTER,			// �G���^�[�I��
	SOUND_LABEL_SE_FALSE,			// �~�b�V�������s��
	SOUND_LABEL_SE_TRUE,			// �~�b�V����������
	SOUND_LABEL_SE_ROCK,			// ���b�N��
	SOUND_LABEL_SE_CANCEL,			// �L�����Z����
	SOUND_LABEL_SE_ENEMYEXPLOSION,	// �G�l�~�[�̔�����

	SOUND_LABEL_MAX,
} SOUND_LABEL;

enum
{	// �Đ��p�t���O
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
