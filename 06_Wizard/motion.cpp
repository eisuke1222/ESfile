//=============================================================================
//
// ���[�V�������� [motion.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "motion.h"			// ���[�V����
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^										(public)	*** CMotion ***
//=============================================================================
CMotion::CMotion()
{
	// �����l�ݒ�
	m_nCntFrame			= 0;				// �t���[���J�E���^
	m_nEditBFrame		= -1;				// �u�����h�t���[���J�E���^
	m_bEditBFrame		= false;			// �u�����h�t���[���ύX
	m_bSetrot			= false;			// �����w��

	m_bMotionStop		= false;			// ���[�V������~
	m_bMotionKeyStop	= false;			// �L�[��~
	m_bMotionChange		= true;				// �؂�ւ�
	m_nMotionNextNum	= 0;				// ���[�V�����ԍ�
	m_nMotionNum		= 0;				// �����[�V�����ԍ�
	m_nMotionKeyNextNum = 0;				// �L�[
	m_nMotionKeyNum		= 0;				// ���L�[
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMotion ***
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// ����������										(public)	*** CMotion ***
//=============================================================================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMotion ***
//=============================================================================
void CMotion::Uninit(void)
{

}

//=============================================================================
// �X�V����											(public)	*** CMotion ***
//=============================================================================
void CMotion::Update(int nModelMax, MOTION_INFO *MotionInfo, CModel *pModel)
{
	if (m_nMotionNum != m_nMotionNextNum || m_bSetrot == true)
	{
		m_nCntFrame		= 0;					// �t���[��������
		m_bMotionChange = true;					// ���[�V�����ύX����
		m_nMotionNum	= m_nMotionNextNum;		// ���[�V�����ԍ��ύX
		m_nMotionKeyNum = m_nMotionKeyNextNum;	// ���[�V�����L�[�ԍ��ύX
		m_bMotionStop	= false;				// ���[�V������~����
		//m_bMotionKeyStop = false;

		if (m_bEditBFrame == false)
		{// �u�����h�t���[���ύX���Ă��Ȃ�
			m_nEditBFrame = -1;		// �u�����h�t���[��������
		}

		if (m_bSetrot == true)
		{// �p�x�ύX
			for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
			{// ���f�����J�E���g 
				pModel[0].Setpos(pModel[0].GetInitpos() + MotionInfo[m_nMotionNum].aKeyInfo[(m_nMotionKeyNum) % MotionInfo[m_nMotionNum].nKeyMax].aKey[0].pos);
				pModel[nCntModel].Setrot(MotionInfo[m_nMotionNum].aKeyInfo[(m_nMotionKeyNum) % MotionInfo[m_nMotionNum].nKeyMax].aKey[nCntModel].rot);
			}
			m_nCntFrame = MotionInfo[m_nMotionNum].aKeyInfo[m_nMotionKeyNum].nFrame;	// �t���[���ύX

			m_bSetrot = false;		// �p�x�ύX����
		}
	}

	if (m_bMotionStop == false)
	{// ���[�V������~���Ă��Ȃ�
		D3DXVECTOR3 posDest, posAdd;	// �ʒu�v�Z�p
		D3DXVECTOR3 rotDest, rotAdd;	// ��]�v�Z�p
		float		fDiff, fRatio;		// �v�Z���ʔ��f�p

		if (m_bMotionChange == true)
		{// ���[�V�����ύX����
			if (m_bEditBFrame == false)
			{// �u�����h�t���[���ύX���Ă��Ȃ�
				fRatio = (MotionInfo[m_nMotionNum].nBrendFrame - m_nCntFrame) * 1.0f;
			}
			else
			{// �u�����h�t���[���ύX����
				fRatio = (m_nEditBFrame - m_nCntFrame) * 1.0f;
			}
		}
		else
		{// ���[�V�����ύX���Ă��Ȃ�
			fRatio = (MotionInfo[m_nMotionNum].aKeyInfo[m_nMotionKeyNum].nFrame - m_nCntFrame) * 1.0f;
		}

		/*CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fDiff);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", Dest.y);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fRatio);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", pModel[0].Getpos().y);*/

		for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
		{// ���f�����J�E���g
			if (m_bMotionChange == true)
			{// ���[�V�����ύX����
				// �ʒu�ύX
				posDest = MotionInfo[m_nMotionNum].aKeyInfo[0].aKey[nCntModel].pos;
				// �����ύX
				rotDest = MotionInfo[m_nMotionNum].aKeyInfo[0].aKey[nCntModel].rot;
			}
			else
			{// ���[�V�����ύX���Ă��Ȃ�
				// ���L�[�ԍ�
				int nNum = (m_nMotionKeyNum + 1) % MotionInfo[m_nMotionNum].nKeyMax;

				if ((m_nMotionKeyNum + 1) % MotionInfo[m_nMotionNum].nKeyMax == 0)
				{// �L�[���Ō�܂ōs����
					if (MotionInfo[m_nMotionNum].bLoop == 0)
						nNum = m_nMotionKeyNum;		// �L�[����
				}
				// �ʒu�ύX
				posDest = MotionInfo[m_nMotionNum].aKeyInfo[nNum].aKey[nCntModel].pos;
				// �����ύX
				rotDest = MotionInfo[m_nMotionNum].aKeyInfo[nNum].aKey[nCntModel].rot;
			}

			posDest += pModel[nCntModel].GetInitpos();	// �ʒu����

			{
				// �����ݒ�
				fDiff = (posDest.x - pModel[nCntModel].Getpos().x);
				// ���݌����ݒ�
				posAdd.x = fDiff / fRatio;

				// �����ݒ�
				fDiff = (posDest.y - pModel[nCntModel].Getpos().y);
				// ���݌����ݒ�
				posAdd.y = fDiff / fRatio;

				// �����ݒ�
				fDiff = (posDest.z - pModel[nCntModel].Getpos().z);
				// ���݌����ݒ�
				posAdd.z = fDiff / fRatio;

				// �ʒu�ݒ�
				pModel[nCntModel].Setpos(pModel[nCntModel].Getpos() + posAdd);
			}

			{
				// �ڕW��������
				if (rotDest.x > D3DX_PI)	{ rotDest.x -= D3DX_PI * 2.0f; }
				if (rotDest.x < -D3DX_PI)	{ rotDest.x += D3DX_PI * 2.0f; }

				// �����ݒ�
				fDiff = (rotDest.x - pModel[nCntModel].Getrot().x);

				// ������������
				if (fDiff > D3DX_PI)		{ fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI)		{ fDiff += D3DX_PI * 2.0f; }

				// ���݌����ݒ�
				rotAdd.x = fDiff / fRatio;

				// �ڕW��������
				if (rotDest.y > D3DX_PI)	{ rotDest.y -= D3DX_PI * 2.0f; }
				if (rotDest.y < -D3DX_PI)	{ rotDest.y += D3DX_PI * 2.0f; }

				// �����ݒ�
				fDiff = (rotDest.y - pModel[nCntModel].Getrot().y);

				// ������������
				if (fDiff > D3DX_PI)		{ fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI)		{ fDiff += D3DX_PI * 2.0f; }

				// ���݌����ݒ�
				rotAdd.y = fDiff / fRatio;

				// �ڕW��������
				if (rotDest.z > D3DX_PI)	{ rotDest.z -= D3DX_PI * 2.0f; }
				if (rotDest.z < -D3DX_PI)	{ rotDest.z += D3DX_PI * 2.0f; }

				// �����ݒ�
				fDiff = (rotDest.z - pModel[nCntModel].Getrot().z);

				// ������������
				if (fDiff > D3DX_PI)		{ fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI)		{ fDiff += D3DX_PI * 2.0f; }

				// ���݌����ݒ�
				rotAdd.z = fDiff / fRatio;
			}

			// ��]�ݒ�
			pModel[nCntModel].Setrot(pModel[nCntModel].Getrot() + rotAdd);
		}

		m_nCntFrame++;		// �t���[���J�E���g

		if (m_bMotionChange == true)
		{// ���[�V�����ύX����
			if (m_bEditBFrame == false)
			{// �u�����h�t���[���ύX���Ă��Ȃ�
				if (m_nCntFrame % MotionInfo[m_nMotionNum].nBrendFrame == 0)
				{// �t���[�����Ō�܂ōs����
					m_nCntFrame		= 0;		// �t���[��������	
					m_bMotionChange = false;	// ���[�V�����ύX����
				}
			}
			else
			{// �u�����h�t���[���ύX����
				if (m_nCntFrame % m_nEditBFrame == 0)
				{// �t���[�����Ō�܂ōs����
					m_nCntFrame		= 0;		// �t���[��
					m_bMotionChange = false;	// ���[�V�����ύX����
					m_nEditBFrame	= -1;		// �u�����h�t���[��������
					m_bEditBFrame	= false;	// �u�����h�t���[���ύX����
				}
			}
		}
		else
		{// ���[�V�����ύX���Ă��Ȃ�
			if (m_nCntFrame % MotionInfo[m_nMotionNum].aKeyInfo[m_nMotionKeyNum].nFrame == 0)
			{// �t���[�����Ō�܂ōs����
				if (m_nMotionKeyNum + 1 == MotionInfo[m_nMotionNum].nKeyMax)
				{// �L�[���Ō�܂ōs����
					if (m_bMotionKeyStop == false)
					{// �L�[��~���Ă��Ȃ�
						if (MotionInfo[m_nMotionNum].bLoop == 0)
						{// ���[�v���Ȃ�
							SetMotionNum(0);	// ���[�V�����ύX
						}

						m_nCntFrame		= 0;	// �t���[��������		
						m_nMotionKeyNum = 0;	// �L�[�ԍ�������
					}
					else
					{// �L�[��~���Ă���
						m_bMotionStop	= true;	// ���[�V������~
					}
				}
				else
				{// �L�[�Đ��r��
					m_nCntFrame		= 0;	// �t���[��������
					m_nMotionKeyNum = (m_nMotionKeyNum + 1) % MotionInfo[m_nMotionNum].nKeyMax;		// �L�[��i�߂�
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CMotion ***
//=============================================================================
void CMotion::Draw(void)
{

}