//--------------------------------------------------------------------------------
//
//�@�E�C���h�E�\���v���O����
//	Author : Xu Wenjie
//	Date   : 2016-04-26
//--------------------------------------------------------------------------------
//  Update : 2016-07-11
//	
//--------------------------------------------------------------------------------
#include <stdio.h>
#include "input.h"
#include "joystick.h"
#include "main.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "gameover.h"
#include "fade.h"
#include "sound.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CLASS_NAME "�T���v��"//�N���X�̖��O
#define WINDOW_NAME "TGS2016Winter"//�E�C���h�E�̖��O

//--------------------------------------------------------------------------------
//  �֐��|�C���^
//--------------------------------------------------------------------------------
typedef void(*ModeFunc)(void);
ModeFunc g_pInitFunc[] = { InitTitle,InitTutorial,InitGame,InitResult,InitGameover };
ModeFunc g_pUninitFunc[] = { UninitTitle,UninitTutorial,UninitGame,UninitResult,UninitGameover };
ModeFunc g_pUpdateFunc[] = { UpdateTitle,UpdateTutorial,UpdateGame,UpdateResult,UpdateGameover };
ModeFunc g_pDrawFunc[] = { DrawTitle,DrawTutorial,DrawGame,DrawResult,DrawGameover };

//--------------------------------------------------------------------------------
//  �v���g�^�C�v�錾
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(HWND *pWnd);
void Draw(void);
void Close(HWND *pWnd);

