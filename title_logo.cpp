/////////////////////////////////////////////////////////////////////////////////
// title_logo.cpp
// Author:Meguro Chisato
// �^�C�g�����S
/////////////////////////////////////////////////////////////////////////////////

#include "title_logo.h"
#include "sound.h"
#include "input.h"
#include "joystick.h"

// �}�N����`
#define TITLE_LOGO_TEXTURE_NAME ("data/TEXTURE/title_logo.png")	// �e�N�X�`���̃t�@�C���p�X
#define TITLE_LOGO_WIDTH (1328)	// ����
#define TITLE_LOGO_HEIGHT (295)	// �c��
#define EXPLOTION_TEXTURE_NAME ("data/TEXTURE/baku.png")	// �e�N�X�`���̃t�@�C���p�X
#define EXPLOTION_WIDTH (1250)	// ����
#define EXPLOTION_HEIGHT (625)	// �c��
#define EXPLOTION_POS (D3DXVECTOR3(-330, -30, 0))
#define ENTER_TEXTURE_NAME ("data/TEXTURE/press_enter.png")	// �e�N�X�`���̃t�@�C���p�X
#define ENTER_WIDTH (434)	// ����
#define ENTER_HEIGHT (93)	// �c��
#define ENTER_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0))

// �����o�ϐ�
LPDIRECT3DTEXTURE9 TitleLogo::s_pTextureTitle_ = nullptr;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 TitleLogo::s_pVtxBufferTitle_ = nullptr;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DTEXTURE9 TitleLogo::s_pTextureExp_ = nullptr;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 TitleLogo::s_pVtxBufferExp_ = nullptr;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
LPDIRECT3DTEXTURE9 TitleLogo::s_pTextureEnter_ = nullptr;//texture�C���^�[�t�F�[�X
LPDIRECT3DVERTEXBUFFER9 TitleLogo::s_pVtxBufferEnter_ = nullptr;//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^

// ������
bool TitleLogo::Init(const D3DXVECTOR3 &pos, float rot, D3DXVECTOR3 &scale)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	pos_ = pos;
	rot_ = rot;
	scl_ = scale;
	col_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	state_ = LOGO_EXPLOTION;
	posExp_ = pos + EXPLOTION_POS;
	sclExp_ = scale * 0.5f;
	uv_ = 0;
	colEnter_ = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	if(!s_pVtxBufferTitle_)
	{
		if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&s_pVtxBufferTitle_,//
		NULL//
		)))
		{
			return false;
		}

		//���z�A�h���X���擾���邽�߂̃|�C���^
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		s_pVtxBufferTitle_->Lock(
			0,//�͈�
			0,//�͈�
			(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
			0);

		// �Ίp���̒���
		length_ = sqrtf(((TITLE_LOGO_WIDTH * scl_.x) / 2.0f) * ((TITLE_LOGO_WIDTH * scl_.x) / 2.0f) + ((TITLE_LOGO_HEIGHT * scl_.y) / 2.0f) * ((TITLE_LOGO_HEIGHT * scl_.y) / 2.0f));

		// �Ίp���̊p�x
		angle_ = atan2f(((TITLE_LOGO_WIDTH * scl_.x) / 2.0f), ((TITLE_LOGO_HEIGHT * scl_.y) / 2.0f));

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI + angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(length_ * sinf(rot_ - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(length_ * sinf(rot_ + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + angle_) + pos_.y - 0.5f, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		//texture���_
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���z�A�h���X���
		s_pVtxBufferTitle_->Unlock();
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if(!s_pTextureTitle_)
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice, TITLE_LOGO_TEXTURE_NAME, &s_pTextureTitle_)))
		{
			MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		}
	}

	if(!s_pVtxBufferExp_)
	{
		if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&s_pVtxBufferExp_,//
		NULL//
		)))
		{
			return false;
		}

		//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		s_pVtxBufferExp_->Lock(
			0,//�͈�
			0,//�͈�
			(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
			0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		//texture���_
		pVtx[0].tex = D3DXVECTOR2((0.25f * uv_), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * uv_), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 1.0f);

		//���z�A�h���X���
		s_pVtxBufferExp_->Unlock();
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if(!s_pTextureExp_)
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice, EXPLOTION_TEXTURE_NAME, &s_pTextureExp_)))
		{
			MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		}
	}

	if(!s_pVtxBufferEnter_)
	{
		if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&s_pVtxBufferEnter_,//
		NULL//
		)))
		{
			return false;
		}

		//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		s_pVtxBufferEnter_->Lock(
			0,//�͈�
			0,//�͈�
			(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
			0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(ENTER_POS.x - ENTER_WIDTH * 0.5f, ENTER_POS.y - ENTER_HEIGHT * 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(ENTER_POS.x + ENTER_WIDTH * 0.5f, ENTER_POS.y - ENTER_HEIGHT * 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(ENTER_POS.x - ENTER_WIDTH * 0.5f, ENTER_POS.y + ENTER_HEIGHT * 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(ENTER_POS.x + ENTER_WIDTH * 0.5f, ENTER_POS.y + ENTER_HEIGHT * 0.5f, 0.0f);

		//rhw�̐ݒ�(�K��1.0f)
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color = colEnter_;
		pVtx[1].color = colEnter_;
		pVtx[2].color = colEnter_;
		pVtx[3].color = colEnter_;

		//texture���_
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���z�A�h���X���
		s_pVtxBufferExp_->Unlock();
	}

	//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
	//���G���[�`�F�b�N�K�{
	if(!s_pTextureEnter_)
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice, ENTER_TEXTURE_NAME, &s_pTextureEnter_)))
		{
			MessageBox(NULL, "D3DXCreateTextureFromFile ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		}
	}


	return true;
}

