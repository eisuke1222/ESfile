//=============================================================================
//
//	���b�V��3D���� [mesh3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "mesh3d.h"			// ���b�V��3D
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
// �R���X�g���N�^								(public)	*** CMesh3D ***
//=============================================================================
CMesh3D::CMesh3D(int nPriority) : CScene(nPriority)
{
	// �����l�ݒ�
	m_pTexture	= NULL;								// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff	= NULL;								// �}�e���A�����ւ̃|�C���^
	m_pIdxBuff	= NULL;								// �C���f�b�N�X���ւ̃|�C���^

	m_meshType	= MESHTYPE_FIELD;					// ���b�V���^�C�v
	m_num		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);			// UV
}

//=============================================================================
// �R���X�g���N�^								(public)	*** CMesh3D ***
//=============================================================================
CMesh3D::CMesh3D(int nPriority, int nID) : CScene(nPriority, nID)
{
	// �����l�ݒ�
	m_pTexture	= NULL;								// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff	= NULL;								// �}�e���A�����ւ̃|�C���^
	m_pIdxBuff	= NULL;								// �C���f�b�N�X���ւ̃|�C���^

	m_meshType	= MESHTYPE_FIELD;					// ���b�V���^�C�v
	m_num		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);			// UV
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CMesh3D ***
//=============================================================================
CMesh3D::~CMesh3D()
{

}

//=============================================================================
// ����������									(public)	*** CMesh3D ***
//=============================================================================
HRESULT CMesh3D::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pVtxBuff->Release();		// ���
		m_pVtxBuff = NULL;			// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIdxBuff->Release();		// ���
		m_pIdxBuff = NULL;			// NULL��
	}

	CScene::Release();				// �����[�X
}

//=============================================================================
// �X�V����										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::Update(void)
{

}

//=============================================================================
// �`�揈��										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �t�B�[���h�̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nMaxIdx, 0, m_nMaxPolygon);

}

