//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_TIMER	"data/TEXTURE/frame_timer.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TIMER_SIZE_X		(35.0f)							// �^�C�}�[�̐����̕�
#define	TIMER_SIZE_Y		(50.0f)							// �^�C�}�[�̐����̍���
#define	TIMER_INTERVAL_X	(0.0f)							// �^�C�}�[�̐����̕\���Ԋu

#define	NUM_TIMER (3)										// �^�C�}�[�̌���

#define	TIMER_POS_X			(1150)							// �^�C�}�[�̕\����ʒuX���W
#define	TIMER_POS_Y			(145)						    // �^�C�}�[�̕\����ʒuY���W																// �^�C�}�[�̕\����ʒu�x���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void GetEnableTimer(bool Enable);
void ResetTimer(int Time = 999);


#endif
