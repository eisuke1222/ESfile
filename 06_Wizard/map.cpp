//=============================================================================
//
// �}�b�v���� [map.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "map.h"			// �}�b�v
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h
#include "texture.h"		// �e�N�X�`��
#include "text.h"			// �e�L�X�g
#include "title.h"			// �^�C�g��
#include "game.h"			// �Q�[��
#include "effect.h"			// �G�t�F�N�g
#include "player.h"			// �v���C���[
#include "enemy.h"			// �G
#include "shadow.h"			// �e

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MC_SKYROT_0			(0.00001f)		// ���]�O
#define	MC_SKYROT_1			(0.00005f)		// ���]�P
#define	MC_AREACOLL_OLD		(0.92f)			// �G���A����O��
#define	MC_AREACOLL_NOW		(0.88f)			// �G���A���茻��

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
// �R���X�g���N�^										(public)	*** CMap ***
//=============================================================================
CMap::CMap() : CScene(CScene::PRIORITY_1, CScene::OBJTYPE_MAP)
{

}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMap ***
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
// ���[�h����										(public)	*** CMap ***
//=============================================================================
HRESULT CMap::Load(void)
{
	return S_OK;
}

//=============================================================================
// �A�����[�h����										(public)	*** CMap ***
//=============================================================================
void CMap::Unload(void)
{

}

//=============================================================================
// �N���G�C�g										(public)	*** CMap ***
//=============================================================================
CMap *CMap::Create(TYPE type)
{
	CMap *pMap = NULL;			// �|�C���^

	if (pMap == NULL)
	{// NULL�̏ꍇ
		if(type == TYPE_NO_00)			// �}�b�v_00
			pMap = new CMap_00;				// �������m��
		else if (type == TYPE_NO_01)	// �}�b�v_01
			pMap = new CMap_01;				// �������m��
		else if (type == TYPE_NO_02)	// �}�b�v_02
			pMap = new CMap_02;				// �������m��
		else if (type == TYPE_NO_03)	// �}�b�v_03
			pMap = new CMap_03;				// �������m��
		else if (type == TYPE_NO_04)	// �}�b�v_04
			pMap = new CMap_04;				// �������m��
	}

	if (pMap != NULL)
	{// NULL�ȊO�̏ꍇ
		pMap->Init();			// ����������
	}

	return pMap;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CMap ***
//=============================================================================
void CMap::Uninit(void)
{
	CScene::SetDeath();		// ���S�t���O
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^										(public)	*** CMap_00 ***
//=============================================================================
CMap_00::CMap_00() : CMap()
{
	// �����l�ݒ�
	m_pField = NULL;	// �t�B�[���h

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		m_pSky[nCount] = NULL;							// NULL��
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// ��]
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMap_00 ***
//=============================================================================
CMap_00::~CMap_00()
{

}

//=============================================================================
// ���[�h����										(public)	*** CMap_00 ***
//=============================================================================
HRESULT CMap_00::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Unload(void)
{

}

//=============================================================================
// ����������										(public)	*** CMap_00 ***
//=============================================================================
HRESULT CMap_00::Init(void)
{
	// ���b�V���t�B�[���h
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	int nMaxVtx = m_pField->GetMaxVtx();

	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 pos = m_pField->GetVtx(nCntVtx);

		if (pos.x < -1500.0f || 1500.0f < pos.x || pos.z < -1500.0f || 1500.0f < pos.z)
		{
			pos.y = 80.0f + (rand() % 80);

			m_pField->SetVtx(nCntVtx, pos);
		}
	}
	m_pField->SetVecNor();
	
	// ��P
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// ��Q
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(true, false, false, false, true);
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Uninit(void)
{
	// �t�B�[���h
	if (m_pField != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pField->Uninit();		// �I������
		m_pField = NULL;		// NULL��
	}

	// ��
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		if (m_pSky[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pSky[nCount]->Uninit();	// �I������
			m_pSky[nCount] = NULL;		// NULL��
		}
	}

	CMap::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Update(void)
{
	// ��0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// ��]

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// �e�N�X�`���ݒ�
	}

	// ��1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// ��]

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// �e�N�X�`���ݒ�
	}
}

//=============================================================================
// �`�揈��											(public)	*** CMap_00 ***
//=============================================================================
void CMap_00::Draw(void)
{

}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CMap_01::m_nMaxModel	= 0;		// ���f����

LPDIRECT3DTEXTURE9		**CMap_01::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CMap_01::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CMap_01::m_pBuffMat	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CMap_01::m_nNumMat		= NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CMap_01::m_ModelInfo	= NULL;		// ���f���C���t�H

//=============================================================================
// �R���X�g���N�^										(public)	*** CMap_01 ***
//=============================================================================
CMap_01::CMap_01() : CMap()
{
	// �����l�ݒ�
	m_pModel = NULL;	// ���f��

	m_pField = NULL;	// �t�B�[���h

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		m_pSky[nCount] = NULL;							// NULL��
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// ��]
	}

	m_pGrass = NULL;		// ��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMap_01 ***
//=============================================================================
CMap_01::~CMap_01()
{

}

//=============================================================================
// ���[�h����										(public)	*** CMap_01 ***
//=============================================================================
HRESULT CMap_01::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\titlemap.txt", &m_nMaxModel);		// ���f���擾

	// �e���̃������m��
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_nMaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_nMaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_nMaxModel];

	// �e���̏�����
	for (int nCount = 0; nCount < m_nMaxModel; nCount++)		// ���f�����J�E���g
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_nMaxModel; nCntParnet++)
	{// ���f�����J�E���g
		// X�t�@�C���̓ǂݍ���
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Unload(void)
{
	// �e���̃������J���ANULL��
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// ���f���C���t�H
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULL��
}

//=============================================================================
// ����������										(public)	*** CMap_01 ***
//=============================================================================
HRESULT CMap_01::Init(void)
{
	// ���f��
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULL�̏ꍇ
		m_pModel = new CModel[m_nMaxModel];	// �������m��

		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{// ���f�����J�E���g
		 // �����l�ݒ�
			m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// ����������
			m_pModel[nCntModel].Init();

			// �e���f����ݒ�
			if (0 <= m_ModelInfo[nCntModel].nIndex)		// �e�ȊO
				m_pModel[nCntModel].SetParent(&m_pModel[m_ModelInfo[nCntModel].nIndex]);
		}
	}

	// ���b�V���t�B�[���h
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
	//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	int nMaxVtx = m_pField->GetMaxVtx();	// ���_���擾

	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{// ���_�J�E���g
		D3DXVECTOR3 pos = m_pField->GetVtx(nCntVtx);	// ���_�ʒu�擾

		// �͈͌v�Z
		float fRange = sqrtf(powf(pos.x, 2.0f) + powf(pos.z, 2.0f));

		if (1100.0f < fRange)
		{// �R�P
			pos.y = 100.0f + (rand() % 10 * 15);
			m_pField->SetVtx(nCntVtx, pos);
		}
		else if (950.0f < fRange)
		{// �R�Q
			pos.y = 80.0f + (rand() % 10 * 10);
			m_pField->SetVtx(nCntVtx, pos);
		}
		else if (300.0f < fRange)
		{// �Ǝ��ӂ͂O��
			pos.y = 0.0f - (rand() % 15);
			m_pField->SetVtx(nCntVtx, pos);
		}

		if (170.0f < pos.x && pos.x < 690.0f && -160.0f < pos.z && pos.z < 160.0f)
		{// �`���[�g���A���͈�
			pos.y = 0.0f - (rand() % 10);
			m_pField->SetVtx(nCntVtx, pos);
		}
		else if (70.0f < pos.x && pos.x < 790.0f && -260.0f < pos.z && pos.z < 260.0f)
		{// �`���[�g���A���͈͂̍�
			pos.y = 0.0f;
			m_pField->SetVtx(nCntVtx, pos);
		}
	}
	m_pField->SetVecNor();

	// ��P
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// ��Q
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(true, false, false, false, true);
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	CScene		*pScene		= NULL;		// �V�[��
	CScene		*pSceneNext	= NULL;		// ���V�[��
	CSceneMesh	*pMesh		= NULL;		// ���b�V��
	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g
				break;
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	// ��
	m_pGrass = new GRASS[m_GRASS_MAX];
	for (int nCntGrass = 0; nCntGrass < m_GRASS_MAX; nCntGrass++)
	{// �ő吔�J�E���g
		D3DXVECTOR3 pos;		// �ʒu

		bool bCheck = false;	// �`�F�b�N
		do
		{// ���[�v
			bCheck = false;		// ������

			// �ʒu�v�Z
			pos = D3DXVECTOR3(-250.0f + (float)(rand() % 900), 0.0f, 400.0f - (float)(rand() % 1000));

			// �͈͌v�Z
			float fRange = sqrtf(powf(pos.x, 2.0f) + powf(pos.z, 2.0f));
			if (fRange < 150.0f)
			{// �͈͓���������
				bCheck = true;	// �`�F�b�N
			}
		} while (bCheck != false);

		// �����l�ݒ�
		m_pGrass[nCntGrass].nCounter = 0;
		m_pGrass[nCntGrass].pos = pos;
		//m_pGrass[nCntGrass].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
		m_pGrass[nCntGrass].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
		{// ���J�E���g
			// �傫���Ɗp�x
			D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
			D3DXVECTOR3 rot = m_pGrass[nCntGrass].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

			// �����Ɗp�x
			float fLenght = -5.0f + (float)(rand() % 10);
			float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);

			// ���̑�
			D3DXVECTOR3 posLeft = m_pGrass[nCntGrass].pos +
				D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
				(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

			for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
			{// �J�E���g
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Init();
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

				if (pMesh != NULL)
				{// NULL�ȊO�̏ꍇ
					D3DXVECTOR3 pos = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					pos.y = pMesh->GetHeight(pos);
					pos.y += 7.0f;
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Setpos(pos);
				}

				m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posLeft;

				D3DXVECTOR3 posVtx[4];
				m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
				m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0] = posVtx[0];
				m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1] = posVtx[1];

				m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
				m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;

				posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
			}
		}
	}

	// ����
	CEffect::Create(CEffect::TYPE_FIRE, CEffect::SHOT_NONE, D3DXVECTOR3(0.0f, 2.0f, -30.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Uninit(void)
{
	// ���f��
	if (m_pModel != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++) // ���f�����J�E���g
			m_pModel[nCntModel].Uninit();	// �I������

		delete[] m_pModel;		// �������J��
		m_pModel = NULL;		// NULL��
	}

	// �t�B�[���h
	if (m_pField != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pField->Uninit();		// �I������
		m_pField = NULL;		// NULL��
	}

	// ��
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		if (m_pSky[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pSky[nCount]->Uninit();	// �I������
			m_pSky[nCount] = NULL;		// NULL��
		}
	}

	// ��
	if (m_pGrass != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntGrass = 0; nCntGrass < m_GRASS_MAX; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// ���J�E���g
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// �I������
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULL��
				}
			}
		}

		delete[] m_pGrass;	// �������J��
		m_pGrass = NULL;		// NULL��
	}

	CMap::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Update(void)
{
	// ��0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// ��]

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// �e�N�X�`���ݒ�
	}

	// ��1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// ��]

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// �e�N�X�`���ݒ�
	}

	CPlayer *pPlayer = CManager::GetTitle()->GetPlayer();
	if (pPlayer != NULL)
	{
		D3DXVECTOR3 posPlayer = pPlayer->Getpos();

		if (m_pGrass != NULL)
		{// NULL�ȊO�̏ꍇ
			for (int nCntGrass = 0; nCntGrass < m_GRASS_MAX; nCntGrass++)
			{// �ő吔�J�E���g
				for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
				{// ���J�E���g
					for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
					{
						D3DXVECTOR3 posVtx[4];		// ���_�󂯎��
						D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// ���_���擾

						float fRange;
						fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));
						if (fRange < 30.0f)
						{// �͈͓���������
							float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
							D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
							if (fRange < 20.0f)
							{
								posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
								posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
							}

							m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
						}
						else
						{// �͈͊O
							if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
							{// �|��Ă���
								posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
								posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
								posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
								posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

								if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
								{
									m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
									m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
									m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
								}
							}
						}

						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
						{// �|��Ă��Ȃ�
							if (m_pGrass->nCounter < 120)
							{// ���J�E���g���Ⴂ
								posVtx[0].x += sinf(0.0f) * 0.023f;
								posVtx[0].x += cosf(0.0f) * 0.023f;
								posVtx[1].x += sinf(0.0f) * 0.023f;
								posVtx[1].x += cosf(0.0f) * 0.023f;
							}
							else
							{// ���J�E���g�ȏ�
								posVtx[0].x -= sinf(0.0f) * 0.023f;
								posVtx[0].x -= cosf(0.0f) * 0.023f;
								posVtx[1].x -= sinf(0.0f) * 0.023f;
								posVtx[1].x -= cosf(0.0f) * 0.023f;
							}
						}
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// ���_���ݒ�
					}
				}

				{
					m_pGrass[nCntGrass].nCounter++;		// �J�E���g����

					if (m_pGrass[nCntGrass].nCounter == 240)
					{// �w�肵���J�E���g�ɂȂ�����
						m_pGrass[nCntGrass].nCounter = 0;	// �J�E���g������
					}
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CMap_01 ***
//=============================================================================
void CMap_01::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{// ���f�����J�E���g
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		if (m_pModel != NULL)
		{// NULL�ȊO�̏ꍇ
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModel[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// �e�\��
			//CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(0.0f, 0.1f, 0.0f));
		}
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CMap_02::m_nMaxModel		= 0;		// ���f����

LPDIRECT3DTEXTURE9		**CMap_02::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CMap_02::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CMap_02::m_pBuffMat	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CMap_02::m_nNumMat		= NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CMap_02::m_ModelInfo	= NULL;		// ���f���C���t�H

//=============================================================================
// �R���X�g���N�^										(public)	*** CMap_02 ***
//=============================================================================
CMap_02::CMap_02() : CMap()
{
	m_nModelWarp = 0;									// ���f�����[�v��
	m_pModelWarp = NULL;								// ���f�����[�v

	m_nModelObject = 0;									// ���f���I�u�W�F�N�g��
	m_pModelObject = NULL;								// ���f���I�u�W�F�N�g

	m_pField = NULL;									// �t�B�[���h

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		m_pSky[nCount] = NULL;							// NULL��
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// ��]
	}

	m_nGrassMax = 0;									// �ő呐
	m_pGrass = NULL;									// ��

	m_nAreaMax = 0;										// �G���A�ő吔

	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaNum[nCount] = 0;							// �G���A��

	m_pArea = NULL;										// �G���A

	m_nRouteMax = 0;									// ���[�g�ő吔
	m_pRoute = NULL;									// ���[�g

	m_nPatternMax = 0;									// �p�^�[���ő吔
	m_pPattern = NULL;									// �p�^�[��

	m_pWarp = NULL;										// ���[�v

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
		m_pWarpCylinder[nCount] = NULL;					// ���[�v�V�����_�[
	m_WarpCylinderUV = D3DXVECTOR2(0.0f, 0.0f);			// ���[�v�V�����_�[UV

	m_pGoalCylinder = NULL;								// �S�[���V�����_�[

	m_bDrawIcon = false;								// �A�C�R���`��
	m_pIcon	= NULL;										// �A�C�R��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMap_02 ***
//=============================================================================
CMap_02::~CMap_02()
{

}

//=============================================================================
// ���[�h����										(public)	*** CMap_02 ***
//=============================================================================
HRESULT CMap_02::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\gamemap.txt", &m_nMaxModel);		// ���f���擾

	// �e���̃������m��
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_nMaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_nMaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_nMaxModel];

	// �e���̏�����
	for (int nCount = 0; nCount < m_nMaxModel; nCount++)		// ���f�����J�E���g
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_nMaxModel; nCntParnet++)
	{// ���f�����J�E���g
		// X�t�@�C���̓ǂݍ���
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Unload(void)
{
		// �e���̃������J���ANULL��
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// ���f���C���t�H
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULL��
}

//=============================================================================
// ����������										(public)	*** CMap_02 ***
//=============================================================================
HRESULT CMap_02::Init(void)
{
	// ���b�V���t�B�[���h
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3600.0f, 0.0f, 3600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(24.0f, 24.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	// ��P
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// ��Q
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Uninit(void)
{
	// ���f��
	if (m_pModelWarp != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++) // ���f�����J�E���g
			m_pModelWarp[nCntModel].Uninit();	// �I������

		delete[] m_pModelWarp;		// �������J��
		m_pModelWarp = NULL;		// NULL��
	}
	if (m_pModelObject != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++) // ���f�����J�E���g
			m_pModelObject[nCntModel].Uninit();	// �I������

		delete[] m_pModelObject;	// �������J��
		m_pModelObject = NULL;		// NULL��
	}

	// �t�B�[���h
	if (m_pField != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pField->Uninit();		// �I������
		m_pField = NULL;		// NULL��
	}

	// ��
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		if (m_pSky[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pSky[nCount]->Uninit();	// �I������
			m_pSky[nCount] = NULL;		// NULL��
		}
	}

	// ��
	if (m_pGrass != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// ���J�E���g
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// �I������
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULL��
				}
			}
		}

		delete[] m_pGrass;	// �������J��
		m_pGrass = NULL;		// NULL��
	}

	// �}�b�v
	if (m_pArea != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pArea;		// �j��
		m_pArea = NULL;			// NULL��
	}
	if (m_pRoute != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pRoute;		// �j��
		m_pRoute = NULL;		// NULL��
	}
	if (m_pPattern != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nPatternMax; nCount++)
		{// �G���A�J�E���g
			delete[] m_pPattern[nCount].nPattern;
			m_pPattern[nCount].nPattern = NULL;
		}

		delete[] m_pPattern;	// �j��
		m_pPattern = NULL;		// NULL��
	}
	if (m_pWarp != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pWarp;		// �j��
		m_pWarp = NULL;			// NULL��
	}

	// ���[�v
	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
	{// ���[�v�J�E���g
		if (m_pWarpCylinder[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pWarpCylinder[nCount]->Uninit();
			m_pWarpCylinder[nCount] = NULL;
		}
	}

	// �S�[���V�����_�[
	if (m_pGoalCylinder != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pGoalCylinder->Uninit();
		m_pGoalCylinder = NULL;
	}

	// �A�C�R��
	if (m_pIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}

	CMap::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Update(void)
{
	CSound *pSound = CManager::GetSound();

	// ��0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// ��]

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// �e�N�X�`���ݒ�
	}

	// ��1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// ��]

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// �e�N�X�`���ݒ�
	}

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->Getpos();
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 CameraposR = pCamera->Getpos(1);
	D3DXVECTOR3 Camerarot = pCamera->Getrot();

	if (m_pGrass != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{// �ő吔�J�E���g
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// ���J�E���g
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{// ���C���J�E���g
					D3DXVECTOR3 posVtx[4];		// ���_�󂯎��
					D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// ���_���擾

					// �J�����͈�
					float fCameraL = Camerarot.y - (D3DX_PI * 0.5f);
					float fCameraR = Camerarot.y + (D3DX_PI * 0.5f);

					// �͈͒���
					if (fCameraL < -D3DX_PI)
						fCameraL += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraL)
						fCameraL -= D3DX_PI * 2.0f;

					if (fCameraR < -D3DX_PI)
						fCameraR += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraR)
						fCameraR -= D3DX_PI * 2.0f;

					float fCameraAngle = atan2f(posCenter.x - CameraposR.x, posCenter.z - CameraposR.z);

					if (Camerarot.y < (-D3DX_PI * 0.5f) || (D3DX_PI * 0.5f) < Camerarot.y)
					{
						if (fCameraR < fCameraAngle && fCameraAngle < fCameraL)
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}
					else
					{
						if (fCameraL < fCameraAngle && fCameraAngle < fCameraR)
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}

					float fRange;
					fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));					
					if (fRange < 850.0f)
					{// �͈͓�
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
					}
					else
					{// �͈͊O
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
					}

					if (fRange < 30.0f)
					{// �͈͓�	
						float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
						D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
						fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
						if (fRange < 20.0f)
						{
							posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
						}

						m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
					}
					else
					{// �͈͊O
						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
						{// �|���
							posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
							posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
							posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
							posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
							m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

							if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
							{
								m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
							}
						}
					}

					if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
					{// �|��Ă��Ȃ�
						if (m_pGrass->nCounter < 120)
						{// ���J�E���g���Ⴂ
							posVtx[0].x += sinf(0.0f) * 0.023f;
							posVtx[0].x += cosf(0.0f) * 0.023f;
							posVtx[1].x += sinf(0.0f) * 0.023f;
							posVtx[1].x += cosf(0.0f) * 0.023f;
						}
						else
						{// ���J�E���g�ȏ�
							posVtx[0].x -= sinf(0.0f) * 0.023f;
							posVtx[0].x -= cosf(0.0f) * 0.023f;
							posVtx[1].x -= sinf(0.0f) * 0.023f;
							posVtx[1].x -= cosf(0.0f) * 0.023f;
						}
					}
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// ���_���ݒ�
				}
			}

			{
				m_pGrass[nCntGrass].nCounter++;		// �J�E���g����

				if (m_pGrass[nCntGrass].nCounter == 240)
				{// ���̃J�E���g�ɂȂ�����
					m_pGrass[nCntGrass].nCounter = 0;	// �J�E���g������
				}
			}
		}
	}

	m_WarpCylinderUV.y -= 0.01f;
	for (int nCntWarp = 0; nCntWarp < m_nModelWarp; nCntWarp++)
	{// ���[�v���f���J�E���g
		if (m_pWarpCylinder[nCntWarp] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pWarpCylinder[nCntWarp]->SetTex(m_WarpCylinderUV);
		}
	}

	if (m_pIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		D3DXCOLOR col = m_pIcon->Getcol();
		if (m_bDrawIcon == true)
		{// �\��
			if (col.a != 1.0f)
			{// �����ȊO
				col.a += 0.05f;
				if (1.0f < col.a) col.a = 1.0f;
			}
		}
		else
		{// ��\��
			if (col.a != 0.0f)
			{// �s�����ȊO
				col.a -= 0.05f;
				if (col.a < 0.0f) col.a = 0.0f;
			}
		}
		m_pIcon->Setcol(col);
	}
	m_bDrawIcon = false;
}

