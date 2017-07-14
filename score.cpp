//--------------------------------------------------------------------------------
//
//�@game.cpp
//--------------------------------------------------------------------------------
#include "main.h"
#include "score.h"
#include "fade.h"



//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define NUM_TEXTURE_SCORE (2)		//�g�p����e�N�X�`���̐�

#define SCORE_POS_X (1200.0f)		//�X�R�A�̕\���ʒux(�ꌅ�ڍ��㒸�_x���W)
#define SCORE_POS_Y (650.0f)		//�X�R�A�̕\���ʒuy
#define SCORE_WIDTH (50.0f)		//�X�R�A�̕����̉���
#define SCORE_HEIGHT (50.0f)		//�X�R�A�̕����̏c��
#define SCORE_BETEWEEN (2.0f)		//�X�R�A�̎���
#define SCORE_DIGIT (5)				//�X�R�A�̌���


#define NUM_SCORE_OBJECT (100)				//�����̍ő�\����
#define SCORE_OBJECT_POS_X (1000.0f)		//�����̕\���ʒux���W
#define SCORE_OBJECT_POS_Y (600.0f)			//�����̕\���ʒuy���W
#define SCORE_OBJECT_WIDTH (200.0f)			//�����|���S���̉���
#define SCORE_OBJECT_HEIGHT (200.0f)		//�����|���S���̉���
#define SCORE_OBJECT_DIFFERENCE_Y (30.0f)	//�������ςݏd�Ȃ�ۂ�y���W�ω���
#define SCORE_OBJECT_DIFFERENCE_X (60.0f)	//�������ςݏd�Ȃ�ۂ�x���W�ω���
#define SCORE_DEVISION (200)				//�������ǉ������X�R�A

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureScore[NUM_TEXTURE_SCORE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferScoreObject = NULL;


int g_nScoreValue = 0;										//�X�R�A


// �G�e�N�X�`���̃t�@�C����
const char *c_apTextureScore[NUM_TEXTURE_SCORE]=
{
	"data/TEXTURE/scoreNumber.png",
	"data/TEXTURE/kane-01.png",
};


//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void InitScore(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if(pDevice == NULL)
	{
		MessageBox(NULL,"�f�o�C�X�擾�Ɏ��s���܂����B","�G���[(InitScore)",MB_OK);
		return;
	}


	//�e�N�X�`���Ǎ�
	for(int n = 0; n < NUM_TEXTURE_SCORE; n++)
	{
		D3DXCreateTextureFromFile(pDevice,	//�f�o�C�X�ւ̃|�C���^
			c_apTextureScore[n],			//�t�@�C���̖��O
			&g_pTextureScore[n]);			//�ǂݍ��ރ������[
	}



	//���_���쐬
	if(FAILED(MakeVertexScore(pDevice)))
	{
		MessageBox(NULL,"���_���̍쐬�Ɏ��s","�G���[(InitScore)",MB_OK);
		return;
	}


	//�X�R�A�̃��Z�b�g
	g_nScoreValue = 0;

}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UninitScore(void)
{
	//�e�N�X�`���̊J��
	for(int n = 0; n < NUM_TEXTURE_SCORE; n++){
		if(g_pTextureScore[n] != NULL){
			g_pTextureScore[n]->Release();
			g_pTextureScore[n] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
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
//  �X�V����
//--------------------------------------------------------------------------------
void UpdateScore(void)
{
	int number;
	int value = (int)g_nScoreValue;
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferScore->Lock(0, 0, (void**)&pVtx, 0);

	for(int n = 0; n < SCORE_DIGIT; n++, pVtx+=4)
	{
		//1���̒l���擾
		number = value % 10;

		//uv���W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(number * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(number * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(number * 0.1f + 0.1f, 1.0f);

		//����1���炷
		value /= 10;
	}

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferScore->Unlock();


	//�Q�[���I�������`�F�b�N
	if(GetScore() >= FINISH_SCORE)
	{
		SetFade(FADE_OUT,MODE_RESULT);
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DrawScore(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetFVF(FVF_VERTEX_2D);


	pDevice->SetStreamSource(0, g_pVtxBufferScoreObject, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, g_pTextureScore[1]);
	int numDrawObject = g_nScoreValue / SCORE_DEVISION;//�`�搔�̌v�Z
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
//  �X�R�A�w��
//--------------------------------------------------------------------------------
void SetScore(int nScoreValue)
{
	g_nScoreValue = nScoreValue;
}

//--------------------------------------------------------------------------------
//  �X�R�A�擾
//--------------------------------------------------------------------------------
int GetScore(void)
{
	return g_nScoreValue;
}

//--------------------------------------------------------------------------------
//  �X�R�A���Z
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
//  ���_�쐬
//--------------------------------------------------------------------------------
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	//�����`�F�b�N
	if(pDevice == NULL)
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̍쐬
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
		//���_���W�̐ݒ�(2D���W�A�E���)
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

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color =
			pVtx[1].color =
			pVtx[2].color =
			pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//uv�l(�e�N�X�`�����W)�̐ݒ�
		pVtx[0].tex=D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex=D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex=D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex=D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@�̃A�����b�N(GPU���`��J�n�o����)
	g_pVtxBufferScore->Unlock();



	//------------------------�Q�[�W�̂����̔|��---------------------
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
		//���_���W�̐ݒ�(2D���W�A�E���)
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

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color =
			pVtx[1].color =
			pVtx[2].color =
			pVtx[3].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//uv�l(�e�N�X�`�����W)�̐ݒ�
		pVtx[0].tex=D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex=D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex=D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex=D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@�̃A�����b�N(GPU���`��J�n�o����)
	g_pVtxBufferScoreObject->Unlock();


	return S_OK;

}

