//=============================================================================
//
//	�V�[�����b�V������ [scenemesh.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scenemesh.h"		// �V�[�����b�V��
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
// �R���X�g���N�^								(public)	*** CSceneMesh ***
//=============================================================================
CSceneMesh::CSceneMesh(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture		= NULL;								// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff		= NULL;								// �}�e���A�����ւ̃|�C���^
	m_pIdxBuff		= NULL;								// �C���f�b�N�X���ւ̃|�C���^

	m_meshType		= MESHTYPE_FIELD;					// ���b�V���^�C�v
	m_num			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_TexUV			= D3DXVECTOR2(0.0f, 0.0f);			// UV

	m_bDrawAdd		= false;							// ���Z����
	m_bDrawSubtract = false;							// ���Y����
	m_bDrawATest	= false;							// ���e�X�g
	m_bDrawZWrite	= false;							// Z���C�g
	m_bDrawLight	= false;							// ���C�g�e��
	m_nATest		= 1;								// ���e�X�g���l
	m_nCullMode		= D3DCULL_CCW;						// �J�����O
	m_dCmpValue		= D3DCMP_LESSEQUAL;						// Z���C�g�ݒ�
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CSceneMesh ***
//=============================================================================
CSceneMesh::~CSceneMesh()
{

}

//=============================================================================
// ����������									(public)	*** CSceneMesh ***
//=============================================================================
HRESULT CSceneMesh::Init(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::Uninit(void)
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

	CScene::SetDeath();				// ���S�t���O
}

//=============================================================================
// �X�V����										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::Update(void)
{

}

//=============================================================================
// �`�揈��										(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX			mtxRot, mtxTrans;									// �v�Z�p�}�g���b�N�X

	if (m_bDrawAdd == true)
	{// ���Z����
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bDrawSubtract == true)
	{// ���Z����
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bDrawATest == true)
	{// �A���t�@�e�X�g
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, m_nATest);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	if (m_bDrawZWrite == true)
	{// Z���C�g
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, m_dCmpValue);
	}
	if (m_bDrawLight == true)
	{// ���C�g�e��
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, m_nCullMode);				// ���ʂ��J�����O

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

	if (m_bDrawAdd == true)
	{// ���Z����
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bDrawSubtract == true)
	{// ���Z����
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bDrawATest == true)
	{// �A���t�@�e�X�g
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
	if (m_bDrawZWrite == true)
	{// Z���C�g
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
	if (m_bDrawLight == true)
	{// ���C�g�e��
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
}

//=============================================================================
// �����l�ݒ菈��								(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetInitAll(MESHTYPE meshType, MESHTEX meshTex, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	m_meshType	= meshType;		// ���b�V���^�C�v
	m_meshTex	= meshTex;		// �e�N�X�`���^�C�v
	m_num		= num;			// ����
	m_pos		= pos;			// �ʒu
	m_rot		= rot;			// ����
	m_size		= size;			// �傫��
	m_TexUV		= TexUV;		// UV

	int			nCount;			// yz����
	D3DXVECTOR2 tex;			// UV�v�Z�p
	D3DXVECTOR3 vecNor;			// �@���v�Z�p

	switch (meshType)
	{// ���b�V���^�C�v
	case MESHTYPE_FIELD:		// �t�B�[���h
		m_nMaxVtx		= ((int)num.x + 1) * ((int)num.z + 1);							// ���_��
		m_nMaxIdx		= (((int)num.x + 1) * 2) * (int)num.z + (((int)num.z - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon	= ((int)num.x * (int)num.z * 2) + (((int)num.z - 1) * 4);		// �|���S����

		nCount = (int)num.z;	// z����

		break;
	case MESHTYPE_WALL:			// ��
		m_nMaxVtx		= ((int)num.x + 1) * ((int)num.y + 1);							// ���_��
		m_nMaxIdx		= (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon	= ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// �|���S����

		nCount = (int)num.y;	// y����

		break;
	case MESHTYPE_CYLINDER:		// �V�����_�[
		m_nMaxVtx		= ((int)num.x + 1) * ((int)num.y + 1);							// ���_��
		m_nMaxIdx		= (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon	= ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// �|���S����

		if (size.z == 0.0f) size.z = 1.0f;		// �\����
		nCount = (int)num.y;	// y����

		break;
	case MESHTYPE_DOME:			// �h�[��
		m_nMaxVtx		= ((int)num.x + 1) * ((int)num.y + 1);							// ���_��
		m_nMaxIdx		= (((int)num.x + 1) * 2) * (int)num.y + (((int)num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon	= ((int)num.x * (int)num.y * 2) + (((int)num.y - 1) * 4);		// �|���S����

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

				//pVtx[0].pos.y = -12.0f + (float)(rand() % 12);

				/*if (nCount_x == 0 || nCount_x == num.x)
				{
				pVtx[0].pos.y = 200.0f;
				}*/

				/*if ((int)(m_num.x / 2) < nCount_x)
				{
				pVtx[0].pos.y = 100.0f * (nCount_x - (m_num.x / 2));
				}*/

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
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.y)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = tex;

				// �@���̐ݒ�
				vecNor.x = pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);
				vecNor.z = pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);
				D3DXVec3Normalize(&vecNor, &vecNor);					// ���K������
				pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);
				//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_DOME:			// �h�[��
				// ���_���̐ݒ�
				float fPercent = (((size.y) * num.y) - (size.y * (nCount_yz))) / ((size.y) * num.y);

				pVtx[0].pos = D3DXVECTOR3(
					sinf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCount_x) * size.z) * (cosf(((D3DX_PI * 0.5f) * (fPercent))) * size.x),
					((size.y / num.y)) - ((size.y / num.y) * (nCount_yz)),
					//0.0f,
					cosf(D3DX_PI - ((D3DX_PI * (1.0f / (num.x / 2))) * nCount_x) * size.z) * (cosf(((D3DX_PI * 0.5f) * (fPercent))) * size.x));

				float fRangeMax = sqrtf(powf(size.x, 2.0f) + powf(size.x, 2.0f));
				float fRange	= sqrtf(powf(pVtx[0].pos.x, 2.0f) + powf(pVtx[0].pos.z, 2.0f));
				pVtx[0].pos.y = cosf(((D3DX_PI * 0.5f) * (nCount_yz / num.y))) * size.y;

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (num.x)) * (nCount_x % ((int)num.x + 1)), (TexUV.y / (num.y)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = tex;

				// �@���̐ݒ�				
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
// �X�e�[�^�X�ݒ菈��								(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetStart(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
{
	m_pos		= pos;			// �ʒu
	m_rot		= rot;			// ����
	m_size		= size;			// �傫��
	m_TexUV		= TexUV;		// UV

	int			nCount;			// yz����
	D3DXVECTOR2 tex;			// UV�v�Z�p
	D3DXVECTOR3 vecNor;			// �@���v�Z�p

	switch (m_meshType)
	{// ���b�V���^�C�v
	case MESHTYPE_FIELD:		// �t�B�[���h
		m_nMaxVtx = ((int)m_num.x + 1) * ((int)m_num.z + 1);							// ���_��
		m_nMaxIdx = (((int)m_num.x + 1) * 2) * (int)m_num.z + (((int)m_num.z - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)m_num.x * (int)m_num.z * 2) + (((int)m_num.z - 1) * 4);		// �|���S����

		nCount = (int)m_num.z;	// z����

		break;
	case MESHTYPE_WALL:			// ��
		m_nMaxVtx = ((int)m_num.x + 1) * ((int)m_num.y + 1);							// ���_��
		m_nMaxIdx = (((int)m_num.x + 1) * 2) * (int)m_num.y + (((int)m_num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)m_num.x * (int)m_num.y * 2) + (((int)m_num.y - 1) * 4);		// �|���S����

		nCount = (int)m_num.y;	// y����

		break;
	case MESHTYPE_CYLINDER:		// �V�����_�[
		m_nMaxVtx = ((int)m_num.x + 1) * ((int)m_num.y + 1);							// ���_��
		m_nMaxIdx = (((int)m_num.x + 1) * 2) * (int)m_num.y + (((int)m_num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)m_num.x * (int)m_num.y * 2) + (((int)m_num.y - 1) * 4);		// �|���S����

		if (size.z == 0.0f) size.z = 1.0f;		// �\����
		nCount = (int)m_num.y;	// y����

		break;
	case MESHTYPE_DOME:			// �h�[��
		m_nMaxVtx = ((int)m_num.x + 1) * ((int)m_num.y + 1);							// ���_��
		m_nMaxIdx = (((int)m_num.x + 1) * 2) * (int)m_num.y + (((int)m_num.y - 1) * 2);	// �C���f�b�N�X��
		m_nMaxPolygon = ((int)m_num.x * (int)m_num.y * 2) + (((int)m_num.y - 1) * 4);		// �|���S����

		if (size.z == 0.0f) size.z = 1.0f;		// �\����
		nCount = (int)m_num.y;	// y����

		break;
	}

	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCount_yz = 0; nCount_yz < nCount + 1; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// x���J�E���g
			switch (m_meshType)
			{// ���b�V���^�C�v
			case MESHTYPE_FIELD:		// �t�B�[���h
				// ���_���̐ݒ�
				pVtx[0].pos =
					//D3DXVECTOR3(-((size.x / 2) * m_num.x) + (size.x * nCount_x), 0.0f,((size.z / 2) * m_num.z) - (size.z * (nCount_yz)));
					D3DXVECTOR3(-size.x + ((size.x * 2) / m_num.x) * nCount_x, 0.0f, size.z - ((size.z * 2) / m_num.z) * nCount_yz);

				//pVtx[0].pos.y = -12.0f + (float)(rand() % 12);

				/*if (nCount_x == 0 || nCount_x == m_num.x)
				{
				pVtx[0].pos.y = 200.0f;
				}*/

				/*if ((int)(m_num.x / 2) < nCount_x)
				{
				pVtx[0].pos.y = 100.0f * (nCount_x - (m_num.x / 2));
				}*/

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (m_num.x)) * (nCount_x % ((int)m_num.x + 1)), (TexUV.y / (m_num.z)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)m_num.x + 1)), TexUV.y * (nCount_yz));
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
					//D3DXVECTOR3(-((size.x / 2) * m_num.x) + (size.x * nCount_x), 0.0f,((size.y / 2) * m_num.y) - (size.y * (nCount_yz)));
					D3DXVECTOR3(-size.x + ((size.x * 2) / m_num.x) * nCount_x, ((size.y * m_num.y) - (size.y * nCount_yz)), 0.0f);

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (m_num.x)) * (nCount_x % ((int)m_num.x + 1)), (TexUV.y / (m_num.z)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)m_num.x + 1)), TexUV.y * (nCount_yz));
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
					sinf(D3DX_PI - ((D3DX_PI * (1.0f / (m_num.x / 2))) * nCount_x) * size.z) * size.x,
					((size.y) * m_num.y) - (size.y * (nCount_yz)),
					cosf(D3DX_PI - ((D3DX_PI * (1.0f / (m_num.x / 2))) * nCount_x) * size.z) * size.x);

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (m_num.x)) * (nCount_x % ((int)m_num.x + 1)), (TexUV.y / (m_num.y)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)m_num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = tex;

				// �@���̐ݒ�
				vecNor.x = pos.x - (pVtx[0].pos.x / pVtx[0].pos.x);
				vecNor.z = pos.z - (pVtx[0].pos.z / pVtx[0].pos.z);
				D3DXVec3Normalize(&vecNor, &vecNor);					// ���K������
				pVtx[0].nor = D3DXVECTOR3(vecNor.x, 1.0f, vecNor.z);
				//pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				break;
			case MESHTYPE_DOME:			// �h�[��
				// ���_���̐ݒ�
				float fPercent = (((size.y) * m_num.y) - (size.y * (nCount_yz))) / ((size.y) * m_num.y);

				pVtx[0].pos = D3DXVECTOR3(
					sinf(D3DX_PI - ((D3DX_PI * (1.0f / (m_num.x / 2))) * nCount_x) * size.z) * (cosf(((D3DX_PI * 0.5f) * (fPercent))) * size.x),
					((size.y / m_num.y)) - ((size.y / m_num.y) * (nCount_yz)),
					//0.0f,
					cosf(D3DX_PI - ((D3DX_PI * (1.0f / (m_num.x / 2))) * nCount_x) * size.z) * (cosf(((D3DX_PI * 0.5f) * (fPercent))) * size.x));

				float fRangeMax = sqrtf(powf(size.x, 2.0f) + powf(size.x, 2.0f));
				float fRange = sqrtf(powf(pVtx[0].pos.x, 2.0f) + powf(pVtx[0].pos.z, 2.0f));
				pVtx[0].pos.y = cosf(((D3DX_PI * 0.5f) * (nCount_yz / m_num.y))) * size.y;

				switch (m_meshTex)
				{// �e�N�X�`���^�C�v
				case MESHTEX_ALL:		// �S��
					tex = D3DXVECTOR2((TexUV.x / (m_num.x)) * (nCount_x % ((int)m_num.x + 1)), (TexUV.y / (m_num.y)) * (nCount_yz));
					break;
				case MESHTEX_NUMSIZE:	// ����
					tex = D3DXVECTOR2(TexUV.x * (nCount_x % ((int)m_num.x + 1)), TexUV.y * (nCount_yz));
					break;
				}
				// �e�N�X�`���̐ݒ�
				pVtx[0].tex = tex;

				// �@���̐ݒ�				
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
}

