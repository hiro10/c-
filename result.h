//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT				"data/TEXTURE/Gameover_back.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_CLEAR		"data/TEXTURE/�w�i�f��.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO			"data/TEXTURE/coollogo_com-229421742.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO_CLEAR	"data/TEXTURE/coollogo_com-303064.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TEXTURE_RESULT_LOGO_RANK	"data/TEXTURE/rank.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	TEXTURE_RESULT_RANKS	"data/TEXTURE/S.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_RANKA	"data/TEXTURE/A.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_RANKB	"data/TEXTURE/B .png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_RANKC	"data/TEXTURE/C.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define	RESULT_LOGO_POS_X	(240)		// ���U���g���S�̈ʒu(X���W)
#define	RESULT_LOGO_POS_Y	(320)		// ���U���g���S�̈ʒu(Y���W)
#define	RESULT_LOGO_WIDTH	(700)		// ���U���g���S�̕�
#define	RESULT_LOGO_HEIGHT	(240)		// ���U���g���S�̍���

// �����N�i�����j�̈ʒu
#define RANK_POS_X (100)
#define RANK_POS_Y (600)

// �����N�AS,A,B,C�̈ʒu�ƃT�C�Y
#define RANK_LOGO_POS_X (750)
#define RANK_LOGO_POS_Y (550)
#define	RANK_LOGO_WIDTH	(400)		
#define	RANK_LOGO_HEIGHT (340)		

#define	COUNT_APPERA_RESULT	(60)		// ���U���g���S�o���܂ł̑҂�����	
#define	APPERA_RESULT_TIME	(0.005f)
#define	LIMIT_COUNT_WAIT	(60 * 5)	// �҂�����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
