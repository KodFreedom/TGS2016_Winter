//--------------------------------------------------------------------------------
//
//　ResultGold.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "resultGold.h"
#include "main.h"
#include "KF_Body.h"
#include "KF_BodyContact.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define RESULT_GOLD_NUM_MAX (100)
#define RESULT_GOLD_SIZE	(80.0f)
#define RESULT_GOLD_MASS   (1.0f)
#define RESULT_GOLD_SPEED (1.0f)
#define RESULT_GOLD_CREATE_CD (20)
#define RESULT_GOLD_TEXTURENAME "data/TEXTURE/kane2.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexResultGold(LPDIRECT3DDEVICE9 pDevice);
void CreateResultGold(void);

typedef struct
{
	CBodyCicle *pBody;
	bool bUse;
}GOLD;

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureResultGold = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultGold = NULL;//頂点バッファ管理インターフェースポインタ
GOLD g_aResultGold[RESULT_GOLD_NUM_MAX];

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitResultGold(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	for (int nCnt = 0;nCnt < RESULT_GOLD_NUM_MAX;nCnt++)
	{
		g_aResultGold[nCnt].bUse = false;
		g_aResultGold[nCnt].pBody = new CBodyCicle;
		g_aResultGold[nCnt].pBody->Init();
		g_aResultGold[nCnt].pBody->SetRadius(RESULT_GOLD_SIZE * 0.5f);
		g_aResultGold[nCnt].pBody->cBody.SetMass(RESULT_GOLD_MASS);
	}

	if (FAILED(MakeVerTexResultGold(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexResultGold ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, RESULT_GOLD_TEXTURENAME, &g_pTextureResultGold)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitResultGold(void)
{
	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (g_aResultGold[nCnt].pBody != NULL)//アドレスの中身確認
		{
			g_aResultGold[nCnt].pBody->Uninit();//終了処理
			delete g_aResultGold[nCnt].pBody;//中身を削除
			g_aResultGold[nCnt].pBody = NULL;//アドレスを空にする
		}
	}

	//safe release
	if (g_pVtxBufferResultGold != NULL)
	{
		g_pVtxBufferResultGold->Release();
		g_pVtxBufferResultGold = NULL;
	}

	if (g_pTextureResultGold != NULL)
	{
		g_pTextureResultGold->Release();
		g_pTextureResultGold = NULL;
	}

	//StopSound(SOUND_LABEL_BGM_ResultGold);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateResultGold(void)
{
	static int nCreateResultGoldCnt = 0;

	if (nCreateResultGoldCnt == 0)
	{
		CreateResultGold();
	}

	nCreateResultGoldCnt = (nCreateResultGoldCnt + 1) % RESULT_GOLD_CREATE_CD;

	VERTEX_2D *pVtx = NULL;
	D3DXVECTOR3 vResultGoldPos;

	g_pVtxBufferResultGold->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (g_aResultGold[nCnt].bUse)
		{
			vResultGoldPos = g_aResultGold[nCnt].pBody->cBody.GetPosCenter();
			//pos
			pVtx[nCnt * 4 + 0].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 1].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 2].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);
			pVtx[nCnt * 4 + 3].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);

			g_aResultGold[nCnt].pBody->Update();

			if (g_aResultGold[nCnt].pBody->cBody.GetPosCenter().y - RESULT_GOLD_SIZE * 0.5f >= GAME_HEIGHT)
			{
				g_aResultGold[nCnt].bUse = false;
			}

			AreaContact(&g_aResultGold[nCnt].pBody->cBody, RESULT_GOLD_SIZE * 0.5f);
		}
	}

	g_pVtxBufferResultGold->Unlock();

}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawResultGold(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferResultGold,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTextureResultGold);

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (g_aResultGold[nCnt].bUse)
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
HRESULT MakeVerTexResultGold(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * RESULT_GOLD_NUM_MAX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferResultGold,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferResultGold->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	D3DXVECTOR3 vResultGoldPos;

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		vResultGoldPos = g_aResultGold[nCnt].pBody->cBody.GetPosCenter();
		//pos
		pVtx[nCnt * 4 + 0].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 1].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, -RESULT_GOLD_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 2].pos = vResultGoldPos + D3DXVECTOR3(-RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);
		pVtx[nCnt * 4 + 3].pos = vResultGoldPos + D3DXVECTOR3(+RESULT_GOLD_SIZE * 0.5f, +RESULT_GOLD_SIZE * 0.5f, 0.0f);

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

		g_aResultGold[nCnt].pBody->Update();

	}

	//仮想アドレス解放
	g_pVtxBufferResultGold->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------
//  バレット設定
//--------------------------------------------------------------------------------
void CreateResultGold(void)
{
	D3DXVECTOR3 vPos = GetRandomVec3(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f));
	D3DXVECTOR3 vDirection = GetRandomVec3(D3DXVECTOR3(-0.9f, 0.1f, 0.0f), D3DXVECTOR3(0.9f, 1.0f, 0.0f));
	D3DXVec3Normalize(&vDirection, &vDirection);

	for (int nCnt = 0; nCnt < RESULT_GOLD_NUM_MAX; nCnt++)
	{
		if (!g_aResultGold[nCnt].bUse)
		{
			g_aResultGold[nCnt].bUse = true;
			g_aResultGold[nCnt].pBody->Init();
			g_aResultGold[nCnt].pBody->SetRadius(RESULT_GOLD_SIZE * 0.5f);
			g_aResultGold[nCnt].pBody->cBody.SetMass(RESULT_GOLD_MASS);
			g_aResultGold[nCnt].pBody->cBody.SetPosCenter(vPos);
			g_aResultGold[nCnt].pBody->cBody.AddForce(vDirection * RESULT_GOLD_SPEED);
			break;
		}
	}
}