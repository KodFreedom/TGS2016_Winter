/////////////////////////////////////////////////////////////////////////////////
// title_logo.h
// Author:Meguro Chisato
// タイトルロゴ
/////////////////////////////////////////////////////////////////////////////////

#include "main.h"

class TitleLogo
{
public:
	enum LOGO_STATE
	{
		LOGO_NONE = 0,
		LOGO_FADE_IN,
		LOGO_EXPLOTION,
		LOGO_FINISH,
		LOGO_STATE_MAX
	};

	TitleLogo():pos_(D3DXVECTOR3(0, 0, 0)), rot_(0.0f), scl_(D3DXVECTOR3(1.0f, 1.0f, 1.0f)), length_(0), angle_(0), col_(D3DCOLOR_RGBA(255, 255, 255, 0)), state_(LOGO_NONE), time_(0), timeEnter_(0), isPushKey_(false){}
	virtual ~TitleLogo() {}

	bool Init(const D3DXVECTOR3 &pos, float rot, D3DXVECTOR3 &scale);
	void Uninit();
	void Update();
	void Draw();
	void StartAnim();
	void SetColor(D3DXCOLOR &color) { col_ = color; }

private:
	void Anim();

	static LPDIRECT3DTEXTURE9 s_pTextureTitle_;//textureインターフェース
	static LPDIRECT3DVERTEXBUFFER9 s_pVtxBufferTitle_;//頂点バッファ管理インターフェースポインタ
	static LPDIRECT3DTEXTURE9 s_pTextureExp_;//textureインターフェース
	static LPDIRECT3DVERTEXBUFFER9 s_pVtxBufferExp_;//頂点バッファ管理インターフェースポインタ
	static LPDIRECT3DTEXTURE9 s_pTextureEnter_;//textureインターフェース
	static LPDIRECT3DVERTEXBUFFER9 s_pVtxBufferEnter_;//頂点バッファ管理インターフェースポインタ

	D3DXVECTOR3 pos_;
	float rot_;
	D3DXVECTOR3 scl_;
	float length_;
	float angle_;
	D3DXCOLOR col_;
	LOGO_STATE state_;
	D3DXVECTOR3 posExp_;
	D3DXVECTOR3 sclExp_;
	int uv_;
	float time_;
	float timeEnter_;
	D3DXCOLOR colEnter_;
	bool isPushKey_;
};