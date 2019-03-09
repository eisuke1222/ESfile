//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "bg.h"				// �w�i
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "scene2d.h"		// �V�[��2D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\bg011.png"		// �e�N�X�`��1
#define TEXTURE_NAME_2	"data\\TEXTURE\\bg08.png"		// �e�N�X�`��2
#define MOVE_SPEED_1	(-0.001f)						// �ړ����x1
#define MOVE_SPEED_2	(-0.0015f)						// �ړ����x2
#define MOVE_SPEED_3	(-0.002f)						// �ړ����x3

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBg::m_apTexture[TEXTURE_MAX]	= {};		// �e�N�X�`�����ւ̃|�C���^
int					CBg::m_nSetNum					= 0;		// �Z�b�g��

//=============================================================================
// �R���X�g���N�^									(public)	*** CBg ***
//=============================================================================
CBg::CBg() : CScene(CScene::PRIORITY_BG)
{
	// �����l�ݒ�
	for (int nCntBg = 0; nCntBg < TEXTURE_MAX; nCntBg++)
	{// �J�E���g
		m_apScene2D[nCntBg] = NULL;
		m_posUV[nCntBg] = D3DXVECTOR2(0.0f, 0.0f);			// �ʒu
	}
	m_nSetNum = 0;											// �Z�b�g��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CBg ***
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBg ***
//=============================================================================
HRESULT CBg::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	if (m_apTexture[0] == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_apTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_apTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBg ***
//=============================================================================
void CBg::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �J�E���g
		if (m_apTexture[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apTexture[nCntTex]->Release();		// ���
			m_apTexture[nCntTex] = NULL;			// NULL��
		}
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CBg ***
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nSetNum, int nTexNum)
{
	CBg *pBg = NULL;				// �|�C���^

	if (pBg == NULL)
	{// NULL�̏ꍇ
		pBg = new CBg;				// �������m��
	}

	if (pBg != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntBg = 0; nCntBg < nSetNum; nCntBg++)
		{// �J�E���g
			if (pBg->m_apScene2D[nCntBg] == NULL)
			{// NULL�ȊO�̏ꍇ
				pBg->m_apScene2D[nCntBg] = new CScene2D(CScene::PRIORITY_BG);					// �������m��
				pBg->m_apScene2D[nCntBg]->SetInitAll(pos, size, col, D3DXVECTOR2(1.0f, 1.0f));	// �����l�ݒ�
				pBg->m_apScene2D[nCntBg]->Init();												// ����������
				pBg->m_apScene2D[nCntBg]->SetObjType(CScene::OBJTYPE_BG);						// �I�u�W�F�N�g�^�C�v�ݒ�
				pBg->m_apScene2D[nCntBg]->BindTexture(m_apTexture[nTexNum]);					// �e�N�X�`���ݒ�
			}
		}
		
		pBg->Init();			// ����������
		m_nSetNum = nSetNum;	// �Z�b�g���ݒ�
	}

	return pBg;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CBg ***
//=============================================================================
HRESULT CBg::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CBg ***
//=============================================================================
void CBg::Uninit(void)
{
	for (int nCntBg = 0; nCntBg < m_nSetNum; nCntBg++)
	{// �J�E���g
		if (m_apScene2D[nCntBg] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apScene2D[nCntBg]->Uninit();		// �I������
			m_apScene2D[nCntBg] = NULL;			// NULL��
		}
	}

	CScene::Release();
}

//=============================================================================
// �X�V����											(public)	*** CBg ***
//=============================================================================
void CBg::Update(void)
{

}

//=============================================================================
// �`�揈��											(public)	*** CBg ***
//=============================================================================
void CBg::Draw(void)
{

}