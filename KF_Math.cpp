//--------------------------------------------------------------------------------
//	�������Z�p�v�Z���w�b�_�[
//�@KF_Math.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "KF_Math.h"
#include <time.h>

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �����]��
//--------------------------------------------------------------------------------
void Vector3Invert(D3DXVECTOR3 *pVector)
{
	pVector->x = -pVector->x;
	pVector->y = -pVector->y;
	pVector->z = -pVector->z;
}

//--------------------------------------------------------------------------------
//  �l������������
//--------------------------------------------------------------------------------
void Vector3Clear(D3DXVECTOR3 *pVector)
{
	pVector->x = 0.0f;
	pVector->y = 0.0f;
	pVector->z = 0.0f;
}

//--------------------------------------------------------------------------------
//  �x�N�g���̖�
//--------------------------------------------------------------------------------
float Vector3Magnitude(D3DXVECTOR3 vVector)
{
	return sqrtf(vVector.x * vVector.x + vVector.y * vVector.y + vVector.z * vVector.z);
}

//--------------------------------------------------------------------------------
//  �x�N�g���̖�(����)
//--------------------------------------------------------------------------------
float Vector3SquareMagnitude(D3DXVECTOR3 vVector)
{
	return (vVector.x * vVector.x + vVector.y * vVector.y + vVector.z * vVector.z);
}

//--------------------------------------------------------------------------------
//  �K�͉�
//--------------------------------------------------------------------------------
void Vector3Normalize(D3DXVECTOR3 *pVector)
{
	float fWork = Vector3Magnitude(*pVector);

	if (fWork > 0)
	{
		//(*this) *= ((real)rWork) / rWork;
		pVector->x /= fWork;
		pVector->y /= fWork;
		pVector->z /= fWork;
	}
}

//--------------------------------------------------------------------------------
//  �x�N�g���ƃx�N�g���̏�Z(���ʂ̓X�J��)(����)
//--------------------------------------------------------------------------------
float ScalarProduct(D3DXVECTOR3 vVector1, D3DXVECTOR3 vVector2)
{
	return (vVector1.x * vVector2.x + vVector1.y * vVector2.y + vVector1.z * vVector2.z);
}

//--------------------------------------------------------------------------------
//  �x�N�g���ƃx�N�g���̏�Z(���ʂ̓X�J��)(2D�O��)
//--------------------------------------------------------------------------------
float Vector3Cross2D(D3DXVECTOR3 vVector1, D3DXVECTOR3 vVector2)
{
	return (vVector1.x * vVector2.y - vVector1.y * vVector2.x);
}

//--------------------------------------------------------------------------------
//  �x�N�g���Ƃ̐����̃x�N�g���̌v�Z
//--------------------------------------------------------------------------------
D3DXVECTOR3 VerticalVector3(D3DXVECTOR3 vVector)
{
	vVector = D3DXVECTOR3(
		cosf(D3DX_PI * 0.5f) * vVector.x - sinf(D3DX_PI * 0.5f) * vVector.y,
		sinf(D3DX_PI * 0.5f) * vVector.x + cosf(D3DX_PI * 0.5f) * vVector.y,
		0.0f);

	D3DXVec3Normalize(&vVector,&vVector);

	return vVector;
}

float GetRandomFloat(float fMin, float fMax)
{
	if (fMin >= fMax)
	{
		return fMin;
	}

	float fWork = (rand() % 10000) * 0.0001f;

	fWork = fWork * (fMax - fMin) + fMin;

	return fWork;
}

D3DXVECTOR3 GetRandomVec3(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
{
	D3DXVECTOR3 vWork;

	vWork.x = GetRandomFloat(vMin.x, vMax.x);
	vWork.y = GetRandomFloat(vMin.y, vMax.y);
	vWork.z = GetRandomFloat(vMin.z, vMax.z);

	return vWork;
}
//
////--------------------------------------------------------------------------------
////  �X�J���ƃx�N�g���̏�Z
////--------------------------------------------------------------------------------
//void CKFVector3::operator*=(const real rValue)
//{
//	m_rX *= rValue;
//	m_rY *= rValue;
//	m_rZ *= rValue;
//}
//
//CKFVector3 CKFVector3::operator*(const real rValue) const
//{
//	return CKFVector3(m_rX * rValue, m_rY * rValue, m_rZ * rValue);
//}
//
////--------------------------------------------------------------------------------
////  �x�N�g���ƃx�N�g���̉��Z
////--------------------------------------------------------------------------------
//void CKFVector3::operator+=(const CKFVector3 &vValue)
//{
//	m_rX += vValue.m_rX;
//	m_rY += vValue.m_rY;
//	m_rZ += vValue.m_rZ;
//}
//
//CKFVector3 CKFVector3::operator+(const CKFVector3 &vValue) const
//{
//	return CKFVector3(m_rX + vValue.m_rX, m_rY + vValue.m_rY, m_rZ + vValue.m_rZ);
//}
//
////--------------------------------------------------------------------------------
////  �x�N�g���ƃx�N�g���̌��Z
////--------------------------------------------------------------------------------
//void CKFVector3::operator-=(const CKFVector3 &vValue)
//{
//	m_rX -= vValue.m_rX;
//	m_rY -= vValue.m_rY;
//	m_rZ -= vValue.m_rZ;
//}
//
//CKFVector3 CKFVector3::operator-(const CKFVector3 &vValue) const
//{
//	return CKFVector3(m_rX - vValue.m_rX, m_rY - vValue.m_rY, m_rZ - vValue.m_rZ);
//}
//
////--------------------------------------------------------------------------------
////  �x�N�g���ƃx�N�g���̊e���̏�Z
////--------------------------------------------------------------------------------
//void CKFVector3::componentProductUpdate(const CKFVector3 &vVector)
//{
//	m_rX *= vVector.m_rX;
//	m_rY *= vVector.m_rY;
//	m_rZ *= vVector.m_rZ;
//}
//
//CKFVector3 CKFVector3::componentProduct(const CKFVector3 &vVector) const
//{
//	return CKFVector3(m_rX * vVector.m_rX, m_rY * vVector.m_rY, m_rZ * vVector.m_rZ);
//}
//

////--------------------------------------------------------------------------------
////  �x�N�g���ƃx�N�g���̏�Z(���ʂ̓x�N�g��)(3D�O��)
////--------------------------------------------------------------------------------
//void CKFVector3::vectorProductUpdate3D(const CKFVector3 &vVector)
//{
//	m_rX = m_rY * vVector.m_rZ - m_rZ * vVector.m_rY;
//	m_rY = m_rZ * vVector.m_rX - m_rX * vVector.m_rZ;
//	m_rZ = m_rX * vVector.m_rY - m_rY * vVector.m_rX;
//}
//
//CKFVector3 CKFVector3::vectorProduct3D(const CKFVector3 &vVector) const
//{
//	return CKFVector3(m_rY * vVector.m_rZ - m_rZ * vVector.m_rY,
//		m_rZ * vVector.m_rX - m_rX * vVector.m_rZ,
//		m_rX * vVector.m_rY - m_rY * vVector.m_rX);
//}
//
////--------------------------------------------------------------------------------
////  �x�N�g���ƃx�N�g���̏�Z(���ʂ̓X�J��)(2D�O��)
////--------------------------------------------------------------------------------
//real CKFVector3::vectorProduct2D(const CKFVector3 &vVector) const
//{
//	return (m_rX * vVector.m_rY - m_rY * vVector.m_rX);
//}