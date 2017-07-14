/////////////////////////////////////////////////////////////////////////////////
// title_logo.cpp
// Author:Meguro Chisato
// タイトルロゴ
/////////////////////////////////////////////////////////////////////////////////

#include "title_logo.h"
#include "sound.h"
#include "input.h"
#include "joystick.h"

// マクロ定義
#define TITLE_LOGO_TEXTURE_NAME ("data/TEXTURE/title_logo.png")	// テクスチャのファイルパス
#define TITLE_LOGO_WIDTH (1328)	// 横幅
#define TITLE_LOGO_HEIGHT (295)	// 縦幅
#define EXPLOTION_TEXTURE_NAME ("data/TEXTURE/baku.png")	// テクスチャのファイルパス
#define EXPLOTION_WIDTH (1250)	// 横幅
#define EXPLOTION_HEIGHT (625)	// 縦幅
#define EXPLOTION_POS (D3DXVECTOR3(-330, -30, 0))
#define ENTER_TEXTURE_NAME ("data/TEXTURE/press_enter.png")	// テクスチャのファイルパス
#define ENTER_WIDTH (434)	// 横幅
#define ENTER_HEIGHT (93)	// 縦幅
#define ENTER_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0))

// メンバ変数
LPDIRECT3DTEXTURE9 TitleLogo::s_pTextureTitle_ = nullptr;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 TitleLogo::s_pVtxBufferTitle_ = nullptr;//頂点バッファ管理インターフェースポインタ
LPDIRECT3DTEXTURE9 TitleLogo::s_pTextureExp_ = nullptr;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 TitleLogo::s_pVtxBufferExp_ = nullptr;//頂点バッファ管理インターフェースポインタ
LPDIRECT3DTEXTURE9 TitleLogo::s_pTextureEnter_ = nullptr;//textureインターフェース
LPDIRECT3DVERTEXBUFFER9 TitleLogo::s_pVtxBufferEnter_ = nullptr;//頂点バッファ管理インターフェースポインタ

// 初期化
bool TitleLogo::Init(const D3DXVECTOR3 &pos, float rot, D3DXVECTOR3 &scale)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	pos_ = pos;
	rot_ = rot;
	scl_ = scale;
	col_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	state_ = LOGO_EXPLOTION;
	posExp_ = pos + EXPLOTION_POS;
	sclExp_ = scale * 0.5f;
	uv_ = 0;
	colEnter_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	if(!s_pVtxBufferTitle_)
	{
		if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&s_pVtxBufferTitle_,//
		NULL//
		)))
		{
			return false;
		}

		//仮想アドレスを取得するためのポインタ
		VERTEX_2D *pVtx;

		//頂点バッファをロックして、仮想アドレスを取得する
		s_pVtxBufferTitle_->Lock(
			0,//範囲
			0,//範囲
			(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
			0);

		// 対角線の長さ
		length_ = sqrtf(((TITLE_LOGO_WIDTH * scl_.x) / 2.0f) * ((TITLE_LOGO_WIDTH * scl_.x) / 2.0f) + ((TITLE_LOGO_HEIGHT * scl_.y) / 2.0f) * ((TITLE_LOGO_HEIGHT * scl_.y) / 2.0f));

		// 対角線の角度
		angle_ = atan2f(((TITLE_LOGO_WIDTH * scl_.x) / 2.0f), ((TITLE_LOGO_HEIGHT * scl_.y) / 2.0f));

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI + angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(length_ * sinf(rot_ - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(length_ * sinf(rot_ + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + angle_) + pos_.y - 0.5f, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		//texture頂点
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//仮想アドレス解放
		s_pVtxBufferTitle_->Unlock();
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if(!s_pTextureTitle_)
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice, TITLE_LOGO_TEXTURE_NAME, &s_pTextureTitle_)))
		{
			MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		}
	}

	if(!s_pVtxBufferExp_)
	{
		if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&s_pVtxBufferExp_,//
		NULL//
		)))
		{
			return false;
		}

		//頂点バッファをロックして、仮想アドレスを取得する
		s_pVtxBufferExp_->Lock(
			0,//範囲
			0,//範囲
			(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
			0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		//texture頂点
		pVtx[0].tex = D3DXVECTOR2((0.25f * uv_), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * uv_), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 1.0f);

		//仮想アドレス解放
		s_pVtxBufferExp_->Unlock();
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if(!s_pTextureExp_)
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice, EXPLOTION_TEXTURE_NAME, &s_pTextureExp_)))
		{
			MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		}
	}

	if(!s_pVtxBufferEnter_)
	{
		if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,//頂点バッファの使用方法
		FVF_VERTEX_2D,//書かなくても大丈夫
		D3DPOOL_MANAGED,//メモリ管理方法(managed：デバイスにお任せ)
		&s_pVtxBufferEnter_,//
		NULL//
		)))
		{
			return false;
		}

		//頂点バッファをロックして、仮想アドレスを取得する
		s_pVtxBufferEnter_->Lock(
			0,//範囲
			0,//範囲
			(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
			0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(ENTER_POS.x - ENTER_WIDTH * 0.5f, ENTER_POS.y - ENTER_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENTER_POS.x + ENTER_WIDTH * 0.5f, ENTER_POS.y - ENTER_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(ENTER_POS.x - ENTER_WIDTH * 0.5f, ENTER_POS.y + ENTER_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENTER_POS.x + ENTER_WIDTH * 0.5f, ENTER_POS.y + ENTER_HEIGHT * 0.5f, 0.0f);

		//rhwの設定(必ず1.0f)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color = colEnter_;
		pVtx[1].color = colEnter_;
		pVtx[2].color = colEnter_;
		pVtx[3].color = colEnter_;

		//texture頂点
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//仮想アドレス解放
		s_pVtxBufferExp_->Unlock();
	}

	//ハードディスクからTextureの読み込み
	//※エラーチェック必須
	if(!s_pTextureEnter_)
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice, ENTER_TEXTURE_NAME, &s_pTextureEnter_)))
		{
			MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		}
	}


	return true;
}

