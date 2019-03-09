//=============================================================================
//
// �Q�[������ [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MAP			(128)		// �}�b�v�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �V�[��2D
class CScene3D;			// �V�[��2D
class CPause;			// �|�[�Y
class CRanking;			// �����L���O

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame
{// �Q�[��
public:
	typedef enum
	{// �Q�[��
		GAME_NONE = 0,		// �������Ă��Ȃ�
		GAME_PLAY,			// �v���C
		GAME_CLEAR,			// �N���A
		GAME_OVER,			// �I�[�o�[
		GAME_MAX
	}GAME;

	CGame();						// �R���X�g���N�^
	~CGame();						// �f�X�g���N�^

	static HRESULT Load(void);		// ���[�h
	static void Unload(void);		// �A�����[�h

	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static void SetPause(bool bPause);					// �|�[�Y�ݒ�
	static bool GetPause(void) { return m_bPause; }		// �|�[�Y�擾

	static void SetGame(GAME game) { m_game = game; m_nCntGame = 0; }	// �Q�[���ݒ�
	static GAME GetGame(void) { return m_game; }						// �Q�[���擾

private:
	typedef struct
	{// �}�b�v
		int nNumModel;		// ���f���ԍ�
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
	}MAP;

	typedef struct
	{// �}�b�v
		int nNumEnemy;		// ���f���ԍ�
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 move;	// ����
		int nShotTiming;	// ���˃^�C�~���O
	}ENEMY;

	static LPDIRECT3DTEXTURE9		m_pTexture;						// �e�N�X�`�����ւ̃|�C���^
	static CScene2D					*m_pScene2D[MAX_PLAYER];		// �V�[��2D

	static CRanking					*m_pRanking;					// �����L���O

	static CPause *m_pPause;		// �|�[�Y
	static bool m_bPause;			// �|�[�Y���Ă��邩

	static GAME m_game;				// �Q�[��
	static int m_nCntGame;			// �Q�[���J�E���^

	static LPDIRECT3DTEXTURE9		m_pENDTexture[2];					// �e�N�X�`�����ւ̃|�C���^
	static CScene3D *m_pScene3D;

	void SetMap(void);				// �}�b�v�ݒ�

	static MAP m_mapState[MAX_MAP];	// �}�b�v���
	static int m_mapMax;			// �}�b�v�ő吔
	static ENEMY m_enemyState[MAX_MAP];	// �G���
	static int m_enemyMax;			// �G�ő吔

	int m_mapNum;					// �}�b�v�ԍ�
	int m_enemyNum;					// �}�b�v�ԍ�
	int m_mapfield;					// �t�B�[���h�ԍ�


protected:

};

#endif