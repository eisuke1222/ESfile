//=========================================================================================================================
//
// �A�N�V�����Q�[������ [main.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"
#include "title.h"		// �^�C�g��
#include "game.h"		// �Q�[��
#include "result.h"		// ���U���g
#include "bgwindow.h"	// �w�i
#include "fade.h"		// �t�F�[�h
#include "sound.h"		// �T�E���h
#include "input.h"		// �L�[�{�[�h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define CLASS_NAME		"WindowClass"				// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"MeteoAdventure"			// �E�C���h�E�̖��O(�L���v�V������)

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		// ����������
void Uninit(void);												// �I������
void Update(void);												// �X�V����
void Draw(void);												// �`�揈��

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3D9				g_pD3D			= NULL;					// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9		g_pD3DDevice	= NULL;					// Direct3D�f�o�C�X�ւ̃|�C���^
bool					g_bWindowMode	= true;					// �E�B���h�E���[�h
MODE					g_Mode			= MODE_TITLE;			// ������ʏ��

//=========================================================================================================================
// ���C���֐�
//=========================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,									// �\������E�B���h�E�̃X�^�C����ݒ�
		WindowProc,									// �E�B���h�E�v���V�[�W���̃A�h���X���w��
		0,											// �ʏ�͎w�肵�Ȃ�����"0"���w��
		0,											// �ʏ�͎w�肵�Ȃ�����"0"���w��
		hInstance,									// WinMain�̈����̃C���X�^���X�n���h�����w��
		LoadIcon(NULL, IDI_APPLICATION),			// �^�X�N�o�[�Ɏg�p����A�C�R�����w��
		LoadCursor(NULL, IDC_ARROW),				// �g�p����}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),					// �E�B���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,										// Windows�ɂ��郁�j���[��ݒ�
		CLASS_NAME,									// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION)				// �t�@�C���A�C�R���Ɏg�p����A�C�R�����w��
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// �N���C�A���g�̈�̐ݒ�
	HWND hWnd;										// �E�B���h�E�n���h���i���ʎq�j
	MSG msg;										// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;							// ���ݎ���
	DWORD dwExecLastTime;							// �Ō�ɏ�����������

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̐���
	hWnd = CreateWindowEx
	(
		0,											// �g���E�B���h�E�X�^�C��
		CLASS_NAME,									// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,								// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,						// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,								// �E�B���h�E�̍���X���W		/*�d�Ȃ�Ə��������炵�Ă����    */
		CW_USEDEFAULT,								// �E�B���h�E�̍���Y���W		/*�Œ�l���Ɠ����ꏊ�ɕ\�����ꑱ����*/
		(rect.right - rect.left),					// �E�B���h�E�̕�
		(rect.bottom - rect.top),					// �E�B���h�E�̍���
		NULL,										// �e�E�B���h�E�̃n���h��
		NULL,										// ���j���[�n���h���܂��̓E�B���h�EID
		hInstance,									// �C���X�^���X�n���h��
		NULL										// �E�B���h�E�쐬�f�[�^
	);

	// ����������(�E�B���h�E�𐶐����Ă���s��) ���t���X�N���[���ύX FALSE:TRUE?
	if (FAILED(Init(hInstance, hWnd, g_bWindowMode)))
	{
		return -1;
	}

	// �E�C���h�E�̕\��(�����������̌�ɍs��)
	ShowWindow(hWnd, nCmdShow);						// �E�B���h�E�̕\���i�E�B���h�E�̕\����Ԃ�ݒ�j
	UpdateWindow(hWnd);								// �E�B���h�E�̔��f�i�E�B���h�E�̃N���C�A���g�̈�̍X�V�j

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();					// ���ݎ������擾

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{	// "WM_QUIT"���b�Z�[�W�������Ă����烋�[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��
				TranslateMessage(&msg);				// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);				// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();			// ���ݎ������擾

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// "60��1�b"�o��
				dwExecLastTime = dwCurrentTime;		// ��������������ێ�

				// �X�V����
				Update();

				// �`�揈��
				Draw();
			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================================================================================================
// �E�C���h�E�v���V�[�W��
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �ϐ��錾
	int nID;		// ���b�Z�[�W�I��ID

	switch (uMsg)
	{
	case WM_DESTROY:
		// "WM_QUIT"���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		// �~�{�^���x��
		if (g_bWindowMode == true)
		{// �E�B���h�E���[�h�̏ꍇ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);	// �I���m�F���b�Z�[�W

			if (nID == IDYES)
			{// YES���������ꍇ
			 // �E�B���h�E��j������iWM_DESTOROY���b�Z�[�W�𑗂�j
				DestroyWindow(hWnd);
			}
		}
		else
		{
			// �E�B���h�E��j������iWM_DESTOROY���b�Z�[�W�𑗂�j
			DestroyWindow(hWnd);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	// ESC�L�[�������ꂽ
			if (g_bWindowMode == true)
			{// �E�B���h�E���[�h�̏ꍇ
				nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

				if (nID == IDYES)
				{// YES���������ꍇ�̂�
				 // �E�B���h�E��j������iWM_DESTOROY���b�Z�[�W�𑗂�j
					DestroyWindow(hWnd);
				}
			}
			else
			{
				// �E�B���h�E��j������iWM_DESTOROY���b�Z�[�W�𑗂�j
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ����̏�����Ԃ�
}

//=========================================================================================================================
// ����������
//=========================================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));									// ���[�N���[���N���A
	d3dpp.BackBufferWidth				 = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight				 = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat				 = d3ddm.Format;				// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount				 = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect					 = D3DSWAPEFFECT_DISCARD;		// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil		 = TRUE;						// �f�v�X�o�b�t�@(�y�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat		 = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed						 = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz	 = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
	d3dpp.PresentationInterval			 = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��(VSync��҂��ĕ`��)

	// Direct3D�f�o�C�X�̐���
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^
		D3DDEVTYPE_HAL,													// �f�o�C�X�^�C�v
		hWnd,															// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,							// �f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,															// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&g_pD3DDevice)))												// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//	�L�[�{�[�h����������
	InitKeyboard(hInstance, hWnd);

	// �T�E���h����������
	InitSound(hWnd);

	// �t�F�[�h����������
	InitFade(MODE_TITLE);

	// �w�i����������
	InitBGWindow();

	// ��ʏ���������
	SetMode(g_Mode);

	return S_OK;
}

//=========================================================================================================================
// �I������
//=========================================================================================================================
void Uninit(void)
{
	// �L�[�{�[�h�I������
	UninitKeyboard();

	// �T�E���h�I������
	UninitSound();

	// �w�i�I������
	UninitBGWindow();

	// ��ʏI������
	UninitTitle();		// �^�C�g��
	UninitGame();		// �Q�[��
	UninitResult();		// ���U���g

	// �t�F�[�h�I������
	UninitFade();

	// Direct3D�f�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pD3DDevice->Release();	// �J��
		g_pD3DDevice = NULL;		// NULL��
	}

	// Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pD3D->Release();			// �J��
		g_pD3D = NULL;				// NULL��
	}
}

//========================================================================================================================
// �X�V����
//========================================================================================================================
void Update(void)
{
	// �L�[�{�[�h�X�V����
	UpdateKeyboard();

	// �w�i�X�V����
	UpdateBGWindow();

	switch (g_Mode)
	{// ��ʍX�V����
	case MODE_TITLE:		// �^�C�g��
		UpdateTitle();
		break;;
	case MODE_GAME:			// �Q�[��
		UpdateGame();
		break;
	case MODE_RESULT:		// ���U���g
		UpdateResult();
		break;;
	}

	// �t�F�[�h�X�V����
	UpdateFade();
}

//=========================================================================================================================
// �`�揈��
//=========================================================================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0
	);

	// �`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// �w�i�`�揈��
		DrawBGWindow();

		switch (g_Mode)
		{// ��ʕ`�揈��
		case MODE_TITLE:	// �^�C�g��
			DrawTitle();
			break;
		case MODE_GAME:		// �Q�[��
			DrawGame();
			break;
		case MODE_RESULT:	// ���U���g
			DrawResult();
			break;
		}

		// �t�F�[�h�`�揈��
		DrawFade();

		// �`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=========================================================================================================================
// �f�o�C�X�̎擾����
//=========================================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=========================================================================================================================
// ��ʐݒ菈��
//=========================================================================================================================
void SetMode(MODE mode)
{
	switch (g_Mode)
	{// ��ʏI������
	case MODE_TITLE:		// �^�C�g��
		UninitTitle();
		StopSound(SOUND_LABEL_BGM_TITLE);
		break;
	case MODE_GAME:			// �Q�[��
		UninitGame();
		break;
	case MODE_RESULT:		// ���U���g
		UninitResult();
		break;
	}

	// �w�i�j������
	DeleteBGWindow();

	switch (mode)
	{// ��ʏ���������
	case MODE_TITLE:		// �^�C�g��
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_TITLE);
		break;
	case MODE_GAME:			// �Q�[��
		InitGame();
		break;
	case MODE_RESULT:		// ���U���g
		InitResult();
		break;
	}

	g_Mode = mode;			// ���[�h��ݒ�
}

//=========================================================================================================================
// ��ʎ擾����
//=========================================================================================================================
MODE GetMode(void)
{
	return g_Mode;			// �l��Ԃ�
}