// 終了
void TitleLogo::Uninit()
{
	if(s_pTextureTitle_)
	{
		s_pTextureTitle_->Release();
		s_pTextureTitle_ = nullptr;
	}

	if(s_pVtxBufferTitle_)
	{
		s_pVtxBufferTitle_->Release();
		s_pVtxBufferTitle_ = nullptr;
	}

	if(s_pTextureExp_)
	{
		s_pTextureExp_->Release();
		s_pTextureExp_ = nullptr;
	}

	if(s_pVtxBufferExp_)
	{
		s_pVtxBufferExp_->Release();
		s_pVtxBufferExp_ = nullptr;
	}

	if(s_pTextureEnter_)
	{
		s_pTextureEnter_->Release();
		s_pTextureEnter_ = nullptr;
	}

	if(s_pVtxBufferEnter_)
	{
		s_pVtxBufferEnter_->Release();
		s_pVtxBufferEnter_ = nullptr;
	}
}

// 更新
void TitleLogo::Update()
{
	Anim();

	if(!isPushKey_ && (GetJoystickTrigger(0, DIJ_BUTTON, DIJ_PS4_◯) || GetKeyboardTrigger(DIK_SPACE)))
	{
		isPushKey_ = true;
	}

	time_ += 0.1f;
	timeEnter_ += 0.1f;

	if(time_ > 1.0f)
	{
		time_ = 0;
		++uv_;
		if(uv_ > 3)
		{
			uv_ = 0;
		}
	}

	if(!isPushKey_)
	{
		if(timeEnter_ > 1.0f)
		{
			timeEnter_ = 0;

			if(colEnter_.a >= 1.0f)
			{
				colEnter_.a = 0;
			}
			else
			{
				colEnter_.a = 1.0f;
			}
		}
	}
	else
	{
		if(timeEnter_ > 0.2f)
		{
			timeEnter_ = 0;

			if(colEnter_.a >= 1.0f)
			{
				colEnter_.a = 0;
			}
			else
			{
				colEnter_.a = 1.0f;
			}
		}
	}

	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	if(s_pVtxBufferExp_)
	{
		//頂点バッファをロックして、仮想アドレスを取得する
		s_pVtxBufferExp_->Lock(
			0,//範囲
			0,//範囲
			(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
			0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		pVtx[0].tex = D3DXVECTOR2((0.25f * uv_), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * uv_), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 1.0f);

		//仮想アドレス解放
		s_pVtxBufferExp_->Unlock();
	}

	if(s_pVtxBufferTitle_)
	{

		//頂点バッファをロックして、仮想アドレスを取得する
		s_pVtxBufferTitle_->Lock(
			0,//範囲
			0,//範囲
			(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
			0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI + angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(length_ * sinf(rot_ - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(length_ * sinf(rot_ + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + angle_) + pos_.y - 0.5f, 0.0f);

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		//仮想アドレス解放
		s_pVtxBufferTitle_->Unlock();
	}

	if(s_pVtxBufferEnter_)
	{
		//頂点バッファをロックして、仮想アドレスを取得する
		s_pVtxBufferEnter_->Lock(
			0,//範囲
			0,//範囲
			(void**)&pVtx,//アドレスが書かれたメモ帳のアドレス
			0);

		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color = colEnter_;
		pVtx[1].color = colEnter_;
		pVtx[2].color = colEnter_;
		pVtx[3].color = colEnter_;

		//仮想アドレス解放
		s_pVtxBufferEnter_->Unlock();
	}
}

// 描画
void TitleLogo::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイス取得

	//Textureの設定
	pDevice->SetTexture(0, s_pTextureExp_);

	pDevice->SetStreamSource(
		0,//ストリーム番号
		s_pVtxBufferExp_,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);

	//Textureの設定
	pDevice->SetTexture(0, s_pTextureTitle_);

	pDevice->SetStreamSource(
		0,//ストリーム番号
		s_pVtxBufferTitle_,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);

	//Textureの設定
	pDevice->SetTexture(0, s_pTextureEnter_);

	pDevice->SetStreamSource(
		0,//ストリーム番号
		s_pVtxBufferEnter_,
		0,//オフセット（開始位置）
		sizeof(VERTEX_2D));//ストライド量

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//プリミティブ描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//オフセット（頂点数）
		NUM_POLYGON);
}

void TitleLogo::Anim()
{
	switch(state_)
	{
	case LOGO_FADE_IN:
		col_.a += 0.01f;

		if(col_.a >= 1.0f)
		{
			col_.a = 1.0f;
			state_ = LOGO_EXPLOTION;
		}
		break;

	case LOGO_EXPLOTION:

		break;
	}
}

void TitleLogo::StartAnim()
{
	state_ = LOGO_EXPLOTION;
}