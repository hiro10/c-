//=============================================================================
//
// ���[�_�[���� [Radar.cpp]
// Author : �����C�O
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "rader.h"
#include "enemy.h"
#include "ace.h"
#include "boss.h"
#include "player.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexRadar(void);
void SetTextureRadar(int pno, int cntPattern);	// 
void SetTextureMaker(int pno, int cntPattern);
void SetVertexRadar(int pno);					// ���_�̌v�Z����
void SetVertexMaker(int pno);
void SetPoint(D3DXVECTOR3 pos, int rno, int type);
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
float GetDistance(D3DXVECTOR3 speed);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		D3DTextureRadar = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DTEXTURE9		D3DTexturePoint = NULL;		// �e�N�X�`���ւ̃|�C���^

Rader							radar[RADAR_MAX];
Maker							maker[POINT_MAX];

float							g_fRadiusRadar;					// �|���S���̔��a
float							g_fBaseAngleRadar;					// �|���S���̊p�x

//=============================================================================
// ����������
//=============================================================================
HRESULT InitRadar()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Rader *radar = NULL;
	Maker *maker = NULL;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(Device,		// �f�o�C�X�̃|�C���^
		TEXTURE_RADAR_NORMAL,				// �t�@�C���̖��O
		&D3DTextureRadar);					// �ǂݍ��ރ������̃|�C���^

	D3DXCreateTextureFromFile(Device,		// �f�o�C�X�̃|�C���^
		TEXTURE_POINT_NORMAL,				// �t�@�C���̖��O
		&D3DTexturePoint);					// �ǂݍ��ރ������̃|�C���^


	//���[�_�[�̕���
	for (int i = 0; i < RADAR_MAX; i++)
	{
		radar = GetRadar(i);
		radar->s_polygon.s_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		radar->s_polygon.s_pos = D3DXVECTOR3(RADAR_POS_X, RADAR_POS_Y, 0.0f);
		radar->s_polygon.s_CountAnim = 0;
		radar->s_polygon.s_PatternAnim = 0;
		radar->s_bUse = true;

		radar->s_polygon.s_size.x = RADAR_SIZE_X;
		radar->s_polygon.s_size.y = RADAR_SIZE_Y;

		D3DXVECTOR2 temp = D3DXVECTOR2(RADAR_SIZE_X, RADAR_SIZE_Y);
		radar->s_polygon.s_Radius = D3DXVec2Length(&temp);					// �G�l�~�[�̔��a��������
		radar->s_polygon.s_Angle = atan2f(RADAR_SIZE_Y, RADAR_SIZE_X);	// �G�l�~�[�̊p�x��������

		radar->s_polygon.s_texture = D3DTextureRadar;					// �e�N�X�`�����
	}

	// �}�[�J�[����
	for (int i = 0; i < POINT_MAX; i++)
	{
		maker = GetMaker(i);
		maker->s_polygon.s_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		maker->s_polygon.s_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		maker->s_polygon.s_CountAnim = 0;
		maker->s_polygon.s_PatternAnim = 0;
		maker->s_bUse = false;

		maker->s_polygon.s_size.x = POINT_SIZE_X;
		maker->s_polygon.s_size.y = POINT_SIZE_Y;


		D3DXVECTOR2 temp = D3DXVECTOR2(POINT_SIZE_X, POINT_SIZE_Y);
		maker->s_polygon.s_Radius = D3DXVec2Length(&temp);					// �G�l�~�[�̔��a��������
		maker->s_polygon.s_Angle = atan2f(POINT_SIZE_Y, POINT_SIZE_X);	// �G�l�~�[�̊p�x��������

		maker->s_polygon.s_texture = D3DTexturePoint;					// �e�N�X�`�����
	}

	// ���_���̍쐬
	MakeVertexRadar();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitRadar(void)
{
	if (D3DTextureRadar != NULL)
	{	// �e�N�X�`���̊J��
		D3DTextureRadar->Release();
		D3DTextureRadar = NULL;
	}

	if (D3DTexturePoint != NULL)
	{	// �e�N�X�`���̊J��
		D3DTexturePoint->Release();
		D3DTexturePoint = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateRadar(void)
{
	Rader *radar = GetRadar(0);
	Player* player = GetPlayer();
	Boss*boss = GetBoss();
	Ace*ace = GetAce();
	Enemy* enemy = GetEnemy(0);

	for (int i = 0; i < RADAR_MAX; i++)
	{
		// �A�j���[�V����
		radar->s_polygon.s_CountAnim++;
		// �A�j���[�V����Wait�`�F�b�N
		if ((radar->s_polygon.s_CountAnim % TIME_ANIMATION_RADAR) == 0)
		{
			// �p�^�[���̐؂�ւ�
			radar->s_polygon.s_PatternAnim = (radar->s_polygon.s_PatternAnim + 1) % ANIM_PATTERN_RADAR;

			// �e�N�X�`�����W��ݒ�
			SetTextureRadar(i, radar->s_polygon.s_PatternAnim);
		}
		SetVertexRadar(i);	// ���_�̌v�Z����

	}

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		enemy = GetEnemy(i);
		if (enemy->use == true)
		{
			if (GetDistance(player->pos, enemy->pos) <= RADAR_DISTANCE)
			{
				D3DXVECTOR3 posInRadar = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 dis = enemy->pos - player->pos;
				float angle = atan2f(dis.x, dis.z);
				float distance = GetDistance(D3DXVECTOR3(dis.x, 0.0f, dis.z));
				posInRadar.x = distance * sinf(angle - player->rot.y);
				posInRadar.z = distance * cosf(angle - player->rot.y);
				SetPoint(posInRadar, 0, ENEMYTYPE_MAX);
			}
		}
	}

	// ace��true�Ȃ�
	if (ace->use == true)
	{
		if (GetDistance(player->pos, ace->pos) <= RADAR_DISTANCE)
		{
			D3DXVECTOR3 posInRadar = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 dis = ace->pos - player->pos;
			float angle = atan2f(dis.x, dis.z);
			float distance = GetDistance(D3DXVECTOR3(dis.x, 0.0f, dis.z));
			posInRadar.x = distance * sinf(angle - player->rot.y);
			posInRadar.z = distance * cosf(angle - player->rot.y);
			SetPoint(posInRadar, 0, ENEMYTYPE_MAX);
		}
	}

	// boss��true�Ȃ�
	if (boss->use == true)
	{
		if (GetDistance(player->pos, boss->pos) <= RADAR_DISTANCE)
		{
			D3DXVECTOR3 posInRadar = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 dis = boss->pos - player->pos;
			float angle = atan2f(dis.x, dis.z);
			float distance = GetDistance(D3DXVECTOR3(dis.x, 0.0f, dis.z));
			posInRadar.x = distance * sinf(angle - player->rot.y);
			posInRadar.z = distance * cosf(angle - player->rot.y);
			SetPoint(posInRadar, 0, ENEMYTYPE_MAX);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawRadar(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	Rader *radar = NULL;
	Maker *maker = NULL;

	for (int i = 0; i < RADAR_MAX; i++)
	{
		// ���[�_�[�̎擾
		radar = GetRadar(i);

		// ���[�_�[��true�Ȃ�
		if (radar->s_bUse)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, radar->s_polygon.s_texture);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, radar->s_polygon.s_vertexWk, sizeof(VERTEX_2D));
		}
	}

	for (int i = 0; i < POINT_MAX; i++)
	{
		// �}�[�J�[�̎擾
		maker = GetMaker(i);

		if (maker->s_bUse == true)
		{
			// ���_�t�H�[�}�b�g�̐ݒ�
			Device->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			Device->SetTexture(0, maker->s_polygon.s_texture);

			// �|���S���̕`��
			Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, maker->s_polygon.s_vertexWk, sizeof(VERTEX_2D));

			// �}�[�J�[�𖢎g�p��
			maker->s_bUse = false;
		}

	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexRadar(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	Rader *radar = NULL;
	Maker *maker = NULL;

	for (int i = 0; i < RADAR_MAX; i++)
	{
		// ���[�_�[�̎擾
		radar = GetRadar(i);
		
		// ���_���W�̐ݒ�	
		SetVertexRadar(i);

		// rhw�̐ݒ�
		radar->s_polygon.s_vertexWk[0].rhw =
			radar->s_polygon.s_vertexWk[1].rhw =
			radar->s_polygon.s_vertexWk[2].rhw =
			radar->s_polygon.s_vertexWk[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		radar->s_polygon.s_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		radar->s_polygon.s_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		radar->s_polygon.s_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		radar->s_polygon.s_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		SetTextureRadar(i, radar->s_polygon.s_PatternAnim);
	}

	for (int i = 0; i < POINT_MAX; i++)
	{
		maker = GetMaker(i);
		// ���_�̌v�Z����

		// ���_���W�̐ݒ�	
		SetVertexMaker(i);

		// rhw�̐ݒ�
		maker->s_polygon.s_vertexWk[0].rhw =
			maker->s_polygon.s_vertexWk[1].rhw =
			maker->s_polygon.s_vertexWk[2].rhw =
			maker->s_polygon.s_vertexWk[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		maker->s_polygon.s_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		maker->s_polygon.s_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		maker->s_polygon.s_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		maker->s_polygon.s_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		SetTextureMaker(i, maker->s_polygon.s_PatternAnim);
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�i���[�_�[�����j
//=============================================================================
void SetTextureRadar(int pno, int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	Rader *radar = GetRadar(pno);

	int x = cntPattern % TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y;

	radar->s_polygon.s_vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	radar->s_polygon.s_vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	radar->s_polygon.s_vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	radar->s_polygon.s_vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�i�}�[�J�[�����j
//=============================================================================
void SetTextureMaker(int pno, int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	Maker *maker = GetMaker(pno);

	int x = cntPattern % TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_RADAR_TYPE01_PATTERN_DIVIDE_Y;

	maker->s_polygon.s_vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	maker->s_polygon.s_vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	maker->s_polygon.s_vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	maker->s_polygon.s_vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�i���[�_�[�����j
//=============================================================================
void SetVertexRadar(int pno)
{
	Rader *radar = GetRadar(pno);

	// ���_���W�̐ݒ�
	radar->s_polygon.s_vertexWk[0].vtx.x = radar->s_polygon.s_pos.x - cosf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[0].vtx.y = radar->s_polygon.s_pos.y - sinf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[0].vtx.z = 0.0f;

	radar->s_polygon.s_vertexWk[1].vtx.x = radar->s_polygon.s_pos.x + cosf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[1].vtx.y = radar->s_polygon.s_pos.y - sinf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[1].vtx.z = 0.0f;

	radar->s_polygon.s_vertexWk[2].vtx.x = radar->s_polygon.s_pos.x - cosf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[2].vtx.y = radar->s_polygon.s_pos.y + sinf(radar->s_polygon.s_Angle - radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[2].vtx.z = 0.0f;

	radar->s_polygon.s_vertexWk[3].vtx.x = radar->s_polygon.s_pos.x + cosf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[3].vtx.y = radar->s_polygon.s_pos.y + sinf(radar->s_polygon.s_Angle + radar->s_polygon.s_rot.z) * radar->s_polygon.s_Radius;
	radar->s_polygon.s_vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// ���_���W�̐ݒ�i�}�[�J�[�����j
//=============================================================================
void SetVertexMaker(int pno)
{
	Maker *maker = GetMaker(pno);

	// ���_���W�̐ݒ�
	maker->s_polygon.s_vertexWk[0].vtx.x = maker->s_polygon.s_pos.x - cosf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[0].vtx.y = maker->s_polygon.s_pos.y - sinf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[0].vtx.z = 0.0f;

	maker->s_polygon.s_vertexWk[1].vtx.x = maker->s_polygon.s_pos.x + cosf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[1].vtx.y = maker->s_polygon.s_pos.y - sinf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[1].vtx.z = 0.0f;

	maker->s_polygon.s_vertexWk[2].vtx.x = maker->s_polygon.s_pos.x - cosf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[2].vtx.y = maker->s_polygon.s_pos.y + sinf(maker->s_polygon.s_Angle - maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[2].vtx.z = 0.0f;

	maker->s_polygon.s_vertexWk[3].vtx.x = maker->s_polygon.s_pos.x + cosf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[3].vtx.y = maker->s_polygon.s_pos.y + sinf(maker->s_polygon.s_Angle + maker->s_polygon.s_rot.z) * maker->s_polygon.s_Radius;
	maker->s_polygon.s_vertexWk[3].vtx.z = 0.0f;
}

//=============================================================================
// SetPoint����
//=============================================================================
void SetPoint(D3DXVECTOR3 pos, int rno, int type)
{
	// �}�[�J�[�̎擾
	Maker* maker = GetMaker(0);

	// ���[�_�[�̎擾
	Rader* radar = GetRadar(rno);

	// �\���ł���ő�l���J��Ԃ�
	for (int i = 0; i < POINT_MAX; i++)
	{
		maker = GetMaker(i);

		// �}�[�J�[�����g�p�Ȃ�
		if (maker->s_bUse == false)
		{
			maker->s_polygon.s_pos.x = radar->s_polygon.s_pos.x - pos.x*RADAR_SIZE_X / RADAR_DISTANCE;
			maker->s_polygon.s_pos.y = radar->s_polygon.s_pos.y + pos.z*RADAR_SIZE_Y / RADAR_DISTANCE;
			if (type == ENEMYTYPE_MAX)
			{
				// ���ˌ��̐ݒ�
				maker->s_polygon.s_vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
				maker->s_polygon.s_Radius = POINT_SIZE_MIDDLE;

			}

			// �}�[�J�[���g�p��
			maker->s_bUse = true;

			// ���_�̌v�Z����
			SetVertexMaker(i);
			return;
		}
	}
}

//=============================================================================
// ��̈ʒu�̋������擾
// pos1: �ڕW�ʒu1 pos2: �ڕW�ʒu2
//=============================================================================
float GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(pos1.x - pos2.x, pos1.y - pos2.y, pos1.z - pos2.z);
	return D3DXVec3Length(&temp);
}

//=============================================================================
// �������擾
// speed: �O�̎��̋���
//=============================================================================
float GetDistance(D3DXVECTOR3 speed)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(speed.x, speed.y, speed.z);
	return D3DXVec3Length(&temp);
}

//=============================================================================
// ���[�_�[�̎擾
//=============================================================================

Rader* GetRadar(int pno)
{
	return &radar[pno];
}

//=============================================================================
// ���[�_�[�ɕ\������}�[�J�[���擾
//=============================================================================
Maker* GetMaker(int pno)
{
	return &maker[pno];
}
