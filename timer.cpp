//--------------------------------------------------------------------------------
//
//�@timer
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "timer.h"
#include "main.h"
#include "fade.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define TIMER_TEXTURENAME "data/TEXTURE/num.png"//�t�@�C����
#define TIMER_POS_X (GAME_WIDTH * 0.8f - 0.5f)
#define TIMER_POS_Y (0.0f - 0.5f)
#define TIMER_WIDTH (36.0f * 1.5f)
#define TIMER_HEIGHT (64.0f * 1.5f)
#define TIMER_SPACE (8.0f)//�����Ԃ̒���
#define TIMER_MAX (99 * 60)
#define DEGITAL_MAX (3)//����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTimer(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTimer = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
int g_nTimer;

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	g_nTimer = TIMER_MAX;

	if (FAILED(MakeVerTexTimer(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexTimer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		TIMER_TEXTURENAME,
		&g_pTextureTimer)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitTimer(void)
{
	if (g_pVtxBufferTimer != NULL)
	{
		g_pVtxBufferTimer->Release();
		g_pVtxBufferTimer = NULL;
	}

	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateTimer(void)
{
	VERTEX_2D *pVtx = NULL;
	int nCnt;
	int nWork = g_nTimer / 60;

	g_pVtxBufferTimer->Lock(0, 0, (void**)&pVtx, 0);

	//texture�v�Z
	for (nCnt = DEGITAL_MAX - 1;nCnt >= 0;nCnt--)
	{
		pVtx[nCnt * 4].tex = D3DXVECTOR2(0.1f * (float)(nWork % 10), 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(0.1f * (float)(nWork % 10) + 0.1f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.1f * (float)(nWork % 10), 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(0.1f * (float)(nWork % 10) + 0.1f, 1.0f);
		nWork /= 10;
	}
	g_pVtxBufferTimer->Unlock();

	g_nTimer--;

	//���Ԑ؂�
	if (g_nTimer == 0)
	{
		SetFade(FADE_OUT, MODE_GAMEOVER);
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾
	int nCnt;

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferTimer,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimer);

	for (nCnt = 0;nCnt < DEGITAL_MAX;nCnt++)
	{
		//�v���~�e�B�u�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCnt * 4,//�I�t�Z�b�g�i���_���j
			NUM_POLYGON);
	}
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	int nCnt;
	//
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * DEGITAL_MAX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferTimer,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTimer->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	for (nCnt = 0;nCnt < DEGITAL_MAX;nCnt++)
	{
		//���_���W�̐ݒ�i2D���W�A�E���j
		pVtx[nCnt * 4].pos = D3DXVECTOR3(TIMER_POS_X + (TIMER_WIDTH + TIMER_SPACE) * nCnt, TIMER_POS_Y, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(TIMER_POS_X + (TIMER_WIDTH + TIMER_SPACE) * nCnt + TIMER_WIDTH, TIMER_POS_Y, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(TIMER_POS_X + (TIMER_WIDTH + TIMER_SPACE) * nCnt, TIMER_POS_Y + TIMER_HEIGHT, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(TIMER_POS_X + (TIMER_WIDTH + TIMER_SPACE) * nCnt + TIMER_WIDTH, TIMER_POS_Y + TIMER_HEIGHT, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[nCnt * 4].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[nCnt * 4].color = D3DCOLOR_RGBA(221, 221, 0, 255);
		pVtx[nCnt * 4 + 1].color = D3DCOLOR_RGBA(221, 221, 0, 255);
		pVtx[nCnt * 4 + 2].color = D3DCOLOR_RGBA(221, 221, 0, 255);
		pVtx[nCnt * 4 + 3].color = D3DCOLOR_RGBA(221, 221, 0, 255);

		//texture���_
		pVtx[nCnt * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	//���z�A�h���X���
	g_pVtxBufferTimer->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  ���Ԏ擾
//--------------------------------------------------------------------------------
int GetTimer(void)
{
	return g_nTimer / 60;
}