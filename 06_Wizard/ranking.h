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
#include "scene2d.h"	// �V�[��2D
#include "scene3d.h"	// �V�[��3D
#include "number.h"		// �ԍ�

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRanking : public CScene
{// �����L���O�i�e�FCScene�j
public:
	const static int m_RANKING_NUM		= 5;
	const static int m_RANKING_DIGIT	= 6;

	typedef enum
	{
		TYPE_2D = 0,
		TYPE_3D,
		TYPE_MAX
	}TYPE;

	CRanking();									// �R���X�g���N�^
	~CRanking();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h

	static void SaveRanking(void);				// �����L���O�Z�[�u
	static void LoadRanking(void);				// �����L���O���[�h

	static CRanking *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval);		// ����

	HRESULT Init(void) = 0;						// ����������
	void Uninit(void);							// �I������
	void Update(void) = 0;						// �X�V����
	void Draw(void) = 0;						// �`�揈��

	static void SetScore(int nScore)			// �X�R�A�ݒ�
		{ m_nScore = nScore; }

private:

protected:
	static int m_nRanking[m_RANKING_NUM];		// �����L���O
	static int m_nScore;						// �X�R�A
	static int m_RankInNum;						// �����N�C���ԍ�

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	D3DXCOLOR	m_col;
	D3DXVECTOR3 m_NextInterval;

};

class CRanking_2D : public CRanking
{// �����L���O2D�i�e�FCRanking�j
public:
	CRanking_2D();								// �R���X�g���N�^
	~CRanking_2D();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	CNumber2D *m_pNumber[m_RANKING_NUM][m_RANKING_DIGIT];	// �ԍ�
	CScene2D *m_pScene2D[m_RANKING_NUM];		// �V�[��2D

	int m_nCntTimer;							// ���ԃJ�E���g

protected:

};

#endif