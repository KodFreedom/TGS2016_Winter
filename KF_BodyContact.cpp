//--------------------------------------------------------------------------------
//	���q�Փˌv�Z�w�b�_�[
//�@KF_ParticleContact.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "KF_BodyContact.h"
#include "main.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define NUM_CONTACT (100)
#define ITERATION_MAX (300)//�Փˏ����ő���s��
#define GROUND_POS (GAME_HEIGHT)
#define LEFT_WALL_POS (0.0f)
#define RIGHT_WALL_POS (GAME_WIDTH)
#define TOP_WALL_POS (0.0f)
#define CONTACT_NORMAL_GROUND (D3DXVECTOR3(0.0f,-1.0f,0.0f))
#define CONTACT_NORMAL_LEFT_WALL (D3DXVECTOR3(1.0f,0.0f,0.0f))
#define CONTACT_NORMAL_RIGHT_WALL (D3DXVECTOR3(-1.0f,0.0f,0.0f))
#define CONTACT_NORMAL_TOP_WALL (D3DXVECTOR3(0.0f,1.0f,0.0f))

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
void ResolveContacts(void);
void Resolve(int nNumContact);
void ResolveVelocity(int nNumContact);
void ResolveInterpenetration(int nNumContact);
float CalculateSeparatingVelocity(int nNumContact);
void ClearBodyContact(int nNumContact);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
KF_CONTACT g_aPContact[NUM_CONTACT];
int g_nNumContacts;

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
void InitBodyContact(void)
{
	int nCnt;

	g_nNumContacts = 0;

	for (nCnt = 0; nCnt < NUM_CONTACT; nCnt++)
	{
		g_aPContact[nCnt].apBody[0] = NULL;
		g_aPContact[nCnt].apBody[1] = NULL;
		g_aPContact[nCnt].fPenetration = 0.0f;
		g_aPContact[nCnt].vContactNormal = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitBodyContact(void)
{
	int nCnt;

	g_nNumContacts = 0;

	for (nCnt = 0; nCnt < NUM_CONTACT; nCnt++)
	{
		g_aPContact[nCnt].apBody[0] = NULL;
		g_aPContact[nCnt].apBody[1] = NULL;
		g_aPContact[nCnt].fPenetration = 0.0f;
		g_aPContact[nCnt].vContactNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateBodyContact(void)
{
	ResolveContacts();
	g_nNumContacts = 0;
}

//--------------------------------------------------------------------------------
//  ���������Փˏ���
//--------------------------------------------------------------------------------
void ResolveContacts(void)
{
	int nCnt;
	//���s��
	int nIterationUsed = 0;

	//while (nIterationUsed < ITERATION_MAX)
	//{
	//	float fMax = 0.0f;
	//	int nMaxIndex = g_nNumContacts - 1;
	//	for (nCnt = 0; nCnt < g_nNumContacts; nCnt++)
	//	{
	//		float fSepVel = CalculateSeparatingVelocity(nCnt);
	//		if (fSepVel < fMax)
	//		{
	//			fMax = fSepVel;
	//			nMaxIndex = nCnt;
	//		}
	//	}

	//	Resolve(nMaxIndex);
	//	nIterationUsed++;

	//	if (nIterationUsed == g_nNumContacts) { break; }
	//}

	for (nCnt = 0; nCnt < g_nNumContacts; nCnt++)
	{
		Resolve(nCnt);
		nIterationUsed++;
		if (nIterationUsed >= ITERATION_MAX)
		{
			break;
		}
	}
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
void Resolve(int nNumContact)
{
	ResolveVelocity(nNumContact);
	ResolveInterpenetration(nNumContact);
	ClearBodyContact(nNumContact);
}

//--------------------------------------------------------------------------------
//  ���x�v�Z
//--------------------------------------------------------------------------------
void ResolveVelocity(int nNumContact)
{
	//�������x�v�Z
	float fSeparatingVelocity = CalculateSeparatingVelocity(nNumContact);

	//�������x�`�F�b�N
	//�Փ˖@���̋t�����ɂȂ�Όv�Z���Ȃ�
	if (fSeparatingVelocity > 0.0f)
	{
		return;
	}

	//�Ȃ��̏ꍇ�v�Z������
	float fNewSeparatingVelocity = -fSeparatingVelocity * RESTITUTION;

	//�Փ˕����ɍ�p�͂��v�Z����
	D3DXVECTOR3 vAccCausedVelocity = g_aPContact[nNumContact].apBody[0]->GetAcceleration();
	if (g_aPContact[nNumContact].apBody[1])
	{
		vAccCausedVelocity -= g_aPContact[nNumContact].apBody[1]->GetAcceleration();
	}
	float fAccCausedSeparatingVelocity = ScalarProduct(vAccCausedVelocity, g_aPContact[nNumContact].vContactNormal);

	//�Փ˖@���̋t�����ɂȂ��
	if (fAccCausedSeparatingVelocity < 0.0f)
	{
		fNewSeparatingVelocity += RESTITUTION * fAccCausedSeparatingVelocity * 1.0f;
		if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	}

	//���x�����v�Z
	float fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//���q�̎��ʎ擾
	float fTotalInverseMass = g_aPContact[nNumContact].apBody[0]->GetInverseMass();
	if (g_aPContact[nNumContact].apBody[1])
	{
		fTotalInverseMass += g_aPContact[nNumContact].apBody[1]->GetInverseMass();
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0) { return; }

	//�Փ˗͌v�Z
	float fImpulse = fDeltaVelocity / fTotalInverseMass;

	//�P�ʋt���ʂ̏Փ˗�
	D3DXVECTOR3 vImpulsePerInverseMass = g_aPContact[nNumContact].vContactNormal * fImpulse;

	//���x�v�Z
	g_aPContact[nNumContact].apBody[0]->SetVelocity(g_aPContact[nNumContact].apBody[0]->GetVelocity() + vImpulsePerInverseMass * g_aPContact[nNumContact].apBody[0]->GetInverseMass());
	
	if (g_aPContact[nNumContact].apBody[1])
	{
		g_aPContact[nNumContact].apBody[1]->SetVelocity(g_aPContact[nNumContact].apBody[1]->GetVelocity() - vImpulsePerInverseMass * g_aPContact[nNumContact].apBody[1]->GetInverseMass());
	}
}

//--------------------------------------------------------------------------------
//  �Փˌv�Z
//--------------------------------------------------------------------------------
void ResolveInterpenetration(int nNumContact)
{
	//�Փ˂��Ȃ�
	if (g_aPContact[nNumContact].fPenetration <= 0) { return; }

	//�t���ʌv�Z
	float fTotalInverseMass = g_aPContact[nNumContact].apBody[0]->GetInverseMass();
	if (g_aPContact[nNumContact].apBody[1])
	{
		fTotalInverseMass += g_aPContact[nNumContact].apBody[1]->GetInverseMass();
	}

	//���ʂ�������̏ꍇ�v�Z���Ȃ�
	if (fTotalInverseMass <= 0) { return; }

	//���ʒP�ʖ߂�ʌv�Z
	D3DXVECTOR3 vMovePerInverseMass = g_aPContact[nNumContact].vContactNormal * (-g_aPContact[nNumContact].fPenetration / fTotalInverseMass);

	//�e���q�߂�ʒu�v�Z
	g_aPContact[nNumContact].apBody[0]->SetPosCenter(g_aPContact[nNumContact].apBody[0]->GetPosCenter() -
		vMovePerInverseMass * g_aPContact[nNumContact].apBody[0]->GetInverseMass());

	g_aPContact[nNumContact].apBody[0]->SetMovement(g_aPContact[nNumContact].apBody[0]->GetMovement() -
		vMovePerInverseMass * g_aPContact[nNumContact].apBody[0]->GetInverseMass());

	if (g_aPContact[nNumContact].apBody[1])
	{
		g_aPContact[nNumContact].apBody[1]->SetPosCenter(g_aPContact[nNumContact].apBody[1]->GetPosCenter() +
		vMovePerInverseMass * g_aPContact[nNumContact].apBody[1]->GetInverseMass());

		g_aPContact[nNumContact].apBody[1]->SetMovement(g_aPContact[nNumContact].apBody[1]->GetMovement() +
			vMovePerInverseMass * g_aPContact[nNumContact].apBody[1]->GetInverseMass());
	}
}

//--------------------------------------------------------------------------------
//  �������x�v�Z
//--------------------------------------------------------------------------------
float CalculateSeparatingVelocity(int nNumContact)
{
	if (!g_aPContact[nNumContact].apBody[0])
	{
		return 0.0f;
	}

	D3DXVECTOR3 vRelativeVelocity = g_aPContact[nNumContact].apBody[0]->GetVelocity();

	//���q�Ɨ��q�̏Փ�
	if (g_aPContact[nNumContact].apBody[1])
	{
		vRelativeVelocity -= g_aPContact[nNumContact].apBody[1]->GetVelocity();
	}

	//���όv�Z
	return ScalarProduct(vRelativeVelocity, g_aPContact[nNumContact].vContactNormal);
}

//--------------------------------------------------------------------------------
//  �Փːݒ�
//--------------------------------------------------------------------------------
void SetBodyContact(CBody *pBody1, CBody *pBody2, float fSetPenetration, D3DXVECTOR3 vSetContactNormal)
{
	if (g_nNumContacts < NUM_CONTACT)
	{
		g_aPContact[g_nNumContacts].apBody[0] = pBody1;
		g_aPContact[g_nNumContacts].apBody[1] = pBody2;
		g_aPContact[g_nNumContacts].fPenetration = fSetPenetration;
		g_aPContact[g_nNumContacts].vContactNormal = vSetContactNormal;
		g_nNumContacts++;//�Փˉ񐔃J�E���g
	}
}

//--------------------------------------------------------------------------------
//  �Փ˃N���A����
//--------------------------------------------------------------------------------
void ClearBodyContact(int nNumContact)
{
	g_aPContact[nNumContact].apBody[0] = NULL;
	g_aPContact[nNumContact].apBody[1] = NULL;
	g_aPContact[nNumContact].fPenetration = 0.0f;
	g_aPContact[nNumContact].vContactNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------------------------------------
//  �Փˌ��o����
//--------------------------------------------------------------------------------
void CheckContactBTB(CBodyBox *pBody1, CBodyBox *pBody2)
{

}

//--------------------------------------------------------------------------------
//  �Փˌ��o����(�~�Ɖ~)
//--------------------------------------------------------------------------------
bool CheckContactCTC(CBodyCicle *pBody1, CBodyCicle *pBody2)
{
	//�Փˉ񐔂��ő吔�ɒ�����Ό��o���Ȃ�
	if (g_nNumContacts > NUM_CONTACT) { return false; }

	//�~�S�̋������v�Z����
	D3DXVECTOR3 vMidLine = pBody1->cBody.GetPosCenter() - pBody2->cBody.GetPosCenter();
	float fMidLineDistance = Vector3Magnitude(vMidLine);

	//�����Ɣ��a�̘a�Ɣ�ׂ�
	if (fMidLineDistance <= 0.0f || fMidLineDistance >= (pBody1->GetRadius() + pBody2->GetRadius()))
	{
		return false;
	}

	////�Փ˖@�����v�Z����
	//D3DXVECTOR3 vContactNormal = vMidLine / fMidLineDistance;

	////�Փ˕ۑ�
	//SetBodyContact(
	//	&pBody1->cBody,
	//	&pBody2->cBody, 
	//	pBody1->GetRadius() + pBody2->GetRadius() - fMidLineDistance, 
	//	vContactNormal);
	return true;
}

//--------------------------------------------------------------------------------
//  �Փˌ��o����(circle to box)
//--------------------------------------------------------------------------------
void CheckContactCTB(CBodyCicle *pBody1, CBodyBox *pBody2)
{
	//�Փˉ񐔂��ő吔�ɒ�����Ό��o���Ȃ�
	if (g_nNumContacts > NUM_CONTACT) { return; }

	float fDistance = 0.0f;
	float fContactDistance = 0.0f;
	float fRadius = pBody1->GetRadius();//�~�̔��a���擾
	int nCnt = 0;
	D3DXVECTOR3 vVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vSide = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vContactNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vCiclePos = pBody1->cBody.GetPosCenter();

	//�l�ӂƂ̏Փ˂����o
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		vVector = pBody2->GetVertex(nCnt);
		vSide = pBody2->GetSide(nCnt);
		fDistance = -Vector3Cross2D(vSide, (vCiclePos - vVector));

		//�ӂ̊O�ɂ����
		if (fDistance >= 0.0f + fRadius) 
		{
			break;
		}

		//�Փː[�x�̌v�Z
		fDistance = -fDistance + fRadius;

		//�Փː[���Ƃق��̕ӂƂ̏Փː[�����ׂ�
		//��Ԑ󂢂̂�􂢏o��
		if (fContactDistance > 0.0f && fDistance >= fContactDistance)
		{
			continue;
		}

		fContactDistance = fDistance;
		vContactNormal = -VerticalVector3(vSide);
	}

	if (nCnt == 4)
	{
		SetBodyContact(
			&pBody1->cBody,
			&pBody2->cBody,
			fContactDistance,
			vContactNormal);
	}
}

//--------------------------------------------------------------------------------
//  �Փˌ��o����(circle to box)
//--------------------------------------------------------------------------------
void CheckContactCTQ(CBodyCicle *pBody1, CBodyQuadrangle *pBody2)
{
	//�Փˉ񐔂��ő吔�ɒ�����Ό��o���Ȃ�
	if (g_nNumContacts > NUM_CONTACT) { return; }

	float fDistance = 0.0f;
	float fContactDistance = 0.0f;
	float fRadius = pBody1->GetRadius();//�~�̔��a���擾
	int nCnt = 0;
	D3DXVECTOR3 vVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vSide = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vContactNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vCiclePos = pBody1->cBody.GetPosCenter();

	//�l�ӂƂ̏Փ˂����o
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		vVector = pBody2->GetVertex(nCnt);
		vSide = pBody2->GetSide(nCnt);
		fDistance = -Vector3Cross2D(vSide, (vCiclePos - vVector));

		//�ӂ̊O�ɂ����
		if (fDistance >= 0.0f + fRadius)
		{
			break;
		}

		//�Փː[�x�̌v�Z
		fDistance = -fDistance + fRadius;

		//�Փː[���Ƃق��̕ӂƂ̏Փː[�����ׂ�
		//��Ԑ󂢂̂�􂢏o��
		if (fContactDistance > 0.0f && fDistance >= fContactDistance)
		{
			continue;
		}

		fContactDistance = fDistance;
		vContactNormal = -VerticalVector3(vSide);
	}

	if (nCnt == 4)
	{
		SetBodyContact(
			&pBody1->cBody,
			&pBody2->cBody,
			fContactDistance,
			vContactNormal);
	}
}

//--------------------------------------------------------------------------------
//  �G���A�Փˏ���
//--------------------------------------------------------------------------------
void AreaContact(CBody *pBody, float fRadius)
{
//	UpdateGroundContact(pBody, fRadius);
	UpdateLeftWallContact(pBody, fRadius);
	UpdateRightWallContact(pBody, fRadius);
//	UpdateTopWallContact(pBody, fRadius);
}

//--------------------------------------------------------------------------------
//  �n�ʏՓˏ���
//--------------------------------------------------------------------------------
void UpdateGroundContact(CBody *pBody, float fRadius)
{
	if (pBody)
	{
		if (pBody->GetPosCenter().y + fRadius >= GROUND_POS)
		{
			SetBodyContact(
				pBody,
				NULL,
				pBody->GetPosCenter().y + fRadius - GROUND_POS,
				CONTACT_NORMAL_GROUND
			);
		}
	}
}

//--------------------------------------------------------------------------------
//  ���Փˏ���
//--------------------------------------------------------------------------------
void UpdateLeftWallContact(CBody *pBody, float fRadius)
{
	if (pBody)
	{
		if (pBody->GetPosCenter().x - fRadius <= LEFT_WALL_POS)
		{
			SetBodyContact(
				pBody,
				NULL,
				LEFT_WALL_POS - pBody->GetPosCenter().x + fRadius,
				CONTACT_NORMAL_LEFT_WALL
			);
		}
	}
}

//--------------------------------------------------------------------------------
//  �E�Փˏ���
//--------------------------------------------------------------------------------
void UpdateRightWallContact(CBody *pBody, float fRadius)
{
	if (pBody)
	{
		if (pBody->GetPosCenter().x + fRadius >= RIGHT_WALL_POS)
		{
			SetBodyContact(
				pBody,
				NULL,
				pBody->GetPosCenter().x + fRadius - RIGHT_WALL_POS,
				CONTACT_NORMAL_RIGHT_WALL
			);
		}
	}
}

void UpdateTopWallContact(CBody *pBody, float fRadius)
{
	if (pBody)
	{
		if (pBody->GetPosCenter().y - fRadius <= TOP_WALL_POS)
		{
			SetBodyContact(
				pBody,
				NULL,
				fRadius - pBody->GetPosCenter().y,
				CONTACT_NORMAL_TOP_WALL
			);
		}
	}
}