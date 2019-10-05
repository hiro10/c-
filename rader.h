//=============================================================================
//
// ���[�_�[���� [radar.h]
// Author :�����C�O 
//
//=============================================================================
#ifndef _RADAR_H_
#define _RADAR_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RADAR_NORMAL		"data/TEXTURE/radar.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_POINT_NORMAL		"data/TEXTURE/bullet.png"		// �ǂݍ��ރe�N�X�`���t�@�C����

#define TIME_ANIMATION_RADAR		(4)
#define TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X	(1)
#define TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y	(1)
#define ANIM_PATTERN_RADAR						(TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X*TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y)
#define	RADAR_POS_X								(140.0f)						// ���[�_�[�̕\���ʒu
#define	RADAR_POS_Y								(130.0f)						// ���[�_�[�̕\���ʒu
#define	RADAR_SIZE_X							(80)													// ���[�_�[�̕�
#define	RADAR_SIZE_Y							(80)													// ���[�_�[�̍���
#define	POINT_SIZE_X							(2)														// �G�\���̑傫��
#define	POINT_SIZE_Y							(2)														// �G�\���̑傫��
#define	POINT_SIZE_UP							(9)														// �G�\���̑傫��
#define	POINT_SIZE_MISSILE						(3)														// �G�\���̑傫��
#define	POINT_SIZE_MIDDLE						(6)														// �G�\���̑傫��
#define	POINT_SIZE_DOWN							(3)														// �G�\���̑傫��
#define	POINT_HEIGHT							(30.0f)													// �G�\���͈̔�
#define	RADAR_DISTANCE							(3000.0f)												// �G�\���̍ő勗��
#define RADAR_MAX								(1)														// ���[�_�[�̐�
#define POINT_MAX								(1024)													// �\���ł���ő�l


//*****************************************************************************
// �N���X��`
//*****************************************************************************

// ���[�_�[
class Rader
{
public:
	PROJECT2D	s_polygon;
	bool		s_bUse;
};

// �}�[�J�[
class Maker
{
public:
	PROJECT2D	s_polygon;
	bool		s_bUse;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRadar(void);
void UninitRadar(void);
void UpdateRadar(void);
Rader* GetRadar(int Ano);
Maker* GetMaker(int Ano);
void DrawRadar();

#endif
#pragma once
