//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "meshcylinder.h"	// ���b�V���V�����_�[
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\bg000.png"		// �e�N�X�`��
#define TEXTURE_NAME_2	"data\\TEXTURE\\bg001.png"		// �e�N�X�`��
#define TEXTURE_NAME_3	"data\\TEXTURE\\bg002.png"		// �e�N�X�`��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMeshCylinder::m_pTexture[MESHCYLINDER_MAX_TEXTURE] = {};		// �e�N�X�`��

//=============================================================================
// �R���X�g���N�^									(public)	*** CMeshCylinder ***
//=============================================================================
CMeshCylinder::CMeshCylinder() : CMesh3D(CScene::PRIORITY_1)
{
	m_posUV = D3DXVECTOR2(0.0f, 0.0f);
	m_moveUV = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMeshCylinder ***
//=============================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//=============================================================================
// ���[�h����							(public)	*** CMeshCylinder ***
//=============================================================================
HRESULT CMeshCylinder::Load(void)
{
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[1]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pTexture[2]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < MESHCYLINDER_MAX_TEXTURE; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CMeshCylinder ***
//=============================================================================
CMeshCylinder *CMeshCylinder::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nTexNum, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex)
{
	CMeshCylinder *pMeshCylinder = NULL;				// �V�[���|�C���^

	if (pMeshCylinder == NULL)
	{// NULL�̏ꍇ
		pMeshCylinder = new CMeshCylinder;				// �V�[���������m��
	}

	if (pMeshCylinder != NULL)
	{// NULL�ȊO�̏ꍇ
		// �����������i�ʒu�A�傫���A�F�AUV�j
		pMeshCylinder->Init(meshType, num, pos, rot, size, col, TexUV, TexMoveUV, meshTex);
		pMeshCylinder->SetObjType(CScene::OBJTYPE_MESHCYLINDER);
		pMeshCylinder->BindTexture(m_pTexture[nTexNum]);	// �e�N�X�`���ݒ�
	}

	return pMeshCylinder;
}

//=============================================================================
// ����������										(public)	*** CMeshCylinder ***
//=============================================================================
HRESULT CMeshCylinder::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex)
{
	CMesh3D::SetInitAll(meshType, num, pos, rot, size, col, TexUV, meshTex);
	CMesh3D::Init();		// ����������
	m_moveUV = TexMoveUV;

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Uninit(void)
{
	CMesh3D::Uninit();						// �I������
}

//=============================================================================
// �X�V����											(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Update(void)
{
	m_posUV += m_moveUV;

	CMesh3D::SetTex(m_posUV);
}

//=============================================================================
// �`�揈��											(public)	*** CMeshCylinder ***
//=============================================================================
void CMeshCylinder::Draw(void)
{
	CMesh3D::Draw();						// �`�揈��
}