//=============================================================================
// �`�揈��											(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	if (1 < m_nPatternMax)
	{
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
		{// ���f�����J�E���g
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			if (m_pModelWarp != NULL)
			{// NULL�ȊO�̏ꍇ
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				m_pModelWarp[nCntModel].Draw();

				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}

	for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
	{// ���f�����J�E���g
	 // ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		if (m_pModelObject != NULL)
		{// NULL�ȊO�̏ꍇ
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModelObject[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// �e�\��
			//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
		}
	}
}

//=============================================================================
// �t�B�[���h�ݒ菈��									(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::SetField(D3DXVECTOR3 pos)
{
	// �G���A��
	m_nAreaNum[AREA_START]	= 1;
	m_nAreaNum[AREA_END]		= 1;
	m_nAreaNum[AREA_NORMAL]	= rand() % 1 + 2;
	m_nAreaNum[AREA_BATTLE]	= rand() % 1 + 2;
	m_nAreaNum[AREA_ITEM]	= 1;

	// �G���A�����v�Z
	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaMax += m_nAreaNum[nCount];

	// �������m��
	if (m_pArea == NULL) m_pArea = new AREA_RANGE[m_nAreaMax];

	// �G���A������
	for (int nCount = 0; nCount < m_nAreaMax; nCount++)
	{// �G���A�J�E���g
		m_pArea[nCount].nArea	= AREA_START;						// �G���A
		m_pArea[nCount].fRange	= 0.0f;								// �͈�
		m_pArea[nCount].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���S
	}

	// �G���A�A���[�g����
	{
		// �G���A
		{
			int nCntArea = 0;		// �G���A�ԍ�
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
			{// �G���A�J�E���g
				for (int nCntNum = 0; nCntNum < m_nAreaNum[nCount]; nCntNum++)
				{// �G���A���J�E���g
					int nCheckCounter = 0;	// �`�F�b�N�J�E���g
					bool bCheck = false;	// �`�F�b�N
					do
					{// ���[�v
						bCheck = false;

						switch (nCount)
						{// �G���A
						case AREA_START:	// �X�^�[�g
							m_pArea[nCntArea].nArea = AREA_START;		// �G���A
							m_pArea[nCntArea].fRange = 280.0f;			// �͈�
							m_pArea[nCntArea].pos	= pos;				// �ʒu
							break;
						case AREA_END:		// �G���h
							m_pArea[nCntArea].nArea = AREA_END;			// �G���A
							m_pArea[nCntArea].fRange = 280.0f;			// �͈�

							// �ʒu
							{
								float fRange = 0.0f;	// �͈�
								do
								{// ���[�v
									m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
									m_pArea[nCntArea].pos.y = 0.0f;
									m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));

									// ���S����̋������v�Z
									fRange = sqrtf(powf(m_pArea[0].pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(m_pArea[0].pos.z - m_pArea[nCntArea].pos.z, 2.0f));
								} while (fRange < 1000.0f);		// ��苗���ȏゾ������
							}
							break;
						case AREA_NORMAL:	// �m�[�}��
						case AREA_BATTLE:	// �o�g��
							// �G���A
							if (nCount == AREA_NORMAL)	m_pArea[nCntArea].nArea = AREA_NORMAL;
							if (nCount == AREA_BATTLE)	m_pArea[nCntArea].nArea = AREA_BATTLE;
							m_pArea[nCntArea].fRange = (float)(rand() % 100 + 380);			// �͈�

							// �ʒu
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						case AREA_ITEM:		// �A�C�e��
							m_pArea[nCntArea].nArea = AREA_ITEM;			//�@�G���A
							m_pArea[nCntArea].fRange = (float)(rand() % 50 + 320);			// �͈�

							// �ʒu
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						}

						// �G���A���m�d�Ȃ��Ă��Ȃ���
						for (int nCntCheck = 0; nCntCheck < nCntArea; nCntCheck++)
						{// �G���A�J�E���g
							// ���S����̈ʒu�v�Z
							float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - m_pArea[nCntCheck].pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - m_pArea[nCntCheck].pos.z, 2.0f));
							if (fRange < m_pArea[nCntArea].fRange + m_pArea[nCntCheck].fRange + 200.0f)
							{// �G���A���d�Ȃ��Ă��Ȃ���
								bCheck = true;		// �d�Ȃ��Ă���
								nCheckCounter++;	// �J�E���g
								break;
							}
						}

						if (nCheckCounter == 30) break;
					} while (bCheck != false);
					
					if (nCheckCounter != 30)
						nCntArea++;		// �G���A�J�E���g
				}
			}
			m_nAreaMax = nCntArea;
		}

		// ���[�g
		{
			// ���[�g��
			m_nRouteNum[AREA_START]	= m_nAreaNum[AREA_START];
			m_nRouteNum[AREA_END]	= m_nAreaNum[AREA_END];
			m_nRouteNum[AREA_NORMAL] = m_nAreaNum[AREA_NORMAL];
			m_nRouteNum[AREA_BATTLE] = m_nAreaNum[AREA_BATTLE];
			m_nRouteNum[AREA_ITEM]	= m_nAreaNum[AREA_ITEM];

			// ���[�g�����v�Z
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
				m_nRouteMax += m_nRouteNum[nCount];

			// �������m��
			if (m_pRoute == NULL) m_pRoute = new ROUTE_RANGE[m_nRouteMax];

			// ���[�g������
			for (int nCount = 0; nCount < m_nRouteMax; nCount++)
			{// ���[�g�J�E���g
				m_pRoute[nCount].nAreaNumStart = 0;									// �X�^�[�g�G���A�ԍ�
				m_pRoute[nCount].nAreaNumEnd = 0;									// �G���h�G���A�ԍ�
				m_pRoute[nCount].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �X�^�[�g�G���A�ʒu
				m_pRoute[nCount].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �G���h�G���A�ʒu
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					m_pRoute[nCount].pos[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g���_�ʒu
				m_pRoute[nCount].fAngle = 0.0f;										// ���[�g�p�x
				m_pRoute[nCount].fLenght = 0.0f;										// ���[�g����
				m_pRoute[nCount].fRange = 0.0f;										// ���[�g�͈́i�����j
			}

			int nRoute = 0;		// ���[�g�ԍ�
			for (int nCountArea = 0; nCountArea < m_nAreaMax; nCountArea++)
			{// �G���A�J�E���g
				int nAreaNum = nCountArea;		// �G���A�ԍ�
				float fAreaLenght = 5000.0f;	// ����

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// �G���A�J�E���g
					if (nCountArea != nCntAreaCheck && 
						(nCountArea != AREA_START || (nCountArea == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nCountArea != AREA_END || (nCountArea == AREA_END && nCntAreaCheck != AREA_START)))
					{// �����G���A�łȂ��A�X�^�[�g�ƃG���h���q���Ȃ�
						// ���S����̈ʒu�v�Z
						float fRange = sqrtf(powf(m_pArea[nCountArea].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nCountArea].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// ���������߂�������
							nAreaNum = nCntAreaCheck;	// �ԍ�
							fAreaLenght = fRange;		// ����
						}
					}
				}

				bool bCheck = false;	// �`�F�b�N
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ���[�g�J�E���g
					if ((m_pRoute[nCntRoute].nAreaNumStart == nCountArea && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nCountArea))
					{// �������[�g���Ȃ���
						bCheck = true;	// ����
					}
				}

				if (bCheck == false)
				{// ��������
					// �͈́i�����j
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// ����
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// �p�x
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nCountArea].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nCountArea].pos.z);

					// �ʒu
					m_pRoute[nRoute].nAreaNumStart	= nCountArea;				// �X�^�[�g�ԍ�
					m_pRoute[nRoute].nAreaNumEnd	= nAreaNum;					// �G���h�ԍ�
					m_pRoute[nRoute].posStart		= m_pArea[nCountArea].pos;	// �X�^�[�g�ʒu
					m_pRoute[nRoute].posEnd			= m_pArea[nAreaNum].pos;	// �G���h�ʒu

					// ���S���v�Z
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// ���_�v�Z
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// ����p�̒��_�v�Z
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ���[�g�J�E���g
					if (nRoute == m_nRouteMax) break;
				}
			}

			// �e�G���A�Ƀ��[�g�����{�q��������
			int *nRouteCounter = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteCounter[nCount] = 0;

			for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
			{// ���[�g�J�E���g
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumStart]++;
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumEnd]++;
			}

			// ���[�̃G���A���v�Z
			int nRouteEndNum = 0;
			int *nRouteEnd = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteEnd[nCount] = 0;

			// �q���ꂽ���[�g��
			int nRoute_0 = 0;	
			int nRoute_1 = 0;
			int nRoute_2 = 0;

			for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
			{// ���[�g�J�E���g
				nRouteEnd[nRouteEndNum] = nCntRoute;	// �ԍ��L��
				nRouteEndNum++;							// �J�E���g

				if (nRouteCounter[nCntRoute] == 0)
					nRoute_0++;									// ���[�g���O
				else if (nRouteCounter[nCntRoute] == 1)
					nRoute_1++;									// ���[�g���P
				else if (1 < nRouteCounter[nCntRoute])
					nRoute_2 += nRouteCounter[nCntRoute] - 1;	// ���[�g���Q
			}

			// �p�^�[�������v�Z
			int nRoutePattern = nRoute - nRoute_2 + nRoute_0;

			if (nRoute_0 != 0)
			{// ���[�g
				int nNumber = 0;
				for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
				{// ���[�g�J�E���g
					if (nRouteCounter[nCntRoute] == 0)
					{// �q�����Ă��Ȃ�
						nNumber = nCntRoute;
						break;
					}
				}

				int nAreaNum = nNumber;			// �G���A�ԍ�
				float fAreaLenght = 5000.0f;	// ����

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// �G���A�J�E���g
					if (nNumber != nCntAreaCheck &&
						(nNumber != AREA_START || (nNumber == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nNumber != AREA_END || (nNumber == AREA_END && nCntAreaCheck != AREA_START))) {// �����G���A�łȂ��A�X�^�[�g�ƃG���h���q���Ȃ�
						// ���S����̈ʒu�v�Z
						float fRange = sqrtf(powf(m_pArea[nNumber].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nNumber].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// ���������߂�������
							nAreaNum = nCntAreaCheck;	// �ԍ�
							fAreaLenght = fRange;		// ����
						}
					}
				}

				bool bCheck = false;	// �`�F�b�N
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ���[�g�J�E���g
					if ((m_pRoute[nCntRoute].nAreaNumStart == nRouteEnd[nCntRoute] && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nRouteEnd[nCntRoute]))
					{// �������[�g���Ȃ���
						bCheck = true;	// ����
					}
				}

				if (bCheck == false)
				{// ��������
					// �͈́i�����j
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// ����
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// �p�x
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nNumber].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nNumber].pos.z);

					// �ʒu
					m_pRoute[nRoute].nAreaNumStart = nNumber;
					m_pRoute[nRoute].nAreaNumEnd = nAreaNum;
					m_pRoute[nRoute].posStart = m_pArea[nNumber].pos;
					m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;

					// ���S���v�Z
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// ���_�v�Z
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// ����p�̒��_�v�Z
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ���[�g�J�E���g
					nRoutePattern--;
				}
			}

			m_nRouteMax = nRoute;	// ���[�g���J�E���g

			m_nPatternMax = nRoutePattern;		// �p�^�[�������L��

			// �������m��
			if (m_pPattern == NULL)
				m_pPattern = new ROUTE_PATTERN[m_nPatternMax];

			if (m_pPattern != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCntPattert = 0; nCntPattert < m_nPatternMax; nCntPattert++)
				{// �p�^�[�����J�E���g
					// �����l�ݒ�
					m_pPattern[nCntPattert].nMaxPattern = 0;
					m_pPattern[nCntPattert].nPattern = new int[m_nAreaMax];
					m_pPattern[nCntPattert].bStart = false;
					m_pPattern[nCntPattert].bEnd = false;

					if (nCntPattert == 0)
					{// �J�E���g���O��������
						m_pPattern[nCntPattert].nPattern[0] = m_pRoute[0].nAreaNumStart;
						m_pPattern[nCntPattert].nPattern[1] = m_pRoute[0].nAreaNumEnd;
						m_pPattern[nCntPattert].nMaxPattern = 2;
					}
					else
					{// �J�E���g���O�ȊO��������
						int nNumber = 0;		// �ԍ��m�F
						bool bCheck = false;	// �`�F�b�N
						// �����ԍ��ɂȂ�Ȃ��悤����
						do
						{// ���[�v
							bCheck = false;		// ������
							for (int nCntCheck = 0; nCntCheck < nCntPattert; nCntCheck++)
							{// �p�^�[���J�E���g
								for (int nCntArea = 0; nCntArea < m_pPattern[nCntCheck].nMaxPattern; nCntArea++)
								{// �G���A�J�E���g
									if (m_pPattern[nCntCheck].nPattern[nCntArea] == nNumber)
									{// ������������
										nNumber++;		// �J�E���g
										bCheck = true;	// �`�F�b�N
										break;
									}
								}
								if (bCheck == true) break;
							}
						} while (bCheck != false);

						// �ԍ��ɂȂ��郋�[�g������
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ���[�g�J�E���g
							if (nNumber == m_pRoute[nCntRoute].nAreaNumStart || nNumber == m_pRoute[nCntRoute].nAreaNumEnd)
							{// �ԍ�������
								m_pPattern[nCntPattert].nPattern[0] = m_pRoute[nCntRoute].nAreaNumStart;
								m_pPattern[nCntPattert].nPattern[1] = m_pRoute[nCntRoute].nAreaNumEnd;
								m_pPattern[nCntPattert].nMaxPattern = 2;
								break;
							}
						}
					}

					for (int nCntPattertNum = 0; nCntPattertNum < m_pPattern[nCntPattert].nMaxPattern; nCntPattertNum++)
					{// �p�^�[���ɂ���G���A���J�E���g
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ���[�g�J�E���g
							if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumStart)
							{// �X�^�[�g�ԍ��Ɠ���
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// �p�^�[���ɂ���G���A���J�E���g
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumEnd)
									{// �G���h�ԍ��Ɠ���
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// �����ԍ����Ȃ�������
										// �ԍ��L��
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumEnd;
										m_pPattern[nCntPattert].nMaxPattern++;	// �p�^�[���̃G���A���𑝂₷
									}
								}
							}
							else if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumEnd)
							{// �G���h�ԍ��Ɠ���
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// �p�^�[���ɂ���G���A���J�E���g
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumStart)
									{// �X�^�[�g�ԍ��Ɠ���
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// �����ԍ����Ȃ�������
										// �ԍ��L��
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumStart;
										m_pPattern[nCntPattert].nMaxPattern++;	// �p�^�[���̃G���A���𑝂₷
									}
								}
							}
						}
					}
				}
			}

			for (int nCntPattern = 0; nCntPattern < m_nPatternMax; nCntPattern++)
			{// �p�^�[�����J�E���g
				for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattern].nMaxPattern; nCntCheck++)
				{// �p�^�[���ɂ���G���A���J�E���g
					if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_START)
					{// �X�^�[�g�ԍ��Ɠ���
						m_pPattern[nCntPattern].bStart = true;		// �X�^�[�g������
					}
					else if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_END)
					{// �G���h�ԍ��Ɠ���
						m_pPattern[nCntPattern].bEnd = true;		// �G���h������
					}
				}
			}

			if (1 < m_nPatternMax)
			{// �p�^�[�������P��葽��
				// �������m��
				if (m_pWarp == NULL)
					m_pWarp = new WARP[m_nPatternMax - 1];

				if (m_pWarp != NULL)
				{// NULL�ȊO�̏ꍇ
					for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
					{// ���[�v�J�E���g
						if (nCntWarp == 0)
						{// �J�E���g���O��������
							bool bCheck = false;	// �`�F�b�N
							do
							{// ���[�v
								bCheck = false;		// ������
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// �p�^�[���ݒ�

								do
								{// ���[�v
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// �p�^�[���ݒ�
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// �p�^�[�����Ⴄ�Ȃ甲����

								if (2 < m_nPatternMax)
								{// �p�^�[�����Q��葽��
									if ((m_pPattern[m_pWarp[nCntWarp].nPattern_1].bStart	== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd	== true) ||
										(m_pPattern[m_pWarp[nCntWarp].nPattern_1].bEnd		== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart	== true))
									{// �X�^�[�g�ƃG���h�����΂ꂽ
										bCheck = true;		// �`�F�b�N
									}
								}
							} while (bCheck != false);
						}
						else
						{// �J�E���g���P�ȏゾ������
							bool bCheck = false;	// �`�F�b�N
							do
							{// ���[�v
								bCheck = false;		// ������
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// �p�^�[���ݒ�

								for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
								{// ���[�v�J�E���g
									if (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_1 ||
										m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_2)
									{// �����ԍ�������
										bCheck = true;		// �`�F�b�N
										break;
									}
								}
							} while (bCheck != false);

							do
							{// ���[�v
								bCheck = false;		// �`�F�b�N
								do
								{// ���[�v
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// �p�^�[���ݒ�
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// �p�^�[�����Ⴄ�Ȃ甲����

								if (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern == 2 &&
									(m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true || m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true))
								{// �p�^�[���̃G���A�������Q + �i�X�^�[�g������A�G���h������j
									bCheck = true;	// �`�F�b�N
								}
							} while (bCheck != false);
						}

						bool bCheck = false;	// �`�F�b�N
						do
						{// ���[�v
							bCheck = false;		// ������
							do
							{// ���[�v
								m_pWarp[nCntWarp].nArea_1 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_1].nMaxPattern;		// �G���A�ݒ�
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_END);		// �X�^�[�g�ƃG���h�ȊO�������甲����
							m_pWarp[nCntWarp].nArea_1 = m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1];		// �G���A�ԍ��ݒ�

							do
							{
								m_pWarp[nCntWarp].nArea_2 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern;		// �G���A�ݒ�
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_END);		// �X�^�[�g�ƃG���h�ȊO�������甲����
							m_pWarp[nCntWarp].nArea_2 = m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2];		// �G���A�ԍ��ݒ�

							for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
							{// ���[�v�J�E���g
								if (m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_2 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_2)
								{// �G���A������
									bCheck = true;		// �`�F�b�N
									break;
								}
							}
						} while (bCheck != false);
					}
				}
			}

			delete[] nRouteCounter;	// �j��
			delete[] nRouteEnd;		// �j��
		}
	}

	int nMaxVtx = m_pField->GetMaxVtx();	// ���_�擾

	// ���_�ʒu�ݒ�
	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 posVtx = m_pField->GetVtx(nCntVtx);

		/*if (posVtx.x < -1500.0f || 1500.0f < posVtx.x || posVtx.z < -1500.0f || 1500.0f < posVtx.z)
		{
		posVtx.y = 80.0f + (rand() % 80);

		m_pField->SetVtx(nCntVtx, posVtx);
		m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}
		else*/
		{
			posVtx.y = 80.0f + (rand() % 80);			// �ʒu�ݒ�

			// ���_�A�F�ݒ�
			m_pField->SetVtx(nCntVtx, posVtx);
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

			// �G���A
			if (m_pArea != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				{// �G���A�J�E���g
					// ���S����̈ʒu�v�Z
					float fRange = sqrtf(powf(m_pArea[nCount].pos.x - posVtx.x, 2.0f) + powf(m_pArea[nCount].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[nCount].fRange)
					{// �͈͓���������
						posVtx.y = 0.0f - (rand() % 15);	// �ʒu�ݒ�

						// ���_�A�F�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//if (m_pArea[nCount].nArea == AREA_START || m_pArea[nCount].nArea == AREA_END)
						//	m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

						/*if (m_pWarp != NULL)
						{
							for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
							{
								if (m_pWarp[nCntWarp].nArea_1 == nCount || m_pWarp[nCntWarp].nArea_2 == nCount)
								{
									if (nCntWarp == 0)
										m_pField->SetCol(nCntVtx, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
									else
										m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

									break;
								}
							}
						}*/

						break;
					}					
				}
			}

			// ���[�g
			if (m_pRoute != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ���[�g�J�E���g
					D3DXVECTOR3 vecA, vecC;		// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
					int nCheck = 0;				// �`�F�b�N
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						vecA = m_pRoute[nCount].pos[nCntPos] - m_pRoute[nCount].pos[(nCntPos + 1) % 4];
						vecC = posVtx - m_pRoute[nCount].pos[(nCntPos + 1) % 4];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
						{// �͈͓���
							nCheck++;	// �J�E���g
						}
					}

					if (nCheck == 4)
					{// �͈͓���������
						posVtx.y = 0.0f - (rand() % 15);	// �ʒu�ݒ�

						// ���_�A�F�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}

			// ���[�v
			if (m_pWarp != NULL)
			{
				for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
				{
					float fRange;
					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_1].fRange * 0.2f)
					{// �͈͓���������
						posVtx.y = 0.0f;		// �ʒu�ݒ�

						// ���_�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
					}

					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_2].fRange * 0.2f)
					{// �͈͓���������
						posVtx.y = 0.0f;		// �ʒu�ݒ�

						// ���_�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
					}
				}
			}
		}
	}
	m_pField->SetVecNor();	// �@���v�Z
}

