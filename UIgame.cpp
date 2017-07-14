//--------------------------------------------------------------------------------
//
//　UIGame.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "UIgame.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define UI_GAME_POS_X (0.0f)//ポリゴンの表示位置X
#define UI_GAME_POS_Y (0.0f)//ポリゴンの表示位置Y
#define UI_GAME_TEXTURENAME "data/TEXTURE/gameUI.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIGame(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureUIGame = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferUIGame = NULL;//頂点バッファ管理インターフェースポインタ

													//--------------------------------------------------------------------------------
													//  初期化処理
													//--------------------------------------------------------------------------------
void InitUIGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	if (FAILED(MakeVerTexUIGame(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexUIGame ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, UI_GAME_TEXTURENAME, &g_pTextureUIGame)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitUIGame(void)
{
	//safe release
	if (g_pVtxBufferUIGame != NULL)
	{
		g_pVtxBufferUIGame->Release();
		g_pVtxBufferUIGame = NULL;
	}

	if (g_pTextureUIGame != NULL)
	{
		g_pTextureUIGame->Release();
		g_pTextureUIGame = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateUIGame(void)
{
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawUIGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferUIGame,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTextureUIGame);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexUIGame(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferUIGame,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferUIGame->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(UI_GAME_POS_X, UI_GAME_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(UI_GAME_POS_X + SCREEN_WIDTH, UI_GAME_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(UI_GAME_POS_X, UI_GAME_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(UI_GAME_POS_X + SCREEN_WIDTH, UI_GAME_POS_Y + SCREEN_HEIGHT, 0.0f);

	//rhwの設定(必ず1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

	//texture頂点
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//仮想アドレス解放
	g_pVtxBufferUIGame->Unlock();

	return S_OK;
}