#ifdef _DEBUG
void DrawFPS(void);
#endif//_DEBUG

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���[�o���ϐ�
//--------------------------------------------------------------------------------
LPDIRECT3D9 g_pD3D = NULL;//Direct3D�C���^�[�t�F�[�X/�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//Direct3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;//�Q�[�����[�h

#ifdef _DEBUG
LPD3DXFONT g_pFont = NULL;//�t�H���g�ւ̃C���^�[�t�F�[�X
int g_nCntFPS = 0;//FPS Countor
#endif//_DEBUG

//--------------------------------------------------------------------------------
//  main�֐�
//--------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(IpCmdLine);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);//WNDCLASSEX�̃������T�C�Y���w��
	wcex.style = CS_CLASSDC;//�\������E�C���h�E�̃X�^�C����ݒ�
	wcex.lpfnWndProc = WndProc;//�֐��|�C���^�A�E�C���h�E�v���V�[�W���̃A�h���X�i�֐����j���w��
	wcex.cbClsExtra = 0;//�ʏ�͎g�p���Ȃ��̂łO���w��
	wcex.cbWndExtra = 0;//�ʏ�͎g�p���Ȃ��̂łO���w��
	wcex.hInstance = hInstance;//WinMain�̃p�����[�^�̃C���X�^���X�n���h��
	wcex.hIcon = NULL;//����icon�o���A�g�p����A�C�R�����w��iWindows�������Ă���j
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);//�}�E�X�J�[�\�����w��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//�E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
	wcex.lpszMenuName = NULL;//Window�ɂ��郁�j���[��ݒ�i����Əd���j
	wcex.lpszClassName = CLASS_NAME;//�E�C���h�E�N���X�̖��O
	wcex.hIconSm = NULL;//�g�����ꂽ�����i�~�jicon�j�A�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	RegisterClassEx(&wcex);//�E�C���h�E�N���X�̓o�^

	HWND hWnd;
	MSG msg;
	DWORD style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);
	RECT cr = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT dr;

	//window size
	AdjustWindowRect(&cr, style, false);
	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	//window pos
	GetWindowRect(GetDesktopWindow(), &dr);
	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	//��ʈ�o�h�~
	int wx = ww > dw ? 0 : (dw - ww) / 2;
	int wy = wh > dh ? 0 : (dh - wh) / 2;

	//�E�C���h�E���쐬
	hWnd = CreateWindowEx(
		0,//�g���E�C���h�E�X�^�C��
		CLASS_NAME,//�N���X�̖��O
		WINDOW_NAME,//�E�C���h�E�̖��O
		style,//**important**window type,�E�C���h�E�̃X�^�C��
		wx,//�E�C���h�E������WX
		wy,//�E�C���h�E������WY
		ww,//���i�E�C���h�E�S�́j
		wh,//�����i�E�C���h�E�S�́j
		NULL,//�e�E�B���h�E�̃n���h��
		NULL,//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,//�C���X�^���X�n���h��
		NULL);//�E�C���h�E�쐬�f�[�^

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//FPS
	DWORD dwFrameCount;
	DWORD dwCurrentTime;//������
	DWORD dwExecLastTime;//���s�I��������
	DWORD dwFPSLastTime;

	//����\��ݒ�
	timeBeginPeriod(1);

	UINT nID = 0;//���b�Z�[�Wbox�߂�l
	bool bWindow = true;//�t���X�N���[���t���b�O

	//�t���X�N���[�����[�h�m�F
	nID = MessageBox(hWnd, "�t���X�N���[�����[�h�ŋN�����܂����H", "�m�F", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);

	//��������
	if (nID == IDYES) {
		bWindow = false;
	}

	if (FAILED(Init(hInstance, hWnd, bWindow)))
	{
		MessageBox(NULL, "INIT ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	//�e�J�E���^�[������
	dwCurrentTime = 0;
	dwFrameCount = 0;
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();//�V�X�e���������~���b�P�ʂŎ擾

	//���b�Z�[�W���[�v
	for (;;)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//window����
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= TIMER_INTERVAL)
			{
				dwExecLastTime = dwCurrentTime;//������������

#ifdef _DEBUG
				if ((dwCurrentTime - dwFPSLastTime) >= 500)//0.5�b���ƂɎ��s
				{
					g_nCntFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrentTime;
					dwFrameCount = 0;
				}
#endif//_DEBUG
				Update(&hWnd);
				Draw();

				dwFrameCount++;
			}
		}
	}

	//��Еt��
	Uninit();

	//�E�C���h�E�N���X�̓o�^������
	//�������F���N���X��
	//�������F�A�v���P�[�V�����C���X�^��
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------
//  �E�C���h�E�v���V�[�W���֐�
//  (CALLBACK : Win32API�֐����Ăяo�����̋K��)
//  hWnd ; �E�C���h�E�̃n���h��
//  uMsg : ���b�Z�[�W�̎��ʎq
//  wPara : ���b�Z�[�W�̍ŏ��̃p�����[�^
//  iParam : ���b�Z�[�W�̓�Ԗڂ̃p�����[�^
//--------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	switch (uMsg) {
	case WM_KEYDOWN://esp key
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			Close(&hWnd);
		}
		break;
	case WM_CLOSE:
		Close(&hWnd);
		return 0;
	case WM_DESTROY://�ۑ����邩�`�F�b�N�̎d�g�݂�����
		PostQuitMessage(0);//WM_QUIT�Ƃ������b�Z�[�W���Ă�
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//--------------------------------------------------------------------------------
//  Init�֐�
//--------------------------------------------------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//�L�[������
	if (FAILED(InitKeyboard(hInstance,hWnd)))
	{
		return E_FAIL;
	}

	//if (FAILED(InitJoystick(hInstance, hWnd)))
	//{
	//	return E_FAIL;
	//}

	//�T�E���h������
	if (FAILED(InitSound(hWnd)))
	{
		return E_FAIL;
	}

	//Direct3D�C���^�[�t�F�[�X�̎擾
	g_pD3D = Direct3DCreate9(D3D9b_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���̃f�B�X�v���[���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm)))
	{
		return E_FAIL;
	}

	//device�̃v���[���g�p�����[�^�[�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;//fullscreen�̎������Ō��߂���
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;//�O�N���̎��K���I�I
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�O�N���̎��ς���I�I
	d3dpp.Windowed = bWindow;//window mode/fullscreen mode(WS_POPUP�v��I)
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//fullscreen�̂�(60hz/59hz)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//fullscreen�݂݂̂���
	//�ق�

	//Direct3Ddevice�쐬
	//�`��A���_�������n�[�h�E�F�A�ōs��
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,//���_�������n�[�h�E�F�A�ɔC��
		&d3dpp,
		&g_pD3DDevice)))//����/���s�̏��������ɕԋp
	{
		//�`��̓n�[�h�E�F�A�A���_������CPU�ōs��
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,//�����_������CPU�ɔC��
			&d3dpp,
			&g_pD3DDevice)))//����/���s�̏��������ɕԋp
		{
			//�`��A���_������CPU�ōs��(3D�قǂ�Ǔ����܂���)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,//���ύX
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,//�����_������CPU�ɔC��
				&d3dpp,
				&g_pD3DDevice)))//����/���s�̏��������ɕԋp
			{
				return E_FAIL;
			}
		}
	}

	//�`��ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//�J�����O
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);//�h��Ԃ�

	//alpha
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�A���t�@�u�����h�L��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//texture alpha
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�T���v���[�X�e�[�g
	g_pD3DDevice->SetSamplerState(0, //layer
		D3DSAMP_MINFILTER,
		D3DTEXF_LINEAR);

	g_pD3DDevice->SetSamplerState(0, //layer
		D3DSAMP_ADDRESSU,
		D3DTADDRESS_WRAP);

	g_pD3DDevice->SetSamplerState(0, //layer
		D3DSAMP_ADDRESSV,
		D3DTADDRESS_WRAP);//WRAP����/CLAMP���т�/MIRROR

#ifdef _DEBUG
	//debug
	D3DXCreateFont(
		g_pD3DDevice,
		18,//�����̍���
		0,//�����̕��i0�͍����ƍ��킹�镝�j
		0,//����
		0,//�~�b�v�}�b�v���x����
		FALSE,//italic
		SHIFTJIS_CHARSET,//CharSet
		OUT_DEFAULT_PRECIS,//OutputPrecision
		DEFAULT_QUALITY,//Quality
		DEFAULT_PITCH,//PitchAndFamily
		"Terminal",//Font Name
		&g_pFont);
#endif//_DEBUG

	//���[�h�ݒ�
	//g_pInitFunc[g_mode]();
	InitFade();
	SetMode(g_mode);
	return S_OK;
}

//--------------------------------------------------------------------------------
//  Uninit�֐�
//--------------------------------------------------------------------------------
void Uninit(void)
{
	//���͏I������
	UninitKeyboard();
	UninitJoystick();

	UninitFade();

	for (int nCnt = 0;nCnt < MODE_MAX;nCnt++)
	{
		g_pUninitFunc[nCnt]();
	}

	//�T�E���h�I������
	UninitSound();

	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();//Direct3D�f�o�C�X�̉��
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();//Direct3D�C���^�[�t�F�[�X���
		g_pD3D = NULL;
	}

#ifdef _DEBUG
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
#endif//_DEBUG
}

