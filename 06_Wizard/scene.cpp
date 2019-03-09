//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"		// �V�[��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����
#include "font.h"		// �t�H���g
#include "imgui_es.h"	// ImGui_es

#include "camera.h"		// �J����

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
CScene		*CScene::m_pTop[PRIORITY_MAX]	= {};		// �擪�̃I�u�W�F�N�g�ւ̃|�C���^			
CScene		*CScene::m_pCur[PRIORITY_MAX]	= {};		// ���݂̃I�u�W�F�N�g�ւ̃|�C���^

int			CScene::m_nNumAll				= 0;		// ����
int			CScene::m_nNumPri[PRIORITY_MAX] = {};		// ����

int			CScene::m_nStartUp				= 0;		// �X�V�͈�

int			CScene::m_nDrawMode				= 0;		// �`�惂�[�h

int			CScene::m_nCntTimer				= 0;		// ���ԃJ�E���g
int			CScene::m_nUpdateTime			= 1;		// �X�V�J�E���g

bool		CScene::m_bDrawUi				= true;		// Ui�`��

//=============================================================================
// �R���X�g���N�^									(public)	*** CScene ***
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	m_pPrev = m_pCur[nPriority];						// �O�̃I�u�W�F�N�g�ݒ�
	m_pNext = NULL;										// ���̃I�u�W�F�N�g�ݒ�

	m_pCur[nPriority] = this;							// ���݂̃I�u�W�F�N�g�ݒ�

	if (m_pPrev != NULL)
	{// �O�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
		m_pPrev->m_pNext = m_pCur[nPriority];			// �O�̎��̃I�u�W�F�N�g�ݒ�
	}

	m_nPriority = nPriority;							// ���ʐݒ�
	m_objType = objType;								// ���
	m_bDeath = false;									// ���S�t���O
	m_nNumAll++;										// �����J�E���g
	m_nNumPri[nPriority]++;								// �����J�E���g

	if (m_pTop[nPriority] == NULL)
	{// �擪�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
		m_pTop[nPriority] = m_pCur[nPriority];			// �擪�̃I�u�W�F�N�g�ݒ�
	}
	else if (m_pTop[nPriority] == m_pCur[nPriority]->m_pPrev)
	{// �擪�̃I�u�W�F�N�g�ƑO�̃I�u�W�F�N�g�������ꍇ
		m_pTop[nPriority]->m_pNext = m_pCur[nPriority];	// �擪�̎��̃I�u�W�F�N�g�ݒ�
	}

	char cObjName[32];					// �I�u�W�F�N�g��
	ConObjtype(objType, cObjName);		// �I�u�W�F�N�g�����ϊ�

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);		// �t�H���g�擾

	// �����ݒ�
	if (pFont != NULL)
	{// NULL�ȊO�̏ꍇ�A
		pFont->SetMessage(CFont::SETTYPE_LOG, "�� Create : [%s(%d)]\n", cObjName, nPriority);
	}

	// ImGuiLog�ݒ�
	CImgui_ES::SetMessage(" Create : [%s(%d)]\n", cObjName, nPriority);
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CScene ***
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// �S�t�F�[�h�ȊO�J������								(public)	*** CScene ***
//=============================================================================
void CScene::ReleaseMode(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_FADE; nCntPriority++)
	{// �v���C�I���e�B���J�E���g
		while (m_pTop[nCntPriority] != NULL)
		{// �擪�̃I�u�W�F�N�g��NULL�ɂȂ�܂�
			m_pTop[nCntPriority]->Uninit();			// �擪�̃I�u�W�F�N�g�I��
			m_pTop[nCntPriority]->Release();		// �J������
		}
	}
}

//=============================================================================
// �S�J������										(public)	*** CScene ***
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �v���C�I���e�B���J�E���g
		while (m_pTop[nCntPriority] != NULL)
		{// �擪�̃I�u�W�F�N�g��NULL�ɂȂ�܂�
			m_pTop[nCntPriority]->Uninit();			// �擪�̃I�u�W�F�N�g�I��
			m_pTop[nCntPriority]->Release();		// �J������
		}
	}
}

