//=============================================================================
//
// �V�[��2D���� [scene2d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"		// �V�[��2D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����

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
// �R���X�g���N�^								(public)	*** CScene2D ***
//=============================================================================
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture		= NULL;									// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff		= NULL;									// �}�e���A�����ւ̃|�C���^

	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= 0.0f;									// ����
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_TexUV			= D3DXVECTOR2(0.0f, 0.0f);				// UV

	m_bDrawAdd		= false;								// ���Z����
	m_bDrawSubtract = false;								// ���Y����
	m_bDrawATest	= false;								// ���e�X�g
	m_nATest		= 1;									// ���e�X�g���l
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CScene2D ***
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������									(public)	*** CScene2D ***
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
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
// �I������										(public)	*** CScene2D ***
//=============================================================================
void CScene2D::Uninit(void)
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
// �X�V����										(public)	*** CScene2D ***
//=============================================================================
void CScene2D::Update(void)
{
	// ��]����
	if (m_rot < -D3DX_PI)	m_rot += D3DX_PI * 2.0f;
	if (D3DX_PI < m_rot)	m_rot -= D3DX_PI * 2.0f;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x - cosf(m_rot) * m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y - cosf(m_rot) * m_size.x, m_pos.y + sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot) * m_size.y + cosf(m_rot) * m_size.x, m_pos.y - sinf(m_rot) * m_size.x + cosf(m_rot) * m_size.y, 0.0f);
	// ���_�J���[�ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��										(public)	*** CScene2D ***
//=============================================================================
void CScene2D::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

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

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
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
}

//=============================================================================
// �A�j���[�V��������							(public)	*** CScene2D ***
//=============================================================================
void CScene2D::SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV)
{
	if (nAnimLineNum <= 0) nAnimLineNum = 1;	// �ԍ�����

	m_TexUV = TexUV;

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

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

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^								(public)	*** CScene2D_Preset ***
//=============================================================================
CScene2D_Preset::CScene2D_Preset(int nPriority, CScene::OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �����l�ݒ�
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// ��ރJ�E���g
		m_nType[nCntType] = TYPE_NONE;			// ���
		m_nCntFrame[nCntType] = 0;					// �t���[���J�E���g
		m_nMaxFrame[nCntType] = 0;					// �t���[���̍ő�l
		m_colDest_1[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �ڕW�̐F1
		m_colDest_2[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �ڕW�̐F2
		m_sizeDest[nCntType]  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		// �ڕW�̑傫��
	}
}

//=============================================================================
// �f�X�g���N�^								(public)	*** CScene2D_Preset ***
//=============================================================================
CScene2D_Preset::~CScene2D_Preset()
{

}

//=============================================================================
// ����������								(public)	*** CScene2D_Preset ***
//=============================================================================
HRESULT CScene2D_Preset::Init(void)
{
	CScene2D::Init();		// ����������

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������									(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Uninit(void)
{
	CScene2D::Uninit();		// �I������
}

//=============================================================================
// �X�V����									(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Update(void)
{
	Preset();				// �v���Z�b�g

	CScene2D::Update();		// �X�V����
}

//=============================================================================
// �`�揈��									(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Draw(void)
{
	CScene2D::Draw();		// �`�揈��
}

//=============================================================================
// �v���Z�b�g����								(public)	*** CScene2D_Preset ***
//=============================================================================
void CScene2D_Preset::Preset(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// ��ރJ�E���g
		switch (m_nType[nCntType])
		{// ���
		case TYPE_NONE:		// �������Ă��Ȃ�
		{
			// ������
			m_nCntFrame[nCntType] = 0;	// �t���[��������
			m_nMaxFrame[nCntType] = 0;	// �t���[���ő�l������
			break;
		}
		case TYPE_COL:		// �F�ύX
		{
			D3DXCOLOR col = CScene2D::Getcol();			// �F�擾

			if (m_nCntFrame[nCntType] == 0)
			{
				m_colDest_1[nCntType].r = (m_colDest_1[nCntType].r - col.r);
				m_colDest_1[nCntType].g = (m_colDest_1[nCntType].g - col.g);
				m_colDest_1[nCntType].b = (m_colDest_1[nCntType].b - col.b);
				m_colDest_1[nCntType].a = (m_colDest_1[nCntType].a - col.a);
			}

			m_nCntFrame[nCntType]++;		// �t���[���J�E���g

			col.r += (m_colDest_1[nCntType].r) / (float)m_nMaxFrame[nCntType];
			col.g += (m_colDest_1[nCntType].g) / (float)m_nMaxFrame[nCntType];
			col.b += (m_colDest_1[nCntType].b) / (float)m_nMaxFrame[nCntType];
			col.a += (m_colDest_1[nCntType].a) / (float)m_nMaxFrame[nCntType];

			if (m_nCntFrame[nCntType] == m_nMaxFrame[nCntType])
			{// �ő�t���[���ɂȂ�����
				m_nCntFrame[nCntType] = 0;
				m_nMaxFrame[nCntType] = 0;
				m_colDest_1[nCntType] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				m_nType[nCntType] = TYPE_NONE;
			}

			CScene2D::Setcol(col);						// �F�ݒ�

			break;
		}
		case TYPE_SIZE:		// �傫���ύX
		{
			D3DXVECTOR3 size = CScene2D::Getsize();		// �T�C�Y�擾

			if (m_nCntFrame[nCntType] == 0)
			{
				m_sizeDest[nCntType] = (m_sizeDest[nCntType] - size);
			}

			m_nCntFrame[nCntType]++;		// �t���[���J�E���g

			size.x += (m_sizeDest[nCntType].x) / (float)m_nMaxFrame[nCntType];
			size.y += (m_sizeDest[nCntType].y) / (float)m_nMaxFrame[nCntType];
			size.z += (m_sizeDest[nCntType].z) / (float)m_nMaxFrame[nCntType];

			if (m_nCntFrame[nCntType] == m_nMaxFrame[nCntType])
			{// �ő�t���[���ɂȂ�����
				m_nCntFrame[nCntType] = 0;
				m_nMaxFrame[nCntType] = 0;
				m_sizeDest[nCntType] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_nType[nCntType] = TYPE_NONE;
			}

			CScene2D::Setsize(size);					// �T�C�Y�ݒ�

			break;
		}
		case TYPE_FLASING:	// �_��
		{

			break;
		}
		}
	}
}