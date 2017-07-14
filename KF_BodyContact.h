//--------------------------------------------------------------------------------
//	�Փˌv�Z�w�b�_�[
//�@KFBodyContact.h
//	Author : Xu Wenjie
//	Date   : 2016-07-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _KF_BODY_CONTACT_H_
#define _KF_BODY_CONTACT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "KF_Body.h"
//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
//-------------------------------------------
//  �~�Ɖ~
//-------------------------------------------
typedef struct {
	CBody *apBody[2];
	D3DXVECTOR3 vContactNormal;//�Փ˖@��
	float fPenetration;//�Փː[�� �}�C�i�X�ɂȂ�Ƃ͏Փ˂��Ȃ�
}KF_CONTACT;

////-------------------------------------------
////  �~�Ɖ~
////-------------------------------------------
//typedef struct {
//	CBodyCicle *apBodyCicle[2];
//	D3DXVECTOR3 vContactNormal;//�Փ˖@��
//	float fPenetration;//�Փː[�� �}�C�i�X�ɂȂ�Ƃ͏Փ˂��Ȃ�
//}KF_CONTACT_CICLE_TO_CICLE;
//
////-------------------------------------------
////  �~�Ɖ~
////-------------------------------------------
//typedef struct {
//	CBodyCicle *pBodyCicle;
//	CBodyBox *pBodyBox;
//	D3DXVECTOR3 vContactNormal;//�Փ˖@��
//	float fPenetration;//�Փː[�� �}�C�i�X�ɂȂ�Ƃ͏Փ˂��Ȃ�
//}KF_CONTACT_CICLE_TO_BOX;
//
////-------------------------------------------
////  �~�Ɖ~
////-------------------------------------------
//typedef struct {
//	CBodyBox *apBodyBox[2];
//	D3DXVECTOR3 vContactNormal;//�Փ˖@��
//	float fPenetration;//�Փː[�� �}�C�i�X�ɂȂ�Ƃ͏Փ˂��Ȃ�
//}KF_CONTACT_BOX_TO_BOX;

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
void InitBodyContact(void);
void UninitBodyContact(void);
void UpdateBodyContact(void);
void SetBodyContact(CBody *pBody1, CBody *pBody2, float fSetPenetration, D3DXVECTOR3 vSetContactNormal);
void CheckContactBTB(CBodyBox *pBody1, CBodyBox *pBody2);
bool CheckContactCTC(CBodyCicle *pBody1, CBodyCicle *pBody2);
void CheckContactCTB(CBodyCicle *pBody1, CBodyBox *pBody2);
void CheckContactCTQ(CBodyCicle *pBody1, CBodyQuadrangle *pBody2);
void AreaContact(CBody *pBody, float fRadius);
void UpdateGroundContact(CBody *pBody, float fRadius);
void UpdateLeftWallContact(CBody *pBody, float fRadius);
void UpdateRightWallContact(CBody *pBody, float fRadius);
void UpdateTopWallContact(CBody *pBody, float fRadius);
#endif