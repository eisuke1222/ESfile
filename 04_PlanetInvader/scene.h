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
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SCENE	(1500)		// �V�[���̍ő吔

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
		PRIORITY_PAUSE,			// �|�[�Y
		PRIORITY_FADE,			// �t�F�[�h
		PRIORITY_MAX
	}PRIORITY;

	typedef enum
	{// �I�u�W�F�N�g�^�C�v
		OBJTYPE_NONE = 0,		// �������Ă��Ȃ�
		OBJTYPE_MESHFIELD,		// ���b�V���t�B�[���h
		OBJTYPE_MESHWALL,		// ���b�V���E�H�[��
		OBJTYPE_MESHCYLINDER,	// ���b�V���V�����_�[
		OBJTYPE_OBJ3D,			// �I�u�W�F�N�g
		OBJTYPE_EFFECT3D,		// �G�t�F�N�g3D
		OBJTYPE_BG,				// �w�i
		OBJTYPE_ENEMY,			// �G
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_BULLET,			// �e
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_EXPLOSION,		// ����
		OBJTYPE_CLOUD,			// �_
		OBJTYPE_BOMB,			// ���e
		OBJTYPE_STAR,			// �f��
		OBJTYPE_GAUGE,			// �Q�[�W
		OBJTYPE_LIFE,			// �̗�
		OBJTYPE_SCORE,			// �X�R�A
		OBJTYPE_RANKING,		// �����L���O
		OBJTYPE_MENU,			// ���j���[
		OBJTYPE_PLAYERNUM,		// �v���C���[�ԍ�
		OBJTYPE_TITLEPLAYER,	// �^�C�g���v���C���[
		OBJTYPE_PAUSE,			// �|�[�Y
		OBJTYPE_PRESS,			// �v���X
		OBJTYPE_TITLE,			// �^�C�g��
		OBJTYPE_RESULT,			// ���U���g
		OBJTYPE_FADE,			// �t�F�[�h
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority);						// �R���X�g���N�^
	CScene(int nPriority, int nID);				// �R���X�g���N�^
	virtual ~CScene();							// �f�X�g���N�^

	static void ReleaseAll(void);				// �S�j��
	static void UpadteAll(void);				// �S�X�V
	static void DrawAll(void);					// �S�`��

	virtual HRESULT Init(void) = 0;				// ����������
	virtual void Uninit(void) = 0;				// �I������
	virtual void Update(void) = 0;				// �X�V����
	virtual void Draw(void) = 0;				// �`�揈��

	static int GetNumAll(void);					// �����擾

	int GetID(void) { return m_nID; }			// ID�擾

	void SetObjType(OBJTYPE objType)	{ m_objType = objType; }		// �I�u�W�F�N�g�^�C�v�ݒ�
	OBJTYPE GetObjType(void)			{ return m_objType; }			// �I�u�W�F�N�g�^�C�v�擾

	static CScene *GetScene(int nPri, int nIdx)	{ return m_apScene[nPri][nIdx]; }		// �V�[���擾

	static void SetStartUp(int nStartUp) { m_nStartUp = nStartUp; }		// �X�V�ݒ�

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_SCENE];	// �V�[��
	static int m_nNumAll;						// ����

	int m_nPriority;							// ���
	int m_nID;									// ID�ԍ�
	OBJTYPE m_objType;							// ���

	static int m_nStartUp;						// �X�V�J�n�ԍ�

protected:
	void Release(void);							// �J������

};

#endif