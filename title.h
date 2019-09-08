//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TITLE		"data/TEXTURE/128428312238816125082.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_TITLE_LOGO	"data/TEXTURE/�^�C�g��.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_LOGO_START	"data/TEXTURE/coollogo_com-1171249.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TITLE_LOGO_POS_X		(120)		// �^�C�g�����S�̈ʒu(X���W)
#define	TITLE_LOGO_POS_Y		(40)		// �^�C�g�����S�̈ʒu(Y���W)
#define	TITLE_LOGO_WIDTH		(1080)		// �^�C�g�����S�̕�
#define	TITLE_LOGO_HEIGHT		(640)		// �^�C�g�����S�̍���

#define	START_POS_X				(400)		// �X�^�[�g�{�^���̈ʒu(X���W)
#define	START_POS_Y				(720)		// �X�^�[�g�{�^���̈ʒu(Y���W)
#define	START_WIDTH				(480)		// �X�^�[�g�{�^���̕�
#define	START_HEIGHT			(120)		// �X�^�[�g�{�^���̍���

#define	COUNT_APPERA_START		(60)		// �X�^�[�g�{�^���o���܂ł̎���
#define	INTERVAL_DISP_START		(60)		// �X�^�[�g�{�^���_�ł̎���

#define	COUNT_WAIT_DEMO			(60 * 5)	// �f���܂ł̑҂�����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
