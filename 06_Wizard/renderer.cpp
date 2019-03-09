//=============================================================================
//
// �����_���[���� [renderer.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "renderer.h"		// �����_���[
#include "manager.h"		// �}�l�[�W���[
#include "scene.h"			// �V�[��
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "font.h"			// �t�H���g
#include "texture.h"		// �e�N�X�`��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CFont	*CRenderer::m_pFont[CFont::FONTNUM_MAX] = {};				// �t�H���g
bool	CRenderer::m_bImGuiWindow				= false;			// ImGui�\��
bool	CRenderer::m_bDrawAll					= true;				// �S�`��
bool	CRenderer::m_bDrawMap					= true;				// �}�b�v�`��

//=============================================================================
// �R���X�g���N�^								(public)	*** CRenderer ***
//=============================================================================
CRenderer::CRenderer()
{
	// �����l�ݒ�
	m_pD3D			= NULL;			// �I�u�W�F�N�g
	m_pD3DDevice	= NULL;			// �f�o�C�X
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CRenderer ***
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// ����������									(public)	*** CRenderer ***
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS	d3dpp;		//�f�B�X�v���C���[�h
    D3DDISPLAYMODE			d3ddm;		//�v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
		return E_FAIL;

	// ���݂̃f�B�X�v���C���[�h���擾
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return E_FAIL;

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount			 = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed					 = bWindow;						// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// Setup Dear ImGui binding		�iImGui�������j
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(m_pD3DDevice);

	// Setup style		�i�����̐F�ύX�ł���j
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h�L��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	// �f�o�b�O���\���p�t�H���g�̐���
	for (int nCntFont = 0; nCntFont < CFont::FONTNUM_MAX; nCntFont++)
	{// �t�H���g���J�E���g
		if (m_pFont[nCntFont] == NULL)
		{// NULL�̏ꍇ
			// �����l�ݒ�
			int			nHeight = 20;										// ����
			int			nWidth	= 0;										// ����
			int			nWeight = 0;										// ��
			RECT		rect	= { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// �ʒu
			UINT		uFmt	= DT_LEFT;									// �i��
			D3DXCOLOR	col		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);;		// �F

			switch (nCntFont)
			{// �J�E���g�ԍ�
			case CFont::FONTNUM_COMMAND:	// �R�}���h
				break;
			case CFont::FONTNUM_LOG:		// ���O
				rect = { 0, 200, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_DEBUG:		// �f�o�b�O
				rect = { 300, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_DATA:		// �f�[�^
				rect = { SCREEN_WIDTH - 350, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_TEST:		// �e�X�g
				rect = { SCREEN_WIDTH - 640, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			}

			// �t�H���g����
			m_pFont[nCntFont] = CFont::Create(nHeight, nWidth, nWeight, rect, uFmt, col);
		}
	}

#ifdef _DEBUG
	m_bImGuiWindow = true;		// �\��
#else
	m_bImGuiWindow = false;		// ��\��
#endif // _DEBUG

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Uninit(void)
{
	CTexture::ReleaseAll();			// �e�N�X�`���j��
	CScene::ReleaseAll();			// �S�V�[���j��

	// �f�o�b�O���\���p�t�H���g�̔j��
	for (int nCntFont = 0; nCntFont < CFont::FONTNUM_MAX; nCntFont++)
	{// �t�H���g���J�E���g
		if (m_pFont[nCntFont] == NULL)
		{// NULL�ȊO�̏ꍇ
			m_pFont[nCntFont]->Uninit();	// �I������
			m_pFont[nCntFont] = NULL;		// NULL��
		}
	}

	CFont::ReleaseAll();			// �S�t�H���g�I������

	// ImGui�I��
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// �f�o�C�X�̔j��
	if(m_pD3DDevice != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pD3DDevice->Release();	// ���
		m_pD3DDevice = NULL;		// NULL��
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if(m_pD3D != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pD3D->Release();			// ���
		m_pD3D = NULL;				// NULL��
	}
}

//=============================================================================
// �X�V����										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Update(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3))
	{// �L�[�{�[�h�iF3�j�L�[�������ꂽ
		m_bImGuiWindow = m_bImGuiWindow ? false : true;		// ImGui�\���؂�ւ�
	}

	if (m_bImGuiWindow == true)
	{// �\������
		// Start the Dear ImGui frame	�i�����Ȃ��Ɠ����Ȃ��j
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// ES�E�B���h�E
		ImGui::ShowESWindow();

		ImGui::EndFrame();

		//ShowCursor(true);
	}
	else
	{
		/*HWND hWnd = GetForegroundWindow();
		RECT rect;

		GetWindowRect(hWnd, (LPRECT)&rect);

		SetCursorPos((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);

		ShowCursor(false);*/
		
	}

	CScene::UpdateAll();			// �S�V�[���X�V����
}

//=============================================================================
// �`�揈��										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(0.4f, 0.6f, 0.9f, 1.0f), 1.0f, 0);

	CCamera *pCamera = NULL;	// �J����
	CManager::MODE mode = CManager::GetMode();

	D3DVIEWPORT9 viewPort;
	m_pD3DDevice->GetViewport(&viewPort);

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �J�����擾�i���C���j
		pCamera = CManager::GetCamera(CManager::CAMERA_MAIN);
		pCamera->SetCamera();

		if (m_bDrawAll == true)
		{// �\������
			CScene::DrawAll();		// �S�V�[���`�揈��
		}

		if (m_bImGuiWindow == true)
		{// �\������
			// �t�B�����[�h�ݒ�
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			// ImGui�`��
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}

		CFont::DrawAll();			// �S�t�H���g�`�揈��

		if (CScene::GetDrawUi() == true)
		{
			if (m_bDrawMap == true)
			{
				// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
				m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DXCOLOR(0.4f, 0.6f, 0.9f, 1.0f), 1.0f, 0);

				// �J�����擾�i�}�b�v�j
				pCamera = CManager::GetCamera(CManager::CAMERA_GAME_MAP);
				pCamera->SetCamera();

				if (m_bDrawAll == true)
				{// �\������
					CScene::DrawAllMap();		// �S�V�[���`��}�b�v����
				}
			}
		}
		
		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}
	m_pD3DDevice->SetViewport(&viewPort);

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	HRESULT result = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	D3DPRESENT_PARAMETERS d3dpp;		//�f�B�X�v���C���[�h

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && m_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		m_pD3DDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}