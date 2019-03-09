//=============================================================================
//
// �J�������� [camera.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene2d.h"	// �V�[��2D
#include "scene3d.h"	// �V�[��3D
#include "enemy.h"		// �G

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCamera
{// �J����
public:
	CCamera();							// �R���X�g���N�^
	virtual ~CCamera();					// �f�X�g���N�^

	virtual HRESULT Init(void);			// ����������
	virtual void Uninit(void);			// �I������
	virtual void Update(void) = 0;		// �X�V����
	virtual void SetCamera(void);		// �`�揈��

	void SetInitPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR)			// �����ݒ�i���_�A�����_�j
		{ m_posVInit = posV; m_posRInit = posR; }
	D3DXVECTOR3 GetInitPos(int nNum)							// �����擾�i���_�A�����_�j
		{ if(nNum == 0) return m_posVInit; else return m_posRInit; }

	void SetPosDest(D3DXVECTOR3 posVDest, D3DXVECTOR3 posRDest)	// �ڕW�ݒ�i���_�A�����_�j
		{ m_posVDest = posVDest; m_posRDest = posRDest;}
	D3DXVECTOR3 GetPosDest(int nNum)							// �ڕW�擾�i���_�A�����_�j
		{ if (nNum == 0) return m_posVDest; else return m_posRDest; }	

										// �ݒ�i���_�A�����_�j
	void SetPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, int nNum = 0)
	{
		m_posV		= posV;								// ���_
		m_posVDest	= posV;								// �ڕW���_
		m_posR		= posR;								// �����_
		m_posRDest	= posR;								// �ڕW���_
		m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �p�x
		if (nNum == 0)
		{
			m_rot.x = D3DX_PI - atan2f(posR.z - posV.z, posR.y - posV.y);		// �p�x
			m_rot.y = D3DX_PI - atan2f(posR.x - posV.x, posR.z - posV.z);		// �p�x
		}
		// �p�x����
		if (D3DX_PI < m_rot.y)
			m_rot.y -= (D3DX_PI * 2);
		if (m_rot.y < -D3DX_PI)
			m_rot.y += (D3DX_PI * 2);

		m_fDistance = sqrtf(powf(posR.x - posV.x, 2.0f) + powf(posR.z - posV.z, 2.0f));		// ����
		m_fDistanceInit = m_fDistance;
	}
	void SetPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot)
	{
		m_posV		= posV;								// ���_
		m_posVDest	= posV;								// �ڕW���_
		m_posR		= posR;								// �����_
		m_posRDest	= posR;								// �ڕW���_
		m_rot		= rot;								// �p�x

		// �p�x����
		if (D3DX_PI < m_rot.y)
			m_rot.y -= (D3DX_PI * 2);
		if (m_rot.y < -D3DX_PI)
			m_rot.y += (D3DX_PI * 2);

		m_fDistance = sqrtf(powf(posR.x - posV.x, 2.0f) + powf(posR.z - posV.z, 2.0f));		// ����
		m_fDistanceInit = m_fDistance;
	}
	void SetPos(D3DXVECTOR3 posR)
	{
		m_posR = posR;									// �����_
		m_posRDest = posR;								// �ڕW���_
		
		// ���_�v�Z�p
		D3DXMATRIX mat;									// ��]
		D3DXVECTOR3	v(0, 0, m_fDistance);				// �����_�Ƃ̋���
		D3DXVECTOR3	vout;								// �v�Z����

		// ���_�v�Z
		D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);		// ��]
		D3DXVec3TransformCoord(&vout, &v, &mat);								// �����ƍ��킹��
		m_posVDest = vout + m_posRDest;					// �����_����̈ʒu

		m_posV = m_posVDest;							// ���_
	}
	D3DXVECTOR3 Getpos(int nNum)		// �擾�i���_�A�����_�j
		{ if(nNum == 0) return m_posV; else return m_posR; }		

	void Setrot(D3DXVECTOR3 rot)		// �p�x�i�ݒ�j
		{ m_rot = rot;	}
	D3DXVECTOR3 Getrot(void)			// �p�x�i�擾�j
		{ return m_rot;	}

	void SetDistance(int nNum, float fDistance)			// �����ݒ�i���݁A�����j
		{ if (nNum == 0) m_fDistance = fDistance; else m_fDistanceInit = fDistance; }
	float GetDistance(int nNum)							// �����擾�i���݁A�����j
		{ if (nNum == 0) return m_fDistance; else return m_fDistanceInit; }

	D3DXMATRIX GetmtxPro(void)			// �v���W�F�N�V�����}�g���b�N�X�擾
		{ return m_mtxProjection; }
	D3DXMATRIX GetmtxView(void)			// �r���[�}�g���b�N�X�擾
		{ return m_mtxView; }

										// �r���[�|�[�g�ݒ�
	void SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height);

