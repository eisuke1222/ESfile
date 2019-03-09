//=============================================================================
//
// �^�C�g������ [title.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CWindow;			// �E�B���h�E
class CPlayer;			// �v���C���[
class CScene3D;
class CScene2D;
class CSceneMesh;
class CUi;
class CRanking;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle
{// �^�C�g��
public:
	typedef enum
	{// �I��
		SELECT_GAME = 0,	// �Q�[��
		SELECT_TUTORIAL,	// �`���[�g���A��
		SELECT_EDIT,		// �ҏW
		SELECT_TITLE,		// �^�C�g��
		SELECT_MAX			// �ő吔
	}SELECT;

	const static int m_MAX_WALL = 4;

	CTitle();								// �R���X�g���N�^
	~CTitle();								// �f�X�g���N�^

	static HRESULT Load(void);				// ���[�h
	static void Unload(void);				// �A�����[�h

	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	CPlayer *GetPlayer(void)				// �v���C���[�擾
		{ return m_pPlayer; }

	bool GetTitle(void)						// �^�C�g����
		{ return m_bTitle; }

	int GetSelectNum(void)					// �I��ԍ��擾
		{ return m_nSelectNum; }

	bool GetTutorial(void)					// �`���[�g���A����
		{ return m_bTutorial; }
	bool GetSelectSet(void)					// �I��ݒ肵�Ă��邩
		{ return m_bSelectSet; }

	CUi *GetUi(void)				// Ui�擾
		{ return m_pUi_Game; }

private:
	CPlayer			*m_pPlayer;				// �v���C���[

	bool			m_bTitle;				// �^�C�g����

	int				m_nSelectNum;			// �I��ԍ�
	int				m_nCntTimer;			// ���ԃJ�E���^

	bool			m_bTutorial;			// �`���[�g���A����
	bool			m_bSelectSet;			// �I��ݒ肵�Ă��邩

	CWindow			*m_pWindow;				// �E�B���h�E
	CScene3D		*m_pIcon;				// �A�C�R��
	CSceneMesh		*m_pReturnCircle;		// �߂�T�[�N��
	CSceneMesh		*m_pWall[m_MAX_WALL];	// ��

	CUi				*m_pUi_Game;			// UI

	bool			m_bRanking;				// 
	CRanking		*m_pRanking;
	CScene2D		*m_pRankingBG;

protected:

};

#endif