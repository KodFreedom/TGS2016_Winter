//=============================================================================
//
// �������� [explosion.cpp]
//
//=============================================================================
#include "explosionEffect.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define	TEXTURE_EXPLOSION	"data/TEXTURE/explosion000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	EXPLOSION_POS_X		(0.0f)							// �����̕\����ʒu�w���W
#define	EXPLOSION_POS_Y		(0.0f)							// �����̕\����ʒu�x���W
#define	EXPLOSION_SIZE_X	(160.0f)							// �����̕�
#define	EXPLOSION_SIZE_Y	(160.0f)							// �����̍���

#define	MAX_EXPLOSION		(100)					// �����̍ő吔

#define	NUM_PATTERN_X			(8)						// �A�j���[�V�����̃p�^�[����
#define	NUM_PATTERN_Y			(2)						// �A�j���[�V�����̃p�^�[����
#define	TEXTURE_SIZE_X		(1.0f / NUM_PATTERN_X)		// �e�N�X�`�����̂P�p�^�[���̕�
#define	TEXTURE_SIZE_Y		(1.0f / NUM_PATTERN_Y)		// �e�N�X�`�����̂P�p�^�[���̍���

#define	INTERVAL_PATTERN	(5)						// �A�j���[�V�����̐؂�ւ��Ԋu

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
	D3DXVECTOR3 move;	// �ړ���
	D3DXCOLOR col;		// �F
	int nCount;			// �J�E���^
	int nPatternX;		// ���p�^�[����
	int nPatternY;		// �c�p�^�[����
	int nPatternNo;		// �p�^�[��No.
	int nType;			// ���
	bool bUse;			// �g�p���Ă��邩�ǂ���
} EXPLOSION_EFFECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void SetVertexExplosion(int nExplosionID);
void SetColorExplosion(int nExplosionID, D3DXCOLOR col);
void SetTextureExplosion(int nExplosionID);

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion[EXPLOSIONTYPE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

EXPLOSION_EFFECT			g_aExplosion[MAX_EXPLOSION];	// �����̃��[�N

														// �e�N�X�`���̃t�@�C����
const char *c_apTexturExplosion[EXPLOSIONTYPE_MAX] =
{
	"data/TEXTURE/baku.png",
	"data/TEXTURE/baku2.png",
	"data/TEXTURE/baku3.png",
	"data/TEXTURE/baku4.png",
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitExplosionEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ResetExplosion();

	//�e�N�X�`���Ǎ�
	for (int n = 0; n < EXPLOSIONTYPE_MAX; n++)
	{
		D3DXCreateTextureFromFile(pDevice,	//�f�o�C�X�ւ̃|�C���^
			c_apTexturExplosion[n],			//�t�@�C���̖��O
			&g_pTextureExplosion[n]);			//�ǂݍ��ރ������[
	}

	//���_���쐬
	MakeVertexExplosion(pDevice);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosionEffect(void)
{
	for (int i = 0; i < EXPLOSIONTYPE_MAX; i++)
	{
		if (g_pTextureExplosion[i] != NULL)
		{// �e�N�X�`���̊J��
			g_pTextureExplosion[i]->Release();
			g_pTextureExplosion[i] = NULL;
		}
	}

	if (g_pVtxBuffExplosion != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateExplosionEffect(void)
{
	EXPLOSION_EFFECT *pExplosion;

	pExplosion = &g_aExplosion[0];
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{// �������g�p���Ă���

			pExplosion->nCount++;
			if ((pExplosion->nCount % INTERVAL_PATTERN) == 0)
			{// �A�j���[�V�����؂�ւ�
				pExplosion->nPatternNo++;
				if (pExplosion->nPatternNo >= pExplosion->nPatternX * pExplosion->nPatternY)
				{// �A�j���[�V�����p�^�[�����I��
					pExplosion->bUse = false;
				}
				SetTextureExplosion(nCntExplosion);
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosionEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	// �|���S���̕`��
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{// �������g�p���Ă���
		 // �e�N�X�`���̐ݒ�
		 //pDevice->SetTexture(0, NULL);
			pDevice->SetTexture(0, g_pTextureExplosion[g_aExplosion[nCntExplosion].nType]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntExplosion * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_EXPLOSION,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_2D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffExplosion,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nCntExplosion].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);

			// rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(TEXTURE_SIZE_X, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, TEXTURE_SIZE_Y);
			pVtx[3].tex = D3DXVECTOR2(TEXTURE_SIZE_X, TEXTURE_SIZE_Y);
		}

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffExplosion->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexExplosion(int nExplosionID)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nExplosionID * 4);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y - EXPLOSION_SIZE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x - EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nExplosionID].pos.x + EXPLOSION_SIZE_X / 2, g_aExplosion[nExplosionID].pos.y + EXPLOSION_SIZE_Y / 2, 0.0f);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorExplosion(int nExplosionID, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nExplosionID * 4);

	// ���_�J���[�̐ݒ�
	pVtx[0].color =
		pVtx[1].color =
		pVtx[2].color =
		pVtx[3].color = col;

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTextureExplosion(int nExplosionID)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nExplosionID * 4);

	// �e�N�X�`�����W�̐ݒ�
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
	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// �����̐ݒ�
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
// �����̃��Z�b�g
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
