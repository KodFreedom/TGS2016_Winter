//--------------------------------------------------------------------------------
//
//�@UI_life.cpp
//	Author : Sakai
//	Date   : 2016-12-03
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "UI_life.h"
#include "main.h"


//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define UI_FIELD_WIDTH  (980)
#define UI_FIELD_POSY   (660)

#define UILIFE_WIDTH    (150)
#define UILIFE_HEIGHT   (60)

#define LIFE_NUM_WIDTH  (80)
#define LIFE_NUM_HEIGHT (100)

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pUILifeTex[2] = {};
LPDIRECT3DVERTEXBUFFER9 g_pUILifeVtx[2] = {};
D3DXVECTOR3 pos_UILife[2], rot_UILife[2];

float fLengthUILife[2];  // �Ίp���̒����A���a
float fAngleUILife[2];   // �Ίp���̊p�x

int nLife = 0;

LPDIRECT3DTEXTURE9 g_pBGLifeTex = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pBGLifeVtx = NULL;


//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitUILife(void)
{
	VERTEX_2D *pVtx;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();


    //-------------------------------------------------------------------------BG
	pDevice->CreateVertexBuffer( sizeof(VERTEX_2D)*4,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pBGLifeVtx,
									NULL);

    g_pBGLifeVtx->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].pos = D3DXVECTOR3(385.0f, 600.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(620.0f, 600.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(385.0f, 720.0f, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(620.0f, 720.0f, 0.0f);

    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    pVtx[0].color = D3DCOLOR_RGBA( 0, 255, 255, 255 );
    pVtx[1].color = D3DCOLOR_RGBA( 0, 255, 255, 255 );
    pVtx[2].color = D3DCOLOR_RGBA( 0, 255, 255, 255 );
    pVtx[3].color = D3DCOLOR_RGBA( 0, 255, 255, 255 );

    pVtx[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
    pVtx[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
    pVtx[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
    pVtx[3].tex = D3DXVECTOR2( 1.0f, 1.0f );

    g_pBGLifeVtx->Unlock();
    //-------------------------------------------------------------------------



    fLengthUILife[0] = sqrtf(((UILIFE_WIDTH / 2) * (UILIFE_WIDTH / 2)) + ((UILIFE_HEIGHT / 2) * (UILIFE_HEIGHT / 2)));
    fAngleUILife[0] = atan2f( (UILIFE_HEIGHT / 2), (UILIFE_WIDTH / 2));

    fLengthUILife[1] = sqrtf(((LIFE_NUM_WIDTH / 2) * (LIFE_NUM_WIDTH / 2)) + ((LIFE_NUM_HEIGHT / 2) * (LIFE_NUM_HEIGHT / 2)));
    fAngleUILife[1] = atan2f( (LIFE_NUM_HEIGHT / 2), (LIFE_NUM_WIDTH / 2));


    pos_UILife[0] = D3DXVECTOR3((UI_FIELD_WIDTH / 5.0f) * 2 + (UILIFE_WIDTH * 0.5f),
                                UI_FIELD_POSY + (UILIFE_HEIGHT * 0.5f),
                                0.0f);
    pos_UILife[1] = D3DXVECTOR3((UI_FIELD_WIDTH / 5.0f) * 2.8f + (LIFE_NUM_WIDTH * 0.5f),
                                UI_FIELD_POSY + (LIFE_NUM_HEIGHT * 0.5f) - 40,
                                0.0f);

    for(int i = 0; i < 2; i++){
        rot_UILife[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }

    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/life.png", &g_pUILifeTex[0] );
    D3DXCreateTextureFromFile( pDevice, "data/TEXTURE/num.png", &g_pUILifeTex[1] );

    for(int i = 0; i < 2; i++){
	    pDevice->CreateVertexBuffer( sizeof(VERTEX_2D)*4,
									    D3DUSAGE_WRITEONLY,
									    FVF_VERTEX_2D,
									    D3DPOOL_MANAGED,
									    &g_pUILifeVtx[i],
									    NULL);

        g_pUILifeVtx[i]->Lock(0, 0, (void**)&pVtx, 0);

        pVtx[0].pos = D3DXVECTOR3(pos_UILife[i].x + (cosf((rot_UILife[i].z + fAngleUILife[i]) + D3DX_PI) * fLengthUILife[i]),
                                    pos_UILife[i].y + (sinf((rot_UILife[i].z + fAngleUILife[i]) + D3DX_PI) * fLengthUILife[i]),
                                    0.0f);
        pVtx[1].pos = D3DXVECTOR3(pos_UILife[i].x + (cosf((rot_UILife[i].z - fAngleUILife[i]) + (D3DX_PI * 2)) * fLengthUILife[i]),
                                    pos_UILife[i].y + (sinf((rot_UILife[i].z - fAngleUILife[i]) + (D3DX_PI * 2)) * fLengthUILife[i]),
                                    0.0f);
        pVtx[2].pos = D3DXVECTOR3(pos_UILife[i].x + (cosf((rot_UILife[i].z - fAngleUILife[i]) + D3DX_PI) * fLengthUILife[i]),
                                    pos_UILife[i].y + (sinf((rot_UILife[i].z - fAngleUILife[i]) + D3DX_PI) * fLengthUILife[i]),
                                    0.0f);
        pVtx[3].pos = D3DXVECTOR3(pos_UILife[i].x + (cosf(rot_UILife[i].z + fAngleUILife[i]) * fLengthUILife[i]),
                                    pos_UILife[i].y + (sinf(rot_UILife[i].z + fAngleUILife[i]) * fLengthUILife[i]),
                                    0.0f);

        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        switch(i){
        case 0:
            pVtx[0].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
            pVtx[1].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
            pVtx[2].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
            pVtx[3].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );

            pVtx[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
            pVtx[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
            pVtx[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
            pVtx[3].tex = D3DXVECTOR2( 1.0f, 1.0f );
            break;
        case 1:
            pVtx[0].color = D3DCOLOR_RGBA( 255, 0, 0, 255 );
            pVtx[1].color = D3DCOLOR_RGBA( 255, 0, 0, 255 );
            pVtx[2].color = D3DCOLOR_RGBA( 255, 0, 0, 255 );
            pVtx[3].color = D3DCOLOR_RGBA( 255, 0, 0, 255 );

            pVtx[0].tex = D3DXVECTOR2( 0.1f * nLife, 0.0f );
            pVtx[1].tex = D3DXVECTOR2( 0.1f * (nLife + 1), 0.0f );
            pVtx[2].tex = D3DXVECTOR2( 0.1f * nLife, 1.0f );
            pVtx[3].tex = D3DXVECTOR2( 0.1f * (nLife + 1), 1.0f );
            break;
        default:
            break;
        }

        g_pUILifeVtx[i]->Unlock();
    }
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitUILife(void)
{
    for(int i = 0; i < 2; i++){
        if(g_pUILifeTex[i] != NULL){
            g_pUILifeTex[i]->Release();
            g_pUILifeTex[i] = NULL;
        }

        if(g_pUILifeVtx[i] != NULL){
            g_pUILifeVtx[i]->Release();
            g_pUILifeVtx[i] = NULL;
        }
    }

    if(g_pBGLifeTex != NULL){
        g_pBGLifeTex->Release();
        g_pBGLifeTex = NULL;
    }

    if(g_pBGLifeVtx != NULL){
        g_pBGLifeVtx->Release();
        g_pBGLifeVtx = NULL;
    }

}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateUILife(void)
{
    VERTEX_2D *pVtx;
    g_pUILifeVtx[1]->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].tex = D3DXVECTOR2( 0.1f * nLife, 0.0f );
    pVtx[1].tex = D3DXVECTOR2( 0.1f * (nLife + 1), 0.0f );
    pVtx[2].tex = D3DXVECTOR2( 0.1f * nLife, 1.0f );
    pVtx[3].tex = D3DXVECTOR2( 0.1f * (nLife + 1), 1.0f );

    g_pUILifeVtx[1]->Unlock();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawUILife(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    pDevice->SetStreamSource(0,g_pBGLifeVtx, 0, sizeof(VERTEX_2D));
    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF( FVF_VERTEX_2D );
    pDevice->SetTexture(0, g_pBGLifeTex);
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    for(int i = 0; i < 2; i++){
        pDevice->SetStreamSource(0,g_pUILifeVtx[i], 0, sizeof(VERTEX_2D));
        // ���_�t�H�[�}�b�g�̐ݒ�
        pDevice->SetFVF( FVF_VERTEX_2D );
        pDevice->SetTexture(0, g_pUILifeTex[i]);
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    }
}


void SetLife(int life){
    nLife = life;
}