// �I��
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

// �X�V
void TitleLogo::Update()
{
	Anim();

	if(!isPushKey_ && (GetJoystickTrigger(0, DIJ_BUTTON, DIJ_PS4_��) || GetKeyboardTrigger(DIK_SPACE)))
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

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	if(s_pVtxBufferExp_)
	{
		//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		s_pVtxBufferExp_->Lock(
			0,//�͈�
			0,//�͈�
			(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
			0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y - EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posExp_.x - EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posExp_.x + EXPLOTION_WIDTH * 0.5f * sclExp_.x, posExp_.y + EXPLOTION_HEIGHT * 0.5f * sclExp_.y, 0.0f);

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		pVtx[0].tex = D3DXVECTOR2((0.25f * uv_), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.25f * uv_), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.25f * (uv_ + 1)), 1.0f);

		//���z�A�h���X���
		s_pVtxBufferExp_->Unlock();
	}

	if(s_pVtxBufferTitle_)
	{

		//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		s_pVtxBufferTitle_->Lock(
			0,//�͈�
			0,//�͈�
			(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
			0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI + angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(length_ * sinf(rot_ + D3DX_PI - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + D3DX_PI - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(length_ * sinf(rot_ - angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ - angle_) + pos_.y - 0.5f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(length_ * sinf(rot_ + angle_) + pos_.x - 0.5f,
			length_ * cosf(rot_ + angle_) + pos_.y - 0.5f, 0.0f);

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color = col_;
		pVtx[1].color = col_;
		pVtx[2].color = col_;
		pVtx[3].color = col_;

		//���z�A�h���X���
		s_pVtxBufferTitle_->Unlock();
	}

	if(s_pVtxBufferEnter_)
	{
		//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		s_pVtxBufferEnter_->Lock(
			0,//�͈�
			0,//�͈�
			(void**)&pVtx,//�A�h���X�������ꂽ�������̃A�h���X
			0);

		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color = colEnter_;
		pVtx[1].color = colEnter_;
		pVtx[2].color = colEnter_;
		pVtx[3].color = colEnter_;

		//���z�A�h���X���
		s_pVtxBufferEnter_->Unlock();
	}
}

// �`��
void TitleLogo::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�擾

	//Texture�̐ݒ�
	pDevice->SetTexture(0, s_pTextureExp_);

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		s_pVtxBufferExp_,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, s_pTextureTitle_);

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		s_pVtxBufferTitle_,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
		NUM_POLYGON);

	//Texture�̐ݒ�
	pDevice->SetTexture(0, s_pTextureEnter_);

	pDevice->SetStreamSource(
		0,//�X�g���[���ԍ�
		s_pVtxBufferEnter_,
		0,//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));//�X�g���C�h��

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	//�v���~�e�B�u�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,//�I�t�Z�b�g�i���_���j
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