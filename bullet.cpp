//--------------------------------------------------------------------------------
//
//　bullet.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "bullet.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "explosionEffect.h"
#include "player.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define BULLET_NUM_MAX (50)
#define BULLET_SIZE	 (60.0f)
#define BULLET_MASS	 (1.0f)
#define BULLET_SPEED (5.0f)
#define BULLET_TEXTURENAME "data/TEXTURE/bullet.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexBullet(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
typedef struct
{
	BULLET_TYPE type;
	CBodyCicle *pBody;
	bool bUse;
}BULLET;

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;//頂点バッファ管理インターフェースポインタ
BULLET g_aBullet[BULLET_NUM_MAX];

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0;nCnt < BULLET_NUM_MAX;nCnt++)
	{
		g_aBullet[nCnt].bUse = false;
		g_aBullet[nCnt].type = BULLET_SAKE;
		g_aBullet[nCnt].pBody = new CBodyCicle;
		g_aBullet[nCnt].pBody->Init();
		g_aBullet[nCnt].pBody->SetRadius(BULLET_SIZE * 0.5f);
		g_aBullet[nCnt].pBody->cBody.SetMass(BULLET_MASS);
	}

	if (FAILED(MakeVerTexBullet(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexBullet ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME, &g_pTextureBullet)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitBullet(void)
{
	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].pBody != NULL)//アドレスの中身確認
		{
			g_aBullet[nCnt].pBody->Uninit();//終了処理
			delete g_aBullet[nCnt].pBody;//中身を削除
			g_aBullet[nCnt].pBody = NULL;//アドレスを空にする
		}
	}

	//safe release
	if (g_pVtxBufferBullet != NULL)
	{
		g_pVtxBufferBullet->Release();
		g_pVtxBufferBullet = NULL;
	}

	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//StopSound(SOUND_LABEL_BGM_Bullet);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateBullet(void)
{
	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vBulletPos;
	ENEMY *pEnemy;

	g_pVtxBufferBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{
			vBulletPos = g_aBullet[nCnt].pBody->cBody.GetPosCenter();
			//pos
			pVtx[nCnt * 4 + 0].pos = vBulletPos + D3DXVECTOR3( - BULLET_SIZE * 0.5f,  - BULLET_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 1].pos = vBulletPos + D3DXVECTOR3( + BULLET_SIZE * 0.5f,  - BULLET_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 2].pos = vBulletPos + D3DXVECTOR3( - BULLET_SIZE * 0.5f,  + BULLET_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 3].pos = vBulletPos + D3DXVECTOR3( + BULLET_SIZE * 0.5f,  + BULLET_SIZE * 0.5f, 0.0f);

			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX + 1.0f / (float)BULLET_MAX, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(g_aBullet[nCnt].type * 1.0f / (float)BULLET_MAX + 1.0f / (float)BULLET_MAX, 1.0f);

			g_aBullet[nCnt].pBody->Update();

			if (g_aBullet[nCnt].pBody->cBody.GetPosCenter().y + BULLET_SIZE * 0.5f <= 0.0f ||
				g_aBullet[nCnt].pBody->cBody.GetPosCenter().y - BULLET_SIZE * 0.5f >= GAME_HEIGHT)
			{
				g_aBullet[nCnt].bUse = false;
				g_aBullet[nCnt].pBody->Init();
				g_aBullet[nCnt].pBody->SetRadius(BULLET_SIZE * 0.5f);
				g_aBullet[nCnt].pBody->cBody.SetMass(BULLET_MASS);
			}

			AreaContact(&g_aBullet[nCnt].pBody->cBody, BULLET_SIZE * 0.5f);

			for (int nCntEnemy = 0;nCntEnemy < ENEMY_NUM_MAX;nCntEnemy++)
			{
				pEnemy = GetEnemy(nCntEnemy);

				if (pEnemy->bUse)
				{
					if (CheckContactCTC(g_aBullet[nCnt].pBody, pEnemy->pBody))
					{
						g_aBullet[nCnt].bUse = false;

						if((g_aBullet[nCnt].type == BULLET_SAKE && pEnemy->type == ENEMY_TABAKO)
							|| (g_aBullet[nCnt].type == BULLET_TABAKO && pEnemy->type == ENEMY_SAKE)
							|| (g_aBullet[nCnt].type == BULLET_WOMAN && pEnemy->type == ENEMY_MAN)
							|| g_aBullet[nCnt].type == BULLET_BOMB)
						{
							pEnemy->bUse = false;

							SetExplosion(pEnemy->pBody->cBody.GetPosCenter(), g_aBullet[nCnt].type);

							if (g_aBullet[nCnt].type == BULLET_BOMB)
							{
								AddScore(BOMB_SCORE);
								SetExplosionEffect(pEnemy->pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_0);
							}
							else if(g_aBullet[nCnt].type == BULLET_WOMAN)
							{
								AddScore(NORMAL_SCORE);
								SetExplosionEffect(pEnemy->pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_1);
							}
							else
							{
								AddScore(NORMAL_SCORE);
								SetExplosionEffect(pEnemy->pBody->cBody.GetPosCenter(), EXPLOSIONTYPE_2);
							}
						}
						else
						{
							D3DXVECTOR3 vSpeed = pEnemy->pBody->cBody.GetVelocity();
							vSpeed *= 1.5f;
							pEnemy->pBody->cBody.SetVelocity(vSpeed);
						}
					}
				}
			}
		}
	}

	g_pVtxBufferBullet->Unlock();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferBullet,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{
			//プリミティブ描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCnt * 4,//オフセット（頂点数）
				NUM_POLYGON);
		}
	}
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * BULLET_NUM_MAX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferBullet,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferBullet->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	D3DXVECTOR3 vBulletPos;

	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		vBulletPos = g_aBullet[nCnt].pBody->cBody.GetPosCenter();
		//pos
		pVtx[nCnt * 4 + 0].pos = vBulletPos + D3DXVECTOR3(-BULLET_SIZE * 0.5f, -BULLET_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = vBulletPos + D3DXVECTOR3(+BULLET_SIZE * 0.5f, -BULLET_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = vBulletPos + D3DXVECTOR3(-BULLET_SIZE * 0.5f, +BULLET_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = vBulletPos + D3DXVECTOR3(+BULLET_SIZE * 0.5f, +BULLET_SIZE * 0.5f, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[nCnt * 4 + 0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

		//texture頂点
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_aBullet[nCnt].pBody->Update();

	}

	//仮想アドレス解放
	g_pVtxBufferBullet->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  バレット設定
//--------------------------------------------------------------------------------
void SetBullet(BULLET_TYPE type, D3DXVECTOR3 vPos, D3DXVECTOR3 vDirection)
{
	for (int nCnt = 0; nCnt < BULLET_NUM_MAX; nCnt++)
	{
		if (!g_aBullet[nCnt].bUse)
		{
			g_aBullet[nCnt].bUse = true;

			g_aBullet[nCnt].pBody->Init();
			g_aBullet[nCnt].pBody->SetRadius(BULLET_SIZE * 0.5f);
			g_aBullet[nCnt].pBody->cBody.SetMass(BULLET_MASS);

			g_aBullet[nCnt].type = type;
			g_aBullet[nCnt].pBody->cBody.SetPosCenter(vPos);
			g_aBullet[nCnt].pBody->cBody.AddForce(vDirection * BULLET_SPEED);
			break;
		}
	}
}