//=============================================================================
// �I�u�W�F�N�g�ݒ菈��								(public)	*** CMap_02 ***
//=============================================================================
void CMap_02::SetObject(void)
{
	// ���[�v
	if (1 < m_nPatternMax)
	{// ���[�v�����݂���
		// ���[�v���f���v�Z
		m_nModelWarp = (m_nPatternMax - 1) * 2;

		// ���[�v���f��
		if (m_pModelWarp == NULL && m_ModelInfo != NULL)
		{// NULL�̏ꍇ
			m_pModelWarp = new CModel[m_nModelWarp];	// �������m��

			for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
			{// ���f�����J�E���g
				// �����l�ݒ�
				m_pModelWarp[nCntModel].SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_ModelInfo[0].pos, m_ModelInfo[0].rot);
				// ����������
				m_pModelWarp[nCntModel].Init();

				// �e���f����ݒ�
				m_pModelWarp[nCntModel].SetParent(NULL);

				if (m_pWarpCylinder[nCntModel] == NULL)
				{// NULL�ȊO�̏ꍇ
					m_pWarpCylinder[nCntModel] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);

					// �����l�ݒ�
					m_pWarpCylinder[nCntModel]->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 10.0f));
					// ����������
					m_pWarpCylinder[nCntModel]->Init();
					m_pWarpCylinder[nCntModel]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
					m_pWarpCylinder[nCntModel]->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
				}
			}
		}

		for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
		{// �p�^�[���J�E���g
			if (m_pModelWarp != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pModelWarp[0 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pModelWarp[1 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);
			}

			if (m_pWarpCylinder[0 + (nCntWarp * 2)] != NULL && m_pWarpCylinder[1 + (nCntWarp * 2)] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pWarpCylinder[0 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pWarpCylinder[1 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);

				if (nCntWarp == 0)
				{// �P��
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
					}
				}
				else if (nCntWarp == 1)
				{// �Q��
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
					}
				}
			}
		}
	}

	// �S�[��
	if (m_pGoalCylinder == NULL)
	{// NULL�̏ꍇ
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// �G���A�J�E���g
			if (nCntArea == AREA_END)
			{// �G���h
				m_pGoalCylinder = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GOAL);

				m_pGoalCylinder->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pArea[nCntArea].pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f), 
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 800.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				m_pGoalCylinder->Init();
				m_pGoalCylinder->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
				m_pGoalCylinder->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
			}
		}
	}

	// �A�C�R��
	if (m_pIcon == NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIcon = new CScene3D(CScene::PRIORITY_5, CScene::OBJTYPE_MAPICON);

		m_pIcon->SetInitAll(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		m_pIcon->Init();
		m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
		m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_ALWAYS);
		m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CScene		*pScene		= NULL;		// �V�[��
	CScene		*pSceneNext	= NULL;		// ���V�[��
	CSceneMesh	*pMesh		= NULL;		// ���b�V��
	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g
					break;
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	int nNumSet = 17;	// �G���A�Z�b�g��
	m_nGrassMax = m_nAreaMax * nNumSet;

	if (m_pGrass == NULL)
	{// NULL�̏ꍇ
		m_pGrass = new GRASS[m_nGrassMax];

		if (m_pGrass != NULL)
		{// NULL�ȊO�̏ꍇ
			for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
			{// �G���A�J�E���g
				for (int nCntGrass = 0; nCntGrass < nNumSet; nCntGrass++)
				{// ���J�E���g
					D3DXVECTOR3 pos;		// �ʒu
					float fAngle = (float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)) * 0.01f;
					float fLenght = (float)(rand() % (int)(m_pArea[nCntArea].fRange - 150.0f) + 100.0f);
					// �ʒu�v�Z
					pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght;

					int nNum = nCntArea * nNumSet + nCntGrass;

					// �����l�ݒ�
					m_pGrass[nNum].nCounter = 0;
					m_pGrass[nNum].pos = pos;
					m_pGrass[nNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
					{// ���J�E���g
						// �傫���Ɗp�x
						D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
						D3DXVECTOR3 rot = m_pGrass[nNum].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

						// �����Ɗp�x
						float fLenght = -5.0f + (float)(rand() % 10);
						float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);

						// ���̑�
						D3DXVECTOR3 posLeft = m_pGrass[nNum].pos +
							D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
							(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

						for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
						{// �J�E���g
							m_pGrass[nNum].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->Init();
							m_pGrass[nNum].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
							m_pGrass[nNum].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

							if (pMesh != NULL)
							{// NULL�ȊO�̏ꍇ
								D3DXVECTOR3 pos = m_pGrass[nNum].pGrass[nCount][nCntNum]->Getpos();
								pos.y = pMesh->GetHeight(pos);
								pos.y += 7.0f;
								m_pGrass[nNum].pGrass[nCount][nCntNum]->Setpos(pos);
							}

							m_pGrass[nNum].posCenter[nCount][nCntNum] = posLeft;

							D3DXVECTOR3 posVtx[4];
							m_pGrass[nNum].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
							m_pGrass[nNum].posVtx[nCount][nCntNum][0] = posVtx[0];
							m_pGrass[nNum].posVtx[nCount][nCntNum][1] = posVtx[1];

							m_pGrass[nNum].bFall[nCount][nCntNum] = false;
							m_pGrass[nNum].nFallCounter[nCount][nCntNum] = 0;

							posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
						}
					}
				}
			}
		}
	}
	
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// �G���A�J�E���g
		if (m_pArea[nCntArea].nArea != AREA_START)
		{
			// �G
			int nNum = rand() % 3 + 3;
			for (int nCount = 0; nCount < nNum; nCount++)
			{
				float fLenght;
				D3DXVECTOR3 pos;

				fLenght = m_pArea[nCntArea].fRange * 0.5f;
				pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f))), 0.0f, cosf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)))) * fLenght;

				CEnemy::Create(CEnemy::TYPE_N_SLIME, pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			}
		}
	}
}

//=============================================================================
// �t�B�[���h�Փ˔��菈��								(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange)
{
	bool bLand = false;
	bool bAreaCheck			= false;	// �G���A�`�F�b�N����
	bool bAreaCheckOld		= false;	// �G���A�`�F�b�N�O��
	bool bRouteCheck		= false;	// ���[�g�`�F�b�N����
	bool bRouteCheckOld		= false;	// ���[�g�`�F�b�N�O��
	int nAreaCheckNum		= 0;		// �G���A�`�F�b�N�ԍ�
	int nRouteCheckNum		= 0;		// ���[�g�`�F�b�N�ԍ�

	bool bAreaIn			= false;	// �G���A�̒�
	int nAreaInNum			= 0;		// �G���A�̒��ԍ�
	bool bRouteIn			= false;	// ���[�g�̒�
	int nRouteInNum			= 0;		// ���[�g�̒��ԍ�

	// �G���A�`�F�b�N
	if (bAreaCheck == false || bAreaCheckOld == false)
	{// �ǂ����FALSE
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// �G���A�J�E���g
			// �G���A�`�F�b�N
			bAreaCheck		= false;
			bAreaCheckOld	= false;
			nAreaCheckNum	= nCntArea;

			float fRange;	// �͈�
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPosOld->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPosOld->z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// �͈͓���
				bAreaCheckOld	= true;			// �͈͓�
				bAreaIn			= true;			// �G���A���ɂ���
				nAreaInNum		= nCntArea;		// �G���A���̔ԍ�
			}

			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (m_pArea[nCntArea].fRange * MC_AREACOLL_NOW <= fRange)
			{// �͈͊O��
				bAreaCheck = true;		// �͈͓�
			}

			// �ǂ����TRUE�Ȃ珈���𔲂���
			if (bAreaCheck == true && bAreaCheckOld == true) break;
		}
	}

	if (bRouteCheck == false || bRouteCheckOld == false)
	{// �ǂ����FALSE
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ���[�g�J�E���g
			// ���[�g�`�F�b�N
			bRouteCheck		= false;
			bRouteCheckOld	= false;
			nRouteCheckNum	= nCntRoute;

			int nHit	= 0;	// ���݃q�b�g��
			int nHitOld = 0;	// �O��q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// �O�όv�Z
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = *pPos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}

				// �O�όv�Z
				vecAOld = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecCOld = *pPosOld - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecAOld.z * vecCOld.x) - (vecAOld.x * vecCOld.z)) <= 0)
				{// �͈͓���
					nHitOld++;	// �J�E���g
				}
			}

			if (nHit != 4)
			{// �͈͊O��
				bRouteCheck = true;			// �͈͊O
			}
			if (nHitOld == 4)
			{// �͈͓���
				bRouteCheckOld	= true;			// �͈͓�
				bRouteIn		= true;			// ���[�g���ɂ���
				nRouteInNum		= nCntRoute;	// ���[�g���̔ԍ�
			}

			// �ǂ����TRUE�Ȃ珈���𔲂���
			if (bRouteCheck == true && bRouteCheckOld == true) break;
		}
	}

	bool bArea = false;		// �G���A����

	// �G���A����
	if (bAreaCheck == true && bAreaCheckOld == true)
	{// �ǂ����TRUE
		float fRouteAngle		= 0.0f;		// ���[�g�p�x
		float fRouteAngleRange	= 0.0f;		// ���[�g�p�x�͈�
		float fRouteAngleL		= 0.0f;		// ���[�g�p�xL
		float fRouteAngleR		= 0.0f;		// ���[�g�p�xR
		bool bRoute				= false;	// ���[�g�ɂ���

		// �G���A���S����v���C���[�̊p�x�v�Z
		float fAngle = atan2f(pPos->x - m_pArea[nAreaCheckNum].pos.x, pPos->z - m_pArea[nAreaCheckNum].pos.z);

		if (bRouteIn == true)
		{// ���[�g���ɂ���
			D3DXVECTOR3 posLei;
			if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posStart)
			{// �X�^�[�g�Ɠ�����������
				posLei = m_pRoute[nRouteInNum].posCol[0];	// ���C�p�ʒu

				bool bCheck = false;	// �`�F�b�N
				while (true)
				{// ���[�v
					// 
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{
						break;
					}
					else
					{
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle)) * 1.0f;
					}
				}
			}
			else if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posEnd)
			{// �G���h�Ɠ�����������
				posLei = m_pRoute[nRouteInNum].posCol[2];	// ���C�p�ʒu

				bool bCheck = false;	// �`�F�b�N
				while (true)
				{// ���[�v
					// ���S����̈ʒu�v�Z
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{// �͈͊O
						break;
					}
					else
					{// �͈͓�
						// ���C��i�߂�
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle + D3DX_PI), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle + D3DX_PI)) * 1.0f;
					}
				}
			}

			// ���[�g�p�x�v�Z
			fRouteAngle = atan2f(m_pRoute[nRouteInNum].posCenter.x - m_pArea[nAreaCheckNum].pos.x, m_pRoute[nRouteInNum].posCenter.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = atan2f(posLei.x - m_pArea[nAreaCheckNum].pos.x, posLei.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = fRouteAngle - fRouteAngleRange;
			fRouteAngleRange = fabsf(fRouteAngleRange);

			if (fRouteAngleRange < -D3DX_PI)
				fRouteAngleRange += D3DX_PI * 2.0f;
			if (D3DX_PI < fRouteAngleRange)
				fRouteAngleRange -= D3DX_PI * 2.0f;

			fRouteAngleRange = fabsf(fRouteAngleRange);

			fRouteAngleL = fRouteAngle - fRouteAngleRange;
			fRouteAngleR = fRouteAngle + fRouteAngleRange;

			if (fRouteAngleL < fAngle && fAngle < fRouteAngleR)
			{
				bRoute = true;
			}
			else if (fRouteAngleL < -D3DX_PI)
			{
				if (fRouteAngleL < fAngle + (-D3DX_PI * 2.0f) && fAngle + (-D3DX_PI * 2.0f)  < fRouteAngleR)
				{
					bRoute = true;
				}
			}
			else if (D3DX_PI < fRouteAngleR)
			{
				if (fRouteAngleL < fAngle + (D3DX_PI * 2.0f) && fAngle + (D3DX_PI * 2.0f)  < fRouteAngleR)
				{
					bRoute = true;
				}
			}
		}

		if (bRouteIn == false || (bRouteIn == true && bRoute == false))
		{// ���[�g���ɂ��Ȃ��A�i���[�g���ɂ��� + ���[�g�͈͊O�j
			pPos->x = m_pArea[nAreaCheckNum].pos.x + sinf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);
			pPos->z = m_pArea[nAreaCheckNum].pos.z + cosf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);

			pMove->x = 0.0f;
			pMove->z = 0.0f;
			bArea = true;	// �G���A����
			bLand = true;
		}
	}

	// ���[�g����
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteCheck == true && bRouteCheckOld == true)
		{// �ǂ����TRUE
			// �Z���^�[�v�Z
			float fAngleCenter	= 0.0f;
			float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteCheckNum].posStart.x - m_pRoute[nRouteCheckNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteCheckNum].posStart.z - m_pRoute[nRouteCheckNum].pos[0].z, 2.0f));

			// �O�όv�Z
			D3DXVECTOR3 vecALR, vecCLR;
			vecALR = m_pRoute[nRouteCheckNum].posStart - m_pRoute[nRouteCheckNum].posEnd;
			vecCLR = *pPos - m_pRoute[nRouteCheckNum].posEnd;

			// ���[�g�̉E����������
			if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle - (D3DX_PI * 0.5f);	// ����
			else
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle + (D3DX_PI * 0.5f);	// �E��

			// �Z���^�[�v�Z
			D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

			// �Z���^�[�ւ̊p�x�v�Z
			float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

			int nHit = 0;	// �q�b�g��

			while (1)
			{
				nHit = 0;	// �q�b�g��
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
				{// �ʒu�J�E���g
					// �O�όv�Z
					D3DXVECTOR3 vecA, vecC;
					vecA = m_pRoute[nRouteCheckNum].posCol[nCntPos] - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
					vecC = *pPos - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

					if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
					{// �͈͓�
						nHit++;
					}
				}

				if (nHit == 4)
				{// �͈͓�
					bLand = true;
					break;		// �����𔲂���
				}
				else
				{// �͈͊O
					// �ʒu���X�V�i�߂Â���j
					pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
					pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
				}
			}

			pMove->x = 0.0f;
			pMove->z = 0.0f;
		}
	}

	// ���̑��̔���i�������ꍇ�j
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteIn == false)
		{// ���[�g�ɂ��Ȃ�
			if (bRouteCheck == true && bRouteCheckOld == false)
			{// ���݂��͈͊O + �O�񂪔͈͊O
				int nRouteNum = 0;		// ���[�g�ԍ�

				for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
				{// ���[�g�J�E���g
					int nHit = 0;	// �q�b�g��

					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
						vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// �͈͓�
							nHit++;		// �J�E���g
						}
					}

					if (nHit == 4)
					{// �͈͓�
						nRouteNum = nCntRoute;		// �ԍ����L��
						break;
					}
				}

				// �Z���^�[�v�Z
				float fAngleCenter	= 0.0f;
				float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteNum].posStart.x - m_pRoute[nRouteNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteNum].posStart.z - m_pRoute[nRouteNum].pos[0].z, 2.0f));

				// �O�όv�Z
				D3DXVECTOR3 vecALR, vecCLR;
				vecALR = m_pRoute[nRouteNum].posStart - m_pRoute[nRouteNum].posEnd;
				vecCLR = *pPos - m_pRoute[nRouteNum].posEnd;

				// ���[�g�̉E����������
				if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
					fAngleCenter = m_pRoute[nRouteNum].fAngle - (D3DX_PI * 0.5f);	// ����
				else
					fAngleCenter = m_pRoute[nRouteNum].fAngle + (D3DX_PI * 0.5f);	// �E��

				// �Z���^�[�v�Z
				D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

				// �Z���^�[�ւ̊p�x�v�Z
				float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

				int nHit = 0;	// �q�b�g��

				while (1)
				{
					nHit = 0;	// �q�b�g��
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						D3DXVECTOR3 vecA, vecC;
						vecA = m_pRoute[nRouteNum].posCol[nCntPos] - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// �͈͓�
							nHit++;
						}
					}

					if (nHit == 4)
					{// �͈͓�
						bLand = true;
						break;		// �����𔲂���
					}
					else
					{// �͈͊O
						// �ʒu���X�V�i�߂Â���j
						pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
						pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
					}
				}

				pMove->x = 0.0f;
				pMove->z = 0.0f;
			}
		}
	}

	return bLand;
}

//=============================================================================
// �G���A���[�g���ɂ��邩���菈��						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionInOut(D3DXVECTOR3 pos)
{
	bool bLand = false;

	if (bLand == false)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// �G���A�J�E���g
			float fRange;	// �͈�
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pos.z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// �͈͓���
				bLand = true;
				break;
			}
		}
	}

	if (bLand == false)
	{
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ���[�g�J�E���g
			int nHit = 0;	// ���݃q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC;
				// �O�όv�Z
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = pos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}
			}

			if (nHit == 4)
			{// �͈͓���
				bLand = true;
				break;
			}
		}
	}

	return bLand;
}

//=============================================================================
// �t�B�[���h���[�v�Փ˔��菈��						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp)
{
	int nWarpNow = 0;
	int nWarpNext = 0;
	bool bUseWarp = false;;

	for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
	{
		float fRange;
		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNow = m_pWarp[nCntWarp].nArea_1;
			nWarpNext = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}

		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNext = m_pWarp[nCntWarp].nArea_1;
			nWarpNow = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}
	}

	if (bUseWarp == true)
	{
		*pPosWarp = m_pArea[nWarpNext].pos;

		if (m_pIcon != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nWarpNow].pos.x, 100.0f, m_pArea[nWarpNow].pos.z));
			m_bDrawIcon = true;			// �A�C�R���`��
		}
	}

	return bUseWarp;
}

//=============================================================================
// �t�B�[���h�S�[���Փ˔��菈��						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bUseGoal = false;;

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{
		if (nCntArea == AREA_END)
		{
			float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (fRange < 35.0f)
			{
				bUseGoal = true;

				if (m_pIcon != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nCntArea].pos.x, 100.0f, m_pArea[nCntArea].pos.z));
					m_bDrawIcon = true;			// �A�C�R���`��
				}

				break;
			}
		}
	}

	return bUseGoal;
}

//=============================================================================
// �t�B�[���h�J�����Փ˔��菈��						(public)	*** CMap_02 ***
//=============================================================================
bool CMap_02::CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	bool bAreaIn	= false;	// �G���A�̒�
	int nAreaInNum	= 0;		// �G���A�̒��ԍ�
	bool bRouteIn	= false;	// ���[�g�̒�
	int nRouteInNum = 0;		// ���[�g�̒��ԍ�
	bool bAreaInV	= false;	// ���_���[�g�̒�
	bool bRouteInV	= false;	// ���_���[�g�̒�

	// �G���A�`�F�b�N
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// �G���A�J�E���g
		float fRange;	// �͈�
		fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - posR.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - posR.z, 2.0f));
		if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
		{// �͈͓���
			bAreaIn		= true;			// �G���A���ɂ���
			nAreaInNum	= nCntArea;		// �G���A���̔ԍ�
			break;
		}
	}

	// ���[�g�`�F�b�N
	for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
	{// ���[�g�J�E���g
		int nHit = 0;	// ���݃q�b�g��
		int nHitV = 0;	// ���݃q�b�g��

		for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
		{// �ʒu�J�E���g
			D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
			// �O�όv�Z
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posR - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// �͈͓���
				nHit++;		// �J�E���g
			}

			// �O�όv�Z
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posV - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// �͈͓���
				nHitV++;		// �J�E���g
			}
		}

		if (nHit == 4)
		{// �͈͓���
			bRouteIn	= true;			// �͈͊O
			nRouteInNum = nCntRoute;	// �G���A���̔ԍ�

			if (nHitV == 4)
			{// �͈͓���
				bRouteInV = true;			// �͈͊O
			}

			break;
		}


	}

	bool bArea = false;		// �G���A����

	// �G���A����
	if (bAreaIn == true)
	{// �G���A���ɂ���
		// �������v�Z
		float fRange = sqrtf(powf(m_pArea[nAreaInNum].pos.x - posV.x, 2.0f) + powf(m_pArea[nAreaInNum].pos.z - posV.z, 2.0f));
		if (fRange < m_pArea[nAreaInNum].fRange)
		{// �͈͓�
			return true;
		}
		else
		{// �͈͊O
			if (bRouteInV == true)
			{// ���_�����[�g���ɂ���
				return true;
			}
			else
			{// ���_�����[�g���ɂ��Ȃ�
				return false;
			}
		}
	}

	// ���[�g����
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteIn == true)
		{// ���[�g���ɂ���
			int nHit = 0;	// ���݃q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// �O�όv�Z
				vecA = m_pRoute[nRouteInNum].posCol[nCntPos] - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = posV - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}
			}

			if (nHit == 4)
			{// �͈͓���
				return true;
			}
			else
			{// �͈͊O
				return false;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CMap_03::m_nMaxModel		= 0;		// ���f����

LPDIRECT3DTEXTURE9		**CMap_03::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CMap_03::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CMap_03::m_pBuffMat	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CMap_03::m_nNumMat		= NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CMap_03::m_ModelInfo	= NULL;		// ���f���C���t�H

//=============================================================================
// �R���X�g���N�^										(public)	*** CMap_03 ***
//=============================================================================
CMap_03::CMap_03() : CMap()
{
	m_nModelWarp = 0;									// ���f�����[�v��
	m_pModelWarp = NULL;								// ���f�����[�v

	m_nModelObject = 0;									// ���f���I�u�W�F�N�g��
	m_pModelObject = NULL;								// ���f���I�u�W�F�N�g

	m_pField = NULL;									// �t�B�[���h

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		m_pSky[nCount] = NULL;							// NULL��
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// ��]
	}

	m_nGrassMax = 0;									// �ő呐
	m_pGrass = NULL;									// ��

	m_nAreaMax = 0;										// �G���A�ő吔

	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaNum[nCount] = 0;							// �G���A��

	m_pArea = NULL;										// �G���A

	m_nRouteMax = 0;									// ���[�g�ő吔
	m_pRoute = NULL;									// ���[�g

	m_nPatternMax = 0;									// �p�^�[���ő吔
	m_pPattern = NULL;									// �p�^�[��

	m_pWarp = NULL;										// ���[�v

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
		m_pWarpCylinder[nCount] = NULL;					// ���[�v�V�����_�[
	m_WarpCylinderUV = D3DXVECTOR2(0.0f, 0.0f);			// ���[�v�V�����_�[UV

	m_pGoalCylinder = NULL;								// �S�[���V�����_�[

	m_bDrawIcon = false;								// �A�C�R���`��
	m_pIcon = NULL;										// �A�C�R��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMap_03 ***
//=============================================================================
CMap_03::~CMap_03()
{

}

//=============================================================================
// ���[�h����										(public)	*** CMap_03 ***
//=============================================================================
HRESULT CMap_03::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\gamemap.txt", &m_nMaxModel);		// ���f���擾

	// �e���̃������m��
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_nMaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_nMaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_nMaxModel];

	// �e���̏�����
	for (int nCount = 0; nCount < m_nMaxModel; nCount++)		// ���f�����J�E���g
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_nMaxModel; nCntParnet++)
	{// ���f�����J�E���g
		// X�t�@�C���̓ǂݍ���
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Unload(void)
{
	// �e���̃������J���ANULL��
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// ���f���C���t�H
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULL��
}

//=============================================================================
// ����������										(public)	*** CMap_03 ***
//=============================================================================
HRESULT CMap_03::Init(void)
{
	// ���b�V���t�B�[���h
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3600.0f, 0.0f, 3600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(24.0f, 24.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	// ��P
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// ��Q
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Uninit(void)
{
	// ���f��
	if (m_pModelWarp != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++) // ���f�����J�E���g
			m_pModelWarp[nCntModel].Uninit();	// �I������

		delete[] m_pModelWarp;		// �������J��
		m_pModelWarp = NULL;		// NULL��
	}
	if (m_pModelObject != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++) // ���f�����J�E���g
			m_pModelObject[nCntModel].Uninit();	// �I������

		delete[] m_pModelObject;	// �������J��
		m_pModelObject = NULL;		// NULL��
	}

	// �t�B�[���h
	if (m_pField != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pField->Uninit();		// �I������
		m_pField = NULL;		// NULL��
	}

	// ��
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		if (m_pSky[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pSky[nCount]->Uninit();	// �I������
			m_pSky[nCount] = NULL;		// NULL��
		}
	}

	// ��
	if (m_pGrass != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// ���J�E���g
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// �I������
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULL��
				}
			}
		}

		delete[] m_pGrass;	// �������J��
		m_pGrass = NULL;		// NULL��
	}

	// �}�b�v
	if (m_pArea != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pArea;		// �j��
		m_pArea = NULL;			// NULL��
	}
	if (m_pRoute != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pRoute;		// �j��
		m_pRoute = NULL;		// NULL��
	}
	if (m_pPattern != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nPatternMax; nCount++)
		{// �G���A�J�E���g
			delete[] m_pPattern[nCount].nPattern;
			m_pPattern[nCount].nPattern = NULL;
		}

		delete[] m_pPattern;	// �j��
		m_pPattern = NULL;		// NULL��
	}
	if (m_pWarp != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pWarp;		// �j��
		m_pWarp = NULL;			// NULL��
	}

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
	{
		if (m_pWarpCylinder[nCount] != NULL)
		{
			m_pWarpCylinder[nCount]->Uninit();
			m_pWarpCylinder[nCount] = NULL;
		}
	}

	// �S�[���V�����_�[
	if (m_pGoalCylinder != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pGoalCylinder->Uninit();
		m_pGoalCylinder = NULL;
	}

	// �A�C�R��
	if (m_pIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}

	CMap::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Update(void)
{
	// ��0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// ��]

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// �e�N�X�`���ݒ�
	}

	// ��1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// ��]

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// �e�N�X�`���ݒ�
	}

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->Getpos();
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 CameraposR = pCamera->Getpos(1);
	D3DXVECTOR3 Camerarot = pCamera->Getrot();

	if (m_pGrass != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{// �ő吔�J�E���g
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// ���J�E���g
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{// ���C���J�E���g
					D3DXVECTOR3 posVtx[4];		// ���_�󂯎��
					D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// ���_���擾

																							// �J�����͈�
					float fCameraL = Camerarot.y - (D3DX_PI * 0.5f);
					float fCameraR = Camerarot.y + (D3DX_PI * 0.5f);

					// �͈͒���
					if (fCameraL < -D3DX_PI)
						fCameraL += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraL)
						fCameraL -= D3DX_PI * 2.0f;

					if (fCameraR < -D3DX_PI)
						fCameraR += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraR)
						fCameraR -= D3DX_PI * 2.0f;

					float fCameraAngle = atan2f(posCenter.x - CameraposR.x, posCenter.z - CameraposR.z);

					if (Camerarot.y < (-D3DX_PI * 0.5f) || (D3DX_PI * 0.5f) < Camerarot.y)
					{
						if (fCameraR < fCameraAngle && fCameraAngle < fCameraL)
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}
					else
					{
						if (fCameraL < fCameraAngle && fCameraAngle < fCameraR)
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}

					float fRange;
					fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));
					if (fRange < 850.0f)
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
					}
					else
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
					}

					bool bCheck = false;
					if (fRange < 30.0f)
					{
						bCheck = true;
					}

					if (bCheck == true)
					{
						float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
						D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
						fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
						if (fRange < 20.0f)
						{
							posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
						}

						m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
					}
					else
					{
						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
						{
							posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
							posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
							posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
							posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
							m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

							if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
							{
								m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
							}
						}
					}

					if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
					{
						if (m_pGrass->nCounter < 120)
						{// ���J�E���g���Ⴂ
							posVtx[0].x += sinf(0.0f) * 0.023f;
							posVtx[0].x += cosf(0.0f) * 0.023f;
							posVtx[1].x += sinf(0.0f) * 0.023f;
							posVtx[1].x += cosf(0.0f) * 0.023f;
						}
						else
						{// ���J�E���g�ȏ�
							posVtx[0].x -= sinf(0.0f) * 0.023f;
							posVtx[0].x -= cosf(0.0f) * 0.023f;
							posVtx[1].x -= sinf(0.0f) * 0.023f;
							posVtx[1].x -= cosf(0.0f) * 0.023f;
						}
					}
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// ���_���ݒ�
				}
			}

			{
				m_pGrass[nCntGrass].nCounter++;		// �J�E���g����

				if (m_pGrass[nCntGrass].nCounter == 240)
				{// �w�肵���J�E���g�ɂȂ�����
					m_pGrass[nCntGrass].nCounter = 0;	// �J�E���g������
				}
			}
		}
	}

	m_WarpCylinderUV.y -= 0.01f;
	for (int nCntWarp = 0; nCntWarp < m_nModelWarp; nCntWarp++)
	{
		if (m_pWarpCylinder[nCntWarp] != NULL)
		{
			m_pWarpCylinder[nCntWarp]->SetTex(m_WarpCylinderUV);
		}
	}

	if (m_pIcon != NULL)
	{
		D3DXCOLOR col = m_pIcon->Getcol();
		if (m_bDrawIcon == true)
		{
			if (col.a != 1.0f)
			{
				col.a += 0.05f;
				if (1.0f < col.a) col.a = 1.0f;
			}
		}
		else
		{
			if (col.a != 0.0f)
			{
				col.a -= 0.05f;
				if (col.a < 0.0f) col.a = 0.0f;
			}
		}
		m_pIcon->Setcol(col);
	}
	m_bDrawIcon = false;
}

