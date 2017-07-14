//--------------------------------------------------------------------------------
//
//�@UIGame.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "UIgame.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define UI_GAME_POS_X (0.0f)//�|���S���̕\���ʒuX
#define UI_GAME_POS_Y (0.0f)//�|���S���̕\���ʒuY
#define UI_GAME_TEXTURENAME "data/TEXTURE/gameUI.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIGame(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIGame = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIGame = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

													//--------------------------------------------------------------------------------
													//  ����������
													//--------------------------------------------------------------------------------
void InitUIGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	if (FAILED(MakeVerTexUIGame(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexUIGame ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, UI_GAME_TEXTURENAME, &g_pTextureUIGame)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitUIGame(void)
{
	//safe release
	if (g_pVtxBufferUIGame != NULL)
	{
		g_pVtxBufferUIGame->Release();
		g_pVtxBufferUIGame = NULL;
	}

	if (g_pTextureUIGame != NULL)
	{
		g_pTextureUIGame->Release();
		g_pTextureUIGame = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateUIGame(void)
{
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawUIGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferUIGame,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureUIGame);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIGame(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferUIGame,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferUIGame->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(UI_GAME_POS_X, UI_GAME_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_GAME_POS_X + SCREEN_WIDTH, UI_GAME_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(UI_GAME_POS_X, UI_GAME_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_GAME_POS_X + SCREEN_WIDTH, UI_GAME_POS_Y + SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBufferUIGame->Unlock();

	return S_OK;
}