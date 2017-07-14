//--------------------------------------------------------------------------------
//
//　game.cpp
//--------------------------------------------------------------------------------
#include "main.h"
#include "score.h"
#include "fade.h"



//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define NUM_TEXTURE_SCORE (2)		//使用するテクスチャの数

#define SCORE_POS_X (1200.0f)		//スコアの表示位置x(一桁目左上頂点x座標)
#define SCORE_POS_Y (650.0f)		//スコアの表示位置y
#define SCORE_WIDTH (50.0f)		//スコアの文字の横幅
#define SCORE_HEIGHT (50.0f)		//スコアの文字の縦幅
#define SCORE_BETEWEEN (2.0f)		//スコアの字間
#define SCORE_DIGIT (5)				//スコアの桁数


#define NUM_SCORE_OBJECT (100)				//お金の最大表示数
#define SCORE_OBJECT_POS_X (1000.0f)		//お金の表示位置x座標
#define SCORE_OBJECT_POS_Y (600.0f)			//お金の表示位置y座標
#define SCORE_OBJECT_WIDTH (200.0f)			//お金ポリゴンの横幅
#define SCORE_OBJECT_HEIGHT (200.0f)		//お金ポリゴンの横幅
#define SCORE_OBJECT_DIFFERENCE_Y (30.0f)	//お金が積み重なる際のy座標変化量
#define SCORE_OBJECT_DIFFERENCE_X (60.0f)	//お金が積み重なる際のx座標変化量
#define SCORE_DEVISION (200)				//お金が追加されるスコア

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureScore[NUM_TEXTURE_SCORE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScoreObject = NULL;


int g_nScoreValue = 0;										//スコア


// 敵テクスチャのファイル名
const char *c_apTextureScore[NUM_TEXTURE_SCORE]=
{
	"data/TEXTURE/scoreNumber.png",
	"data/TEXTURE/kane-01.png",
};


//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitScore(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if(pDevice == NULL)
	{
		MessageBox(NULL,"デバイス取得に失敗しました。","エラー(InitScore)",MB_OK);
		return;
	}


	//テクスチャ読込
	for(int n = 0; n < NUM_TEXTURE_SCORE; n++)
	{
		D3DXCreateTextureFromFile(pDevice,	//デバイスへのポインタ
			c_apTextureScore[n],			//ファイルの名前
			&g_pTextureScore[n]);			//読み込むメモリー
	}



	//頂点情報作成
	if(FAILED(MakeVertexScore(pDevice)))
	{
		MessageBox(NULL,"頂点情報の作成に失敗","エラー(InitScore)",MB_OK);
		return;
	}


	//スコアのリセット
	g_nScoreValue = 0;

}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitScore(void)
{
	//テクスチャの開放
	for(int n = 0; n < NUM_TEXTURE_SCORE; n++){
		if(g_pTextureScore[n] != NULL){
			g_pTextureScore[n]->Release();
			g_pTextureScore[n] = NULL;
		}
	}

	//頂点バッファの開放
	if(g_pVtxBufferScore != NULL){
		g_pVtxBufferScore->Release();
		g_pVtxBufferScore = NULL;
	}

	if(g_pVtxBufferScoreObject != NULL){
		g_pVtxBufferScoreObject->Release();
		g_pVtxBufferScoreObject = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateScore(void)
{
	int number;
	int value = (int)g_nScoreValue;
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、仮想アドレスを取得
	g_pVtxBufferScore->Lock(0, 0, (void**)&pVtx, 0);

	for(int n = 0; n < SCORE_DIGIT; n++, pVtx+=4)
	{
		//1桁の値を取得
		number = value % 10;

		//uv座標の設定
		pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

		//桁を1つずらす
		value /= 10;
	}

	// 頂点データをアンロックする
	g_pVtxBufferScore->Unlock();


	//ゲーム終了条件チェック
	if(GetScore() >= FINISH_SCORE)
	{
		SetFade(FADE_OUT,MODE_RESULT);
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawScore(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetFVF(FVF_VERTEX_2D);


	pDevice->SetStreamSource(0, g_pVtxBufferScoreObject, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, g_pTextureScore[1]);
	int numDrawObject = g_nScoreValue / SCORE_DEVISION;//描画数の計算
	numDrawObject = min(numDrawObject, SCORE_DEVISION);
	for(int n = 0; n < numDrawObject; n++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (n * 4), NUM_POLYGON);
	}


	pDevice->SetStreamSource(0, g_pVtxBufferScore, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, g_pTextureScore[0]);
	for(int n = 0; n < SCORE_DIGIT; n++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (n * 4), NUM_POLYGON);
	}


}

//--------------------------------------------------------------------------------
//  スコア指定
//--------------------------------------------------------------------------------
void SetScore(int nScoreValue)
{
	g_nScoreValue = nScoreValue;
}

//--------------------------------------------------------------------------------
//  スコア取得
//--------------------------------------------------------------------------------
int GetScore(void)
{
	return g_nScoreValue;
}

//--------------------------------------------------------------------------------
//  スコア加算
//--------------------------------------------------------------------------------
void AddScore(int nAddValue)
{
	g_nScoreValue += nAddValue;
	if (g_nScoreValue >= FINISH_SCORE)
	{
		g_nScoreValue = FINISH_SCORE;
	}
}


//--------------------------------------------------------------------------------
//  頂点作成
//--------------------------------------------------------------------------------
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	//引数チェック
	if(pDevice == NULL)
	{
		return E_FAIL;
	}


	//頂点バッファの作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * SCORE_DIGIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufferScore,
		NULL
		);


	VERTEX_2D* pVtx;
	g_pVtxBufferScore->Lock(0, 0, (void**)&pVtx, 0);
	for(int i=0; i<SCORE_DIGIT; i++, pVtx+=4)
	{
		//頂点座標の設定(2D座標、右回り)
		pVtx[0].pos=D3DXVECTOR3(SCORE_POS_X - (SCORE_BETEWEEN + SCORE_WIDTH) * i,
			SCORE_POS_Y,
			0.0f);
		pVtx[1].pos=D3DXVECTOR3(SCORE_POS_X - (SCORE_BETEWEEN + SCORE_WIDTH) * i + SCORE_WIDTH,
			SCORE_POS_Y,
			0.0f);
		pVtx[2].pos=D3DXVECTOR3(SCORE_POS_X - (SCORE_BETEWEEN + SCORE_WIDTH) * i,
			SCORE_POS_Y + SCORE_HEIGHT,
			0.0f);
		pVtx[3].pos=D3DXVECTOR3(SCORE_POS_X - (SCORE_BETEWEEN + SCORE_WIDTH) * i + SCORE_WIDTH,
			SCORE_POS_Y + SCORE_HEIGHT,
			0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color =
			pVtx[1].color =
			pVtx[2].color =
			pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//uv値(テクスチャ座標)の設定
		pVtx[0].tex=D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex=D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex=D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex=D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファのアンロック(GPUが描画開始出来る)
	g_pVtxBufferScore->Unlock();



	//------------------------ゲージのお金の板ポリ---------------------
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * NUM_SCORE_OBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufferScoreObject,
		NULL
		);

	g_pVtxBufferScoreObject->Lock(0, 0, (void**)&pVtx, 0);
	for(int i=0; i < NUM_SCORE_OBJECT; i++, pVtx+=4)
	{
		//頂点座標の設定(2D座標、右回り)
		pVtx[0].pos=D3DXVECTOR3(SCORE_OBJECT_POS_X + (SCORE_OBJECT_DIFFERENCE_X * (i%2)),
			SCORE_OBJECT_POS_Y - (SCORE_OBJECT_DIFFERENCE_Y * i),
			0.0f);
		pVtx[1].pos=D3DXVECTOR3(SCORE_OBJECT_POS_X + SCORE_OBJECT_WIDTH + (SCORE_OBJECT_DIFFERENCE_X * (i%2)),
			SCORE_OBJECT_POS_Y - (SCORE_OBJECT_DIFFERENCE_Y * i),
			0.0f);
		pVtx[2].pos=D3DXVECTOR3(SCORE_OBJECT_POS_X + (SCORE_OBJECT_DIFFERENCE_X * (i%2)),
			SCORE_OBJECT_POS_Y + SCORE_OBJECT_HEIGHT - (SCORE_OBJECT_DIFFERENCE_Y * i),
			0.0f);
		pVtx[3].pos=D3DXVECTOR3(SCORE_OBJECT_POS_X + SCORE_OBJECT_WIDTH + (SCORE_OBJECT_DIFFERENCE_X * (i%2)),
			SCORE_OBJECT_POS_Y + SCORE_OBJECT_HEIGHT - (SCORE_OBJECT_DIFFERENCE_Y * i),
			0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color =
			pVtx[1].color =
			pVtx[2].color =
			pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//uv値(テクスチャ座標)の設定
		pVtx[0].tex=D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex=D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex=D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex=D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファのアンロック(GPUが描画開始出来る)
	g_pVtxBufferScoreObject->Unlock();


	return S_OK;

}

