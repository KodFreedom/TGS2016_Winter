//--------------------------------------------------------------------------------
//
//�@Explosion.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "explosion.h"
#include "enemy.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"
#include "score.h"
#include "explosionEffect.h"


//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define EXPLOSION_NUM_MAX (50)
#define EXPLOSION_RADIUS_BOMB (50.0f)
#define EXPLOSION_RADIUS_NORMAL (100.0f)

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct
{
	bool bUse;
	int nCnt;
	D3DXVECTOR3 vPos;
	BULLET_TYPE type;
	CBodyCicle *pBody;
}EXPLOSION;

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
EXPLOSION g_aExplosion[EXPLOSION_NUM_MAX];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	for (int nCnt = 0;nCnt < EXPLOSION_NUM_MAX;nCnt++)
	{
		g_aExplosion[nCnt].bUse = false;
		g_aExplosion[nCnt].nCnt = 0;
		g_aExplosion[nCnt].type = BULLET_SAKE;
		g_aExplosion[nCnt].pBody = new CBodyCicle;
		g_aExplosion[nCnt].pBody->Init();
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitExplosion(void)
{
	for (int nCnt = 0; nCnt < EXPLOSION_NUM_MAX; nCnt++)
	{
		if (g_aExplosion[nCnt].pBody != NULL)//�A�h���X�̒��g�m�F
		{
			g_aExplosion[nCnt].pBody->Uninit();//�I������
			delete g_aExplosion[nCnt].pBody;//���g���폜
			g_aExplosion[nCnt].pBody = NULL;//�A�h���X����ɂ���
		}
	}

	//StopSound(SOUND_LABEL_BGM_Explosion);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateExplosion(void)
{
	ENEMY *pEnemy;
	for (int nCnt = 0; nCnt < EXPLOSION_NUM_MAX; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse)
		{
			for (int nCntEnemy = 0;nCntEnemy < ENEMY_NUM_MAX;nCntEnemy++)
			{
				pEnemy = GetEnemy(nCntEnemy);

				if (pEnemy->bUse)
				{
					if (CheckContactCTC(g_aExplosion[nCnt].pBody, pEnemy->pBody))
					{
						pEnemy->bUse = false;
						SetExplosion(pEnemy->pBody->cBody.GetPosCenter(), g_aExplosion[nCnt].type);

						if (g_aExplosion[nCnt].type == BULLET_BOMB)
						{
							AddScore(BOMB_SCORE);
							SetExplosionEffect(g_aExplosion[nCnt].pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_0);
						}
						else if (g_aExplosion[nCnt].type == BULLET_WOMAN)
						{
							SetExplosionEffect(g_aExplosion[nCnt].pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_1);
							AddScore(NORMAL_SCORE);
						}
						else
						{
							SetExplosionEffect(g_aExplosion[nCnt].pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_2);
							AddScore(NORMAL_SCORE);
						}
					}
				}
			}
			g_aExplosion[nCnt].bUse = false;
		}
	}
}


void SetExplosion(D3DXVECTOR3 vPos, BULLET_TYPE type)
{
	for (int nCnt = 0; nCnt < EXPLOSION_NUM_MAX; nCnt++)
	{
		if (!g_aExplosion[nCnt].bUse)
		{
			g_aExplosion[nCnt].bUse = true;
			g_aExplosion[nCnt].nCnt = 0;
			g_aExplosion[nCnt].type = type;
			g_aExplosion[nCnt].pBody->Init();
			g_aExplosion[nCnt].pBody->cBody.SetPosCenter(vPos);
			

			if (type == BULLET_BOMB)
			{
				g_aExplosion[nCnt].pBody->SetRadius(EXPLOSION_RADIUS_BOMB);
			}
			else
			{
				g_aExplosion[nCnt].pBody->SetRadius(EXPLOSION_RADIUS_NORMAL);
			}
			break;
		}
	}
}