//=============================================================================
// UV�ړ�����								(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetTex(D3DXVECTOR2 TexMoveUV)
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
	case MESHTYPE_DOME:				// �h�[��
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

//=============================================================================
// ���_�ݒ菈��								(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetVtx(int nNum, D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[nNum].pos = pos;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�擾����									(public)	*** CSceneMesh ***
//=============================================================================
D3DXVECTOR3 CSceneMesh::GetVtx(int nNum)
{
	D3DXVECTOR3 pos;	// �ʒu

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pos = pVtx[nNum].pos;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return pos;		// �l��Ԃ�
}

//=============================================================================
// �@���擾����									(public)	*** CSceneMesh ***
//=============================================================================
D3DXVECTOR3 CSceneMesh::GetNor(int nNum)
{
	D3DXVECTOR3 nor;	// �@��

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �@����ݒ�
	nor = pVtx[nNum].nor;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return nor;		// �l��Ԃ�
}

//=============================================================================
// ���_�F�ݒ菈��								(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetCol(int nNum, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[nNum].col = col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���b�V�����ɂ��邩�`�F�b�N����					(public)	*** CSceneMesh ***
//=============================================================================
bool CSceneMesh::ColInMesh(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	bool bLand = false;
	D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
	D3DXVECTOR3 posmtx = pos - m_pos;
	
	// �x�N�g���v�Z
	vecA = pVtx[(int)(m_num.x)].pos - pVtx[(int)(0)].pos;
	vecC = posmtx - pVtx[(int)(0)].pos;

	if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
	{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
	 // �x�N�g���v�Z
		vecA = pVtx[(int)(m_nMaxVtx - 1)].pos - pVtx[(int)(m_num.x)].pos;
		vecC = posmtx - pVtx[(int)(m_num.x)].pos;

		if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
		{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
		 // �x�N�g���v�Z
			vecA = pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos - pVtx[(int)(m_nMaxVtx - 1)].pos;
			vecC = posmtx - pVtx[(int)(m_nMaxVtx - 1)].pos;

			if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
			 // �x�N�g���v�Z
				vecA = pVtx[(int)(0)].pos - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;
				vecC = posmtx - pVtx[(int)((m_nMaxVtx - 1) - (m_num.x))].pos;

				if (0 < ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
				{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
					bLand = true;
				}
			}
		}
	}

	/*if (m_pos.x - m_size.x <= pos.x && pos.x <= m_pos.x + m_size.x &&
		m_pos.z - m_size.z <= pos.z && pos.z <= m_pos.z + m_size.z)
	{
		bLand = true;
	}*/

	return bLand;		// �l��Ԃ�
}

