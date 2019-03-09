//=============================================================================
//
// ���[�V�������� [motion.h]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "motion.h"			// ���[�V����
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[

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
// �R���X�g���N�^								(public)	*** CMotion ***
//=============================================================================
CMotion::CMotion()
{
	// �����l�ݒ�
	m_nCntFrame = 0;							// �t���[���J�E���^
	m_nEditBFrame = -1;							// �u�����h�t���[���ҏW�p
	m_bEditBFrame = false;						// �u�����h�t���[���ҏW
	m_bSetrot = false;							// �p�x�ҏW

	m_bAnimStop = false;						// ���[�V������~
	m_bAnimChange = true;						// ���[�V�����ύX
	m_nAnimNextNum = 0;							// �����[�V�����ԍ�
	m_nAnimNum = 0;								// ���[�V�����ԍ�
	m_nAnimKeyNextNum = 0;						// ���L�[�ԍ�
	m_nAnimKeyNum = 0;							// �L�[�ԍ�
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CMotion ***
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// ����������									(public)	*** CMotion ***
//=============================================================================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������										(public)	*** CMotion ***
//=============================================================================
void CMotion::Uninit(void)
{

}

//=============================================================================
// �X�V����										(public)	*** CMotion ***
//=============================================================================
void CMotion::Update(int nModelMax, ANIM_INFO *AnimInfo, CModel *pModel, D3DXVECTOR3 posParent)
{
	if (m_nAnimNum != m_nAnimNextNum || m_bSetrot == true)
	{// ���[�V�����ҏW
		m_nCntFrame = 0;					// �t���[���J�E���g
		m_bAnimChange = true;				// ���[�V�����ύX
		m_nAnimNum = m_nAnimNextNum;		// ���[�V�����ύX
		m_nAnimKeyNum = m_nAnimKeyNextNum;	// �L�[�ύX

		if (m_bEditBFrame == false)
		{// �u�����h�t���[���ҏW���Ă��Ȃ�
			m_nEditBFrame = -1;
		}

		if (m_bSetrot == true)
		{// �p�x�ύX
			// �ʒu
			pModel[0].Setpos(posParent + AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum) % AnimInfo[m_nAnimNum].nKeyMax].aKey[0].pos);

			for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
			{// �J�E���g
				// �p�x
				pModel[nCntModel].Setrot(AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum) % AnimInfo[m_nAnimNum].nKeyMax].aKey[nCntModel].rot);
			}
			// �t���[��
			m_nCntFrame = AnimInfo[m_nAnimNum].aKeyInfo[m_nAnimKeyNum].nFrame;

			// �p�x
			m_bSetrot = false;
		}
	}

	if (m_bAnimStop == false)
	{// ���[�V��������
		D3DXVECTOR3 Dest, Add;	// �ڕW�A���Z
		float fDiff, frac;		// �v�Z�p

		if (m_bAnimChange == true)
		{// ���[�V�����ύX
			if (m_bEditBFrame == false)
			{// �u�����h�t���[���ύX���Ă��Ȃ�
				frac = (AnimInfo[m_nAnimNum].nBrendFrame - m_nCntFrame) * 1.0f;
			}
			else
			{// �u�����h�t���[���ύX����
				frac = (m_nEditBFrame - m_nCntFrame) * 1.0f;
			}
		}
		else
		{// ���̑�
			frac = (AnimInfo[m_nAnimNum].aKeyInfo[m_nAnimKeyNum].nFrame - m_nCntFrame) * 1.0f;
		}

		if (m_bAnimChange == true)
		{// ���[�V�����ύX
			Dest = posParent + AnimInfo[m_nAnimNum].aKeyInfo[0].aKey[0].pos;
		}
		else
		{// ���̑�
			Dest = posParent + AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum + 1) % AnimInfo[m_nAnimNum].nKeyMax].aKey[0].pos;
		}

		// �����ݒ�
		fDiff = (Dest.x - pModel[0].Getpos().x);
		// ���݌����ݒ�
		Add.x = fDiff / frac;

		// �����ݒ�
		fDiff = (Dest.y - pModel[0].Getpos().y);
		// ���݌����ݒ�
		Add.y = fDiff / frac;

		// �����ݒ�
		fDiff = (Dest.z - pModel[0].Getpos().z);
		// ���݌����ݒ�
		Add.z = fDiff / frac;

		pModel[0].Setpos(pModel[0].Getpos() + Add);

		/*CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", fDiff);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", Dest.y);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", frac);
		pFont->SetMessage(CFont::SETTYPE_ADD, "%.2f\n", pModel[0].Getpos().y);*/

		for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
		{// �J���f�����E���g
			if (m_bAnimChange == true)
			{// ���[�V�����ύX
				// �����ύX
				Dest = AnimInfo[m_nAnimNum].aKeyInfo[0].aKey[nCntModel].rot;
			}
			else
			{// ���̑�
				// �����ύX
				Dest = AnimInfo[m_nAnimNum].aKeyInfo[(m_nAnimKeyNum + 1) % AnimInfo[m_nAnimNum].nKeyMax].aKey[nCntModel].rot;
			}

			{
				// �ڕW��������
				if (Dest.x > D3DX_PI) { Dest.x -= D3DX_PI * 2.0f; }
				if (Dest.x < -D3DX_PI) { Dest.x += D3DX_PI * 2.0f; }

				// �����ݒ�
				fDiff = (Dest.x - pModel[nCntModel].Getrot().x);

				// ������������
				if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

				// ���݌����ݒ�
				Add.x = fDiff / frac;

				// �ڕW��������
				if (Dest.y > D3DX_PI) { Dest.y -= D3DX_PI * 2.0f; }
				if (Dest.y < -D3DX_PI) { Dest.y += D3DX_PI * 2.0f; }

				// �����ݒ�
				fDiff = (Dest.y - pModel[nCntModel].Getrot().y);

				// ������������
				if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

				// ���݌����ݒ�
				Add.y = fDiff / frac;

				// �ڕW��������
				if (Dest.z > D3DX_PI) { Dest.z -= D3DX_PI * 2.0f; }
				if (Dest.z < -D3DX_PI) { Dest.z += D3DX_PI * 2.0f; }

				// �����ݒ�
				fDiff = (Dest.z - pModel[nCntModel].Getrot().z);

				// ������������
				if (fDiff > D3DX_PI) { fDiff -= D3DX_PI * 2.0f; }
				if (fDiff < -D3DX_PI) { fDiff += D3DX_PI * 2.0f; }

				// ���݌����ݒ�
				Add.z = fDiff / frac;
			}

			// �p�x�ύX
			pModel[nCntModel].Setrot(pModel[nCntModel].Getrot() + Add);
		}

		m_nCntFrame++;		// �t���[���J�E���g

		if (m_bAnimChange == true)
		{// ���[�V�����ύX
			if (m_bEditBFrame == false)
			{// �u�����h�t���[���ύX���Ă��Ȃ�
				if (m_nCntFrame % AnimInfo[m_nAnimNum].nBrendFrame == 0)
				{// ���t���[���ɂȂ�����
					m_nCntFrame = 0;			// �t���[��������
					m_bAnimChange = false;		// ���[�V�����ύX�I��
				}
			}
			else
			{// �u�����h�t���[���ύX
				if (m_nCntFrame % m_nEditBFrame == 0)
				{// ���t���[���ɂȂ�����
					m_nCntFrame = 0;			// �t���[��������
					m_bAnimChange = false;		// ���[�V�����ύX
					m_nEditBFrame = -1;			// �u�����h�t���[���ύX
					m_bEditBFrame = false;
				}
			}
		}
		else
		{// ���[�V����������
			if (m_nCntFrame % AnimInfo[m_nAnimNum].aKeyInfo[m_nAnimKeyNum].nFrame == 0)
			{// ���t���[���ɂȂ�����
				if (m_nAnimKeyNum + 1 == AnimInfo[m_nAnimNum].nKeyMax)
				{// �L�[���ŏI�L�[�ɂȂ�����
					if (AnimInfo[m_nAnimNum].bLoop == 0)
					{// ���[�v���Ȃ�
						m_nAnimNextNum = 0;	// �O�ԃ��[�V������
					}

					m_nAnimKeyNum = 0;		// �L�[�ԍ�������
					m_nCntFrame = 0;		// �t���[��������
				}
				else
				{// ���̑�
					m_nAnimKeyNum = (m_nAnimKeyNum + 1) % AnimInfo[m_nAnimNum].nKeyMax;		// �L�[�ύX
					m_nCntFrame = 0;		// �t���[��������
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��										(public)	*** CMotion ***
//=============================================================================
void CMotion::Draw(void)
{

}