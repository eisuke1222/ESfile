//=============================================================================
//
// �������� [explosion.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "explosion.h"		// ����
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "sound.h"			// �T�E���h
#include "camera.h"			// �J����
#include "game.h"			// �Q�[��
#include "player.h"			// �v���C���[
#include "enemy.h"			// �G
#include "object3d.h"		// �I�u�W�F�N�g
#include "effect3d.h"		// �G�t�F�N�g3D
#include "effect.h"			// �G�t�F�N�g
#include "life.h"			// �̗�
#include "score.h"			// �X�R�A

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\explosion000.png"		// �e�N�X�`��
#define TEX_UV_U			(0.125f)								// �e�N�X�`��UV_U	
#define TEX_UV_V			(0.5f)									// �e�N�X�`��UV_V
#define ANIM_SPEED			(3)										// �A�j���[�V�����X�s�[�h
#define ANIM_PATTERN		(16)									// �A�j���[�V�����p�^�[��
#define ANIM_LINENUM		(8)										// �A�j���[�V�����܂�Ԃ�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			CExplosion::m_pTexture = NULL;			// �e�N�X�`�����ւ̃|�C���^

//=============================================================================
// �R���X�g���N�^									(public)	*** CExplosion ***
//=============================================================================
CExplosion::CExplosion() : CScene3D(CScene::PRIORITY_4)
{
	m_nAnimCounter	= 0;			// �A�j���[�V�����J�E���^
	m_AnimPattern	= 0;			// �A�j���[�V�����p�^�[��
	m_explosionType = EXPLOSIONTYPE_NORMAL;		// �����^�C�v
	m_nShotIdx = 0;
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CExplosion ***
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// ���[�h����							(public)	*** CExplosion ***
//=============================================================================
HRESULT CExplosion::Load(void)
{
	if (m_pTexture == NULL)
	{// NULL�̏ꍇ
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTexture->Release();		// ���
		m_pTexture = NULL;			// NULL��
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CExplosion ***
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx)
{
	CExplosion *pExplosion = NULL;				// �|�C���^

	if (pExplosion == NULL)
	{// NULL�̏ꍇ
		pExplosion = new CExplosion;			// �������m��
	}

	if (pExplosion != NULL)
	{// NULL�ȊO�̏ꍇ
		pExplosion->Init(pos, size, col, explosionType, nShotIdx);		// ����������
		pExplosion->SetObjType(CScene::OBJTYPE_EXPLOSION);				// �I�u�W�F�N�g�^�C�v�ݒ�
		pExplosion->BindTexture(m_pTexture);							// �e�N�X�`���ݒ�
	}

	return pExplosion;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CExplosion ***
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx)
{
	CScene3D::SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, col, D3DXVECTOR2(TEX_UV_U, TEX_UV_V), CScene3D::SCENE3DTYPE_BILLBOARD);		// �����l�ݒ�
	CScene3D::Init();																															// ����������

	m_explosionType = explosionType;
	m_nShotIdx = nShotIdx;

	CSound *pSound = CManager::GetSound();							// �T�E���h

	pSound->PlaySoundA(CSound::SOUND_LABEL_SE005);

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Uninit(void)
{
	CScene3D::Uninit();			// �I������
}

//=============================================================================
// �X�V����											(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Update(void)
{
	m_nAnimCounter++;	// �A�j���[�V�����J�E���^

	if ((m_nAnimCounter % ANIM_SPEED) == 0)
	{// ���܂�̂Ȃ���
		m_AnimPattern = (m_AnimPattern + 1) % ANIM_PATTERN;		// �p�^�[����i�߂�
	}

	CScene3D::SetTex(ANIM_LINENUM, m_AnimPattern, D3DXVECTOR2(TEX_UV_U, TEX_UV_V), D3DXVECTOR2(0.0f, 0.0f));		// �p�^�[���X�V

	if (m_explosionType == EXPLOSIONTYPE_DAMAGE)
	{
		D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();	// �J���������_�擾
		D3DXVECTOR3 pos = CScene3D::Getpos();	// �ʒu���擾

		// ����`�F�b�N------------------------------------------------------------
		CModel3D *pModel3D;						// �V�[��3D
		CScene::OBJTYPE objType;				// �I�u�W�F�N�g���
		D3DXVECTOR3 targetpos, targetsize;		// ����p : ����̈ʒu�A����̑傫��											
		CEnemy *pEnemy;							// �G
		CEnemy::ENEMYTYPE enemyType;			// �G���
		CObject3D *pObject3D;					// �I�u�W�F�N�g3D
		CObject3D::OBJECTTYPE objectType;		// �I�u�W�F�N�g���

		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_1, nCntScene);	// �V�[���ǂݍ���

			if (pModel3D != NULL)
			{// NULL�ȊO�̏ꍇ
				objType = pModel3D->GetObjType();			// �I�u�W�F�N�g�^�C�v�擾

				if (objType == CScene::OBJTYPE_OBJ3D)
				{// ��ރ`�F�b�N
				 // ����̏��擾
					pObject3D = (CObject3D*)pModel3D;					// �I�u�W�F�N�g�^
					objectType = pObject3D->GetobjectType();			// �I�u�W�F�N�g�^�C�v�擾

					if (objectType != CObject3D::OBJECTTYPE_O_G && pObject3D->GetDelete() == false)
					{
						targetpos = pObject3D->Getpos();		// �ʒu	
						targetsize = pObject3D->Getsize(0);		// �傫��

						D3DXVECTOR3 posOBJ;
						D3DXVECTOR3 sizeOBJ;

						if (objectType == CObject3D::OBJECTTYPE_BILL_1)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.8f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.8f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.8f, 0.8f, 0.8f);
						}
						else if (objectType == CObject3D::OBJECTTYPE_BILL_2)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.6f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.6f, 0.6f, 0.6f);
						}
						else if (objectType == CObject3D::OBJECTTYPE_BILL_3)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.7f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.8f, 0.8f, 0.8f);
						}

						if (objectType != CObject3D::OBJECTTYPE_LIFE)
						{

							//if (((targetpos.x - posOBJ.x) * (targetpos.x - posOBJ.x)) + ((targetpos.z - posOBJ.z) * (targetpos.z - posOBJ.z)) < (targetsize.x + BULLET_SIZE_X) * (targetsize.x + BULLET_SIZE_X))
							if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + (EXPLOSION_SIZE_X * 4.0f)) * (sizeOBJ.z + (EXPLOSION_SIZE_X * 4.0f)))
							{// �͈̓`�F�b�N�i�~�A��ʊO�j
								pObject3D->HitDamage(3, CObject3D::BREAKTYPE_FALL, atan2f(posOBJ.x - (pos.x), posOBJ.z - (pos.z)), m_nShotIdx);			// �_���[�W

								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// �J�E���g
									CEffect3D::Create(D3DXVECTOR3(targetpos.x, targetpos.y + (targetsize.y * 0.75f), targetpos.z),
										D3DXVECTOR3(-D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f))),
										D3DXVECTOR3(0.02f, -0.001f, 0.03f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, 2.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f));
								}
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// �J�E���g
									CEffect::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y, targetpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, -1.8f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f - 1.5f),
										D3DXVECTOR3(EFFECT_SIZE_X * 2.5f, EFFECT_SIZE_Y * 2.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
								}

								break;
							}
						}
					}
				}
			}
		}

		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCntScene);	// �V�[���ǂݍ���

			if (pModel3D != NULL)
			{// NULL�ȊO�̏ꍇ
				objType = pModel3D->GetObjType();			// �I�u�W�F�N�g�^�C�v�擾

				if (objType == CScene::OBJTYPE_ENEMY)
				{// ��ރ`�F�b�N
				 // ����̏��擾
					targetpos = pModel3D->Getpos();			// �ʒu	
					targetsize = pModel3D->Getsize(0);		// �傫��

					if (objType == OBJTYPE_ENEMY)
					{	// �G
						pEnemy = (CEnemy*)pModel3D;		// �G�^�ɕϊ�

						enemyType = pEnemy->GetenemyType();

						D3DXVECTOR3 posOBJ;
						D3DXVECTOR3 sizeOBJ;

						if (enemyType == CEnemy::ENEMYTYPE_S_1 || enemyType == CEnemy::ENEMYTYPE_S_2)
						{
							posOBJ = targetpos;
							sizeOBJ = targetsize;
						}
						else if (enemyType == CEnemy::ENEMYTYPE_G_1 || enemyType == CEnemy::ENEMYTYPE_G_2)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);
						}
						else if (enemyType == CEnemy::ENEMYTYPE_O_1)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.55f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);
						}
						else if (enemyType == CEnemy::ENEMYTYPE_B_1)
						{
							posOBJ = D3DXVECTOR3(
								CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.55f),
								300.0f,
								CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
							sizeOBJ = targetsize;
							sizeOBJ *= *D3DXVECTOR3(0.3f, 0.5f, 0.5f);
						}
						else if (enemyType == CEnemy::ENEMYTYPE_B_2)
						{
							posOBJ = targetpos;
							sizeOBJ = targetsize;
						}
						/*else if (enemyType == CEnemy::ENEMYTYPE_B_3)
						{
						posOBJ = D3DXVECTOR3(
						CameraposR.x + (((targetpos.x) - (CameraposR.x)) * 0.6f),
						300.0f,
						CameraposR.z - 350.0f + (((targetpos.z) - (CameraposR.z - 300.0f)) * 0.55f));
						sizeOBJ = targetsize;
						sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);
						}*/

						if (enemyType != CEnemy::ENEMYTYPE_B_3)
						{
							if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + (EXPLOSION_SIZE_X * 4.0f)) * (sizeOBJ.z + (EXPLOSION_SIZE_X * 4.0f)))
							{// �͈̓`�F�b�N�i�~�A��ʊO�j
								if (pEnemy->HitDamage(4) == true)
								{// HP���O�ȉ��ɂȂ���
									D3DXVECTOR3 posModel = pEnemy->Getpos();		// �ʒu�擾

									CExplosion::Create(posOBJ, D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// �����𐶐�

									pEnemy->Uninit();			// �I������							

									for (int nCnt = 0; nCnt < 15; nCnt++)
									{// �J�E���g
										CEffect::Create(posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, -5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
											D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.1f, -0.1f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
									}

									CScore::AddScore(m_nShotIdx, 1300);		// �X�R�A���Z

									if (enemyType == CEnemy::ENEMYTYPE_B_1)
									{
										for (int nCnt = 0; nCnt < MAX_SCENE; nCnt++)
										{// �V�[���J�E���g
											CModel3D *pEnemyDelete = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCnt);	// �V�[���ǂݍ���

											if (pEnemyDelete != NULL)
											{// NULL�ȊO�̏ꍇ
												CScene::OBJTYPE enemyObj = pEnemyDelete->GetObjType();			// �I�u�W�F�N�g�^�C�v�擾

												if (enemyObj == CScene::OBJTYPE_ENEMY)
												{
													CExplosion::Create(pEnemyDelete->Getpos(), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// �����𐶐�

													pEnemyDelete->Uninit();
												}
											}
										}

										CManager::MODE mode = CManager::GetMode();
										CGame::GAME game = CGame::GetGame();

										if (mode == CManager::MODE_GAME)
										{
											if (game == CGame::GAME_PLAY)
											{
												CGame::SetGame(CGame::GAME_CLEAR);
											}
										}

										CScore::AddScore(m_nShotIdx, 2000);		// �X�R�A���Z
									}
								}

								// �ʒu����
								pos = posOBJ;

								break;
							}
						}
					}
				}
			}
		}
	}

	CScene3D::Update();			// �X�V����

	if (m_AnimPattern == ANIM_PATTERN - 1)
	{// �p�^�[�����Ō�܂œ��B������
		Uninit();				// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CExplosion ***
//=============================================================================
void CExplosion::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�f�t�H���g�̏ꍇFALSE�ɂȂ��Ă��邩��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//��r���@�i���傫���j�j
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	CScene3D::Draw();			// �`�揈��

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//��r���@�i���傫���j�j
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}