//=============================================================================
// �T�C���X�V����									(public)	*** CSceneMesh ***
//=============================================================================
float CSceneMesh::GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];					// �g�p���_�̔ԍ���ۑ�
	D3DXVECTOR3 Hitnor[2];				// �g�p���_�̖@����ۑ�
	bool bRange_Block = false;			// �u���b�N�͈͓���
	float fUpDown = 0.0f;				// ��

	// ���݂̃u���b�N���m�F
	int nHitBlock =
		(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
		(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;			// �e���C���`�F�b�N

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// �u���b�N���̂S���_���J�E���g
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nData = 0, nChack1 = 0, nChack2 = 0;	// �l�����p�i���̃J�E���g�A�덷�����p�j
			int nVtxNum_N = 0, nVtxNum = 0;				// �g�p���钸�_�ԍ��i���̒��_�ԍ��A���݂̒��_�ԍ��j

			nData = (nCntVtx + 1) % 4;					// ���̃J�E���g������

			// �J�E���g���̌덷���Ȃ����i2*2 = 0134; for = 0143 �� 0134�j
			if (nCntVtx == 2) { nChack1 = 1; }
			else if (nCntVtx == 3) { nChack1 = -1; }
			if (nData == 2) { nChack2 = 1; }
			else if (nData == 3) { nChack2 = -1; }

			// ���_�ԍ����v�Z
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// ���_�ԍ���ۑ�
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// �x�N�g���v�Z
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
				nCntLand_Block++;						// �`�F�b�N�����J�E���g

				if (nCntLand_Block == 4)
				{// �`�F�b�N�����S�ɂȂ�����
					bRange_Block = true;				// �u���b�N���ɂ��邱�Ƃ��m��

					D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// �@���v�Z�p�i���K���p�A�x�N�g���j

					// ����
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
					// �E��
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
					// ���όv�Z
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
					// ���K��
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// �@���̐ݒ�
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{// �u���b�N���ɂ�����
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nVtxNum = 0;							// �g�p���钸�_�ԍ�

			// �x�N�g���v�Z
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
				nVtxNum = 2;
			}
			else
			{// �͈͓��i�����j�ɂ��邱�Ƃ��m�F
				nVtxNum = 1;
			}

			// �x�N�g���v�Z
			D3DXVECTOR3 vecP = D3DXVECTOR3(pos.x + sinf(rot.y), pos.y, pos.z + cosf(rot.y)) - pos;

			fUpDown = ((pVtx[nCntVtxNum[nVtxNum]].nor.x * vecP.x) + (pVtx[nCntVtxNum[nVtxNum]].nor.y * vecP.y) + (pVtx[nCntVtxNum[nVtxNum]].nor.z * vecP.z)) /
				(sqrtf((float)(pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.x, 2) + pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.y, 2) + pow((double)pVtx[nCntVtxNum[nVtxNum]].nor.z, 2))) *
					sqrtf((float)(pow((double)vecP.x, 2) + pow((double)vecP.y, 2) + pow(vecP.z, 2))));
		}
	}
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fUpDown;		// �l��Ԃ�
}

