//=============================================================================
//
// �V�[�����b�V���I�[�r�b�g���� [scenemeshorbit.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scenemeshorbit.h"	// �V�[�����b�V���I�[�r�b�g
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "font.h"			// �t�H���g

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

//=============================================================================
// �R���X�g���N�^								(public)	*** CSceneMeshOrbit ***
//=============================================================================
CSceneMeshOrbit::CSceneMeshOrbit(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture		= NULL;										// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff		= NULL;										// ���_�o�b�t�@���ւ̃|�C���^

	m_nMaxVtx		= 0;										// �ő咸�_��

	m_aPosVtx		= NULL;										// ���_�ʒu
	m_fMaxcol_a		= 1.0f;										// �ő哧���x
	m_fMincol_a		= 0.0f; 									// �ŏ������x
																
	for (int nCount = 0; nCount < m_MAXOFFSET; nCount++)
		m_aOffSet[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �I�t�Z�b�g�ʒu
}

//=============================================================================
// �f�X�g���N�^								(public)	*** CSceneMeshOrbit ***
//=============================================================================
CSceneMeshOrbit::~CSceneMeshOrbit()
{

}

//=============================================================================
// ���[�h����								(public)	*** CSceneMeshOrbit ***
//=============================================================================
HRESULT CSceneMeshOrbit::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����								(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Unload(void)
{

}

//=============================================================================
// �m�ۏ���									(public)	*** CSceneMeshOrbit ***
//=============================================================================
CSceneMeshOrbit *CSceneMeshOrbit::Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1, D3DXVECTOR3 col_rgb, float fMaxcol_a, float fMincol_a, int nMaxVtx, LPDIRECT3DTEXTURE9 pTexture)
{
	CSceneMeshOrbit *pOrbit = NULL;			// �|�C���^

	if (pOrbit == NULL)
	{// NULL�̏ꍇ
		pOrbit = new CSceneMeshOrbit(CScene::PRIORITY_3, CScene::OBJTYPE_MESHFIELD);	// �������m��
	}

	if (pOrbit != NULL)
	{// NULL�ȊO�̏ꍇ
		pOrbit->m_pTexture		= pTexture;		// �e�N�X�`��
		pOrbit->m_nMaxVtx		= nMaxVtx;		// �ő咸�_��
		pOrbit->m_aOffSet[0]	= OffSet_0;		// �I�t�Z�b�g�O
		pOrbit->m_aOffSet[1]	= OffSet_1;		// �I�t�Z�b�g�P
		pOrbit->m_col_rgb		= col_rgb;		// �F
		pOrbit->m_fMaxcol_a		= fMaxcol_a;	// �ő哧���x
		pOrbit->m_fMincol_a		= fMincol_a;	// �ŏ������x

		if (pOrbit->m_aPosVtx == NULL)
			pOrbit->m_aPosVtx = new D3DXVECTOR3[nMaxVtx];		// �������m��

		pOrbit->Init();		// ����������
	}

	return pOrbit;		// �l��Ԃ�
}

//=============================================================================
// ����������									(public)	*** CSceneMeshOrbit ***
//=============================================================================
HRESULT CSceneMeshOrbit::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// �f�o�C�X�擾

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMaxVtx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < m_nMaxVtx; nCount++)
	{
		// ���_����ݒ�
		pVtx[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �@���̐ݒ�
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if (nCount % 2 == 0)
		{// ��
			// ���_�J���[�ݒ�
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMincol_a - ((m_fMincol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
			// �e�N�X�`���ݒ�
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (m_nMaxVtx * 0.5f)) * (nCount / 2)), 1.0f);
		}
		else
		{// ��
			// ���_�J���[�ݒ�
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMaxcol_a - ((m_fMaxcol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
			// �e�N�X�`���ݒ�
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (m_nMaxVtx * 0.5f)) * (nCount / 2)), 0.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������									(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Uninit(void)
{
	if (m_aPosVtx != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_aPosVtx;			// �J��
		m_aPosVtx = NULL;			// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pVtxBuff->Release();		// ���
		m_pVtxBuff = NULL;			// NULL��
	}

	CScene::SetDeath();		// ���S�t���O
}

//=============================================================================
// �X�V����									(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Update(void)
{
	// ����ւ�
	for (int nCount = m_nMaxVtx - 1; 1 < nCount; nCount--)
	{// ���_���J�E���g
		m_aPosVtx[nCount] = m_aPosVtx[nCount - 2];
	}

	// �I�t�Z�b�g�ʒu�ݒ�
	for (int nCount = 0; nCount < m_MAXOFFSET; nCount++)
	{// �I�t�Z�b�g���J�E���g
		D3DXVec3TransformCoord(&m_aPosVtx[nCount], &m_aOffSet[nCount], m_pMtxParent);
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < m_nMaxVtx; nCount++)
	{// ���_���J�E���g
		// ���_����ݒ�		
		pVtx[nCount].pos = m_aPosVtx[nCount];

		if (nCount % 2 == 0)
		{// ��
		 // ���_�J���[�ݒ�
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMincol_a - ((m_fMincol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
		}
		else
		{// ��
		 // ���_�J���[�ݒ�
			pVtx[nCount].col = D3DXCOLOR(m_col_rgb.x, m_col_rgb.y, m_col_rgb.z, (m_fMaxcol_a - ((m_fMaxcol_a / (m_nMaxVtx / 2)) * (nCount / 2))));
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��									(public)	*** CSceneMeshOrbit ***
//=============================================================================
void CSceneMeshOrbit::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	D3DXMATRIX			mtxWorld;										// �v�Z�p�}�g���b�N�X

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ���ʂ��J�����O

	// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, NULL);
	//pDevice->SetTexture(0, m_pTexture);
	
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nMaxVtx - 2);

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z�o�b�t�@�ւ̏�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
}