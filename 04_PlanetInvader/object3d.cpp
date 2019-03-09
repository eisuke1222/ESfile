//=============================================================================
//
// �I�u�W�F�N�g���� [object3d.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "object3d.h"		// �I�u�W�F�N�g
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "sound.h"			// �T�E���h
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����
#include "player.h"			// �v���C���[
#include "enemy.h"			// �G
#include "effect.h"			// �G�t�F�N�g
#include "life.h"			// ���C�t
#include "explosion.h"		// ����
#include "score.h"			// �X�R�A

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME_1		"data\\MODEL\\o_g000.x"		// ���f��1
#define MODEL_NAME_2		"data\\MODEL\\bill000.x"	// ���f��2
#define MODEL_NAME_3		"data\\MODEL\\bill001.x"	// ���f��3
#define MODEL_NAME_4		"data\\MODEL\\bill002.x"	// ���f��4
#define MODEL_NAME_5		"data\\MODEL\\bill003.x"	// ���f��5

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CObject3D::m_pTexture[OBJECTTYPE_MAX] = {};		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				CObject3D::m_pMesh[OBJECTTYPE_MAX] = {};			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			CObject3D::m_pBuffMat[OBJECTTYPE_MAX] = {};			// �}�e���A�����ւ̃|�C���^
DWORD					CObject3D::m_nNumMat[OBJECTTYPE_MAX] = {};			// �}�e���A�����̐�

