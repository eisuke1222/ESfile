//=============================================================================
//
// ���e�i�c���j���� [bomb.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BOMB			(5)			// �c��
#define BOMB_SIZE_X			(25)		// �傫���iX)
#define BOMB_SIZE_Y			(25)		// �傫���iY)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBomb : public CScene
{// ���e�i�e�FCScene�j
public:
	CBomb();									// �R���X�g���N�^
	~CBomb();									// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CBomb *Create(int nMaxBomb, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nBombNum);		// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	void AddBomb(void);							// ���e���Z
	void ResetBomb(void);						// �{�����Z�b�g
	void DeleteBomb(void);						// ���e�j��

	static bool SetShot(int nPlayerNum) { return m_bShotBomb[nPlayerNum]; }							// ����
	static void SetAdd(int nPlayerNum, bool bAdd) { m_bAddBomb[nPlayerNum] = bAdd; }				// ���Z
	static void SetReset(int nPlayerNum, bool bReset) { m_bResetBomb[nPlayerNum] = bReset; }		// ���Z�b�g
	static void SetDelete(int nPlayerNum, bool bDelete) { m_bDeleteBomb[nPlayerNum] = bDelete; }	// �j��

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;	// �e�N�X�`�����ւ̃|�C���^
	CScene2D *m_apScene2D[MAX_BOMB];		// �V�[��2D
	
	int	m_nBombNum;								// ���e�ԍ�

	static bool m_bShotBomb[MAX_PLAYER];		// ���e����
	static bool m_bAddBomb[MAX_PLAYER];			// ���e���Z
	static bool m_bResetBomb[MAX_PLAYER];		// ���e���Z�b�g
	static bool m_bDeleteBomb[MAX_PLAYER];		// ���e�j��
	static int m_nMaxBomb[MAX_PLAYER];			// ���e�ő吔
	static int m_nBombAll[MAX_PLAYER];			// ���e���ݐ�

protected:

};

#endif