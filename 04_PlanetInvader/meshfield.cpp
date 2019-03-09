//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "meshfield.h"		// ���b�V���t�B�[���h
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\field000.jpg"		// �e�N�X�`��
#define TEXTURE_NAME_2	"data\\TEXTURE\\field001.jpg"		// �e�N�X�`��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMeshField::m_pTexture[MAX_MESHFIELD_TEXTURE] = {};		// �e�N�X�`��
D3DXVECTOR2 CMeshField::m_posUV = m_posUV = D3DXVECTOR2(0.0f, 0.0f);

//=============================================================================
// �R���X�g���N�^									(public)	*** CMeshField ***
//=============================================================================
CMeshField::CMeshField() : CMesh3D(CScene::PRIORITY_1)
{
	
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMeshField ***
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
// ���[�h����							(public)	*** CMeshField ***
//=============================================================================
HRESULT CMeshField::Load(void)
{
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_MESHFIELD_TEXTURE; nCntTex++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CMeshField ***
//=============================================================================
CMeshField *CMeshField::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex)
{
	CMeshField *pMeshField = NULL;				// �V�[���|�C���^

	if (pMeshField == NULL)
	{// NULL�̏ꍇ
		pMeshField = new CMeshField;			// �V�[���������m��
	}

	if (pMeshField != NULL)
	{// NULL�ȊO�̏ꍇ
		// �����������i�ʒu�A�傫���A�F�AUV�j
		pMeshField->Init(meshType, num, pos, rot, size, col, TexUV, meshfieldType, meshTex);
		pMeshField->SetObjType(CScene::OBJTYPE_MESHFIELD);
		pMeshField->BindTexture(m_pTexture[meshfieldType]);	// �e�N�X�`���ݒ�
	}

	return pMeshField;
}

//=============================================================================
// ����������										(public)	*** CMeshField ***
//=============================================================================
HRESULT CMeshField::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex)
{
	//CMesh3D::SetInitAll(meshType, num, pos, rot, size, col, TexUV, meshTex);
	CMesh3D::SetInitAll(meshType, D3DXVECTOR3(1.0f, 0.0f, 1.0f), pos, rot, size, col, D3DXVECTOR2(TexUV.x * num.x, TexUV.y * num.z), meshTex);
	CMesh3D::Init();		// ����������

	m_meshfieldType = meshfieldType;

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Uninit(void)
{
	CMesh3D::Uninit();						// �I������
}

//=============================================================================
// �X�V����											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();
	D3DXVECTOR3 pos = CMesh3D::Getpos();

	if (m_meshfieldType == MESHFIELDTYPE_O)
	{
		m_posUV.x += -0.001f;
		m_posUV.y += 0.002f;

		CMesh3D::SetTex(m_posUV);
	}

	if (Camerapos.z - pos.z >= 600)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��											(public)	*** CMeshField ***
//=============================================================================
void CMeshField::Draw(void)
{
	CMesh3D::Draw();						// �`�揈��
}