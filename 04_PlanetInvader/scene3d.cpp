//=============================================================================
//
// �V�[��3D���� [scene3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3d.h"		// �V�[��3D
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[

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
// �R���X�g���N�^								(public)	*** CScene3D ***
//=============================================================================
CScene3D::CScene3D(int nPriority) : CScene(nPriority)
{
	// �����l�ݒ�
	m_pTexture	= NULL;									// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff	= NULL;									// �}�e���A�����ւ̃|�C���^

	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);				// UV
}

//=============================================================================
// �R���X�g���N�^								(public)	*** CScene3D ***
//=============================================================================
CScene3D::CScene3D(int nPriority, int nID) : CScene(nPriority, nID)
{
	// �����l�ݒ�
	m_pTexture	= NULL;									// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff	= NULL;									// �}�e���A�����ւ̃|�C���^

	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);				// UV
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CScene3D ***
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// ����������									(public)	*** CScene3D ***
//=============================================================================
HRESULT CScene3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// �f�o�C�X�擾

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);
	// �@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	// ���_�J���[�ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_TexUV.x - m_TexUV.x, m_TexUV.y - m_TexUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x,			 m_TexUV.y - m_TexUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexUV.x - m_TexUV.x, m_TexUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x,			 m_TexUV.y);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������										(public)	*** CScene3D ***
//=============================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pVtxBuff->Release();		// ���
		m_pVtxBuff = NULL;			// NULL��
	}

	// �V�[���J��
	CScene::Release();
}

//=============================================================================
// �X�V����										(public)	*** CScene3D ***
//=============================================================================
void CScene3D::Update(void)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f);
	// ���_�J���[�ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��										(public)	*** CScene3D ***
//=============================================================================
void CScene3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// �f�o�C�X�擾

	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X

	// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	
	if (m_scene3dType == SCENE3DTYPE_NORMAL)
	{
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}
	else if (m_scene3dType == SCENE3DTYPE_BILLBOARD)
	{
		// �t�s��
		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
			

//=============================================================================
// �A�j���[�V��������							(public)	*** CScene3D ***
//=============================================================================
void CScene3D::SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV)
{
	if (nAnimLineNum <= 0) nAnimLineNum = 1;

	m_TexUV = TexUV;	// UV�ύX

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum)				+ TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum)			 + TexMoveUV.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum) + m_TexUV.x + TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum)			 + TexMoveUV.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum)				+ TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum) + m_TexUV.y + TexMoveUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x * (nAnimPattern % nAnimLineNum) + m_TexUV.x + TexMoveUV.x, m_TexUV.y * (nAnimPattern / nAnimLineNum) + m_TexUV.y + TexMoveUV.y);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}