//--------------------------------------------------------------------------------
//  Update�֐�
//--------------------------------------------------------------------------------
void Update(HWND *pWnd)
{
	UpdateKeyboard();
	UpdateJoystick();
	UpdateFade();
	g_pUpdateFunc[g_mode]();
}

//--------------------------------------------------------------------------------
//  Draw�֐�
//--------------------------------------------------------------------------------
void Draw(void)
{
	//���(�o�b�N�o�b�t�@)�̃N���A
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))//1frame once
	{
		g_pDrawFunc[g_mode]();
		DrawFade();
#ifdef _DEBUG
		DrawFPS();
#endif//_DEBUG
		g_pD3DDevice->EndScene();//�`��I��
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�[�̂���ւ�	
}

//--------------------------------------------------------------------------------
//  GetDevice�֐�
//--------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//--------------------------------------------------------------------------------
//  close�m�F�֐�
//--------------------------------------------------------------------------------
void Close(HWND *pWnd)
{
	UINT nID = 0;//���b�Z�[�Wbox�߂�l

	//�I���m�F
	nID = MessageBox(*pWnd, "�I�����܂����H", "�m�F", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);

	//��������
	if (nID == IDYES) {
		//WM_DESTROY���b�Z�[�W�𑗐M
		DestroyWindow(*pWnd);
	}
}

//--------------------------------------------------------------------------------
//  DrawFPS�֐�
//--------------------------------------------------------------------------------
#ifdef _DEBUG
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(aStr, "FPS:%d\n", g_nCntFPS);

	g_pFont->DrawTextA(
		NULL,
		aStr,//������
		-1,//������-1��NULL�܂�
		&rect,//�͈�
		DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}
#endif//_DEBUG

//--------------------------------------------------------------------------------
//  SetMode�֐�
//--------------------------------------------------------------------------------
void SetMode(MODE mode)
{
	//�I������
	g_pUninitFunc[g_mode]();//����������ĂȂ��Ă����S�Ɏ��s�ł���悤�Ɏ�������

	//����������
	g_pInitFunc[mode]();

	g_mode = mode;
}

//--------------------------------------------------------------------------------
//  GetMode�֐�
//--------------------------------------------------------------------------------
MODE GetMode(void)
{
	return g_mode;
}