//=============================================================================
// �`�揈��											(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	if (1 < m_nPatternMax)
	{
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
		{// ���f�����J�E���g
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			if (m_pModelWarp != NULL)
			{// NULL�ȊO�̏ꍇ
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				m_pModelWarp[nCntModel].Draw();

				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

				// �e�\��
				//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
			}
		}
	}

	for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
	{// ���f�����J�E���g
	 // ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		if (m_pModelObject != NULL)
		{// NULL�ȊO�̏ꍇ
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModelObject[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// �e�\��
			//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
		}
	}
}

//=============================================================================
// �t�B�[���h�ݒ菈��									(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::SetField(D3DXVECTOR3 pos)
{
	// �G���A��
	m_nAreaNum[AREA_START]	= 1;
	m_nAreaNum[AREA_END]	= 1;
	m_nAreaNum[AREA_NORMAL]	= 0;
	m_nAreaNum[AREA_BATTLE]	= 1;
	m_nAreaNum[AREA_ITEM]	= 0;

	// �G���A�����v�Z
	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaMax += m_nAreaNum[nCount];

	// �������m��
	if (m_pArea == NULL) m_pArea = new AREA_RANGE[m_nAreaMax];

	// �G���A������
	for (int nCount = 0; nCount < m_nAreaMax; nCount++)
	{// �G���A�J�E���g
		m_pArea[nCount].nArea	= AREA_START;						// �G���A
		m_pArea[nCount].fRange	= 0.0f;								// �͈�
		m_pArea[nCount].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���S
	}

	// �G���A�A���[�g����
	{
		// �G���A
		{
			int nCntArea = 0;		// �G���A�ԍ�
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
			{// �G���A�J�E���g
				for (int nCntNum = 0; nCntNum < m_nAreaNum[nCount]; nCntNum++)
				{// �G���A���J�E���g
					int nCheckCounter = 0;	// �`�F�b�N�J�E���g
					bool bCheck = false;	// �`�F�b�N
					do
					{// ���[�v
						bCheck = false;

						switch (nCount)
						{// �G���A
						case AREA_START:	// �X�^�[�g
							m_pArea[nCntArea].nArea = AREA_START;		// �G���A
							m_pArea[nCntArea].fRange = 280.0f;			// �͈�
							m_pArea[nCntArea].pos	= pos;				// �ʒu
							break;
						case AREA_END:		// �G���h
							m_pArea[nCntArea].nArea = AREA_END;			// �G���A
							m_pArea[nCntArea].fRange = 280.0f;			// �͈�

							// �ʒu
							{
								bool bRange = false;
								do
								{// ���[�v
									m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
									m_pArea[nCntArea].pos.y = 0.0f;
									m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));

									// ���S����̋������v�Z
									float fRange = sqrtf(powf(m_pArea[0].pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(m_pArea[0].pos.z - m_pArea[nCntArea].pos.z, 2.0f));

									if (fRange < 1000.0f)
									{
										bCheck = true;
									}

									if (-1000.0f < m_pArea[nCntArea].pos.x && m_pArea[nCntArea].pos.x < 1000.0 &&
										-1000.0f < m_pArea[nCntArea].pos.z && m_pArea[nCntArea].pos.z < 1000.0f)
									{
										bCheck = true;
									}
								} while (bRange != false);
							}
							break;
						case AREA_NORMAL:	// �m�[�}��
						case AREA_BATTLE:	// �o�g��
							// �G���A
							if (nCount == AREA_NORMAL)	m_pArea[nCntArea].nArea = AREA_NORMAL;
							if (nCount == AREA_BATTLE)	m_pArea[nCntArea].nArea = AREA_BATTLE;
							m_pArea[nCntArea].fRange = (float)(rand() % 100 + 580);			// �͈�

							// �ʒu
							m_pArea[nCntArea].pos.x = (float)((rand() % ((900 - (int)m_pArea[nCntArea].fRange) * 2)) - (900 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((900 - (int)m_pArea[nCntArea].fRange) * 2)) - (900 - (int)m_pArea[nCntArea].fRange));
							break;
						case AREA_ITEM:		// �A�C�e��
							m_pArea[nCntArea].nArea = AREA_ITEM;			//�@�G���A
							m_pArea[nCntArea].fRange = (float)(rand() % 50 + 320);			// �͈�

							// �ʒu
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						}

						// �G���A���m�d�Ȃ��Ă��Ȃ���
						for (int nCntCheck = 0; nCntCheck < nCntArea; nCntCheck++)
						{// �G���A�J�E���g
							// ���S����̈ʒu�v�Z
							float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - m_pArea[nCntCheck].pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - m_pArea[nCntCheck].pos.z, 2.0f));
							if (fRange < m_pArea[nCntArea].fRange + m_pArea[nCntCheck].fRange + 100.0f)
							{// �G���A���d�Ȃ��Ă��Ȃ���
								bCheck = true;		// �d�Ȃ��Ă���
								nCheckCounter++;	// �J�E���g
								break;
							}
						}

						if (nCheckCounter == 30) break;
					} while (bCheck != false);
					
					if (nCheckCounter != 30)
						nCntArea++;		// �G���A�J�E���g
				}
			}
			m_nAreaMax = nCntArea;
		}

		// ���[�g
		{
			// ���[�g��
			m_nRouteNum[AREA_START]	= m_nAreaNum[AREA_START];
			m_nRouteNum[AREA_END]	= m_nAreaNum[AREA_END];
			m_nRouteNum[AREA_NORMAL] = m_nAreaNum[AREA_NORMAL];
			m_nRouteNum[AREA_BATTLE] = m_nAreaNum[AREA_BATTLE];
			m_nRouteNum[AREA_ITEM]	= m_nAreaNum[AREA_ITEM];

			// ���[�g�����v�Z
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
				m_nRouteMax += m_nRouteNum[nCount];

			// �������m��
			if (m_pRoute == NULL) m_pRoute = new ROUTE_RANGE[m_nRouteMax];

			// ���[�g������
			for (int nCount = 0; nCount < m_nRouteMax; nCount++)
			{// ���[�g�J�E���g
				m_pRoute[nCount].nAreaNumStart = 0;									// �X�^�[�g�G���A�ԍ�
				m_pRoute[nCount].nAreaNumEnd = 0;									// �G���h�G���A�ԍ�
				m_pRoute[nCount].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �X�^�[�g�G���A�ʒu
				m_pRoute[nCount].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �G���h�G���A�ʒu
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					m_pRoute[nCount].pos[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g���_�ʒu
				m_pRoute[nCount].fAngle = 0.0f;										// ���[�g�p�x
				m_pRoute[nCount].fLenght = 0.0f;										// ���[�g����
				m_pRoute[nCount].fRange = 0.0f;										// ���[�g�͈́i�����j
			}

			int nRoute = 0;		// ���[�g�ԍ�
			for (int nCountArea = 0; nCountArea < m_nAreaMax; nCountArea++)
			{// �G���A�J�E���g
				int nAreaNum = nCountArea;		// �G���A�ԍ�
				float fAreaLenght = 5000.0f;	// ����

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// �G���A�J�E���g
					if (nCountArea != nCntAreaCheck && 
						(nCountArea != AREA_START || (nCountArea == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nCountArea != AREA_END || (nCountArea == AREA_END && nCntAreaCheck != AREA_START)))
					{// �����G���A�łȂ��A�X�^�[�g�ƃG���h���q���Ȃ�
						// ���S����̈ʒu�v�Z
						float fRange = sqrtf(powf(m_pArea[nCountArea].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nCountArea].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// ���������߂�������
							nAreaNum = nCntAreaCheck;	// �ԍ�
							fAreaLenght = fRange;		// ����
						}
					}
				}

				bool bCheck = false;	// �`�F�b�N
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ���[�g�J�E���g
					if ((m_pRoute[nCntRoute].nAreaNumStart == nCountArea && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nCountArea))
					{// �������[�g���Ȃ���
						bCheck = true;	// ����
					}
				}

				if (bCheck == false)
				{// ��������
					// �͈́i�����j
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// ����
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// �p�x
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nCountArea].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nCountArea].pos.z);

					// �ʒu
					m_pRoute[nRoute].nAreaNumStart	= nCountArea;				// �X�^�[�g�ԍ�
					m_pRoute[nRoute].nAreaNumEnd	= nAreaNum;					// �G���h�ԍ�
					m_pRoute[nRoute].posStart		= m_pArea[nCountArea].pos;	// �X�^�[�g�ʒu
					m_pRoute[nRoute].posEnd			= m_pArea[nAreaNum].pos;	// �G���h�ʒu

					// ���S���v�Z
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// ���_�v�Z
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// ����p�̒��_�v�Z
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ���[�g�J�E���g
					if (nRoute == m_nRouteMax) break;
				}
			}

			// �e�G���A�Ƀ��[�g�����{�q��������
			int *nRouteCounter = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteCounter[nCount] = 0;

			for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
			{// ���[�g�J�E���g
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumStart]++;
				nRouteCounter[m_pRoute[nCntRoute].nAreaNumEnd]++;
			}

			// ���[�̃G���A���v�Z
			int nRouteEndNum = 0;
			int *nRouteEnd = new int[m_nAreaMax];
			for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				nRouteEnd[nCount] = 0;

			// �q���ꂽ���[�g��
			int nRoute_0 = 0;	
			int nRoute_1 = 0;
			int nRoute_2 = 0;

			for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
			{// ���[�g�J�E���g
				nRouteEnd[nRouteEndNum] = nCntRoute;	// �ԍ��L��
				nRouteEndNum++;							// �J�E���g

				if (nRouteCounter[nCntRoute] == 0)
					nRoute_0++;									// ���[�g���O
				else if (nRouteCounter[nCntRoute] == 1)
					nRoute_1++;									// ���[�g���P
				else if (1 < nRouteCounter[nCntRoute])
					nRoute_2 += nRouteCounter[nCntRoute] - 1;	// ���[�g���Q
			}

			// �p�^�[�������v�Z
			int nRoutePattern = nRoute - nRoute_2 + nRoute_0;

			if (nRoute_0 != 0)
			{// ���[�g
				int nNumber = 0;
				for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
				{// ���[�g�J�E���g
					if (nRouteCounter[nCntRoute] == 0)
					{// �q�����Ă��Ȃ�
						nNumber = nCntRoute;
						break;
					}
				}

				int nAreaNum = nNumber;			// �G���A�ԍ�
				float fAreaLenght = 5000.0f;	// ����

				for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
				{// �G���A�J�E���g
					if (nNumber != nCntAreaCheck &&
						(nNumber != AREA_START || (nNumber == AREA_START && nCntAreaCheck != AREA_END)) &&
						(nNumber != AREA_END || (nNumber == AREA_END && nCntAreaCheck != AREA_START))) {// �����G���A�łȂ��A�X�^�[�g�ƃG���h���q���Ȃ�
						// ���S����̈ʒu�v�Z
						float fRange = sqrtf(powf(m_pArea[nNumber].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nNumber].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
						if (fRange < fAreaLenght)
						{// ���������߂�������
							nAreaNum = nCntAreaCheck;	// �ԍ�
							fAreaLenght = fRange;		// ����
						}
					}
				}

				bool bCheck = false;	// �`�F�b�N
				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ���[�g�J�E���g
					if ((m_pRoute[nCntRoute].nAreaNumStart == nRouteEnd[nCntRoute] && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
						(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nRouteEnd[nCntRoute]))
					{// �������[�g���Ȃ���
						bCheck = true;	// ����
					}
				}

				if (bCheck == false)
				{// ��������
					// �͈́i�����j
					m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

					// ����
					m_pRoute[nRoute].fLenght = fAreaLenght;

					// �p�x
					m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nNumber].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nNumber].pos.z);

					// �ʒu
					m_pRoute[nRoute].nAreaNumStart = nNumber;
					m_pRoute[nRoute].nAreaNumEnd = nAreaNum;
					m_pRoute[nRoute].posStart = m_pArea[nNumber].pos;
					m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;

					// ���S���v�Z
					m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

					// ���_�v�Z
					m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
					m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

					// ����p�̒��_�v�Z
					m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
					m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
					m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
					m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

					nRoute++;	// ���[�g�J�E���g
					nRoutePattern--;
				}
			}

			m_nRouteMax = nRoute;	// ���[�g���J�E���g

			m_nPatternMax = nRoutePattern;		// �p�^�[�������L��

			// �������m��
			if (m_pPattern == NULL)
				m_pPattern = new ROUTE_PATTERN[m_nPatternMax];

			if (m_pPattern != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCntPattert = 0; nCntPattert < m_nPatternMax; nCntPattert++)
				{// �p�^�[�����J�E���g
					// �����l�ݒ�
					m_pPattern[nCntPattert].nMaxPattern = 0;
					m_pPattern[nCntPattert].nPattern = new int[m_nAreaMax];
					m_pPattern[nCntPattert].bStart = false;
					m_pPattern[nCntPattert].bEnd = false;

					if (nCntPattert == 0)
					{// �J�E���g���O��������
						m_pPattern[nCntPattert].nPattern[0] = m_pRoute[0].nAreaNumStart;
						m_pPattern[nCntPattert].nPattern[1] = m_pRoute[0].nAreaNumEnd;
						m_pPattern[nCntPattert].nMaxPattern = 2;
					}
					else
					{// �J�E���g���O�ȊO��������
						int nNumber = 0;		// �ԍ��m�F
						bool bCheck = false;	// �`�F�b�N
						// �����ԍ��ɂȂ�Ȃ��悤����
						do
						{// ���[�v
							bCheck = false;		// ������
							for (int nCntCheck = 0; nCntCheck < nCntPattert; nCntCheck++)
							{// �p�^�[���J�E���g
								for (int nCntArea = 0; nCntArea < m_pPattern[nCntCheck].nMaxPattern; nCntArea++)
								{// �G���A�J�E���g
									if (m_pPattern[nCntCheck].nPattern[nCntArea] == nNumber)
									{// ������������
										nNumber++;		// �J�E���g
										bCheck = true;	// �`�F�b�N
										break;
									}
								}
								if (bCheck == true) break;
							}
						} while (bCheck != false);

						// �ԍ��ɂȂ��郋�[�g������
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ���[�g�J�E���g
							if (nNumber == m_pRoute[nCntRoute].nAreaNumStart || nNumber == m_pRoute[nCntRoute].nAreaNumEnd)
							{// �ԍ�������
								m_pPattern[nCntPattert].nPattern[0] = m_pRoute[nCntRoute].nAreaNumStart;
								m_pPattern[nCntPattert].nPattern[1] = m_pRoute[nCntRoute].nAreaNumEnd;
								m_pPattern[nCntPattert].nMaxPattern = 2;
								break;
							}
						}
					}

					for (int nCntPattertNum = 0; nCntPattertNum < m_pPattern[nCntPattert].nMaxPattern; nCntPattertNum++)
					{// �p�^�[���ɂ���G���A���J�E���g
						for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
						{// ���[�g�J�E���g
							if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumStart)
							{// �X�^�[�g�ԍ��Ɠ���
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// �p�^�[���ɂ���G���A���J�E���g
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumEnd)
									{// �G���h�ԍ��Ɠ���
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// �����ԍ����Ȃ�������
										// �ԍ��L��
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumEnd;
										m_pPattern[nCntPattert].nMaxPattern++;	// �p�^�[���̃G���A���𑝂₷
									}
								}
							}
							else if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumEnd)
							{// �G���h�ԍ��Ɠ���
								for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
								{// �p�^�[���ɂ���G���A���J�E���g
									if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumStart)
									{// �X�^�[�g�ԍ��Ɠ���
										break;
									}

									if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
									{// �����ԍ����Ȃ�������
										// �ԍ��L��
										m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumStart;
										m_pPattern[nCntPattert].nMaxPattern++;	// �p�^�[���̃G���A���𑝂₷
									}
								}
							}
						}
					}
				}
			}

			for (int nCntPattern = 0; nCntPattern < m_nPatternMax; nCntPattern++)
			{// �p�^�[�����J�E���g
				for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattern].nMaxPattern; nCntCheck++)
				{// �p�^�[���ɂ���G���A���J�E���g
					if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_START)
					{// �X�^�[�g�ԍ��Ɠ���
						m_pPattern[nCntPattern].bStart = true;		// �X�^�[�g������
					}
					else if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_END)
					{// �G���h�ԍ��Ɠ���
						m_pPattern[nCntPattern].bEnd = true;		// �G���h������
					}
				}
			}

			if (1 < m_nPatternMax)
			{// �p�^�[�������P��葽��
				// �������m��
				if (m_pWarp == NULL)
					m_pWarp = new WARP[m_nPatternMax - 1];

				if (m_pWarp != NULL)
				{// NULL�ȊO�̏ꍇ
					for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
					{// ���[�v�J�E���g
						if (nCntWarp == 0)
						{// �J�E���g���O��������
							bool bCheck = false;	// �`�F�b�N
							do
							{// ���[�v
								bCheck = false;		// ������
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// �p�^�[���ݒ�

								do
								{// ���[�v
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// �p�^�[���ݒ�
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// �p�^�[�����Ⴄ�Ȃ甲����

								if (2 < m_nPatternMax)
								{// �p�^�[�����Q��葽��
									if ((m_pPattern[m_pWarp[nCntWarp].nPattern_1].bStart	== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd	== true) ||
										(m_pPattern[m_pWarp[nCntWarp].nPattern_1].bEnd		== true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart	== true))
									{// �X�^�[�g�ƃG���h�����΂ꂽ
										bCheck = true;		// �`�F�b�N
									}
								}
							} while (bCheck != false);
						}
						else
						{// �J�E���g���P�ȏゾ������
							bool bCheck = false;	// �`�F�b�N
							do
							{// ���[�v
								bCheck = false;		// ������
								m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// �p�^�[���ݒ�

								for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
								{// ���[�v�J�E���g
									if (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_1 ||
										m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_2)
									{// �����ԍ�������
										bCheck = true;		// �`�F�b�N
										break;
									}
								}
							} while (bCheck != false);

							do
							{// ���[�v
								bCheck = false;		// �`�F�b�N
								do
								{// ���[�v
									m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// �p�^�[���ݒ�
								} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// �p�^�[�����Ⴄ�Ȃ甲����

								if (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern == 2 &&
									(m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true || m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true))
								{// �p�^�[���̃G���A�������Q + �i�X�^�[�g������A�G���h������j
									bCheck = true;	// �`�F�b�N
								}
							} while (bCheck != false);
						}

						bool bCheck = false;	// �`�F�b�N
						do
						{// ���[�v
							bCheck = false;		// ������
							do
							{// ���[�v
								m_pWarp[nCntWarp].nArea_1 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_1].nMaxPattern;		// �G���A�ݒ�
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_END);		// �X�^�[�g�ƃG���h�ȊO�������甲����
							m_pWarp[nCntWarp].nArea_1 = m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1];		// �G���A�ԍ��ݒ�

							do
							{
								m_pWarp[nCntWarp].nArea_2 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern;		// �G���A�ݒ�
							} while (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_START ||
								m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_END);		// �X�^�[�g�ƃG���h�ȊO�������甲����
							m_pWarp[nCntWarp].nArea_2 = m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2];		// �G���A�ԍ��ݒ�

							for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
							{// ���[�v�J�E���g
								if (m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_2 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_1 ||
									m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_2)
								{// �G���A������
									bCheck = true;		// �`�F�b�N
									break;
								}
							}
						} while (bCheck != false);
					}
				}
			}

			delete[] nRouteCounter;	// �j��
			delete[] nRouteEnd;		// �j��
		}
	}

	int nMaxVtx = m_pField->GetMaxVtx();	// ���_�擾

	// ���_�ʒu�ݒ�
	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 posVtx = m_pField->GetVtx(nCntVtx);

		/*if (posVtx.x < -1500.0f || 1500.0f < posVtx.x || posVtx.z < -1500.0f || 1500.0f < posVtx.z)
		{
		posVtx.y = 80.0f + (rand() % 80);

		m_pField->SetVtx(nCntVtx, posVtx);
		m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}
		else*/
		{
			posVtx.y = 80.0f + (rand() % 80);			// �ʒu�ݒ�

			// ���_�A�F�ݒ�
			m_pField->SetVtx(nCntVtx, posVtx);
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

			// �G���A
			if (m_pArea != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				{// �G���A�J�E���g
					// ���S����̈ʒu�v�Z
					float fRange = sqrtf(powf(m_pArea[nCount].pos.x - posVtx.x, 2.0f) + powf(m_pArea[nCount].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[nCount].fRange)
					{// �͈͓���������
						posVtx.y = 0.0f - (rand() % 15);	// �ʒu�ݒ�

						// ���_�A�F�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//if (m_pArea[nCount].nArea == AREA_START || m_pArea[nCount].nArea == AREA_END)
						//	m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

						/*if (m_pWarp != NULL)
						{
							for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
							{
								if (m_pWarp[nCntWarp].nArea_1 == nCount || m_pWarp[nCntWarp].nArea_2 == nCount)
								{
									if (nCntWarp == 0)
										m_pField->SetCol(nCntVtx, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
									else
										m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

									break;
								}
							}
						}*/

						break;
					}					
				}
			}

			// ���[�g
			if (m_pRoute != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ���[�g�J�E���g
					D3DXVECTOR3 vecA, vecC;		// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
					int nCheck = 0;				// �`�F�b�N
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						vecA = m_pRoute[nCount].pos[nCntPos] - m_pRoute[nCount].pos[(nCntPos + 1) % 4];
						vecC = posVtx - m_pRoute[nCount].pos[(nCntPos + 1) % 4];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
						{// �͈͓���
							nCheck++;	// �J�E���g
						}
					}

					if (nCheck == 4)
					{// �͈͓���������
						posVtx.y = 0.0f - (rand() % 15);	// �ʒu�ݒ�

						// ���_�A�F�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}

			// ���[�v
			if (m_pWarp != NULL)
			{
				for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
				{
					float fRange;
					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_1].fRange * 0.2f)
					{// �͈͓���������
						posVtx.y = 0.0f;		// �ʒu�ݒ�

						// ���_�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
					}

					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_2].fRange * 0.2f)
					{// �͈͓���������
						posVtx.y = 0.0f;		// �ʒu�ݒ�

						// ���_�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
					}
				}
			}
		}
	}
	m_pField->SetVecNor();	// �@���v�Z
}

