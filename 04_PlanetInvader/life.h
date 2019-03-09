//=============================================================================
//
// �̗́i�c�@�j���� [life.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIFE			(3)			// �c�@��
#define LIFE_SIZE_X			(25)		// �傫���iX)
#define LIFE_SIZE_Y			(25)		// �傫���iY)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;		// �V�[��2D

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLife : public CScene
{// �̗́i�e�FCScene�j
public:
	CLife();									// �R���X�g���N�^
	~CLife();									// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLifeNum);		// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	void AddLife(void);							// �̗͉��Z
	void DeleteLife(void);						// �̗͔j��

	static void SetAdd(int nPlayerNum, bool bAdd) { m_bAddLife[nPlayerNum] = bAdd; }				// ���Z
	static void SetRespawn(int nPlayerNum, bool bRespawn) { m_bRespawn[nPlayerNum] = bRespawn; }	// ���X�|�[��

	static void SetLiveState(int nPlayerNum) { if (m_nLifeAll[nPlayerNum] == 0) m_bLiveState[nPlayerNum] = false; else m_bLiveState[nPlayerNum] = true; }	// �����l��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`�����ւ̃|�C���^
	CScene2D *m_apScene2D[MAX_LIFE];			// �V�[��2D

	int m_nLifeNum;								// �̗͔ԍ�

	static bool m_bAddLife[MAX_PLAYER];			// ���Z
	static bool m_bRespawn[MAX_PLAYER];			// ���X�|�[���\��
	static int m_nLifeAll[MAX_PLAYER];			// �̗͂̍ő�l

	static bool m_bLiveState[MAX_PLAYER];		// �����l��

protected:

};

#endif