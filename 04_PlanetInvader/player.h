//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "model3d.h"	// ���f��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CModel3D
{// �v���C���[�i�e�FCModel3D�j
public:
	CPlayer();									// �R���X�g���N�^
	~CPlayer();									// �f�X�g���N�^

	typedef enum
	{// ���
		PLAYERSTATE_NONE = 0,		// �������Ă��Ȃ�
		PLAYERSTATE_NORMAL,			// �ʏ�
		PLAYERSTATE_DAMAGE,			// �_���[�W
		PLAYERSTATE_RESPAWN,		// ����
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum);					// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	bool HitDamage(int nDamage)
	{// �q�b�g�_���[�W
		if (m_state == PLAYERSTATE_NORMAL)
		{// �ʏ���
			m_nLife -= nDamage;					// �̗͂����炷
			m_state = PLAYERSTATE_DAMAGE;		// �_���[�W��Ԃ�
		}
		if (m_nLife < 1) return true;	// �l��Ԃ�
		return false;		// �l��Ԃ�
	}

	int GetPlayerNum(void) { return m_nPlayerNum; }			// �v���C���[�ԍ��擾

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;	// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh;		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;		// �}�e���A�����̐�

	int				m_nPlayerNum;				// �v���C���[�ԍ�
	bool			m_bmove;					// �ړ���
	D3DXVECTOR3		m_pos;						// �ʒu
	D3DXVECTOR3		m_move;						// �ړ���
	int				m_nCntTimer;				// ���ԃJ�E���^
	float			m_frotDest;					// ��������
	int				m_nCntstate;				// ��ԃJ�E���g
	PLAYERSTATE		m_state;					// ���
	int				m_nLife;					// �̗�

	int				m_nCntShot;					// ���˃J�E���^

protected:

};

#endif