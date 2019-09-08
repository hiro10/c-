//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author :�����C�O
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_GAUGE00		("data/TEXTURE/HP�f��.png")				// �T���v���p�摜
#define TEXTURE_GAME_GAUGEFRAME00	("data/TEXTURE/�Q�[�W�J�o�[2 (1).png")	// �T���v���p�摜
#define GAUGE00_SIZE_X			(350)	// �e�N�X�`���T�C�Y
#define GAUGE00_SIZE_Y			(35)	// ����
#define GAUGE00_POS_X			(10)	// �|���S���̏����ʒuX(����)
#define GAUGE00_POS_Y			(850)	// ����
#define GAUGEFRAME00_SIZE_X			(GAUGE00_SIZE_X + GAUGE00_SIZE_X / 5)	// �e�N�X�`���T�C�Y
#define GAUGEFRAME00_SIZE_Y			(GAUGE00_SIZE_Y + GAUGE00_SIZE_Y / 3)	// ����
#define GAUGEFRAME00_POS_X			(GAUGE00_POS_X - GAUGE00_SIZE_X / 25)	// �|���S���̏����ʒuX(����)
#define GAUGEFRAME00_POS_Y			(GAUGE00_POS_Y - GAUGE00_SIZE_Y / 3)	// ����

//HP�\�L�p
#define	TEXTURE_HP	("data/TEXTURE/coollogo_com-63881504.png")			// �ǂݍ��ރe�N�X�`���t�@�C����
#define	HP_POS_X	(20)												// �^�C�g�����S�̕\���ʒu
#define	HP_POS_Y	(800)												// �^�C�g�����S�̕\���ʒu                         
#define	HP_POS_Z	(0)													// �^�C�g�����S�̕\���ʒu
#define	HP_SIZE_X	(60)												// �^�C�g�����S�̕�
#define	HP_SIZE_Y	(50)												// �^�C�g�����S�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

#endif

