#//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author :�����C�O
//
//=============================================================================
#ifndef _MPGAUGE_H_
#define _MPGAUGE_H_

// �}�N����`
#define TEXTURE_GAME_MPGAUGE00		("data/TEXTURE/mp.png")				// �T���v���p�摜
#define TEXTURE_GAME_MPGAUGEFRAME00	("data/TEXTURE/�Q�[�W�J�o�[2 (1).png")	// �T���v���p�摜


#define MPGAUGE00_SIZE_X			(200)	// �e�N�X�`���T�C�Y
#define MPGAUGE00_SIZE_Y			(20)	// ����
#define MPGAUGE00_POS_X			(10)	// �|���S���̏����ʒuX(����)
#define MPGAUGE00_POS_Y			(900)	// ����

#define MPGAUGEFRAME00_SIZE_X			(220)	// �e�N�X�`���T�C�Y
#define MPGAUGEFRAME00_SIZE_Y			(35)	// ����
#define MPGAUGEFRAME00_POS_X			(7)	// �|���S���̏����ʒuX(����)
#define MPGAUGEFRAME00_POS_Y			(890)	// ����


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMpGauge(void);
void UninitMpGauge(void);
void UpdateMpGauge(void);
void DrawMpGauge(void);

#endif

