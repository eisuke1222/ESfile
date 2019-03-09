//=============================================================================
//
// �G�t�F�N�g3D���� [effect3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "effect3d.h"		// �G�t�F�N�g3D
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����
#include "effect.h"			// �G�t�F�N�g

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME		"data\\MODEL\\break000.x"		// ���f��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CEffect3D::m_pTexture	= {};			// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				CEffect3D::m_pMesh		= {};			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			CEffect3D::m_pBuffMat	= {};			// �}�e���A�����ւ̃|�C���^
DWORD					CEffect3D::m_nNumMat	= {};			// �}�e���A�����̐�

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::CEffect3D() : CModel3D(CScene::PRIORITY_1)
{
	m_nCntTimer = 0;								// ���ԃJ�E���^
	m_rotAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �������Z
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_bStop = false;								// �~�܂�
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CEffect3D::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	if (m_pBuffMat == NULL && m_pMesh == NULL)
	{// NULL�̏ꍇ		
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat];
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		for (DWORD tex = 0; tex < m_nNumMat; tex++)
		{// �J�E���g
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// �e�N�X�`�����g�p���Ă���
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[tex])))
				{
					MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CEffect3D::Unload(void)
{

	// �e�N�X�`���̔j��
	for (DWORD tex = 0; tex < m_nNumMat; tex++)
	{// �J�E���g
		if (m_pTexture[tex] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[tex]->Release();		// ���
			m_pTexture[tex] = NULL;			// NULL��
		}
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pBuffMat->Release();				// ���
		m_pBuffMat = NULL;					// NULL��
	}

	// �}�e���A���̊J��
	if (m_pMesh != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pMesh->Release();					// ���
		m_pMesh = NULL;						// NULL��
	}

	m_nNumMat = 0;				// ������
}

//=============================================================================
// �m�ۏ���											(public)	*** CEffect3D ***
//=============================================================================
CEffect3D *CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move)
{
	CEffect3D *pEffect3D = NULL;			// �|�C���^

	if (pEffect3D == NULL)
	{// NULL�̏ꍇ
		pEffect3D = new CEffect3D;			// �������m��
	}

	if (pEffect3D != NULL)
	{// NULL�ȊO�̏ꍇ
		pEffect3D->m_nID = pEffect3D->GetID();				// �eID�擾

		pEffect3D->Init(pos, rot, rotAdd, move);			// ����������
		pEffect3D->SetObjType(CScene::OBJTYPE_EFFECT3D);	// �I�u�W�F�N�g�^�C�v�ݒ�
	}

	return pEffect3D;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CEffect3D ***
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move)
{
	CModel3D::SetInitAll(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat, pos, rot);		// �����l�ݒ�
	CModel3D::Init();																// ����������

	m_rotAdd = rotAdd;
	m_move = move;

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Uninit(void)
{
	CModel3D::Uninit();						// �I������
}

//=============================================================================
// �X�V����											(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();	// �J�������_�擾
	D3DXVECTOR3 pos = CEffect3D::Getpos();						// �ʒu�擾
	D3DXVECTOR3 rot = CEffect3D::Getrot();						// �����擾
	D3DXVECTOR3 sizeMax = CEffect3D::Getsize(0);				// �傫���擾

	m_nCntTimer++;

	if (m_nCntTimer % 10 == 0)
	{
		if (m_bStop == false)
		{
			CEffect::Create(D3DXVECTOR3(pos.x, pos.y + sizeMax.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(EFFECT_SIZE_X * 1.5f, EFFECT_SIZE_Y * 1.5f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.03f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
		}
		else
		{
			CEffect::Create(D3DXVECTOR3(pos.x, pos.y + sizeMax.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, -2.5f), D3DXVECTOR3(EFFECT_SIZE_X * 2.5f, EFFECT_SIZE_Y * 2.5f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
		}
	}

	if (m_bStop == false)
	{
		pos += m_move;
		rot += m_rotAdd;

		m_move.y -= 0.1f;

		if (m_bStop == false)
		{
			if (pos.y < 0)
			{
				m_bStop = true;
			}
		}

		if (m_bStop == false)
		{
			// ����`�F�b�N------------------------------------------------------------
			CModel3D *pModel3D;						// �V�[��3D
			CScene::OBJTYPE objType;				// �I�u�W�F�N�g���
			CEffect3D *pEffect3D;					// �G�t�F�N�g3D
			D3DXVECTOR3 targetpos, targetsize;		// ����p : ����̈ʒu�A����̑傫��											

			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{// �V�[���J�E���g
				if (nCntScene != m_nID)
				{// �����ȊO
					pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_1, nCntScene);	// �V�[���ǂݍ���

					if (pModel3D != NULL)
					{// NULL�ȊO�̏ꍇ
						objType = pModel3D->GetObjType();			// �I�u�W�F�N�g�^�C�v�擾
						pEffect3D = (CEffect3D*)pModel3D;

						if (objType == OBJTYPE_EFFECT3D && pEffect3D->m_bStop == true)
						{
							targetpos = pModel3D->Getpos();			// �ʒu	
							targetsize = pModel3D->Getsize(0);		// �傫��

							if (((targetpos.x - pos.x) * (targetpos.x - pos.x)) + ((targetpos.y - pos.y) * (targetpos.y - pos.y)) < (targetsize.y + sizeMax.y) * (targetsize.y + sizeMax.y))
							{// �͈̓`�F�b�N�i�~�A��ʊO�j
								m_bStop = true;

								m_nCntTimer = 0;

								break;
							}
						}
					}
				}
			}
		}
	}

	Setpos(pos);
	Setrot(rot);

	if (Camerapos.z - pos.z >= 0)
	{// �J�����͈͊O�ɏo����
		Uninit();					// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Draw(void)
{
	CModel3D::Draw();						// �`�揈��
}