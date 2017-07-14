//--------------------------------------------------------------------------------
//
//　UI_bullet.cpp
//	Author : Sakai
//	Date   : 2016-12-03
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "UI_bullet.h"
#include "main.h"


//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define UI_FIELD_WIDTH  (980)
#define UI_FIELD_POSY   (600)

#define UIBUL_WIDTH     (120)
#define UIBUL_HEIGHT    (120)

//--------------------------------------------------------------------------------
//  グローバル変数
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pUIBulletTex[4] = {};
LPDIRECT3DVERTEXBUFFER9 g_pUIBulletVtx[4] = {};
D3DXVECTOR3 pos_UIBullet[4], rot_UIBullet[4];
float scl_UIBullet[4];

float fLengthUIBullet;  // 対角線の長さ、半径
float fAngleUIBullet;   // 対角線の角度

int nDirRotate, nDirScale;

bool isChanging = false;
BULLET_TYPE bulType;

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void InitUIBullet(void)
{
    nDirRotate = 1;
    nDirScale = 1;

	VERTEX_2D *pVtx;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    fLengthUIBullet = sqrtf(((UIBUL_WIDTH / 2) * (UIBUL_WIDTH / 2)) + ((UIBUL_HEIGHT / 2) * (UIBUL_HEIGHT / 2)));
    fAngleUIBullet = atan2f( (UIBUL_HEIGHT / 2), (UIBUL_WIDTH / 2));

    pos_UIBullet[0] = D3DXVECTOR3((UI_FIELD_WIDTH / 5.0f) * 0 + (UIBUL_WIDTH * 0.5f) + 38,
                                UI_FIELD_POSY + (UIBUL_HEIGHT * 0.5f),
                                0.0f);
    pos_UIBullet[1] = D3DXVECTOR3((UI_FIELD_WIDTH / 5.0f) * 1 + (UIBUL_WIDTH * 0.5f) + 38,
                                UI_FIELD_POSY + (UIBUL_HEIGHT * 0.5f),
                                0.0f);
    pos_UIBullet[2] = D3DXVECTOR3((UI_FIELD_WIDTH / 5.0f) * 3 + (UIBUL_WIDTH * 0.5f) + 38,
                                UI_FIELD_POSY + (UIBUL_HEIGHT * 0.5f),
                                0.0f);
    pos_UIBullet[3] = D3DXVECTOR3((UI_FIELD_WIDTH / 5.0f) * 4 + (UIBUL_WIDTH * 0.5f) + 38,
                                UI_FIELD_POSY + (UIBUL_HEIGHT * 0.5f),
                                0.0f);

    for(int i = 0; i < 4; i++){
        rot_UIBullet[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        scl_UIBullet[i] = 1.0f;
    }

    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/bom-01.png", &g_pUIBulletTex[0] );
    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/tabako-01.png", &g_pUIBulletTex[1] );
    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/sake-01.png", &g_pUIBulletTex[2] );
    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/mesu-01.png", &g_pUIBulletTex[3] );



    for(int i = 0; i < 4; i++){
	    pDevice->CreateVertexBuffer( sizeof(VERTEX_2D)*4,
									    D3DUSAGE_WRITEONLY,
									    FVF_VERTEX_2D,
									    D3DPOOL_MANAGED,
									    &g_pUIBulletVtx[i],
									    NULL);

	    g_pUIBulletVtx[i]->Lock(0, 0, (void**)&pVtx, 0);
	
        pVtx[0].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf((rot_UIBullet[i].z + fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                    pos_UIBullet[i].y + (sinf((rot_UIBullet[i].z + fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                    0.0f);
        pVtx[1].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf((rot_UIBullet[i].z - fAngleUIBullet) + (D3DX_PI * 2)) * fLengthUIBullet),
                                    pos_UIBullet[i].y + (sinf((rot_UIBullet[i].z - fAngleUIBullet) + (D3DX_PI * 2)) * fLengthUIBullet),
                                    0.0f);
        pVtx[2].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf((rot_UIBullet[i].z - fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                    pos_UIBullet[i].y + (sinf((rot_UIBullet[i].z - fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                    0.0f);
        pVtx[3].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf(rot_UIBullet[i].z + fAngleUIBullet) * fLengthUIBullet),
                                    pos_UIBullet[i].y + (sinf(rot_UIBullet[i].z + fAngleUIBullet) * fLengthUIBullet),
                                    0.0f);

        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        // 頂点カラー
        pVtx[0].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );
        pVtx[1].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );
        pVtx[2].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );
        pVtx[3].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );

        pVtx[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
        pVtx[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
        pVtx[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
        pVtx[3].tex = D3DXVECTOR2( 1.0f, 1.0f );

        g_pUIBulletVtx[i]->Unlock();
    }
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UninitUIBullet(void)
{
    for(int i = 0; i < 4; i++){
        if(g_pUIBulletTex[i] != NULL){
            g_pUIBulletTex[i]->Release();
            g_pUIBulletTex[i] = NULL;
        }

        if(g_pUIBulletVtx[i] != NULL){
            g_pUIBulletVtx[i]->Release();
            g_pUIBulletVtx[i] = NULL;
        }
    }
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void UpdateUIBullet(void)
{
    if(isChanging){
        rot_UIBullet[bulType].z += 0.2f * nDirRotate;
        if(fabs(rot_UIBullet[bulType].z) > (D3DX_PI / 4))
            nDirRotate *= -1;

        scl_UIBullet[bulType] += 0.05f * nDirScale;
        if(scl_UIBullet[bulType] > 1.5f)
            nDirScale *= -1;
        else if(scl_UIBullet[bulType] < 1.0f){
            rot_UIBullet[bulType].z = 0.0f;
            scl_UIBullet[bulType] = 1.0f;
            nDirRotate = 1;
            nDirScale = 1;
            isChanging = false;
        }

        VERTEX_2D *pVtx;
        g_pUIBulletVtx[bulType]->Lock(0, 0, (void**)&pVtx, 0);

        pVtx[0].pos = D3DXVECTOR3(pos_UIBullet[bulType].x + (cosf((rot_UIBullet[bulType].z + fAngleUIBullet) + D3DX_PI) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    pos_UIBullet[bulType].y + (sinf((rot_UIBullet[bulType].z + fAngleUIBullet) + D3DX_PI) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    0.0f);
        pVtx[1].pos = D3DXVECTOR3(pos_UIBullet[bulType].x + (cosf((rot_UIBullet[bulType].z - fAngleUIBullet) + (D3DX_PI * 2)) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    pos_UIBullet[bulType].y + (sinf((rot_UIBullet[bulType].z - fAngleUIBullet) + (D3DX_PI * 2)) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    0.0f);
        pVtx[2].pos = D3DXVECTOR3(pos_UIBullet[bulType].x + (cosf((rot_UIBullet[bulType].z - fAngleUIBullet) + D3DX_PI) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    pos_UIBullet[bulType].y + (sinf((rot_UIBullet[bulType].z - fAngleUIBullet) + D3DX_PI) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    0.0f);
        pVtx[3].pos = D3DXVECTOR3(pos_UIBullet[bulType].x + (cosf(rot_UIBullet[bulType].z + fAngleUIBullet) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    pos_UIBullet[bulType].y + (sinf(rot_UIBullet[bulType].z + fAngleUIBullet) * fLengthUIBullet * scl_UIBullet[bulType]),
                                    0.0f);

        g_pUIBulletVtx[bulType]->Unlock();
    }
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void DrawUIBullet(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    for(int i = 0; i < 4; i++){
        pDevice->SetStreamSource(0,g_pUIBulletVtx[i], 0, sizeof(VERTEX_2D));

        // 頂点フォーマットの設定
        pDevice->SetFVF( FVF_VERTEX_2D );
        pDevice->SetTexture(0, g_pUIBulletTex[i]);
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    }
}


void ChangeBulletType(BULLET_TYPE type){

    VERTEX_2D *pVtx;

    for(int i = 0; i < 4; i++){
        rot_UIBullet[i].z = 0.0f;
        scl_UIBullet[i] = 1.0f;
        nDirRotate = 1;
        nDirScale = 1;

        g_pUIBulletVtx[i]->Lock(0, 0, (void**)&pVtx, 0);
        if(i == type){
            pVtx[0].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
            pVtx[1].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
            pVtx[2].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
            pVtx[3].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
        }
        else{
            pVtx[0].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf((rot_UIBullet[i].z + fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                        pos_UIBullet[i].y + (sinf((rot_UIBullet[i].z + fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                        0.0f);
            pVtx[1].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf((rot_UIBullet[i].z - fAngleUIBullet) + (D3DX_PI * 2)) * fLengthUIBullet),
                                        pos_UIBullet[i].y + (sinf((rot_UIBullet[i].z - fAngleUIBullet) + (D3DX_PI * 2)) * fLengthUIBullet),
                                        0.0f);
            pVtx[2].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf((rot_UIBullet[i].z - fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                        pos_UIBullet[i].y + (sinf((rot_UIBullet[i].z - fAngleUIBullet) + D3DX_PI) * fLengthUIBullet),
                                        0.0f);
            pVtx[3].pos = D3DXVECTOR3(pos_UIBullet[i].x + (cosf(rot_UIBullet[i].z + fAngleUIBullet) * fLengthUIBullet),
                                        pos_UIBullet[i].y + (sinf(rot_UIBullet[i].z + fAngleUIBullet) * fLengthUIBullet),
                                        0.0f);

            pVtx[0].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );
            pVtx[1].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );
            pVtx[2].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );
            pVtx[3].color = D3DCOLOR_RGBA( 50, 50, 50, 255 );
        }
        g_pUIBulletVtx[i]->Unlock();
    }
	isChanging = true;
    bulType = type;
}
