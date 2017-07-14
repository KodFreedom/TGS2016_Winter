//--------------------------------------------------------------------------------
//
//�@joystick.cpp
//	Author : Xu Wenjie
//	Date   : 2016-09-17
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "input.h"
#include "joystick.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define	NUM_BUTTON_MAX		(128)	// �L�[�̍ő吔
#define	LIMIT_COUNT_REPEAT	(20)	// ���s�[�g�J�E���g���~�b�^�[

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE *pDidInstance, VOID *pContext);
BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8	g_apDevJoystick[NUM_JOYSTICK_MAX];								// ���̓f�o�C�X(Joystick)�ւ̃|�C���^
//DIDEVCAPS				g_adiDevCaps[NUM_JOYSTICK_MAX];									// Joystick�̔\�͏��
bool					g_abActive[NUM_JOYSTICK_MAX];									// �E�C���h�E���A�N�e�B�u�̎�true�ɂȂ�ϐ�
DIJOYSTATE2				g_adiJoystickState[NUM_JOYSTICK_MAX];							// Joystick�̓��͏�񃏁[�N
DIJOYSTATE2				g_adiJoystickStateTrigger[NUM_JOYSTICK_MAX];					// Joystick�̃g���K�[��񃏁[�N
DIJOYSTATE2				g_adiJoystickStateRelease[NUM_JOYSTICK_MAX];					// Joystick�̃����[�X��񃏁[�N
DIJOYSTATE2				g_adiJoystickStateRepeat[NUM_JOYSTICK_MAX];						// Joystick�̃��s�[�g��񃏁[�N
int						g_aJoystickStateRepeatCnt[NUM_JOYSTICK_MAX][NUM_BUTTON_MAX];	// Joystick�̃��s�[�g�J�E���^
int						g_nCntJoystick;													// Joystick�J�E���^�[

