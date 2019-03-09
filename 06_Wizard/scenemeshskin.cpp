//=============================================================================
//
// �V�[�����b�V���I�[�r�b�g���� [scenemeshorbit.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scenemeshskin.h"	// �V�[�����b�V���X�L��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "font.h"			// �t�H���g

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_NAME		"data\\TEXTURE\\MODE\\GAME\\gradation005.jpg"

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
// �R���X�g���N�^								(public)	*** CSceneMeshSkin ***
//=============================================================================
CSceneMeshSkin::CSceneMeshSkin(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture	= NULL;										// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff	= NULL;										// ���_�o�b�t�@���ւ̃|�C���^

	for (int nCount = 0; nCount < MAX_VTX; nCount++)
		m_aPosVtx[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	for (int nCount = 0; nCount < MAX_OFFSET; nCount++)
		m_aOffSet[nCount] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �I�t�Z�b�g
}

//=============================================================================
// �f�X�g���N�^								(public)	*** CSceneMeshSkin ***
//=============================================================================
CSceneMeshSkin::~CSceneMeshSkin()
{

}

//=============================================================================
// ���[�h����								(public)	*** CSceneMeshSkin ***
//=============================================================================
HRESULT CSceneMeshSkin::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����								(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Unload(void)
{

}

//=============================================================================
// �m�ۏ���									(public)	*** CSceneMeshSkin ***
//=============================================================================
CSceneMeshSkin *CSceneMeshSkin::Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1)
{
	CSceneMeshSkin *pSkin = NULL;			// �|�C���^

	if (pSkin == NULL)
	{// NULL�̏ꍇ
		pSkin = new CSceneMeshSkin(3, CScene::OBJTYPE_MESHFIELD);		// �������m��
	}

	if (pSkin != NULL)
	{// NULL�ȊO�̏ꍇ
		pSkin->m_aOffSet[0] = OffSet_0;	// �I�t�Z�b�g�O
		pSkin->m_aOffSet[1] = OffSet_1;	// �I�t�Z�b�g�P
		pSkin->Init();					// ����������
	}

	return pSkin;		// �l��Ԃ�
}

//=============================================================================
// ����������								(public)	*** CSceneMeshSkin ***
//=============================================================================
HRESULT CSceneMeshSkin::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VTX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_VTX; nCount++)
	{// ���_���J�E���g
		// ���_����ݒ�
		//pVtx[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �@���̐ݒ�
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// ���_�J���[�ݒ�
		pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		if (nCount % 2 == 0)
		{// ��
			// ���_����ݒ�
			pVtx[nCount].pos = D3DXVECTOR3(50.0f * (nCount / 2), 100.0f, 0.0f);
			// �e�N�X�`���ݒ�
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (MAX_VTX * 0.5f)) * (nCount / 2)), 1.0f);
		}
		else
		{// ��
			// ���_����ݒ�
			pVtx[nCount].pos = D3DXVECTOR3(50.0f * (nCount / 2), 0.0f, 0.0f);
			// �e�N�X�`���ݒ�
			pVtx[nCount].tex = D3DXVECTOR2(((1.0f / (MAX_VTX * 0.5f)) * (nCount / 2)), 0.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Uninit(void)
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
// �X�V����										(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Update(void)
{
	static D3DXVECTOR3 pos[2];
	D3DXVECTOR3 skin;

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_6))
	{
		pos[0].z -= 2.0f;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_7))
	{
		pos[0].z += 2.0f;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_8))
	{
		pos[1].z -= 2.0f;
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_9))
	{
		pos[1].z += 2.0f;
	}

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_VTX; nCount++)
	{// �J�E���g
		if (nCount / 2 == 0)
		{
			skin.x = ((pos[0].x + pVtx[nCount].pos.x) * 1.0f) + ((pos[1].x + pVtx[nCount].pos.x) * 0.0f);
			skin.y = ((pos[0].y + pVtx[nCount].pos.y) * 1.0f) + ((pos[1].y + pVtx[nCount].pos.y) * 0.0f);
			skin.z = ((pos[0].z + pVtx[nCount].pos.z) * 1.0f) + ((pos[1].z + pVtx[nCount].pos.z) * 0.0f);
		}
		else if (nCount / 2 == 1)
		{
			skin.x = ((pos[0].x + pVtx[nCount].pos.x) * 0.2f) + ((pos[1].x + pVtx[nCount].pos.x) * 0.8f);
			skin.y = ((pos[0].y + pVtx[nCount].pos.y) * 0.2f) + ((pos[1].y + pVtx[nCount].pos.y) * 0.8f);
			skin.z = ((pos[0].z + pVtx[nCount].pos.z) * 0.2f) + ((pos[1].z + pVtx[nCount].pos.z) * 0.8f);
		}
		else if (nCount / 2 == 2)
		{
			skin.x = ((pos[0].x + pVtx[nCount].pos.x) * 0.0f) + ((pos[1].x + pVtx[nCount].pos.x) * 1.0f);
			skin.y = ((pos[0].y + pVtx[nCount].pos.y) * 0.0f) + ((pos[1].y + pVtx[nCount].pos.y) * 1.0f);
			skin.z = ((pos[0].z + pVtx[nCount].pos.z) * 0.0f) + ((pos[1].z + pVtx[nCount].pos.z) * 1.0f);
		}

		// ���_����ݒ�		
		pVtx[nCount].pos = skin;
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��										(public)	*** CSceneMeshSkin ***
//=============================================================================
void CSceneMeshSkin::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	D3DXMATRIX mtxWorld;												// �v�Z�p�}�g���b�N�X

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ���ʂ��J�����O

	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, NULL);
	//pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, MAX_VTX - 2);

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ���C�g�e���󂯂Ȃ�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
}