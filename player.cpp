//--------------------------------------------------------------------------------
//
//�@player.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "input.h"
#include "joystick.h"
#include "bullet.h"
#include "sound.h"
#include "fade.h"
#include "UI_bullet.h"
#include "UI_life.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define PLAYER_POS_X (GAME_WIDTH * 0.5f)//�|���S���̕\���ʒuX
#define PLAYER_POS_Y (GAME_HEIGHT)//�|���S���̕\���ʒuY
#define PLAYER_ROT_SPEED (0.05f)
#define PLAYER_WIDTH	 (100.0f)
#define PLAYER_HEIGHT	 (50.0f)
#define PLAYER_ROT_MIN	(-D3DX_PI * 0.85f)
#define PLAYER_ROT_MAX	(-D3DX_PI * 0.15f)
#define PLAYER_LIFE (9)
#define PLAYER_TEXTURENAME "data/TEXTURE/player.png"//�t�@�C����

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vVecToPos[4];
	//D3DXVECTOR3 vOrighin;	//���ˈʒu
	float fRot;
	BULLET_TYPE type;
	int nLife;
}PLAYER;

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBG = NULL;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
PLAYER g_player;

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	g_player.vPos = D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0.0f);
	g_player.fRot = -D3DX_PI * 0.5f;
	g_player.vVecToPos[0] = D3DXVECTOR3(0.0f, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[1] = D3DXVECTOR3(PLAYER_WIDTH * 0.5f, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[2] = D3DXVECTOR3(0.0f, PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[3] = D3DXVECTOR3(PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.type = BULLET_BOMB;
	g_player.nLife = PLAYER_LIFE;
	ChangeBulletType(g_player.type);

	if (FAILED(MakeVerTexPlayer(pDevice)))//texture�|�C���^�ւ̃|�C���^
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURENAME, &g_pTexturePlayer)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/haikei.jpg", &g_pTextureBG)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitPlayer(void)
{
	//safe release
	if (g_pVtxBufferPlayer != NULL)
	{
		g_pVtxBufferPlayer->Release();
		g_pVtxBufferPlayer = NULL;
	}

	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	if ((GetKeyboardPress(DIK_A) || GetJoystickStickStatus(PS4_CONTROLLER_NUM, DIJ_L_STICKX) < -STICK_MIN) && g_player.fRot > PLAYER_ROT_MIN)
	{
		g_player.fRot -= PLAYER_ROT_SPEED;
	}
	if ((GetKeyboardPress(DIK_D) || GetJoystickStickStatus(PS4_CONTROLLER_NUM, DIJ_L_STICKX) > STICK_MIN) && g_player.fRot < PLAYER_ROT_MAX)
	{
		g_player.fRot += PLAYER_ROT_SPEED;
	}
	if (GetKeyboardTrigger(DIK_Q) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_L1))
	{
		g_player.type = (BULLET_TYPE)((int)g_player.type - 1);
		if (g_player.type == (BULLET_TYPE)(BULLET_BOMB - 1))
		{
			g_player.type = BULLET_WOMAN;
		}
		ChangeBulletType(g_player.type);
		PlaySound(SOUND_LABEL_SE_CHANGE);
	}
	if (GetKeyboardTrigger(DIK_E) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_R1))
	{
		g_player.type = (BULLET_TYPE)((int)g_player.type + 1);
		if (g_player.type == BULLET_MAX)
		{
			g_player.type = BULLET_BOMB;
		}
		ChangeBulletType(g_player.type);
		PlaySound(SOUND_LABEL_SE_CHANGE);
	}
	//key����
	if (GetKeyboardTrigger(DIK_SPACE) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_��))
	{
		D3DXVECTOR3 vDirection;

		vDirection = D3DXVECTOR3(cosf(g_player.fRot), sinf(g_player.fRot), 0.0f);
		SetBullet(g_player.type, g_player.vPos, vDirection);
		PlaySound(SOUND_LABEL_SE_SHOT);
		//SetFade(FADE_OUT, MODE_TUTORIAL);
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferPlayer->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);

	for (int nCnt = 0;nCnt < 4;nCnt++)
	{
		D3DXVECTOR3 vVertex = g_player.vVecToPos[nCnt];

		//AFFIN�]��
		vVertex = D3DXVECTOR3(
			g_player.vVecToPos[nCnt].x * cosf(g_player.fRot) - g_player.vVecToPos[nCnt].y * sinf(g_player.fRot),
			g_player.vVecToPos[nCnt].x * sinf(g_player.fRot) + g_player.vVecToPos[nCnt].y * cosf(g_player.fRot),
			0.0f);

		pVtx[nCnt].pos = g_player.vPos + vVertex;
	}

	g_pVtxBufferPlayer->Unlock();

	SetLife(g_player.nLife);
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferBG,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTextureBG);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);



	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		g_pVtxBufferPlayer,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//Texture�̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);

	
}

//--------------------------------------------------------------------------------
//  ���_�̍쐬
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferPlayer,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferPlayer->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);


	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].color = D3DCOLOR_RGBA(0, 0, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(0, 0, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(0, 0, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(0, 0, 255, 255);

	//texture���_
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���z�A�h���X���
	g_pVtxBufferPlayer->Unlock();

	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&g_pVtxBufferBG,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	g_pVtxBufferBG->Lock(
		0,//�͈�
		0,//�͈�
		(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
		0);


	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, GAME_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAME_WIDTH, GAME_HEIGHT, 0.0f);

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
	g_pVtxBufferBG->Unlock();

	return S_OK;
}

void SubPlayerLife(void)
{
	g_player.nLife--;

	if (g_player.nLife == 0)
	{
		SetFade(FADE_OUT, MODE_GAMEOVER);
	}
}