//=============================================================================
// �ݒ菈��										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::SetInitAll(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	// �f�o�C�X�擾6
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_meshType = meshType;		// ���b�V���^�C�v
	m_meshTex = meshTex;		// �e�N�X�`���^�C�v
	m_num = num;				// ����
	m_pos = pos;				// �ʒu
	m_rot = rot;				// ����
	m_TexUV = TexUV;			// UV

	D3DXVECTOR2 tex;			// UV�v�Z�p
	int nCount;					// yz����

	switch (meshType)
	{// ���b�V���^�C�v
	case MESHTYPE_FIELD:		// �t�B�[���h
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.z + 1);							// ���_��
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.z + (((int)num.z - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)num.x * (int)num.z * 2) + (((int)num.z - 1) * 4);		// �|���S����

		nCount = (int)num.z;	// z����

		break;
	case MESHTYPE_WALL:			// ��
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// ���_��
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// �|���S����

		nCount = (int)num.y;	// y����

		break;
	case MESHTYPE_CYLINDER:		// �V�����_�[
		m_nMaxVtx = ((int)num.x + 1) * ((int)num.y + 1);							// ���_��
		m_nMaxIdx = (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// �|���S����

		if (size.z == 0.0f) size.z = 1.0f;		// �\����
		nCount = (int)num.y;	// y����

		break;
	}

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMaxVtx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �C���f�b�N�X�o�b�t�@����
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nMaxIdx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCount_yz = 0; nCount_yz < nCount + 1; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < num.x + 1; nCount_x++)
		{// x���J�E���g
			switch (meshType)
			{// ���b�V���^�C�v
			case MESHTYPE_FIELD:		// �t�B�[���h
				// ���_���̐ݒ�
				pVtx[0].pos =
					//D3DXVECTOR3(-((size.x / 2) * num.x) + (size.x * nCount_x), 0.0f,((size.z / 2) * num.z) - (size.z * (nCount_yz)));
					D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCount_x, 0.0f, size.z - ((size.z * 2) / num.z) * nCount_yz);
				
				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCount_yz));	
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = tex;

				// �@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				
				break;
			case MESHTYPE_WALL:			// �E�H�[��
				// ���_���̐ݒ�
				pVtx[0].pos =
					//D3DXVECTOR3(-((size.x / 2) * num.x) + (size.x * nCount_x), 0.0f,((size.y / 2) * num.y) - (size.y * (nCount_yz)));
					D3DXVECTOR3(-size.x + ((size.x * 2) / num.x) * nCount_x, ((size.y * num.y) - (size.y * nCount_yz)), 0.0f);

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = tex;
				
				// �@���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_CYLINDER:		// �V�����_�[
				// ���_���̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(
					sinf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCount_x) * size.z) * size.x,
					((size.y) * num.y) - (size.y * (nCount_yz)),
					cosf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCount_x) * size.z) * size.x);

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.z)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = tex;

				// �@���̐ݒ�
				D3DXVECTOR3 vecNor;
				vecNor.x = pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);
				vecNor.z = pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);
				D3DXVec3Normalize(&vecNor, &vecNor);					// ���K������
				pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);
				//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			}

			// ���_�J���[�̐ݒ�
			pVtx[0].col = col;

			pVtx += 1;			// ���_�o�b�t�@��i�߂�
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;					// �C���f�b�N�X�f�[�^�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int nCount_yz = 0; nCount_yz < nCount; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < num.x + 1; nCount_x++)
		{// x���J�E���g
			// �C���f�b�N�X���̐ݒ�
			pIdx[0] = (WORD)(((nCount_yz + 1) * (num.x + 1)) + nCount_x);
			pIdx[1] = (WORD)(((nCount_yz) * (num.x + 1)) + nCount_x);

			pIdx += 2;			// �C���f�b�N�X�o�b�t�@��i�߂�

			if (nCount > 1 && nCount_yz != nCount - 1 && nCount_x == num.x)
			{// ���[�ɓ��B������
			 // �C���f�b�N�X���̐ݒ�i�k�ރ|���S���j
				pIdx[0] = (WORD)(((nCount_yz) * (num.x + 1)) + nCount_x);
				pIdx[1] = (WORD)(((nCount_yz + 2) * (num.x + 1)));

				pIdx += 2;		// �C���f�b�N�X�o�b�t�@��i�߂�
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}

//=============================================================================
// UV�ړ�����										(public)	*** CMesh3D ***
//=============================================================================
void CMesh3D::SetTex(D3DXVECTOR2 TexMoveUV)
{
	int nCount;						// yz����

	switch (m_meshType)
	{// ���b�V���^�C�v
	case MESHTYPE_FIELD:			// �t�B�[���h
		nCount = (int)m_num.z;		// z����

		break;
	case MESHTYPE_WALL:				// �E�H�[��
		nCount = (int)m_num.y;		// y����

		break;
	case MESHTYPE_CYLINDER:			// �V�����_�[
		nCount = (int)m_num.y;		// y����

		break;
	}

	VERTEX_3D *pVtx;				// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount_yz = 0; nCount_yz < nCount + 1; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// x���J�E���g
			 // �e�N�X�`���̐ݒ�
			switch (m_meshTex)
			{// �e�N�X�`���^�C�v
			case MESHTEX_ALL:		// �S��
				pVtx[0].tex =
					D3DXVECTOR2((m_TexUV.x / (m_num.x)) * (nCount_x % ((int)m_num.x + 1)) + TexMoveUV.x, (m_TexUV.y / (m_num.y)) * (nCount_yz)+TexMoveUV.y);

				break;
			case MESHTEX_NUMSIZE:	// ����
				pVtx[0].tex =
					D3DXVECTOR2(m_TexUV.x * (nCount_x % ((int)m_num.x + 1)) + TexMoveUV.x, m_TexUV.y * (nCount_yz)+TexMoveUV.y);

				break;
			}
								
			pVtx += 1;		// ���_�o�b�t�@��i�߂�
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}