//=============================================================================
//
// �I�u�W�F�N�g���� [object3d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

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
class CObject3D : public CModel3D
{// �I�u�W�F�N�g�i�e�FCModel3D�j
public:
	typedef enum
	{// ���f���^�C�v
		OBJECTTYPE_O_G = 0,		// �C�����E
		OBJECTTYPE_BILL_1,		// �r��1
		OBJECTTYPE_BILL_2,		// �r��2
		OBJECTTYPE_BILL_3,		// �r��3
		OBJECTTYPE_LIFE,		// 
		OBJECTTYPE_MAX
	}OBJECTTYPE;

	typedef enum
	{
		BREAKTYPE_NORMAL = 0,
		BREAKTYPE_FALL,
		BREAKTYPE_MAX
	}BREAKTYPE;

	CObject3D();									// �R���X�g���N�^
	~CObject3D();									// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType);					// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	OBJECTTYPE GetobjectType(void) { return m_objectType; }			// �I�u�W�F�N�g�^�C�v�擾

	bool HitDamage(int nDamage, BREAKTYPE breakType, float breakrot, int nShotIdx)
	{// �q�b�g�_���[�W
		if (m_bDelete != true)
		{// �j�����ĂȂ�������
			m_nLife -= nDamage;						// �̗͂����炷
			m_breakType = breakType;				// �j��^�C�v
			m_breakrot = breakrot;					// �j�����
			m_breakrotDiff = D3DX_PI * 0.8f;		// �j������c��
			m_nShotIdx = nShotIdx;					// ����ID

			if (m_nLife < 1) m_bDelete = true;		// �j���J�n
		}

		return m_bDelete;
	}

	bool GetDelete(void) { return m_bDelete; }		// �j��ς�

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[OBJECTTYPE_MAX];		// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[OBJECTTYPE_MAX];			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat[OBJECTTYPE_MAX];			// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat[OBJECTTYPE_MAX];			// �}�e���A�����̐�

	OBJECTTYPE	m_objectType;						// ���f���^�C�v
	BREAKTYPE	m_breakType;						// �j��^�C�v
	bool		m_bDelete;							// �j��
	int			m_nLife;							// �̗�
	int			m_nCntTimer;						// ���ԃJ�E���^

	float		m_breakrot;							// �j�����
	float		m_breakrotDiff;						// �j������c��
	int			m_nCntBreak;						// �j��J�E���^

	int			m_nShotIdx;							// ����ID

protected:

};

#endif