//=============================================================================
// �I�u�W�F�N�g�ݒ菈��								(public)	*** CMap_03 ***
//=============================================================================
void CMap_03::SetObject(void)
{
	if (1 < m_nPatternMax)
	{// ���[�v�����݂���
		// ���[�v���f���v�Z
		m_nModelWarp = (m_nPatternMax - 1) * 2;

		// ���[�v���f��
		if (m_pModelWarp == NULL && m_ModelInfo != NULL)
		{// NULL�̏ꍇ
			m_pModelWarp = new CModel[m_nModelWarp];	// �������m��

			for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
			{// ���f�����J�E���g
				// �����l�ݒ�
				m_pModelWarp[nCntModel].SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_ModelInfo[0].pos, m_ModelInfo[0].rot);
				// ����������
				m_pModelWarp[nCntModel].Init();

				// �e���f����ݒ�
				m_pModelWarp[nCntModel].SetParent(NULL);

				if (m_pWarpCylinder[nCntModel] == NULL)
				{// NULL�ȊO�̏ꍇ
					m_pWarpCylinder[nCntModel] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);

					// �����l�ݒ�
					m_pWarpCylinder[nCntModel]->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 10.0f));
					// ����������
					m_pWarpCylinder[nCntModel]->Init();
					m_pWarpCylinder[nCntModel]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
					m_pWarpCylinder[nCntModel]->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
				}
			}
		}

		for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
		{// �p�^�[���J�E���g
			if (m_pModelWarp != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pModelWarp[0 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pModelWarp[1 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);
			}

			if (m_pWarpCylinder[0 + (nCntWarp * 2)] != NULL && m_pWarpCylinder[1 + (nCntWarp * 2)] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pWarpCylinder[0 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pWarpCylinder[1 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);

				if (nCntWarp == 0)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
					}
				}
				else if (nCntWarp == 1)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
					}
				}
			}
		}
	}

	if (m_pGoalCylinder == NULL)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{
			if (nCntArea == AREA_END)
			{
				m_pGoalCylinder = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GOAL);

				m_pGoalCylinder->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pArea[nCntArea].pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 800.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f));
				m_pGoalCylinder->Init();
				m_pGoalCylinder->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
				m_pGoalCylinder->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
			}
		}
	}

	if (m_pIcon == NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIcon = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_MAPICON);

		m_pIcon->SetInitAll(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		m_pIcon->Init();
		m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
		m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_NOTEQUAL);
		m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CScene		*pScene		= NULL;		// �V�[��
	CScene		*pSceneNext	= NULL;		// ���V�[��
	CSceneMesh	*pMesh		= NULL;		// ���b�V��
	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g
					break;
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	int nNumSet = 20;
	m_nGrassMax = m_nAreaMax * nNumSet;

	if (m_pGrass == NULL)
	{
		m_pGrass = new GRASS[m_nGrassMax];

		if (m_pGrass != NULL)
		{
			for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
			{
				for (int nCntGrass = 0; nCntGrass < nNumSet; nCntGrass++)
				{
					D3DXVECTOR3 pos;		// �ʒu
					float fAngle = (float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)) * 0.01f;
					float fLenght = (float)(rand() % (int)(m_pArea[nCntArea].fRange - 150.0f) + 100.0f);
					// �ʒu�v�Z
					pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght;

					int nNum = nCntArea * nNumSet + nCntGrass;

					// �����l�ݒ�
					m_pGrass[nNum].nCounter = 0;
					m_pGrass[nNum].pos = pos;
					m_pGrass[nNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
					{// ���J�E���g
						D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
						D3DXVECTOR3 rot = m_pGrass[nNum].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

						float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);
						float fLenght = -5.0f + (float)(rand() % 10);

						D3DXVECTOR3 posLeft = m_pGrass[nNum].pos +
							D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
							(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

						for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
						{
							m_pGrass[nNum].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->Init();
							m_pGrass[nNum].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
							m_pGrass[nNum].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

							if (pMesh != NULL)
							{
								D3DXVECTOR3 pos = m_pGrass[nNum].pGrass[nCount][nCntNum]->Getpos();
								pos.y = pMesh->GetHeight(pos);
								pos.y += 7.0f;
								m_pGrass[nNum].pGrass[nCount][nCntNum]->Setpos(pos);
							}

							m_pGrass[nNum].posCenter[nCount][nCntNum] = posLeft;

							D3DXVECTOR3 posVtx[4];
							m_pGrass[nNum].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
							m_pGrass[nNum].posVtx[nCount][nCntNum][0] = posVtx[0];
							m_pGrass[nNum].posVtx[nCount][nCntNum][1] = posVtx[1];

							m_pGrass[nNum].bFall[nCount][nCntNum] = false;
							m_pGrass[nNum].nFallCounter[nCount][nCntNum] = 0;

							posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
						}
					}
				}
			}
		}
	}

	/*if (m_pModelObject == NULL)
	{
		m_nModelObject = 500;
		m_pModelObject = new CModel[m_nModelObject];

		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
		{
			// �����l�ݒ�
			m_pModelObject[nCntModel].SetInitAll(m_pTexture[1], m_pMesh[1], m_pBuffMat[1], m_nNumMat[1], m_ModelInfo[1].pos, m_ModelInfo[1].rot);
			// ����������
			m_pModelObject[nCntModel].Init();

			// �e���f����ݒ�
			m_pModelObject[nCntModel].SetParent(NULL);

			D3DXVECTOR3 pos;
			bool bCheck = false;
			do
			{
				bCheck = false;

				pos.x = -2000.0f + (float)(rand() % (2000 * 2));
				pos.y = 0.0f;
				pos.z = -2000.0f + (float)(rand() % (2000 * 2));

				if (bCheck == false)
				{
					for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
					{
						float fRange = sqrtf(powf(pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(pos.z - m_pArea[nCntArea].pos.z, 2.0f));
						if (fRange < m_pArea[nCntArea].fRange)
						{
							bCheck = true;
							break;
						}
					}
				}
				if (bCheck == false)
				{
					for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
					{
						int nCheck = 0;
						for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
						{
							D3DXVECTOR3 vecA, vecC;
							vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];
							vecC = pos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];

							if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
							{// �͈͓���
								nCheck++;	// �J�E���g
							}
						}

						if (nCheck == 4)
						{// �͈͓���������
							bCheck = true;
							break;
						}
					}
				}
			} while (bCheck != false);

			if (pMesh != NULL)
			{
				pos.y = pMesh->GetHeight(pos);
			}

			m_pModelObject[nCntModel].Setpos(pos);
		}
	}*/

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// �G���A�J�E���g
		if (m_pArea[nCntArea].nArea != AREA_START)
		{
			// �G
//			CEnemy::Create(CEnemy::TYPE_N_SLIME, m_pArea[nCntArea].pos + D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
//			break;

			int nNum = rand() % 3 + 3;
			for (int nCount = 0; nCount < nNum; nCount++)
			{
				float fLenght;
				D3DXVECTOR3 pos;

				fLenght = m_pArea[nCntArea].fRange * 0.5f;
				pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f))), 0.0f, cosf((float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)))) * fLenght;

				CEnemy::Create(CEnemy::TYPE_N_SLIME, pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			}
		}
	}
}

//=============================================================================
// �t�B�[���h�Փ˔��菈��								(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange)
{
	bool bLand = false;
	bool bAreaCheck			= false;	// �G���A�`�F�b�N����
	bool bAreaCheckOld		= false;	// �G���A�`�F�b�N�O��
	bool bRouteCheck		= false;	// ���[�g�`�F�b�N����
	bool bRouteCheckOld		= false;	// ���[�g�`�F�b�N�O��
	int nAreaCheckNum		= 0;		// �G���A�`�F�b�N�ԍ�
	int nRouteCheckNum		= 0;		// ���[�g�`�F�b�N�ԍ�

	bool bAreaIn			= false;	// �G���A�̒�
	int nAreaInNum			= 0;		// �G���A�̒��ԍ�
	bool bRouteIn			= false;	// ���[�g�̒�
	int nRouteInNum			= 0;		// ���[�g�̒��ԍ�

	// �G���A�`�F�b�N
	if (bAreaCheck == false || bAreaCheckOld == false)
	{// �ǂ����FALSE
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// �G���A�J�E���g
			// �G���A�`�F�b�N
			bAreaCheck		= false;
			bAreaCheckOld	= false;
			nAreaCheckNum	= nCntArea;

			float fRange;	// �͈�
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPosOld->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPosOld->z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// �͈͓���
				bAreaCheckOld	= true;			// �͈͓�
				bAreaIn			= true;			// �G���A���ɂ���
				nAreaInNum		= nCntArea;		// �G���A���̔ԍ�
			}

			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (m_pArea[nCntArea].fRange * MC_AREACOLL_NOW <= fRange)
			{// �͈͊O��
				bAreaCheck = true;		// �͈͓�
			}

			// �ǂ����TRUE�Ȃ珈���𔲂���
			if (bAreaCheck == true && bAreaCheckOld == true) break;
		}
	}

	if (bRouteCheck == false || bRouteCheckOld == false)
	{// �ǂ����FALSE
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ���[�g�J�E���g
			// ���[�g�`�F�b�N
			bRouteCheck		= false;
			bRouteCheckOld	= false;
			nRouteCheckNum	= nCntRoute;

			int nHit	= 0;	// ���݃q�b�g��
			int nHitOld = 0;	// �O��q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// �O�όv�Z
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = *pPos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}

				// �O�όv�Z
				vecAOld = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecCOld = *pPosOld - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecAOld.z * vecCOld.x) - (vecAOld.x * vecCOld.z)) <= 0)
				{// �͈͓���
					nHitOld++;	// �J�E���g
				}
			}

			if (nHit != 4)
			{// �͈͊O��
				bRouteCheck = true;			// �͈͊O
			}
			if (nHitOld == 4)
			{// �͈͓���
				bRouteCheckOld	= true;			// �͈͓�
				bRouteIn		= true;			// ���[�g���ɂ���
				nRouteInNum		= nCntRoute;	// ���[�g���̔ԍ�
			}

			// �ǂ����TRUE�Ȃ珈���𔲂���
			if (bRouteCheck == true && bRouteCheckOld == true) break;
		}
	}

	bool bArea = false;		// �G���A����

	// �G���A����
	if (bAreaCheck == true && bAreaCheckOld == true)
	{// �ǂ����TRUE
		float fRouteAngle		= 0.0f;		// ���[�g�p�x
		float fRouteAngleRange	= 0.0f;		// ���[�g�p�x�͈�
		float fRouteAngleL		= 0.0f;		// ���[�g�p�xL
		float fRouteAngleR		= 0.0f;		// ���[�g�p�xR
		bool bRoute				= false;	// ���[�g�ɂ���

		// �G���A���S����v���C���[�̊p�x�v�Z
		float fAngle = atan2f(pPos->x - m_pArea[nAreaCheckNum].pos.x, pPos->z - m_pArea[nAreaCheckNum].pos.z);

		if (bRouteIn == true)
		{// ���[�g���ɂ���
			D3DXVECTOR3 posLei;
			if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posStart)
			{// �X�^�[�g�Ɠ�����������
				posLei = m_pRoute[nRouteInNum].posCol[0];	// ���C�p�ʒu

				bool bCheck = false;	// �`�F�b�N
				while (true)
				{// ���[�v
					// 
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{
						break;
					}
					else
					{
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle)) * 1.0f;
					}
				}
			}
			else if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posEnd)
			{// �G���h�Ɠ�����������
				posLei = m_pRoute[nRouteInNum].posCol[2];	// ���C�p�ʒu

				bool bCheck = false;	// �`�F�b�N
				while (true)
				{// ���[�v
					// ���S����̈ʒu�v�Z
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{// �͈͊O
						break;
					}
					else
					{// �͈͓�
						// ���C��i�߂�
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle + D3DX_PI), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle + D3DX_PI)) * 1.0f;
					}
				}
			}

			// ���[�g�p�x�v�Z
			fRouteAngle = atan2f(m_pRoute[nRouteInNum].posCenter.x - m_pArea[nAreaCheckNum].pos.x, m_pRoute[nRouteInNum].posCenter.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = atan2f(posLei.x - m_pArea[nAreaCheckNum].pos.x, posLei.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = fRouteAngle - fRouteAngleRange;
			fRouteAngleRange = fabsf(fRouteAngleRange);

			if (fRouteAngleRange < -D3DX_PI)
				fRouteAngleRange += D3DX_PI * 2.0f;
			if (D3DX_PI < fRouteAngleRange)
				fRouteAngleRange -= D3DX_PI * 2.0f;

			fRouteAngleRange = fabsf(fRouteAngleRange);

			fRouteAngleL = fRouteAngle - fRouteAngleRange;
			fRouteAngleR = fRouteAngle + fRouteAngleRange;

			if (fRouteAngleL < fAngle && fAngle < fRouteAngleR)
			{
				bRoute = true;
			}
		}

		if (bRouteIn == false || (bRouteIn == true && bRoute == false))
		{// ���[�g���ɂ��Ȃ��A�i���[�g���ɂ��� + ���[�g�͈͊O�j
			pPos->x = m_pArea[nAreaCheckNum].pos.x + sinf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);
			pPos->z = m_pArea[nAreaCheckNum].pos.z + cosf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);

			pMove->x = 0.0f;
			pMove->z = 0.0f;
			bArea = true;	// �G���A����
			bLand = true;
		}
	}

	// ���[�g����
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteCheck == true && bRouteCheckOld == true)
		{// �ǂ����TRUE
			// �Z���^�[�v�Z
			float fAngleCenter	= 0.0f;
			float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteCheckNum].posStart.x - m_pRoute[nRouteCheckNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteCheckNum].posStart.z - m_pRoute[nRouteCheckNum].pos[0].z, 2.0f));

			// �O�όv�Z
			D3DXVECTOR3 vecALR, vecCLR;
			vecALR = m_pRoute[nRouteCheckNum].posStart - m_pRoute[nRouteCheckNum].posEnd;
			vecCLR = *pPos - m_pRoute[nRouteCheckNum].posEnd;

			// ���[�g�̉E����������
			if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle - (D3DX_PI * 0.5f);	// ����
			else
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle + (D3DX_PI * 0.5f);	// �E��

			// �Z���^�[�v�Z
			D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

			// �Z���^�[�ւ̊p�x�v�Z
			float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

			int nHit = 0;	// �q�b�g��

			while (1)
			{
				nHit = 0;	// �q�b�g��
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
				{// �ʒu�J�E���g
					// �O�όv�Z
					D3DXVECTOR3 vecA, vecC;
					vecA = m_pRoute[nRouteCheckNum].posCol[nCntPos] - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
					vecC = *pPos - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

					if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
					{// �͈͓�
						nHit++;
					}
				}

				if (nHit == 4)
				{// �͈͓�
					bLand = true;
					break;		// �����𔲂���
				}
				else
				{// �͈͊O
					// �ʒu���X�V�i�߂Â���j
					pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
					pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
				}
			}

			pMove->x = 0.0f;
			pMove->z = 0.0f;
		}
	}

	// ���̑��̔���i�������ꍇ�j
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteIn == false)
		{// ���[�g�ɂ��Ȃ�
			if (bRouteCheck == true && bRouteCheckOld == false)
			{// ���݂��͈͊O + �O�񂪔͈͊O
				int nRouteNum = 0;		// ���[�g�ԍ�

				for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
				{// ���[�g�J�E���g
					int nHit = 0;	// �q�b�g��

					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
						vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// �͈͓�
							nHit++;		// �J�E���g
						}
					}

					if (nHit == 4)
					{// �͈͓�
						nRouteNum = nCntRoute;		// �ԍ����L��
						break;
					}
				}

				// �Z���^�[�v�Z
				float fAngleCenter	= 0.0f;
				float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteNum].posStart.x - m_pRoute[nRouteNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteNum].posStart.z - m_pRoute[nRouteNum].pos[0].z, 2.0f));

				// �O�όv�Z
				D3DXVECTOR3 vecALR, vecCLR;
				vecALR = m_pRoute[nRouteNum].posStart - m_pRoute[nRouteNum].posEnd;
				vecCLR = *pPos - m_pRoute[nRouteNum].posEnd;

				// ���[�g�̉E����������
				if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
					fAngleCenter = m_pRoute[nRouteNum].fAngle - (D3DX_PI * 0.5f);	// ����
				else
					fAngleCenter = m_pRoute[nRouteNum].fAngle + (D3DX_PI * 0.5f);	// �E��

				// �Z���^�[�v�Z
				D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

				// �Z���^�[�ւ̊p�x�v�Z
				float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

				int nHit = 0;	// �q�b�g��

				while (1)
				{
					nHit = 0;	// �q�b�g��
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						D3DXVECTOR3 vecA, vecC;
						vecA = m_pRoute[nRouteNum].posCol[nCntPos] - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// �͈͓�
							nHit++;
						}
					}

					if (nHit == 4)
					{// �͈͓�
						bLand = true;
						break;		// �����𔲂���
					}
					else
					{// �͈͊O
						// �ʒu���X�V�i�߂Â���j
						pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
						pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
					}
				}

				pMove->x = 0.0f;
				pMove->z = 0.0f;
			}
		}
	}

	return bLand;
}

//=============================================================================
// �G���A���[�g���ɂ��邩���菈��						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionInOut(D3DXVECTOR3 pos)
{
	bool bLand = false;

	if (bLand == false)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// �G���A�J�E���g
			float fRange;	// �͈�
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pos.z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// �͈͓���
				bLand = true;
				break;
			}
		}
	}

	if (bLand == false)
	{
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ���[�g�J�E���g
			int nHit = 0;	// ���݃q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC;
				// �O�όv�Z
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = pos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}
			}

			if (nHit == 4)
			{// �͈͓���
				bLand = true;
				break;
			}
		}
	}

	return bLand;
}

//=============================================================================
// �t�B�[���h���[�v�Փ˔��菈��						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp)
{
	int nWarpNow = 0;
	int nWarpNext = 0;
	bool bUseWarp = false;

	for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
	{
		float fRange;
		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNow = m_pWarp[nCntWarp].nArea_1;
			nWarpNext = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}

		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNext = m_pWarp[nCntWarp].nArea_1;
			nWarpNow = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}
	}

	if (bUseWarp == true)
	{
		*pPosWarp = m_pArea[nWarpNext].pos;

		if (m_pIcon != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nWarpNow].pos.x, 100.0f, m_pArea[nWarpNow].pos.z));
			m_bDrawIcon = true;			// �A�C�R���`��
		}
	}

	return bUseWarp;
}

//=============================================================================
// �t�B�[���h�S�[���Փ˔��菈��						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bUseGoal = false;;

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{
		if (nCntArea == AREA_END)
		{
			float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (fRange < 35.0f)
			{
				bUseGoal = true;

				if (m_pIcon != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nCntArea].pos.x, 100.0f, m_pArea[nCntArea].pos.z));
					m_bDrawIcon = true;			// �A�C�R���`��
				}

				break;
			}
		}
	}

	return bUseGoal;
}

