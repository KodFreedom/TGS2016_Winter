//=============================================================================
//
// 爆発処理 [explosion.h]
//
//=============================================================================
#ifndef _EXPLOSION_EFFECT_H_
#define _EXPLOSION_EFFECT_H_

#include "main.h"

//*************************************
// 爆発の種類
//*************************************
enum
{
	EXPLOSIONTYPE_0 = 0,//bomb
	EXPLOSIONTYPE_1,//woman
	EXPLOSIONTYPE_2,//other
	EXPLOSIONTYPE_3,//other
	EXPLOSIONTYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitExplosionEffect(void);
void UninitExplosionEffect(void);
void UpdateExplosionEffect(void);
void DrawExplosionEffect(void);

void SetExplosionEffect(D3DXVECTOR3 pos, int nType);
void ResetExplosion(void);

#endif
