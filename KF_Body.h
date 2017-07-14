//--------------------------------------------------------------------------------
//	�Փ˕��w�b�_�[
//�@KF_Body.h
//	Author : Xu Wenjie
//	Date   : 2016-07-25
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _KF_BODY_H_
#define _KF_BODY_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "KF_Precision.h"
#include "KF_Math.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
//#define DAMPING_MAX ((float)0.0)//�ő��R��
//#define DAMPING_MIN ((float)0.995)//�ŏ���R��

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
//class CBody
//{
//public:
//	typedef struct {
//		D3DXVECTOR3 vVertexPos;
//		D3DXVECTOR3 vLengthToCenter;
//		D3DXVECTOR3 vDirection;
//	}KF_VERTEX;
//
//	CBody();//�R���X�g���N�^
//
//	//���z�֐�
//	virtual ~CBody();//�f�X�g���N�^
//	virtual void Init(void);
//	virtual void Update(void);
//	virtual void Uninit(void);
//
//	//�����o�[�ϐ�
//	void SetPosCenter(D3DXVECTOR3 vSetPosCenter);
//	D3DXVECTOR3 GetPosCenter(void);
//	void SetGravityCenter(D3DXVECTOR3 vGravityCenter);
//	D3DXVECTOR3 GetGravityCenter(void);
//	void SetVelocity(D3DXVECTOR3 vSetVelocity);
//	D3DXVECTOR3 GetVelocity(void);
//	void SetAcceleration(D3DXVECTOR3 vSetAcceleration);
//	D3DXVECTOR3 GetAcceleration(void);
//	void SetMass(float fMass);
//	float GetMass(void);
//	void SetGravity(D3DXVECTOR3 vSetGravity);
//	void SetDrag(float fSetK1, float fSetK2);
//	void ClearAccumulator(void);//���t���[����p�͂��N���A����
//	void AddForce(D3DXVECTOR3 vForce);//��p�͂𑝉�����
//
//	//��p�͌v�Z
//	void UpdateGravity(void);
//	void UpdateDrag(void);
//private:
//	D3DXVECTOR3 m_vPositionCenter;//���S�ʒu
//	D3DXVECTOR3 m_vGravityCenter;//�d�S�ʒu
//	D3DXVECTOR3 m_vVelocity;//���x
//	D3DXVECTOR3 m_vAcceleration;//���q�̉����x
//	D3DXVECTOR3 m_vGravity;//�d��
//	D3DXVECTOR3 m_vForceAccum;//���킹����p��
//	float m_fInverseMass;//���ʂ̋t��
//	float m_fK1;//��R�͌W��(���x)
//	float m_fK2;//��R�͌W��(�������x)
//};

class CBody
{
public:
	CBody();//�R���X�g���N�^
	~CBody();//�f�X�g���N�^
	void Init(void);
	void Update(void);
	void Uninit(void);

	//�����o�[�ϐ�
	void SetPosCenter(D3DXVECTOR3 vSetPosCenter);
	D3DXVECTOR3 GetPosCenter(void);
	void SetGravityCenter(D3DXVECTOR3 vGravityCenter);
	D3DXVECTOR3 GetGravityCenter(void);
	void SetVelocity(D3DXVECTOR3 vSetVelocity);
	D3DXVECTOR3 GetVelocity(void);
	void SetMovement(D3DXVECTOR3 vSetMovement);
	D3DXVECTOR3 GetMovement(void);
	void SetAcceleration(D3DXVECTOR3 vSetAcceleration);
	D3DXVECTOR3 GetAcceleration(void);
	void SetMass(float fMass);
	float GetMass(void);
	void SetInverseMass(float fInverseMass);
	float GetInverseMass(void);
	void SetGravity(D3DXVECTOR3 vSetGravity);
	void SetDamping(float fSetDamping);
	void SetRotation(float fSetRotation);
	float GetRotation(void);
	//void SetDrag(float fSetK1, float fSetK2);
	void ClearAccumulator(void);//���t���[����p�͂��N���A����
	void AddForce(D3DXVECTOR3 vForce);//��p�͂𑝉�����

