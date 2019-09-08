//=============================================================================
//
// �J�������� [camera.h]
// Author : �����@�C�O
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	GAME_MODE_POS_CAMERA_P_X		(0.0f)								// �J�������_�̏����ʒu(X���W)
#define	GAME_MODE_POS_CAMERA_P_Y		(925.0f)							// �J�������_�̏����ʒu(Y���W)
#define	GAME_MODE_POS_CAMERA_P_Z		(-100.0f)							// �J�������_�̏����ʒu(Z���W)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z			(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(100000.0f)										// �r���[���ʂ�FarZ
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)								// ��]��
#define CAMERA_CMK			(100.0f)										// �����_�܂ł̋���
#define CAMERA_LOCK			(170.0f)										// ���b�N�I�����̒����_�܂ł̋���
#define FIRST_CAMERA_POS_LIMIT (-30.0f)
#define BOSS_CAMERA_POS_LIMIT (100.0f)
//*****************************************************************************
// �\����(�N���X)��`
//*****************************************************************************

 class  Camera
 {
public:
	D3DXVECTOR3		PosCameraP;				// �J�����̎��_
	D3DXVECTOR3		PosCameraR;				// �J�����̒����_
	D3DXVECTOR3		PosCameraU;				// �J�����̏����
	D3DXMATRIX		MtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX		MtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3		Player_To_Target;
	D3DXVECTOR3		Unitvec_Player_To_Target;
	D3DXVECTOR3		RotCameraP;				// �J�����̎��_
	float			Cam_len;				// ���_���王�_�܂ł̋���
	bool			LockOnTrigger;			// ���b�N�I���̎g�p�A���g�p����
	
	// �J������z���͈͎̔w��֐�
	void			CameraAriaLimit(float z1,float z2);
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float distance);
void DrawCamera(void);
D3DXVECTOR3 GetRotCamera(void);
D3DXMATRIX GetMtxView(void);
Camera *GetCamerawk(void);

#endif