//=============================================================================
// �t�B�[���h�J�����Փ˔��菈��						(public)	*** CMap_03 ***
//=============================================================================
bool CMap_03::CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	bool bAreaIn	= false;	// �G���A�̒�
	int nAreaInNum	= 0;		// �G���A�̒��ԍ�
	bool bRouteIn	= false;	// ���[�g�̒�
	int nRouteInNum = 0;		// ���[�g�̒��ԍ�
	bool bRouteInV	= false;	// ���_���[�g�̒�

	// �G���A�`�F�b�N
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// �G���A�J�E���g
		float fRange;	// �͈�
		fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - posR.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - posR.z, 2.0f));
		if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
		{// �͈͓���
			bAreaIn		= true;			// �G���A���ɂ���
			nAreaInNum	= nCntArea;		// �G���A���̔ԍ�
			break;
		}
	}

	// ���[�g�`�F�b�N
	for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
	{// ���[�g�J�E���g
		int nHit = 0;	// ���݃q�b�g��
		int nHitV = 0;	// ���݃q�b�g��

		for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
		{// �ʒu�J�E���g
			D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
			// �O�όv�Z
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posR - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// �͈͓���
				nHit++;		// �J�E���g
			}

			// �O�όv�Z
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posV - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// �͈͓���
				nHitV++;		// �J�E���g
			}
		}

		if (nHit == 4)
		{// �͈͓���
			bRouteIn	= true;			// �͈͊O
			nRouteInNum = nCntRoute;	// �G���A���̔ԍ�

			if (nHitV == 4)
			{// �͈͓���
				bRouteInV = true;			// �͈͊O
			}

			break;
		}


	}

	bool bArea = false;		// �G���A����

	// �G���A����
	if (bAreaIn == true)
	{// �G���A���ɂ���
		// �������v�Z
		float fRange = sqrtf(powf(m_pArea[nAreaInNum].pos.x - posV.x, 2.0f) + powf(m_pArea[nAreaInNum].pos.z - posV.z, 2.0f));
		if (fRange < m_pArea[nAreaInNum].fRange)
		{// �͈͓�
			return true;
		}
		else
		{// �͈͊O
			if (bRouteInV == true)
			{// ���_�����[�g���ɂ���
				return true;
			}
			else
			{// ���_�����[�g���ɂ��Ȃ�
				return false;
			}
		}
	}

	// ���[�g����
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteIn == true)
		{// ���[�g���ɂ���
			int nHit = 0;	// ���݃q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// �O�όv�Z
				vecA = m_pRoute[nRouteInNum].posCol[nCntPos] - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = posV - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}
			}

			if (nHit == 4)
			{// �͈͓���
				return true;
			}
			else
			{// �͈͊O
				return false;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CMap_04::m_nMaxModel		= 0;		// ���f����

LPDIRECT3DTEXTURE9		**CMap_04::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CMap_04::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CMap_04::m_pBuffMat	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CMap_04::m_nNumMat		= NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CMap_04::m_ModelInfo	= NULL;		// ���f���C���t�H

//=============================================================================
// �R���X�g���N�^										(public)	*** CMap_04 ***
//=============================================================================
CMap_04::CMap_04() : CMap()
{
	m_nModelWarp = 0;									// ���f�����[�v��
	m_pModelWarp = NULL;								// ���f�����[�v

	m_nModelObject = 0;									// ���f���I�u�W�F�N�g��
	m_pModelObject = NULL;								// ���f���I�u�W�F�N�g

	m_pField = NULL;									// �t�B�[���h

	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		m_pSky[nCount] = NULL;							// NULL��
		m_SkyRot[nCount] = D3DXVECTOR2(0.0f, 0.0f);		// ��]
	}

	m_nGrassMax = 0;									// �ő呐
	m_pGrass = NULL;									// ��

	m_nAreaMax = 0;										// �G���A�ő吔

	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaNum[nCount] = 0;							// �G���A��

	m_pArea = NULL;										// �G���A

	m_nRouteMax = 0;									// ���[�g�ő吔
	m_pRoute = NULL;									// ���[�g

	m_nPatternMax = 0;									// �p�^�[���ő吔
	m_pPattern = NULL;									// �p�^�[��

	m_pWarp = NULL;										// ���[�v

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
		m_pWarpCylinder[nCount] = NULL;					// ���[�v�V�����_�[
	m_WarpCylinderUV = D3DXVECTOR2(0.0f, 0.0f);		// ���[�v�V�����_�[UV

	m_bGoal = false;									// �S�[���ł��邩
	m_pGoalCylinder = NULL;								// �S�[���V�����_�[

	m_bDrawIcon = false;								// �A�C�R���`��
	m_pIcon = NULL;										// �A�C�R��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMap_04 ***
//=============================================================================
CMap_04::~CMap_04()
{

}

//=============================================================================
// ���[�h����										(public)	*** CMap_04 ***
//=============================================================================
HRESULT CMap_04::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MAP\\gamemap.txt", &m_nMaxModel);		// ���f���擾

	// �e���̃������m��
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_nMaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_nMaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_nMaxModel];

	// �e���̏�����
	for (int nCount = 0; nCount < m_nMaxModel; nCount++)		// ���f�����J�E���g
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_nMaxModel; nCntParnet++)
	{// ���f�����J�E���g
		// X�t�@�C���̓ǂݍ���
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Unload(void)
{
		// �e���̃������J���ANULL��
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// ���f���C���t�H
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULL��
}

//=============================================================================
// ����������										(public)	*** CMap_04 ***
//=============================================================================
HRESULT CMap_04::Init(void)
{
	// ���b�V���t�B�[���h
	m_pField = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHFIELD);
	m_pField->SetInitAll(CSceneMesh::MESHTYPE_FIELD, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(80.0f, 0.0f, 80.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1800.0f, 0.0f, 1800.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(12.0f, 12.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(3600.0f, 0.0f, 3600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(24.0f, 24.0f));
		//D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 0.0f, 8000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(10.0f, 10.0f));
	m_pField->Init();
	m_pField->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_FIELD));

	// ��P
	m_pSky[SKY_NO_0] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_0]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8000.0f, 2000.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(0.75f, 1.0f));
	m_pSky[SKY_NO_0]->Init();
	m_pSky[SKY_NO_0]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_1));
	m_pSky[SKY_NO_0]->DrawEdit(false, false, false, false, true);

	// ��Q
	m_pSky[SKY_NO_1] = new CSceneMesh(CScene::PRIORITY_1, CScene::OBJTYPE_MESHCYLINDER);
	m_pSky[SKY_NO_1]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(15.0f, 5.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.3f, 0.0f), D3DXVECTOR3(6000.0f, 1500.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f, 1.0f));
	m_pSky[SKY_NO_1]->Init();
	m_pSky[SKY_NO_1]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_SKY_2));
	m_pSky[SKY_NO_1]->DrawEdit(false, false, false, false, true);

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Uninit(void)
{
	// ���f��
	if (m_pModelWarp != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++) // ���f�����J�E���g
			m_pModelWarp[nCntModel].Uninit();	// �I������

		delete[] m_pModelWarp;		// �������J��
		m_pModelWarp = NULL;		// NULL��
	}
	if (m_pModelObject != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++) // ���f�����J�E���g
			m_pModelObject[nCntModel].Uninit();	// �I������

		delete[] m_pModelObject;	// �������J��
		m_pModelObject = NULL;		// NULL��
	}

	// �t�B�[���h
	if (m_pField != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pField->Uninit();		// �I������
		m_pField = NULL;		// NULL��
	}

	// ��
	for (int nCount = 0; nCount < SKY_MAX; nCount++)
	{// �ő吔�J�E���g
		if (m_pSky[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pSky[nCount]->Uninit();	// �I������
			m_pSky[nCount] = NULL;		// NULL��
		}
	}

	// ��
	if (m_pGrass != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// ���J�E���g
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Uninit();		// �I������
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum] = NULL;			// NULL��
				}
			}
		}

		delete[] m_pGrass;	// �������J��
		m_pGrass = NULL;		// NULL��
	}

	// �}�b�v
	if (m_pArea != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pArea;		// �j��
		m_pArea = NULL;			// NULL��
	}
	if (m_pRoute != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pRoute;		// �j��
		m_pRoute = NULL;		// NULL��
	}
	if (m_pPattern != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nPatternMax; nCount++)
		{// �G���A�J�E���g
			delete[] m_pPattern[nCount].nPattern;
			m_pPattern[nCount].nPattern = NULL;
		}

		delete[] m_pPattern;	// �j��
		m_pPattern = NULL;		// NULL��
	}
	if (m_pWarp != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_pWarp;		// �j��
		m_pWarp = NULL;			// NULL��
	}

	for (int nCount = 0; nCount < m_WARPCYLINDER_NUM; nCount++)
	{
		if (m_pWarpCylinder[nCount] != NULL)
		{
			m_pWarpCylinder[nCount]->Uninit();
			m_pWarpCylinder[nCount] = NULL;
		}
	}

	// �S�[���V�����_�[
	if (m_pGoalCylinder != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pGoalCylinder->Uninit();
		m_pGoalCylinder = NULL;
	}

	// �A�C�R��
	if (m_pIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}

	CMap::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Update(void)
{
	// ��0
	if (m_pSky[SKY_NO_0] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_0].x -= MC_SKYROT_0;			// ��]

		m_pSky[SKY_NO_0]->SetTex(m_SkyRot[SKY_NO_0]);	// �e�N�X�`���ݒ�
	}

	// ��1
	if (m_pSky[SKY_NO_1] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_SkyRot[SKY_NO_1].x -= MC_SKYROT_1;			// ��]

		m_pSky[SKY_NO_1]->SetTex(m_SkyRot[SKY_NO_1]);	// �e�N�X�`���ݒ�
	}

	CPlayer *pPlayer = CManager::GetGame()->GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->Getpos();
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 CameraposR = pCamera->Getpos(1);
	D3DXVECTOR3 Camerarot = pCamera->Getrot();

	if (m_pGrass != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntGrass = 0; nCntGrass < m_nGrassMax; nCntGrass++)
		{// �ő吔�J�E���g
			for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
			{// ���J�E���g
				for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
				{// ���C���J�E���g
					D3DXVECTOR3 posVtx[4];		// ���_�󂯎��
					D3DXVECTOR3 posCenter = m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->Getpos();
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);		// ���_���擾

																							// �J�����͈�
					float fCameraL = Camerarot.y - (D3DX_PI * 0.5f);
					float fCameraR = Camerarot.y + (D3DX_PI * 0.5f);

					// �͈͒���
					if (fCameraL < -D3DX_PI)
						fCameraL += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraL)
						fCameraL -= D3DX_PI * 2.0f;

					if (fCameraR < -D3DX_PI)
						fCameraR += D3DX_PI * 2.0f;
					if (D3DX_PI < fCameraR)
						fCameraR -= D3DX_PI * 2.0f;

					float fCameraAngle = atan2f(posCenter.x - CameraposR.x, posCenter.z - CameraposR.z);

					if (Camerarot.y < (-D3DX_PI * 0.5f) || (D3DX_PI * 0.5f) < Camerarot.y)
					{
						if (fCameraR < fCameraAngle && fCameraAngle < fCameraL)
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}
					else
					{
						if (fCameraL < fCameraAngle && fCameraAngle < fCameraR)
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
						}
						else
						{// �͈̓`�F�b�N
							m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
						}
					}

					float fRange;
					fRange = sqrtf(powf(posCenter.x - posPlayer.x, 2.0f) + powf(posCenter.z - posPlayer.z, 2.0f));
					if (fRange < 850.0f)
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(true);
					}
					else
					{
						m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetDraw(false);
					}

					bool bCheck = false;
					if (fRange < 30.0f)
					{
						bCheck = true;
					}

					if (bCheck == true)
					{
						float fAngle = atan2f(posCenter.x - posPlayer.x, posCenter.z - posPlayer.z);
						D3DXVECTOR3 posCenterNext = m_pGrass[nCntGrass].posCenter[nCount][nCntNum] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
						fRange = sqrtf(powf(posCenter.x - posCenterNext.x, 2.0f) + powf(posCenter.z - posCenterNext.z, 2.0f));
						if (fRange < 20.0f)
						{
							posVtx[0] = posVtx[0] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							posVtx[1] = posVtx[1] + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * 1.0f;
							m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenterNext;
						}

						m_pGrass[nCntGrass].bFall[nCount][nCntNum] = true;
					}
					else
					{
						if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == true)
						{
							posVtx[0].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].x - posVtx[0].x) * 0.1f;
							posVtx[0].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][0].z - posVtx[0].z) * 0.1f;
							posVtx[1].x += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].x - posVtx[1].x) * 0.1f;
							posVtx[1].z += (m_pGrass[nCntGrass].posVtx[nCount][nCntNum][1].z - posVtx[1].z) * 0.1f;
							m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum]++;

							if (m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] == 20)
							{
								m_pGrass[nCntGrass].bFall[nCount][nCntNum] = false;
								m_pGrass[nCntGrass].posCenter[nCount][nCntNum] = posCenter;
								m_pGrass[nCntGrass].nFallCounter[nCount][nCntNum] = 0;
							}
						}
					}

					if (m_pGrass[nCntGrass].bFall[nCount][nCntNum] == false)
					{
						if (m_pGrass->nCounter < 120)
						{// ���J�E���g���Ⴂ
							posVtx[0].x += sinf(0.0f) * 0.023f;
							posVtx[0].x += cosf(0.0f) * 0.023f;
							posVtx[1].x += sinf(0.0f) * 0.023f;
							posVtx[1].x += cosf(0.0f) * 0.023f;
						}
						else
						{// ���J�E���g�ȏ�
							posVtx[0].x -= sinf(0.0f) * 0.023f;
							posVtx[0].x -= cosf(0.0f) * 0.023f;
							posVtx[1].x -= sinf(0.0f) * 0.023f;
							posVtx[1].x -= cosf(0.0f) * 0.023f;
						}
					}
					m_pGrass[nCntGrass].pGrass[nCount][nCntNum]->SetVtxSize(posVtx);		// ���_���ݒ�
				}
			}

			{
				m_pGrass[nCntGrass].nCounter++;		// �J�E���g����

				if (m_pGrass[nCntGrass].nCounter == 240)
				{// �w�肵���J�E���g�ɂȂ�����
					m_pGrass[nCntGrass].nCounter = 0;	// �J�E���g������
				}
			}
		}
	}

	m_WarpCylinderUV.y -= 0.01f;
	for (int nCntWarp = 0; nCntWarp < m_nModelWarp; nCntWarp++)
	{
		if (m_pWarpCylinder[nCntWarp] != NULL)
		{
			m_pWarpCylinder[nCntWarp]->SetTex(m_WarpCylinderUV);
		}
	}

	if (m_bGoal == true)
	{
		if (m_pGoalCylinder != NULL)
		{
			D3DXVECTOR3 size = m_pGoalCylinder->Getsize();
			if (size.y != 800.0f)
			{
				size.y += 5.0f;
				if (800.0f < size.y) size.y = 800.0f;
			}
			m_pGoalCylinder->SetStart(m_pGoalCylinder->Getpos(), m_pGoalCylinder->Getrot(), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
		}

		if (m_pIcon != NULL)
		{
			D3DXCOLOR col = m_pIcon->Getcol();
			if (m_bDrawIcon == true)
			{
				if (col.a != 1.0f)
				{
					col.a += 0.05f;
					if (1.0f < col.a) col.a = 1.0f;
				}
			}
			else
			{
				if (col.a != 0.0f)
				{
					col.a -= 0.05f;
					if (col.a < 0.0f) col.a = 0.0f;
				}
			}
			m_pIcon->Setcol(col);
		}
		m_bDrawIcon = false;
	}
}

//=============================================================================
// �`�揈��											(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	if (1 < m_nPatternMax)
	{
		for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
		{// ���f�����J�E���g
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			if (m_pModelWarp != NULL)
			{// NULL�ȊO�̏ꍇ
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

				m_pModelWarp[nCntModel].Draw();

				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

				// �e�\��
				//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
			}
		}
	}

	for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
	{// ���f�����J�E���g
	 // ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		if (m_pModelObject != NULL)
		{// NULL�ȊO�̏ꍇ
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModelObject[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// �e�\��
			//CShadow::SetShadow(m_pModelWarp[nCntModel].GetmtxWorld(), m_pMesh[0], m_nNumMat[0], D3DXVECTOR3(mtxWorld._41, mtxWorld._42 + 0.1f, mtxWorld._43));
		}
	}
}

