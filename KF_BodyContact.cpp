//--------------------------------------------------------------------------------
//	粒子衝突計算ヘッダー
//　KF_ParticleContact.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "KF_BodyContact.h"
#include "main.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define NUM_CONTACT (100)
#define ITERATION_MAX (300)//衝突処理最大実行回数
#define GROUND_POS (GAME_HEIGHT)
#define LEFT_WALL_POS (0.0f)
#define RIGHT_WALL_POS (GAME_WIDTH)
#define TOP_WALL_POS (0.0f)
#define CONTACT_NORMAL_GROUND (D3DXVECTOR3(0.0f,-1.0f,0.0f))
#define CONTACT_NORMAL_LEFT_WALL (D3DXVECTOR3(1.0f,0.0f,0.0f))
#define CONTACT_NORMAL_RIGHT_WALL (D3DXVECTOR3(-1.0f,0.0f,0.0f))
#define CONTACT_NORMAL_TOP_WALL (D3DXVECTOR3(0.0f,1.0f,0.0f))

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
void ResolveContacts(void);
void Resolve(int nNumContact);
void ResolveVelocity(int nNumContact);
void ResolveInterpenetration(int nNumContact);
float CalculateSeparatingVelocity(int nNumContact);
void ClearBodyContact(int nNumContact);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
KF_CONTACT g_aPContact[NUM_CONTACT];
int g_nNumContacts;

//--------------------------------------------------------------------------------
//  初期化
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
//  終了処理
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
//  更新処理
//--------------------------------------------------------------------------------
void UpdateBodyContact(void)
{
	ResolveContacts();
	g_nNumContacts = 0;
}

