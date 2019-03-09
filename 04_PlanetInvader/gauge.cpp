//=============================================================================
//
// �Q�[�W���� [gauge.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "gauge.h"			// �w�i
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "scene2d.h"		// �V�[��2D
#include "bomb.h"			// ���e

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\bg000.png"		// �e�N�X�`��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			CGauge::m_apTexture = NULL;					// �e�N�X�`�����ւ̃|�C���^
bool						CGauge::m_bResetGauge[MAX_PLAYER] = {};		// �Q�[�W���Z�b�g

//=============================================================================
// �R���X�g���N�^									(public)	*** CGauge ***
//=============================================================================
CGauge::CGauge() : CScene(CScene::PRIORITY_UI)
{
	// �����l�ݒ�
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX_TEXTURE; nCntGauge++)
	{// �J�E���g
		m_apScene2D[nCntGauge] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CGauge ***
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
// ���[�h����							(public)	*** CGauge ***
//=============================================================================
HRESULT CGauge::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	if (m_apTexture == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_apTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CGauge ***
//=============================================================================
void CGauge::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CGauge ***
//=============================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nGaugeNum)
{
	CGauge *pGauge = NULL;					// �|�C���^

	if (pGauge == NULL)
	{// NULL�̏ꍇ
		pGauge = new CGauge;				// �������m��
	}

	if (pGauge != NULL)
	{// NULL�ȊO�̏ꍇ
		pGauge->m_nGaugeNum = nGaugeNum;	// �Q�[�W�ԍ�

		for (int nCntGauge = 0; nCntGauge < GAUGE_MAX_TEXTURE; nCntGauge++)
		{// �J�E���g
			if (pGauge->m_apScene2D[nCntGauge] == NULL)
			{// NULL�ȊO�̏ꍇ
				pGauge->m_apScene2D[nCntGauge] = new CScene2D(CScene::PRIORITY_UI);		// �������m��

				switch (nCntGauge)
				{// �J�E���g�ԍ�
				case 0:
					pGauge->m_apScene2D[nCntGauge]->SetInitAll(pos, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));					// �����l�ݒ�
					pGauge->m_pos[pGauge->m_nGaugeNum][nCntGauge] = pos;																										// �ʒu
					pGauge->m_size[pGauge->m_nGaugeNum][nCntGauge] = size;																									// �傫��

					break;
				case 1:
					pGauge->m_apScene2D[nCntGauge]->SetInitAll(pos, D3DXVECTOR3(size.x * 0.95f, size.y * 0.8f, 0.0f), col, D3DXVECTOR2(1.0f, 1.0f));	// �����l�ݒ�
					pGauge->m_pos[pGauge->m_nGaugeNum][nCntGauge] = pos;																										// �ʒu
					pGauge->m_size[pGauge->m_nGaugeNum][nCntGauge] = D3DXVECTOR3(size.x * 0.95f, size.y * 0.8f, 0.0f);														// �傫��

					break;
				}
				pGauge->m_apScene2D[nCntGauge]->Init();										// ����������
				pGauge->m_apScene2D[nCntGauge]->SetObjType(CScene::OBJTYPE_GAUGE);			// �I�u�W�F�N�g�^�C�v�ݒ�
				//pGauge->m_apScene2D[nCntGauge]->BindTexture(m_apTexture);					// �e�N�X�`���ݒ�
			}
		}
		pGauge->m_bResetGauge[pGauge->m_nGaugeNum] = false;		// ���Z�b�g

		pGauge->Init();		// ����������
	}

	return pGauge;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CGauge ***
//=============================================================================
HRESULT CGauge::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CGauge ***
//=============================================================================
void CGauge::Uninit(void)
{
	for (int nCntGauge = 0; nCntGauge < GAUGE_MAX_TEXTURE; nCntGauge++)
	{// �J�E���g
		if (m_apScene2D[nCntGauge] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apScene2D[nCntGauge]->Uninit();		// �I������
			m_apScene2D[nCntGauge] = NULL;			// NULL��
		}
	}

	CScene::Release();		// �����[�X
}

//=============================================================================
// �X�V����											(public)	*** CGauge ***
//=============================================================================
void CGauge::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();			// ���̓f�o�C�X
	float fDiff = (m_pos[m_nGaugeNum][1].x - m_size[m_nGaugeNum][1].x) - (m_pos[m_nGaugeNum][0].x - m_size[m_nGaugeNum][0].x);	// �Ŕw�ʂƂ̈ʒu�̍������v�Z
	D3DXVECTOR3 pos = m_apScene2D[1]->Getpos();		// �ʒu���擾
	D3DXVECTOR3 size = m_apScene2D[1]->Getsize();	// �傫�����擾
	
#ifdef _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_X) == true)
	{
		m_apScene2D[1]->Setsize(D3DXVECTOR3(0.0f, m_size[m_nGaugeNum][1].y, m_size[m_nGaugeNum][1].z));
	}
#endif

	if (size.x != m_size[m_nGaugeNum][1].x)
	{// �傫�����ύX���ꂽ
		if (m_size[m_nGaugeNum][1].x < size.x)
		{// �傫�����ő�ȉ�
			size.x = m_size[m_nGaugeNum][1].x;	// �傫����ݒ�̒l��
			CBomb::SetAdd(m_nGaugeNum, true);
		}
		else
		{
			size.x += 0.23f;				// �傫�����Ă���
		}

		m_apScene2D[1]->Setpos(D3DXVECTOR3((m_pos[m_nGaugeNum][0].x - m_size[m_nGaugeNum][0].x) + fDiff + size.x, m_pos[m_nGaugeNum][1].y, m_pos[m_nGaugeNum][1].z));	// �ʒu��ݒ�
		m_apScene2D[1]->Setsize(size);		// �傫����ݒ�
	}
	else
	{
		if (m_bResetGauge[m_nGaugeNum] == true)
		{
			m_apScene2D[1]->Setsize(D3DXVECTOR3(0.0f, m_size[m_nGaugeNum][1].y, m_size[m_nGaugeNum][1].z));
			m_bResetGauge[m_nGaugeNum] = false;
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CGauge ***
//=============================================================================
void CGauge::Draw(void)
{

}