//=============================================================================
//
// �^�C�g������ [title.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// �^�C�g��
#include "manager.h"	// �}�l�[�W���[
#include "input.h"		// ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel3D;			// ���f��3D
class CScene3D;			// �V�[��3D
class CScene2D;			// �V�[��2D
class CRanking;			// �����L���O
class CPlayer;			// �v���C���[

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle
{// �^�C�g��
public:
	typedef enum
	{// �^�C�g��
		TITLE_NONE = 0,		// �������Ă��Ȃ�
		TITLE_LOGO,			// �^�C�g�����S
		TITLE_MENU,			// ���j���[
		TITLE_CUSTOM,		// �J�X�^��
		TITLE_RANKING,		// �����L���O
		TITLE_MAX
	}TITLE;

	CTitle();						// �R���X�g���N�^
	~CTitle();						// �f�X�g���N�^

	static HRESULT Load(void);		// ���[�h
	static void Unload(void);		// �A�����[�h

	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static void SetTitle(TITLE title);								// �^�C�g���ݒ�
	static TITLE GetTitle(void) { return m_title; }					// �^�C�g���擾

	static bool GetTitlePress(void) { return m_bPress_R; }			// �v���X�擾
	static void SetTitleRDelete(void) { m_pRanking = NULL; }		// �^�C�g���j��

	static void SetPlayGame(void) { if (m_bPlayGame != true) { m_bPlayGame = true; m_nCntTitle = 0; } }			// �v���C�Q�[��
	static bool GetPlayGame(void) { return m_bPlayGame; }			// �v���C�Q�[��

private:
	static LPDIRECT3DTEXTURE9	*m_pModelTexture;			// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh;					// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;					// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;					// �}�e���A�����̐�
	static CModel3D				*m_pModel3D;				// ���f��3D

	static LPDIRECT3DTEXTURE9	m_pPressTexture;			// �e�N�X�`�����ւ̃|�C���^
	static CScene3D				*m_pPress3D;				// �v���X3D

	static LPDIRECT3DTEXTURE9	m_pStarTexture;				// �e�N�X�`�����ւ̃|�C���^
	static CScene3D				*m_pStar3D;					// �X�^�[3D

	static LPDIRECT3DTEXTURE9	m_pSceneTexture;			// �e�N�X�`�����ւ̃|�C���^
	static CScene2D				*m_pScene2D[MAX_PLAYER];		// �V�[��2D

	static CRanking				*m_pRanking;				// �����L���O

	static TITLE				m_title;					// �^�C�g��

	static int					m_nCntTitle;				// �^�C�g���J�E���^
	static bool					m_bTitleSet_T;				// �^�C�g���ݒ�T
	static bool					m_bTitleSet_R;				// �^�C�g���ݒ�R
	static bool					m_bPressSet;				// �v���X�ݒ�
	static bool					m_bPress_R;					// �v���X����

	static CPlayer				*m_pPlayer[MAX_PLAYER];		// �v���C���[
	static bool					m_bPlayGame;				// �v���C�Q�[��

	static bool					m_bPlayerUse[MAX_CONTROLLERS];			// �v���C���[�g�p
	static bool					m_bPlayerPress[MAX_CONTROLLERS];		// �v���C���[�v���X

protected:

};

#endif