//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "effect.h"			// �G�t�F�N�g
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "sound.h"			// �T�E���h
#include "fade.h"			// �t�F�[�h
#include "texture.h"		// �e�N�X�`��
#include "texanim.h"		// �e�N�X�`���A�j���[�V����
#include "text.h"			// �e�L�X�g
#include "map.h"			// �}�b�v
#include "player.h"			// �v���C���[
#include "enemy.h"			// �G

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
// �R���X�g���N�^										(public)	*** CEffect ***
//=============================================================================
CEffect::CEffect() : CScene(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT)
{
	// �����l�ݒ�
	m_nShot = SHOT_NONE;							// �������l
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_size	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CEffect ***
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ���[�h����										(public)	*** CEffect ***
//=============================================================================
HRESULT CEffect::Load(void)
{
	CEffect_C04::Load();

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CEffect ***
//=============================================================================
void CEffect::Unload(void)
{
	CEffect_C04::Unload();
}

//=============================================================================
// �N���G�C�g										(public)	*** CEffect ***
//=============================================================================
CEffect *CEffect::Create(TYPE type, SHOT shot, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CEffect *pEffect = NULL;			// �|�C���^

	if (pEffect == NULL)
	{// NULL�̏ꍇ
		if(type == TYPE_2D)				// �G�t�F�N�g2D
			pEffect = new CEffect2D;		// �������m��
		else if (type == TYPE_3D)		// �G�t�F�N�g3D
			pEffect = new CEffect3D;		// �������m��
		else if (type == TYPE_C00)		// �G�t�F�N�g_�J�X�^��00
			pEffect = new CEffect_C00;		// �������m��
		else if (type == TYPE_C01)		// �G�t�F�N�g_�J�X�^��01
			pEffect = new CEffect_C01;		// �������m��
		else if (type == TYPE_C02)		// �G�t�F�N�g_�J�X�^��02
			pEffect = new CEffect_C02;		// �������m��
		else if (type == TYPE_C03)		// �G�t�F�N�g_�J�X�^��03
			pEffect = new CEffect_C03;		// �������m��
		else if (type == TYPE_C04)		// �G�t�F�N�g_�J�X�^��04
			pEffect = new CEffect_C04;		// �������m��
		else if (type == TYPE_FIRE)		// �G�t�F�N�g_��
			pEffect = new CEffect_Fire;		// �������m��
		else if (type == TYPE_CREATE)	// �G�t�F�N�g_�N���G�C�g
			pEffect = new CEffect_Create;	// �������m��
	}

	if (pEffect != NULL)
	{// NULL�ȊO�̏ꍇ
		pEffect->m_nShot	= shot;		// �������l
		pEffect->m_pos		= pos;		// �ʒu
		pEffect->m_rot		= rot;		// ��]
		pEffect->m_move		= move;		// �ړ���
		pEffect->m_size		= size;		// �傫��
		pEffect->m_col		= col;		// �F
		pEffect->Init();				// ����������
	}

	return pEffect;		// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CEffect ***
//=============================================================================
void CEffect::Uninit(void)
{
	CScene::SetDeath();		// ���S�t���O
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect2D ***
//=============================================================================
CEffect2D::CEffect2D() : CEffect()
{
	// �����l�ݒ�
	m_pEffect2D = NULL;		// �G�t�F�N�g2D
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect2D ***
//=============================================================================
CEffect2D::~CEffect2D()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect2D ***
//=============================================================================
HRESULT CEffect2D::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect2D ***
//=============================================================================
HRESULT CEffect2D::Init(void)
{
	return S_OK;	// �l��Ԃ�
}	

//=============================================================================
// �I������										(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Uninit(void)
{
	CEffect::Uninit();		// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Update(void)
{

}

//=============================================================================
// �`�揈��										(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect2D ***
//=============================================================================
void CEffect2D::ColisionChack(void)
{

}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::CEffect3D() : CEffect()
{
	// �����l�ݒ�
	m_pEffect3D = NULL;		// �G�t�F�N�g3D

	m_nCntTimer = 0;		// ���ԃJ�E���g
	m_nPownTime = 0;		// ��������
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect3D ***
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect3D ***
//=============================================================================
HRESULT CEffect3D::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect3D ***
//=============================================================================
HRESULT CEffect3D::Init(void)
{
	if (m_pEffect3D == NULL)
		m_pEffect3D = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);		// �������m��

	if (m_pEffect3D != NULL)
	{// NULL�ȊO�̏ꍇ
		// �����l�ݒ�
		m_pEffect3D->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		// ����������
		m_pEffect3D->Init();
		// �e�N�X�`�����f
		m_pEffect3D->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
		// �`��ҏW
		m_pEffect3D->DrawEdit(true, false, false, true, true);
	}

	m_nCntTimer = 0;
	m_nPownTime = 3;

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Uninit(void)
{
	if (m_pEffect3D != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pEffect3D->Uninit();	// �I������
		m_pEffect3D = NULL;		// NULL��
	}

	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Update(void)
{
	m_pos = m_pEffect3D->Getpos();	// �ʒu�擾�i�G�t�F�N�g�j
	m_pos += m_move;				// �ړ��ʂ����Z
	m_pEffect3D->Setpos(m_pos);		// �ʒu�ݒ�i�G�t�F�N�g�j

	m_nCntTimer++;

	if (m_nCntTimer % m_nPownTime == 0)
		CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col);

	ColisionChack();	// ����`�F�b�N
}

//=============================================================================
// �`�揈��										(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect3D ***
//=============================================================================
void CEffect3D::ColisionChack(void)
{
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_TITLE)
	{
		if (m_pos.x < 160.0f || 690.0f < m_pos.x ||
			m_pos.z < -160.0f || 160.0f < m_pos.z)
		{// �͈̓`�F�b�N
			Uninit();	// �I������
			return;		// �l��Ԃ�
		}
	}
	else
	{
		if (m_pos.x < -1500.0f || 1500.0f < m_pos.x ||
			m_pos.z < -1500.0f || 1500.0f < m_pos.z)
		{// �͈̓`�F�b�N
			Uninit();	// �I������
			return;		// �l��Ԃ�
		}
	}

	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			// �}�b�v
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{
					CMap_02 *pMap_02 = (CMap_02*)pMap;
					if (pMap_02->CollisionInOut(m_pos) == false)
					{
						Uninit();	// �I������
						return;		// �l��Ԃ�
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;
					if (pMap_03->CollisionInOut(m_pos) == false)
					{
						Uninit();	// �I������
						return;		// �l��Ԃ�
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;
					if (pMap_04->CollisionInOut(m_pos) == false)
					{
						Uninit();	// �I������
						return;		// �l��Ԃ�
					}
				}
			}

			if (m_nShot == SHOT_PLAYER)
			{
				// �G
				{
					CScene *pScene = NULL;		// �V�[��
					CScene *pSceneNext = NULL;		// ���V�[��

					pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

					while (pScene != NULL)
					{// NULL�ȊO�̏ꍇ
						pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
							CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
							if (pEnemy->Collision(&m_pos, NULL, NULL, m_pEffect3D->Getsize(), m_pEffect3D->Getsize()))
							{// �Փ˂���
								pEnemy->SetLockOn(true);
								if (pEnemy->HitDamage(-6) == true)
								{// �_���[�W����						
									if (pEnemy->GetHP() == 0)
									{// HP���O�ɂȂ���
										pEnemy->Uninit();		// �I������
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(2);
											}
										}
									}
								}

								Uninit();	// �I������
								return;		// �l��Ԃ�
							}
						}
						pScene = pSceneNext;	// ���V�[����
					}
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					if (pPlayer->Collision(m_pos) == true)
					{
						pPlayer->AddHP(-10);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect_C00 ***
//=============================================================================
CEffect_C00::CEffect_C00() : CEffect()
{
	// �����l�ݒ�
	m_pEffect3D = NULL;		// �G�t�F�N�g3D

	m_DeleteTime = 0;

	m_bSizeUp = false;		// �傫��
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect_C00 ***
//=============================================================================
CEffect_C00::~CEffect_C00()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect_C00 ***
//=============================================================================
HRESULT CEffect_C00::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect_C00 ***
//=============================================================================
HRESULT CEffect_C00::Init(void)
{
	if (m_pEffect3D == NULL)
		m_pEffect3D = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);		// �������m��

	if (m_pEffect3D != NULL)
	{// NULL�ȊO�̏ꍇ
		// �����l�ݒ�
		if (m_bSizeUp == false)
			m_pEffect3D->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		else
			m_pEffect3D->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
		// ����������
		m_pEffect3D->Init();
		// �e�N�X�`�����f
		m_pEffect3D->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
		// �`��ҏW
		m_pEffect3D->DrawEdit(true, false, false, true, true);
	}

	m_DeleteTime = 30;

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Uninit(void)
{
	if (m_pEffect3D != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pEffect3D->Uninit();	// �I������
		m_pEffect3D = NULL;		// NULL��
	}

	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Update(void)
{
	if (m_pEffect3D != NULL)
	{// NULL�ȊO�̏ꍇ
		D3DXVECTOR3 pos = m_pEffect3D->Getpos();
		pos += m_move;

		D3DXVECTOR3 size = m_pEffect3D->Getsize();
		if (m_bSizeUp == false)
		{
			size.x -= (float)(m_size.x / (float)m_DeleteTime);
			size.y -= (float)(m_size.y / (float)m_DeleteTime);
		}
		else
		{
			size.x += (float)(m_size.x / (float)m_DeleteTime);
			size.y += (float)(m_size.y / (float)m_DeleteTime);
		}

		D3DXCOLOR col = m_pEffect3D->Getcol();
		col.a -= (float)(m_col.a / (float)m_DeleteTime);

		if (col.a < 0.0f)
		{
			Uninit();
			return;
		}
		else
		{
			m_pEffect3D->Setpos(pos);
			m_pEffect3D->Setsize(size);
			m_pEffect3D->Setcol(col);
		}
	}
}

//=============================================================================
// �`�揈��										(public)	*** CEffect_C00 ***
//=============================================================================
void CEffect_C00::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C00::ColisionChack(void)
{

}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect_C01 ***
//=============================================================================
CEffect_C01::CEffect_C01() : CEffect()
{
	// �����l�ݒ�
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		m_pEffect3D[nCount] = NULL;		// �G�t�F�N�g3D
		m_frot[nCount] = 0.0f;
	}

	m_nCntTimer = 0;		// ���ԃJ�E���g
	m_nPownTime = 0;		// ��������

	m_bExplosion = false;	// �N��
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect_C01 ***
//=============================================================================
CEffect_C01::~CEffect_C01()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect_C01 ***
//=============================================================================
HRESULT CEffect_C01::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect_C01 ***
//=============================================================================
HRESULT CEffect_C01::Init(void)
{
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		m_pEffect3D[nCount] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);		// �������m��

		if (m_pEffect3D[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			// �����l�ݒ�
			if (nCount == 0)
			{
				m_pEffect3D[nCount]->SetInitAll(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			}
			else
			{
				D3DXVECTOR3 pos = m_pos;
				float fAngle = ((D3DX_PI * 2.0f) / (EFFECT_MAX - 1)) * (nCount - 1);
				pos = pos + D3DXVECTOR3(sinf(fAngle), cosf(fAngle), cosf(fAngle)) * 8.0f;
				m_frot[nCount] = fAngle;

				m_pEffect3D[nCount]->SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size * 0.5f, m_col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			}
			// ����������
			m_pEffect3D[nCount]->Init();
			// �e�N�X�`�����f
			m_pEffect3D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
			// �`��ҏW
			m_pEffect3D[nCount]->DrawEdit(true, false, false, true, true);
		}
	}

	m_nCntTimer = 0;
	m_nPownTime = 3;

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Uninit(void)
{
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		if (m_pEffect3D[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pEffect3D[nCount]->Uninit();	// �I������
			m_pEffect3D[nCount] = NULL;		// NULL��
		}
	}

	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Update(void)
{
	m_nCntTimer++;

	for (int nCount = 0; nCount < EFFECT_MAX; nCount++)
	{
		if (nCount == 0)
		{
			m_pos = m_pEffect3D[nCount]->Getpos();	// �ʒu�擾�i�G�t�F�N�g�j
			m_pos += m_move;						// �ړ��ʂ����Z
			m_pEffect3D[nCount]->Setpos(m_pos);		// �ʒu�ݒ�i�G�t�F�N�g�j

			if (m_nCntTimer % m_nPownTime == 0)
				CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col);
		}
		else
		{
			m_frot[nCount] += 0.1f;

			D3DXVECTOR3 posNext = m_pos + m_move;
			float fAngle = atan2f(posNext.x - m_pos.x, posNext.z - m_pos.z);

			D3DXVECTOR3 pos = m_pos;
			if ((-(D3DX_PI * 0.5f) < fAngle && fAngle < 0.0f) || (D3DX_PI * 0.5f) < fAngle)
			{
				pos = pos + D3DXVECTOR3(sinf(m_frot[nCount]), cosf(m_frot[nCount]), sinf(m_frot[nCount])) * 8.0f;
			}
			else
			{
				pos = pos + D3DXVECTOR3(-sinf(m_frot[nCount]), cosf(m_frot[nCount]), sinf(m_frot[nCount])) * 8.0f;
			}
			pos += m_move;							// �ړ��ʂ����Z
			m_pEffect3D[nCount]->Setpos(pos);		// �ʒu�ݒ�i�G�t�F�N�g�j

			if (m_nCntTimer % m_nPownTime == 0)
				CEffect::Create(CEffect::TYPE_C00, m_nShot, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size * 0.5f, m_col);
		}
	
		if (m_pos.x < -1500.0f || 1500.0f < m_pos.x ||
			m_pos.z < -1500.0f || 1500.0f < m_pos.z)
		{// �͈̓`�F�b�N
			Uninit();	// �I������
			return;		// �l��Ԃ�
		}
	}

	ColisionChack();	// ����`�F�b�N			
}

//=============================================================================
// �`�揈��										(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect_C01 ***
//=============================================================================
void CEffect_C01::ColisionChack(void)
{
	CSound			*pSound = CManager::GetSound();				// �T�E���h�擾
	CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_TITLE)
	{
		if (m_pos.x < 160.0f || 690.0f < m_pos.x ||
			m_pos.z < -160.0f || 160.0f < m_pos.z)
		{// �͈̓`�F�b�N
			Uninit();	// �I������
			return;		// �l��Ԃ�
		}
	}
	else
	{
		if (m_pos.x < -1500.0f || 1500.0f < m_pos.x ||
			m_pos.z < -1500.0f || 1500.0f < m_pos.z)
		{// �͈̓`�F�b�N
			Uninit();	// �I������
			return;		// �l��Ԃ�
		}
	}

	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			// �}�b�v
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{
					CMap_02 *pMap_02 = (CMap_02*)pMap;
					if (pMap_02->CollisionInOut(m_pos) == false)
					{
						Uninit();	// �I������
						return;		// �l��Ԃ�
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;
					if (pMap_03->CollisionInOut(m_pos) == false)
					{
						Uninit();	// �I������
						return;		// �l��Ԃ�
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;
					if (pMap_04->CollisionInOut(m_pos) == false)
					{
						Uninit();	// �I������
						return;		// �l��Ԃ�
					}
				}
			}
		}
	}
	CScene *pScene = NULL;		// �V�[��
	CScene *pSceneNext = NULL;		// ���V�[��

	float fHeight = 0.0f;

	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g

				if (pMesh->ColInMesh(m_pos) == true)
				{// �͈̓`�F�b�N
					fHeight = pMesh->GetHeight(m_pos);		// �����擾

					if (m_pos.y < fHeight)
					{
						CEffect::Create(CEffect::TYPE_C02, m_nShot, D3DXVECTOR3(m_pos.x, fHeight, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_RANGE000);

						Uninit();	// �I������
						return;		// �l��Ԃ�
					}
					break;
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				// �G
				{
					pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

					while (pScene != NULL)
					{// NULL�ȊO�̏ꍇ
						pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
							CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
							if (pEnemy->Collision(&m_pos, NULL, NULL, m_pEffect3D[0]->Getsize(), m_pEffect3D[0]->Getsize()))
							{// �Փ˂���
								pEnemy->SetLockOn(true);
								if (pEnemy->HitDamage(-15) == true)
								{// �_���[�W����
									if (pEnemy->GetHP() == 0)
									{// HP���O�ɂȂ���
										pEnemy->Uninit();		// �I������

										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											pGame->SetSS(true);
										}
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(3);
											}
										}
									}
								}

								if (m_bExplosion == true)
								{
									CEffect::Create(CEffect::TYPE_C02, m_nShot, D3DXVECTOR3(m_pos.x, fHeight, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

									if (pSound != NULL)
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_RANGE000);
								}

								Uninit();	// �I������
								return;		// �l��Ԃ�
							}
						}
						pScene = pSceneNext;	// ���V�[����
					}
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					if (pPlayer->Collision(m_pos) == true)
					{
						pPlayer->AddHP(-8);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect_C02 ***
//=============================================================================
CEffect_C02::CEffect_C02() : CEffect()
{
	// �����l�ݒ�
	m_pSceneMesh = NULL;						// �V�[�����b�V��
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect_C02 ***
//=============================================================================
CEffect_C02::~CEffect_C02()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect_C02 ***
//=============================================================================
HRESULT CEffect_C02::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect_C02 ***
//=============================================================================
HRESULT CEffect_C02::Init(void)
{
	if (m_pSceneMesh == NULL)
	{
		m_pSceneMesh = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);

		m_pSceneMesh->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, -5.0f, 0.0f), 
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size, m_col, D3DXVECTOR2(1.0f, 1.0f));
		m_pSceneMesh->Init();
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Uninit(void)
{
	if (m_pSceneMesh != NULL)
	{
		m_pSceneMesh->Uninit();
		m_pSceneMesh = NULL;
	}

	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Update(void)
{
	if (m_pSceneMesh != NULL)
	{
		D3DXVECTOR3 size = m_pSceneMesh->Getsize();
		if (size.x < 60.0f)
		{
			size.x += 6.0f;
			size.y += 6.0f;

			m_size = size;
		}
		else
		{
			m_col.a -= 0.05f;
		}

		if (m_col.a < 0.0f)
		{
			Uninit();
			return;
		}
		else
		{
			m_pSceneMesh->SetStart(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, m_col, D3DXVECTOR2(1.0f, 1.0f));
		}

		ColisionChack();
	}
}

//=============================================================================
// �`�揈��										(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect_C02 ***
//=============================================================================
void CEffect_C02::ColisionChack(void)
{
	CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				CScene *pScene = NULL;		// �V�[��
				CScene *pSceneNext = NULL;		// ���V�[��

				if (m_pSceneMesh != NULL)
				{
					pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

					while (pScene != NULL)
					{// NULL�ȊO�̏ꍇ
						pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
							CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
							D3DXVECTOR3 pos = pEnemy->Getpos();
							float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
							if (fRange < m_size.x)
							{
								pEnemy->SetLockOn(true);
								if (pEnemy->HitDamage(-12) == true)
								{// �_���[�W����
									float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
									pEnemy->SetMove(D3DXVECTOR3(sinf(fAngle), 0.1f, cosf(fAngle)) * 30.0f);
									pEnemy->SetKnockBack(true);

									if (pEnemy->GetHP() == 0)
									{// HP���O�ɂȂ���
										pEnemy->Uninit();		// �I������

										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											pGame->SetSS(true);
										}
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(1);
											}
										}
									}
								}

								//Uninit();
							}
						}
						pScene = pSceneNext;	// ���V�[����
					}
				}
			}		
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					D3DXVECTOR3 pos = pPlayer->Getpos();
					float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
					if (fRange < m_size.x)
					{
						pPlayer->AddHP(-8);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect_C03 ***
//=============================================================================
CEffect_C03::CEffect_C03() : CEffect()
{
	// �����l�ݒ�
	m_pSceneMesh = NULL;		// �V�[�����b�V��

	m_nCntTimer = 0;			// ���ԃJ�E���g
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect_C03 ***
//=============================================================================
CEffect_C03::~CEffect_C03()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect_C03 ***
//=============================================================================
HRESULT CEffect_C03::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect_C03 ***
//=============================================================================
HRESULT CEffect_C03::Init(void)
{
	CScene *pScene = NULL;		// �V�[��
	CScene *pSceneNext = NULL;		// ���V�[��
	CSceneMesh *pMesh = NULL;

	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g

				break;
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	CTexAnim *pTexAnim = NULL; 
	// ���@�w
	pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size,
		D3DXVECTOR2(0.0714f, 1.0f), m_col, CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
	pTexAnim->SetAnim(14, 14, 3, 0, false, true);

	for (int nCount = 0; nCount < 5; nCount++)
	{
		D3DXVECTOR3 pos = m_pos;
		pos = pos + D3DXVECTOR3(sinf((D3DX_PI * 2.0f / 5) * nCount + m_rot.y), 0.0f, cosf((D3DX_PI * 2.0f / 5) * nCount + +m_rot.y)) * 100.0f;

		if (pMesh != NULL)
		{// 
			if (pMesh->ColInMesh(m_pos) == true)
			{// �͈̓`�F�b�N
				pos.y = pMesh->GetHeight(pos);
				pos.y += 1.8f;

				if (m_pos.y + 10.0f < pos.y)
				{
					pos.y = m_pos.y;
				}
			}
		}

		// ���@�w
		pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_size,
			D3DXVECTOR2(0.0714f, 1.0f), m_col, CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
		pTexAnim->SetAnim(14, 14, 3, 0, false, true);

		CEffect::Create(CEffect::TYPE_C04, m_nShot, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (m_pSceneMesh == NULL)
	{
		m_pSceneMesh = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_EFFECT);

		m_pSceneMesh->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(40.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f));
		m_pSceneMesh->Init();
		m_pSceneMesh->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
		m_pSceneMesh->DrawEdit(true, false, true, false, true, 0, D3DCULL_NONE);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Uninit(void)
{
	if (m_pSceneMesh != NULL)
	{
		m_pSceneMesh->Uninit();
		m_pSceneMesh = NULL;
	}

	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Update(void)
{
	if (m_pSceneMesh != NULL)
	{
		D3DXVECTOR3 size = m_pSceneMesh->Getsize();
		if (size.y < 8.0f)
		{
			size.y += 1.0f;

			m_size = size;
		}
		else
		{
			m_col.a -= 0.05f;
		}

		if (m_col.a < 0.0f)
		{
			Uninit();
			return;
		}
		else
		{
			m_pSceneMesh->SetStart(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, m_col, D3DXVECTOR2(1.0f, 1.0f));
		}

		ColisionChack();
	}
}

//=============================================================================
// �`�揈��										(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect_C03 ***
//=============================================================================
void CEffect_C03::ColisionChack(void)
{
	CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				CScene *pScene = NULL;		// �V�[��
				CScene *pSceneNext = NULL;		// ���V�[��

				pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

				while (pScene != NULL)
				{// NULL�ȊO�̏ꍇ
					pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

					if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
					{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
						CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
						D3DXVECTOR3 pos = pEnemy->Getpos();
						float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
						if (fRange < 120.0f)
						{
							float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
							if (pEnemy->GetType() == CEnemy::TYPE_N_SLIME)
								pEnemy->SetMove(D3DXVECTOR3(sinf(fAngle), 0.2f, cosf(fAngle)) * 30.0f);
							else
								pEnemy->SetMove(D3DXVECTOR3(sinf(fAngle), 0.01f, cosf(fAngle)) * 50.0f);

							pEnemy->SetKnockBack(true);
							pEnemy->SetLockOn(true);

							if (pEnemy->HitDamage(-26) == true)
							{// �_���[�W����
								if (pEnemy->GetHP() == 0)
								{// HP���O�ɂȂ���
									pEnemy->Uninit();		// �I������

									CGame *pGame = CManager::GetGame();
									if (pGame != NULL)
									{
										pGame->SetSS(true);
									}
								}
								else
								{
									CGame *pGame = CManager::GetGame();
									if (pGame != NULL)
									{
										CPlayer *pPlayer = pGame->GetPlayer();
										if (pPlayer != NULL)
										{
											pPlayer->AddMP(1);
										}
									}
								}
							}

							//Uninit();
						}
					}
					pScene = pSceneNext;	// ���V�[����
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					D3DXVECTOR3 pos = pPlayer->Getpos();
					float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
					if (fRange < 60.0f)
					{
						pPlayer->AddHP(-10);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CEffect_C04::m_nMaxModel	= 0;		// ���f����

LPDIRECT3DTEXTURE9		**CEffect_C04::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CEffect_C04::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CEffect_C04::m_pBuffMat	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CEffect_C04::m_nNumMat		= NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CEffect_C04::m_ModelInfo	= NULL;		// ���f���C���t�H

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect_C04 ***
//=============================================================================
CEffect_C04::CEffect_C04() : CEffect()
{
	// �����l�ݒ�
	m_pModel = NULL;		// ���f��

	m_nCntTimer = 0;		// ���ԃJ�E���g
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect_C04 ***
//=============================================================================
CEffect_C04::~CEffect_C04()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect_C04 ***
//=============================================================================
HRESULT CEffect_C04::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MODEL\\skillmodel.txt", &m_nMaxModel);		// ���f���擾

	// �e���̃������m��
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_nMaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_nMaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_nMaxModel];

	// �e���̏�����
	for (int nCount = 0; nCount < m_nMaxModel; nCount++)		// ���f�����J�E���g
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_nMaxModel; nCntParnet++)
	{// ���f�����J�E���g
		// X�t�@�C���̓ǂݍ���
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Unload(void)
{
	// �e���̃������J���ANULL��
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// ���f���C���t�H
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULL��
}

//=============================================================================
// ����������									(public)	*** CEffect_C04 ***
//=============================================================================
HRESULT CEffect_C04::Init(void)
{
	// ���f��
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULL�̏ꍇ
		m_pModel = new CModel;	// �������m��

		// �����l�ݒ�
		m_pModel->SetInitAll(m_pTexture[0], m_pMesh[0], m_pBuffMat[0], m_nNumMat[0], m_pos + D3DXVECTOR3(0.0f, -110.0f, 0.0f), D3DXVECTOR3(0.0f, (float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f, 0.0f));
		// ����������
		m_pModel->Init();

		// �e���f��
		m_pModel->SetParent(NULL);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Uninit(void)
{
	// ���f��
	if (m_pModel != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pModel->Uninit();		// �I������

		delete m_pModel;		// �������J��
		m_pModel = NULL;		// NULL��
	}

	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Update(void)
{
	if (m_pMesh != NULL)
	{
		if (m_nCntTimer < 10)
		{
			D3DXVECTOR3 pos = m_pModel->Getpos();
			pos.y += 10.0f;
			m_pModel->Setpos(pos);
		}
		else if (m_nCntTimer < 11)
		{
			D3DXVECTOR3 pos = m_pModel->Getpos();
			pos.y -= 3.0f;
			m_pModel->Setpos(pos);
		}

		if (30 < m_nCntTimer)
		{
			m_col.a -= 0.1f;
			m_pModel->Setcol_a(m_col.a);
		}

		ColisionChack();

		m_nCntTimer++;

		if (m_nCntTimer == 40)
		{
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��										(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{// ���f�����J�E���g
	 // ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		if (m_pModel != NULL)
		{// NULL�ȊO�̏ꍇ
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			m_pModel[nCntModel].Draw();

			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// �e�\��
	//		CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(0.0f, 0.1f, 0.0f));
		}
	}
}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect_C04 ***
//=============================================================================
void CEffect_C04::ColisionChack(void)
{
	/*CManager::MODE	 mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			if (m_nShot == SHOT_PLAYER)
			{
				CScene *pScene = NULL;		// �V�[��
				CScene *pSceneNext = NULL;		// ���V�[��

				if (m_pModel != NULL)
				{
					pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

					while (pScene != NULL)
					{// NULL�ȊO�̏ꍇ
						pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

						if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
						{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
							CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
							D3DXVECTOR3 pos = pEnemy->Getpos();
							float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
							if (fRange < 60.0f)
							{
								if (pEnemy->HitDamage(-10) == true)
								{// �_���[�W����
									if (pEnemy->GetHP() == 0)
									{// HP���O�ɂȂ���
										pEnemy->Uninit();		// �I������

										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											pGame->SetSS(true);
										}
									}
									else
									{
										CGame *pGame = CManager::GetGame();
										if (pGame != NULL)
										{
											CPlayer *pPlayer = pGame->GetPlayer();
											if (pPlayer != NULL)
											{
												pPlayer->AddMP(1);
											}
										}
									}
								}

								//Uninit();
								return;
							}
						}
						pScene = pSceneNext;	// ���V�[����
					}
				}
			}
			else if (m_nShot == SHOT_ENEMY)
			{
				CPlayer *pPlayer = pGame->GetPlayer();
				if (pPlayer != NULL)
				{
					D3DXVECTOR3 pos = pPlayer->Getpos();
					float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
					if (fRange < 60.0f)
					{
						pPlayer->AddHP(-10);
					}
				}
			}		
		}
	}*/
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect_Fire ***
//=============================================================================
CEffect_Fire::CEffect_Fire() : CEffect()
{
	// �����l�ݒ�
	m_nCntTimer = 0;			// ���ԃJ�E���g
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect_Fire ***
//=============================================================================
CEffect_Fire::~CEffect_Fire()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect_Fire ***
//=============================================================================
HRESULT CEffect_Fire::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect_Fire ***
//=============================================================================
HRESULT CEffect_Fire::Init(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Uninit(void)
{
	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Update(void)
{
	if (m_nCntTimer % 3 == 0)
	{
		CEffect_C00 *pEffect = NULL;
		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.08f, 0.3f, (-1.0f + (float)(rand() % 3))* 0.08f), m_size * 0.6f, D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));
		pEffect->SetDeleteTime(60);
		pEffect->SetSizeUp(true);

		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.1f, 0.3f, (-1.0f + (float)(rand() % 3))* 0.1f), m_size * 0.6f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		pEffect->SetDeleteTime(20);

		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.1f, 0.5f, (-1.0f + (float)(rand() % 3))* 0.1f), m_size, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		pEffect->SetDeleteTime(25);
	}

	m_nCntTimer++;
}

//=============================================================================
// �`�揈��										(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect_Fire ***
//=============================================================================
void CEffect_Fire::ColisionChack(void)
{
	
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CEffect_Create ***
//=============================================================================
CEffect_Create::CEffect_Create() : CEffect()
{
	// �����l�ݒ�
	m_nCntTimer = 0;			// ���ԃJ�E���g
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CEffect_Create ***
//=============================================================================
CEffect_Create::~CEffect_Create()
{

}

//=============================================================================
// ���[�h����									(public)	*** CEffect_Create ***
//=============================================================================
HRESULT CEffect_Create::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CEffect_Create ***
//=============================================================================
HRESULT CEffect_Create::Init(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Uninit(void)
{
	CEffect::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Update(void)
{
	for (int nCount = 0; nCount < 10; nCount++)
	{
		CEffect_C00 *pEffect = NULL;
		pEffect = (CEffect_C00*)CEffect::Create(CEffect::TYPE_C00, m_nShot, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3((-1.0f + (float)(rand() % 3))* 0.08f, 0.3f, (-1.0f + (float)(rand() % 3))* 0.08f), m_size * 0.6f, D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.7f));
		pEffect->SetDeleteTime(60);
		pEffect->SetSizeUp(true);
	}

	Uninit();
}

//=============================================================================
// �`�揈��										(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::Draw(void)
{

}

//=============================================================================
// ����`�F�b�N����								(public)	*** CEffect_Create ***
//=============================================================================
void CEffect_Create::ColisionChack(void)
{

}