//=============================================================================
//
// pause��ʏ��� [pause.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	TEXTURE_UI_LOGO1	("data/TEXTURE/�G�@�ڋ�.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	UILOGO1_POS_X	(470)										// �|�[�Y���S�̕\���ʒu
#define	UILOGO1_POS_Y	(5)										// �|�[�Y���S�̕\���ʒu
#define	UILOGO1_POS_Z	(0)											// �|�[�Y���S�̕\���ʒu
#define	UILOGO1_SIZE_X	(400)										// �|�[�Y���S�̕�
#define	UILOGO1_SIZE_Y	(100)										// �|�[�Y���S�̍���

#define	TEXTURE_UIACE_LOGO	("data/TEXTURE/�G�[�X�o��.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	UIACELOGO1_POS_X	(470)										// �|�[�Y���S�̕\���ʒu
#define	UIACELOGO1_POS_Y	(5)										// �|�[�Y���S�̕\���ʒu
#define	UIACELOGO1_POS_Z	(0)											// �|�[�Y���S�̕\���ʒu
#define	UIACELOGO1_SIZE_X	(500)										// �|�[�Y���S�̕�
#define	UIACELOGO1_SIZE_Y	(150)										// �|�[�Y���S�̍���

#define	TEXTURE_UIITEM_LOGO	("data/TEXTURE/�^�[�Q�b�g.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	UIITEMLOGO1_POS_X	(240)										// �|�[�Y���S�̕\���ʒu
#define	UIITEMLOGO1_POS_Y	(150)										// �|�[�Y���S�̕\���ʒu
#define	UIITEMLOGO1_POS_Z	(0)											// �|�[�Y���S�̕\���ʒu
#define	UIITEMLOGO1_SIZE_X	(900)										// �|�[�Y���S�̕�
#define	UIITEMLOGO1_SIZE_Y	(300)										// �|�[�Y���S�̍���

#define	TEXTURE_UI2_LOGO	("data/TEXTURE/sign-1513493_960_720 (2).png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	UILOGO2_POS_X	(0)										// �|�[�Y���S�̕\���ʒu
#define	UILOGO2_POS_Y	(0)										// �|�[�Y���S�̕\���ʒu
#define UILOGO2_POS_Z	(0)											// �|�[�Y���S�̕\���ʒu
#define	UILOGO2_SIZE_X	(SCREEN_WIDTH)										// �|�[�Y���S�̕�
#define	UILOGO2_SIZE_Y	(SCREEN_HEIGHT)										// �|�[�Y���S�̍���


#define MODE_GAME_COUNT		(30)
#define MODE_BOSS_COUNT		(10)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUi(void);
void UninitUi(void);
void UpdateUi(void);
void DrawUi(void);

#endif
