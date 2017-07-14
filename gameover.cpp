//--------------------------------------------------------------------------------
//
//　Gameover.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "input.h"
#include "joystick.h"
#include "gameover.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define GAMEOVER_POS_X (-0.5f)//ポリゴンの表示位置X
#define GAMEOVER_POS_Y (-0.5f)//ポリゴンの表示位置Y
#define GAMEOVER_TEXTURENAME "data/TEXTURE/game_over-01.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexGameover(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureGameover = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGameover = NULL;//頂点バッファ管理インターフェースポインタ

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitGameover(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	if (FAILED(MakeVerTexGameover(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexGameover ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, GAMEOVER_TEXTURENAME, &g_pTextureGameover)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//InitPressEnter();
	PlaySound(SOUND_LABEL_BGM_RESULT_OVER);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitGameover(void)
{
	//safe release
	if (g_pVtxBufferGameover != NULL)
	{
		g_pVtxBufferGameover->Release();
		g_pVtxBufferGameover = NULL;
	}

	if (g_pTextureGameover != NULL)
	{
		g_pTextureGameover->Release();
		g_pTextureGameover = NULL;
	}

	//UninitPressEnter();
	//StopSound(SOUND_LABEL_BGM_RESULT_OVER);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateGameover(void)
{
	//key入力
	if (GetKeyboardTrigger(DIK_SPACE) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_◯))
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawGameover(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferGameover,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTextureGameover);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexGameover(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferGameover,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferGameover->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(GAMEOVER_POS_X, GAMEOVER_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAMEOVER_POS_X + SCREEN_WIDTH, GAMEOVER_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(GAMEOVER_POS_X, GAMEOVER_POS_Y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAMEOVER_POS_X + SCREEN_WIDTH, GAMEOVER_POS_Y + SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBufferGameover->Unlock();

	return S_OK;
}