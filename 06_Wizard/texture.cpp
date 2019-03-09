//=============================================================================
//
// �e�N�X�`������ [texture.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "texture.h"		// �e�N�X�`��
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
LPDIRECT3DTEXTURE9	CTexture::m_pTexture[CTexture::TYPE_MAX] = { NULL };				// �e�N�X�`�����ւ̃|�C���^
int					CTexture::m_pTexType[CTexture::TYPE_MAX] = { CTexture::TYPE_MAX };	// �e�N�X�`�����
int					CTexture::m_nMaxTexture = 0;										// �e�N�X�`����

const char			*CTexture::m_TexName[CTexture::TYPE_MAX] =
{// �e�N�X�`����
	{ "data\\TEXTURE\\MODE\\OTHER\\press2.png" },				// �v���X
	{ "data\\TEXTURE\\MODE\\OTHER\\number.png" },				// �ԍ�
	{ "data\\TEXTURE\\MODE\\OTHER\\end.png" },					// �I���m�F
	{ "data\\TEXTURE\\MODE\\OTHER\\yesno.png" },				// YESNO
	{ "data\\TEXTURE\\MODE\\OTHER\\button.png" },				// �{�^��
	{ "data\\TEXTURE\\MODE\\TITLE\\titlelogo.png" },			// �^�C�g��
	{ "data\\TEXTURE\\MODE\\TITLE\\titlelogo04.png" },			// �^�C�g���G�t�F�N�g�p
	{ "data\\TEXTURE\\MODE\\TITLE\\menu000.png" },				// �^�C�g�����j���[
	{ "data\\TEXTURE\\MODE\\TITLE\\menu001.png" },				// �^�C�g�����j���[BG
	{ "data\\TEXTURE\\MODE\\GAME\\field003.jpg" },				// �t�B�[���h
	{ "data\\TEXTURE\\MODE\\GAME\\effect000.jpg" },				// �G�t�F�N�g
	{ "data\\TEXTURE\\MODE\\GAME\\map2.png" },					// �}�b�v
	{ "data\\TEXTURE\\MODE\\GAME\\mapicon.png" },				// �}�b�v�A�C�R��
	{ "data\\TEXTURE\\MODE\\GAME\\sky001.png" },				// ��P
	{ "data\\TEXTURE\\MODE\\GAME\\sky002.jpg" },				// ��Q
	{ "data\\TEXTURE\\MODE\\GAME\\grass1.png" },				// ��
	{ "data\\TEXTURE\\MODE\\GAME\\warp000.png" },				// ���[�v
	{ "data\\TEXTURE\\MODE\\GAME\\goal000.png" },				// �S�[��
	{ "data\\TEXTURE\\MODE\\GAME\\stepanim.png" },				// �X�e�b�v
	{ "data\\TEXTURE\\MODE\\GAME\\walk000.png" },				// ����
	{ "data\\TEXTURE\\MODE\\GAME\\skill000.png" },				// �X�L��
	{ "data\\TEXTURE\\MODE\\GAME\\rockon00.png" },				// ���b�N�I���t���[��
	{ "data\\TEXTURE\\MODE\\GAME\\rockon01.png" },				// ���b�N�I���v���C
	{ "data\\TEXTURE\\MODE\\GAME\\frame.png" },					// UI�t���[��
	{ "data\\TEXTURE\\MODE\\GAME\\charicon.png" },				// UI�L����
	{ "data\\TEXTURE\\MODE\\GAME\\hpsp.png" },					// UIHPSP
	{ "data\\TEXTURE\\MODE\\GAME\\weapon.png" },				// UI����
	{ "data\\TEXTURE\\MODE\\GAME\\playername.png" },			// UI�v���C���[��
	{ "data\\TEXTURE\\MODE\\GAME\\enemyname.png" },				// UI�G��
	{ "data\\TEXTURE\\MODE\\GAME\\rank.png" },					// UI�����N
	{ "data\\TEXTURE\\MODE\\GAME\\check.png" },					// �`�F�b�N
	{ "data\\TEXTURE\\MODE\\GAME\\tutorialplus.png" },			// �`���[�g���A���v���X
	{ "data\\TEXTURE\\MODE\\GAME\\tutoriallogo.png" },			// �`���[�g���A�����S
	{ "data\\TEXTURE\\MODE\\GAME\\tutorial.png" },				// �`���[�g���A��
	{ "data\\TEXTURE\\MODE\\GAME\\clear.png" },					// �N���A
	{ "data\\TEXTURE\\MODE\\GAME\\mision.png" },				// �~�b�V����
	{ "data\\TEXTURE\\MODE\\GAME\\skip.png" },					// �X�L�b�v
	{ "data\\TEXTURE\\MODE\\GAME\\pause000.png" },				// �|�[�Y���S
	{ "data\\TEXTURE\\MODE\\GAME\\pause001.png" },				// �|�[�Y���j���[
	{ "data\\TEXTURE\\MODE\\GAME\\pause002.png" },				// �|�[�Y�w�i
	{ "data\\TEXTURE\\MODE\\RESULT\\clear.png" },				// �N���A
	{ "data\\TEXTURE\\MODE\\RESULT\\clear_bg.png" },			// �N���A�G�t�F�N�g�p
	{ "data\\TEXTURE\\MODE\\RESULT\\over.png" },				// �I�[�o�[
	{ "data\\TEXTURE\\MODE\\RESULT\\over_bg.png" },				// �I�[�o�[�G�t�F�N�g�p
	{ "data\\TEXTURE\\MODE\\RESULT\\rank_num.png" },			// �����N
};

//=============================================================================
// �R���X�g���N�^									(public)	*** CTexture ***
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// �f�X�g���N�^									(public)	*** CTexture ***
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// �S�I������									(public)	*** CTexture ***
//=============================================================================
void CTexture::ReleaseAll(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// �e�N�X�`�����J�E���g
		if (m_pTexture[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[nCount]->Release();		// ���
			m_pTexture[nCount] = NULL;			// NULL��
		}
	}

	m_nMaxTexture = 0;		// �e�N�X�`����
}

//=============================================================================
// �e�N�X�`���擾����								(public)	*** CTexture ***
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int nNum)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	if (nNum < TYPE_MAX)
	{// �e�N�X�`���̍ő吔�ȉ���������
		if (0 < m_nMaxTexture)
		{// �O�ȏゾ������
			for (int nCount = 0; nCount < m_nMaxTexture; nCount++)
			{// �e�N�X�`�����J�E���g
				if (m_pTexType[nCount] == nNum)
				{// �ԍ�����v
					return m_pTexture[nCount];	// �l��Ԃ�
				}
			}

			// ��v����ԍ����Ȃ�
			D3DXCreateTextureFromFile(pDevice, m_TexName[nNum], &m_pTexture[m_nMaxTexture]);	// �e�N�X�`���ǂݍ���
			m_pTexType[m_nMaxTexture] = nNum;	// �ԍ��ݒ�

			m_nMaxTexture++;	// �e�N�X�`�����J�E���g

			return m_pTexture[m_nMaxTexture - 1];	// �l��Ԃ�
		}
		else
		{// �O�ȉ�
			D3DXCreateTextureFromFile(pDevice, m_TexName[nNum], &m_pTexture[m_nMaxTexture]);	// �e�N�X�`���ǂݍ���
			m_pTexType[m_nMaxTexture] = nNum;		// �ԍ��ݒ�

			m_nMaxTexture++;	// �e�N�X�`�����J�E���g
		}

		return m_pTexture[0];	// �l��Ԃ�
	}

	return NULL;	// �l��Ԃ�
}