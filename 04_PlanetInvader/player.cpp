//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "player.h"			// �v���C���[
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "sound.h"			// �T�E���h
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����
#include "enemy.h"			// �G
#include "bullet.h"			// �e
#include "bomb.h"			// ���e
#include "effect.h"			// �G�t�F�N�g

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME			"data\\MODEL\\player.x"			// ���f��
#define MAX_LIFE			(1)								// �̗�
#define MOVE_SPEED_UP		(1.5f)							// �ړ����x�i�����j
#define MOVE_SPEED_DOWN		(0.3f)							// �ړ����x�i�����j

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			*CPlayer::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH					CPlayer::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER				CPlayer::m_pBuffMat		= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD						CPlayer::m_nNumMat		= NULL;		// �}�e���A�����̐�

//=============================================================================
// �R���X�g���N�^									(public)	*** CPlayer ***
//=============================================================================
CPlayer::CPlayer() : CModel3D(CScene::PRIORITY_3)
{
	m_nPlayerNum = 0;								// �v���C���[�ԍ�

	m_bmove = false;								// �ړ���
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_nCntTimer = 0;								// ���ԃJ�E���^
	m_frotDest = 0.0f;								// ��������
	m_nCntstate = 0;								// ��ԃJ�E���g
	m_state = PLAYERSTATE_RESPAWN;					// ���
	m_nLife = MAX_LIFE;								// �̗�

	m_nCntShot = 0;									// ���˃J�E���^
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CPlayer ***
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CPlayer::Load(void)
{
	if (m_pBuffMat == NULL && m_pMesh == NULL)
	{// NULL�̏ꍇ
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat];
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		for (DWORD tex = 0; tex < m_nNumMat; tex++)
		{// �J�E���g
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// �e�N�X�`�����g�p���Ă�����
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[tex])))
				{
					MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CPlayer::Unload(void)
{
	// �e�N�X�`���̔j��
	for (DWORD tex = 0; tex < m_nNumMat; tex++)
	{// �J�E���g
		if (m_pTexture[tex] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[tex]->Release();			// ���
			m_pTexture[tex] = NULL;				// NULL��
		}
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pBuffMat->Release();					// ���
		m_pBuffMat = NULL;						// NULL��
	}

	// �}�e���A���̊J��
	if (m_pMesh != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pMesh->Release();						// ���
		m_pMesh = NULL;							// NULL��
	}

	m_nNumMat = 0;				// ������
}

//=============================================================================
// �m�ۏ���											(public)	*** CPlayer ***
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum)
{
	CPlayer *pPlayer = NULL;				// �|�C���^

	if (pPlayer == NULL)
	{// NULL�̏ꍇ
		pPlayer = new CPlayer;				// �������m��
	}

	if (pPlayer != NULL)
	{// NULL�ȊO�̏ꍇ
		pPlayer->Init(pos, rot, nPlayerNum);							// ����������
		pPlayer->SetObjType(CScene::OBJTYPE_PLAYER);		// �I�u�W�F�N�g�^�C�v�ݒ�
	}

	return pPlayer;
}