//=============================================================================
// �S�X�V����										(public)	*** CScene ***
//=============================================================================
void CScene::UpdateAll(void)
{
	static bool bStop = false;
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	//	bStop = bStop ? false : true;

	if (bStop == false)
	{
		if (m_nCntTimer == 0)
		{
			for (int nCntPriority = m_nStartUp; nCntPriority < PRIORITY_MAX; nCntPriority++)
			{// �v���C�I���e�B���J�E���g
				CScene *pScene = m_pTop[nCntPriority];		// �X�V�̃I�u�W�F�N�g�i�擪���L���j
				CScene *pNext = NULL;						// ���̃I�u�W�F�N�g�L��

				while (pScene != NULL)
				{// �X�V�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
					pNext = pScene->m_pNext;				// ���̃I�u�W�F�N�g�ݒ�
					if (pScene->m_bDeath == false)
					{// �j�����Ȃ�
						pScene->Update();					// �X�V����
					}
					if (pScene->m_bDeath == true)
					{// �j������
						pScene->Release();					// �J������
					}
					pScene = pNext;							// �X�V�̃I�u�W�F�N�g�Ɏ��̃I�u�W�F�N�g�ݒ�
				}
			}
		}

		if (m_nUpdateTime < 1) m_nUpdateTime = 1;
		m_nCntTimer = (m_nCntTimer + 1) % m_nUpdateTime;
	}
	else
	{
		CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
		pCamera->SetMove(false);
	}
}
//=============================================================================
// �S�`�揈��										(public)	*** CScene ***
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �v���C�I���e�B���J�E���g
		if (m_bDrawUi == true || (m_bDrawUi == false && nCntPriority < PRIORITY_UI))
		{
			if (nCntPriority != PRIORITY_MAP_1 && nCntPriority != PRIORITY_MAP_2)
			{// �v���C�I���e�B����
				CScene *pScene = m_pTop[nCntPriority];		// �`��̃I�u�W�F�N�g�i�擪���L���j
				CScene *pNext = NULL;						// ���̃I�u�W�F�N�g�L��

				while (pScene != NULL)
				{// �`��̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
					pNext = pScene->m_pNext;				// ���̃I�u�W�F�N�g�ݒ�

					if (DrawSelect(0, pScene->GetObjType()) != true)
					{
						if (pScene->m_bDeath == false)
						{// �j�����Ȃ�
							pScene->Draw();						// �`�揈��
						}
						if (pScene->m_bDeath == true)
						{// �j������
							pScene->Release();					// �J������
						}
					}
					DrawSelect(1, pScene->GetObjType());

					pScene = pNext;							// �`��̃I�u�W�F�N�g�Ɏ��̃I�u�W�F�N�g�ݒ�
				}
			}
		}
	}
}


//=============================================================================
// �S�`��}�b�v����									(public)	*** CScene ***
//=============================================================================
void CScene::DrawAllMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �v���C�I���e�B���J�E���g
		CScene *pScene = m_pTop[nCntPriority];		// �`��̃I�u�W�F�N�g�i�擪���L���j
		CScene *pNext = NULL;						// ���̃I�u�W�F�N�g�L��

		while (pScene != NULL)
		{// �`��̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
			pNext = pScene->m_pNext;				// ���̃I�u�W�F�N�g�ݒ�

			if (DrawSelect(0, pScene->GetObjType()) != true)
			{
				if (pScene->GetObjType() != OBJTYPE_PLAYER && pScene->GetObjType() != OBJTYPE_ENEMY && pScene->GetObjType() != OBJTYPE_GAUGE)
				{// �I�u�W�F�N�g����
					if (pScene->m_bDeath == false)
					{// �j�����Ȃ�
						pScene->Draw();					// �`�揈��
					}
				}
				if (pScene->m_bDeath == true)
				{// �j������
					pScene->Release();					// �J������
				}
			}
			DrawSelect(1, pScene->GetObjType());

			pScene = pNext;							// �`��̃I�u�W�F�N�g�Ɏ��̃I�u�W�F�N�g�ݒ�
		}
	}
}

//=============================================================================
// �J������											(protected)	*** CScene ***
//=============================================================================
void CScene::Release(void)
{
	if (this != NULL)
	{// ������NULL�ȊO�̏ꍇ
		int nPriority = m_nPriority;
		OBJTYPE objType = m_objType;

		if (m_pPrev != NULL)	// m_pTop�ȊO
		{// �O�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
			if (m_pTop[nPriority] == m_pPrev)
			{// �擪�̃I�u�W�F�N�g���O�̃I�u�W�F�N�g�Ɠ����ꍇ
				m_pTop[nPriority]->m_pNext = m_pNext;		// �擪�̎��̃I�u�W�F�N�g�ݒ�
			}

			m_pPrev->m_pNext = m_pNext;						// �O�̎��̃I�u�W�F�N�g�ݒ�
		}

		if (m_pNext != NULL)	// m_pCur�ȊO
		{// ���̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
			if (m_pCur[nPriority] == m_pNext)
			{// ���݂̃I�u�W�F�N�g���O�̃I�u�W�F�N�g�Ɠ����ꍇ
				m_pCur[nPriority]->m_pPrev = m_pPrev;		// ���݂̑O�̃I�u�W�F�N�g�ݒ�
			}

			m_pNext->m_pPrev = m_pPrev;						// ���̑O�̃I�u�W�F�N�g�ݒ�
		}

		if (m_pTop[nPriority] == this)
		{// �擪�̃I�u�W�F�N�g�������Ɠ����ꍇ
			m_pTop[nPriority] = m_pNext;					// �擪�̃I�u�W�F�N�g�����ɍX�V
		}
		if (m_pCur[nPriority] == this)
		{// ���݂̃I�u�W�F�N�g�������Ɠ����ꍇ
			m_pCur[nPriority] = m_pPrev;					// ���݂̃I�u�W�F�N�g��O�ɍX�V
		}

		m_pPrev = NULL;										// �O�̃I�u�W�F�N�g
		m_pNext = NULL;										// ���̃I�u�W�F�N�g

		char cObjName[32];									// �I�u�W�F�N�g��
		ConObjtype(objType, cObjName);						// �I�u�W�F�N�g�����ϊ�

		CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);		// �t�H���g�擾

		// �����ݒ�
		if (pFont != NULL)
		{// NULL�ȊO�̏ꍇ�A
			pFont->SetMessage(CFont::SETTYPE_LOG, "�� Delete : [%s(%d)]\n", cObjName, nPriority);
		}

		// ImGuiLog�ݒ�
		CImgui_ES::SetMessage(" Delete : [%s(%d)]\n", cObjName, nPriority);

		delete this;										// �j��
		m_nNumPri[nPriority]--;								// �v���C�I���e�B���������炷
		m_nNumAll--;										// ���������炷
	}
}

