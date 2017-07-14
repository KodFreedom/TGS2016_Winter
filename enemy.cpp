//--------------------------------------------------------------------------------
//
//�@Enemy.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "enemy.h"
#include "player.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"
#include "sound.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define ENEMY_SIZE	 (80.0f)
#define ENEMY_MASS   (1.0f)
#define ENEMY_SPEED (0.8f)
#define ENEMY_CREATE_CD (30)
#define ENEMY_TEXTURENAME "data/TEXTURE/enemy.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice);
void CreateEnemy(void);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
ENEMY g_aEnemy[ENEMY_NUM_MAX];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	for (int nCnt = 0;nCnt < ENEMY_NUM_MAX;nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;
		g_aEnemy[nCnt].type = ENEMY_SAKE;
		g_aEnemy[nCnt].pBody = new CBodyCicle;
		g_aEnemy[nCnt].pBody->Init();
		g_aEnemy[nCnt].pBody->SetRadius(ENEMY_SIZE * 0.5f);
		g_aEnemy[nCnt].pBody->cBody.SetMass(ENEMY_MASS);
	}

	if (FAILED(MakeVerTexEnemy(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexEnemy ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME, &g_pTextureEnemy)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (g_aEnemy[nCnt].pBody != NULL)//�A�h���X�̒��g�m�F
		{
			g_aEnemy[nCnt].pBody->Uninit();//�I������
			delete g_aEnemy[nCnt].pBody;//���g���폜
			g_aEnemy[nCnt].pBody = NULL;//�A�h���X����ɂ���
		}
	}

	//safe release
	if (g_pVtxBufferEnemy != NULL)
	{
		g_pVtxBufferEnemy->Release();
		g_pVtxBufferEnemy = NULL;
	}

	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

	//StopSound(SOUND_LABEL_BGM_Enemy);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	static int nCreateEnemyCnt = 0;

	if (nCreateEnemyCnt == 0)
	{
		CreateEnemy();
	}

	nCreateEnemyCnt = (nCreateEnemyCnt + 1) % ENEMY_CREATE_CD;

	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vEnemyPos;

	g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse)
		{
			vEnemyPos = g_aEnemy[nCnt].pBody->cBody.GetPosCenter();
			//pos
			pVtx[nCnt * 4 + 0].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 1].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 2].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 3].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);

			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f + 1.0f / 3.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f + 1.0f / 3.0f, 1.0f);

			g_aEnemy[nCnt].pBody->Update();

			if (g_aEnemy[nCnt].pBody->cBody.GetPosCenter().y - ENEMY_SIZE * 0.5f >= GAME_HEIGHT)
			{
				g_aEnemy[nCnt].bUse = false;
				SubPlayerLife();
				PlaySound(SOUND_LABEL_SE_DAMAGE);
			}

			AreaContact(&g_aEnemy[nCnt].pBody->cBody, ENEMY_SIZE * 0.5f);
		}
	}

	g_pVtxBufferEnemy->Unlock();

}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferEnemy,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureEnemy);

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse)
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
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * ENEMY_NUM_MAX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferEnemy,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferEnemy->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	D3DXVECTOR3 vEnemyPos;

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		vEnemyPos = g_aEnemy[nCnt].pBody->cBody.GetPosCenter();
		//pos
		pVtx[nCnt * 4 + 0].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);

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

		g_aEnemy[nCnt].pBody->Update();

	}

	//���z�A�h���X���
	g_pVtxBufferEnemy->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �o���b�g�ݒ�
//--------------------------------------------------------------------------------
void CreateEnemy(void)
{
	int nType = rand() % (int)ENEMY_MAX;
	D3DXVECTOR3 vPos = GetRandomVec3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f));
	D3DXVECTOR3 vDirection = GetRandomVec3(D3DXVECTOR3(-0.9f, 0.1f, 0.0f), D3DXVECTOR3(0.9f, 1.0f, 0.0f));
	D3DXVec3Normalize(&vDirection, &vDirection);

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (!g_aEnemy[nCnt].bUse)
		{
			g_aEnemy[nCnt].bUse = true;
			g_aEnemy[nCnt].pBody->Init();
			g_aEnemy[nCnt].pBody->SetRadius(ENEMY_SIZE * 0.5f);
			g_aEnemy[nCnt].pBody->cBody.SetMass(ENEMY_MASS);
			g_aEnemy[nCnt].type = (ENEMY_TYPE)nType;
			g_aEnemy[nCnt].pBody->cBody.SetPosCenter(vPos);
			g_aEnemy[nCnt].pBody->cBody.AddForce(vDirection * ENEMY_SPEED);
			break;
		}
	}
}

ENEMY *GetEnemy(int nID)
{
	return &g_aEnemy[nID];
}