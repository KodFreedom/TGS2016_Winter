//--------------------------------------------------------------------------------
//
//�@Fade.cpp
//	Author : Xu Wenjie
//	Date   : 2016-06-07
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "fade.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define FADE_POS_X (-0.5f)//�|���S���̕\���ʒuX
#define FADE_POS_Y (-0.5f)//�|���S���̕\���ʒuY
#define FADE_RATE (1.0f / (60.0f * 0.5f))//�t�F�[�h�W��
#define FADE_TEXTURENAME "data/TEXTURE/fade.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexFade(LPDIRECT3DDEVICE9 pDevice);
void SetColorFade(D3DXCOLOR color);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFade = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
D3DXCOLOR g_colorFade;
FADE g_fade = FADE_IN;//fade mode
MODE g_modeNext = MODE_GAME;//roop mode
int g_nCnt;

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	g_nCnt = 0;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);//�^�����ɂ���

	if (FAILED(MakeVerTexFade(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexFade ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, FADE_TEXTURENAME, &g_pTextureFade)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitFade(void)
{
	//safe release
	if (g_pVtxBufferFade != NULL)
	{
		g_pVtxBufferFade->Release();
	}

	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateFade(void)
{
	if (g_fade == FADE_NONE) { return; }

	//fade���ԃJ�E���g
	if (g_nCnt != 0) 
	{
		g_nCnt -= 1;
		return;
	}

	if (g_fade == FADE_IN)
	{
		g_colorFade.a -= FADE_RATE;//alpha�����Z���Č��̉�ʂ𕂂��オ�点��

		if (g_colorFade.a <= 0.0f)
		{
			g_colorFade.a = 0.0f;
			g_fade = FADE_NONE;
		}
	}
	else if (g_fade == FADE_OUT)
	{
		g_colorFade.a += FADE_RATE;

		if (g_colorFade.a >= 1.0f)
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;
			SetMode(g_modeNext);
		}
	}

	SetColorFade(g_colorFade);
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawFade(void)
{
	if (g_fade == FADE_NONE)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferFade,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureFade);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexFade(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferFade,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferFade->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_POS_X + SCREEN_WIDTH, FADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_POS_X + SCREEN_WIDTH, FADE_POS_Y + SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//texture���_
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���z�A�h���X���
	g_pVtxBufferFade->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
void SetColorFade(D3DXCOLOR color)
{
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferFade->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = color;
	pVtx[1].color = color;
	pVtx[2].color = color;
	pVtx[3].color = color;

	//���z�A�h���X���
	g_pVtxBufferFade->Unlock();
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
void SetFade(FADE fade, MODE modeNext)
{
	if (g_fade != FADE_NONE) { return; }

	g_fade = fade;
	g_modeNext = modeNext;
	g_nCnt = 10;
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
FADE GetFade(void)
{
	return g_fade;
}