//--------------------------------------------------------------------------------
//	�Փ˕��w�b�_�[
//�@KF_Body.h
//	Author : Xu Wenjie
//	Date   : 2016-07-25
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "KF_Body.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  Body
//--------------------------------------------------------------------------------
//---------------------------------------
//  �R���X�g���N�^
//---------------------------------------
CBody::CBody()
{
	m_fInverseMass = 0.0f;
	m_fDamping = 0.0f;
	m_fRotation = 0.0f;
	//m_fK1 = 0.0f;
	//m_fK2 = 0.0f;
	m_vAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vForceAccum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravityCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vPositionCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//---------------------------------------
//  �f�X�g���N�^
//---------------------------------------
CBody::~CBody()
{
	m_fInverseMass = 0.0f;
	m_fDamping = 0.0f;
	m_fRotation = 0.0f;
	//m_fK1 = 0.0f;
	//m_fK2 = 0.0f;
	m_vAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vForceAccum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravityCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vPositionCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//---------------------------------------
//  ������
//---------------------------------------
void CBody::Init(void)
{
	m_fInverseMass = 0.0f;
	m_fDamping = DAMPING;
	m_fRotation = 0.0f;
	//m_fK1 = 0.0f;
	//m_fK2 = 0.0f;
	m_vAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vForceAccum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravityCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vPositionCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//---------------------------------------
//  �I������
//---------------------------------------
void CBody::Uninit(void)
{
	m_fInverseMass = 0.0f;
	m_fDamping = 0.0f;
	m_fRotation = 0.0f;
	//m_fK1 = 0.0f;
	//m_fK2 = 0.0f;
	m_vAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vForceAccum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vGravityCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vPositionCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//---------------------------------------
//  �X�V
//---------------------------------------
void CBody::Update(void)
{
	m_vAcceleration = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	UpdateGravity();

	//�v�Z�ς݉����x
	//D3DXVECTOR3 vResultAcc = m_vAcceleration;

	//�ʒu�X�V
	m_vPositionCenter += m_vVelocity;
	m_vMovement += m_vVelocity;

	//�͂ɂ���ĉ����x���X�V
	m_vAcceleration += m_vForceAccum * m_fInverseMass;

	//���x�X�V
	m_vVelocity += m_vAcceleration;

	//��R��
	m_vVelocity *= m_fDamping;

	ClearAccumulator();
}

//---------------------------------------
//  ���S�ݒ�
//---------------------------------------
void CBody::SetPosCenter(D3DXVECTOR3 vSetPosCenter)
{
	m_vPositionCenter = vSetPosCenter;
}

//---------------------------------------
//  ���S�擾
//---------------------------------------
D3DXVECTOR3 CBody::GetPosCenter(void)
{
	return m_vPositionCenter;
}

//---------------------------------------
//  �d�S�ݒ�
//---------------------------------------
void CBody::SetGravityCenter(D3DXVECTOR3 vGravityCenter)
{
	m_vGravityCenter = vGravityCenter;
}

//---------------------------------------
//  �d�S�擾
//---------------------------------------
D3DXVECTOR3 CBody::GetGravityCenter(void)
{
	return m_vGravityCenter;
}

//---------------------------------------
//  ���x�ݒ�
//---------------------------------------
void CBody::SetVelocity(D3DXVECTOR3 vSetVelocity)
{
	m_vVelocity = vSetVelocity;
}

//---------------------------------------
//  ���x�擾
//---------------------------------------
D3DXVECTOR3 CBody::GetVelocity(void)
{
	return m_vVelocity;
}

//---------------------------------------
//  �ړ��ʎ擾
//---------------------------------------
void CBody::SetMovement(D3DXVECTOR3 vSetMovement)
{
	m_vMovement = vSetMovement;
}

//---------------------------------------
//  �ړ��ʎ擾
//---------------------------------------
D3DXVECTOR3 CBody::GetMovement(void)
{
	return m_vMovement;
}

//---------------------------------------
//  �����x�ݒ�
//---------------------------------------
void CBody::SetAcceleration(D3DXVECTOR3 vSetAcceleration)
{
	m_vAcceleration = vSetAcceleration;
}

//---------------------------------------
//  �����x�擾
//---------------------------------------
D3DXVECTOR3 CBody::GetAcceleration(void)
{
	return m_vAcceleration;
}

//---------------------------------------
//  ���ʐݒ�
//---------------------------------------
void CBody::SetMass(float fMass)
{
	if (fMass == 0.0f) { return; }
	m_fInverseMass = 1.0f / fMass;
}

//---------------------------------------
//  ���ʎ擾
//---------------------------------------
float CBody::GetMass(void)
{
	if (m_fInverseMass == 0.0f) { return FLOAT_MAX; }
	return (1.0f / m_fInverseMass);
}

//---------------------------------------
//  �t���ʐݒ�
//---------------------------------------
void CBody::SetInverseMass(float fInverseMass)
{
	m_fInverseMass = fInverseMass;
}

//---------------------------------------
//  �t���ʎ擾
//---------------------------------------
float CBody::GetInverseMass(void)
{
	return m_fInverseMass;
}

//---------------------------------------
//  �d�͐ݒ�
//---------------------------------------
void CBody::SetGravity(D3DXVECTOR3 vSetGravity)
{
	m_vGravity = vSetGravity;
}

//---------------------------------------
//  ��R�͌W���ݒ�
//---------------------------------------
void CBody::SetDamping(float fSetDamping)
{
	m_fDamping = fSetDamping;
}

//---------------------------------------
//  �p�x�ݒ�
//---------------------------------------
void CBody::SetRotation(float fSetRotation)
{
	m_fRotation = fSetRotation;
}

//---------------------------------------
//  �p�x�擾
//---------------------------------------
float CBody::GetRotation(void)
{
	return m_fRotation;
}
//void CBody::SetDrag(float fSetK1, float fSetK2)
//{
//	m_fK1 = fSetK1;
//	m_fK2 = fSetK2;
//}

//---------------------------------------
//  ��p�͂��N���A����
//---------------------------------------
void CBody::ClearAccumulator(void)
{
	m_vForceAccum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//---------------------------------------
//  ��p�͂𑫂�
//---------------------------------------
void CBody::AddForce(D3DXVECTOR3 vForce)
{
	m_vForceAccum += vForce;
}

//---------------------------------------
//  �d�͍X�V
//---------------------------------------
void CBody::UpdateGravity(void)
{
	AddForce(m_vGravity * GetMass() * DURATION);
}

////---------------------------------------
////  ��R�͍X�V
////---------------------------------------
//void CBody::UpdateDrag(void)
//{
//	D3DXVECTOR3 vForce = GetVelocity();
//
//	//��R�͌W���v�Z
//	float fDragCoeff = Vector3Magnitude(vForce);//���x�̃X�J���l
//	fDragCoeff = m_fK1 * fDragCoeff + m_fK2 * fDragCoeff * fDragCoeff;
//
//	//��R�͌v�Z
//	Vector3Normalize(&vForce);
//	vForce *= -fDragCoeff;
//
//	//��R�͂���p�͂ɉ��Z
//	AddForce(vForce);
//}

//--------------------------------------------------------------------------------
//  BodyCicle
//--------------------------------------------------------------------------------
//---------------------------------------
//  �R���X�g���N�^
//---------------------------------------
CBodyCicle::CBodyCicle()
{
	m_fRadius = 0.0f;
}

//---------------------------------------
//  �f�X�g���N�^
//---------------------------------------
CBodyCicle::~CBodyCicle()
{
	m_fRadius = 0.0f;
}

//---------------------------------------
//  ������
//---------------------------------------
void CBodyCicle::Init(void)
{
	cBody.Init();
	m_fRadius = 0.0f;
}

//---------------------------------------
//  �I������
//---------------------------------------
void CBodyCicle::Uninit(void)
{
	cBody.Uninit();
	m_fRadius = 0.0f;
}

//---------------------------------------
//  �X�V����
//---------------------------------------
void CBodyCicle::Update(void)
{
	cBody.Update();
}

//---------------------------------------
//  ���a�ݒ�
//---------------------------------------
void CBodyCicle::SetRadius(float fSetRadius)
{
	m_fRadius = fSetRadius;
}

//---------------------------------------
//  ���a�擾
//---------------------------------------
float CBodyCicle::GetRadius(void)
{
	return m_fRadius;
}

//--------------------------------------------------------------------------------
//  BodyBox
//--------------------------------------------------------------------------------
//---------------------------------------
//  �R���X�g���N�^
//---------------------------------------
CBodyBox::CBodyBox()
{
	int nCnt;
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_vHalfSize = D3DXVECTOR2(0.0f, 0.0f);
}

//---------------------------------------
//  �f�X�g���N�^
//---------------------------------------
CBodyBox::~CBodyBox()
{
	int nCnt;
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_vHalfSize = D3DXVECTOR2(0.0f, 0.0f);
}

//---------------------------------------
//  ������
//---------------------------------------
void CBodyBox::Init(void)
{
	cBody.Init();
	int nCnt;
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_vHalfSize = D3DXVECTOR2(0.0f, 0.0f);
}

//---------------------------------------
//  �I������
//---------------------------------------
void CBodyBox::Uninit(void)
{
	cBody.Uninit();
	int nCnt;
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_vHalfSize = D3DXVECTOR2(0.0f, 0.0f);
}

//---------------------------------------
//  �X�V����
//---------------------------------------
void CBodyBox::Update(void)
{
	cBody.Update();

	UpdateVertex();
	UpdateSide();
}

//---------------------------------------
//  BoxSize�ݒ�
//---------------------------------------
void CBodyBox::SetHalfSize(D3DXVECTOR2 vSetHalfSize)
{
	m_vHalfSize = vSetHalfSize;
	m_fLength = sqrtf(m_vHalfSize.x * m_vHalfSize.x + m_vHalfSize.y * m_vHalfSize.y);
	InitVertex();
	UpdateSide();
}

//---------------------------------------
//  BoxSize�擾
//---------------------------------------
D3DXVECTOR2 CBodyBox::GetHalfSize(void)
{
	return m_vHalfSize;
}

//---------------------------------------
//  ���_������
//---------------------------------------
void CBodyBox::InitVertex(void)
{
	D3DXVECTOR3 vPosCenter = cBody.GetPosCenter();

	//pos
	m_aVertex[0] = D3DXVECTOR3(vPosCenter.x - m_vHalfSize.x, vPosCenter.y - m_vHalfSize.y, 0.0f);
	m_aVertex[1] = D3DXVECTOR3(vPosCenter.x + m_vHalfSize.x, vPosCenter.y - m_vHalfSize.y, 0.0f);
	m_aVertex[2] = D3DXVECTOR3(vPosCenter.x + m_vHalfSize.x, vPosCenter.y + m_vHalfSize.y, 0.0f);
	m_aVertex[3] = D3DXVECTOR3(vPosCenter.x - m_vHalfSize.x, vPosCenter.y + m_vHalfSize.y, 0.0f);

}

//---------------------------------------
//  ���_�X�V
//---------------------------------------
void CBodyBox::UpdateVertex(void)
{
	int nCnt;
	float fRotation = cBody.GetRotation();

	////rotation
	//for (nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	m_aVertex[nCnt] = D3DXVECTOR3(
	//		cosf(fRotation) * m_aVertex[nCnt].x - sinf(fRotation) * m_aVertex[nCnt].y,
	//		sinf(fRotation) * m_aVertex[nCnt].x + cosf(fRotation) * m_aVertex[nCnt].y,
	//		0.0f);
	//}

	//D3DXVECTOR3 vMovement = cBody.GetMovement();
	//cBody.SetMovement(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	////pos
	//m_aVertex[0] += vMovement;
	//m_aVertex[1] += vMovement;
	//m_aVertex[2] += vMovement;
	//m_aVertex[3] += vMovement;

	D3DXVECTOR3 vPos = cBody.GetPosCenter();

	m_aVertex[0] = D3DXVECTOR3(vPos.x + cosf(fRotation + D3DX_PI + m_fAngle) * m_fLength, vPos.y + sin(fRotation + D3DX_PI + m_fAngle) * m_fLength, 0.0f);
	m_aVertex[1] = D3DXVECTOR3(vPos.x + cosf(fRotation - m_fAngle) * m_fLength, vPos.y + sinf(fRotation - m_fAngle) * m_fLength, 0.0f);
	m_aVertex[2] = D3DXVECTOR3(vPos.x + cosf(fRotation + m_fAngle) * m_fLength, vPos.y + sinf(fRotation + m_fAngle) * m_fLength, 0.0f);
	m_aVertex[3] = D3DXVECTOR3(vPos.x + cosf(fRotation - D3DX_PI - m_fAngle) * m_fLength, vPos.y + sinf(fRotation - D3DX_PI - m_fAngle) * m_fLength, 0.0f);
}

//---------------------------------------
//  �l�ӍX�V
//---------------------------------------
void CBodyBox::UpdateSide(void)
{
	int nCnt = 0;

	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aSide[nCnt] = m_aVertex[(nCnt + 1) % 4] - m_aVertex[nCnt];
		D3DXVec3Normalize(&m_aSide[nCnt], &m_aSide[nCnt]);
	}
}

//---------------------------------------
//  ���_�擾
//---------------------------------------
D3DXVECTOR3 CBodyBox::GetVertex(int nNumVertex)
{
	if (nNumVertex < 0 || nNumVertex > 3) 
	{ 
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
	}

	return m_aVertex[nNumVertex];
}

//---------------------------------------
//  �l�ӎ擾
//---------------------------------------
D3DXVECTOR3 CBodyBox::GetSide(int nNumSide)
{
	if (nNumSide < 0 || nNumSide > 3) 
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return m_aSide[nNumSide];
}

//---------------------------------------
//  �p�x�ݒ�
//---------------------------------------
void CBodyBox::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}

//---------------------------------------
//  �p�x�擾
//---------------------------------------
float CBodyBox::GetAngle(void)
{
	return m_fAngle;
}

//---------------------------------------
//  �Ίp���擾
//---------------------------------------
float CBodyBox::GetLength(void)
{
	return m_fLength;
}

//--------------------------------------------------------------------------------
//  BodyQuadrangle
//--------------------------------------------------------------------------------
//---------------------------------------
//  �R���X�g���N�^
//---------------------------------------
CBodyQuadrangle::CBodyQuadrangle()
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//---------------------------------------
//  �f�X�g���N�^
//---------------------------------------
CBodyQuadrangle::~CBodyQuadrangle()
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//---------------------------------------
//  ������
//---------------------------------------
void CBodyQuadrangle::Init(void)
{
	cBody.Init();
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//---------------------------------------
//  �I������
//---------------------------------------
void CBodyQuadrangle::Uninit(void)
{
	cBody.Uninit();
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aSide[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//---------------------------------------
//  �X�V����
//---------------------------------------
void CBodyQuadrangle::Update(void)
{
	cBody.Update();

	UpdateSide();
}

//---------------------------------------
//  �l�ӍX�V
//---------------------------------------
void CBodyQuadrangle::UpdateSide(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_aSide[nCnt] = m_aVertex[(nCnt + 1) % 4] - m_aVertex[nCnt];
		D3DXVec3Normalize(&m_aSide[nCnt], &m_aSide[nCnt]);
	}
}

//---------------------------------------
//  ���_�ݒ�
//---------------------------------------
void CBodyQuadrangle::SetVertex(int nNumVertex, D3DXVECTOR3 vSetVertex)
{
	if (nNumVertex < 0 || nNumVertex > 3) { return; }

	m_aVertex[nNumVertex] = vSetVertex;
}

//---------------------------------------
//  ���_�擾
//---------------------------------------
D3DXVECTOR3 CBodyQuadrangle::GetVertex(int nNumVertex)
{
	if (nNumVertex < 0 || nNumVertex > 3)
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return m_aVertex[nNumVertex];
}

//---------------------------------------
//  �l�ӎ擾
//---------------------------------------
D3DXVECTOR3 CBodyQuadrangle::GetSide(int nNumSide)
{
	if (nNumSide < 0 || nNumSide > 3)
	{
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	return m_aSide[nNumSide];
}