//=============================================================================
// �t�B�[���h�ݒ菈��									(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::SetField(D3DXVECTOR3 pos)
{
	// �G���A��
	m_nAreaNum[AREA_START]	= 0;
	m_nAreaNum[AREA_END]	= 0;
	m_nAreaNum[AREA_NORMAL]	= 0;
	m_nAreaNum[AREA_BATTLE]	= 1;
	m_nAreaNum[AREA_ITEM]	= 0;

	// �G���A�����v�Z
	for (int nCount = 0; nCount < AREA_MAX; nCount++)
		m_nAreaMax += m_nAreaNum[nCount];

	// �������m��
	if (m_pArea == NULL) m_pArea = new AREA_RANGE[m_nAreaMax];

	// �G���A������
	for (int nCount = 0; nCount < m_nAreaMax; nCount++)
	{// �G���A�J�E���g
		m_pArea[nCount].nArea	= AREA_START;						// �G���A
		m_pArea[nCount].fRange	= 0.0f;								// �͈�
		m_pArea[nCount].pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���S
	}

	// �G���A�A���[�g����
	{
		// �G���A
		{
			int nCntArea = 0;		// �G���A�ԍ�
			for (int nCount = 0; nCount < AREA_MAX; nCount++)
			{// �G���A�J�E���g
				for (int nCntNum = 0; nCntNum < m_nAreaNum[nCount]; nCntNum++)
				{// �G���A���J�E���g
					int nCheckCounter = 0;	// �`�F�b�N�J�E���g
					bool bCheck = false;	// �`�F�b�N
					do
					{// ���[�v
						bCheck = false;

						switch (nCount)
						{// �G���A
						case AREA_START:	// �X�^�[�g
							m_pArea[nCntArea].nArea = AREA_START;		// �G���A
							m_pArea[nCntArea].fRange = 280.0f;			// �͈�
							m_pArea[nCntArea].pos	= pos;				// �ʒu
							break;
						case AREA_END:		// �G���h
							m_pArea[nCntArea].nArea = AREA_END;			// �G���A
							m_pArea[nCntArea].fRange = 280.0f;			// �͈�

							// �ʒu
							{
								float fRange = 0.0f;	// �͈�
								do
								{// ���[�v
									m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
									m_pArea[nCntArea].pos.y = 0.0f;
									m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));

									// ���S����̋������v�Z
									fRange = sqrtf(powf(m_pArea[0].pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(m_pArea[0].pos.z - m_pArea[nCntArea].pos.z, 2.0f));
								} while (fRange < 1400.0f &&
									-700.0f < m_pArea[nCntArea].pos.x && m_pArea[nCntArea].pos.x < 700.0f &&
									-700.0f < m_pArea[nCntArea].pos.z && m_pArea[nCntArea].pos.z < 700.0f);		// ��苗���ȏゾ������
							}
							break;
						case AREA_NORMAL:	// �m�[�}��
						case AREA_BATTLE:	// �o�g��
							// �G���A
							if (nCount == AREA_NORMAL)	m_pArea[nCntArea].nArea = AREA_NORMAL;
							if (nCount == AREA_BATTLE)	m_pArea[nCntArea].nArea = AREA_BATTLE;
							/*m_pArea[nCntArea].fRange = (float)(rand() % 100 + 580);			// �͈�

							// �ʒu
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));*/

							m_pArea[nCntArea].fRange = 800.0f;			// �͈�

							// �ʒu
							m_pArea[nCntArea].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

							break;
						case AREA_ITEM:		// �A�C�e��
							m_pArea[nCntArea].nArea = AREA_ITEM;			//�@�G���A
							m_pArea[nCntArea].fRange = (float)(rand() % 50 + 320);			// �͈�

							// �ʒu
							m_pArea[nCntArea].pos.x = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							m_pArea[nCntArea].pos.y = 0.0f;
							m_pArea[nCntArea].pos.z = (float)((rand() % ((1470 - (int)m_pArea[nCntArea].fRange) * 2)) - (1470 - (int)m_pArea[nCntArea].fRange));
							break;
						}

						// �G���A���m�d�Ȃ��Ă��Ȃ���
						for (int nCntCheck = 0; nCntCheck < nCntArea; nCntCheck++)
						{// �G���A�J�E���g
							// ���S����̈ʒu�v�Z
							float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - m_pArea[nCntCheck].pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - m_pArea[nCntCheck].pos.z, 2.0f));
							if (fRange < m_pArea[nCntArea].fRange + m_pArea[nCntCheck].fRange + 100.0f)
							{// �G���A���d�Ȃ��Ă��Ȃ���
								bCheck = true;		// �d�Ȃ��Ă���
								nCheckCounter++;	// �J�E���g
								break;
							}
						}

						if (nCheckCounter == 30) break;
					} while (bCheck != false);
					
					if (nCheckCounter != 30)
						nCntArea++;		// �G���A�J�E���g
				}
			}
			m_nAreaMax = nCntArea;
		}

		if (1 < m_nAreaMax)
		{
			// ���[�g
			{
				// ���[�g��
				m_nRouteNum[AREA_START] = m_nAreaNum[AREA_START];
				m_nRouteNum[AREA_END] = m_nAreaNum[AREA_END];
				m_nRouteNum[AREA_NORMAL] = m_nAreaNum[AREA_NORMAL];
				m_nRouteNum[AREA_BATTLE] = m_nAreaNum[AREA_BATTLE];
				m_nRouteNum[AREA_ITEM] = m_nAreaNum[AREA_ITEM];

				// ���[�g�����v�Z
				for (int nCount = 0; nCount < AREA_MAX; nCount++)
					m_nRouteMax += m_nRouteNum[nCount];

				// �������m��
				if (m_pRoute == NULL) m_pRoute = new ROUTE_RANGE[m_nRouteMax];

				// ���[�g������
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ���[�g�J�E���g
					m_pRoute[nCount].nAreaNumStart = 0;									// �X�^�[�g�G���A�ԍ�
					m_pRoute[nCount].nAreaNumEnd = 0;									// �G���h�G���A�ԍ�
					m_pRoute[nCount].posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �X�^�[�g�G���A�ʒu
					m_pRoute[nCount].posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �G���h�G���A�ʒu
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
						m_pRoute[nCount].pos[nCntPos] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���[�g���_�ʒu
					m_pRoute[nCount].fAngle = 0.0f;										// ���[�g�p�x
					m_pRoute[nCount].fLenght = 0.0f;										// ���[�g����
					m_pRoute[nCount].fRange = 0.0f;										// ���[�g�͈́i�����j
				}

				int nRoute = 0;		// ���[�g�ԍ�
				for (int nCountArea = 0; nCountArea < m_nAreaMax; nCountArea++)
				{// �G���A�J�E���g
					int nAreaNum = nCountArea;		// �G���A�ԍ�
					float fAreaLenght = 5000.0f;	// ����

					for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
					{// �G���A�J�E���g
						if (nCountArea != nCntAreaCheck &&
							(nCountArea != AREA_START || (nCountArea == AREA_START && nCntAreaCheck != AREA_END)) &&
							(nCountArea != AREA_END || (nCountArea == AREA_END && nCntAreaCheck != AREA_START)))
						{// �����G���A�łȂ��A�X�^�[�g�ƃG���h���q���Ȃ�
							// ���S����̈ʒu�v�Z
							float fRange = sqrtf(powf(m_pArea[nCountArea].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nCountArea].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
							if (fRange < fAreaLenght)
							{// ���������߂�������
								nAreaNum = nCntAreaCheck;	// �ԍ�
								fAreaLenght = fRange;		// ����
							}
						}
					}

					bool bCheck = false;	// �`�F�b�N
					for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
					{// ���[�g�J�E���g
						if ((m_pRoute[nCntRoute].nAreaNumStart == nCountArea && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
							(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nCountArea))
						{// �������[�g���Ȃ���
							bCheck = true;	// ����
						}
					}

					if (bCheck == false)
					{// ��������
						// �͈́i�����j
						m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

						// ����
						m_pRoute[nRoute].fLenght = fAreaLenght;

						// �p�x
						m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nCountArea].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nCountArea].pos.z);

						// �ʒu
						m_pRoute[nRoute].nAreaNumStart = nCountArea;				// �X�^�[�g�ԍ�
						m_pRoute[nRoute].nAreaNumEnd = nAreaNum;					// �G���h�ԍ�
						m_pRoute[nRoute].posStart = m_pArea[nCountArea].pos;	// �X�^�[�g�ʒu
						m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;	// �G���h�ʒu

						// ���S���v�Z
						m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

						// ���_�v�Z
						m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

						// ����p�̒��_�v�Z
						m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

						nRoute++;	// ���[�g�J�E���g
						if (nRoute == m_nRouteMax) break;
					}
				}

				// �e�G���A�Ƀ��[�g�����{�q��������
				int *nRouteCounter = new int[m_nAreaMax];
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
					nRouteCounter[nCount] = 0;

				for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
				{// ���[�g�J�E���g
					nRouteCounter[m_pRoute[nCntRoute].nAreaNumStart]++;
					nRouteCounter[m_pRoute[nCntRoute].nAreaNumEnd]++;
				}

				// ���[�̃G���A���v�Z
				int nRouteEndNum = 0;
				int *nRouteEnd = new int[m_nAreaMax];
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
					nRouteEnd[nCount] = 0;

				// �q���ꂽ���[�g��
				int nRoute_0 = 0;
				int nRoute_1 = 0;
				int nRoute_2 = 0;

				for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
				{// ���[�g�J�E���g
					nRouteEnd[nRouteEndNum] = nCntRoute;	// �ԍ��L��
					nRouteEndNum++;							// �J�E���g

					if (nRouteCounter[nCntRoute] == 0)
						nRoute_0++;									// ���[�g���O
					else if (nRouteCounter[nCntRoute] == 1)
						nRoute_1++;									// ���[�g���P
					else if (1 < nRouteCounter[nCntRoute])
						nRoute_2 += nRouteCounter[nCntRoute] - 1;	// ���[�g���Q
				}

				// �p�^�[�������v�Z
				int nRoutePattern = nRoute - nRoute_2 + nRoute_0;

				if (nRoute_0 != 0)
				{// ���[�g
					int nNumber = 0;
					for (int nCntRoute = 0; nCntRoute < m_nAreaMax; nCntRoute++)
					{// ���[�g�J�E���g
						if (nRouteCounter[nCntRoute] == 0)
						{// �q�����Ă��Ȃ�
							nNumber = nCntRoute;
							break;
						}
					}

					int nAreaNum = nNumber;			// �G���A�ԍ�
					float fAreaLenght = 5000.0f;	// ����

					for (int nCntAreaCheck = 0; nCntAreaCheck < m_nAreaMax; nCntAreaCheck++)
					{// �G���A�J�E���g
						if (nNumber != nCntAreaCheck &&
							(nNumber != AREA_START || (nNumber == AREA_START && nCntAreaCheck != AREA_END)) &&
							(nNumber != AREA_END || (nNumber == AREA_END && nCntAreaCheck != AREA_START))) {// �����G���A�łȂ��A�X�^�[�g�ƃG���h���q���Ȃ�
							// ���S����̈ʒu�v�Z
							float fRange = sqrtf(powf(m_pArea[nNumber].pos.x - m_pArea[nCntAreaCheck].pos.x, 2.0f) + powf(m_pArea[nNumber].pos.z - m_pArea[nCntAreaCheck].pos.z, 2.0f));
							if (fRange < fAreaLenght)
							{// ���������߂�������
								nAreaNum = nCntAreaCheck;	// �ԍ�
								fAreaLenght = fRange;		// ����
							}
						}
					}

					bool bCheck = false;	// �`�F�b�N
					for (int nCntRoute = 0; nCntRoute < nRoute; nCntRoute++)
					{// ���[�g�J�E���g
						if ((m_pRoute[nCntRoute].nAreaNumStart == nRouteEnd[nCntRoute] && m_pRoute[nCntRoute].nAreaNumEnd == nAreaNum) ||
							(m_pRoute[nCntRoute].nAreaNumStart == nAreaNum && m_pRoute[nCntRoute].nAreaNumEnd == nRouteEnd[nCntRoute]))
						{// �������[�g���Ȃ���
							bCheck = true;	// ����
						}
					}

					if (bCheck == false)
					{// ��������
						// �͈́i�����j
						m_pRoute[nRoute].fRange = (float)(rand() % 30 + 100);

						// ����
						m_pRoute[nRoute].fLenght = fAreaLenght;

						// �p�x
						m_pRoute[nRoute].fAngle = atan2f(m_pArea[nAreaNum].pos.x - m_pArea[nNumber].pos.x, m_pArea[nAreaNum].pos.z - m_pArea[nNumber].pos.z);

						// �ʒu
						m_pRoute[nRoute].nAreaNumStart = nNumber;
						m_pRoute[nRoute].nAreaNumEnd = nAreaNum;
						m_pRoute[nRoute].posStart = m_pArea[nNumber].pos;
						m_pRoute[nRoute].posEnd = m_pArea[nAreaNum].pos;

						// ���S���v�Z
						m_pRoute[nRoute].posCenter = m_pRoute[nRoute].posStart + D3DXVECTOR3(sinf(m_pRoute[nRoute].fAngle), 0.0f, cosf(m_pRoute[nRoute].fAngle)) * (m_pRoute[nRoute].fLenght * 0.5f);

						// ���_�v�Z
						m_pRoute[nRoute].pos[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].pos[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;
						m_pRoute[nRoute].pos[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].pos[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * m_pRoute[nRoute].fRange;

						// ����p�̒��_�v�Z
						m_pRoute[nRoute].posCol[0].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[0].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[0].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].x = m_pRoute[nRoute].posStart.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[1].y = m_pRoute[nRoute].posStart.y + 0.0f;
						m_pRoute[nRoute].posCol[1].z = m_pRoute[nRoute].posStart.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[2].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[2].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle - (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].x = m_pRoute[nRoute].posEnd.x + sinf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);
						m_pRoute[nRoute].posCol[3].y = m_pRoute[nRoute].posEnd.y + 0.0f;
						m_pRoute[nRoute].posCol[3].z = m_pRoute[nRoute].posEnd.z + cosf(m_pRoute[nRoute].fAngle + (D3DX_PI * 0.5f)) * (m_pRoute[nRoute].fRange * 0.6f);

						nRoute++;	// ���[�g�J�E���g
						nRoutePattern--;
					}
				}

				m_nRouteMax = nRoute;	// ���[�g���J�E���g

				m_nPatternMax = nRoutePattern;		// �p�^�[�������L��

				// �������m��
				if (m_pPattern == NULL)
					m_pPattern = new ROUTE_PATTERN[m_nPatternMax];

				if (m_pPattern != NULL)
				{// NULL�ȊO�̏ꍇ
					for (int nCntPattert = 0; nCntPattert < m_nPatternMax; nCntPattert++)
					{// �p�^�[�����J�E���g
						// �����l�ݒ�
						m_pPattern[nCntPattert].nMaxPattern = 0;
						m_pPattern[nCntPattert].nPattern = new int[m_nAreaMax];
						m_pPattern[nCntPattert].bStart = false;
						m_pPattern[nCntPattert].bEnd = false;

						if (nCntPattert == 0)
						{// �J�E���g���O��������
							m_pPattern[nCntPattert].nPattern[0] = m_pRoute[0].nAreaNumStart;
							m_pPattern[nCntPattert].nPattern[1] = m_pRoute[0].nAreaNumEnd;
							m_pPattern[nCntPattert].nMaxPattern = 2;
						}
						else
						{// �J�E���g���O�ȊO��������
							int nNumber = 0;		// �ԍ��m�F
							bool bCheck = false;	// �`�F�b�N
							// �����ԍ��ɂȂ�Ȃ��悤����
							do
							{// ���[�v
								bCheck = false;		// ������
								for (int nCntCheck = 0; nCntCheck < nCntPattert; nCntCheck++)
								{// �p�^�[���J�E���g
									for (int nCntArea = 0; nCntArea < m_pPattern[nCntCheck].nMaxPattern; nCntArea++)
									{// �G���A�J�E���g
										if (m_pPattern[nCntCheck].nPattern[nCntArea] == nNumber)
										{// ������������
											nNumber++;		// �J�E���g
											bCheck = true;	// �`�F�b�N
											break;
										}
									}
									if (bCheck == true) break;
								}
							} while (bCheck != false);

							// �ԍ��ɂȂ��郋�[�g������
							for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
							{// ���[�g�J�E���g
								if (nNumber == m_pRoute[nCntRoute].nAreaNumStart || nNumber == m_pRoute[nCntRoute].nAreaNumEnd)
								{// �ԍ�������
									m_pPattern[nCntPattert].nPattern[0] = m_pRoute[nCntRoute].nAreaNumStart;
									m_pPattern[nCntPattert].nPattern[1] = m_pRoute[nCntRoute].nAreaNumEnd;
									m_pPattern[nCntPattert].nMaxPattern = 2;
									break;
								}
							}
						}

						for (int nCntPattertNum = 0; nCntPattertNum < m_pPattern[nCntPattert].nMaxPattern; nCntPattertNum++)
						{// �p�^�[���ɂ���G���A���J�E���g
							for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
							{// ���[�g�J�E���g
								if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumStart)
								{// �X�^�[�g�ԍ��Ɠ���
									for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
									{// �p�^�[���ɂ���G���A���J�E���g
										if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumEnd)
										{// �G���h�ԍ��Ɠ���
											break;
										}

										if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
										{// �����ԍ����Ȃ�������
											// �ԍ��L��
											m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumEnd;
											m_pPattern[nCntPattert].nMaxPattern++;	// �p�^�[���̃G���A���𑝂₷
										}
									}
								}
								else if (m_pPattern[nCntPattert].nPattern[nCntPattertNum] == m_pRoute[nCntRoute].nAreaNumEnd)
								{// �G���h�ԍ��Ɠ���
									for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattert].nMaxPattern; nCntCheck++)
									{// �p�^�[���ɂ���G���A���J�E���g
										if (m_pPattern[nCntPattert].nPattern[nCntCheck] == m_pRoute[nCntRoute].nAreaNumStart)
										{// �X�^�[�g�ԍ��Ɠ���
											break;
										}

										if (nCntCheck == m_pPattern[nCntPattert].nMaxPattern - 1)
										{// �����ԍ����Ȃ�������
											// �ԍ��L��
											m_pPattern[nCntPattert].nPattern[m_pPattern[nCntPattert].nMaxPattern] = m_pRoute[nCntRoute].nAreaNumStart;
											m_pPattern[nCntPattert].nMaxPattern++;	// �p�^�[���̃G���A���𑝂₷
										}
									}
								}
							}
						}
					}
				}

				for (int nCntPattern = 0; nCntPattern < m_nPatternMax; nCntPattern++)
				{// �p�^�[�����J�E���g
					for (int nCntCheck = 0; nCntCheck < m_pPattern[nCntPattern].nMaxPattern; nCntCheck++)
					{// �p�^�[���ɂ���G���A���J�E���g
						if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_START)
						{// �X�^�[�g�ԍ��Ɠ���
							m_pPattern[nCntPattern].bStart = true;		// �X�^�[�g������
						}
						else if (m_pPattern[nCntPattern].nPattern[nCntCheck] == AREA_END)
						{// �G���h�ԍ��Ɠ���
							m_pPattern[nCntPattern].bEnd = true;		// �G���h������
						}
					}
				}

				if (1 < m_nPatternMax)
				{// �p�^�[�������P��葽��
					// �������m��
					if (m_pWarp == NULL)
						m_pWarp = new WARP[m_nPatternMax - 1];

					if (m_pWarp != NULL)
					{// NULL�ȊO�̏ꍇ
						for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
						{// ���[�v�J�E���g
							if (nCntWarp == 0)
							{// �J�E���g���O��������
								bool bCheck = false;	// �`�F�b�N
								do
								{// ���[�v
									bCheck = false;		// ������
									m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// �p�^�[���ݒ�

									do
									{// ���[�v
										m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// �p�^�[���ݒ�
									} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// �p�^�[�����Ⴄ�Ȃ甲����

									if (2 < m_nPatternMax)
									{// �p�^�[�����Q��葽��
										if ((m_pPattern[m_pWarp[nCntWarp].nPattern_1].bStart == true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true) ||
											(m_pPattern[m_pWarp[nCntWarp].nPattern_1].bEnd == true && m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true))
										{// �X�^�[�g�ƃG���h�����΂ꂽ
											bCheck = true;		// �`�F�b�N
										}
									}
								} while (bCheck != false);
							}
							else
							{// �J�E���g���P�ȏゾ������
								bool bCheck = false;	// �`�F�b�N
								do
								{// ���[�v
									bCheck = false;		// ������
									m_pWarp[nCntWarp].nPattern_1 = rand() % m_nPatternMax;		// �p�^�[���ݒ�

									for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
									{// ���[�v�J�E���g
										if (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_1 ||
											m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntCheck].nPattern_2)
										{// �����ԍ�������
											bCheck = true;		// �`�F�b�N
											break;
										}
									}
								} while (bCheck != false);

								do
								{// ���[�v
									bCheck = false;		// �`�F�b�N
									do
									{// ���[�v
										m_pWarp[nCntWarp].nPattern_2 = rand() % m_nPatternMax;	// �p�^�[���ݒ�
									} while (m_pWarp[nCntWarp].nPattern_1 == m_pWarp[nCntWarp].nPattern_2);		// �p�^�[�����Ⴄ�Ȃ甲����

									if (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern == 2 &&
										(m_pPattern[m_pWarp[nCntWarp].nPattern_2].bStart == true || m_pPattern[m_pWarp[nCntWarp].nPattern_2].bEnd == true))
									{// �p�^�[���̃G���A�������Q + �i�X�^�[�g������A�G���h������j
										bCheck = true;	// �`�F�b�N
									}
								} while (bCheck != false);
							}

							bool bCheck = false;	// �`�F�b�N
							do
							{// ���[�v
								bCheck = false;		// ������
								do
								{// ���[�v
									m_pWarp[nCntWarp].nArea_1 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_1].nMaxPattern;		// �G���A�ݒ�
								} while (m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_START ||
									m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1] == AREA_END);		// �X�^�[�g�ƃG���h�ȊO�������甲����
								m_pWarp[nCntWarp].nArea_1 = m_pPattern[m_pWarp[nCntWarp].nPattern_1].nPattern[m_pWarp[nCntWarp].nArea_1];		// �G���A�ԍ��ݒ�

								do
								{
									m_pWarp[nCntWarp].nArea_2 = rand() % m_pPattern[m_pWarp[nCntWarp].nPattern_2].nMaxPattern;		// �G���A�ݒ�
								} while (m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_START ||
									m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2] == AREA_END);		// �X�^�[�g�ƃG���h�ȊO�������甲����
								m_pWarp[nCntWarp].nArea_2 = m_pPattern[m_pWarp[nCntWarp].nPattern_2].nPattern[m_pWarp[nCntWarp].nArea_2];		// �G���A�ԍ��ݒ�

								for (int nCntCheck = 0; nCntCheck < nCntWarp; nCntCheck++)
								{// ���[�v�J�E���g
									if (m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_1 ||
										m_pWarp[nCntWarp].nArea_1 == m_pWarp[nCntCheck].nArea_2 ||
										m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_1 ||
										m_pWarp[nCntWarp].nArea_2 == m_pWarp[nCntCheck].nArea_2)
									{// �G���A������
										bCheck = true;		// �`�F�b�N
										break;
									}
								}
							} while (bCheck != false);
						}
					}
				}

				delete[] nRouteCounter;	// �j��
				delete[] nRouteEnd;		// �j��
			}
		}
	}

	int nMaxVtx = m_pField->GetMaxVtx();	// ���_�擾

	// ���_�ʒu�ݒ�
	for (int nCntVtx = 0; nCntVtx < nMaxVtx; nCntVtx++)
	{
		D3DXVECTOR3 posVtx = m_pField->GetVtx(nCntVtx);

		/*if (posVtx.x < -1500.0f || 1500.0f < posVtx.x || posVtx.z < -1500.0f || 1500.0f < posVtx.z)
		{
		posVtx.y = 80.0f + (rand() % 80);

		m_pField->SetVtx(nCntVtx, posVtx);
		m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}
		else*/
		{
			posVtx.y = 80.0f + (rand() % 80);			// �ʒu�ݒ�

			// ���_�A�F�ݒ�
			m_pField->SetVtx(nCntVtx, posVtx);
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
			m_pField->SetCol(nCntVtx, D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

			// �G���A
			if (m_pArea != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCount = 0; nCount < m_nAreaMax; nCount++)
				{// �G���A�J�E���g
					// ���S����̈ʒu�v�Z
					float fRange = sqrtf(powf(m_pArea[nCount].pos.x - posVtx.x, 2.0f) + powf(m_pArea[nCount].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[nCount].fRange)
					{// �͈͓���������
						posVtx.y = 0.0f - (rand() % 15);	// �ʒu�ݒ�

						// ���_�A�F�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						//if (m_pArea[nCount].nArea == AREA_START || m_pArea[nCount].nArea == AREA_END)
						//	m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

						/*if (m_pWarp != NULL)
						{
							for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
							{
								if (m_pWarp[nCntWarp].nArea_1 == nCount || m_pWarp[nCntWarp].nArea_2 == nCount)
								{
									if (nCntWarp == 0)
										m_pField->SetCol(nCntVtx, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
									else
										m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

									break;
								}
							}
						}*/

						break;
					}					
				}
			}

			// ���[�g
			if (m_pRoute != NULL)
			{// NULL�ȊO�̏ꍇ
				for (int nCount = 0; nCount < m_nRouteMax; nCount++)
				{// ���[�g�J�E���g
					D3DXVECTOR3 vecA, vecC;		// �v�Z�p�x�N�g���i���_���m�A���_��POS�j
					int nCheck = 0;				// �`�F�b�N
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						vecA = m_pRoute[nCount].pos[nCntPos] - m_pRoute[nCount].pos[(nCntPos + 1) % 4];
						vecC = posVtx - m_pRoute[nCount].pos[(nCntPos + 1) % 4];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
						{// �͈͓���
							nCheck++;	// �J�E���g
						}
					}

					if (nCheck == 4)
					{// �͈͓���������
						posVtx.y = 0.0f - (rand() % 15);	// �ʒu�ݒ�

						// ���_�A�F�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
						m_pField->SetCol(nCntVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}

			// ���[�v
			if (m_pWarp != NULL)
			{
				for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
				{
					float fRange;
					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_1].fRange * 0.2f)
					{// �͈͓���������
						posVtx.y = 0.0f;		// �ʒu�ݒ�

						// ���_�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
					}

					fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - posVtx.x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - posVtx.z, 2.0f));
					if (fRange < m_pArea[m_pWarp[nCntWarp].nArea_2].fRange * 0.2f)
					{// �͈͓���������
						posVtx.y = 0.0f;		// �ʒu�ݒ�

						// ���_�ݒ�
						m_pField->SetVtx(nCntVtx, posVtx);
					}
				}
			}
		}
	}
	m_pField->SetVecNor();	// �@���v�Z
}

