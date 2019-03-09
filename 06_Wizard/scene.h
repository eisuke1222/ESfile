//=============================================================================
//
// �V�[������ [scene.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene
{// �V�[��
public:
	typedef enum
	{// �v���C�I���e�B
		PRIORITY_BG = 0,		// �w�i
		PRIORITY_1,				// 1
		PRIORITY_2,				// 2
		PRIORITY_3,				// 3
		PRIORITY_4,				// 4
		PRIORITY_5,				// 5
		PRIORITY_UI,			// UI
		PRIORITY_MAP_1,			// �}�b�v1
		PRIORITY_MAP_2,			// �}�b�v2
		PRIORITY_PAUSE,			// �|�[�Y
		PRIORITY_FADE,			// �t�F�[�h
		PRIORITY_MAX			// �ő吔
	}PRIORITY;

	typedef enum
	{// �I�u�W�F�N�g�^�C�v
		OBJTYPE_NONE = 0,		// �������Ă��Ȃ�
		OBJTYPE_MESHFIELD,		// ���b�V���t�B�[���h
		OBJTYPE_MESHWALL,		// ���b�V���E�H�[��
		OBJTYPE_MESHCYLINDER,	// ���b�V���V�����_�[
		OBJTYPE_BG,				// �w�i
		OBJTYPE_ENEMY,			// �G
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_BULLET,			// �e
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_GAUGE,			// �Q�[�W
		OBJTYPE_NUMBER,			// ����
		OBJTYPE_SCORE,			// �X�R�A
		OBJTYPE_RANKING,		// �����L���O
		OBJTYPE_MENU,			// ���j���[
		OBJTYPE_ROCKON,			// ���b�N�I��
		OBJTYPE_MAPICON,		// �}�b�v�A�C�R��
		OBJTYPE_WARP,			// ���[�v
		OBJTYPE_GOAL,			// �S�[��
		OBJTYPE_GRASS,			// ��
		OBJTYPE_GUARD,			// �K�[�h
		OBJTYPE_PAUSE,			// �|�[�Y
		OBJTYPE_PRESS,			// �v���X
		OBJTYPE_MAP,			// �}�b�v
		OBJTYPE_UI,				// UI
		OBJTYPE_WINDOW,			// �E�B���h�E
		OBJTYPE_TITLE,			// �^�C�g��
		OBJTYPE_FADE,			// �t�F�[�h
		OBJTYPE_CAMERA,			// �J����
		OBJTYPE_RESULT,			// ���U���g
		OBJTYPE_MAX				// �ő吔
	}OBJTYPE;

	CScene(int nPriority, OBJTYPE objType);				// �R���X�g���N�^
	virtual ~CScene();									// �f�X�g���N�^

	static void ReleaseMode(void);						// �t�F�[�h�ȊO�j��
	static void ReleaseAll(void);						// �S�j��
	static void UpdateAll(void);						// �S�X�V
	static void DrawAll(void);							// �S�`��
	static void DrawAllMap(void);						// �S�`��}�b�v

	virtual HRESULT Init(void) = 0;						// ����������
	virtual void Uninit(void) = 0;						// �I������
	virtual void Update(void) = 0;						// �X�V����
	virtual void Draw(void) = 0;						// �`�揈��

	static int GetNumAll(void) { return m_nNumAll; }	// �����擾
	static int GetNumPri(int nPriority)					// �v���C�I���e�B�����擾
		{ return m_nNumPri[nPriority]; }

	void SetPriority(int nPriority)						// �v���C�I���e�B�ݒ�
		{ m_nPriority = nPriority; }

	void SetObjType(OBJTYPE objType)					// �I�u�W�F�N�g�^�C�v�ݒ�
		{ m_objType = objType; }		
	OBJTYPE GetObjType(void)							// �I�u�W�F�N�g�^�C�v�擾
		{ return m_objType; }			

	static void SetStartUp(int nStartUp)				// �X�V�ݒ�
		{ m_nStartUp = nStartUp; }		

	static CScene *GetScene(PRIORITY nPriority)			// �V�[���擾
		{ return m_pTop[nPriority]; }
	CScene *GetSceneNext(void) { return m_pNext; }		// ���V�[���擾

	static void SetDrawMode(int nDrawMode)				// �`�惂�[�h�ݒ�
		{ m_nDrawMode = nDrawMode; }

	static void SetUpdateTime(int nTime)				// �X�V�J�E���g�ݒ�
		{ m_nUpdateTime = nTime; }

	static void SetDrawUi(bool bDrawUi)					// Ui�`��ݒ�
		{ m_bDrawUi = bDrawUi; }
	static bool GetDrawUi(void)							// Ui�`��ݒ�
		{ return m_bDrawUi; }

private:
	static bool DrawSelect(int nStartEnd, OBJTYPE objType);	// �`��I��
	void Release(void);									// �J������
	void ConObjtype(OBJTYPE objType, char *cObjType);	// �I�u�W�F�N�g�����ϊ�

	static CScene *m_pTop[PRIORITY_MAX];				// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX];				// ���݂̃I�u�W�F�N�g�ւ̃|�C���^

	CScene *m_pPrev;									// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pNext;									// ���̃I�u�W�F�N�g�ւ̃|�C���^

	int m_nPriority;									// ���
	OBJTYPE m_objType;									// ���
	bool m_bDeath;										// ���S�t���O

	static int m_nNumAll;								// ����
	static int m_nNumPri[PRIORITY_MAX];					// �v���C�I���e�B����

	static int m_nStartUp;								// �X�V�J�n�ԍ�

	static int m_nDrawMode;								// �`�惂�[�h

	static int m_nCntTimer;								// ���ԃJ�E���g
	static int m_nUpdateTime;							// �X�V�J�E���g

	static bool m_bDrawUi;								// Ui�`��

protected:
	void SetDeath(void) { m_bDeath = true; }			// �j������

};

#endif