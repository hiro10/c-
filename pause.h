//=============================================================================
//
// pause��ʏ��� [pause.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	TEXTURE_PAUSE_LOGO1	("data/TEXTURE/coollogo_com-26727588.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PAUSELOGO1_POS_X	(330)										// �|�[�Y���S�̕\���ʒu
#define	PAUSELOGO1_POS_Y	(350)										// �|�[�Y���S�̕\���ʒu
#define	PAUSELOGO1_POS_Z	(0)											// �|�[�Y���S�̕\���ʒu
#define	PAUSELOGO1_SIZE_X	(650)										// �|�[�Y���S�̕�
#define	PAUSELOGO1_SIZE_Y	(200)										// �|�[�Y���S�̍���

#define	TEXTURE_RETURNTITLE_LOGO1	("data/TEXTURE/�^�C�g���ɖ߂�  (1).png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RETURNTITLELOGO1_POS_X	(830)										// ���S�̕\���ʒu
#define	RETURNTITLELOGO1_POS_Y	(850)										// ���S�̕\���ʒu
#define	RETURNTITLELOGO1_POS_Z	(0)											// ���S�̕\���ʒu
#define	RETURNTITLELOGO1_SIZE_X	(250)										// ���S�̕�
#define	RETURNTITLELOGO1_SIZE_Y	(100)										// ���S�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

#endif