//=============================================================================
// �R���X�g���N�^									(public)	*** CObject3D ***
//=============================================================================
CObject3D::CObject3D() : CModel3D(CScene::PRIORITY_1)
{
	m_objectType = OBJECTTYPE_BILL_1;	// ���f���^�C�v
	m_breakType = BREAKTYPE_NORMAL;		// �j��^�C�v
	m_bDelete = false;					// �j��
	m_nLife = 0;						// �̗�
	m_nCntTimer = 0;					// ���ԃJ�E���^
	m_breakrot = 0.0f;					// �j�����
	m_breakrotDiff = 0.0f;				// �j������c��
	m_nShotIdx = 0;						// ����ID
	m_nCntBreak = 0;
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CObject3D ***
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CObject3D::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	char cName[255] = {};		// ���O�i�[

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{// �J�E���g
		if (m_pBuffMat[nCntModel] == NULL && m_pMesh[nCntModel] == NULL)
		{// NULL�̏ꍇ		
			switch (nCntModel)
			{
			case OBJECTTYPE_O_G:
				wsprintf(cName, MODEL_NAME_1);

				break;
			case OBJECTTYPE_BILL_1:
				wsprintf(cName, MODEL_NAME_2);

				break;
			case OBJECTTYPE_BILL_2:
				wsprintf(cName, MODEL_NAME_3);

				break;
			case OBJECTTYPE_BILL_3:
				wsprintf(cName, MODEL_NAME_4);

				break;
			case OBJECTTYPE_LIFE:
				wsprintf(cName, MODEL_NAME_5);

				break;
			}

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(cName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntModel], NULL, &m_nNumMat[nCntModel], &m_pMesh[nCntModel]);

			D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
			D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
			pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
			matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
			m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

			for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
			{// �J�E���g
				matDef[tex] = pmat[tex].MatD3D;
				matDef[tex].Ambient = matDef[tex].Diffuse;
				m_pTexture[nCntModel][tex] = NULL;
				if (pmat[tex].pTextureFilename != NULL &&
					lstrlen(pmat[tex].pTextureFilename) > 0)
				{// �e�N�X�`�����g�p���Ă���
					if (FAILED(D3DXCreateTextureFromFile(pDevice,
						pmat[tex].pTextureFilename,
						&m_pTexture[nCntModel][tex])))
					{
						MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CObject3D::Unload(void)
{

	for (int nCntModel = 0; nCntModel < OBJECTTYPE_MAX; nCntModel++)
	{// �J�E���g
		// �e�N�X�`���̔j��
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// �J�E���g
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pTexture[nCntModel][tex]->Release();		// ���
				m_pTexture[nCntModel][tex] = NULL;			// NULL��
			}
		}

		// �}�e���A���̊J��
		if (m_pBuffMat[nCntModel] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pBuffMat[nCntModel]->Release();				// ���
			m_pBuffMat[nCntModel] = NULL;					// NULL��
		}

		// �}�e���A���̊J��
		if (m_pMesh[nCntModel] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pMesh[nCntModel]->Release();					// ���
			m_pMesh[nCntModel] = NULL;						// NULL��
		}

		m_nNumMat[nCntModel] = 0;				// ������
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CObject3D ***
//=============================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType)
{
	CObject3D *pObject3D = NULL;			// �|�C���^

	if (pObject3D == NULL)
	{// NULL�̏ꍇ
		pObject3D = new CObject3D;			// �������m��
	}

	if (pObject3D != NULL)
	{// NULL�ȊO�̏ꍇ
		pObject3D->Init(pos, rot, objectType);				// ����������
		pObject3D->SetObjType(CScene::OBJTYPE_OBJ3D);		// �I�u�W�F�N�g�^�C�v�ݒ�
	}

	return pObject3D;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CObject3D ***
//=============================================================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType)
{
	CModel3D::SetInitAll(m_pTexture[objectType], m_pMesh[objectType], m_pBuffMat[objectType], m_nNumMat[objectType], pos, rot);		// �����l�ݒ�
	CModel3D::Init();																// ����������

	m_objectType = objectType;				// �I�u�W�F�N�g�^�C�v
	m_bDelete = false;						// �j��

	switch (m_objectType)
	{
	case OBJECTTYPE_O_G:
	case OBJECTTYPE_LIFE:
		m_nLife = 0;

		break;
	case OBJECTTYPE_BILL_1:
		m_nLife = 5;

		break;
	case OBJECTTYPE_BILL_2:
		m_nLife = 3;

		break;
	case OBJECTTYPE_BILL_3:
		m_nLife = 7;

		break;
	}

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CObject3D ***
//=============================================================================
void CObject3D::Uninit(void)
{
	CModel3D::Uninit();						// �I������
}

//=============================================================================
// �X�V����											(public)	*** CObject3D ***
//=============================================================================
void CObject3D::Update(void)
{
	CSound *pSound = CManager::GetSound();							// �T�E���h
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();	// �J�������_�擾
	D3DXVECTOR3 CameraposV = CManager::GetCamera()->GetposV();	// �J�������_�擾
	D3DXVECTOR3 pos = CObject3D::Getpos();						// �ʒu�擾
	D3DXVECTOR3 sizeMax = CObject3D::Getsize(0);				// �傫���擾
	D3DXVECTOR3 sizeMin = CObject3D::Getsize(1);				// �傫���擾
	bool bDelete = false;

	if (m_objectType != OBJECTTYPE_O_G)
	{
		if (m_objectType == OBJECTTYPE_LIFE)
		{
			pos.z -= 3.0f;

			// ����`�F�b�N------------------------------------------------------------
			CModel3D *pModel3D;						// �V�[��3D
			CScene::OBJTYPE objType;				// �I�u�W�F�N�g���
			D3DXVECTOR3 targetpos, targetsize;		// ����p : ����̈ʒu�A����̑傫��											
			CPlayer *pPlayer;						// �v���C���[

			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{// �V�[���J�E���g
				pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCntScene);	// �V�[���ǂݍ���

				if (pModel3D != NULL)
				{// NULL�ȊO�̏ꍇ
					objType = pModel3D->GetObjType();			// �I�u�W�F�N�g�^�C�v�擾

					if (objType == OBJTYPE_PLAYER)
					{	// �v���C���[
						// ����̏��擾
						targetpos = pModel3D->Getpos();			// �ʒu	
						targetsize = pModel3D->Getsize(0);		// �傫��

						D3DXVECTOR3 posOBJ = D3DXVECTOR3(
							CameraposR.x + (((pos.x) - (CameraposR.x)) * 0.6f),
							300.0f,
							CameraposR.z - 350.0f + (((pos.z) - (CameraposR.z - 300.0f)) * 0.55f));
						D3DXVECTOR3 sizeOBJ = sizeMax;
						sizeOBJ *= *D3DXVECTOR3(0.5f, 0.5f, 0.5f);

						if (((targetpos.x - posOBJ.x) * (targetpos.x - posOBJ.x)) + ((targetpos.z - posOBJ.z) * (targetpos.z - posOBJ.z)) < (targetsize.x + sizeOBJ.x) * (targetsize.x + sizeOBJ.x))
						{// �͈̓`�F�b�N�i�~�A��ʊO�j
							Uninit();

							/*for (int nCnt = 0; nCnt < 15; nCnt++)
							{// �J�E���g
								CEffect::Create(posOBJ, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f, 5.0f, cosf(((D3DX_PI * 2.0f) / 15.0f) * nCnt) * 1.0f),
									D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.2f, -0.2f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_EXPLOSION, CEffect::TEXTURE_EFFECT);
							}*/

							pPlayer = (CPlayer*)pModel3D;

							CLife::SetAdd(pPlayer->GetPlayerNum(), true);	// �̗́i�c�@�j�����炷

							pSound->PlaySoundA(CSound::SOUND_LABEL_SE009);

							break;
						}
					}
				}
			}
		}
		else
		{// ���E�ȊO
			if (m_bDelete == true)
			{// �j�󂷂�
				m_nCntTimer++;

				if (m_objectType != OBJECTTYPE_BILL_3)
				{
					if (m_breakType == BREAKTYPE_NORMAL)
					{
						if (30.0f < pos.y + sizeMax.y)
						{// ���ʒu�܂�
							pos.y -= 2.0f;			// ������
						}

						if (m_nCntTimer % 3 == 0)
						{
							CEffect::Create(D3DXVECTOR3(pos.x + sizeMin.x + (rand() % (int)(-sizeMin.x + sizeMax.x) + 1), pos.y + sizeMax.y + EFFECT_SIZE_Y, pos.z + sizeMin.z + (rand() % (int)(-sizeMin.z + sizeMax.z) + 1)),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(((D3DX_PI * 2.0f) / 15.0f) * (rand() % 15 * 1.0f)) * 0.01f, 0.01f, cosf(((D3DX_PI * 2.0f) / 15.0f) * (rand() % 15 * 1.0f)) * 0.01f),
								D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(0.8f, 0.8f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.015f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// �G�t�F�N�g
						}
					}
					else if (m_breakType == BREAKTYPE_FALL)
					{
						if (0.0f < m_breakrotDiff)
						{
							D3DXVECTOR3 rot = CObject3D::Getrot();
							float frot_x, frot_z;
							frot_x = 0.01f + (0.01f * (m_nCntTimer / 10));
							frot_z = 0.01f + (0.01f * (m_nCntTimer / 10));

							m_breakrotDiff -= frot_x;
							m_breakrotDiff -= frot_z;

							CObject3D::Setrot(D3DXVECTOR3(CObject3D::Getrot().x + frot_x, CObject3D::Getrot().y, CObject3D::Getrot().z + sinf(-m_breakrot) * frot_z));		// �ʒu�X�V

							if (m_breakrotDiff <= D3DX_PI * 0.5f && m_nCntBreak < 10)
							{
								CEffect::Create(D3DXVECTOR3(pos.x + sinf(m_breakrot) * ((sizeMax.y / 10) * m_nCntBreak), pos.y + sizeMax.x + EFFECT_SIZE_Y, pos.z + cosf(-m_breakrot) * ((sizeMax.y / 10) * m_nCntBreak)),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.8f, -2.8f),
									D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(1.3f, 1.3f, 0.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), 0.008f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// �G�t�F�N�g

								m_nCntBreak++;
							}

							if (m_breakrotDiff <= 0.0f)
							{
								for (int nCnt = 0; nCnt < 15; nCnt++)
								{
									CEffect::Create(D3DXVECTOR3(pos.x + sinf(m_breakrot) * ((sizeMax.y / 15) * nCnt), pos.y + sizeMax.x + EFFECT_SIZE_Y, pos.z + cosf(-m_breakrot) * ((sizeMax.y / 15) * nCnt)),
										D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_breakrot + (((D3DX_PI * 2.0f) / 15) * nCnt)) * 2.2f, 0.0f, cosf(m_breakrot + (((D3DX_PI * 2.0f) / 15) * nCnt)) * -2.2f + -2.0f),
										D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// �G�t�F�N�g
								}

								// ����`�F�b�N------------------------------------------------------------
								CModel3D *pModel3D;						// �V�[��3D
								CScene::OBJTYPE objType;				// �I�u�W�F�N�g���
								D3DXVECTOR3 targetpos, targetsize;		// ����p : ����̈ʒu�A����̑傫��											
								bool bExplosion = false;				// ����
								bool bDelete = false;					// �j��

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

											CEnemy *pEnemy = (CEnemy*)pModel3D;						// �G
											CEnemy::ENEMYTYPE enemyType = pEnemy->GetenemyType();	// �G���

											if (enemyType == CEnemy::ENEMYTYPE_G_1)
											{
												for (int nCnt = 0; nCnt < 10; nCnt++)
												{
													D3DXVECTOR3 posObj = D3DXVECTOR3(
														pos.x + sinf(m_breakrot) * ((sizeMax.y / 10) * nCnt),
														pos.y,
														pos.z + cosf(-m_breakrot) * ((sizeMax.y / 10) * nCnt));

													if ((((targetpos.x - posObj.x) * (targetpos.x - posObj.x)) + ((targetpos.z - posObj.z) * (targetpos.z - posObj.z))) < (((targetsize.x - (sizeMax.x * 5.0f)) * (targetsize.x - (sizeMax.x * 5.0f)))))
													{
														CExplosion::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y, targetpos.z), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// �����𐶐�

														pEnemy->Uninit();

														CScore::AddScore(m_nShotIdx, 1500);		// �X�R�A���Z

														break;
													}
												}
											}
										}
									}
								}
							}
						}
						else
						{
							// ����`�F�b�N------------------------------------------------------------
							CModel3D *pModel3D;						// �V�[��3D
							CScene::OBJTYPE objType;				// �I�u�W�F�N�g���
							D3DXVECTOR3 targetpos, targetsize;		// ����p : ����̈ʒu�A����̑傫��											
							bool bExplosion = false;				// ����
							bool bDelete = false;					// �j��

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

										CEnemy *pEnemy = (CEnemy*)pModel3D;						// �G
										CEnemy::ENEMYTYPE enemyType = pEnemy->GetenemyType();	// �G���

										if (enemyType == CEnemy::ENEMYTYPE_G_1)
										{
											for (int nCnt = 0; nCnt < 10; nCnt++)
											{
												D3DXVECTOR3 posObj = D3DXVECTOR3(
													pos.x + sinf(m_breakrot) * ((sizeMax.y / 10) * nCnt),
													pos.y,
													pos.z + cosf(-m_breakrot) * ((sizeMax.y / 10) * nCnt));

												if ((((targetpos.x - posObj.x) * (targetpos.x - posObj.x)) + ((targetpos.z - posObj.z) * (targetpos.z - posObj.z))) < (((targetsize.x - (sizeMax.x * 5.0f)) * (targetsize.x - (sizeMax.x * 5.0f)))))
												{
													CExplosion::Create(D3DXVECTOR3(targetpos.x, targetpos.y + targetsize.y, targetpos.z), D3DXVECTOR3(EXPLOSION_SIZE_X * 2.0f, EXPLOSION_SIZE_Y * 2.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CExplosion::EXPLOSIONTYPE_NORMAL, m_nShotIdx);						// �����𐶐�

													pEnemy->Uninit();

													CScore::AddScore(m_nShotIdx, 1500);		// �X�R�A���Z

													break;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				else
				{
					m_nCntBreak++;

					if (m_nCntBreak < 45)
					{
						if (m_nCntBreak % 3 == 0)
						{
							CExplosion::Create(D3DXVECTOR3(pos.x - sizeMax.x + ((sizeMax.x * 2.0f / 5.0f) * (rand() % 5)), pos.y + sizeMax.y + EFFECT_SIZE_Y, pos.z - sizeMax.z + ((sizeMax.z * 2.0f / 5.0f) * (rand() % 5))),
								D3DXVECTOR3(EXPLOSION_SIZE_X * 4.0f, EXPLOSION_SIZE_Y * 4.0f, 0.0f), D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), CExplosion::EXPLOSIONTYPE_DAMAGE, m_nShotIdx);		// �����𐶐�
						}
					}
					else
					{
						if (m_nCntBreak == 345)
						{
							for (int nCnt = 0; nCnt < 15; nCnt++)
							{
								CEffect::Create(D3DXVECTOR3(pos.x - sizeMax.x + ((sizeMax.x * 2.0f / 5.0f) * (rand() % 5)), pos.y + sizeMax.y + EFFECT_SIZE_Y, pos.z - sizeMax.z + ((sizeMax.z * 2.0f / 5.0f) * (rand() % 5))),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf((((D3DX_PI * 2.0f) / 15) * nCnt)) * 2.2f, 0.0f, cosf((((D3DX_PI * 2.0f) / 15) * nCnt)) * -2.2f + -2.0f),
									D3DXVECTOR3(0.1f, 0.1f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.01f, CEffect::EFFECTTYPE_SMOKE, CEffect::TEXTURE_SMOKE);		// �G�t�F�N�g
							}
						}
						bDelete = true;
					}
				}

				CObject3D::Setpos(pos);		// �ʒu�X�V
			}
		}
	}

	if (CameraposV.z - pos.z >= 0 || bDelete == true)
	{// �J�����͈͊O�ɏo����
		Uninit();					// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CObject3D ***
//=============================================================================
void CObject3D::Draw(void)
{
	CModel3D::Draw();						// �`�揈��
}