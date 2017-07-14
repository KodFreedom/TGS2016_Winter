//=============================================================================
//
// 爆発処理 [explosion.cpp]
//
//=============================================================================
#include "explosionEffect.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define	TEXTURE_EXPLOSION	"data/TEXTURE/explosion000.png"	// 読み込むテクスチャファイル名
#define	EXPLOSION_POS_X		(0.0f)							// 爆発の表示基準位置Ｘ座標
#define	EXPLOSION_POS_Y		(0.0f)							// 爆発の表示基準位置Ｙ座標
#define	EXPLOSION_SIZE_X	(160.0f)							// 爆発の幅
#define	EXPLOSION_SIZE_Y	(160.0f)							// 爆発の高さ

#define	MAX_EXPLOSION		(100)					// 爆発の最大数

#define	NUM_PATTERN_X			(8)						// アニメーションのパターン数
#define	NUM_PATTERN_Y			(2)						// アニメーションのパターン数
#define	TEXTURE_SIZE_X		(1.0f / NUM_PATTERN_X)		// テクスチャ内の１パターンの幅
#define	TEXTURE_SIZE_Y		(1.0f / NUM_PATTERN_Y)		// テクスチャ内の１パターンの高さ

#define	INTERVAL_PATTERN	(5)						// アニメーションの切り替え間隔

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
	D3DXVECTOR3 move;	// 移動量
	D3DXCOLOR col;		// 色
	int nCount;			// カウンタ
	int nPatternX;		// 横パターン数
	int nPatternY;		// 縦パターン数
	int nPatternNo;		// パターンNo.
	int nType;			// 種類
	bool bUse;			// 使用しているかどうか
} EXPLOSION_EFFECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void SetVertexExplosion(int nExplosionID);
void SetColorExplosion(int nExplosionID, D3DXCOLOR col);
void SetTextureExplosion(int nExplosionID);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion[EXPLOSIONTYPE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		// 頂点バッファへのポインタ

EXPLOSION_EFFECT			g_aExplosion[MAX_EXPLOSION];	// 爆発のワーク

														// テクスチャのファイル名
const char *c_apTexturExplosion[EXPLOSIONTYPE_MAX] =
{
	"data/TEXTURE/baku.png",
	"data/TEXTURE/baku2.png",
	"data/TEXTURE/baku3.png",
	"data/TEXTURE/baku4.png",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitExplosionEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ResetExplosion();

	//テクスチャ読込
	for (int n = 0; n < EXPLOSIONTYPE_MAX; n++)
	{
		D3DXCreateTextureFromFile(pDevice,	//デバイスへのポインタ
			c_apTexturExplosion[n],			//ファイルの名前
			&g_pTextureExplosion[n]);			//読み込むメモリー
	}

	//頂点情報作成
	MakeVertexExplosion(pDevice);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosionEffect(void)
{
	for (int i = 0; i < EXPLOSIONTYPE_MAX; i++)
	{
		if (g_pTextureExplosion[i] != NULL)
		{// テクスチャの開放
			g_pTextureExplosion[i]->Release();
			g_pTextureExplosion[i] = NULL;
		}
	}

	if (g_pVtxBuffExplosion != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosionEffect(void)
{
	EXPLOSION_EFFECT *pExplosion;

	pExplosion = &g_aExplosion[0];
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{// 爆発を使用している

			pExplosion->nCount++;
			if ((pExplosion->nCount % INTERVAL_PATTERN) == 0)
			{// アニメーション切り替え
				pExplosion->nPatternNo++;
				if (pExplosion->nPatternNo >= pExplosion->nPatternX * pExplosion->nPatternY)
				{// アニメーションパターンが終了
					pExplosion->bUse = false;
				}
				SetTextureExplosion(nCntExplosion);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosionEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	// ポリゴンの描画
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{// 爆発を使用している
		 // テクスチャの設定
		 //pDevice->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pTextureExplosion[g_aExplosion[nCntExplosion].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntExplosion * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_EXPLOSION,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_2D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffExplosion,						// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);

			// rhwの設定
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_SIZE_X, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, TEXTURE_SIZE_Y);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_SIZE_X, TEXTURE_SIZE_Y);
		}

		// 頂点データをアンロックする
		g_pVtxBuffExplosion->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexExplosion(int nExplosionID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nExplosionID * 4);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);

	// 頂点データをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorExplosion(int nExplosionID, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nExplosionID * 4);

	// 頂点カラーの設定
	pVtx[0].color =
		pVtx[1].color =
		pVtx[2].color =
		pVtx[3].color = col;

	// 頂点データをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTextureExplosion(int nExplosionID)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nExplosionID * 4);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo % g_aExplosion[nExplosionID].nPatternX) * (1.0f / g_aExplosion[nExplosionID].nPatternX),
		(g_aExplosion[nExplosionID].nPatternNo / g_aExplosion[nExplosionID].nPatternX) * (1.0f / g_aExplosion[nExplosionID].nPatternY));
	pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo % g_aExplosion[nExplosionID].nPatternX + 1) * (1.0f / g_aExplosion[nExplosionID].nPatternX),
		(g_aExplosion[nExplosionID].nPatternNo / g_aExplosion[nExplosionID].nPatternX) * (1.0f / g_aExplosion[nExplosionID].nPatternY));
	pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo % g_aExplosion[nExplosionID].nPatternX) * (1.0f / g_aExplosion[nExplosionID].nPatternX),
		(g_aExplosion[nExplosionID].nPatternNo / g_aExplosion[nExplosionID].nPatternX + 1) * (1.0f / g_aExplosion[nExplosionID].nPatternY));
	pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo % g_aExplosion[nExplosionID].nPatternX + 1) * (1.0f / g_aExplosion[nExplosionID].nPatternX),
		(g_aExplosion[nExplosionID].nPatternNo / g_aExplosion[nExplosionID].nPatternX + 1) * (1.0f / g_aExplosion[nExplosionID].nPatternY));
	/*
	pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo % NUM_PATTERN_X + 1) * TEXTURE_SIZE_X,
	(g_aExplosion[nExplosionID].nPatternNo / NUM_PATTERN_X) * TEXTURE_SIZE_Y);
	pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo % NUM_PATTERN_X) * TEXTURE_SIZE_X,
	(g_aExplosion[nExplosionID].nPatternNo / NUM_PATTERN_X + 1) * TEXTURE_SIZE_Y);
	pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo % NUM_PATTERN_X + 1) * TEXTURE_SIZE_X,
	(g_aExplosion[nExplosionID].nPatternNo / NUM_PATTERN_X + 1) * TEXTURE_SIZE_Y);
	*/
	/*
	pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo + 1) * TEXTURE_SIZE_X,
	0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nExplosionID].nPatternNo * TEXTURE_SIZE_X,
	TEXTURE_SIZE_Y);
	pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nExplosionID].nPatternNo + 1) * TEXTURE_SIZE_X,
	TEXTURE_SIZE_Y);
	*/
	// 頂点データをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 爆発の設定
