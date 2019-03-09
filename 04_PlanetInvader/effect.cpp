//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "effect.h"			// �G�t�F�N�g
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\effect000.jpg"		// �e�N�X�`��1
#define TEXTURE_NAME_2	"data\\TEXTURE\\effect000.png"		// �e�N�X�`��2
#define TEXTURE_NAME_3	"data\\TEXTURE\\smoke000.png"		// �e�N�X�`��3
#define TEXTURE_NAME_4	"data\\TEXTURE\\tank000.png"		// �e�N�X�`��4

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		CEffect::m_pTexture[CEffect::TEXTURE_MAX] = {};			// �e�N�X�`�����ւ̃|�C���^

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect ***
//=============================================================================
CEffect::CEffect() : CScene3D(CScene::PRIORITY_2)
{
	m_effectType	= EFFECTTYPE_NONE;						// �G�t�F�N�g
	m_nCntTimer		= 0;									// ���ԃJ�E���^
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// �ړ���
	m_sizeDown		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// �傫������
	m_faDown		= 0.0f;									// �A���t�@����
}

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect ***
//=============================================================================
CEffect::CEffect(int) : CScene3D(CScene::PRIORITY_3)
{
	m_effectType	= EFFECTTYPE_NONE;						// �G�t�F�N�g
	m_nCntTimer		= 0;									// ���ԃJ�E���^
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// �ړ���
	m_sizeDown		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);;		// �傫������
	m_faDown		= 0.0f;									// �A���t�@����
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CEffect ***
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ���[�h����							(public)	*** CEffect ***
//=============================================================================
HRESULT CEffect::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pTexture[1]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pTexture[2]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_4, &m_pTexture[3]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CEffect ***
//=============================================================================
void CEffect::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �J�E���g
		if (m_pTexture[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[nCntTex]->Release();		// ���
			m_pTexture[nCntTex] = NULL;			// NULL��
		}
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CEffect ***
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture)
{
	CEffect *pEffect = NULL;				// �|�C���^

	if (pEffect == NULL)
	{// NULL�̏ꍇ
		if (effectType == EFFECTTYPE_PLAYER_T || effectType == EFFECTTYPE_EXPLOSION)
		{// �^�C�g���̂�
			pEffect = new CEffect(0);			// �������m��
		}
		else
		{// ���̑�
			pEffect = new CEffect;				// �������m��
		}
	}

	if (pEffect != NULL)
	{// NULL�ȊO�̏ꍇ
		pEffect->Init(pos, rot, move, size, sizeDown, col, faDown, effectType, texture);		// ����������
		pEffect->SetObjType(CScene::OBJTYPE_EFFECT);							// �I�u�W�F�N�g�^�C�v�ݒ�
		pEffect->BindTexture(m_pTexture[texture]);								// �e�N�X�`���ݒ�
	}

	return pEffect;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CEffect ***
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture)
{
	CScene3D::SetInitAll(pos, rot, size, col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);		// �����l�ݒ�
	CScene3D::Init();																													// ����������

	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();		// �J���������_�擾

	m_TexNum = texture;									// �e�N�X�`���ԍ�
	m_pos = pos - CameraposR;							// �J�����ړ�������
	m_effectType = effectType;							// �G�t�F�N�g�^�C�v
	m_moveInit = move;									// �ړ��ʏ���
	m_sizeDown = sizeDown;								// �傫������
	m_faDown = faDown;									// �A���t�@����

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CEffect ***
//=============================================================================
void CEffect::Uninit(void)
{
	CScene3D::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CEffect ***
//=============================================================================
void CEffect::Update(void)
{	
	CManager::MODE mode = CManager::GetMode();						// ���[�h�擾
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();		// �J���������_�擾
	D3DXVECTOR3 CameraposV = CManager::GetCamera()->GetposV();		// �J���������_�擾
	D3DXVECTOR3 pos = CScene3D::Getpos();		// �ʒu���擾
	D3DXVECTOR3 size = CScene3D::Getsize();		// �傫�����擾
	D3DXCOLOR col = CScene3D::Getcol();			// �F���擾

	bool bDelete = false;						// �j��

	if (mode == CManager::MODE_TUTORIAL || mode == CManager::MODE_GAME)
	{
		// �J�����ɑ΂��Ă̈ʒu��ݒ�
		pos.x = CameraposR.x + m_pos.x;
		pos.z = CameraposR.z + m_pos.z;
	}
	
	m_nCntTimer++;								// �J�E���g

	if (m_effectType == EFFECTTYPE_EXPLOSION)
	{
		if (m_nCntTimer < 30)
		{
			m_move.x = m_moveInit.x;
			m_move.y = (m_moveInit.y - m_move.y) * (m_nCntTimer * 0.03f);
			m_move.z = m_moveInit.z;
		}
	}
	else
	{
		m_move = m_moveInit;
	}

	pos += m_move;								// �ړ��ʂ���
	
	col.a -= m_faDown;							// �A���t�@����

	size += m_sizeDown;							// �傫��������

	CScene3D::Setpos(pos);						// �ʒu��ݒ�
	CScene3D::Setsize(size);					// �傫����ݒ�
	CScene3D::Setcol(col);						// �F��ݒ�
	
	if (mode == CManager::MODE_TUTORIAL || mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		m_pos = pos - CameraposR;				// ���݈ʒu���v�Z
	}

	CScene3D::Update();							// �X�V����

	if (size.x < 0 || size.y < 0 || col.a < 0.0f)
	{// �傫�����O��菬����
		bDelete = true;				// �j������
	}

	if (mode != CManager::MODE_TITLE)
	{
		if (pos.x < CameraposR.x - 260.0f || CameraposR.x + 260.0f < pos.x || pos.z < CameraposR.z - 400.0f || CameraposR.z + 250.0f < pos.z)
		{// �J�����͈͊O�ɏo����
			bDelete = true;				// �j������
		}
	}

	if (bDelete == true)
	{// �j������
		Uninit();					// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CEffect ***
//=============================================================================
void CEffect::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//��r���@�i���傫���j�j
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_TexNum == TEXTURE_EFFECT)
	{
		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CScene3D::Draw();				// �`�揈��

	if (m_TexNum == TEXTURE_EFFECT)
	{
		// ���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//��r���@�i���傫���j�j
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}