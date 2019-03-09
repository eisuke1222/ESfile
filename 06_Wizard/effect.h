//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene2d.h"	// �V�[��2D
#include "scene3d.h"	// �V�[��3D
#include "scenemesh.h"	// �V�[�����b�V��
#include "model.h"		// ���f��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffect : public CScene
{// �G�t�F�N�g�i�e�FCScene�j
public:
	typedef enum
	{// ���
		TYPE_2D = 0,	// 2D
		TYPE_3D,		// 3D
		TYPE_C00,		// �J�X�^��00
		TYPE_C01,		// �J�X�^��01
		TYPE_C02,		// �J�X�^��02
		TYPE_C03,		// �J�X�^��03
		TYPE_C04,		// �J�X�^��04
		TYPE_FIRE,		// ��
		TYPE_CREATE,	// �N���G�C�g
		TYPE_MAX		// �ő吔
	}TYPE;

	typedef enum
	{// ���l
		SHOT_NONE = 0,	// �ݒ肵�Ȃ�
		SHOT_PLAYER,	// �v���C���[
		SHOT_ENEMY,		// �G
		SHOT_MAX		// �ő吔
	}SHOT;

	CEffect();												// �R���X�g���N�^
	virtual ~CEffect();										// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

															// �N���G�C�g
	static CEffect *Create(TYPE type, SHOT shot, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col);

	virtual HRESULT Init(void) = 0;							// ����������
	virtual void Uninit(void);								// �I������
	virtual void Update(void) = 0;							// �X�V����
	virtual void Draw(void) = 0;							// �`�揈��

private:
	virtual void ColisionChack(void) = 0;					// ����`�F�b�N����

protected:
	SHOT		m_nShot;									// �������l
	D3DXVECTOR3 m_pos;										// �ʒu
	D3DXVECTOR3 m_rot;										// ��]
	D3DXVECTOR3 m_move;										// �ړ���
	D3DXVECTOR3 m_size;										// �傫��
	D3DXCOLOR	m_col;										// �F

};

class CEffect2D : public CEffect
{// �G�t�F�N�g2D�i�e�FCEffect�j
public:
	CEffect2D();											// �R���X�g���N�^
	~CEffect2D();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	void ColisionChack(void);								// ����`�F�b�N����

	CScene2D	*m_pEffect2D;								// �G�t�F�N�g2D

protected:

};

class CEffect3D : public CEffect
{// �G�t�F�N�g3D�i�e�FCEffect�j
public:
	CEffect3D();											// �R���X�g���N�^
	~CEffect3D();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	void ColisionChack(void);								// ����`�F�b�N����

	CScene3D	*m_pEffect3D;								// �G�t�F�N�g3D

	int			m_nCntTimer;								// ���ԃJ�E���g
	int         m_nPownTime;								// ��������

protected:

};

class CEffect_C00 : public CEffect
{// �G�t�F�N�g_�J�X�^��00�i�e�FCEffect�j
public:
	CEffect_C00();											// �R���X�g���N�^
	~CEffect_C00();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	void SetDeleteTime(int nTime)							// �j�����Ԑݒ�
		{ m_DeleteTime = nTime; }

	void SetSizeUp(bool bSizeUp)							// �傫������
		{ m_bSizeUp = bSizeUp; }

private:
	void ColisionChack(void);								// ����`�F�b�N����

	CScene3D	*m_pEffect3D;								// �G�t�F�N�g3D

	int			m_DeleteTime;								// �j������

	bool		m_bSizeUp;									// �傫��

protected:

};

class CEffect_C01 : public CEffect
{// �G�t�F�N�g_�J�X�^��01�i�e�FCEffect�j
public:
	typedef enum
	{// ���
		EFFECT_ATK = 0,		// �U��
		EFFECT_H00,			// �G�t�F�N�g�p0
		EFFECT_H01,			// �G�t�F�N�g�p1
		EFFECT_H02,			// �G�t�F�N�g�p2
		EFFECT_MAX			// �ő吔
	}EFFECT;

	CEffect_C01();											// �R���X�g���N�^
	~CEffect_C01();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	void SetExplosion(bool bExplosion)						// �N���ݒ�
		{ m_bExplosion = bExplosion; }

private:
	void ColisionChack(void);								// ����`�F�b�N����

	CScene3D	*m_pEffect3D[EFFECT_MAX];					// �G�t�F�N�g3D
	float		m_frot[EFFECT_MAX];							// �G�t�F�N�g����

	int			m_nCntTimer;								// ���ԃJ�E���g
	int         m_nPownTime;								// ��������

	bool		m_bExplosion;								// �N��

protected:

};

class CEffect_C02 : public CEffect
{// �G�t�F�N�g_�J�X�^��02�i�e�FCEffect�j
public:
	CEffect_C02();											// �R���X�g���N�^
	~CEffect_C02();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	void ColisionChack(void);								// ����`�F�b�N����

	CSceneMesh	*m_pSceneMesh;								// �V�[�����b�V��

protected:

};

class CEffect_C03 : public CEffect
{// �G�t�F�N�g_�J�X�^��03�i�e�FCEffect�j
public:
	CEffect_C03();											// �R���X�g���N�^
	~CEffect_C03();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	void ColisionChack(void);								// ����`�F�b�N����

	CSceneMesh	*m_pSceneMesh;								// �V�[�����b�V��

	int			m_nCntTimer;								// ���ԃJ�E���g

protected:

};

class CEffect_C04 : public CEffect
{// �G�t�F�N�g_�J�X�^��04�i�e�FCEffect�j
public:
	CEffect_C04();											// �R���X�g���N�^
	~CEffect_C04();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	void ColisionChack(void);								// ����`�F�b�N����

	static int						m_nMaxModel;			// ���f����

	static LPDIRECT3DTEXTURE9		**m_pTexture;			// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH				*m_pMesh;				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER				*m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	static DWORD					*m_nNumMat;				// �}�e���A�����̐�

	static CModel::MODEL_INFO		*m_ModelInfo;			// ���f���C���t�H

	CModel							*m_pModel;				// ���f��

	int								m_nCntTimer;			// ���ԃJ�E���g

protected:

};

class CEffect_Fire : public CEffect
{// �G�t�F�N�g_�΁i�e�FCEffect�j
public:
	CEffect_Fire();											// �R���X�g���N�^
	~CEffect_Fire();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	void ColisionChack(void);								// ����`�F�b�N����

	int			m_nCntTimer;								// ���ԃJ�E���g

protected:

};

class CEffect_Create : public CEffect
{// �G�t�F�N�g_�N���G�C�g�i�e�FCEffect�j
public:
	CEffect_Create();										// �R���X�g���N�^
	~CEffect_Create();										// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	void ColisionChack(void);								// ����`�F�b�N����

	int			m_nCntTimer;								// ���ԃJ�E���g

protected:

};

#endif