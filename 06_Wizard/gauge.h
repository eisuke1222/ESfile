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
#include "scene2d.h"	// �V�[��2D
#include "scene3d.h"	// �V�[��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGauge : public CScene
{// �Q�[�W�i�e�FCScene�j
public:
	typedef enum
	{// ���
		TYPE_2D = 0,	// 2D
		TYPE_3D,		// 3D
		TYPE_MAX		// �ő吔
	}TYPE;

	typedef enum
	{// �Q�[�W
		GAUGE_BG = 0,	// �w�i
		GAUGE_BAR,		// �o�[
		GAUGE_MAX		// �ő吔
	}GAUGE;

	CGauge();												// �R���X�g���N�^
	virtual ~CGauge();										// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

															// �N���G�C�g
	static CGauge *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);	

	virtual HRESULT Init(void) = 0;							// ����������
	virtual void Uninit(void);								// �I������
	virtual void Update(void) = 0;							// �X�V����
	virtual void Draw(void) = 0;							// �`�揈��

															// �o�[�ݒ�
	virtual void SetBar(D3DXVECTOR3 pos, int nNow, int nMax) = 0;	

private:

protected:
	D3DXVECTOR3 m_pos;										// �ʒu
	D3DXVECTOR3 m_size;										// �傫��
	D3DXCOLOR	m_col;										// �F

};

class CGauge2D : public CGauge
{// �Q�[�W2D�i�e�FCGauge�j
public:
	CGauge2D();												// �R���X�g���N�^
	~CGauge2D();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	void SetBar(D3DXVECTOR3 pos, int nNow, int nMax);		// �o�[�ݒ�

private:
	CScene2D	*m_pGause2D[TYPE_MAX];						// �Q�[�W2D

protected:

};

class CGauge3D : public CGauge
{// �Q�[�W3D�i�e�FCGauge�j
public:
	CGauge3D();												// �R���X�g���N�^
	~CGauge3D();											// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	void SetBar(D3DXVECTOR3 pos, int nNow, int nMax);		// �o�[�ݒ�

private:
	CScene3D	*m_pGause3D[TYPE_MAX];						// �Q�[�W3D

protected:

};

#endif