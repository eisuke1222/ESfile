//=============================================================================
//
// �e���� [bullet.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "bullet.h"			// �e
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����
#include "explosion.h"		// ����
#include "player.h"			// �v���C���[
#include "enemy.h"			// �G
#include "object3d.h"		// �I�u�W�F�N�g
#include "effect3d.h"		// �G�t�F�N�g3D
#include "effect.h"			// �G�t�F�N�g
#include "life.h"			// �̗�
#include "score.h"			// �X�R�A
#include "game.h"			// �Q�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_0	"data\\TEXTURE\\bullet000.png"		// �e�N�X�`��1
#define TEXTURE_NAME_1	"data\\TEXTURE\\bomb.png"			// �e�N�X�`��2

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CBullet::m_pTexture[BULLET_MAX_TEXTURE] = {};		// �e�N�X�`�����ւ̃|�C���^

//=============================================================================
// �R���X�g���N�^									(public)	*** CBullet ***
//=============================================================================
CBullet::CBullet() : CScene3D(CScene::PRIORITY_3)
{
	// �����l�ݒ�
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_move	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CBullet ***
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CBullet::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_pTexture[0] == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_0, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CBullet::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < BULLET_MAX_TEXTURE; nCntTex++)
	{// �J�E���g
		if (m_pTexture[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[nCntTex]->Release();		// ���
			m_pTexture[nCntTex] = NULL;			// NULL��
		}
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CBullet ***
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState)
{
	CBullet *pBullet = NULL;				// �|�C���^

	if (pBullet == NULL)
	{// NULL�̏ꍇ
		pBullet = new CBullet;				// �������m��
	}

	if (pBullet != NULL)
	{// NULL�ȊO�̏ꍇ
		pBullet->Init(pos, move, size, col, bulletType, bulletState);	// ����������
		pBullet->SetObjType(CScene::OBJTYPE_BULLET);					// �I�u�W�F�N�g�^�C�v�ݒ�

		pBullet->BindTexture(m_pTexture[bulletState]);					// �e�N�X�`���ݒ�	
	}

	return pBullet;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CBullet ***
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState)
{
	CScene3D::SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, col, D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);			// �����ݒ�
	CScene3D::Init();						// ����������

	m_bulletType = bulletType;				// ���
	m_bulletState = bulletState;			// ���
	m_move = move;							// �ړ��ʑ��
	m_Life = 1 * 60;						// �̗�

	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();	// �J���������_�擾

	m_pos = pos - Camerapos;				// �J�����ړ�������

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CBullet ***
//=============================================================================
void CBullet::Uninit(void)
{
	CScene3D::Uninit();						// �I������
}

//=============================================================================
// �X�V����											(public)	*** CBullet ***
//=============================================================================
void CBullet::Update(void)
{
	D3DXVECTOR3 CameraposV = CManager::GetCamera()->GetposV();	// �J�������_�擾
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();	// �J���������_�擾
	D3DXVECTOR3 pos = CScene3D::Getpos();	// �ʒu���擾
	int nBulletDamage;						// �e�_���[�W

	if (m_bulletState == BULLETSTATE_NORMAL)
	{
		nBulletDamage = 1;
	}
	else if (m_bulletState == BULLETSTATE_BOMB)
	{
		nBulletDamage = 3;
	}


	// �J�����ɑ΂��Ă̈ʒu��ݒ�
	pos.x = CameraposR.x + m_pos.x;
	pos.z = CameraposR.z + m_pos.z;			

	pos += m_move;							// �ړ��ʑ��

	m_Life--;								// ���C�t����

	CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_BULLET, CEffect::TEXTURE_EFFECT);		// �G�t�F�N�g

	// ����`�F�b�N------------------------------------------------------------
	CModel3D *pModel3D;						// �V�[��3D
	CScene::OBJTYPE objType;				// �I�u�W�F�N�g���
	D3DXVECTOR3 targetpos, targetsize;		// ����p : ����̈ʒu�A����̑傫��											
	CPlayer *pPlayer;						// �v���C���[
	CEnemy *pEnemy;							// �G
	CEnemy::ENEMYTYPE enemyType;			// �G���
	CObject3D *pObject3D;					// �I�u�W�F�N�g3D
	CObject3D::OBJECTTYPE objectType;		// �I�u�W�F�N�g���
	bool bExplosion = false;				// ����
	bool bDelete = false;					// �j��

	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{// �V�[���J�E���g
		pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_1, nCntScene);	// �V�[���ǂݍ���

		if (pModel3D != NULL)
		{// NULL�ȊO�̏ꍇ
			objType = pModel3D->GetObjType();			// �I�u�W�F�N�g�^�C�v�擾

			if (objType == CScene::OBJTYPE_OBJ3D && (m_bulletType == BULLETTYPE_PLAYER_1 || m_bulletType == BULLETTYPE_PLAYER_2))
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
						if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + BULLET_SIZE_X) * (sizeOBJ.z + BULLET_SIZE_X))
						{// �͈̓`�F�b�N�i�~�A��ʊO�j
							bExplosion = true;					// ��������
							m_Life = 0;							// �̗͂O��
							bDelete = true;						// �j������

							if (pObject3D->HitDamage(nBulletDamage, (CObject3D::BREAKTYPE)m_bulletState, atan2f(posOBJ.x - (pos.x), posOBJ.z - (pos.z)), m_bulletType) == true)
							{
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// �J�E���g
									CEffect3D::Create(D3DXVECTOR3(targetpos.x, targetpos.y + (targetsize.y * 0.75f), targetpos.z),
										D3DXVECTOR3(-D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f)), -D3DX_PI + ((D3DX_PI * 2.0f) * ((rand() % 100 + 1) * 0.01f))),
										D3DXVECTOR3(0.02f, -0.001f, 0.03f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, 2.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f));
								}
							}

							for (int nCnt = 0; nCnt < 15; nCnt++)
							{// �J�E���g
								CEffect::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y + EFFECT_SIZE_Y, targetpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f, -1.8f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 0.8f - 1.5f),
									D3DXVECTOR3(EFFECT_SIZE_X * 2.5f, EFFECT_SIZE_Y * 2.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);
							}

							// �ʒu����
							//pos = posOBJ;

							CScore::AddScore(m_bulletType, 800);		// �X�R�A���Z

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

			if (objType == CScene::OBJTYPE_PLAYER && m_bulletType == BULLETTYPE_ENEMY ||
				objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER_1 ||
				objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER_2)
			{// ��ރ`�F�b�N
				// ����̏��擾
				targetpos = pModel3D->Getpos();			// �ʒu	
				targetsize = pModel3D->Getsize(0);		// �傫��

				if (objType == OBJTYPE_PLAYER)
				{	// �v���C���[
					if (((targetpos.x - pos.x) * (targetpos.x - pos.x)) + ((targetpos.z - pos.z) * (targetpos.z - pos.z)) < (targetsize.x + BULLET_SIZE_X) * (targetsize.x + BULLET_SIZE_X))
					{// �͈̓`�F�b�N�i�~�A��ʊO�j
						bExplosion = true;					// ��������
						m_Life = 0;							// �̗͂O��
						bDelete = true;						// �j������

						pPlayer = (CPlayer*)pModel3D;	// �v���C���[�^�ɕϊ�

						if (pPlayer->HitDamage(nBulletDamage) == true)
						{// HP���O�ȉ��ɂȂ���
							bExplosion = true;			// ��������
							D3DXVECTOR3 posModel = pPlayer->Getpos();	// �ʒu�擾
							int nPlayerNum = pPlayer->GetPlayerNum();	// �v���C���[�ԍ�

							pPlayer->Uninit();			// �I������

							for (int nCnt = 0; nCnt < 15; nCnt++)
							{// �J�E���g
								CEffect::Create(posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, -5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
									D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.1f, -0.1f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
							}

							CLife::SetRespawn(nPlayerNum, true);	// �̗́i�c�@�j�����炷
							CLife::SetLiveState(nPlayerNum);		// ������
						}

						break;
					}
				}
				else if (objType == OBJTYPE_ENEMY)
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
						if (((posOBJ.x - pos.x) * (posOBJ.x - pos.x)) + ((posOBJ.z - pos.z) * (posOBJ.z - pos.z)) < (sizeOBJ.x + BULLET_SIZE_X) * (sizeOBJ.z + BULLET_SIZE_X))
						{// �͈̓`�F�b�N�i�~�A��ʊO�j
							bExplosion = true;					// ��������
							m_Life = 0;							// �̗͂O��
							bDelete = true;						// �j������

							if (pEnemy->HitDamage(nBulletDamage) == true)
							{// HP���O�ȉ��ɂȂ���
								bExplosion = true;			// ��������
								D3DXVECTOR3 posModel = pEnemy->Getpos();		// �ʒu�擾

								pEnemy->Uninit();			// �I������							

								for (int nCnt = 0; nCnt < 15; nCnt++)
								{// �J�E���g
									CEffect::Create(posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, -5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
										D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.1f, -0.1f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
								}

								CExplosion::Create(posOBJ, D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_bulletType);						// �����𐶐�

								if (m_bulletState == BULLETSTATE_NORMAL)
								{
									CScore::AddScore(m_bulletType, 1000);		// �X�R�A���Z
								}
								else
								{
									CScore::AddScore(m_bulletType, 1200);		// �X�R�A���Z
								}

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
												CExplosion::Create(pEnemyDelete->Getpos(), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_bulletType);						// �����𐶐�

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
								}
							}

							// �ʒu����
							//pos = posOBJ;

							break;
						}
					}
				}
			}
		}
	}

	if (pos.x < CameraposR.x - 260.0f || CameraposR.x + 260.0f < pos.x || pos.z < CameraposR.z - 400.0f || CameraposR.z + 180.0f < pos.z)
	{// �͈̓`�F�b�N�i�~�A��ʊO�j
		bDelete = true;			// �j������
	}
	if (m_Life < 0 && m_bulletState == BULLETSTATE_BOMB)
	{// �͈̓`�F�b�N�i�~�A��ʊO�j		
		bExplosion = true;		// ��������
		bDelete = true;			// �j������
	}

	if (bExplosion == true)
	{// ��������
		switch (m_bulletState)
		{// �e���
		case BULLETSTATE_NORMAL:	// �ʏ�
			CExplosion::Create(pos, D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_bulletType);						// �����𐶐�

			break;
		case BULLETSTATE_BOMB:		// �{��
			CExplosion::Create(pos, D3DXVECTOR3(EXPLOSION_SIZE_X * 4.0f, EXPLOSION_SIZE_Y * 4.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_DAMAGE, m_bulletType);		// �����𐶐�

			break;
		}
	}

	CScene3D::Setpos(pos);			// �ʒu��ݒ�

	m_pos = pos - CameraposR;		// ���݈ʒu���v�Z

	if (bDelete == true)
	{// �j������
		Uninit();					// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CBullet ***
//=============================================================================
void CBullet::Draw(void)
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

	CScene3D::Draw();		// �`�揈��

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//��r���@�i���傫���j�j
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}