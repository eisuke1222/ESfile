//=============================================================================
//
// �G(���l)���� [enemy.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��
#include "motion.h"		// ���[�V����

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;			// ���f��
class CMotion;			// ���[�V����

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CScene
{// �G�i�e�FCScene�j
public:
	typedef struct
	{// ���f�����
		char cFileName[256];		// �t�@�C����
		int nIndex;					// ID�ԍ�
		D3DXVECTOR3 pos;			// �ʒu
		D3DXVECTOR3 rot;			// �p�x
	}MODEL_INFO;

	CEnemy();										// �R���X�g���N�^
	~CEnemy();										// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// ����

	HRESULT Init(void);											// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

	// �ʒu�i�ݒ�A�擾�j
	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 Getpos(void) { return m_pos; }

	// �G�̓����蔻��
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);

	// �t�@�C���ǂݍ���
	static char *ReadFile(FILE *pFile, char *pDest)
	{
		fgets(pDest, 256, pFile);

		if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
		{
			ReadFile(pFile, pDest);
		}
		else
		{
			do
			{
				if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
				{
					ReadFile(pFile, pDest);
				}
				else if (memcmp(pDest, " ", strlen(" ")) == 0)
				{
					wsprintf(&pDest[0], &pDest[strlen(" ")]);
				}
				else if (memcmp(pDest, "\t", strlen("\t")) == 0)
				{
					wsprintf(&pDest[0], &pDest[strlen("\t")]);
				}
				else if (memcmp(pDest, "�@", strlen("�@")) == 0)
				{
					wsprintf(&pDest[0], &pDest[strlen("�@")]);
				}
			} while (
				memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0 ||
				memcmp(pDest, " ", strlen(" ")) == 0 || memcmp(pDest, "\t", strlen("\t")) == 0 ||
				memcmp(pDest, "�@", strlen("�@")) == 0);
		}

		return pDest;
	}
	static void PopString(char *pStrCur, char *pStr)
	{
		do
		{
			if (memcmp(pStrCur, "=", strlen("=")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen("=")]);
			}
			else if (memcmp(pStrCur, " ", strlen(" ")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen(" ")]);
			}
			else if (memcmp(pStrCur, "\t", strlen("\t")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen("\t")]);
			}
			else if (memcmp(pStrCur, "�@", strlen("�@")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen("�@")]);
			}
		} while (
			memcmp(pStrCur, "=", strlen("=")) == 0 ||
			memcmp(pStrCur, " ", strlen(" ")) == 0 || memcmp(pStrCur, "\t", strlen("\t")) == 0 ||
			memcmp(pStrCur, "�@", strlen("�@")) == 0);

		int nCntEnd = 0;
		strcpy(pStr, pStrCur);

		do
		{
			if (memcmp(pStr, "#", strlen("#")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("#")]);
			}
			else if (memcmp(pStr, "\n", strlen("\n")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("\n")]);
			}
			else if (memcmp(pStr, " ", strlen(" ")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen(" ")]);
			}
			else if (memcmp(pStr, "\t", strlen("\t")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("\t")]);
			}
			else if (memcmp(pStr, "�@", strlen("�@")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("�@")]);
			}
			else
			{
				wsprintf(&pStr[0], &pStr[1]);
			}

			nCntEnd++;
		} while (
			memcmp(pStr, "#", strlen("#")) != 0 && memcmp(pStr, "\n", strlen("\n")) != 0 &&
			memcmp(pStr, " ", strlen(" ")) != 0 && memcmp(pStr, "\t", strlen("\t")) != 0 &&
			memcmp(pStr, "�@", strlen("�@")) != 0);

		strcpy(pStr, pStrCur);
		wsprintf(&pStrCur[0], &pStrCur[nCntEnd]);
		pStr[nCntEnd] = '\0';
	}

private:
	static int					m_MaxModel;			// ���f����
	static int					m_MaxAnim;			// �A�j���[�V������

	static LPDIRECT3DTEXTURE9	**m_pTexture;		// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh;			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_nNumMat;			// �}�e���A�����̐�

	static MODEL_INFO			*m_ModelInfo;
	static CMotion::ANIM_INFO	*m_AnimInfo;

	D3DXMATRIX					m_mtxWorld;			// ���[���h�}�g���b�N�X
	CModel						*m_pModel;			// ���f��
	CMotion						*m_pMotion;			// ���[�V����

	bool						m_bStart;			// �X�^�[�g
	int							m_nCntStart;

	D3DXVECTOR3					m_vtxMax;			// �傫���i�ő�j
	D3DXVECTOR3					m_vtxMin;			// �傫���i�ŏ��j
	D3DXVECTOR3					m_pos;				// �ʒu����
	D3DXVECTOR3					m_posOld;			// �ʒu�O��
	D3DXVECTOR3					m_rot;				// ����
	D3DXVECTOR3					m_move;				// �ړ���
	float						m_frotDest;			// �ڕW����
	bool						m_bJump;			// �W�����v
	bool						m_bSlip;			// ����
	float						m_fSliprot;			// ����p�x
	float						m_fDownSpeed;		// �������x
	int							m_nWallJump;		// �ǃL�b�N�J�E���g
	bool						m_bWallJump;		// �ǃL�b�N���Ă��邩
	float						m_bWallJumpmove;	// �ǃL�b�N�ړ���

	bool						m_bmoveUp;			// �A�C�e���ŃX�s�[�h���オ���Ă����邩�ǂ���
	int							m_nMoveCounter;		// �X�s�[�hUP���鎞�Ԃ̃J�E���^�[
	FLOAT						m_fScaleX = 1.0f, m_fScaleY = 1.0f, m_fScaleZ = 1.0f;   //�g��k���p�ϐ�

	float						m_shadowHeight;		// �e����

protected:

};

#endif