//=============================================================================
// ����������										(public)	*** CPlayer ***
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerNum)
{
	CModel3D::SetInitAll(m_pTexture, m_pMesh, m_pBuffMat, m_nNumMat, pos, rot);			// �����l�ݒ�
	CModel3D::Init();																	// ����������
	
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// �J���������_�擾

	m_pos = pos - Camerapos;				// �J�����ړ�������
	
	m_nPlayerNum = nPlayerNum;				// �v���C���[�ԍ�
	m_bmove = false;						// �ړ���
	m_nCntstate = 0;						// ��ԃJ�E���g
	m_state = PLAYERSTATE_RESPAWN;			// ���
	m_nLife = MAX_LIFE;						// �̗�
	m_nCntShot = 0;							// ���˃J�E���^

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Uninit(void)
{
	CModel3D::Uninit();						// �I������
}

//=============================================================================
// �X�V����											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Update(void)
{
	CManager::MODE mode = CManager::GetMode();							// ���[�h�擾
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// �J���������_�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();		// ���̓f�o�C�X(�L�[�{�[�h)
	CInputXPad *pInputXPad = CManager::GetInputXPad();					// ���̓f�o�C�X(�L�[�{�[�h)
	CSound *pSound = CManager::GetSound();							// �T�E���h
	CInputDIPad *pInputDIPad = CManager::GetInputDIPad();				// ���̓f�o�C�X(DI�p�b�h)

	D3DXVECTOR3 pos = CModel3D::Getpos();		// �ʒu���擾
	D3DXVECTOR3 rot = CModel3D::Getrot();		// �������擾
	D3DXVECTOR3 sizeMax = CModel3D::Getsize(0);	// �傫�����擾�i�ő�j
	D3DXVECTOR3 sizeMin = CModel3D::Getsize(1);	// �傫�����擾�i�ŏ��j

	if (mode == CManager::MODE_TUTORIAL || mode == CManager::MODE_GAME)
	{
		// �J�����ɑ΂��Ă̈ʒu��ݒ�
		pos.x = Camerapos.x + m_pos.x;
		pos.z = Camerapos.z + m_pos.z;

		switch (m_state)
		{// ���
		case PLAYERSTATE_NORMAL:	// �ʏ�
			break;
		case PLAYERSTATE_DAMAGE:	// �_���[�W
			m_nCntstate++;			// ��ԃJ�E���g

			// �_��
			if (m_nCntstate / 3 % 2)
			{// ��
				CModel3D::Setcol(D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f));
			}
			else
			{// ��
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
			}

			if (m_nCntstate == 60)
			{// ���̃J�E���g�ɒB������ʏ��
				m_nCntstate = 0;
				m_state = PLAYERSTATE_NORMAL;
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			break;
		case PLAYERSTATE_RESPAWN:	// ����
			m_nCntstate++;			// ��ԃJ�E���g

			// �_��
			if (m_nCntstate / 10 % 2)
			{// �s����
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{// ����
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}

			if (m_nCntstate == 150)
			{// ���̃J�E���g�ɒB������ʏ��
				m_nCntstate = 0;
				
				m_state = PLAYERSTATE_NORMAL;
				CModel3D::Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			break;
		}
		if (m_bmove == false)
		{// �ړ��s��
			pos.z += 3.0f;			// �����ړ�

			if (Camerapos.z - 300.0f < pos.z)
			{// ���ʒu�Ŏ~�܂�
				m_bmove = true;		// �ړ���
			}
		}
		else
		{
		/*	{
				// �ړ�����
				if (pInputKeyboard->GetPress(DIK_W) == true)
				{// �C�ӂ̃L�[�iW�j���������Ƃ�
					if (pInputKeyboard->GetPress(DIK_A) == true)
					{// + �C�ӂ̃L�[�iA�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP);

						m_frotDest = -D3DX_PI * 0.25f;		// �ڕW����
					}
					else if (pInputKeyboard->GetPress(DIK_D) == true)
					{// + �C�ӂ̃L�[�iS�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.25f) * MOVE_SPEED_UP);

						m_frotDest = D3DX_PI * 0.25f;		// �ڕW����
					}
					else
					{// �C�ӂ̃L�[�iW�j�݂̂��������Ƃ�
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.0f) * MOVE_SPEED_UP);

						m_frotDest = D3DX_PI * 0.0f;		// �ڕW����
					}
				}
				else if (pInputKeyboard->GetPress(DIK_S) == true)
				{// �C�ӂ̃L�[�iS�j���������Ƃ�
					if (pInputKeyboard->GetPress(DIK_A) == true)
					{// + �C�ӂ̃L�[�iA�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.75f) * (MOVE_SPEED_UP * 0.8f));
					}
					else if (pInputKeyboard->GetPress(DIK_D) == true)
					{// + �C�ӂ̃L�[�iS�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.75f) * (MOVE_SPEED_UP * 0.8f));
					}
					else
					{// �C�ӂ̃L�[�iS�j�݂̂��������Ƃ�
						m_move += D3DXVECTOR3(sinf(D3DX_PI) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI) * (MOVE_SPEED_UP * 0.8f));
					}

					m_frotDest = D3DX_PI * 0.0f;			// �ڕW����
				}
				else if (pInputKeyboard->GetPress(DIK_A) == true)
				{// �C�ӂ̃L�[�iA�j���������Ƃ�
					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// + �C�ӂ̃L�[�iW�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.25f) * MOVE_SPEED_UP);
					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// + �C�ӂ̃L�[�iS�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.75f) * MOVE_SPEED_UP);
					}
					else
					{// �C�ӂ̃L�[�iA�j�݂̂��������Ƃ�
						m_move += D3DXVECTOR3(sinf(-D3DX_PI * 0.5f) * MOVE_SPEED_UP, 0.0f, cosf(-D3DX_PI * 0.5f) * MOVE_SPEED_UP);
					}

					m_frotDest = -D3DX_PI * 0.25f;			// �ڕW����
				}
				else if (pInputKeyboard->GetPress(DIK_D) == true)
				{// �C�ӂ̃L�[�iD�j���������Ƃ�
					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// + �C�ӂ̃L�[�iW�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.25f) * MOVE_SPEED_UP);
					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// + �C�ӂ̃L�[�iS�j���������Ƃ�
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.75f) * MOVE_SPEED_UP);
					}
					else
					{// �C�ӂ̃L�[�iD�j�݂̂��������Ƃ�
						m_move += D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * MOVE_SPEED_UP, 0.0f, cosf(D3DX_PI * 0.5f) * MOVE_SPEED_UP);
					}

					m_frotDest = D3DX_PI * 0.25f;			// �ڕW����
				}
				else
				{
					m_frotDest = D3DX_PI * 0.0f;			// �ڕW����
				}
			}*/

			if (pInputXPad->GetStick(0, m_nPlayerNum) == true)
			{
				m_move += D3DXVECTOR3(sinf(pInputXPad->GetStickRot(0, m_nPlayerNum)) * MOVE_SPEED_UP, 0.0f, cosf(pInputXPad->GetStickRot(0, m_nPlayerNum)) * MOVE_SPEED_UP);

				if (pInputXPad->GetStickRot(0, m_nPlayerNum) < -D3DX_PI * 0.75f ||
					D3DX_PI * 0.75f < pInputXPad->GetStickRot(0, m_nPlayerNum))
				{
					m_frotDest = D3DX_PI * 0.0f;		// �ڕW����
				}
				else  if (pInputXPad->GetStickRot(0, m_nPlayerNum) < -D3DX_PI * 0.25f)
				{
					m_frotDest = -D3DX_PI * 0.25f;		// �ڕW����
				}
				else if (D3DX_PI * 0.25f < pInputXPad->GetStickRot(0, m_nPlayerNum))
				{
					m_frotDest = D3DX_PI * 0.25f;		// �ڕW����
				}
				else
				{
					m_frotDest = D3DX_PI * 0.0f;		// �ڕW����
				}
			}
			else
			{
				m_frotDest = D3DX_PI * 0.0f;			// �ڕW����
			}

			// ���ˏ���
			//if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
			if (pInputXPad->GetPress(XINPUT_GAMEPAD_X, m_nPlayerNum) == true || pInputXPad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, m_nPlayerNum) == true)
			{// �C�ӂ̃L�[�i�X�y�[�X�j���������Ƃ�
				if (m_nCntShot % 15 == 0)
				{
					CBullet::Create(D3DXVECTOR3(pos.x - 12.0f, pos.y, pos.z + 10.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (CBullet::BULLETTYPE)m_nPlayerNum, CBullet::BULLETSTATE_NORMAL);
					CBullet::Create(D3DXVECTOR3(pos.x + 12.0f, pos.y, pos.z + 10.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (CBullet::BULLETTYPE)m_nPlayerNum, CBullet::BULLETSTATE_NORMAL);

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE003);
				}

				m_nCntShot++;
			}
			else
			{
				m_nCntShot = 0;
			}

			//else if (pInputKeyboard->GetTrigger(DIK_B) == true)
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_Y, m_nPlayerNum) == true || pInputXPad->GetPress(XINPUT_GAMEPAD_LEFT_SHOULDER, m_nPlayerNum) == true)
			{// �C�ӂ̃L�[�i�X�y�[�X�j���������Ƃ�
				if (CBomb::SetShot(m_nPlayerNum) == true)
				{
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z + 10.0f), D3DXVECTOR3(0.0f, 0.0f, 5.0f), D3DXVECTOR3(BULLET_SIZE_X * 1.8f, BULLET_SIZE_Y * 1.8f, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), (CBullet::BULLETTYPE)m_nPlayerNum, CBullet::BULLETSTATE_BOMB);
					CBomb::SetDelete(m_nPlayerNum, true);

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE004);
				}
			}
#ifdef _DEBUG
			if (pInputKeyboard->GetTrigger(DIK_N) == true)
			{// �C�ӂ̃L�[�i�X�y�[�X�j���������Ƃ�
				CBomb::SetAdd(m_nPlayerNum, true);
			}

			if (pInputKeyboard->GetTrigger(DIK_M) == true)
			{// �C�ӂ̃L�[�i�X�y�[�X�j���������Ƃ�
				CEnemy::Create(D3DXVECTOR3(-200.0f, 10.0f, Camerapos.z + 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.3f), CEnemy::ENEMYTYPE_G_1, 70);
			}
			if (pInputKeyboard->GetTrigger(DIK_COMMA) == true)
			{// �C�ӂ̃L�[�i�X�y�[�X�j���������Ƃ�
				CEnemy::Create(D3DXVECTOR3(0.0f, 300.0f, Camerapos.z + 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.5f), CEnemy::ENEMYTYPE_S_1, 60);
			}
			if (pInputKeyboard->GetTrigger(DIK_PERIOD) == true)
			{// �C�ӂ̃L�[�i�X�y�[�X�j���������Ƃ�
				CEnemy::Create(D3DXVECTOR3(300.0f, -100.0f, Camerapos.z + 900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.15f), CEnemy::ENEMYTYPE_O_1, 80);
			}
#endif
		}

		CEffect::Create(D3DXVECTOR3(pos.x, pos.y, pos.z - 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_PLAYER, CEffect::TEXTURE_EFFECT);		// �G�t�F�N�g

		// 6453 6773
		// 320
		// 642 352
		// 290
		// (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f)

		// 610(z all)
		// 120(x all)

		// �͈͊O�̏���
		if (pos.z + m_move.z < (Camerapos.z - 380.0f) - sizeMax.z)
		{
			m_move.z = 0.0f;
		}
		if ((Camerapos.z + 120.0f) + sizeMax.z < pos.z + m_move.z)
		{
			m_move.z = 0.0f;
		}
		if (pos.x + m_move.x < (Camerapos.x - 200.0f) - sizeMax.x)
		{
			m_move.x = 0.0f;
		}
		if ((Camerapos.x + 200.0f) + sizeMax.x < pos.x + m_move.x)
		{
			m_move.x = 0.0f;
		}
		//if (pos.x + m_move.x < (Camerapos.x - 180.0f - (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) + sizeMax.x)
		//{
		//	if (m_move.x < 0)
		//	{
		//		m_move.x = 0.0f;
		//	}
		//	pos.x = (Camerapos.x - 180.0f - (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) + sizeMax.x;
		//}
		//if ((Camerapos.x + 180.0f + (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) - sizeMax.x < pos.x + m_move.x)
		//{
		//	if (0 < m_move.x)
		//	{
		//		m_move.x = 0.0f;
		//	}
		//	pos.x = (Camerapos.x + 180.0f + (120.0f * ((pos.z - (Camerapos.z - 290.0f)) / 610.0f))) - sizeMax.x;
		//}

		pos += m_move;		// �ړ��ʑ��

		float fDiff = (m_frotDest - rot.z) / 8.0f;		// �����̍����v�Z
		rot.z += fDiff;		// �������

		// ��������
		m_move.x += (0.0f - m_move.x) * MOVE_SPEED_DOWN;
		m_move.z += (0.0f - m_move.z) * MOVE_SPEED_DOWN;

		CModel3D::Setpos(pos);					// �ʒu��ݒ�
		CModel3D::Setrot(rot);					// ������ݒ�

		m_pos = pos - Camerapos;				// ���݈ʒu���v�Z
	}
}

//=============================================================================
// �`�揈��											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Draw(void)
{
	CModel3D::Draw();						// �`�揈��
}