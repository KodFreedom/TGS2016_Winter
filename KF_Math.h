//--------------------------------------------------------------------------------
//	�������Z�p�v�Z���w�b�_�[
//�@KF_Math.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _KF_MATH_H_
#define _KF_MATH_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include <math.h>
#include "main.h"
#include "KF_Precision.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
//class CKFVector3
//{
//	//�x�N�g���v�Z
//	void operator*=(const real rValue);
//	CKFVector3 operator*(const real rValue) const;
//
//	void operator+=(const CKFVector3 &vValue);
//	CKFVector3 operator+(const CKFVector3 &vValue) const;
//
//	void operator-=(const CKFVector3 &vValue);
//	CKFVector3 operator-(const CKFVector3 &vValue) const;
//
//	void componentProductUpdate(const CKFVector3 &vVector);
//	CKFVector3 componentProduct(const CKFVector3 &vVector) const;
//
//	real scalarProduct(const CKFVector3 &vVector) const;
//
//	CKFVector3 vectorProduct3D(const CKFVector3 &vVector) const;
//	void vectorProductUpdate3D(const CKFVector3 &vVector);
//
//	real vectorProduct2D(const CKFVector3 &vVector) const;
//
//private:
//	real rPad;//3bit�̒l��4bit�ɂ��邽�߂̕ϐ�(�������x���グ�邽��)
//};

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
void Vector3Invert(D3DXVECTOR3 *pVector);
void Vector3Clear(D3DXVECTOR3 *pVector);

//�x�N�g���K�͉�
float Vector3Magnitude(D3DXVECTOR3 vVector);
float Vector3SquareMagnitude(D3DXVECTOR3 vVector);
void Vector3Normalize(D3DXVECTOR3 *pVector);
float ScalarProduct(D3DXVECTOR3 vVector1, D3DXVECTOR3 vVector2);
float Vector3Cross2D(D3DXVECTOR3 vVector1, D3DXVECTOR3 vVector2);
D3DXVECTOR3 VerticalVector3(D3DXVECTOR3 vVector);

float GetRandomFloat(float fMin, float fMax);
D3DXVECTOR3 GetRandomVec3(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);
#endif