//=============================================================================
// �u���b�N�@���擾����							(public)	*** CSceneMesh ***
//=============================================================================
float CSceneMesh::GetBlockNor(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];					// �g�p���_�̔ԍ���ۑ�
	bool bRange_Block = false;			// �u���b�N�͈͓���
	float fNor = 0.0f;					// ��

	// ���݂̃u���b�N���m�F
	int nHitBlock =
		(int)((pos.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
		(int)((int)(((pos.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;			// �e���C���`�F�b�N

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// �u���b�N���̂S���_���J�E���g
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nData = 0, nChack1 = 0, nChack2 = 0;	// �l�����p�i���̃J�E���g�A�덷�����p�j
			int nVtxNum_N = 0, nVtxNum = 0;				// �g�p���钸�_�ԍ��i���̒��_�ԍ��A���݂̒��_�ԍ��j

			nData = (nCntVtx + 1) % 4;					// ���̃J�E���g������

														// �J�E���g���̌덷���Ȃ����i2*2 = 0134; for = 0143 �� 0134�j
			if (nCntVtx == 2) { nChack1 = 1; }
			else if (nCntVtx == 3) { nChack1 = -1; }
			if (nData == 2) { nChack2 = 1; }
			else if (nData == 3) { nChack2 = -1; }

			// ���_�ԍ����v�Z
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// ���_�ԍ���ۑ�
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// �x�N�g���v�Z
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = pos - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
				nCntLand_Block++;						// �`�F�b�N�����J�E���g

				if (nCntLand_Block == 4)
				{// �`�F�b�N�����S�ɂȂ�����
					bRange_Block = true;				// �u���b�N���ɂ��邱�Ƃ��m��

					break;
				}
			}
		}

		if (bRange_Block == true)
		{// �u���b�N���ɂ�����
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nVtxNum = 0;							// �g�p���钸�_�ԍ�

			// �x�N�g���v�Z
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = pos - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
				nVtxNum = 2;
			}
			else
			{// �͈͓��i�����j�ɂ��邱�Ƃ��m�F
				nVtxNum = 1;
			}

			fNor = pVtx[nCntVtxNum[nVtxNum]].nor.y;
		}
	}
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fNor;		// �l��Ԃ�
}

//=============================================================================
// �����ݒ菈��									(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCount_yz = 0; nCount_yz < m_num.z + 1; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// x���J�E���g
			float fLenght = sqrtf((float)(pow((double)(pVtx[0].pos.x - pos.x), 2) + pow((double)(pVtx[0].pos.z - pos.z), 2)));

			if (fLenght < fRange)
			{
				float fPercent = fLenght / fRange;

				//pVtx[0].pos.y += sinf(((D3DX_PI * 0.5f) * (1.0f - fPercent))) * fValue;
				pVtx[0].pos.y += cosf(((D3DX_PI * 0.5f) * (fPercent))) * fValue;

			}
			pVtx += 1;
		}
	}
}