//=============================================================================
// �I�u�W�F�N�g�ݒ菈��								(public)	*** CMap_04 ***
//=============================================================================
void CMap_04::SetObject(void)
{
	if (1 < m_nPatternMax)
	{// ���[�v�����݂���
		// ���[�v���f���v�Z
		m_nModelWarp = (m_nPatternMax - 1) * 2;

		// ���[�v���f��
		if (m_pModelWarp == NULL && m_ModelInfo != NULL)
		{// NULL�̏ꍇ
			m_pModelWarp = new CModel[m_nModelWarp];	// �������m��

			for (int nCntModel = 0; nCntModel < m_nModelWarp; nCntModel++)
			{// ���f�����J�E���g
				// �����l�ݒ�
				m_pModelWarp[nCntModel].SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_ModelInfo[0].pos, m_ModelInfo[0].rot);
				// ����������
				m_pModelWarp[nCntModel].Init();

				// �e���f����ݒ�
				m_pModelWarp[nCntModel].SetParent(NULL);

				if (m_pWarpCylinder[nCntModel] == NULL)
				{// NULL�ȊO�̏ꍇ
					m_pWarpCylinder[nCntModel] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);

					// �����l�ݒ�
					m_pWarpCylinder[nCntModel]->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 10.0f));
					// ����������
					m_pWarpCylinder[nCntModel]->Init();
					m_pWarpCylinder[nCntModel]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
					m_pWarpCylinder[nCntModel]->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
				}
			}
		}

		for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
		{// �p�^�[���J�E���g
			if (m_pModelWarp != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pModelWarp[0 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pModelWarp[1 + (nCntWarp * 2)].Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);
			}

			if (m_pWarpCylinder[0 + (nCntWarp * 2)] != NULL && m_pWarpCylinder[1 + (nCntWarp * 2)] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pWarpCylinder[0 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_1].pos);
				m_pWarpCylinder[1 + (nCntWarp * 2)]->Setpos(m_pArea[m_pWarp[nCntWarp].nArea_2].pos);

				if (nCntWarp == 0)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(0.1f, 0.3f, 1.0f, 0.8f));
					}
				}
				else if (nCntWarp == 1)
				{
					for (int nCount = 0; nCount < m_pWarpCylinder[0 + (nCntWarp * 2)]->GetMaxVtx(); nCount++)
					{
						m_pWarpCylinder[0 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
						m_pWarpCylinder[1 + (nCntWarp * 2)]->SetCol(nCount, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.8f));
					}
				}
			}
		}
	}

	if (m_pGoalCylinder == NULL)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{
			m_pGoalCylinder = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GOAL);

			m_pGoalCylinder->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pArea[nCntArea].pos + D3DXVECTOR3(0.0f, -50.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
			m_pGoalCylinder->Init();
			m_pGoalCylinder->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
			m_pGoalCylinder->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
		}
	}

	if (m_pIcon == NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIcon = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_MAPICON);

		m_pIcon->SetInitAll(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		m_pIcon->Init();
		m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
		m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_NOTEQUAL);
		m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CScene		*pScene		= NULL;		// �V�[��
	CScene		*pSceneNext	= NULL;		// ���V�[��
	CSceneMesh	*pMesh		= NULL;		// ���b�V��
	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g
					break;
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	int nNumSet = 28;
	m_nGrassMax = m_nAreaMax * nNumSet;

	if (m_pGrass == NULL)
	{
		m_pGrass = new GRASS[m_nGrassMax];

		if (m_pGrass != NULL)
		{
			for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
			{
				for (int nCntGrass = 0; nCntGrass < nNumSet; nCntGrass++)
				{
					D3DXVECTOR3 pos;		// �ʒu
					float fAngle = (float)(rand() % (int)(D3DX_PI * 100.0f * 2.0f)) * 0.01f;
					float fLenght = (float)(rand() % (int)(m_pArea[nCntArea].fRange - 150.0f) + 100.0f);
					// �ʒu�v�Z
					pos = m_pArea[nCntArea].pos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght;

					int nNum = nCntArea * nNumSet + nCntGrass;

					// �����l�ݒ�
					m_pGrass[nNum].nCounter = 0;
					m_pGrass[nNum].pos = pos;
					m_pGrass[nNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					for (int nCount = 0; nCount < m_GRASS_NUM; nCount++)
					{// ���J�E���g
						D3DXVECTOR3 size = D3DXVECTOR3(12.0f, 14.0f, 0.0f);
						D3DXVECTOR3 rot = m_pGrass[nNum].rot + D3DXVECTOR3(0.0f, (D3DX_PI * 2.0f / (float)(m_GRASS_NUM * 2)) * nCount, 0.0f);

						float fAngle = -D3DX_PI + ((float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f);
						float fLenght = -5.0f + (float)(rand() % 10);

						D3DXVECTOR3 posLeft = m_pGrass[nNum].pos +
							D3DXVECTOR3(sinf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f)), 7.0f, cosf(rot.y - (D3DX_PI * 0.5f)) * ((size.x * 2.0f * 1.0f))) +
							(D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)) * fLenght);

						for (int nCntNum = 0; nCntNum < m_GRASS_LINE; nCntNum++)
						{
							m_pGrass[nNum].pGrass[nCount][nCntNum] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_GRASS);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->SetInitAll(posLeft, rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
							m_pGrass[nNum].pGrass[nCount][nCntNum]->Init();
							m_pGrass[nNum].pGrass[nCount][nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GRASS));
							m_pGrass[nNum].pGrass[nCount][nCntNum]->DrawEdit(false, false, true, false, true, 0, D3DCULL_NONE);

							if (pMesh != NULL)
							{
								D3DXVECTOR3 pos = m_pGrass[nNum].pGrass[nCount][nCntNum]->Getpos();
								pos.y = pMesh->GetHeight(pos);
								pos.y += 7.0f;
								m_pGrass[nNum].pGrass[nCount][nCntNum]->Setpos(pos);
							}

							m_pGrass[nNum].posCenter[nCount][nCntNum] = posLeft;

							D3DXVECTOR3 posVtx[4];
							m_pGrass[nNum].pGrass[nCount][nCntNum]->GetVtxSize(posVtx);
							m_pGrass[nNum].posVtx[nCount][nCntNum][0] = posVtx[0];
							m_pGrass[nNum].posVtx[nCount][nCntNum][1] = posVtx[1];

							m_pGrass[nNum].bFall[nCount][nCntNum] = false;
							m_pGrass[nNum].nFallCounter[nCount][nCntNum] = 0;

							posLeft += D3DXVECTOR3(sinf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f), 0.0f, cosf(rot.y + (D3DX_PI * 0.5f)) * (size.x * 2.0f));
						}
					}
				}
			}
		}
	}

	/*if (m_pModelObject == NULL)
	{
		m_nModelObject = 500;
		m_pModelObject = new CModel[m_nModelObject];

		for (int nCntModel = 0; nCntModel < m_nModelObject; nCntModel++)
		{
			// �����l�ݒ�
			m_pModelObject[nCntModel].SetInitAll(m_pTexture[1], m_pMesh[1], m_pBuffMat[1], m_nNumMat[1], m_ModelInfo[1].pos, m_ModelInfo[1].rot);
			// ����������
			m_pModelObject[nCntModel].Init();

			// �e���f����ݒ�
			m_pModelObject[nCntModel].SetParent(NULL);

			D3DXVECTOR3 pos;
			bool bCheck = false;
			do
			{
				bCheck = false;

				pos.x = -2000.0f + (float)(rand() % (2000 * 2));
				pos.y = 0.0f;
				pos.z = -2000.0f + (float)(rand() % (2000 * 2));

				if (bCheck == false)
				{
					for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
					{
						float fRange = sqrtf(powf(pos.x - m_pArea[nCntArea].pos.x, 2.0f) + powf(pos.z - m_pArea[nCntArea].pos.z, 2.0f));
						if (fRange < m_pArea[nCntArea].fRange)
						{
							bCheck = true;
							break;
						}
					}
				}
				if (bCheck == false)
				{
					for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
					{
						int nCheck = 0;
						for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
						{
							D3DXVECTOR3 vecA, vecC;
							vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];
							vecC = pos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % 4];

							if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) < 0)
							{// �͈͓���
								nCheck++;	// �J�E���g
							}
						}

						if (nCheck == 4)
						{// �͈͓���������
							bCheck = true;
							break;
						}
					}
				}
			} while (bCheck != false);

			if (pMesh != NULL)
			{
				pos.y = pMesh->GetHeight(pos);
			}

			m_pModelObject[nCntModel].Setpos(pos);
		}
	}*/

	CGame *pGame = CManager::GetGame();
	CGame::GAME game = CGame::GAME_TUTORIAL;
	if (pGame != NULL)
	{
		game = pGame->GetGame();
	}

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// �G���A�J�E���g
		//if (m_pArea[nCntArea].nArea == AREA_START)
		{
			// �G
			if (game == CGame::GAME_PLAY_B)
				CEnemy::Create(CEnemy::TYPE_B_SLIME, m_pArea[nCntArea].pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			break;
		}
	}
}

//=============================================================================
// �t�B�[���h�Փ˔��菈��								(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange)
{
	bool bLand = false;
	bool bAreaCheck			= false;	// �G���A�`�F�b�N����
	bool bAreaCheckOld		= false;	// �G���A�`�F�b�N�O��
	bool bRouteCheck		= false;	// ���[�g�`�F�b�N����
	bool bRouteCheckOld		= false;	// ���[�g�`�F�b�N�O��
	int nAreaCheckNum		= 0;		// �G���A�`�F�b�N�ԍ�
	int nRouteCheckNum		= 0;		// ���[�g�`�F�b�N�ԍ�

	bool bAreaIn			= false;	// �G���A�̒�
	int nAreaInNum			= 0;		// �G���A�̒��ԍ�
	bool bRouteIn			= false;	// ���[�g�̒�
	int nRouteInNum			= 0;		// ���[�g�̒��ԍ�

	// �G���A�`�F�b�N
	if (bAreaCheck == false || bAreaCheckOld == false)
	{// �ǂ����FALSE
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// �G���A�J�E���g
			// �G���A�`�F�b�N
			bAreaCheck		= false;
			bAreaCheckOld	= false;
			nAreaCheckNum	= nCntArea;

			float fRange;	// �͈�
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPosOld->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPosOld->z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// �͈͓���
				bAreaCheckOld	= true;			// �͈͓�
				bAreaIn			= true;			// �G���A���ɂ���
				nAreaInNum		= nCntArea;		// �G���A���̔ԍ�
			}

			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
			if (m_pArea[nCntArea].fRange * MC_AREACOLL_NOW <= fRange)
			{// �͈͊O��
				bAreaCheck = true;		// �͈͓�
			}

			// �ǂ����TRUE�Ȃ珈���𔲂���
			if (bAreaCheck == true && bAreaCheckOld == true) break;
		}
	}

	if (bRouteCheck == false || bRouteCheckOld == false)
	{// �ǂ����FALSE
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ���[�g�J�E���g
			// ���[�g�`�F�b�N
			bRouteCheck		= false;
			bRouteCheckOld	= false;
			nRouteCheckNum	= nCntRoute;

			int nHit	= 0;	// ���݃q�b�g��
			int nHitOld = 0;	// �O��q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// �O�όv�Z
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = *pPos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}

				// �O�όv�Z
				vecAOld = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecCOld = *pPosOld - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecAOld.z * vecCOld.x) - (vecAOld.x * vecCOld.z)) <= 0)
				{// �͈͓���
					nHitOld++;	// �J�E���g
				}
			}

			if (nHit != 4)
			{// �͈͊O��
				bRouteCheck = true;			// �͈͊O
			}
			if (nHitOld == 4)
			{// �͈͓���
				bRouteCheckOld	= true;			// �͈͓�
				bRouteIn		= true;			// ���[�g���ɂ���
				nRouteInNum		= nCntRoute;	// ���[�g���̔ԍ�
			}

			// �ǂ����TRUE�Ȃ珈���𔲂���
			if (bRouteCheck == true && bRouteCheckOld == true) break;
		}
	}

	bool bArea = false;		// �G���A����

	// �G���A����
	if (bAreaCheck == true && bAreaCheckOld == true)
	{// �ǂ����TRUE
		float fRouteAngle		= 0.0f;		// ���[�g�p�x
		float fRouteAngleRange	= 0.0f;		// ���[�g�p�x�͈�
		float fRouteAngleL		= 0.0f;		// ���[�g�p�xL
		float fRouteAngleR		= 0.0f;		// ���[�g�p�xR
		bool bRoute				= false;	// ���[�g�ɂ���

		// �G���A���S����v���C���[�̊p�x�v�Z
		float fAngle = atan2f(pPos->x - m_pArea[nAreaCheckNum].pos.x, pPos->z - m_pArea[nAreaCheckNum].pos.z);

		if (bRouteIn == true)
		{// ���[�g���ɂ���
			D3DXVECTOR3 posLei;
			if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posStart)
			{// �X�^�[�g�Ɠ�����������
				posLei = m_pRoute[nRouteInNum].posCol[0];	// ���C�p�ʒu

				bool bCheck = false;	// �`�F�b�N
				while (true)
				{// ���[�v
					// 
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{
						break;
					}
					else
					{
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle)) * 1.0f;
					}
				}
			}
			else if (m_pArea[nAreaCheckNum].pos == m_pRoute[nRouteInNum].posEnd)
			{// �G���h�Ɠ�����������
				posLei = m_pRoute[nRouteInNum].posCol[2];	// ���C�p�ʒu

				bool bCheck = false;	// �`�F�b�N
				while (true)
				{// ���[�v
					// ���S����̈ʒu�v�Z
					float fRange = sqrtf(powf(m_pArea[nAreaCheckNum].pos.x - posLei.x, 2.0f) + powf(m_pArea[nAreaCheckNum].pos.z - posLei.z, 2.0f));
					if (m_pArea[nAreaCheckNum].fRange < fRange)
					{// �͈͊O
						break;
					}
					else
					{// �͈͓�
						// ���C��i�߂�
						posLei = posLei + D3DXVECTOR3(sinf(m_pRoute[nRouteInNum].fAngle + D3DX_PI), 0.0f, cosf(m_pRoute[nRouteInNum].fAngle + D3DX_PI)) * 1.0f;
					}
				}
			}

			// ���[�g�p�x�v�Z
			fRouteAngle = atan2f(m_pRoute[nRouteInNum].posCenter.x - m_pArea[nAreaCheckNum].pos.x, m_pRoute[nRouteInNum].posCenter.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = atan2f(posLei.x - m_pArea[nAreaCheckNum].pos.x, posLei.z - m_pArea[nAreaCheckNum].pos.z);
			fRouteAngleRange = fRouteAngle - fRouteAngleRange;
			fRouteAngleRange = fabsf(fRouteAngleRange);

			if (fRouteAngleRange < -D3DX_PI)
				fRouteAngleRange += D3DX_PI * 2.0f;
			if (D3DX_PI < fRouteAngleRange)
				fRouteAngleRange -= D3DX_PI * 2.0f;

			fRouteAngleRange = fabsf(fRouteAngleRange);

			fRouteAngleL = fRouteAngle - fRouteAngleRange;
			fRouteAngleR = fRouteAngle + fRouteAngleRange;

			if (fRouteAngleL < fAngle && fAngle < fRouteAngleR)
			{
				bRoute = true;
			}
		}

		if (bRouteIn == false || (bRouteIn == true && bRoute == false))
		{// ���[�g���ɂ��Ȃ��A�i���[�g���ɂ��� + ���[�g�͈͊O�j
			pPos->x = m_pArea[nAreaCheckNum].pos.x + sinf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);
			pPos->z = m_pArea[nAreaCheckNum].pos.z + cosf(fAngle) * (m_pArea[nAreaCheckNum].fRange * MC_AREACOLL_NOW);

			pMove->x = 0.0f;
			pMove->z = 0.0f;
			bArea = true;	// �G���A����
			bLand = true;
		}
	}

	// ���[�g����
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteCheck == true && bRouteCheckOld == true)
		{// �ǂ����TRUE
			// �Z���^�[�v�Z
			float fAngleCenter	= 0.0f;
			float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteCheckNum].posStart.x - m_pRoute[nRouteCheckNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteCheckNum].posStart.z - m_pRoute[nRouteCheckNum].pos[0].z, 2.0f));

			// �O�όv�Z
			D3DXVECTOR3 vecALR, vecCLR;
			vecALR = m_pRoute[nRouteCheckNum].posStart - m_pRoute[nRouteCheckNum].posEnd;
			vecCLR = *pPos - m_pRoute[nRouteCheckNum].posEnd;

			// ���[�g�̉E����������
			if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle - (D3DX_PI * 0.5f);	// ����
			else
				fAngleCenter = m_pRoute[nRouteCheckNum].fAngle + (D3DX_PI * 0.5f);	// �E��

			// �Z���^�[�v�Z
			D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

			// �Z���^�[�ւ̊p�x�v�Z
			float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

			int nHit = 0;	// �q�b�g��

			while (1)
			{
				nHit = 0;	// �q�b�g��
				for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
				{// �ʒu�J�E���g
					// �O�όv�Z
					D3DXVECTOR3 vecA, vecC;
					vecA = m_pRoute[nRouteCheckNum].posCol[nCntPos] - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
					vecC = *pPos - m_pRoute[nRouteCheckNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

					if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
					{// �͈͓�
						nHit++;
					}
				}

				if (nHit == 4)
				{// �͈͓�
					bLand = true;
					break;		// �����𔲂���
				}
				else
				{// �͈͊O
					// �ʒu���X�V�i�߂Â���j
					pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
					pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
				}
			}

			pMove->x = 0.0f;
			pMove->z = 0.0f;
		}
	}

	// ���̑��̔���i�������ꍇ�j
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteIn == false)
		{// ���[�g�ɂ��Ȃ�
			if (bRouteCheck == true && bRouteCheckOld == false)
			{// ���݂��͈͊O + �O�񂪔͈͊O
				int nRouteNum = 0;		// ���[�g�ԍ�

				for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
				{// ���[�g�J�E���g
					int nHit = 0;	// �q�b�g��

					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
						vecA = m_pRoute[nCntRoute].pos[nCntPos] - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nCntRoute].pos[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// �͈͓�
							nHit++;		// �J�E���g
						}
					}

					if (nHit == 4)
					{// �͈͓�
						nRouteNum = nCntRoute;		// �ԍ����L��
						break;
					}
				}

				// �Z���^�[�v�Z
				float fAngleCenter	= 0.0f;
				float fLenghtCenter = sqrtf(powf(m_pRoute[nRouteNum].posStart.x - m_pRoute[nRouteNum].pos[0].x, 2.0f) + powf(m_pRoute[nRouteNum].posStart.z - m_pRoute[nRouteNum].pos[0].z, 2.0f));

				// �O�όv�Z
				D3DXVECTOR3 vecALR, vecCLR;
				vecALR = m_pRoute[nRouteNum].posStart - m_pRoute[nRouteNum].posEnd;
				vecCLR = *pPos - m_pRoute[nRouteNum].posEnd;

				// ���[�g�̉E����������
				if (((vecALR.z * vecCLR.x) - (vecALR.x * vecCLR.z)) <= 0)
					fAngleCenter = m_pRoute[nRouteNum].fAngle - (D3DX_PI * 0.5f);	// ����
				else
					fAngleCenter = m_pRoute[nRouteNum].fAngle + (D3DX_PI * 0.5f);	// �E��

				// �Z���^�[�v�Z
				D3DXVECTOR3 posCenter = *pPos + D3DXVECTOR3(sinf(fAngleCenter), 0.0f, cosf(fAngleCenter)) * fLenghtCenter;

				// �Z���^�[�ւ̊p�x�v�Z
				float fAngle = atan2f(pPos->x - posCenter.x, pPos->z - posCenter.z);

				int nHit = 0;	// �q�b�g��

				while (1)
				{
					nHit = 0;	// �q�b�g��
					for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
					{// �ʒu�J�E���g
						// �O�όv�Z
						D3DXVECTOR3 vecA, vecC;
						vecA = m_pRoute[nRouteNum].posCol[nCntPos] - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
						vecC = *pPos - m_pRoute[nRouteNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

						if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
						{// �͈͓�
							nHit++;
						}
					}

					if (nHit == 4)
					{// �͈͓�
						bLand = true;
						break;		// �����𔲂���
					}
					else
					{// �͈͊O
						// �ʒu���X�V�i�߂Â���j
						pPos->x = pPos->x + sinf(fAngle + D3DX_PI) * 1.0f;
						pPos->z = pPos->z + cosf(fAngle + D3DX_PI) * 1.0f;
					}
				}

				pMove->x = 0.0f;
				pMove->z = 0.0f;
			}
		}
	}

	return bLand;
}

//=============================================================================
// �G���A���[�g���ɂ��邩���菈��						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionInOut(D3DXVECTOR3 pos)
{
	bool bLand = false;

	if (bLand == false)
	{
		for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
		{// �G���A�J�E���g
			float fRange;	// �͈�
			fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pos.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pos.z, 2.0f));
			if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
			{// �͈͓���
				bLand = true;
				break;
			}
		}
	}

	if (bLand == false)
	{
		for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
		{// ���[�g�J�E���g
			int nHit = 0;	// ���݃q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC;
				// �O�όv�Z
				vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = pos - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}
			}

			if (nHit == 4)
			{// �͈͓���
				bLand = true;
				break;
			}
		}
	}

	return bLand;
}

//=============================================================================
// �t�B�[���h���[�v�Փ˔��菈��						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp)
{
	int nWarpNow = 0;
	int nWarpNext = 0;
	bool bUseWarp = false;

	for (int nCntWarp = 0; nCntWarp < m_nPatternMax - 1; nCntWarp++)
	{
		float fRange;
		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_1].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNow = m_pWarp[nCntWarp].nArea_1;
			nWarpNext = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}

		fRange = sqrtf(powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.x - pPos->x, 2.0f) + powf(m_pArea[m_pWarp[nCntWarp].nArea_2].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			nWarpNext = m_pWarp[nCntWarp].nArea_1;
			nWarpNow = m_pWarp[nCntWarp].nArea_2;
			bUseWarp = true;
			break;
		}
	}

	if (bUseWarp == true)
	{
		*pPosWarp = m_pArea[nWarpNext].pos;

		if (m_pIcon != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nWarpNow].pos.x, 100.0f, m_pArea[nWarpNow].pos.z));
			m_bDrawIcon = true;			// �A�C�R���`��
		}
	}

	return bUseWarp;
}

//=============================================================================
// �t�B�[���h�S�[���Փ˔��菈��						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionGoal(D3DXVECTOR3 *pPos)
{
	bool bUseGoal = false;;

	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{
		float fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - pPos->x, 2.0f) + powf(m_pArea[nCntArea].pos.z - pPos->z, 2.0f));
		if (fRange < 35.0f)
		{
			bUseGoal = true;

			if (m_pIcon != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pIcon->Setpos(D3DXVECTOR3(m_pArea[nCntArea].pos.x, 100.0f, m_pArea[nCntArea].pos.z));
				m_bDrawIcon = true;			// �A�C�R���`��
			}

			break;
		}
	}

	return bUseGoal;
}

//=============================================================================
// �t�B�[���h�J�����Փ˔��菈��						(public)	*** CMap_04 ***
//=============================================================================
bool CMap_04::CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	bool bAreaIn	= false;	// �G���A�̒�
	int nAreaInNum	= 0;		// �G���A�̒��ԍ�
	bool bRouteIn	= false;	// ���[�g�̒�
	int nRouteInNum = 0;		// ���[�g�̒��ԍ�
	bool bRouteInV	= false;	// ���_���[�g�̒�

	// �G���A�`�F�b�N
	for (int nCntArea = 0; nCntArea < m_nAreaMax; nCntArea++)
	{// �G���A�J�E���g
		float fRange;	// �͈�
		fRange = sqrtf(powf(m_pArea[nCntArea].pos.x - posR.x, 2.0f) + powf(m_pArea[nCntArea].pos.z - posR.z, 2.0f));
		if (fRange <= m_pArea[nCntArea].fRange * MC_AREACOLL_OLD)
		{// �͈͓���
			bAreaIn		= true;			// �G���A���ɂ���
			nAreaInNum	= nCntArea;		// �G���A���̔ԍ�
			break;
		}
	}

	// ���[�g�`�F�b�N
	for (int nCntRoute = 0; nCntRoute < m_nRouteMax; nCntRoute++)
	{// ���[�g�J�E���g
		int nHit = 0;	// ���݃q�b�g��
		int nHitV = 0;	// ���݃q�b�g��

		for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
		{// �ʒu�J�E���g
			D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
			// �O�όv�Z
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posR - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// �͈͓���
				nHit++;		// �J�E���g
			}

			// �O�όv�Z
			vecA = m_pRoute[nCntRoute].posCol[nCntPos] - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
			vecC = posV - m_pRoute[nCntRoute].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

			if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
			{// �͈͓���
				nHitV++;		// �J�E���g
			}
		}

		if (nHit == 4)
		{// �͈͓���
			bRouteIn	= true;			// �͈͊O
			nRouteInNum = nCntRoute;	// �G���A���̔ԍ�

			if (nHitV == 4)
			{// �͈͓���
				bRouteInV = true;			// �͈͊O
			}

			break;
		}


	}

	bool bArea = false;		// �G���A����

	// �G���A����
	if (bAreaIn == true)
	{// �G���A���ɂ���
		// �������v�Z
		float fRange = sqrtf(powf(m_pArea[nAreaInNum].pos.x - posV.x, 2.0f) + powf(m_pArea[nAreaInNum].pos.z - posV.z, 2.0f));
		if (fRange < m_pArea[nAreaInNum].fRange)
		{// �͈͓�
			return true;
		}
		else
		{// �͈͊O
			if (bRouteInV == true)
			{// ���_�����[�g���ɂ���
				return true;
			}
			else
			{// ���_�����[�g���ɂ��Ȃ�
				return false;
			}
		}
	}

	// ���[�g����
	if (bArea == false && bAreaIn == false)
	{// �G���A���肵�Ă��Ȃ� + �G���A�ɂ��Ȃ�
		if (bRouteIn == true)
		{// ���[�g���ɂ���
			int nHit = 0;	// ���݃q�b�g��

			for (int nCntPos = 0; nCntPos < m_ROUTEPOS_NUM; nCntPos++)
			{// �ʒu�J�E���g
				D3DXVECTOR3 vecA, vecC, vecAOld, vecCOld;
				// �O�όv�Z
				vecA = m_pRoute[nRouteInNum].posCol[nCntPos] - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];
				vecC = posV - m_pRoute[nRouteInNum].posCol[(nCntPos + 1) % m_ROUTEPOS_NUM];

				if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
				{// �͈͓���
					nHit++;		// �J�E���g
				}
			}

			if (nHit == 4)
			{// �͈͓���
				return true;
			}
			else
			{// �͈͊O
				return false;
			}
		}
	}

	return false;
}