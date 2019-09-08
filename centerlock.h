//=============================================================================
//
// �����̃��b�N�I���̏��� [center_rock.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _CENTERROCK_H_
#define _CENTERROCK_H_

//******************************************************************************
// �}�N����`
//******************************************************************************
#define	NUM_CENTERROCK				(2)			// �|���S����

#define TEXTURE_GAME_CENTERROCK		("data/TEXTURE/���b�N�I���v.png")	// �摜
#define TEXTURE_CENTERROCK_SIZE_X	(100)		// �e�N�X�`���T�C�Y
#define TEXTURE_CENTERROCK_SIZE_Y	(100/2)		// ����

#define TEXTURE_PATTERN_DIVIDE_X_CENTERROCK	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y_CENTERROCK	(1)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM_CENTERROCK			(TEXTURE_PATTERN_DIVIDE_X_CENTERROCK*TEXTURE_PATTERN_DIVIDE_Y_CENTERROCK)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION_CENTERROCK			(1)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define CENTERROCK_MAX						(1) // �ő吔

//*****************************************************************************
// �\����(�N���X)��`
//*****************************************************************************

class CenterLock
{
public:
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �|���S���̈ړ���
	D3DXVECTOR3		rot;						// �|���S���̉�]��
	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g
	
	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N

	float			Radius;						// �J�[�\���̔��a
	float			BaseAngle;					// �J�[�\���̊p�x

} ;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCenterLock(void);
void UninitCenterLock(void);
void UpdateCenterLock(void);
void DrawCenterLock(void);
bool *GetUseCenterLock(void);
CenterLock *GetCenterLock(int no);


#endif
