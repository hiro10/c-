//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : �����C�O
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "gameobject.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ENEMY				(80)						// �G�l�~�[�ő吔
#define	VALUE_ROTATE_ENEMY		(D3DX_PI * 0.025f)			// ��]���x
#define VALUE_MOVE_ENEMY		(0.5f)						// �e�̈ړ����x
#define	ENEMY_RADIUS				(100.0f)				// ���a
#define ENEMY_TEXTURE_FILENAME  "data/TEXTURE/UFO.bmp"	    // �ǂݍ��ރe�N�X�`���t�@�C����
#define ENEMY_DAMAGE			(25)

//**************************************
// ���
//**************************************
enum
{
	ENEMYTYPE_ENEMY = 0,	// �G�l�~�[
	ENEMYTYPE_ENEMY2,
	ENEMYTYPE_ENEMY3,
	ENEMYTYPE_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Type = ENEMYTYPE_ENEMY);
void DeleteEnemy(int IdxEnemy);
Enemy *GetEnemy(int IdxEnemy);
D3DXVECTOR3 GetPositionEnemy(int IdxEnemy);
D3DXVECTOR3 GetSclEnemy(int IdxEnemy);

#endif