//=============================================================================
// �����擾����									(public)	*** CSceneMesh ***
//=============================================================================
float CSceneMesh::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtxNum[4];					// �g�p���_�̔ԍ���ۑ�
	D3DXVECTOR3 Hitnor[2];				// �g�p���_�̖@����ۑ�
	bool bRange_Block = false;			// �u���b�N�͈͓���
	float fHeight = -500.0f;			// ����

	D3DXVECTOR3 posmtx = D3DXVECTOR3((float)((int)pos.x), (float)((int)pos.y), (float)((int)pos.z)) - m_pos;

	// ���݂̃u���b�N���m�F
	int nHitBlock =
		(int)((posmtx.x + m_size.x) / ((m_size.x * 2.0f) / m_num.x)) +
		(int)((int)(((posmtx.z - m_size.z) / ((m_size.z * 2.0f) / m_num.z)) * -1) * m_num.x);

	int nCntLand_Block = 0;			// �e���C���`�F�b�N

	if (0 <= nHitBlock && nHitBlock < (m_nMaxPolygon + (((int)m_num.z - 1) * 4)) / 2)
	{
		for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
		{// �u���b�N���̂S���_���J�E���g
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nData = 0, nChack1 = 0, nChack2 = 0;	// �l�����p�i���̃J�E���g�A�덷�����p�j
			int nVtxNum_N = 0, nVtxNum = 0;				// �g�p���钸�_�ԍ��i���̒��_�ԍ��A���݂̒��_�ԍ��j

			nData = (nCntVtx + 1) % 4;					// ���̃J�E���g������

			// �J�E���g���̌덷���Ȃ����i2*2 = 0134; for = 0143 �� 0134�j
			if (nCntVtx == 2) { nChack1 = 1; }
			else if (nCntVtx == 3) { nChack1 = -1; }
			if (nData == 2) { nChack2 = 1; }
			else if (nData == 3) { nChack2 = -1; }

			// ���_�ԍ����v�Z
			nVtxNum_N = (nData % 2) + (((int)m_num.x + 1) * (nData / 2)) + nChack2 + (nHitBlock + (nHitBlock / ((int)m_num.x)));
			nVtxNum = (nCntVtx % 2) + (((int)m_num.x + 1) * (nCntVtx / 2)) + nChack1 + (nHitBlock + (nHitBlock / ((int)m_num.x)));

			// ���_�ԍ���ۑ�
			nCntVtxNum[nCntVtx] = nVtxNum - nChack1;

			// �x�N�g���v�Z
			vecA = pVtx[nVtxNum_N].pos - pVtx[nVtxNum].pos;
			vecC = posmtx - pVtx[nVtxNum].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
				nCntLand_Block++;						// �`�F�b�N�����J�E���g

				if (nCntLand_Block == 4)
				{// �`�F�b�N�����S�ɂȂ�����
					bRange_Block = true;				// �u���b�N���ɂ��邱�Ƃ��m��

					D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// �@���v�Z�p�i���K���p�A�x�N�g���j

					// ����
					vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
					vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
					// �E��
					vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
					vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
					// ���όv�Z
					D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
					D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
					// ���K��
					D3DXVec3Normalize(&nor[0], &nor[0]);
					D3DXVec3Normalize(&nor[1], &nor[1]);

					// �@���̐ݒ�
					Hitnor[0] = nor[0];
					Hitnor[1] = nor[1];

					break;
				}
			}
		}

		if (bRange_Block == true)
		{// �u���b�N���ɂ�����
			D3DXVECTOR3 vecA, vecC;						// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nVtxNum = 0;							// �g�p���钸�_�ԍ�

			// �x�N�g���v�Z
			vecA = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[0]].pos;
			vecC = posmtx - pVtx[nCntVtxNum[0]].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
				nVtxNum = 2;
			}
			else
			{// �͈͓��i�����j�ɂ��邱�Ƃ��m�F
				nVtxNum = 1;
			}

			// �x�N�g���v�Z
			D3DXVECTOR3 vecP = posmtx - pVtx[nCntVtxNum[nVtxNum]].pos;
			vecP.y -= pVtx[nCntVtxNum[nVtxNum]].pos.y;

			// �����v�Z
			//if ((((Hitnor[nVtxNum / 2 ^ 1].x * vecP.x) + (Hitnor[nVtxNum / 2 ^ 1].z * vecP.z)) / -Hitnor[nVtxNum / 2 ^ 1].y) + pVtx[nCntVtxNum[nVtxNum]].pos.y <= posmtx.y)
			{
				fHeight = (((Hitnor[nVtxNum / 2 ^ 1].x * vecP.x) + (Hitnor[nVtxNum / 2 ^ 1].z * vecP.z)) / -Hitnor[nVtxNum / 2 ^ 1].y) + pVtx[nCntVtxNum[nVtxNum]].pos.y;
				fHeight += m_pos.y;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fHeight;		// �l��Ԃ�
}

