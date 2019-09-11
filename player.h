//=============================================================================
//
// �v���C���[���� [player.h]
// Author :�@�����@�C�O
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER			"data/MODEL/F16_Thuderbirds1.x"	// �ǂݍ��ރ��f����
#define PLAER_TEXTURE_FILENAME	"data/TEXTURE/F16_��.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	PLAYER_RADIUS		(40.0f)						// ���a
#define	VALUE_MOVE_PLAYER	(0.25f)						// �ړ����x
#define	RATE_MOVE_PLAYER	(0.025f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.025f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.10f)						// ��]�����W��
#define	VALUE_MOVE_PLAYER_BULLET	(50.0f)				// �e�̈ړ����x
#define PLAYER_LIFEMAX		(1000)
#define PLAYER_DEAD			(0)							// �v���C���[�̎��S

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

D3DXVECTOR3 GetPositionPlayer(void);
D3DXVECTOR3 GetRotationPlayer(void);
D3DXVECTOR3 GetRotationDestPlayer(void);


#endif