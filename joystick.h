//--------------------------------------------------------------------------------
//
//�@joystick.h
//	Author : Xu Wenjie
//	Date   : 2016-09-17
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------------------
#define NUM_JOYSTICK_MAX (1)
#define PS4_CONTROLLER_NUM (0)
//#define XBOX_CONTROLLER_NUM (0)
#define STICK_MIN (50)
#define STICK_MAX (1000)

//--------------------------------------------------------------------------------
// DirectInput Joystick (PS4)
//--------------------------------------------------------------------------------
#define DIJ_PS4_��           (0)
#define DIJ_PS4_�~           (1)
#define DIJ_PS4_��           (2)
#define DIJ_PS4_��           (3)
#define DIJ_PS4_L1           (4)
#define DIJ_PS4_R1           (5)
#define DIJ_PS4_L2           (6)
#define DIJ_PS4_R2           (7)
#define DIJ_PS4_SHARE        (8)
#define DIJ_PS4_OPTIONS      (9)
#define DIJ_PS4_L3           (10)
#define DIJ_PS4_R3           (11)
#define DIJ_PS4_PS           (12)
#define DIJ_PS4_TOUCHPAD     (13)
#define DIJ_PS4_UP           (0)
#define DIJ_PS4_DOWN         (18000)
#define DIJ_PS4_LEFT         (27000)
#define DIJ_PS4_RIGHT        (9000)
#define DIJ_PS4_L_UP         (-1000)
#define DIJ_PS4_L_DOWN       (1000)
#define DIJ_PS4_L_LEFT       (-1000)
#define DIJ_PS4_L_RIGHT      (1000)
#define DIJ_PS4_R_UP         (-1000)
#define DIJ_PS4_R_DOWN       (1000)
#define DIJ_PS4_R_LEFT       (-990)
#define DIJ_PS4_R_RIGHT      (1000)

//--------------------------------------------------------------------------------
// DirectInput Joystick (XBOX ONE)
//--------------------------------------------------------------------------------
//#define DIJ_XBOX_A                (0)
//#define DIJ_XBOX_B                (1)
//#define DIJ_XBOX_X                (2)
//#define DIJ_XBOX_Y                (3)
//#define DIJ_XBOX_LB               (4)
//#define DIJ_XBOX_RB               (5)
//#define DIJ_XBOX_BACK             (6)
//#define DIJ_XBOX_MENU             (7)
//#define DIJ_XBOX_LS               (8)
//#define DIJ_XBOX_RS               (9)
//#define DIJ_XBOX_UP               (0)
//#define DIJ_XBOX_DOWN             (18000)
//#define DIJ_XBOX_LEFT             (27000)
//#define DIJ_XBOX_RIGHT            (9000)
//#define DIJ_XBOX_L_UP             (-1000)
//#define DIJ_XBOX_L_DOWN           (1000)
//#define DIJ_XBOX_L_LEFT           (-1000)
//#define DIJ_XBOX_L_RIGHT          (1000)
//#define DIJ_XBOX_R_UP             (-1000)
//#define DIJ_XBOX_R_DOWN           (1000)
//#define DIJ_XBOX_R_LEFT           (-990)
//#define DIJ_XBOX_R_RIGHT          (1000)

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
typedef enum {
	DIJ_IXDIJ_NONE,
	DIJ_BUTTON,//�{�^��
	DIJ_POV,//�����L�[
	DIJ_L_STICKX,//���X�e�B�b�NX
	DIJ_L_STICKY,//���X�e�B�b�NY
	DIJ_R_STICKX,//�E�X�e�B�b�NX
	DIJ_R_STICKY,//�E�X�e�B�b�NY
	DIJ_MAX
}DIJ;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd);
void UninitJoystick(void);
void UpdateJoystick(void);

bool GetJoystickPress(int nNumJoystick, DIJ dij, int nButton);
bool GetJoystickTrigger(int nNumJoystick, DIJ dij, int nButton);
bool GetJoystickRepeat(int nNumJoystick, int nButton);
bool GetJoystickRelease(int nNumJoystick, DIJ dij, int nButton);
int GetJoystickStickStatus(int nNumJoystick, DIJ dij);
DIJOYSTATE2 GetJoystickStatus(int nCnt);

#endif