//=============================================================================
//
// �Q�[�W���� [gauge.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAUGE_MAX_TEXTURE	(2)			// �e�N�X�`����
#define GAUGE_SIZE_X		(80)		// �傫���iX)
#define GAUGE_SIZE_Y		(20)		// �傫���iY)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �V�[��2D

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGauge : public CScene
{// �Q�[�W�i�e�FCScene�j
public:
	CGauge();									// �R���X�g���N�^
	~CGauge();									// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CGauge *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nGaugeNum);		// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static void SetReset(int nPlayerNum, bool bReset) { m_bResetGauge[nPlayerNum] = bReset; }

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;		// �e�N�X�`�����ւ̃|�C���^
	CScene2D *m_apScene2D[GAUGE_MAX_TEXTURE];	// �V�[��2D

	int m_nGaugeNum;							// �Q�[�W�ԍ�

	static bool m_bResetGauge[MAX_PLAYER];					// ���Z�b�g
	D3DXVECTOR3 m_pos[MAX_PLAYER][GAUGE_MAX_TEXTURE];		// �ʒu
	D3DXVECTOR3 m_size[MAX_PLAYER][GAUGE_MAX_TEXTURE];		// �傫��

protected:

};

#endif