	//��p�͌v�Z
	void UpdateGravity(void);
	//void UpdateDrag(void);
private:
	D3DXVECTOR3 m_vPositionCenter;//���S�ʒu
	D3DXVECTOR3 m_vGravityCenter;//�d�S�ʒu
	D3DXVECTOR3 m_vVelocity;//���x
	D3DXVECTOR3 m_vMovement;//1frane�̈ړ���
	D3DXVECTOR3 m_vAcceleration;//���q�̉����x
	D3DXVECTOR3 m_vGravity;//�d��
	D3DXVECTOR3 m_vForceAccum;//���킹����p��
	float m_fInverseMass;//���ʂ̋t��
	float m_fRotation;
	float m_fDamping;
	//float m_fK1;//��R�͌W��(���x)
	//float m_fK2;//��R�͌W��(�������x)
};

class CBodyCicle
{
public:
	CBodyCicle();//�R���X�g���N�^
	~CBodyCicle();//�f�X�g���N�^

	void Init(void);
	void Update(void);
	void Uninit(void);

	void SetRadius(float fSetRadius);
	float GetRadius(void);

	CBody cBody;
private:
	float m_fRadius;
};

class CBodyBox
{
public:
	CBodyBox();//�R���X�g���N�^
	~CBodyBox();//�f�X�g���N�^

	void Init(void);
	void Update(void);
	void Uninit(void);

	void SetHalfSize(D3DXVECTOR2 vSetHalfSize);
	D3DXVECTOR2 GetHalfSize(void);
	D3DXVECTOR3 GetVertex(int nNumVertex);
	D3DXVECTOR3 GetSide(int nNumSide);
	void SetAngle(float fAngle);
	float GetAngle(void);
	float GetLength(void);

	CBody cBody;
private:
	D3DXVECTOR2 m_vHalfSize;//
	D3DXVECTOR3 m_aVertex[4];
	D3DXVECTOR3 m_aSide[4];
	float m_fAngle;
	float m_fLength;

	void InitVertex(void);
	void UpdateVertex(void);
	void UpdateSide(void);
};

class CBodyQuadrangle
{
public:
	CBodyQuadrangle();//�R���X�g���N�^
	~CBodyQuadrangle();//�f�X�g���N�^

	void Init(void);
	void Update(void);
	void Uninit(void);

	void SetVertex(int nNumVertex, D3DXVECTOR3 vSetVertex);
	D3DXVECTOR3 GetVertex(int nNumVertex);
	D3DXVECTOR3 GetSide(int nNumSide);

	CBody cBody;
private:
	D3DXVECTOR3 m_aVertex[4];
	D3DXVECTOR3 m_aSide[4];

	void UpdateSide(void);
};

//class CBodyCicle : public CBody
//{
//public:
//	CBodyCicle();//�R���X�g���N�^
//	~CBodyCicle();//�f�X�g���N�^
//
//	void Init(void);
//	void Update(void);
//	void Uninit(void);
//
//	void SetRadius(float fSetRadius);
//	float GetRadius(void);
//private:
//	float m_fRadius;
//};
//
//class CBodyBox : public CBody
//{
//public:
//	CBodyBox();//�R���X�g���N�^
//	~CBodyBox();//�f�X�g���N�^
//
//	void Init(void);
//	void Update(void);
//	void Uninit(void);
//
//	void SetHalfSize(D3DXVECTOR2 vSetHalfSize);
//	D3DXVECTOR2 GetHalfSize(void);
//	void SetRotation(float fSetRotation);
//	float GetRotation(void);
//private:
//	D3DXVECTOR2 m_vHalfSize;//
//	float m_fRotation;
//	KF_VERTEX m_aVertex[4];
//
//	void InitVertex(void);
//	void UpdateVertex(void);
//};

//class CBodyTriangle : public CBody
//{
//public:
//	CBodyTriangle();//�R���X�g���N�^
//	~CBodyTriangle();//�f�X�g���N�^
//
//	void Init(void);
//	void Update(void);
//	void Uninit(void);
//
//	void SetHalfSize(D3DXVECTOR2 vSetHalfSize);
//
//private:
//	D3DXVECTOR2 vHalfSize;//
//	float fAngle;
//	KF_VERTEX aVertex[3];
//};

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
#endif