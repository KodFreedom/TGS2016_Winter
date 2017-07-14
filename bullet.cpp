//--------------------------------------------------------------------------------
//
//�@bullet.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "bullet.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "explosionEffect.h"
#include "player.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define BULLET_NUM_MAX (50)
#define BULLET_SIZE	 (60.0f)
#define BULLET_MASS	 (1.0f)
#define BULLET_SPEED (5.0f)
#define BULLET_TEXTURENAME "data/TEXTURE/bullet.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexBullet(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct
{
	BULLET_TYPE type;
	CBodyCicle *pBody;
	bool bUse;
}BULLET;

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
BULLET g_aBullet[BULLET_NUM_MAX];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	for (int nCnt = 0;nCnt < BULLET_NUM_MAX;nCnt++)
	{
		g_aBullet[nCnt].bUse = false;
		g_aBullet[nCnt].type = BULLET_SAKE;
		g_aBullet[nCnt].pBody = new CBodyCicle;
		g_aBullet[nCnt].pBody->Init();
		g_aBullet[nCnt].pBody->SetRadius(BULLET_SIZE * 0.5f);
		g_aBullet[nCnt].pBody->cBody.SetMass(BULLET_MASS);
	}

	if (FAILED(MakeVerTexBullet(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexBullet ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME, &g_pTextureBullet)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitBullet(void)
{
	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].pBody != NULL)//�A�h���X�̒��g�m�F
		{
			g_aBullet[nCnt].pBody->Uninit();//�I������
			delete g_aBullet[nCnt].pBody;//���g���폜
			g_aBullet[nCnt].pBody = NULL;//�A�h���X����ɂ���
		}
	}

	//safe release
	if (g_pVtxBufferBullet != NULL)
	{
		g_pVtxBufferBullet->Release();
		g_pVtxBufferBullet = NULL;
	}

	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//StopSound(SOUND_LABEL_BGM_Bullet);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateBullet(void)
{
	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vBulletPos;
	ENEMY *pEnemy;

	g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{
			vBulletPos = g_aBullet[nCnt].pBody->cBody.GetPosCenter();
			//pos
			pVtx[nCnt * 4 + 0].pos = vBulletPos + D3DXVECTOR3( - BULLET_SIZE * 0.5f,  - BULLET_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 1].pos = vBulletPos + D3DXVECTOR3( + BULLET_SIZE * 0.5f,  - BULLET_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 2].pos = vBulletPos + D3DXVECTOR3( - BULLET_SIZE * 0.5f,  + BULLET_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 3].pos = vBulletPos + D3DXVECTOR3( + BULLET_SIZE * 0.5f,  + BULLET_SIZE * 0.5f, 0.0f);

			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX + 1.0f / (float)BULLET_MAX, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX + 1.0f / (float)BULLET_MAX, 1.0f);

			g_aBullet[nCnt].pBody->Update();

			if (g_aBullet[nCnt].pBody->cBody.GetPosCenter().y + BULLET_SIZE * 0.5f <= 0.0f ||
				g_aBullet[nCnt].pBody->cBody.GetPosCenter().y - BULLET_SIZE * 0.5f >= GAME_HEIGHT)
			{
				g_aBullet[nCnt].bUse = false;
				g_aBullet[nCnt].pBody->Init();
				g_aBullet[nCnt].pBody->SetRadius(BULLET_SIZE * 0.5f);
				g_aBullet[nCnt].pBody->cBody.SetMass(BULLET_MASS);
			}

			AreaContact(&g_aBullet[nCnt].pBody->cBody, BULLET_SIZE * 0.5f);

			for (int nCntEnemy = 0;nCntEnemy < ENEMY_NUM_MAX;nCntEnemy++)
			{
				pEnemy = GetEnemy(nCntEnemy);

				if (pEnemy->bUse)
				{
					if (CheckContactCTC(g_aBullet[nCnt].pBody, pEnemy->pBody))
					{
						g_aBullet[nCnt].bUse = false;

						if((g_aBullet[nCnt].type == BULLET_SAKE && pEnemy->type == ENEMY_TABAKO)
							|| (g_aBullet[nCnt].type == BULLET_TABAKO && pEnemy->type == ENEMY_SAKE)
							|| (g_aBullet[nCnt].type == BULLET_WOMAN && pEnemy->type == ENEMY_MAN)
							|| g_aBullet[nCnt].type == BULLET_BOMB)
						{
							pEnemy->bUse = false;

							SetExplosion(pEnemy->pBody->cBody.GetPosCenter(), g_aBullet[nCnt].type);

							if (g_aBullet[nCnt].type == BULLET_BOMB)
							{
								AddScore(BOMB_SCORE);
								SetExplosionEffect(pEnemy->pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_0);
							}
							else if(g_aBullet[nCnt].type == BULLET_WOMAN)
							{
								AddScore(NORMAL_SCORE);
								SetExplosionEffect(pEnemy->pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_1);
							}
							else
							{
								AddScore(NORMAL_SCORE);
								SetExplosionEffect(pEnemy->pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_2);
							}
						}
						else
						{
							D3DXVECTOR3 vSpeed = pEnemy->pBody->cBody.GetVelocity();
							vSpeed *= 1.5f;
							pEnemy->pBody->cBody.SetVelocity(vSpeed);
						}
					}
				}
			}
		}
	}

	g_pVtxBufferBullet->Unlock();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferBullet,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{
			//�v���~�e�B�u�`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCnt * 4,//�I�t�Z�b�g�i���_���j
				NUM_POLYGON);
		}
	}
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * BULLET_NUM_MAX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferBullet,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferBullet->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	D3DXVECTOR3 vBulletPos;

	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		vBulletPos = g_aBullet[nCnt].pBody->cBody.GetPosCenter();
		//pos
		pVtx[nCnt * 4 + 0].pos = vBulletPos + D3DXVECTOR3(-BULLET_SIZE * 0.5f, -BULLET_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = vBulletPos + D3DXVECTOR3(+BULLET_SIZE * 0.5f, -BULLET_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = vBulletPos + D3DXVECTOR3(-BULLET_SIZE * 0.5f, +BULLET_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = vBulletPos + D3DXVECTOR3(+BULLET_SIZE * 0.5f, +BULLET_SIZE * 0.5f, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[nCnt * 4 + 0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

		//texture���_
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_aBullet[nCnt].pBody->Update();

	}

	//���z�A�h���X���
	g_pVtxBufferBullet->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �o���b�g�ݒ�
//--------------------------------------------------------------------------------
void SetBullet(BULLET_TYPE type, D3DXVECTOR3 vPos, D3DXVECTOR3 vDirection)
{
	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (!g_aBullet[nCnt].bUse)
		{
			g_aBullet[nCnt].bUse = true;

			g_aBullet[nCnt].pBody->Init();
			g_aBullet[nCnt].pBody->SetRadius(BULLET_SIZE * 0.5f);
			g_aBullet[nCnt].pBody->cBody.SetMass(BULLET_MASS);

			g_aBullet[nCnt].type = type;
			g_aBullet[nCnt].pBody->cBody.SetPosCenter(vPos);
			g_aBullet[nCnt].pBody->cBody.AddForce(vDirection * BULLET_SPEED);
			break;
		}
	}
}