//--------------------------------------------------------------------------------
//
//　Enemy.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "enemy.h"
#include "player.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"
#include "sound.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define ENEMY_SIZE	 (80.0f)
#define ENEMY_MASS   (1.0f)
#define ENEMY_SPEED (0.8f)
#define ENEMY_CREATE_CD (30)
#define ENEMY_TEXTURENAME "data/TEXTURE/enemy.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice);
void CreateEnemy(void);

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemy = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemy = NULL;//頂点バッファ管理インターフェースポインタ
ENEMY g_aEnemy[ENEMY_NUM_MAX];

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0;nCnt < ENEMY_NUM_MAX;nCnt++)
	{
		g_aEnemy[nCnt].bUse = false;
		g_aEnemy[nCnt].type = ENEMY_SAKE;
		g_aEnemy[nCnt].pBody = new CBodyCicle;
		g_aEnemy[nCnt].pBody->Init();
		g_aEnemy[nCnt].pBody->SetRadius(ENEMY_SIZE * 0.5f);
		g_aEnemy[nCnt].pBody->cBody.SetMass(ENEMY_MASS);
	}

	if (FAILED(MakeVerTexEnemy(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexEnemy ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURENAME, &g_pTextureEnemy)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (g_aEnemy[nCnt].pBody != NULL)//アドレスの中身確認
		{
			g_aEnemy[nCnt].pBody->Uninit();//終了処理
			delete g_aEnemy[nCnt].pBody;//中身を削除
			g_aEnemy[nCnt].pBody = NULL;//アドレスを空にする
		}
	}

	//safe release
	if (g_pVtxBufferEnemy != NULL)
	{
		g_pVtxBufferEnemy->Release();
		g_pVtxBufferEnemy = NULL;
	}

	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

	//StopSound(SOUND_LABEL_BGM_Enemy);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	static int nCreateEnemyCnt = 0;

	if (nCreateEnemyCnt == 0)
	{
		CreateEnemy();
	}

	nCreateEnemyCnt = (nCreateEnemyCnt + 1) % ENEMY_CREATE_CD;

	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vEnemyPos;

	g_pVtxBufferEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse)
		{
			vEnemyPos = g_aEnemy[nCnt].pBody->cBody.GetPosCenter();
			//pos
			pVtx[nCnt * 4 + 0].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 1].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 2].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 3].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);

			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f + 1.0f / 3.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(g_aEnemy[nCnt].type * 1.0f / 3.0f + 1.0f / 3.0f, 1.0f);

			g_aEnemy[nCnt].pBody->Update();

			if (g_aEnemy[nCnt].pBody->cBody.GetPosCenter().y - ENEMY_SIZE * 0.5f >= GAME_HEIGHT)
			{
				g_aEnemy[nCnt].bUse = false;
				SubPlayerLife();
				PlaySound(SOUND_LABEL_SE_DAMAGE);
			}

			AreaContact(&g_aEnemy[nCnt].pBody->cBody, ENEMY_SIZE * 0.5f);
		}
	}

	g_pVtxBufferEnemy->Unlock();

}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferEnemy,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTextureEnemy);

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse)
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
HRESULT MakeVerTexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * ENEMY_NUM_MAX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferEnemy,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferEnemy->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	D3DXVECTOR3 vEnemyPos;

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		vEnemyPos = g_aEnemy[nCnt].pBody->cBody.GetPosCenter();
		//pos
		pVtx[nCnt * 4 + 0].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, -ENEMY_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = vEnemyPos + D3DXVECTOR3(-ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = vEnemyPos + D3DXVECTOR3(+ENEMY_SIZE * 0.5f, +ENEMY_SIZE * 0.5f, 0.0f);

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

		g_aEnemy[nCnt].pBody->Update();

	}

	//仮想アドレス解放
	g_pVtxBufferEnemy->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  バレット設定
//--------------------------------------------------------------------------------
void CreateEnemy(void)
{
	int nType = rand() % (int)ENEMY_MAX;
	D3DXVECTOR3 vPos = GetRandomVec3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f));
	D3DXVECTOR3 vDirection = GetRandomVec3(D3DXVECTOR3(-0.9f, 0.1f, 0.0f), D3DXVECTOR3(0.9f, 1.0f, 0.0f));
	D3DXVec3Normalize(&vDirection, &vDirection);

	for (int nCnt = 0; nCnt < ENEMY_NUM_MAX; nCnt++)
	{
		if (!g_aEnemy[nCnt].bUse)
		{
			g_aEnemy[nCnt].bUse = true;
			g_aEnemy[nCnt].pBody->Init();
			g_aEnemy[nCnt].pBody->SetRadius(ENEMY_SIZE * 0.5f);
			g_aEnemy[nCnt].pBody->cBody.SetMass(ENEMY_MASS);
			g_aEnemy[nCnt].type = (ENEMY_TYPE)nType;
			g_aEnemy[nCnt].pBody->cBody.SetPosCenter(vPos);
			g_aEnemy[nCnt].pBody->cBody.AddForce(vDirection * ENEMY_SPEED);
			break;
		}
	}
}

ENEMY *GetEnemy(int nID)
{
	return &g_aEnemy[nID];
}