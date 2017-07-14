//--------------------------------------------------------------------------------
//
//�@Tutorial.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "input.h"
#include "joystick.h"
#include "tutorial.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define TUTORIAL_POS_X (-0.5f)//�|���S���̕\���ʒuX
#define TUTORIAL_POS_Y (-0.5f)//�|���S���̕\���ʒuY
#define TUTORIAL_TEXTURENAME "data/TEXTURE/����-01.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTutorial(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorial = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	if (FAILED(MakeVerTexTutorial(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexTutorial ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, TUTORIAL_TEXTURENAME, &g_pTextureTutorial)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	PlaySound(SOUND_LABEL_BGM_TUTORIAL);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitTutorial(void)
{
	//safe release
	if (g_pVtxBufferTutorial != NULL)
	{
		g_pVtxBufferTutorial->Release();
		g_pVtxBufferTutorial = NULL;
	}

	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	StopSound(SOUND_LABEL_BGM_TUTORIAL);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateTutorial(void)
{
	//key����
	if (GetKeyboardTrigger(DIK_SPACE) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_��))
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferTutorial,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexTutorial(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferTutorial,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferTutorial->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_POS_X + SCREEN_WIDTH, TUTORIAL_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_POS_X + SCREEN_WIDTH, TUTORIAL_POS_Y + SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBufferTutorial->Unlock();

	return S_OK;
}