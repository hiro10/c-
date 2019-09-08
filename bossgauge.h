//=============================================================================
//
// �Q�[�W���� [bossgauge.h]
// Author :�����C�O
//
//=============================================================================
#ifndef _BOSSGAUGE_H_
#define _BOSSGAUGE_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_GAME_BOSSGAUGE00		("data/TEXTURE/HP�f�ރ{�X�p.png")		// �T���v���p�摜
#define TEXTURE_GAME_BOSSGAUGEFRAME00	("data/TEXTURE/�Q�[�W�J�o�[BOSS.png")	// �T���v���p�摜
#define BOSSGAUGE00_SIZE_X			(350)	// �e�N�X�`���T�C�Y
#define BOSSGAUGE00_SIZE_Y			(30)	// ����
#define BOSSGAUGE00_POS_X			(40)	// �|���S���̏����ʒuX(����)
#define BOSSGAUGE00_POS_Y			(30)	// ����

#define BOSSGAUGEFRAME00_SIZE_X			(1000)	// �e�N�X�`���T�C�Y
#define BOSSGAUGEFRAME00_SIZE_Y			(BOSSGAUGE00_SIZE_Y + BOSSGAUGE00_SIZE_Y / 3)	// ����
#define BOSSGAUGEFRAME00_POS_X			(0)		// �|���S���̏����ʒuX(����)
#define BOSSGAUGEFRAME00_POS_Y			(BOSSGAUGE00_POS_Y - BOSSGAUGE00_SIZE_Y / 3)	// ����

//HP�\�L�p
#define	TEXTURE_BOSSHP1	("data/TEXTURE/boss_hp�f��.png")	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	BOSSHP1_POS_X	(200)							    // �^�C�g�����S�̕\���ʒu
#define	BOSSHP1_POS_Y	(-10)								// �^�C�g�����S�̕\���ʒu                         
#define	BOSSHP1_POS_Z	(0)									// �^�C�g�����S�̕\���ʒu
#define	BOSSHP1_SIZE_X	(100)								// �^�C�g�����S�̕�
#define	BOSSHP1_SIZE_Y	(50)								// �^�C�g�����S�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBossGauge(void);
void UninitBossGauge(void);
void UpdateBossGauge(void);
void DrawBossGauge(void);

#endif





