//--------------------------------------------------------------------------------
//
//　player.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-01
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "input.h"
#include "joystick.h"
#include "bullet.h"
#include "sound.h"
#include "fade.h"
#include "UI_bullet.h"
#include "UI_life.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define PLAYER_POS_X (GAME_WIDTH * 0.5f)//ポリゴンの表示位置X
#define PLAYER_POS_Y (GAME_HEIGHT)//ポリゴンの表示位置Y
#define PLAYER_ROT_SPEED (0.05f)
#define PLAYER_WIDTH	 (100.0f)
#define PLAYER_HEIGHT	 (50.0f)
#define PLAYER_ROT_MIN	(-D3DX_PI * 0.85f)
#define PLAYER_ROT_MAX	(-D3DX_PI * 0.15f)
#define PLAYER_LIFE (9)
#define PLAYER_TEXTURENAME "data/TEXTURE/player.png"//ファイル名

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vVecToPos[4];
	//D3DXVECTOR3 vOrighin;	//発射位置
	float fRot;
	BULLET_TYPE type;
	int nLife;
}PLAYER;

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPlayer = NULL;//頂点バッファ管理インターフェースポインタ
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBG = NULL;//頂点バッファ管理インターフェースポインタ
PLAYER g_player;

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	g_player.vPos = D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0.0f);
	g_player.fRot = -D3DX_PI * 0.5f;
	g_player.vVecToPos[0] = D3DXVECTOR3(0.0f, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[1] = D3DXVECTOR3(PLAYER_WIDTH * 0.5f, -PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[2] = D3DXVECTOR3(0.0f, PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.vVecToPos[3] = D3DXVECTOR3(PLAYER_WIDTH * 0.5f, PLAYER_HEIGHT * 0.5f, 0.0f);
	g_player.type = BULLET_BOMB;
	g_player.nLife = PLAYER_LIFE;
	ChangeBulletType(g_player.type);

	if (FAILED(MakeVerTexPlayer(pDevice)))//textureポインタへのポインタ
	{
		MessageBox(NULL, "MakeVerTexPlayer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURENAME, &g_pTexturePlayer)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/haikei.jpg", &g_pTextureBG)))
	{
		MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitPlayer(void)
{
	//safe release
	if (g_pVtxBufferPlayer != NULL)
	{
		g_pVtxBufferPlayer->Release();
		g_pVtxBufferPlayer = NULL;
	}

	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdatePlayer(void)
{
	if ((GetKeyboardPress(DIK_A) || GetJoystickStickStatus(PS4_CONTROLLER_NUM, DIJ_L_STICKX) < -STICK_MIN) && g_player.fRot > PLAYER_ROT_MIN)
	{
		g_player.fRot -= PLAYER_ROT_SPEED;
	}
	if ((GetKeyboardPress(DIK_D) || GetJoystickStickStatus(PS4_CONTROLLER_NUM, DIJ_L_STICKX) > STICK_MIN) && g_player.fRot < PLAYER_ROT_MAX)
	{
		g_player.fRot += PLAYER_ROT_SPEED;
	}
	if (GetKeyboardTrigger(DIK_Q) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_L1))
	{
		g_player.type = (BULLET_TYPE)((int)g_player.type - 1);
		if (g_player.type == (BULLET_TYPE)(BULLET_BOMB - 1))
		{
			g_player.type = BULLET_WOMAN;
		}
		ChangeBulletType(g_player.type);
		PlaySound(SOUND_LABEL_SE_CHANGE);
	}
	if (GetKeyboardTrigger(DIK_E) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_R1))
	{
		g_player.type = (BULLET_TYPE)((int)g_player.type + 1);
		if (g_player.type == BULLET_MAX)
		{
			g_player.type = BULLET_BOMB;
		}
		ChangeBulletType(g_player.type);
		PlaySound(SOUND_LABEL_SE_CHANGE);
	}
	//key入力
	if (GetKeyboardTrigger(DIK_SPACE) || GetJoystickTrigger(PS4_CONTROLLER_NUM, DIJ_BUTTON, DIJ_PS4_◯))
	{
		D3DXVECTOR3 vDirection;

		vDirection = D3DXVECTOR3(cosf(g_player.fRot), sinf(g_player.fRot), 0.0f);
		SetBullet(g_player.type, g_player.vPos, vDirection);
		PlaySound(SOUND_LABEL_SE_SHOT);
		//SetFade(FADE_OUT, MODE_TUTORIAL);
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferPlayer->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);

	for (int nCnt = 0;nCnt < 4;nCnt++)
	{
		D3DXVECTOR3 vVertex = g_player.vVecToPos[nCnt];

		//AFFIN転換
		vVertex = D3DXVECTOR3(
			g_player.vVecToPos[nCnt].x * cosf(g_player.fRot) - g_player.vVecToPos[nCnt].y * sinf(g_player.fRot),
			g_player.vVecToPos[nCnt].x * sinf(g_player.fRot) + g_player.vVecToPos[nCnt].y * cosf(g_player.fRot),
			0.0f);

		pVtx[nCnt].pos = g_player.vPos + vVertex;
	}

	g_pVtxBufferPlayer->Unlock();

	SetLife(g_player.nLife);
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferBG,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //Textureの設定
	pDevice->SetTexture(0, g_pTextureBG);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);



	pDevice->SetStreamSource(
		0,//ストリーム番号
		g_pVtxBufferPlayer,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//Textureの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);

	
}

//--------------------------------------------------------------------------------
//  頂点の作成
//--------------------------------------------------------------------------------
HRESULT MakeVerTexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferPlayer,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferPlayer->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);


	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定(必ず1.0f)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].color = D3DCOLOR_RGBA(0, 0, 255, 255);
	pVtx[1].color = D3DCOLOR_RGBA(0, 0, 255, 255);
	pVtx[2].color = D3DCOLOR_RGBA(0, 0, 255, 255);
	pVtx[3].color = D3DCOLOR_RGBA(0, 0, 255, 255);

	//texture頂点
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//仮想アドレス解放
	g_pVtxBufferPlayer->Unlock();

	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&g_pVtxBufferBG,//
		NULL//
	)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックして、仮想アドレスを取得する
	g_pVtxBufferBG->Lock(
		0,//範囲
		0,//範囲
		(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
		0);


	//頂点座標の設定（2D座標、右回り）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAME_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, GAME_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAME_WIDTH, GAME_HEIGHT, 0.0f);

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
	g_pVtxBufferBG->Unlock();

	return S_OK;
}

void SubPlayerLife(void)
{
	g_player.nLife--;

	if (g_player.nLife == 0)
	{
		SetFade(FADE_OUT, MODE_GAMEOVER);
	}
}