//=============================================================================
void SetExplosionEffect(D3DXVECTOR3 pos, int nType)
{
	EXPLOSION_EFFECT *pExplosion;

	pExplosion = &g_aExplosion[0];
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			pExplosion->pos = pos;
			pExplosion->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pExplosion->nCount = 0;
			pExplosion->nPatternNo = 0;
			pExplosion->nType = nType;
			switch (nType)
			{
			case 0:
			{
				pExplosion->nPatternX = 4;
				pExplosion->nPatternY = 1;
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
				break;
			}
			case 1:
			{
				pExplosion->nPatternX = 8;
				pExplosion->nPatternY = 2;
				PlaySound(SOUND_LABEL_SE_LUWAO);
				break;
			}
			case 2:
			{
				pExplosion->nPatternX = 8;
				pExplosion->nPatternY = 2;
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
				break;
			}
			case 3:
			{
				pExplosion->nPatternX = 8;
				pExplosion->nPatternY = 2;
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
				break;
			}
			default:
				break;
			}

			SetVertexExplosion(nCntExplosion);
			SetTextureExplosion(nCntExplosion);

			pExplosion->bUse = true;
			
			break;
		}
	}
}

//=============================================================================
// 爆発のリセット
//=============================================================================
void ResetExplosion(void)
{
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(EXPLOSION_POS_X, EXPLOSION_POS_Y, 0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].nCount = 0;
		g_aExplosion[nCntExplosion].nPatternNo = 0;
		g_aExplosion[nCntExplosion].nType = -1;
		g_aExplosion[nCntExplosion].bUse = false;
	}
}