//--------------------------------------------------------------------------------
//  Joystick���͏����̏�����
//--------------------------------------------------------------------------------
HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd)
{
	LPDIRECTINPUT8 pInput = GetInputObject();// DirectInput�I�u�W�F�N�g�ւ̃|�C���^

	for (g_nCntJoystick = 0; g_nCntJoystick < NUM_JOYSTICK_MAX;g_nCntJoystick++)
	{
		g_apDevJoystick[g_nCntJoystick] = NULL;

		//// �f�o�C�X�̍쐬
		//if (FAILED(pInput->CreateDevice(GUID_Joystick, &g_apDevJoystick[g_nCntJoystick], NULL)))
		//{ 
		//	MessageBox(hWnd, "Joystick���Ȃ��I", "�x���I", MB_ICONWARNING);
		//	return E_FAIL;
		//}

		// Joystick�f�o�C�X��񋓂��č쐬
		if (FAILED(pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY)) || g_apDevJoystick[g_nCntJoystick] == NULL)
		{
			MessageBox(hWnd, "Joystick�f�o�C�X��񋓂��č쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		// �f�[�^�t�H�[�}�b�g��ݒ�
		if (FAILED(g_apDevJoystick[g_nCntJoystick]->SetDataFormat(&c_dfDIJoystick2)))
		{
			MessageBox(hWnd, "Joystick�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		if (FAILED(g_apDevJoystick[g_nCntJoystick]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			MessageBox(hWnd, "Joystick�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		// ���̒l�͈̔͂̐ݒ�
		if (FAILED(g_apDevJoystick[g_nCntJoystick]->EnumObjects(EnumAxesCallback, NULL, DIDFT_AXIS)))
		{
			MessageBox(hWnd, "Joystick�̎��̒l�͈̔͂�ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
			return E_FAIL;
		}

		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		g_apDevJoystick[g_nCntJoystick]->Acquire();
	}

	return S_OK;
}

//--------------------------------------------------------------------------------
//  Joystick�I������
//--------------------------------------------------------------------------------
void UninitJoystick(void)
{
	for (int nCnt = 0; nCnt < NUM_JOYSTICK_MAX;nCnt++)
	{
		if (g_apDevJoystick[nCnt] != NULL)
		{
			g_apDevJoystick[nCnt]->Unacquire();
			g_apDevJoystick[nCnt]->Release();
			g_apDevJoystick[nCnt] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------
//  Joystick�X�V����
//--------------------------------------------------------------------------------
void UpdateJoystick(void)
{
	DIJOYSTATE2	adiJoystickState[NUM_JOYSTICK_MAX];

	for (int nCnt = 0; nCnt < NUM_JOYSTICK_MAX;nCnt++)
	{
		if (g_apDevJoystick[nCnt] == NULL) { continue; }

		// ��Ԃ��擾����O�Ƀf�[�^���|�[�����O
		if (FAILED(g_apDevJoystick[nCnt]->Poll()))
		{
			while (g_abActive[nCnt] && g_apDevJoystick[nCnt]->Acquire() == DIERR_INPUTLOST)
			{
				g_apDevJoystick[nCnt]->Acquire();
			}
		}

		// �f�o�C�X����f�[�^���擾
		if (SUCCEEDED(g_apDevJoystick[nCnt]->GetDeviceState(sizeof(DIJOYSTATE2), &adiJoystickState[nCnt])))
		{
			//x���̈ʒu
			//�L�[�g���K�[�A�����[�X���̎擾
			g_adiJoystickStateTrigger[nCnt].lX = (g_adiJoystickState[nCnt].lX != adiJoystickState[nCnt].lX) * adiJoystickState[nCnt].lX;
			g_adiJoystickStateRelease[nCnt].lX = (g_adiJoystickState[nCnt].lX != adiJoystickState[nCnt].lX) * g_adiJoystickState[nCnt].lX;
			// �L�[�v���X����ۑ�
			g_adiJoystickState[nCnt].lX = adiJoystickState[nCnt].lX;

			//y���̈ʒu
			//�L�[�g���K�[�A�����[�X���̎擾
			g_adiJoystickStateTrigger[nCnt].lY = (g_adiJoystickState[nCnt].lY != adiJoystickState[nCnt].lY) * adiJoystickState[nCnt].lY;
			g_adiJoystickStateRelease[nCnt].lY = (g_adiJoystickState[nCnt].lY != adiJoystickState[nCnt].lY) * g_adiJoystickState[nCnt].lY;
			// �L�[�v���X����ۑ�
			g_adiJoystickState[nCnt].lY = adiJoystickState[nCnt].lY;

			//z���̈ʒu
			//�L�[�g���K�[�A�����[�X���̎擾
			g_adiJoystickStateTrigger[nCnt].lZ = (g_adiJoystickState[nCnt].lZ != adiJoystickState[nCnt].lZ) * adiJoystickState[nCnt].lZ;
			g_adiJoystickStateRelease[nCnt].lZ = (g_adiJoystickState[nCnt].lZ != adiJoystickState[nCnt].lZ) * g_adiJoystickState[nCnt].lZ;
			// �L�[�v���X����ۑ�
			g_adiJoystickState[nCnt].lZ = adiJoystickState[nCnt].lZ;

			//x���̉�]
			//�L�[�g���K�[�A�����[�X���̎擾
			g_adiJoystickStateTrigger[nCnt].lRx = (g_adiJoystickState[nCnt].lRx != adiJoystickState[nCnt].lRx) * adiJoystickState[nCnt].lRx;
			g_adiJoystickStateRelease[nCnt].lRx = (g_adiJoystickState[nCnt].lRx != adiJoystickState[nCnt].lRx) * g_adiJoystickState[nCnt].lRx;
			// �L�[�v���X����ۑ�
			g_adiJoystickState[nCnt].lRx = adiJoystickState[nCnt].lRx;

			//y���̉�]
			//�L�[�g���K�[�A�����[�X���̎擾
			g_adiJoystickStateTrigger[nCnt].lRy = (g_adiJoystickState[nCnt].lRy != adiJoystickState[nCnt].lRy) * adiJoystickState[nCnt].lRy;
			g_adiJoystickStateRelease[nCnt].lRy = (g_adiJoystickState[nCnt].lRy != adiJoystickState[nCnt].lRy) * g_adiJoystickState[nCnt].lRy;
			// �L�[�v���X����ۑ�
			g_adiJoystickState[nCnt].lRy = adiJoystickState[nCnt].lRy;

			//z���̉�]
			//�L�[�g���K�[�A�����[�X���̎擾
			g_adiJoystickStateTrigger[nCnt].lRz = (g_adiJoystickState[nCnt].lRz != adiJoystickState[nCnt].lRz) * adiJoystickState[nCnt].lRz;
			g_adiJoystickStateRelease[nCnt].lRz = (g_adiJoystickState[nCnt].lRz != adiJoystickState[nCnt].lRz) * g_adiJoystickState[nCnt].lRz;
			// �L�[�v���X����ۑ�
			g_adiJoystickState[nCnt].lRz = adiJoystickState[nCnt].lRz;

			//2�̒ǉ���
			for (int nCntSlider = 0;nCntSlider < 2;nCntSlider++)
			{
				//�L�[�g���K�[�A�����[�X���̎擾
				g_adiJoystickStateTrigger[nCnt].rglSlider[nCntSlider] = (g_adiJoystickState[nCnt].rglSlider[nCntSlider] != adiJoystickState[nCnt].rglSlider[nCntSlider]) * adiJoystickState[nCnt].rglSlider[nCntSlider];
				g_adiJoystickStateRelease[nCnt].rglSlider[nCntSlider] = (g_adiJoystickState[nCnt].rglSlider[nCntSlider] != adiJoystickState[nCnt].rglSlider[nCntSlider]) * g_adiJoystickState[nCnt].rglSlider[nCntSlider];
				// �L�[�v���X����ۑ�
				g_adiJoystickState[nCnt].rglSlider[nCntSlider] = adiJoystickState[nCnt].rglSlider[nCntSlider];
			}

			//button
			for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
			{
				//�L�[�g���K�[�A�����[�X���̎擾
				g_adiJoystickStateTrigger[nCnt].rgbButtons[nCntButton] = (g_adiJoystickState[nCnt].rgbButtons[nCntButton] ^ adiJoystickState[nCnt].rgbButtons[nCntButton]) & adiJoystickState[nCnt].rgbButtons[nCntButton];
				g_adiJoystickStateRelease[nCnt].rgbButtons[nCntButton] = (g_adiJoystickState[nCnt].rgbButtons[nCntButton] ^ adiJoystickState[nCnt].rgbButtons[nCntButton]) & g_adiJoystickState[nCnt].rgbButtons[nCntButton];

				//repeat
				if (adiJoystickState[nCnt].rgbButtons[nCntButton] & 0x80)
				{
					if (g_aJoystickStateRepeatCnt[nCnt][nCntButton] < LIMIT_COUNT_REPEAT)
					{
						g_aJoystickStateRepeatCnt[nCnt][nCntButton]++;
						if (g_aJoystickStateRepeatCnt[nCnt][nCntButton] == 1 || g_aJoystickStateRepeatCnt[nCnt][nCntButton] >= LIMIT_COUNT_REPEAT)
						{
							g_adiJoystickStateRepeat[nCnt].rgbButtons[nCntButton] = adiJoystickState[nCnt].rgbButtons[nCntButton];
						}
						else
						{
							g_adiJoystickStateRepeat[nCnt].rgbButtons[nCntButton] = 0;
						}
					}
				}
				else
				{
					g_adiJoystickStateRepeat[nCnt].rgbButtons[nCntButton] = 0;
					g_aJoystickStateRepeatCnt[nCnt][nCntButton] = 0;
				}

				// �L�[�v���X����ۑ�
				g_adiJoystickState[nCnt].rgbButtons[nCntButton] = adiJoystickState[nCnt].rgbButtons[nCntButton];
			}
		}
		else
		{
			// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
			g_apDevJoystick[nCnt]->Acquire();
		}
	}
}

//--------------------------------------------------------------------------------
//  Joystick�̃v���X��Ԃ��擾
//--------------------------------------------------------------------------------
bool GetJoystickPress(int nNumJoystick, DIJ dij, int nButton)
{
	if (nNumJoystick < 0 || nNumJoystick >= NUM_JOYSTICK_MAX) { return false; }

	switch (dij)
	{
	case DIJ_BUTTON:
		return (g_adiJoystickState[nNumJoystick].rgbButtons[nButton] & 0x80) ? true : false;

	case DIJ_POV:
		return (g_adiJoystickState[nNumJoystick].rgdwPOV[0] == nButton) ? true : false;

	case DIJ_L_STICKX:
		return (g_adiJoystickState[nNumJoystick].lX == nButton) ? true : false;

	case DIJ_L_STICKY:
		return (g_adiJoystickState[nNumJoystick].lY == nButton) ? true : false;

	case DIJ_R_STICKX:
		return (nButton - 10 <= g_adiJoystickState[nNumJoystick].lZ && g_adiJoystickState[nNumJoystick].lZ <= nButton) ? true : false;

	case DIJ_R_STICKY:
		return (nButton - 10 <= g_adiJoystickState[nNumJoystick].lRz && g_adiJoystickState[nNumJoystick].lRz <= nButton) ? true : false;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  Joystick�̃g���K�[��Ԃ��擾
//--------------------------------------------------------------------------------
bool GetJoystickTrigger(int nNumJoystick, DIJ dij, int nButton)
{
	switch (dij)
	{
	case DIJ_BUTTON:
		return (g_adiJoystickStateTrigger[nNumJoystick].rgbButtons[nButton] & 0x80) ? true : false;

	case DIJ_POV:
		return (g_adiJoystickStateTrigger[nNumJoystick].rgdwPOV[0] == nButton) ? true : false;

	case DIJ_L_STICKX:
		return (g_adiJoystickStateTrigger[nNumJoystick].lX == nButton) ? true : false;

	case DIJ_L_STICKY:
		return (g_adiJoystickStateTrigger[nNumJoystick].lY == nButton) ? true : false;

	case DIJ_R_STICKX:
		return (nButton - 10 <= g_adiJoystickStateTrigger[nNumJoystick].lZ && g_adiJoystickStateTrigger[nNumJoystick].lZ <= nButton) ? true : false;

	case DIJ_R_STICKY:
		return (nButton - 10 <= g_adiJoystickStateTrigger[nNumJoystick].lRz && g_adiJoystickStateTrigger[nNumJoystick].lRz <= nButton) ? true : false;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  Joystick�̃��s�[�g��Ԃ��擾
//--------------------------------------------------------------------------------
bool GetJoystickRepeat(int nNumJoystick, int nButton)
{
	return (g_adiJoystickStateRepeat[nNumJoystick].rgbButtons[nButton] & 0x80) ? true : false;
}

//--------------------------------------------------------------------------------
//  Joystick�̃����|�X��Ԃ��擾
//--------------------------------------------------------------------------------
bool GetJoystickRelease(int nNumJoystick, DIJ dij, int nButton)
{
	switch (dij)
	{
	case DIJ_BUTTON:
		return (g_adiJoystickStateRelease[nNumJoystick].rgbButtons[nButton] & 0x80) ? true : false;

	case DIJ_POV:
		return (g_adiJoystickStateRelease[nNumJoystick].rgdwPOV[0] == nButton) ? true : false;

	case DIJ_L_STICKX:
		return (g_adiJoystickStateRelease[nNumJoystick].lX == nButton) ? true : false;

	case DIJ_L_STICKY:
		return (g_adiJoystickStateRelease[nNumJoystick].lY == nButton) ? true : false;

	case DIJ_R_STICKX:
		return (nButton - 10 <= g_adiJoystickStateRelease[nNumJoystick].lZ && g_adiJoystickStateRelease[nNumJoystick].lZ <= nButton) ? true : false;

	case DIJ_R_STICKY:
		return (nButton - 10 <= g_adiJoystickStateRelease[nNumJoystick].lRz && g_adiJoystickStateRelease[nNumJoystick].lRz <= nButton) ? true : false;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  �񋓂��ꂽ�f�o�C�X���ƂɌĂяo�����R�[���o�b�N�֐�
//--------------------------------------------------------------------------------
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE *pDidInstance, VOID *pContext)
{
	LPDIRECTINPUT8 pInput = GetInputObject();
	static GUID joysticDiscrimination[NUM_JOYSTICK_MAX];//�e�f�o�C�X�̎��ʎq���i�[
	DIDEVCAPS diDevCaps;// Joystick�̔\�͏��

						//���łɎ擾�ς݂̃f�o�C�X�������ꍇ�͂��������s����
	for (int nCnt = 0;nCnt < g_nCntJoystick;nCnt++)
	{
		if (joysticDiscrimination[nCnt] == pDidInstance->guidInstance)
		{
			return DIENUM_CONTINUE;
		}
	}

	//�񋓂��ꂽJoystick�ւ̃C���^�[�t�F�C�X���擾����
	if (FAILED(pInput->CreateDevice(pDidInstance->guidInstance, &g_apDevJoystick[g_nCntJoystick], NULL)))
	{
		return DIENUM_CONTINUE;//�񋓂𑱂���
	}

	//Joystick�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(g_apDevJoystick[g_nCntJoystick]->GetCapabilities(&diDevCaps)))
	{
		//Joystick�\�͂̎擾�Ɏ��s
		g_apDevJoystick[g_nCntJoystick]->Release();
		g_apDevJoystick[g_nCntJoystick] = NULL;
		return DIENUM_CONTINUE;//�񋓂𑱂���
	}

	//Joystick�̔\�͂��`�F�b�N

	//�f�o�C�X�̎��ʎq��ۑ�
	joysticDiscrimination[g_nCntJoystick] = pDidInstance->guidInstance;

	//���̃f�o�C�X���g��
	return DIENUM_STOP;//�񋓂��I������ꍇ
					   //return DIENUM_CONTINUE;//����Joystick���T���ꍇ
}

//--------------------------------------------------------------------------------
//  �����ƂɌĂяo�����R�[���o�b�N�֐�
//--------------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	DIPROPRANGE diprg;

	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = +1000;

	if (FAILED(g_apDevJoystick[g_nCntJoystick]->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

//--------------------------------------------------------------------------------
//  �W���C�X�e�B�b�N�̃X�e�B�b�N��Ԃ��擾
//--------------------------------------------------------------------------------
int GetJoystickStickStatus(int nNumJoystick, DIJ dij)
{
	switch (dij)
	{
	case DIJ_L_STICKX:
		return g_adiJoystickState[nNumJoystick].lX;

	case DIJ_L_STICKY:
		return g_adiJoystickState[nNumJoystick].lY;

	case DIJ_R_STICKX:
		return g_adiJoystickState[nNumJoystick].lRx;

	case DIJ_R_STICKY:
		return g_adiJoystickState[nNumJoystick].lRy;
	}

	return 0;
}

DIJOYSTATE2 GetJoystickStatus(int nCnt)
{
	return g_adiJoystickState[nCnt];
}