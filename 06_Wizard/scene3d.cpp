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
// �R���X�g���N�^									(public)	*** CScene3D ***
//=============================================================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture		= NULL;									// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff		= NULL;									// ���_�o�b�t�@���ւ̃|�C���^

	m_bVtxSize		= false;								// ���_�ʒu�A�傫������

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_TexUV			= D3DXVECTOR2(0.0f, 0.0f);				// UV

	m_bDraw			= true;									// �`��

	m_bDrawAdd		= false;								// ���Z����
	m_bDrawSubtract = false;								// ���Y����
	m_bDrawATest	= false;								// ���e�X�g
	m_bDrawZWrite	= false;								// Z���C�g
	m_bDrawLight	= false;								// ���C�g�e��
	m_nATest		= 1;									// ���e�X�g���l
	m_nCullMode		= D3DCULL_CCW;							// �J�����O
	m_dCmpValue		= D3DCMP_LESSEQUAL;						// Z���C�g�ݒ�

	m_scene3dType	= SCENE3DTYPE_NORMAL;					// ���
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
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

	/*D3DXVECTOR3 nor[2], vec0[2], vec1[2];		// �@���v�Z�p�i���K���p�A�x�N�g���j

	// ����
	vec0[0] = pVtx[1].pos - pVtx[0].pos;
	vec0[1] = pVtx[2].pos - pVtx[0].pos;
	// �E��
	vec1[0] = pVtx[2].pos - pVtx[3].pos;
	vec1[1] = pVtx[1].pos - pVtx[3].pos;
	// ���όv�Z
	D3DXVec3Cross(&nor[0], &vec0[0], &vec0[1]);
	D3DXVec3Cross(&nor[1], &vec1[0], &vec1[1]);
	// ���K��
	D3DXVec3Normalize(&nor[0], &nor[0]);
	D3DXVec3Normalize(&nor[1], &nor[1]);

	// �@���̐ݒ�
	pVtx[0].nor = nor[0];
	pVtx[1].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[2].nor = (nor[0] + nor[1]) * 0.5f;
	pVtx[3].nor = nor[1];*/
	if (m_size.z != 0.0f)
	{// �n��
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	if (m_size.y != 0.0f)
	{// ��
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
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

	return S_OK;	// �l��Ԃ�
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

	CScene::SetDeath();		// ���S�t���O
}

//=============================================================================
// �X�V����										(public)	*** CScene3D ***
//=============================================================================
void CScene3D::Update(void)
{
	// ��]����
	if (m_rot.x < -D3DX_PI)		m_rot.x += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot.x)		m_rot.x -= D3DX_PI * 2.0f;
	if (m_rot.y < -D3DX_PI)		m_rot.y += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot.y)		m_rot.y -= D3DX_PI * 2.0f;
	if (m_rot.z < -D3DX_PI)		m_rot.z += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot.z)		m_rot.z -= D3DX_PI * 2.0f;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bVtxSize == false)
	{// ���_�ύX���Ă��Ȃ�
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);
	}
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
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX			mtxRot, mtxTrans, mtxView;							// �v�Z�p�}�g���b�N�X

	if (m_bDraw == true)
	{
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

		// �r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		if (m_scene3dType == SCENE3DTYPE_NORMAL)
		{
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
		}
		else if (m_scene3dType == SCENE3DTYPE_BILLBOARD)
		{//	�r���{�[�h
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

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

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
}

//=============================================================================
// ���_�ʒu�A�傫����������						(public)	*** CScene3D ***
//=============================================================================
void CScene3D::SetVtxSize(D3DXVECTOR3 *VtxSize)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = VtxSize[0];
	pVtx[1].pos = VtxSize[1];
	pVtx[2].pos = VtxSize[2];
	pVtx[3].pos = VtxSize[3];

	if (m_bVtxSize == false)
		m_bVtxSize = true;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�ʒu�A�傫���擾����						(public)	*** CScene3D ***
//=============================================================================
void CScene3D::GetVtxSize(D3DXVECTOR3 *VtxSize)
{
	D3DXVECTOR3 pos[4];

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	VtxSize[0] = pVtx[0].pos;
	VtxSize[1] = pVtx[1].pos;
	VtxSize[2] = pVtx[2].pos;
	VtxSize[3] = pVtx[3].pos;

	if (m_bVtxSize == false)
		m_bVtxSize = true;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �A�j���[�V��������							(public)	*** CScene3D ***
//=============================================================================
void CScene3D::SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV)
{
	if (nAnimLineNum <= 0) nAnimLineNum = 1;	// �ԍ�����

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

//=============================================================================
// ���_�擾����								(public)	*** CScene3D ***
//=============================================================================
D3DXVECTOR3 CScene3D::GetVtx(int nNum)
{
	D3DXVECTOR3 pos;	// �ʒu

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pos = pVtx[nNum].pos;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return pos;
}

//=============================================================================
// �@���擾����								(public)	*** CScene3D ***
//=============================================================================
D3DXVECTOR3 CScene3D::GetNor(int nNum)
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
// �����擾����								(public)	*** CScene3D ***
//=============================================================================
float CScene3D::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fHeight = 0.0f;						// ����

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_DATA);

	for (int nCntPolygon = 0; nCntPolygon < 2; nCntPolygon++)
	{// �|���S�����J�E���g
		bool bRange = false;					// �|���S���͈͓���
		int nCntLand = 0;						// �e���C���`�F�b�N

		for (int nCntVtx = 0; nCntVtx < 3; nCntVtx++)
		{// �|���S�����̂R���_���J�E���g
			D3DXVECTOR3 vecA, vecC;				// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
			int nChack1 = 0, nChack2 = 0;		// �l�����p�i�덷�����p�j

			// �J�E���g���̌덷���Ȃ����i1*1 = 013; for = 031 �� 013�j
			if (nCntVtx + 1 == 2) { nChack1 = 1; }
			else if (nCntVtx + 1 == 3) { nChack1 = -1; }
			if (nCntVtx + 2 == 2) { nChack2 = 1; }
			else if (nCntVtx + 2 == 3) { nChack2 = -1; }

			// �x�N�g���v�Z
			vecA = pVtx[((nCntVtx + 1) % 3 + nCntPolygon) + (nChack2 * (nCntPolygon % 2))].pos - pVtx[((nCntVtx) % 3 + nCntPolygon) + (nChack1 * (nCntPolygon % 2))].pos;
			vecC = pos - pVtx[((nCntVtx) % 3 + nCntPolygon) + (nChack1 * (nCntPolygon % 2))].pos;

			if (0 <= ((vecA.z * vecC.x) - (vecA.x * vecC.z)))
			{// �͈͓��i�E���j�ɂ��邱�Ƃ��m�F
				nCntLand++;						// �`�F�b�N�����J�E���g

				if (nCntLand == 3)
				{// �`�F�b�N�����R�ɂȂ�����
					bRange = true;				// �|���S�����ɂ��邱�Ƃ��m��

					int nVtxNum = 0;			// �g�p���_�̔ԍ�

					if (nCntPolygon % 2 == 0)
					{// ����
						nVtxNum = ((0) % 3 + nCntPolygon);
					}
					else
					{// �E��
						nVtxNum = ((2) % 3 + nCntPolygon);
					}

					// �x�N�g���v�Z
					D3DXVECTOR3 vecP = pos - pVtx[nVtxNum].pos;
					vecP.y -= pVtx[nVtxNum].pos.y;

					//pFont->SetMessage(CFont::SETTYPE_ADD, "nVtxNum[%d]\n\n", nVtxNum);

					// �����v�Z
					fHeight = (((pVtx[nVtxNum].nor.x * vecP.x) + (pVtx[nVtxNum].nor.z * vecP.z)) / -pVtx[nVtxNum].nor.y) + pVtx[nVtxNum].pos.y;

					//pFont->SetMessage(CFont::SETTYPE_ADD, "Polygon3D : ����[%d] [%d]\n\n", nCntPolygon, bRange);
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return fHeight;		// �l��Ԃ�
}

//=============================================================================
// �r���{�[�h��]����								(public)	*** CScene3D ***
//=============================================================================
void CScene3D::SetBillRot(float rot)
{
	// ��]����
	if (rot < -D3DX_PI)	rot += D3DX_PI * 2.0f;
	if (D3DX_PI < rot)	rot -= D3DX_PI * 2.0f;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(rot) * m_size.y - cosf(rot) * m_size.x,	 m_pos.y + sinf(rot) * m_size.x + cosf(rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(rot) * m_size.y + cosf(rot) * m_size.x,	 m_pos.y - sinf(rot) * m_size.x + cosf(rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(rot) * m_size.y - cosf(rot) * m_size.x,	 m_pos.y + sinf(rot) * m_size.x - cosf(rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(rot) * m_size.y + cosf(rot) * m_size.x,	 m_pos.y - sinf(rot) * m_size.x - cosf(rot) * m_size.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}