//=============================================================================
//
// ���[�V�������� [motion.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
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
class CMotion
{// ���[�V����
public:
	typedef struct
	{// �L�[	
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
	}KEY;

	typedef struct
	{// �L�[���
		int nFrame;				// �t���[��
		KEY *aKey;				// �L�[
	}KEY_INFO;

	typedef struct
	{// ���[�V�������
		int bLoop;				// ���[�v
		int nKeyMax;			// �L�[�ő吔
		int nBrendFrame;		// �u�����h�t���[��
		KEY_INFO *aKeyInfo;		// �L�[���
	}MOTION_INFO;

	CMotion();							// �R���X�g���N�^
	~CMotion();							// �f�X�g���N�^

	HRESULT Init(void);					// ����������
	void Uninit(void);					// �I������
										// �X�V����
	void Update(int nModelMax, MOTION_INFO *MotionInfo, CModel *pModel);	
	void Draw(void);					// �`�揈��

	void SetMotionNum(int nNum) 							// ���[�V�����ݒ�
		{ if (m_nMotionNextNum != nNum) { m_nMotionNextNum = nNum; m_nMotionKeyNextNum = 0; m_nEditBFrame = -1; m_bEditBFrame = false; } }
	void SetMotionNum(int nNum, int nKeyNum) 				// ���[�V�����ݒ�i�X�^�[�g�L�[�ݒ�L�[�j
		{ if (m_nMotionNextNum != nNum) { m_nMotionNextNum = nNum; m_nMotionKeyNextNum = nKeyNum; m_nEditBFrame = -1; m_bEditBFrame = false; } }
	void SetMotionNum(int nNum, int nKeyNum, int nBFrame)	// ���[�V�����ݒ�i�X�^�[�g�L�[�ݒ�L�[�A�u�����h�t���[���ꎞ�ύX�j
		{ if (m_nMotionNextNum != nNum) { m_nMotionNextNum = nNum; m_nMotionKeyNextNum = nKeyNum; m_nEditBFrame = nBFrame; m_bEditBFrame = true; } }
	void SetMotionNumrot(int nNum, int nKeyNum) 			// ���[�V���������ݒ�i�L�[�̊p�x����j
		{ m_nMotionNextNum = nNum; m_nMotionKeyNextNum = nKeyNum; m_bSetrot = true; }

	int GetMotionNum(void)				// ���[�V�����擾
		{ return m_nMotionNum; }	
	int GetMotionKeyNum(void) 			// �L�[�擾
		{ return m_nMotionKeyNum; }
	int GetCntFrame(void) 				// �t���[���擾
		{ return m_nCntFrame; }
	void SetStop(bool bStop) 			// ���[�V������~
		{ m_bMotionStop = bStop; }
	void SetKeyStop(bool bKeyStop) 		// ���[�V�����L�[��~
		{ m_bMotionKeyStop = bKeyStop; }

private:
	int		m_nCntFrame;				// �t���[���J�E���^
	int		m_nEditBFrame;				// �u�����h�t���[���J�E���^
	bool	m_bEditBFrame;				// �u�����h�t���[���ύX
	bool	m_bSetrot;					// �����w��

	bool	m_bMotionStop;				// ���[�V������~
	bool	m_bMotionKeyStop;			// �L�[��~
	bool	m_bMotionChange;			// �؂�ւ�
	int		m_nMotionNum;				// ���[�V�����ԍ�
	int		m_nMotionNextNum;			// �����[�V�����ԍ�
	int		m_nMotionKeyNextNum;		// �L�[
	int		m_nMotionKeyNum;			// ���L�[

protected:

};

#endif