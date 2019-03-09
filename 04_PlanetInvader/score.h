//=============================================================================
//
// �X�R�A���� [score.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCORE_MAX_DIGIT		(6)			// �X�R�A����
#define SCORE_SIZE_X		(10)		// �傫���iX)
#define SCORE_SIZE_Y		(10)		// �傫���iY)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber2D;		// ����2D

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScore : public CScene
{// �w�i�i�e�FCScene�j
public:
	CScore();									// �R���X�g���N�^
	~CScore();									// �f�X�g���N�^

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nScoreNum);		// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static void AddScore(int nScoreNum, int nScore) { m_nScore[nScoreNum] += nScore; }			// �X�R�A���Z
	static int GetScore(int nScoreNum) { return m_nScore[nScoreNum]; }							// �X�R�A�擾

private:
	int m_nScoreNum;							// �X�R�A�ԍ�

	CNumber2D *m_apNumber2D[SCORE_MAX_DIGIT];				// ����2D
	static int m_nScore[MAX_PLAYER];						// �X�R�A

protected:

};

#endif