//=============================================================================
//
// ���C������ [main.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"Planet Invader "	// �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
#ifdef _DEBUG
int						g_nCountFPS;				// FPS�J�E���^
#endif
bool					g_bWindowMode = false;

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (g_bWindowMode == true)
	{
		ShowCursor(false);
	}

	CManager *pManager = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,									// �\������E�B���h�E�̃X�^�C����ݒ�
		WndProc,									// �E�B���h�E�v���V�[�W���̃A�h���X���w��
		0,											// �ʏ�͎w�肵�Ȃ�����"0"���w��
		0,											// �ʏ�͎w�肵�Ȃ�����"0"���w��
		hInstance,									// WinMain�̈����̃C���X�^���X�n���h�����w��
		NULL,										// �^�X�N�o�[�Ɏg�p����A�C�R�����w��
		LoadCursor(NULL, IDC_ARROW),				// �g�p����}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1),					// �E�B���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,										// Windows�ɂ��郁�j���[��ݒ�
		CLASS_NAME,									// �E�B���h�E�N���X�̖��O
		NULL										// �t�@�C���A�C�R���Ɏg�p����A�C�R�����w��
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };// �N���C�A���g�̈�̐ݒ�
	HWND hWnd;										// �E�B���h�E�n���h���i���ʎq�j
	MSG msg;										// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;							// ���ݎ���
	DWORD dwExecLastTime;							// �Ō�ɏ�����������
	DWORD dwFrameCount;								// FPS�l�J�E���g
	DWORD dwFPSLastTime;							// FPS�l�Ō�ɏ�����������

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,					// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,								// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,						// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,								// �E�B���h�E�̍���X���W		/*�d�Ȃ�Ə��������炵�Ă����    */
		CW_USEDEFAULT,								// �E�B���h�E�̍���Y���W		/*�Œ�l���Ɠ����ꏊ�ɕ\�����ꑱ����*/
		(rect.right - rect.left),					// �E�B���h�E�̕�
		(rect.bottom - rect.top),					// �E�B���h�E�̍���
		NULL,										// �e�E�B���h�E�̃n���h��
		NULL,										// ���j���[�n���h���܂��̓E�B���h�EID
		hInstance,									// �C���X�^���X�n���h��
		NULL);										// �E�B���h�E�쐬�f�[�^

	srand((unsigned int)time(0));					// �����_��

	if (pManager == NULL)
	{// NULL�̏ꍇ
		pManager = new CManager;
	}

	if (pManager != NULL)
	{// NULL�ȊO�̏ꍇ
		if (FAILED(pManager->Init(hInstance, hWnd, g_bWindowMode)))
		{
			return -1;
		}

		// ����\��ݒ�
		timeBeginPeriod(1);

		// �t���[���J�E���g������
		dwCurrentTime =
			dwFrameCount = 0;
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();				// ���ݎ������擾

		// �E�C���h�E�̕\��
		ShowWindow(hWnd, nCmdShow);						// �E�B���h�E�̕\���i�E�B���h�E�̕\����Ԃ�ݒ�j
		UpdateWindow(hWnd);								// �E�B���h�E�̔��f�i�E�B���h�E�̃N���C�A���g�̈�̍X�V�j

		// ���b�Z�[�W���[�v
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
			{// Windows�̏���
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
					break;
				}
				else
				{
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);				// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
					DispatchMessage(&msg);				// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
				}
			}
			else
			{// DirectX�̏���
				dwCurrentTime = timeGetTime();		// ���݂̎��Ԃ��擾
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5�b���ƂɎ��s
#ifdef _DEBUG
					// FPS���Z�o
					g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
					dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
					dwFrameCount = 0;
				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60�b�o��
					dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

					// �X�V����
					pManager->Update();

					// �`�揈��
					pManager->Draw();

					dwFrameCount++;
				}
			}
		}
	}

	// �I������
	if (pManager != NULL)
	{// NULL�ȊO�̏ꍇ
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

#ifdef _DEBUG
//=============================================================================
// FPS�l�擾
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
#endif