//=============================================================================
// �@���v�Z����									(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SetVecNor(void)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	typedef struct
	{// �e�u���b�N�̖@���v�Z�p
		D3DXVECTOR3 nor[4];
	}NORDATA;

	// �u���b�N�����������m��
	NORDATA *norData = NULL;
	norData = new NORDATA[(int)(m_num.x * m_num.z)];

	if (norData != NULL)
	{// NULL�ȊO��������
		for (int nCntBlock = 0; nCntBlock < (m_num.x * m_num.z); nCntBlock++)
		{// �u���b�N���J�E���g
			int nCntVtxNum[4];		// ���_�̔ԍ��v�Z�p

			// �u���b�N�̒��_�𒲂ׂ�
			nCntVtxNum[0] = (0 % 2) + (((int)m_num.x + 1) * (0 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[1] = (1 % 2) + (((int)m_num.x + 1) * (1 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[2] = (2 % 2) + (((int)m_num.x + 1) * (2 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));
			nCntVtxNum[3] = (3 % 2) + (((int)m_num.x + 1) * (3 / 2)) + (nCntBlock + (nCntBlock / ((int)m_num.x)));

			D3DXVECTOR3 nor[2], vec0[2], vec1[2];			// �@���v�Z�p�i���K���p�A�x�N�g���j

			// ����
			vec0[0] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[2]].pos;
			vec0[1] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[2]].pos;
			// �E��
			vec1[0] = pVtx[nCntVtxNum[3]].pos - pVtx[nCntVtxNum[1]].pos;
			vec1[1] = pVtx[nCntVtxNum[0]].pos - pVtx[nCntVtxNum[1]].pos;
			// ���όv�Z
			D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
			D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
			// ���K��
			D3DXVec3Normalize(&nor[0], &nor[0]);
			D3DXVec3Normalize(&nor[1], &nor[1]);

			// �@���̕ۑ�
			norData[nCntBlock].nor[0] = (nor[0] + nor[1]) * 0.5f;
			norData[nCntBlock].nor[1] = nor[1];
			norData[nCntBlock].nor[2] = nor[0];
			norData[nCntBlock].nor[3] = (nor[0] + nor[1]) * 0.5f;
		}

		for (int nCntVtx = 0; nCntVtx < m_nMaxVtx; nCntVtx++)
		{// ���_���J�E���g
			D3DXVECTOR3 nor;	// �@���v�Z����
			int SetBlock[4];	// �u���b�N�ԍ�

								// ���_�ɐڂ���u���b�N�𒲂ׂ�
			SetBlock[0] = (nCntVtx - (int)(m_num.x + 1)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[1] = (nCntVtx - (int)(m_num.x)) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[2] = (nCntVtx - 1) - (nCntVtx / (int)(m_num.x + 1));
			SetBlock[3] = (nCntVtx)-(nCntVtx / (int)(m_num.x + 1));

			if (nCntVtx != 0 && nCntVtx != 0 + m_num.x && nCntVtx != (m_nMaxVtx - 1) - m_num.x && nCntVtx != (m_nMaxVtx - 1))
			{// �p�ȊO
				if (nCntVtx <= m_num.x)
				{// �㉡���C��
					nor = (norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[2] + norData[SetBlock[3]].nor[1]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == 0)
				{// ���c���C��
					nor = (norData[SetBlock[1]].nor[2] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 3;
				}
				else if (nCntVtx % (int)(m_num.x + 1) == (int)(m_num.x))
				{// �E�c���C��
					nor = (norData[SetBlock[0]].nor[1] + norData[SetBlock[0]].nor[2] + norData[SetBlock[2]].nor[1]) / 3;
				}
				else if ((m_nMaxVtx - 1) - m_num.x <= nCntVtx)
				{// �������C��
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2]) / 3;
				}
				else
				{// ���̑�����
					nor = (norData[SetBlock[0]].nor[2] + norData[SetBlock[0]].nor[1] + norData[SetBlock[1]].nor[2] +
						norData[SetBlock[2]].nor[1] + norData[SetBlock[3]].nor[1] + norData[SetBlock[3]].nor[2]) / 6;
				}
			}
			else
			{// �p
				if (nCntVtx == 0)
				{// ����
					nor = norData[SetBlock[3]].nor[0];
				}
				else if (nCntVtx == 0 + m_num.x)
				{// �E��
					nor = norData[SetBlock[2]].nor[1];
				}
				else if (nCntVtx == (m_nMaxVtx - 1) - m_num.x)
				{// ����
					nor = norData[SetBlock[1]].nor[2];
				}
				else if (nCntVtx == (m_nMaxVtx - 1))
				{// �E��
					nor = norData[SetBlock[0]].nor[3];
				}
			}

			// �@���̐ݒ�
			pVtx[nCntVtx].nor = nor;
		}

		if (norData != NULL)
		{// NULL�ȊO�̏ꍇ
			delete[] norData;	// �j��
			norData = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �T�C���X�V����									(public)	*** CSceneMesh ***
//=============================================================================
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine)
{
	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);			// �t�H���g�擾

	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:
		posStart = pos;
		break;
	case SINSTART_CENTER:
		posStart = m_size;
		break;
	case SINSTART_LU:
		posStart = pVtx[0].pos;
		break;
	case SINSTART_RU:
		posStart = pVtx[(int)(m_num.x + 1)].pos;
		break;
	case SINSTART_LD:
		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;
		break;
	case SINSTART_RD:
		posStart = pVtx[(m_nMaxVtx - 1)].pos;
		break;
	}

	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:
		fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2)));
		break;
	case SINTYPE_RIGHT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_LEFT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_UP:
		fMax = m_size.z * 2.0f;
		break;
	case SINTYPE_DOWN:
		fMax = m_size.z * 2.0f;
		break;
	}

	// ���_���̐ݒ�
	for (int nCount_yz = 0; nCount_yz < m_num.z + 1; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// x���J�E���g
			float fnow = 0.0f;

			switch (sinType)
			{
			case SINTYPE_CIRCLE:
				fnow = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2)));
				break;
			case SINTYPE_RIGHT:
				fnow = pVtx[0].pos.x - posStart.x;
				frot *= -1;
				break;
			case SINTYPE_LEFT:
				fnow = pVtx[0].pos.x - posStart.x;
				break;
			case SINTYPE_UP:
				fnow = pVtx[0].pos.z - posStart.z;
				break;
			case SINTYPE_DOWN:
				fnow = pVtx[0].pos.z - posStart.z;
				break;
			}

			float fPP = fnow / fMax;

			pVtx[0].pos.y = sinf(((D3DX_PI) * (fPP * fLine)) + frot) * (fUp/* * (1.2f - fPP)*/);

			pVtx += 1;
		}
	}

	// �@�����v�Z
	SetVecNor();
}
void CSceneMesh::SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange)
{
	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);		// �t�H���g�擾

	VERTEX_3D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posStart;

	switch (sinStart)
	{
	case SINSTART_POS:
		posStart = pos - m_pos;
		break;
	case SINSTART_CENTER:
		posStart = m_size;
		break;
	case SINSTART_LU:
		posStart = pVtx[0].pos;
		break;
	case SINSTART_RU:
		posStart = pVtx[(int)(m_num.x + 1)].pos;
		break;
	case SINSTART_LD:
		posStart = pVtx[(m_nMaxVtx - 1) - (int)(m_num.x + 1)].pos;
		break;
	case SINSTART_RD:
		posStart = pVtx[(m_nMaxVtx - 1)].pos;
		break;
	}

	float fMax;

	switch (sinType)
	{
	case SINTYPE_CIRCLE:
		fMax = sqrtf((float)(pow((double)m_size.x, 2) + pow((double)m_size.z, 2)));
		break;
	case SINTYPE_RIGHT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_LEFT:
		fMax = m_size.x * 2.0f;
		break;
	case SINTYPE_UP:
		fMax = m_size.z * 2.0f;
		break;
	case SINTYPE_DOWN:
		fMax = m_size.z * 2.0f;
		break;
	}

	// ���_���̐ݒ�
	for (int nCount_yz = 0; nCount_yz < m_num.z + 1; nCount_yz++)
	{// yz���J�E���g
		for (int nCount_x = 0; nCount_x < m_num.x + 1; nCount_x++)
		{// x���J�E���g
			float fLenght = sqrtf((float)(pow((double)(pVtx[0].pos.x - posStart.x), 2) + pow((double)(pVtx[0].pos.z - posStart.z), 2)));

			if (fLenght < fRange)
			{
				float fPercent = fLenght / fRange;
				float fnow = 0.0f;

				pVtx[0].pos.y = sinf(((D3DX_PI) * (fPercent * fLine)) + frot) * (fUp * (1.0f - fPercent));
			}

			pVtx += 1;
		}
	}

	// �@�����v�Z
	SetVecNor();
}