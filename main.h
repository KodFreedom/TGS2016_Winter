//--------------------------------------------------------------------------------
//
//�@main.h
//	Author : Xu Wenjie
//	Date   : 2016-12-02
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include <Windows.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <XAudio2.h>//sound
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define SCREEN_WIDTH (1280)//�E�C���h�E��
#define SCREEN_HEIGHT (720)//�E�C���h�E����
#define GAME_WIDTH (980)
#define GAME_HEIGHT (600)
#define NUM_POLYGON (2)//�|���S����
#define NUM_VERTEX (4)//���_��
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)//���_�t�H�[�}�b�g
#define TIMER_INTERVAL (1000/60)//�X�V�Ԋu

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef struct//���_���\���́@��̒��_�t�H�[�}�b�g�ƍ\�������킹�邱��
{
	D3DXVECTOR3 pos;//xyz
	float rhw;//rhw
	D3DCOLOR color;//color
	D3DXVECTOR2 tex;//uv
}VERTEX_2D;

typedef enum//��ʑJ�ڂ̃��[�h�̗�
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_GAMEOVER,
	MODE_MAX
}MODE;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif