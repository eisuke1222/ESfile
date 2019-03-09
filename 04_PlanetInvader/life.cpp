//=============================================================================
//
// �̗́i�c�@�j���� [life.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "life.h"			// �̗�
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "scene2d.h"		// �V�[��2D
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h
#include "game.h"			// �Q�[��
#include "player.h"			// �v���C���[
#include "bomb.h"			// ���e

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\heart.png"		// �e�N�X�`��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			CLife::m_pTexture				= NULL;				// �e�N�X�`��
bool						CLife::m_bAddLife[MAX_PLAYER]	= {};				// �����g�p
bool						CLife::m_bRespawn[MAX_PLAYER]	= {};				// �����g�p
int							CLife::m_nLifeAll[MAX_PLAYER]	= {};				// �ő吔
bool						CLife::m_bLiveState[MAX_PLAYER] = {};				// ������

//=============================================================================
// �R���X�g���N�^									(public)	*** CLife ***
//=============================================================================
CLife::CLife() : CScene(CScene::PRIORITY_UI)
{
	// �����ݒ�
	m_nLifeNum = 0;										// �̗͔ԍ�

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{// �J�E���g
		m_apScene2D[nCntLife] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CLife ***
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// ���[�h����							(public)	*** CLife ***
//=============================================================================
HRESULT CLife::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	if (m_pTexture == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CLife ***
//=============================================================================
void CLife::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTexture->Release();		// ���
		m_pTexture = NULL;			// NULL��
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CLife ***
//=============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLifeNum)
{
	CLife *pLife = NULL;			// �|�C���^

	if (pLife == NULL)
	{// NULL�̏ꍇ
		pLife = new CLife;			// �������m��
	}

	if (pLife != NULL)
	{// NULL�ȊO�̏ꍇ
		pLife->m_nLifeNum = nLifeNum;				// �̗͔ԍ�

		for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
		{// �J�E���g
			if (pLife->m_apScene2D[nCntLife] == NULL)
			{
				pLife->m_apScene2D[nCntLife] = new CScene2D(CScene::PRIORITY_UI);																			// �������m��
				pLife->m_apScene2D[nCntLife]->SetInitAll(D3DXVECTOR3(pos.x + ((size.x * 2.0f) * nCntLife), pos.y, 0.0f), size, col, D3DXVECTOR2(1.0f, 1.0f));		// �����l�ݒ�
				pLife->m_apScene2D[nCntLife]->Init();																												// ����������
				pLife->m_apScene2D[nCntLife]->SetObjType(CScene::OBJTYPE_LIFE);																						// �I�u�W�F�N�g�^�C�v�ݒ�
				pLife->m_apScene2D[nCntLife]->BindTexture(m_pTexture);																								// �e�N�X�`���ݒ�
			}
		}
		
		m_bAddLife[pLife->m_nLifeNum] = false;		// ���Z
		m_bRespawn[pLife->m_nLifeNum] = true;		// ���X�|�[��
		m_nLifeAll[pLife->m_nLifeNum] = MAX_LIFE;	// �̗͍ő吔
		m_bLiveState[pLife->m_nLifeNum] = true;		// �̗͍ő吔

		pLife->Init();		// ����������
	}

	return pLife;
}

//=============================================================================
// ����������										(public)	*** CLife ***
//=============================================================================
HRESULT CLife::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CLife ***
//=============================================================================
void CLife::Uninit(void)
{
	for (int nCntLife = 0; nCntLife < m_nLifeAll[m_nLifeNum]; nCntLife++)
	{// �J�E���g
		m_apScene2D[nCntLife]->Uninit();			// �I������
		m_apScene2D[nCntLife] = NULL;				// NULL��
	}

	CScene::Release();
}

//=============================================================================
// �X�V����											(public)	*** CLife ***
//=============================================================================
void CLife::Update(void)
{
	CManager::MODE mode = CManager::GetMode();						// ���[�h�擾
	CGame::GAME game = CGame::GetGame();							// �Q�[���擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// ���̓f�o�C�X
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// �J���������_�擾

	if (m_bAddLife[m_nLifeNum] == true && m_nLifeAll[m_nLifeNum] < MAX_LIFE - 1)
	{//
		AddLife();
		m_bAddLife[m_nLifeNum] = false;
	}
	if (m_bRespawn[m_nLifeNum] == true && 0 < m_nLifeAll[m_nLifeNum])
	{//
		if (CManager::GetPlayNum() == MAX_PLAYER)
		{
			CPlayer::Create(D3DXVECTOR3(Camerapos.x - 100.0f + (200.0f * m_nLifeNum), 300.0f, Camerapos.z - 500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), m_nLifeNum);			// �v���C���[�̐���
		}
		else
		{
			CPlayer::Create(D3DXVECTOR3(Camerapos.x, 300.0f, Camerapos.z - 500.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), m_nLifeNum);			// �v���C���[�̐���
		}

		DeleteLife();						// ���C�t����
		m_bRespawn[m_nLifeNum] = false;		// �����s�\
	}

	if (m_bLiveState[m_nLifeNum] == false)
	{

		int nCntLive = 0;
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			if (m_bLiveState[nCnt] == false)
			{
				nCntLive++;
			}
		}

		if (nCntLive == CManager::GetPlayNum())
		{

			if (mode == CManager::MODE_TUTORIAL)
			{
				CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
			}
			else if (mode == CManager::MODE_GAME)
			{
				if (game == CGame::GAME_PLAY)
				{
					CGame::SetGame(CGame::GAME_OVER);
				}
			}
		}
	}

#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_V) == true)
	{
		if (m_nLifeAll[m_nLifeNum] < MAX_LIFE)
		{
			for (int nCntLife = m_nLifeAll[m_nLifeNum]; nCntLife < MAX_LIFE; nCntLife++)
			{
				m_apScene2D[nCntLife]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			m_nLifeAll[m_nLifeNum] = MAX_LIFE;
		}
	}
#endif
}

//=============================================================================
// �`�揈��											(public)	*** CLife ***
//=============================================================================
void CLife::Draw(void)
{

}

//=============================================================================
// �ǉ�����											(public)	*** CBomb ***
//=============================================================================
void CLife::AddLife(void)
{
	if (m_nLifeAll[m_nLifeNum] < MAX_LIFE - 1)
	{// �ő吔��菭�Ȃ�
		m_apScene2D[m_nLifeAll[m_nLifeNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nLifeAll[m_nLifeNum]++;
	}
}

//=============================================================================
// �������											(public)	*** CLife ***
//=============================================================================
void CLife::DeleteLife(void)
{
	if (0 < m_nLifeAll[m_nLifeNum])
	{// �P�ȏ゠��Ƃ�
		m_nLifeAll[m_nLifeNum]--;
		m_apScene2D[m_nLifeAll[m_nLifeNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));		// �I������

		CBomb::SetReset(m_nLifeNum, true);						// �{�����Z�b�g
	}
}