private:

protected:
	void Debug(void);					// �f�o�b�O����

	D3DXVECTOR3		m_posVInit;			// �������_
	D3DXVECTOR3		m_posV;				// ���_
	D3DXVECTOR3		m_posVDest;			// �ڕW���_
	D3DXVECTOR3		m_posRInit;			// ���������_
	D3DXVECTOR3		m_posR;				// �����_
	D3DXVECTOR3		m_posRDest;			// �ڕW�����_
	D3DXVECTOR3		m_vecU;				// ������x�N�g��
	D3DXVECTOR3		m_rot;				// �p�x
	float			m_fDistance;		// ����
	float			m_fDistanceInit;	// ��������

	D3DXMATRIX		m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX		m_mtxView;			// �r���[�}�g���b�N�X
	D3DVIEWPORT9	m_viewport;			// �r���[�|�[�g

};

class CCameraMain : public CCamera
{// ���C���J�����i�e�FCCamera�j
public:
	typedef enum
	{// ���b�N�I���}�[�J�[
		ROCKON_MARKER_FRAME = 0,	// �t���[��
		ROCKON_MARKER_PLAY,			// ���b�N�I����
		ROCKON_MARKER_MAX			// �ő吔
	}ROCKON_MARKER;

	CCameraMain();						// �R���X�g���N�^
	~CCameraMain();						// �f�X�g���N�^

	HRESULT Init(void);					// ����������
	void Uninit(void);					// �I������
	void Update(void);					// �X�V����
	void SetCamera(void);				// �`�揈��

	void SetPickUp(bool bPickUp)		// �s�b�N�A�b�v����
		{ m_bPickUp = bPickUp; }
	bool GetPickUp(void)				// �s�b�N�A�b�v�擾
		{ return m_bPickUp; }

	void SetGameEnd(bool bGameEnd)		// �Q�[���G���h����
		{ m_bGameEnd = bGameEnd; }
	bool GetGameEnd(void)				// �Q�[���G���h�擾
		{ return m_bGameEnd; }

	void SetMove(bool bMove)			// �ړ�����
		{ m_bMove = bMove; }
	bool GetMove(void)					// �ړ��擾
		{ return m_bMove; }

	void SetMoveCollision(bool bMoveCollision)	// �ړ�����
		{ m_bMoveCollision = bMoveCollision; }
	bool GetMoveCollision(void)					// �ړ��擾
		{ return m_bMoveCollision; }

	void SetLockOn(bool bLockOn)		// ���b�N�I���ݒ�
		{ m_bLockOn = bLockOn; }	
	bool GetLockOn(void)				// ���b�N�I�����Ă��邩�擾
		{ return m_bLockOn; }
	float GetLockOnRot(void)			// ���b�N�I���p�x�擾
		{ return m_fLockOn; }
	CEnemy *GetLockOnEnemy(void)		// ���b�N�I�����Ă�G�擾
		{ return m_pLockOn; }

private:
	void PickUp(void);					// �s�b�N�A�b�v
	void GameEnd(void);					// �Q�[���G���h
	void Following(void);				// �Ǐ]����
	void Move(void);					// �ړ�����
	void LockOn(void);					// ���b�N�I������
	void ColisionChack(void);			// ����`�F�b�N����

	bool m_bPickUp;						// �s�b�N�A�b�v
	int m_nCntPickUp;					// �s�b�N�A�b�v�J�E���g
	bool m_bGameEnd;					// �Q�[���G���h
	int m_nCntGameEnd;					// �Q�[���G���h�J�E���g
	bool m_bMove;						// �ړ��ł��邩
	bool m_bMoveCollision;				// �ړ��R���W����
	CEnemy *m_pLockOn;					// ���b�N�I���G
	bool m_bLockOn;						// ���b�N�I��
	float m_fLockOn;					// ���b�N�I���p�x

	CEnemy *m_pLockOnL;					// ���b�N�I���GL
	CEnemy *m_pLockOnR;					// ���b�N�I���GR

	CScene3D *m_pLockOnMarker[ROCKON_MARKER_MAX];	// ���b�N�I���}�[�J�[

protected:

};

class CCameraMap : public CCamera
{// �~�j�}�b�v�i�e�FCCamera�j
public:
	CCameraMap();						// �R���X�g���N�^
	~CCameraMap();						// �f�X�g���N�^

	HRESULT Init(void);					// ����������
	void Uninit(void);					// �I������
	void Update(void);					// �X�V����
	void SetCamera(void);				// �`�揈��

private:

protected:

};

#endif