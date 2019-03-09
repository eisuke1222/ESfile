//=============================================================================
//
// �V�[��X���� [scenex.h]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scenex.h"			// �V�[��X
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
// �R���X�g���N�^									(public)	*** CSceneX ***
//=============================================================================
CSceneX::CSceneX(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture			= NULL;									// �e�N�X�`�����ւ̃|�C���^
	m_pMesh				= NULL;									// ���b�V�����ւ̃|�C���^
	m_pBuffMat			= NULL;									// �}�e���A�����ւ̃|�C���^
	m_nNumMat			= NULL;									// �}�e���A�����̐�

	m_pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_col				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
	m_bcolChange_rgb	= false;								// �F�ύXrgb
	m_bcolChange_a		= false;								// �F�ύXa
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CSceneX ***
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// ����������									(public)	*** CSceneX ***
//=============================================================================Z
HRESULT CSceneX::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	int		nNumVtx;		// ���_��
	DWORD	sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE	*pVtxBuff;		// ���_�o�b�t�@�̃|�C���^

	if (m_pMesh != NULL)
	{// NULL�ȊO�̏ꍇ
		// ���_���̎擾
		nNumVtx = m_pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// �J�E���g
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// ���_���W�̑��

			// �ő�����߂�
			if (vtx.x > m_vtxMax.x)	m_vtxMax.x = vtx.x;
			if (vtx.y > m_vtxMax.y)	m_vtxMax.y = vtx.y;
			if (vtx.z > m_vtxMax.z)	m_vtxMax.z = vtx.z;

			// �ŏ������߂�
			if (vtx.x < m_vtxMin.x)	m_vtxMin.x = vtx.x;
			if (vtx.y < m_vtxMin.y)	m_vtxMin.y = vtx.y;
			if (vtx.z < m_vtxMin.z)	m_vtxMin.z = vtx.z;

			pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N
		m_pMesh->UnlockVertexBuffer();
	}

	return S_OK;
}

//=============================================================================
// �I������										(public)	*** CSceneX ***
//=============================================================================
void CSceneX::Uninit(void)
{
	// �V�[���J��
	CScene::SetDeath();
}

//=============================================================================
// �X�V����										(public)	*** CSceneX ***
//=============================================================================
void CSceneX::Update(void)
{
	// ���݌�������
	if (m_rot.x > D3DX_PI)	{ m_rot.x -= D3DX_PI * 2.0f; }
	if (m_rot.x < -D3DX_PI) { m_rot.x += D3DX_PI * 2.0f; }
	if (m_rot.y > D3DX_PI)	{ m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }
	if (m_rot.z > D3DX_PI)	{ m_rot.z -= D3DX_PI * 2.0f; }
	if (m_rot.z < -D3DX_PI) { m_rot.z += D3DX_PI * 2.0f; }
}

//=============================================================================
// �`�揈��										(public)	*** CSceneX ***
//=============================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX			mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X
	D3DXMATRIX			mtxWorld;					// �e�}�g���b�N�X
	D3DMATERIAL9		matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL		*pmat = NULL;				// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxWorld);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_pBuffMat != NULL)
	{// NULL�ȊO�̏ꍇ
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	}
	D3DXCOLOR col;	// �F

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// �J�E���g
		if (m_pBuffMat != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_bcolChange_rgb == true)
			{// �F�ύX
				col = pmat[nCntMat].MatD3D.Diffuse;
				pmat[nCntMat].MatD3D.Diffuse = m_col;
			}
			if (m_bcolChange_a == true)
			{// �F�ύX
				col.a = pmat[nCntMat].MatD3D.Diffuse.a;
				pmat[nCntMat].MatD3D.Diffuse.a = m_col.a;
			}

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pmat[nCntMat].MatD3D);
		}

		if (m_pTexture[nCntMat] != NULL)
		{// NULL�ȊO�̏ꍇ
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}
		else
		{// NULL�̏ꍇ
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, NULL);
		}

		if (m_pMesh != NULL)
		{
			// �I�u�W�F�N�g(�p�[�c)�̕`��
			m_pMesh->DrawSubset(nCntMat);
		}

		if (m_bcolChange_rgb == true)
		{// �F�ύX
			pmat[nCntMat].MatD3D.Diffuse = col;
		}
		if (m_bcolChange_a == true)
		{// �F�ύX
			pmat[nCntMat].MatD3D.Diffuse.a = col.a;
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	m_bcolChange_rgb	= false;		// �ύX�I��
	m_bcolChange_a		= false;		// �ύX�I��
}