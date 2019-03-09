//=============================================================================
//
// �e�L�X�g���� [text.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TEXT_H_
#define _TEXT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "model.h"		// ���f��
#include "motion.h"		// ���[�V����

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CText
{// �e�L�X�g
public:
	typedef enum
	{// �^�C�v
		TYPE_MODEL = 0,				// ���f��
		TYPE_MOTION,				// ���[�V����
		TYPE_MESHSKIN,				// �X�L��
		TYPE_MESHFIELD,				// �t�B�[���h
		TYPE_MAX					// �ő吔
	}TYPE;

	typedef struct
	{// �e�L�X�g
		int		nIdx;				// �e�L�X�g�ԍ�
		TYPE	type;				// �^�C�v
		int		nTypeNum;			// �^�C�v�ԍ�
		char	pFileName[256];		// �t�@�C����
		int		nMax;				// �ő�l
		bool	bLoad;				// ���[�h������
	}TEXT;

	typedef struct
	{// ���f��
		char				pFileName[256];		// �t�@�C����
		LPDIRECT3DTEXTURE9 *pTexture;			// �e�N�X�`�����ւ̃|�C���^
		LPD3DXMESH			pMesh;				// ���b�V�����ւ̃|�C���^
		LPD3DXBUFFER		pBuffMat;			// �}�e���A�����ւ̃|�C���^
		DWORD				nNumMat;			// �}�e���A�����̐�
		bool				bLoad;				// ���[�h������
	}MODEL_STORAGE;

	CText();														// �R���X�g���N�^
	~CText();														// �f�X�g���N�^

	static HRESULT Load(void);										// ���[�h
	static void Unload(void);										// �A�����[�h

	static char *ReadFile(FILE *pFile, char *pDest);				// �t�@�C���ǂݍ���
	static void PopString(char *pStrCur, char *pStr);				// �ǂݍ��ݒ���

	static CModel::MODEL_INFO	*GetModel(char *cName, int *nMax);	// ���f���擾
	static CMotion::MOTION_INFO	*GetMotion(char *cName);			// ���[�V�����擾

																	// ���f���f�[�^�擾
	static void GetXModelData(char *cName, LPDIRECT3DTEXTURE9 **pTexture, LPD3DXMESH *pMesh, LPD3DXBUFFER *pBuffMat, DWORD *nNumMat);
	static void ReleaseXModelData(void);							// ���f���f�[�^�J��

private:
	static void LoadModelText(char *cName, int *nMax, int nNum);	// ���f�����[�h
	static void LoadMotionText(char *cName, int *nMax, int nNum);	// ���[�V�������[�h

	static int GetMaxModel(void);									// ���f�����擾
	static void LoadXModelData(char *cName, int nNum);				// ���f���f�[�^���[�h

	static int					m_nMaxText;							// �e�L�X�g��
	static int					m_nMaxType[TYPE_MAX];				// �^�C�v�ʃe�L�X�g��

	static TEXT					*m_Text;							// �e�L�X�g
	static CModel::MODEL_INFO	**m_pModelInfo;						// ���f��
	static CMotion::MOTION_INFO	**m_pMotionInfo;					// ���[�V����

	static int					m_nMaxModel;						// ���f����
	static MODEL_STORAGE		*m_pModelStorage;					// ���f���ۊ�

protected:

};

#endif