//=============================================================================
// �I�u�W�F�N�g�^�C�v�����ϊ�����						(public)	*** CScene ***
//=============================================================================
bool CScene::DrawSelect(int nStartEnd, OBJTYPE objType)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (nStartEnd == 0)
	{
		if (m_nDrawMode == 0)
		{
			// �t�B�����[�h�ݒ�
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		else if (m_nDrawMode == 1)
		{
			// �t�B�����[�h�ݒ�
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else if (m_nDrawMode == 2)
		{
			if (objType != OBJTYPE_PLAYER)
			{
				// �t�B�����[�h�ݒ�
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
		}
		else if (m_nDrawMode == 3)
		{
			if (objType == OBJTYPE_PLAYER)
			{
				// �t�B�����[�h�ݒ�
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
		}
		else if (m_nDrawMode == 4)
		{
			if (objType != OBJTYPE_PLAYER)
			{
				return true;
			}
		}
		else if (m_nDrawMode == 5)
		{
			if (objType == OBJTYPE_PLAYER)
			{
				return true;
			}
		}
	}
	else
	{
		// �t�B�����[�h�ݒ�
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	return false;
}

//=============================================================================
// �I�u�W�F�N�g�^�C�v�����ϊ�����						(public)	*** CScene ***
//=============================================================================
void CScene::ConObjtype(CScene::OBJTYPE objType, char *cObjType)
{
	switch (objType)
	{// �I�u�W�F�N�g�^�C�v
	case OBJTYPE_MESHFIELD:		// ���b�V���t�B�[���h
		wsprintf(cObjType, "Meshfield");
		break;
	case OBJTYPE_MESHWALL:		// ���b�V���E�H�[��
		wsprintf(cObjType, "Meshwall");
		break;
	case OBJTYPE_MESHCYLINDER:	// ���b�V���V�����_�[
		wsprintf(cObjType, "Meshcylinder");
		break;
	case OBJTYPE_BG:			// �w�i
		wsprintf(cObjType, "Bg");
		break;
	case OBJTYPE_ENEMY:			// �G
		wsprintf(cObjType, "Enemy");
		break;
	case OBJTYPE_PLAYER:		// �v���C���[
		wsprintf(cObjType, "Player");
		break;
	case OBJTYPE_BULLET:		// �e
		wsprintf(cObjType, "Bullet");
		break;
	case OBJTYPE_EFFECT:		// �G�t�F�N�g
		wsprintf(cObjType, "Effect");
		break;
	case OBJTYPE_NUMBER:		// �ԍ�
		wsprintf(cObjType, "Number");
		break;
	case OBJTYPE_SCORE:			// �X�R�A
		wsprintf(cObjType, "Score");
		break;
	case OBJTYPE_RANKING:		// �����L���O
		wsprintf(cObjType, "Ranking");
		break;
	case OBJTYPE_MENU:			// ���j���[
		wsprintf(cObjType, "Menu");
		break;
	case OBJTYPE_PAUSE:			// �|�[�Y
		wsprintf(cObjType, "Pause");
		break;
	case OBJTYPE_TITLE:			// �^�C�g��
		wsprintf(cObjType, "Title");
		break;
	case OBJTYPE_FADE:			// �t�F�[�h
		wsprintf(cObjType, "Fade");
		break;
	case OBJTYPE_CAMERA:		// �J����
		wsprintf(cObjType, "Camera");
		break;
	default:					// ���̑�
		wsprintf(cObjType, "Unknow");
		break;
	}
}