//--------------------------------------------------------------------------------
//  速さ順次衝突処理
//--------------------------------------------------------------------------------
void ResolveContacts(void)
{
	int nCnt;
	//実行回数
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
//  デストラクタ
//--------------------------------------------------------------------------------
void Resolve(int nNumContact)
{
	ResolveVelocity(nNumContact);
	ResolveInterpenetration(nNumContact);
	ClearBodyContact(nNumContact);
}

//--------------------------------------------------------------------------------
//  速度計算
//--------------------------------------------------------------------------------
void ResolveVelocity(int nNumContact)
{
	//分離速度計算
	float fSeparatingVelocity = CalculateSeparatingVelocity(nNumContact);

	//分離速度チェック
	//衝突法線の逆方向になれば計算しない
	if (fSeparatingVelocity > 0.0f)
	{
		return;
	}

	//ないの場合計算し直す
	float fNewSeparatingVelocity = -fSeparatingVelocity * RESTITUTION;

	//衝突方向に作用力を計算する
	D3DXVECTOR3 vAccCausedVelocity = g_aPContact[nNumContact].apBody[0]->GetAcceleration();
	if (g_aPContact[nNumContact].apBody[1])
	{
		vAccCausedVelocity -= g_aPContact[nNumContact].apBody[1]->GetAcceleration();
	}
	float fAccCausedSeparatingVelocity = ScalarProduct(vAccCausedVelocity, g_aPContact[nNumContact].vContactNormal);

	//衝突法線の逆方向になれば
	if (fAccCausedSeparatingVelocity < 0.0f)
	{
		fNewSeparatingVelocity += RESTITUTION * fAccCausedSeparatingVelocity * 1.0f;
		if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	}

	//速度差分計算
	float fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//粒子の質量取得
	float fTotalInverseMass = g_aPContact[nNumContact].apBody[0]->GetInverseMass();
	if (g_aPContact[nNumContact].apBody[1])
	{
		fTotalInverseMass += g_aPContact[nNumContact].apBody[1]->GetInverseMass();
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0) { return; }

	//衝突力計算
	float fImpulse = fDeltaVelocity / fTotalInverseMass;

	//単位逆質量の衝突力
	D3DXVECTOR3 vImpulsePerInverseMass = g_aPContact[nNumContact].vContactNormal * fImpulse;

	//速度計算
	g_aPContact[nNumContact].apBody[0]->SetVelocity(g_aPContact[nNumContact].apBody[0]->GetVelocity() + vImpulsePerInverseMass * g_aPContact[nNumContact].apBody[0]->GetInverseMass());
	
	if (g_aPContact[nNumContact].apBody[1])
	{
		g_aPContact[nNumContact].apBody[1]->SetVelocity(g_aPContact[nNumContact].apBody[1]->GetVelocity() - vImpulsePerInverseMass * g_aPContact[nNumContact].apBody[1]->GetInverseMass());
	}
}

//--------------------------------------------------------------------------------
//  衝突計算
//--------------------------------------------------------------------------------
void ResolveInterpenetration(int nNumContact)
{
	//衝突しない
	if (g_aPContact[nNumContact].fPenetration <= 0) { return; }

	//逆質量計算
	float fTotalInverseMass = g_aPContact[nNumContact].apBody[0]->GetInverseMass();
	if (g_aPContact[nNumContact].apBody[1])
	{
		fTotalInverseMass += g_aPContact[nNumContact].apBody[1]->GetInverseMass();
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0) { return; }

	//質量単位戻り量計算
	D3DXVECTOR3 vMovePerInverseMass = g_aPContact[nNumContact].vContactNormal * (-g_aPContact[nNumContact].fPenetration / fTotalInverseMass);

	//各粒子戻り位置計算
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
//  分離速度計算
//--------------------------------------------------------------------------------
float CalculateSeparatingVelocity(int nNumContact)
{
	if (!g_aPContact[nNumContact].apBody[0])
	{
		return 0.0f;
	}

	D3DXVECTOR3 vRelativeVelocity = g_aPContact[nNumContact].apBody[0]->GetVelocity();

	//粒子と粒子の衝突
	if (g_aPContact[nNumContact].apBody[1])
	{
		vRelativeVelocity -= g_aPContact[nNumContact].apBody[1]->GetVelocity();
	}

	//内積計算
	return ScalarProduct(vRelativeVelocity, g_aPContact[nNumContact].vContactNormal);
}

//--------------------------------------------------------------------------------
//  衝突設定
//--------------------------------------------------------------------------------
void SetBodyContact(CBody *pBody1, CBody *pBody2, float fSetPenetration, D3DXVECTOR3 vSetContactNormal)
{
	if (g_nNumContacts < NUM_CONTACT)
	{
		g_aPContact[g_nNumContacts].apBody[0] = pBody1;
		g_aPContact[g_nNumContacts].apBody[1] = pBody2;
		g_aPContact[g_nNumContacts].fPenetration = fSetPenetration;
		g_aPContact[g_nNumContacts].vContactNormal = vSetContactNormal;
		g_nNumContacts++;//衝突回数カウント
	}
}

//--------------------------------------------------------------------------------
//  衝突クリア処理
//--------------------------------------------------------------------------------
void ClearBodyContact(int nNumContact)
{
	g_aPContact[nNumContact].apBody[0] = NULL;
	g_aPContact[nNumContact].apBody[1] = NULL;
	g_aPContact[nNumContact].fPenetration = 0.0f;
	g_aPContact[nNumContact].vContactNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------------------------------------
//  衝突検出処理
//--------------------------------------------------------------------------------
void CheckContactBTB(CBodyBox *pBody1, CBodyBox *pBody2)
{

}

//--------------------------------------------------------------------------------
//  衝突検出処理(円と円)
//--------------------------------------------------------------------------------
bool CheckContactCTC(CBodyCicle *pBody1, CBodyCicle *pBody2)
{
	//衝突回数が最大数に超えれば検出しない
	if (g_nNumContacts > NUM_CONTACT) { return false; }

	//円心の距離を計算する
	D3DXVECTOR3 vMidLine = pBody1->cBody.GetPosCenter() - pBody2->cBody.GetPosCenter();
	float fMidLineDistance = Vector3Magnitude(vMidLine);

	//距離と半径の和と比べる
	if (fMidLineDistance <= 0.0f || fMidLineDistance >= (pBody1->GetRadius() + pBody2->GetRadius()))
	{
		return false;
	}

	////衝突法線を計算する
	//D3DXVECTOR3 vContactNormal = vMidLine / fMidLineDistance;

	////衝突保存
	//SetBodyContact(
	//	&pBody1->cBody,
	//	&pBody2->cBody, 
	//	pBody1->GetRadius() + pBody2->GetRadius() - fMidLineDistance, 
	//	vContactNormal);
	return true;
}

//--------------------------------------------------------------------------------
//  衝突検出処理(circle to box)
//--------------------------------------------------------------------------------
void CheckContactCTB(CBodyCicle *pBody1, CBodyBox *pBody2)
{
	//衝突回数が最大数に超えれば検出しない
	if (g_nNumContacts > NUM_CONTACT) { return; }

	float fDistance = 0.0f;
	float fContactDistance = 0.0f;
	float fRadius = pBody1->GetRadius();//円の半径を取得
	int nCnt = 0;
	D3DXVECTOR3 vVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vSide = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vContactNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vCiclePos = pBody1->cBody.GetPosCenter();

	//四辺との衝突を検出
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		vVector = pBody2->GetVertex(nCnt);
		vSide = pBody2->GetSide(nCnt);
		fDistance = -Vector3Cross2D(vSide, (vCiclePos - vVector));

		//辺の外にあれば
		if (fDistance >= 0.0f + fRadius) 
		{
			break;
		}

		//衝突深度の計算
		fDistance = -fDistance + fRadius;

		//衝突深さとほかの辺との衝突深さを比べて
		//一番浅いのを洗い出す
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
//  衝突検出処理(circle to box)
//--------------------------------------------------------------------------------
void CheckContactCTQ(CBodyCicle *pBody1, CBodyQuadrangle *pBody2)
{
	//衝突回数が最大数に超えれば検出しない
	if (g_nNumContacts > NUM_CONTACT) { return; }

	float fDistance = 0.0f;
	float fContactDistance = 0.0f;
	float fRadius = pBody1->GetRadius();//円の半径を取得
	int nCnt = 0;
	D3DXVECTOR3 vVector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vSide = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vContactNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vCiclePos = pBody1->cBody.GetPosCenter();

	//四辺との衝突を検出
	for (nCnt = 0; nCnt < 4; nCnt++)
	{
		vVector = pBody2->GetVertex(nCnt);
		vSide = pBody2->GetSide(nCnt);
		fDistance = -Vector3Cross2D(vSide, (vCiclePos - vVector));

		//辺の外にあれば
		if (fDistance >= 0.0f + fRadius)
		{
			break;
		}

		//衝突深度の計算
		fDistance = -fDistance + fRadius;

		//衝突深さとほかの辺との衝突深さを比べて
		//一番浅いのを洗い出す
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
//  エリア衝突処理
//--------------------------------------------------------------------------------
void AreaContact(CBody *pBody, float fRadius)
{
//	UpdateGroundContact(pBody, fRadius);
	UpdateLeftWallContact(pBody, fRadius);
	UpdateRightWallContact(pBody, fRadius);
//	UpdateTopWallContact(pBody, fRadius);
}

//--------------------------------------------------------------------------------
//  地面衝突処理
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
//  左衝突処理
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
//  右衝突処理
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