//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_MAX_NUM		(5)			// �����L���O��
#define RANKING_MAX_DIGIT	(6)			// �����L���O����
#define RANKING_SIZE_X		(10)		// �傫���iX)
#define RANKING_SIZE_Y		(10)		// �傫���iY)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber3D;		// ����3D
class CScene3D;			// �V�[��3D

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRanking : public CScene
{// �����L���O�i�e�FCScene�j
public:
	CRanking();									// �R���X�g���N�^
	~CRanking();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h

	static void SaveRanking(void);				// �����L���O�Z�[�u
	static void LoadRanking(void);				// �����L���O���[�h

	static CRanking *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval, int nNum);		// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`�����ւ̃|�C���^

	static CNumber3D *m_apNumber3D[RANKING_MAX_NUM][RANKING_MAX_DIGIT];		// ����3D
	static CScene3D *m_apScene3D[RANKING_MAX_NUM];							// �V�[��3D
	static int m_nRanking[RANKING_MAX_NUM];		// �����L���O

	D3DXVECTOR3 m_pos[RANKING_MAX_NUM];			// �ʒu
	float m_fScene3DDiff;						// ���ʂƃX�R�A�̍�

	static int m_RankInNum;

protected:

};

#endif