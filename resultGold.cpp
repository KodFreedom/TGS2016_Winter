//--------------------------------------------------------------------------------
//
//�@ResultGold.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "resultGold.h"
#include "main.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define RESULT_GOLD_NUM_MAX (100)
#define RESULT_GOLD_SIZE	(80.0f)
#define RESULT_GOLD_MASS   (1.0f)
#define RESULT_GOLD_SPEED (1.0f)
#define RESULT_GOLD_CREATE_CD (20)
#define RESULT_GOLD_TEXTURENAME "data/TEXTURE/kane2.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexResultGold(LPDIRECT3DDEVICE9 pDevice);
void CreateResultGold(void);

typedef struct
{
	CBodyCicle *pBody;
	bool bUse;
}GOLD;

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureResultGold = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultGold = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
GOLD g_aResultGold[RESULT_GOLD_NUM_MAX];

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitResultGold(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	for (int nCnt = 0;nCnt < RESULT_GOLD_NUM_MAX;nCnt++)
	{
		g_aResultGold[nCnt].bUse = false;
		g_aResultGold[nCnt].pBody = new CBodyCicle;
		g_aResultGold[nCnt].pBody->Init();
		g_aResultGold[nCnt].pBody->SetRadius(RESULT_GOLD_SIZE * 0.5f);
		g_aResultGold[nCnt].pBody->cBody.SetMass(RESULT_GOLD_MASS);
	}

	if (FAILED(MakeVerTexResultGold(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexResultGold ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, RESULT_GOLD_TEXTURENAME, &g_pTextureResultGold)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitResultGold(void)
{
	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (g_aResultGold[nCnt].pBody != NULL)//�A�h���X�̒��g�m�F
		{
			g_aResultGold[nCnt].pBody->Uninit();//�I������
			delete g_aResultGold[nCnt].pBody;//���g���폜
			g_aResultGold[nCnt].pBody = NULL;//�A�h���X����ɂ���
		}
	}

	//safe release
	if (g_pVtxBufferResultGold != NULL)
	{
		g_pVtxBufferResultGold->Release();
		g_pVtxBufferResultGold = NULL;
	}

	if (g_pTextureResultGold != NULL)
	{
		g_pTextureResultGold->Release();
		g_pTextureResultGold = NULL;
	}

	//StopSound(SOUND_LABEL_BGM_ResultGold);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateResultGold(void)
{
	static int nCreateResultGoldCnt = 0;

	if (nCreateResultGoldCnt == 0)
	{
		CreateResultGold();
	}

	nCreateResultGoldCnt = (nCreateResultGoldCnt + 1) % RESULT_GOLD_CREATE_CD;

	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vResultGoldPos;

	g_pVtxBufferResultGold->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (g_aResultGold[nCnt].bUse)
		{
			vResultGoldPos = g_aResultGold[nCnt].pBody->cBody.GetPosCenter();
			//pos
			pVtx[nCnt * 4 + 0].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 1].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 2].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 3].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);

			g_aResultGold[nCnt].pBody->Update();

			if (g_aResultGold[nCnt].pBody->cBody.GetPosCenter().y - RESULT_GOLD_SIZE * 0.5f >= GAME_HEIGHT)
			{
				g_aResultGold[nCnt].bUse = false;
			}

			AreaContact(&g_aResultGold[nCnt].pBody->cBody, RESULT_GOLD_SIZE * 0.5f);
		}
	}

	g_pVtxBufferResultGold->Unlock();

}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawResultGold(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferResultGold,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultGold);

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (g_aResultGold[nCnt].bUse)
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
HRESULT MakeVerTexResultGold(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * RESULT_GOLD_NUM_MAX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferResultGold,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferResultGold->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	D3DXVECTOR3 vResultGoldPos;

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		vResultGoldPos = g_aResultGold[nCnt].pBody->cBody.GetPosCenter();
		//pos
		pVtx[nCnt * 4 + 0].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);

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

		g_aResultGold[nCnt].pBody->Update();

	}

	//���z�A�h���X���
	g_pVtxBufferResultGold->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  �o���b�g�ݒ�
//--------------------------------------------------------------------------------
void CreateResultGold(void)
{
	D3DXVECTOR3 vPos = GetRandomVec3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f));
	D3DXVECTOR3 vDirection = GetRandomVec3(D3DXVECTOR3(-0.9f, 0.1f, 0.0f), D3DXVECTOR3(0.9f, 1.0f, 0.0f));
	D3DXVec3Normalize(&vDirection, &vDirection);

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (!g_aResultGold[nCnt].bUse)
		{
			g_aResultGold[nCnt].bUse = true;
			g_aResultGold[nCnt].pBody->Init();
			g_aResultGold[nCnt].pBody->SetRadius(RESULT_GOLD_SIZE * 0.5f);
			g_aResultGold[nCnt].pBody->cBody.SetMass(RESULT_GOLD_MASS);
			g_aResultGold[nCnt].pBody->cBody.SetPosCenter(vPos);
			g_aResultGold[nCnt].pBody->cBody.AddForce(vDirection * RESULT_GOLD_SPEED);
			break;
		}
	}
}