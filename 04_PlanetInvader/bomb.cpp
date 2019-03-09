//=============================================================================
//
// ���e�i�c���j���� [bomb.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "bomb.h"			// ���e
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "scene2d.h"		// �V�[��2D
#include "gauge.h"			// �Q�[�W

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\bomb.png"		// �e�N�X�`��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBomb::m_pTexture				= NULL;					// �e�N�X�`�����ւ̃|�C���^
bool				CBomb::m_bShotBomb[MAX_PLAYER]	= {};					// ����
bool				CBomb::m_bAddBomb[MAX_PLAYER]	= {};					// ���Z
bool				CBomb::m_bResetBomb[MAX_PLAYER]	= {};					// ���Z�b�g
bool				CBomb::m_bDeleteBomb[MAX_PLAYER]= {};					// �j��
int 				CBomb::m_nMaxBomb[MAX_PLAYER]	= {};					// �ő吔
int 				CBomb::m_nBombAll[MAX_PLAYER]	= {};					// ���ݐ�

//=============================================================================
// �R���X�g���N�^									(public)	*** CBomb ***
//=============================================================================
CBomb::CBomb() : CScene(CScene::PRIORITY_UI)
{
	// �����ݒ�
	for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
	{// �J�E���g
		m_apScene2D[nCntBomb] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CBomb ***
//=============================================================================
CBomb::~CBomb()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBomb ***
//=============================================================================
HRESULT CBomb::Load(void)
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
// �A�����[�h����								(public)	*** CBomb ***
//=============================================================================
void CBomb::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{// NULL�ȊO�̏�
		m_pTexture->Release();		// ���
		m_pTexture = NULL;			// NULL��
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CBomb ***
//=============================================================================
CBomb *CBomb::Create(int nMaxBomb, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nBombNum)
{
	CBomb *pBomb = NULL;				// �|�C���^

	if (pBomb == NULL)
	{// NULL�̏ꍇ
		pBomb = new CBomb;				// �������m��
	}

	if (pBomb != NULL)
	{// NULL�ȊO�̏ꍇ
		pBomb->m_nBombNum = nBombNum;		// ���e�ԍ�

		for (int nCntBomb = 0; nCntBomb < nMaxBomb; nCntBomb++)
		{// �J�E���g
			if (pBomb->m_apScene2D[nCntBomb] == NULL)
			{
				pBomb->m_apScene2D[nCntBomb] = new CScene2D(CScene::PRIORITY_UI);																			// �������m��
				pBomb->m_apScene2D[nCntBomb]->SetInitAll(D3DXVECTOR3(pos.x + ((size.x * 2.0f) * nCntBomb), pos.y, 0.0f), size, col, D3DXVECTOR2(1.0f, 1.0f));		// �����l�ݒ�
				pBomb->m_apScene2D[nCntBomb]->Init();																												// ����������
				pBomb->m_apScene2D[nCntBomb]->SetObjType(CScene::OBJTYPE_BOMB);																						// �I�u�W�F�N�g�^�C�v�ݒ�
				pBomb->m_apScene2D[nCntBomb]->BindTexture(m_pTexture);																								// �e�N�X�`���ݒ�		
			}
		}
		
		pBomb->m_bShotBomb[pBomb->m_nBombNum] = true;			// ����
		pBomb->m_bAddBomb[pBomb->m_nBombNum] = false;			// ���Z
		pBomb->m_bResetBomb[pBomb->m_nBombNum] = false;			// ���Z�b�g
		pBomb->m_bDeleteBomb[pBomb->m_nBombNum] = false;		// �j��
		pBomb->m_nMaxBomb[pBomb->m_nBombNum] = nMaxBomb;		// ���e�ő吔
		pBomb->m_nBombAll[pBomb->m_nBombNum] = nMaxBomb;		// ���e���ݐ�

		pBomb->Init();				// ����������
	}

	return pBomb;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CBomb ***
//=============================================================================
HRESULT CBomb::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CBomb ***
//=============================================================================
void CBomb::Uninit(void)
{
	for (int nCntBomb = 0; nCntBomb < m_nBombAll[m_nBombNum]; nCntBomb++)
	{// �J�E���g
		if (m_apScene2D[nCntBomb] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apScene2D[nCntBomb]->Uninit();	// �I������
			m_apScene2D[nCntBomb] = NULL;		// NULL��
		}
	}

	CScene::Release();
}

//=============================================================================
// �X�V����											(public)	*** CBomb ***
//=============================================================================
void CBomb::Update(void)
{
	if (m_bAddBomb[m_nBombNum] == true && m_nBombAll[m_nBombNum] < m_nMaxBomb[m_nBombNum])
	{
		AddBomb();
		m_bAddBomb[m_nBombNum] = false;
	}
	if (m_bResetBomb[m_nBombNum] == true)
	{
		ResetBomb();
		m_bResetBomb[m_nBombNum] = false;
	}
	if (m_bDeleteBomb[m_nBombNum] == true && 0 < m_nBombAll[m_nBombNum])
	{
		DeleteBomb();
		m_bDeleteBomb[m_nBombNum] = false;
	}

	if (m_nBombAll[m_nBombNum] == m_nMaxBomb[m_nBombNum])
	{
		CGauge::SetReset(m_nBombNum, false);
	}
	else
	{
		CGauge::SetReset(m_nBombNum, true);
	}
}

//=============================================================================
// �`�揈��											(public)	*** CBomb ***
//=============================================================================
void CBomb::Draw(void)
{

}

//=============================================================================
// �ǉ�����											(public)	*** CBomb ***
//=============================================================================
void CBomb::AddBomb(void)
{
	if (m_nBombAll[m_nBombNum] < m_nMaxBomb[m_nBombNum])
	{// �ő吔��菭�Ȃ�
		m_apScene2D[m_nBombAll[m_nBombNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nBombAll[m_nBombNum]++;	// ���ݐ��𑝂₷
		m_bShotBomb[m_nBombNum] = true;
	}
}

//=============================================================================
// ���Z�b�g����											(public)	*** CBomb ***
//=============================================================================
void CBomb::ResetBomb(void)
{
	if (m_nBombAll[m_nBombNum] < m_nMaxBomb[m_nBombNum])
	{// �ő吔��菭�Ȃ�
		for (int nCntBomb = m_nBombAll[m_nBombNum]; nCntBomb < m_nMaxBomb[m_nBombNum]; nCntBomb++)
		{// �J�E���g
			m_apScene2D[nCntBomb]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		m_nBombAll[m_nBombNum] = m_nMaxBomb[m_nBombNum];	// ���ݐ����ő��
		m_bShotBomb[m_nBombNum] = true;
	}
}

//=============================================================================
// �������											(public)	*** CBomb ***
//=============================================================================
void CBomb::DeleteBomb(void)
{
	if (0 < m_nBombAll)
	{// �P�ȏ゠��Ƃ�
		m_nBombAll[m_nBombNum]--;							// ���ݐ������炷
		m_apScene2D[m_nBombAll[m_nBombNum]]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		if (m_nBombAll[m_nBombNum] == 0)
		{
			m_bShotBomb[m_nBombNum] = false;
		}
	}
}