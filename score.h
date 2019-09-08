//=============================================================================
//
// �X�R�A���� [score.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_FRAME_SCORE	"data/TEXTURE/frame_score.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SCORE_SIZE_X		(35.0f)							// �X�R�A�̐����̕�
#define	SCORE_SIZE_Y		(50.0f)							// �X�R�A�̐����̍���
#define	SCORE_INTERVAL_X	(0.0f)							// �X�R�A�̐����̕\���Ԋu

#define	NUM_SCORE			(8)								// �X�R�A�̌���

#define	SCORE_POS_X			(SCREEN_WIDTH - (SCORE_INTERVAL_X + SCORE_SIZE_X) * NUM_SCORE - 20.0f)	// �X�R�A�̕\����ʒu�w���W
#define	SCORE_POS_Y			(60.0f)																	// �X�R�A�̕\����ʒu�x���W


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void ChangeScore(int value);

#endif
