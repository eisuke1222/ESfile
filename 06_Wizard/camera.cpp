//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "camera.h"			// �J����
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "sound.h"			// �T�E���h
#include "fade.h"			// �t�F�[�h
#include "texture.h"		// �e�N�X�`��
#include "title.h"			// �^�C�g��
#include "game.h"			// �Q�[��
#include "scenemesh.h"		// �V�[�����b�V��
#include "map.h"			// �}�b�v
#include "ui.h"				// Ui
#include "player.h"			// �v���C���[
#include "enemy.h"			// �G

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MC_DRAW_MIN		(10.0f)								// �`��ŒZ
#define MC_DRAW_MAX		(10000.0f)							// �`��Œ�
#define MC_MOVE_POS		(5.0f)								// �ړ��i�ʒu�j
#define MC_MOVE_ROT		(0.02f)								// �ړ��i��]�j
#define MC_SET_RATIO	(0.25f)								// �ݒ芄��
#define MC_XPAD_STICK	(0.0000014f)						// XPad�X�e�B�b�N
#define MC_MOUSE_X		(0.0005625f)						// �}�E�XX
#define MC_MOUSE_Y		(0.001f)							// �}�E�XY
#define MC_ROT_X_MIN	(-1.0f)								// ��]X�ŏ�
#define MC_ROT_X_MAX	(1.0f)								// ��]X�ő�
#define MC_ROCKON_RANGE	(600.0f)							// ���b�N�I���͈�
#define MC_ROCKON_FREE	(D3DX_PI * 0.1f)					// ���b�N�I�����̂�����

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
// �R���X�g���N�^										(public)	*** CCamera ***
//=============================================================================
CCamera::CCamera()
{
	// �����l�ݒ�
	m_posVInit			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �������_
	m_posV				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���ݎ��_
	m_posVDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW���_
	m_posRInit			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���������_
	m_posR				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���ݒ����_
	m_posRDest			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW�����_
	m_vecU				= D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ������x�N�g��
	m_rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �p�x
	m_fDistance			= 0.0f;								// ����

	m_viewport.X		= 0;								// X�J�n�ʒu
	m_viewport.Y		= 0;								// Y�J�n�ʒu
	m_viewport.Width	= SCREEN_WIDTH;						// X�J�n�ʒu����̑傫��
	m_viewport.Height	= SCREEN_HEIGHT;					// Y�J�n�ʒu����̑傫��
	m_viewport.MinZ		= 0.0f;								// �ŏ�
	m_viewport.MaxZ		= 1.0f;								// �ő�
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CCamera ***
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ����������										(public)	*** CCamera ***
//=============================================================================
HRESULT CCamera::Init(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CCamera ***
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �J�����ݒ菈��										(public)	*** CCamera ***
//=============================================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾

	// �r���[�|�[�g�ݒ�
	pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f), (float)m_viewport.Width / (float)m_viewport.Height, MC_DRAW_MIN, MC_DRAW_MAX);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// �r���[�|�[�g����									(public)	*** CCamera ***
//=============================================================================
void CCamera::SetViewPort(DWORD X, DWORD Y, DWORD Width, DWORD Height)
{
	m_viewport.X		= X;		// �ʒuX
	m_viewport.Y		= Y;		// �ʒuY
	m_viewport.Width	= Width;	// �T�C�YX
	m_viewport.Height	= Height;	// �T�C�YY
	m_viewport.MinZ		= 0.0f;		// �ŏ�
	m_viewport.MaxZ		= 1.0f;		// �ő�
}

//=============================================================================
// �f�o�b�O����										(protected)	*** CCamera ***
//=============================================================================
void CCamera::Debug(void)
{
	// �ړ�����
	if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
	{// (I)�L�[�������ꂽ
	 // �ړ��i�O�j
		m_posVDest.z += MC_MOVE_POS;
		m_posRDest.z += MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
	{// �iK�j�L�[�������ꂽ
	 // �ړ��i���j
		m_posVDest.z -= MC_MOVE_POS;
		m_posRDest.z -= MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
	{// �iJ�j�L�[�������ꂽ
	 // �ړ��i���j
		m_posVDest.x -= MC_MOVE_POS;
		m_posRDest.x -= MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_L) == true)
	{// �iL�j�L�[�������ꂽ
	 // �ړ��i�E�j
		m_posVDest.x += MC_MOVE_POS;
		m_posRDest.x += MC_MOVE_POS;
	}

	// ���_��]
	if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{// �iQ�j�L�[�������ꂽ
	 // �E��]
		m_rot.y -= MC_MOVE_ROT;
		m_posVDest.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{// �iE�j�L�[�������ꂽ
	 // ����]
		m_rot.y += MC_MOVE_ROT;
		m_posVDest.x = m_posR.x + sinf(m_rot.y) * m_fDistance;
		m_posVDest.z = m_posR.z + cosf(m_rot.y) * m_fDistance;
	}

	// ���_�㉺��]
	if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
	{// �iT�j�L�[�������ꂽ
	 // ��
		m_posVDest.y += MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
	{// �iG�j�L�[�������ꂽ
	 // ��
		m_posVDest.y -= MC_MOVE_POS;
	}

	// �����_�㉺��]
	if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
	{// �iY�j�L�[�������ꂽ
	 // ��
		m_posRDest.y += MC_MOVE_POS;
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_H) == true)
	{// �iH�j�L�[�������ꂽ
	 // ��
		m_posRDest.y -= MC_MOVE_POS;
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CCameraMain ***
//=============================================================================
CCameraMain::CCameraMain() : CCamera()
{
	// �����l�ݒ�
	m_bPickUp			= false;	// �s�b�N�A�b�v
	m_nCntPickUp		= 0;		// �s�b�N�A�b�v�J�E���g
	m_bGameEnd			= false;	// �Q�[���G���h
	m_nCntGameEnd		= 0;		// �Q�[���G���h�J�E���g
	m_bMove				= false;	// �ړ��ł���
	m_bMoveCollision	= true;		// �ړ��ł���
	m_pLockOn			= NULL;		// ���b�N�I���G
	m_bLockOn			= false;	// ���b�N�I��

	m_pLockOnL			= NULL;		// ���b�N�I���GL
	m_pLockOnR			= NULL;		// ���b�N�I���GR

	// ���b�N�I���}�[�J�[
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
		m_pLockOnMarker[nCount] = NULL;
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CCameraMain ***
//=============================================================================
CCameraMain::~CCameraMain()
{

}

//=============================================================================
// ����������									(public)	*** CCameraMain ***
//=============================================================================
HRESULT CCameraMain::Init(void)
{
	CCamera::Init();		// ����������

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Uninit(void)
{
	m_bPickUp		= false;			// �s�b�N�A�b�v
	m_nCntPickUp	= 0;				// �s�b�N�A�b�v�J�E���g
	m_bGameEnd		= false;			// �Q�[���G���h
	m_nCntGameEnd	= 0;				// �Q�[���G���h�J�E���g

	m_pLockOn		= NULL;				// ���b�N�I���G

	m_pLockOnL		= NULL;				// ���b�N�I���GL
	m_pLockOnR		= NULL;				// ���b�N�I���GR

	// ���b�N�I���}�[�J�[
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
	{// ���b�N�I���}�[�J�[�J�E���g
		if (m_pLockOnMarker[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pLockOnMarker[nCount] = NULL;
		}
	}

	CCamera::Uninit();		// �I������
}

//=============================================================================
// �X�V����										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Update(void)
{
#ifdef _DEBUG	// �f�o�b�O�̂�
	//Debug();	// �f�o�b�O����
#endif // _DEBUG

	if (m_bMove == true)
	{// �ړ��ł���
		if (m_bGameEnd == false)
		{// �Q�[���G���h�ȊO
			if (m_bPickUp == true)
			{// �s�b�N�A�b�v��������
				PickUp();			// �s�b�N�A�b�v
			}
			else
			{// �s�b�N�A�b�v�ȊO
				Following();		// �Ǐ]
				LockOn();			// ���b�N�I��

				if (m_bMoveCollision == true)
				{// �ړ��R���W����������
					Move();					// �ړ�
					ColisionChack();		// ����`�F�b�N
				}
			}
		}
		else
		{// �Q�[�����G���h��������
			GameEnd();		// �Q�[���G���h
		}
	}

	// �p�x����
	if (D3DX_PI < m_rot.x)
		m_rot.x -= (D3DX_PI * 2);
	if (m_rot.x < -D3DX_PI)
		m_rot.x += (D3DX_PI * 2);
	if (D3DX_PI < m_rot.y)
		m_rot.y -= (D3DX_PI * 2);
	if (m_rot.y < -D3DX_PI)
		m_rot.y += (D3DX_PI * 2);
	if (D3DX_PI < m_rot.z)
		m_rot.z -= (D3DX_PI * 2);
	if (m_rot.z < -D3DX_PI)
		m_rot.z += (D3DX_PI * 2);

	// �l���
	m_posR.x += (m_posRDest.x - m_posR.x) * MC_SET_RATIO;
	m_posR.y += (m_posRDest.y - m_posR.y) * MC_SET_RATIO;
	m_posR.z += (m_posRDest.z - m_posR.z) * MC_SET_RATIO;
	m_posV.x += (m_posVDest.x - m_posV.x) * MC_SET_RATIO;
	m_posV.y += (m_posVDest.y - m_posV.y) * MC_SET_RATIO;
	m_posV.z += (m_posVDest.z - m_posV.z) * MC_SET_RATIO;

	// �t�H���g�擾
	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_DATA);

	// �l�ݒ�
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "Camera : ���_�@[%.2f, %.2f, %.2f]\n", m_posV.x, m_posV.y, m_posV.z);
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "Camera : �����_[%.2f, %.2f, %.2f]\n", m_posR.x, m_posR.y, m_posR.z);
	CFont::SetMessageData(CScene::OBJTYPE_CAMERA, CFont::SETTYPE_ADD, "\n");
}

//=============================================================================
// �J�����ݒ菈��									(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::SetCamera(void)
{
	CCamera::SetCamera();		// �J�����ݒ�
}

//=============================================================================
// �s�b�N�A�b�v����								(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::PickUp(void)
{
	CSound *pSound = CManager::GetSound();		// �T�E���h�擾

	// ���b�N�I���}�[�J�[
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
	{// ���b�N�I���}�[�J�[�J�E���g
		if (m_pLockOnMarker[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pLockOnMarker[nCount]->Uninit();		// �I������
			m_pLockOnMarker[nCount] = NULL;			// NULL��
		}
	}

	CScene	*pScene		= NULL;		// �V�[��
	CScene	*pSceneNext = NULL;		// ���V�[��
	CEnemy	*pEnemy		= NULL;		// �G

	pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

	while (pScene != NULL)
	{// NULL�ȊO�̏ꍇ
		pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

		if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
		{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
			pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g

			break;
		}
		pScene = pSceneNext;	// ���V�[����
	}

	if (pEnemy != NULL)
	{// NULL�ȊO�̏ꍇ
		D3DXVECTOR3 pos = pEnemy->Getpos();		// �G�̈ʒu�擾

		// �����Ɗp�x�擾
		float fLenght	= sqrtf(powf(pos.x - m_posR.x, 2.0f) + powf(pos.z - m_posR.z, 2.0f));
		float fAngle	= atan2f(pos.x - m_posR.x, pos.z - m_posR.z);

		if (m_nCntPickUp < 90)
		{// ���t���[���܂�
			// ���i
			m_posRDest.x += sinf(fAngle) * (fLenght / (90 - m_nCntPickUp));
			m_posRDest.z += cosf(fAngle) * (fLenght / (90 - m_nCntPickUp));

			m_rot.x += ((0.06f - m_rot.x) / (90 - m_nCntPickUp));
		}
		else if (m_nCntPickUp == 90)
		{// ���t���[���ɂȂ�����
			// �p�x�ؑ�
			m_rot.y = D3DX_PI + (D3DX_PI * 2.0f * 0.3f);

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DON);
			}
		}
		else if (m_nCntPickUp == 120)
		{// ���t���[���ɂȂ�����
			// �p�x�ؑ�
			m_rot.y = -D3DX_PI + -(D3DX_PI * 2.0f * 0.3f);

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DON);
			}
		}
		else if (m_nCntPickUp == 150)
		{// ���t���[���ɂȂ�����
			// �p�x�ؑ�
			m_rot.y = D3DX_PI;

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DON);
			}
		}
		else if (m_nCntPickUp == 180)
		{// ���t���[���ɂȂ�����
			// ����
			m_bPickUp = false;
			m_nCntPickUp = 0;
			return;
		}

		// ���_�v�Z�p
		D3DXMATRIX mat;						// ��]
		D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
		D3DXVECTOR3	vout;					// �v�Z����

		// ���_�v�Z
		D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
		D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
		m_posVDest = vout + m_posRDest;		// �����_����̈ʒu

		m_nCntPickUp++;
	}
}

//=============================================================================
// �Q�[���G���h����								(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::GameEnd(void)
{
	m_pLockOn	= NULL;					// ���b�N�I���G

	m_pLockOnL	= NULL;					// ���b�N�I���GL
	m_pLockOnR	= NULL;					// ���b�N�I���GR

	// ���b�N�I���}�[�J�[
	for (int nCount = 0; nCount < ROCKON_MARKER_MAX; nCount++)
	{// ���b�N�I���}�[�J�[�J�E���g
		if (m_pLockOnMarker[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pLockOnMarker[nCount]->Uninit();		// �I������
			m_pLockOnMarker[nCount] = NULL;			// NULL��
		}
	}

	CGame *pGame = CManager::GetGame();		// �Q�[���擾
	if (pGame != NULL)
	{// NULL�ȊO�̏ꍇ
		CPlayer	*pPlayer = pGame->GetPlayer();		// �v���C���[�擾
		if (pPlayer != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_nCntGameEnd < 165)
			{// ���t���[���܂�
				// ����
				m_posRDest = pPlayer->Getpos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f);
				m_rot.x = -0.5f;
				m_rot.y = pPlayer->Getrot().y;
				m_fDistance = 100.0f;

				// ���_�v�Z�p
				D3DXMATRIX mat;						// ��]
				D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
				D3DXVECTOR3	vout;					// �v�Z����

				// ���_�v�Z
				D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
				D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
				m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
				m_posV = m_posVDest;
				m_posR = m_posRDest;
			}
			else
			{// ���̑�
				// �ォ�猩���낷�{��]
				if (m_nCntGameEnd == 165)
					pGame->SetSS_Death(true);		// �X�N�V��

				// �����_�X�V
				m_posRDest = pPlayer->Getpos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f);

				// �㏸
				if (-1.2f < m_rot.x)
					m_rot.x -= 0.004f;

				// ��]
				m_rot.y += 0.004f;

				// ����
				if (m_fDistance < 250.0f)
					m_fDistance += 0.5f;

				// ���_�v�Z�p
				D3DXMATRIX mat;						// ��]
				D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
				D3DXVECTOR3	vout;					// �v�Z����

				// ���_�v�Z
				D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
				D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
				m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
			}

			m_nCntGameEnd++;
		}
	}
}

//=============================================================================
// �Ǐ]����										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Following(void)
{
	CManager::MODE	mode = CManager::GetMode();		// ���[�h�擾
	CPlayer	*pPlayer = NULL;
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();	// �L�[�{�[�h�擾
	CInputMouse		*pMouse = CManager::GetInputMouse();	// �}�E�X�擾
	CInputXPad		*pXPad = CManager::GetInputXPad();		// X�p�b�h�擾

	if (mode == CManager::MODE_TITLE)
		pPlayer = CManager::GetTitle()->GetPlayer();// �v���C���[�擾
	else if (mode == CManager::MODE_GAME)
		pPlayer = CManager::GetGame()->GetPlayer();	// �v���C���[�擾

	// �ǔ��ݒ�
	if (pPlayer != NULL)
	{// NULL�ȊO�̏ꍇ
		if (m_bLockOn == true)
		{// ���b�N�I�����Ă���
			if (m_pLockOn != NULL)
			{// NULL�ȊO�̏ꍇ
				D3DXVECTOR3 posEnemy = m_pLockOn->Getpos();		// �ʒu�擾�i���b�N�I���j
				float		fLenght = 0.0f;						// �����v�Z�p

				// �����v�Z
				fLenght = sqrtf(powf(posEnemy.x - pPlayer->Getpos().x, 2) + powf(posEnemy.z - pPlayer->Getpos().z, 2));

				if (fLenght < MC_ROCKON_RANGE)
				{// ���b�N�I���͈�
					//if (10.0f < fLenght)
					{
						// ��]�ݒ�
						m_rot.x = -0.3f;// D3DX_PI + atan2f(fLenght * 0.25f, posEnemy.y - m_posVDest.y + (pPlayer->Getpos().y * 0.6f));

						D3DXVECTOR3 posPlayer = pPlayer->Getpos();		// �ʒu�擾
						m_fLockOn = (D3DX_PI + atan2f(posEnemy.x - posPlayer.x, posEnemy.z - posPlayer.z));		// ��]�v�Z
						float fDiff = m_rot.y - m_fLockOn;				// �����v�Z

						// ��]����
						if (fDiff < -D3DX_PI)
							fDiff += D3DX_PI * 2.0f;
						if (D3DX_PI < fDiff)
							fDiff -= D3DX_PI * 2.0f;

						// �J����������
						if (fDiff < -MC_ROCKON_FREE)
							m_rot.y -= fDiff + MC_ROCKON_FREE;
						if (MC_ROCKON_FREE < fDiff)
							m_rot.y -= fDiff - MC_ROCKON_FREE;
					}
				}
				else
				{// ���b�N�I���͈͊O
					m_pLockOn = NULL;		// NULL��
					m_bLockOn = false;		// ���b�N�I�����Ă��Ȃ�

					m_pLockOnL = NULL;
					m_pLockOnR = NULL;
				}
			}
			else
			{// ���b�N�I���͈͊O
				m_pLockOn = NULL;		// NULL��
				m_bLockOn = false;		// ���b�N�I�����Ă��Ȃ�

				m_pLockOnL = NULL;
				m_pLockOnR = NULL;
			}
		}

		// X�p�b�h
		{
			if (pXPad != NULL)
			{// NULL�ȊO�̏ꍇ
				if (pXPad->GetTrigger(CInputXPad::XPADOTHER_TRIGGER_LEFT, 0) == true)
				{
					m_rot.y = pPlayer->Getrot().y + D3DX_PI;

					m_pLockOn = NULL;		// NULL��
					m_bLockOn = false;		// ���b�N�I�����Ă��Ȃ�

					m_pLockOnL = NULL;
					m_pLockOnR = NULL;
				}
			}
		}

		// �L�[�{�[�h
		{
			if (pKey != NULL)
			{// NULL�ȊO�̏ꍇ
				if (pKey->GetTrigger(DIK_C) == true)
				{
					m_rot.y = pPlayer->Getrot().y + D3DX_PI;

					m_pLockOn = NULL;		// NULL��
					m_bLockOn = false;		// ���b�N�I�����Ă��Ȃ�

					m_pLockOnL = NULL;
					m_pLockOnR = NULL;
				}
			}
		}

		// �����_�X�V
		m_posRDest = D3DXVECTOR3(pPlayer->Getpos().x, 50.0f, pPlayer->Getpos().z);

		// ���_�v�Z�p
		D3DXMATRIX mat;						// ��]
		D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
		D3DXVECTOR3	vout;					// �v�Z����

		// ���_�v�Z
		D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
		D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
		m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
	}
}

//=============================================================================
// �ړ�����										(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::Move(void)
{
	CInputKeyboard	*pKey	= CManager::GetInputKeyboard();	// �L�[�{�[�h�擾
	CInputMouse		*pMouse = CManager::GetInputMouse();	// �}�E�X�擾
	CInputXPad		*pXPad	= CManager::GetInputXPad();		// X�p�b�h�擾
	bool			bInput	= false;;						// ���͂�����

	// X�p�b�h
	{
		if (bInput == false)
		{// ���͂��Ă��Ȃ�
			if (pXPad != NULL)
			{// NULL�ȊO�̏ꍇ
				if (pXPad->GetStick(1, 0) == true)
				{// X�p�b�h�i�E�X�e�B�b�N�j����͂���
					if (m_bLockOn == false)
					{// ���b�N�I�����Ă��Ȃ�						
						// X��]
						if (MC_ROT_X_MIN < m_rot.x + pXPad->GetStickMove(1, 0).y * MC_XPAD_STICK &&
							m_rot.x + pXPad->GetStickMove(1, 0).y * MC_XPAD_STICK < MC_ROT_X_MAX)
						{// ��]����
							m_rot.x += pXPad->GetStickMove(1, 0).y * MC_XPAD_STICK;
						}

						// Y��]
						m_rot.y += pXPad->GetStickMove(1, 0).x * MC_XPAD_STICK;

						bInput = true;		// ���͂���
					}
					else
					{// ���b�N�I�����Ă���
						if (pXPad->GetStickNum(1, 0).y < CInputXPad::m_STICKMIN * 0.6f || CInputXPad::m_STICKMAX * 0.6f < pXPad->GetStickNum(1, 0).y)
						{// �E�X�e�B�b�NY���ȏ�Ń��b�N�I������
							m_pLockOn = NULL;		// NULL��
							m_bLockOn = false;		// ���b�N�I�����Ă��Ȃ�

							m_pLockOnL = NULL;
							m_pLockOnR = NULL;
						}

						if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_R_LEFT, 0) == true)
						{// �E�X�e�B�b�NY���ȏ�Ń��b�N�I������
							if (m_pLockOnL != NULL)
							{
								m_pLockOn = m_pLockOnL;
								m_pLockOnL = NULL;
							}
						}
						else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_R_RIGHT, 0) == true)
						{// �E�X�e�B�b�NY���ȏ�Ń��b�N�I������
							if (m_pLockOnR != NULL)
							{
								m_pLockOn = m_pLockOnR;
								m_pLockOnR = NULL;
							}
						}
					}
				}
			}
		}
	}

	// �}�E�X
	{
		if (bInput == false)
		{// ���͂��Ă��Ȃ�
			if (pMouse != NULL)
			{// NULL�ȊO�̏ꍇ
				if (m_bLockOn == false)
				{// ���b�N�I�����Ă��Ȃ�
					// X��]
					if (MC_ROT_X_MIN < m_rot.x - pMouse->GetPointer().y * MC_MOUSE_X &&
						m_rot.x - pMouse->GetPointer().y * MC_MOUSE_X < MC_ROT_X_MAX)
					{// ��]����
						m_rot.x -= pMouse->GetPointer().y * MC_MOUSE_X;
					}

					// Y��]
					m_rot.y += pMouse->GetPointer().x * MC_MOUSE_Y;

					bInput = true;		// ���͂���
				}
				else
				{// ���b�N�I�����Ă���
					if (pMouse->GetPointer().y < -200.0f || 200.0f < pMouse->GetPointer().y)
					{// �E�X�e�B�b�NY���ȏ�Ń��b�N�I������
						m_pLockOn = NULL;		// NULL��
						m_bLockOn = false;		// ���b�N�I�����Ă��Ȃ�

						m_pLockOnL = NULL;
						m_pLockOnR = NULL;
					}

					if (pMouse->GetPointer().x < -200.0f)
					{// �E�X�e�B�b�NY���ȏ�Ń��b�N�I������
						if (m_pLockOnL != NULL)
						{
							m_pLockOn = m_pLockOnL;
							m_pLockOnL = NULL;
						}
					}
					else if (200.0f < pMouse->GetPointer().x)
					{// �E�X�e�B�b�NY���ȏ�Ń��b�N�I������
						if (m_pLockOnR != NULL)
						{
							m_pLockOn = m_pLockOnR;
							m_pLockOnR = NULL;
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// ���b�N�I������									(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::LockOn(void)
{
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();	// �L�[�{�[�h�擾
	CInputMouse		*pMouse = CManager::GetInputMouse();	// �}�E�X�擾
	CInputXPad		*pXPad = CManager::GetInputXPad();		// X�p�b�h�擾
	bool			bInput = false;;						// ���͂�����
	CFade::FADE		fade = CFade::GetFade();					// �t�F�[�h�擾

	CManager::MODE	mode = CManager::GetMode();		// ���[�h�擾
	CPlayer	*pPlayer = NULL;

	if (mode == CManager::MODE_TITLE)
		pPlayer = CManager::GetTitle()->GetPlayer();// �v���C���[�擾
	else if (mode == CManager::MODE_GAME)
		pPlayer = CManager::GetGame()->GetPlayer();	// �v���C���[�擾

	CScene	*pScene			= NULL;		// �V�[��
	CScene	*pSceneNext		= NULL;		// ���V�[��
	CEnemy	*pLockOnEnemy	= NULL;		// �G
	D3DXVECTOR3	posLockOnEnemy;			// �G�ʒu
	float fLockOnLenght = 5000.0f;		// ���b�N�I���\�ȓG�̍ŒZ����
	bool bRangeInEnemy	= false;		// ���b�N�I���͈̔͂ɓG�����邩

	// �p�x�v�Z
	float fLockOnDiffL = 5000.0f;
	float fLockOnDiffR = 5000.0f;

	if (m_bLockOn == false)
	{// ���b�N�I�����Ă��Ȃ�
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
				CEnemy		*pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
				D3DXVECTOR3 posEnemy = pEnemy->Getpos();		// �ʒu�擾�i�G�j
				float		fLenght = 0.0f;					// �����v�Z�p

				// �����v�Z
				fLenght = sqrtf(powf(posEnemy.x - pPlayer->Getpos().x, 2) + powf(posEnemy.z - pPlayer->Getpos().z, 2));

				if (fLenght < MC_ROCKON_RANGE)
				{// ��苗���ȉ���������
					if (fLenght < fLockOnLenght)
					{
						fLockOnLenght = fLenght;		// ������ۊ�
						pLockOnEnemy = NULL;			// NULL��
						pLockOnEnemy = pEnemy;		// ���b�N�I������G��ݒ�
						posLockOnEnemy = posEnemy;		// ���b�N�I������G�Ƃ̋�����ۑ�
						bRangeInEnemy = true;			// ���b�N�I������G������
					}
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}
	else
	{// ���b�N�I�����Ă���
		if (m_pLockOn != NULL)
		{// NULL�ȊO�̏ꍇ
			posLockOnEnemy = m_pLockOn->Getpos();	// �ʒu�擾

			if (m_pLockOnMarker[ROCKON_MARKER_FRAME] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pLockOnMarker[ROCKON_MARKER_FRAME]->Setpos((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f);
			}
			if (m_pLockOnMarker[ROCKON_MARKER_PLAY] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->Setpos((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f);
			}

			CManager::MODE mode = CManager::GetMode();		// ���[�h�擾
			if (mode == CManager::MODE_GAME)
			{// ���[�h���Q�[����������
				CUi_Game *pUi_Game = (CUi_Game*)CManager::GetGame()->GetUi();
				if (pUi_Game != NULL)
					pUi_Game->SetGauge(CUi_Game::GAUGE_ENEMY_HP, m_pLockOn->GetHP(), m_pLockOn->GetMAXHP());		// �GHP�X�V
			}

			pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾
			float fLockOnAngle = atan2f(posLockOnEnemy.x - pPlayer->Getpos().x, posLockOnEnemy.z - pPlayer->Getpos().z);

			while (pScene != NULL)
			{// NULL�ȊO�̏ꍇ
				pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

				if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
				{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
					if (pScene != m_pLockOn)
					{
						CEnemy		*pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
						D3DXVECTOR3 posEnemy = pEnemy->Getpos();	// �ʒu�擾�i�G�j
						float		fLenght = 0.0f;					// �����v�Z�p

						// �����v�Z
						fLenght = sqrtf(powf(posEnemy.x - pPlayer->Getpos().x, 2) + powf(posEnemy.z - pPlayer->Getpos().z, 2));

						if (fLenght < MC_ROCKON_RANGE)
						{// ��苗���ȉ���������
							D3DXVECTOR3 vecA, vecC;
							vecA = posLockOnEnemy - pPlayer->Getpos();
							vecC = pEnemy->Getpos() - pPlayer->Getpos();

							if (((vecA.z * vecC.x) - (vecA.x * vecC.z)) <= 0)
							{// ����
								float fDiff = atan2f(posEnemy.x - pPlayer->Getpos().x, posEnemy.z - pPlayer->Getpos().z);
								fDiff = fDiff - fLockOnAngle;
								fDiff = fabsf(fDiff);

								if (fDiff < fLockOnDiffL)
								{// ���߂�
									fLockOnDiffL = fDiff;
									m_pLockOnL = NULL;
									m_pLockOnL = pEnemy;
								}
							}
							else
							{// �E��
								float fDiff = atan2f(posEnemy.x - pPlayer->Getpos().x, posEnemy.z - pPlayer->Getpos().z);
								fDiff = fDiff - fLockOnAngle;
								fDiff = fabsf(fDiff);

								if (fDiff < fLockOnDiffR)
								{// ���߂�
									fLockOnDiffR = fDiff;
									m_pLockOnR = NULL;
									m_pLockOnR = pEnemy;
								}
							}
						}
					}
				}
				pScene = pSceneNext;	// ���V�[����
			}
		}
	}

	if (bRangeInEnemy == true)
	{// �͈͓��ɓG������
		if (m_pLockOnMarker[ROCKON_MARKER_FRAME] == NULL)
		{// NULL�̏ꍇ
			m_pLockOnMarker[ROCKON_MARKER_FRAME] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_ROCKON);
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->SetInitAll((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->Init();
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_CAMERA_ROCKON_FRAME));
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->DrawEdit(false, false, true, true, true, 0, 3, D3DCMP_ALWAYS);
		}
		else
		{// NULL�ȊO�̏ꍇ
			m_pLockOnMarker[ROCKON_MARKER_FRAME]->Setpos((posLockOnEnemy + D3DXVECTOR3(0.0f, 10.0f, 0.0f)) + D3DXVECTOR3(sinf(m_rot.y), 0.0f, cosf(m_rot.y)) * 10.0f);
		}

		if (m_pLockOnMarker[ROCKON_MARKER_PLAY] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pLockOnMarker[ROCKON_MARKER_PLAY]->Uninit();		// �I������
			m_pLockOnMarker[ROCKON_MARKER_PLAY] = NULL;			// NULL��
		}

		CManager::MODE mode = CManager::GetMode();		// ���[�h�擾
		if (mode == CManager::MODE_GAME)
		{// ���[�h���Q�[����������
			CUi_Game *pUi_Game = (CUi_Game*)CManager::GetGame()->GetUi();
			if (pUi_Game != NULL)
				pUi_Game->SetGauge(CUi_Game::GAUGE_ENEMY_HP, pLockOnEnemy->GetHP(), pLockOnEnemy->GetMAXHP());		// �GHP�X�V

			CUi_Game::SetEnemyName(pLockOnEnemy->GetType() + 1);		// �G���O�X�V
		}
	}
	else
	{// �͈͓��ɓG�����Ȃ�
		if (m_bLockOn == true)
		{// ���b�N�I�����Ă���
			if (m_pLockOnMarker[ROCKON_MARKER_PLAY] == NULL)
			{// NULL�̏ꍇ
				m_pLockOnMarker[ROCKON_MARKER_PLAY] = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_ROCKON);
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->SetInitAll(m_pLockOnMarker[ROCKON_MARKER_FRAME]->Getpos(),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->Init();
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_CAMERA_ROCKON_PLAY));
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->DrawEdit(false, false, true, true, true, 0, 3, D3DCMP_ALWAYS);
			}
		}
		else
		{// ���b�N�I�����Ă��Ȃ�
			if (m_pLockOnMarker[ROCKON_MARKER_FRAME] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pLockOnMarker[ROCKON_MARKER_FRAME]->Uninit();		// �I������
				m_pLockOnMarker[ROCKON_MARKER_FRAME] = NULL;		// NULL��
			}
			if (m_pLockOnMarker[ROCKON_MARKER_PLAY] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pLockOnMarker[ROCKON_MARKER_PLAY]->Uninit();		// �I������
				m_pLockOnMarker[ROCKON_MARKER_PLAY] = NULL;			// NULL��
			}

			CUi_Game::SetEnemyName(0);
		}
	}

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if ((pKey != NULL && (pKey->GetTrigger(DIK_LCONTROL) == true || pKey->GetTrigger(DIK_Q) == true)) ||
			(pMouse != NULL && pMouse->GetTrigger(4) == true) || 
			(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_LEFT_SHOULDER, 0) == true))
		{// NULL�ȊO�̏ꍇ�A�L�[�{�[�h�i��CTRL�j�AX�p�b�h�iL1�j�������ꂽ
			if (pPlayer != NULL)
			{// NULL�ȊO�̏ꍇ
				// ���b�N�I���؂�ւ�
				m_bLockOn = m_bLockOn ? false : true;

				if (m_bLockOn == true)
				{// ���b�N�I�����Ă���
					if (bRangeInEnemy == true)
					{// �͈͓��ɓG������
						D3DXVECTOR3 posPlayer = pPlayer->Getpos();		// �ʒu�擾

						m_rot.x = -0.3f;// D3DX_PI + atan2f(fLenght * 0.25f, posEnemy.y - m_posVDest.y);
						m_rot.y = D3DX_PI + atan2f(posLockOnEnemy.x - posPlayer.x, posLockOnEnemy.z - posPlayer.z);

						m_pLockOn = NULL;		// NULL��
						m_pLockOn = pLockOnEnemy;		// ���b�N�I�������ݒ�

						m_pLockOnL = NULL;
						m_pLockOnR = NULL;

						m_fLockOn = (D3DX_PI + atan2f(posLockOnEnemy.x - posPlayer.x, posLockOnEnemy.z - posPlayer.z));		// ��]�v�Z
						pPlayer->SetrotDest(m_fLockOn + D3DX_PI);
						CUi_Game::SetEnemyName(pLockOnEnemy->GetType() + 1);
					}
					else
					{
						m_bLockOn = false;
					}
				}
			}
		}
	}
}

//=============================================================================
// ����`�F�b�N����								(public)	*** CCameraMain ***
//=============================================================================
void CCameraMain::ColisionChack(void)
{
	bool bArea = false;		// �G���A����

	// �}�b�v
	{
		CManager::MODE mode = CManager::GetMode();
		if (mode == CManager::MODE_GAME)
		{// �Q�[���̂�
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{
				CMap *pMap = pGame->GetMap();
				if (pMap != NULL)
				{
					int nMapNum = pGame->GetMapNum();
					if (nMapNum == 2)
					{
						CMap_02 *pMap_02 = (CMap_02*)pMap;

						if (pMap_02->CollisionCamera(m_posVDest, m_posRDest) == false)
						{
							if (pMap_02->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_02->CollisionCamera(m_posVDest, m_posRDest) == false)
								{// ���ȉ��������烋�[�v
									m_fDistance -= 1.0f;		// �����𒲐�

									// ���_�v�Z�p
									D3DXMATRIX mat;						// ��]
									D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
									D3DXVECTOR3	vout;					// �v�Z����

									// ���_�v�Z
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
									D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
									m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
								}
								bArea = true;	// �G���A����
							}
						}
						else
						{
							if (pMap_02->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_02->CollisionCamera(m_posVDest, m_posRDest) == true)
								{// ���ȏ�ȏゾ�����烋�[�v
									m_fDistance += 1.0f;	// �����𒲐�

									if (m_fDistanceInit < m_fDistance)
									{// �����̒l�𒴂�����
										m_fDistance = m_fDistanceInit;	// �����𒲐�
										break;
									}

									// ���_�v�Z�p
									D3DXMATRIX mat;						// ��]
									D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
									D3DXVECTOR3	vout;					// �v�Z����

																		// ���_�v�Z
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
									D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
									m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
								}
								bArea = true;	// �G���A����
							}
						}
					}
					else if (nMapNum == 3)
					{
						CMap_03 *pMap_03 = (CMap_03*)pMap;

						if (pMap_03->CollisionCamera(m_posVDest, m_posRDest) == false)
						{
							if (pMap_03->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_03->CollisionCamera(m_posVDest, m_posRDest) == false)
								{// ���ȉ��������烋�[�v
									m_fDistance -= 1.0f;		// �����𒲐�

									// ���_�v�Z�p
									D3DXMATRIX mat;						// ��]
									D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
									D3DXVECTOR3	vout;					// �v�Z����

									// ���_�v�Z
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
									D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
									m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
								}
								bArea = true;	// �G���A����
							}
						}
						else
						{
							if (pMap_03->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_03->CollisionCamera(m_posVDest, m_posRDest) == true)
								{// ���ȏ�ȏゾ�����烋�[�v
									m_fDistance += 1.0f;	// �����𒲐�

									if (m_fDistanceInit < m_fDistance)
									{// �����̒l�𒴂�����
										m_fDistance = m_fDistanceInit;	// �����𒲐�
										break;
									}

									// ���_�v�Z�p
									D3DXMATRIX mat;						// ��]
									D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
									D3DXVECTOR3	vout;					// �v�Z����

									// ���_�v�Z
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
									D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
									m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
								}
								bArea = true;	// �G���A����
							}
						}
					}
					else if (nMapNum == 4)
					{
						CMap_04 *pMap_04 = (CMap_04*)pMap;

						if (pMap_04->CollisionCamera(m_posVDest, m_posRDest) == false)
						{
							if (pMap_04->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_04->CollisionCamera(m_posVDest, m_posRDest) == false)
								{// ���ȉ��������烋�[�v
									m_fDistance -= 1.0f;		// �����𒲐�

																// ���_�v�Z�p
									D3DXMATRIX mat;						// ��]
									D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
									D3DXVECTOR3	vout;					// �v�Z����

																		// ���_�v�Z
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
									D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
									m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
								}
								bArea = true;	// �G���A����
							}
						}
						else
						{
							if (pMap_04->CollisionInOut(m_posRDest) == true)
							{
								while (pMap_04->CollisionCamera(m_posVDest, m_posRDest) == true)
								{// ���ȏ�ȏゾ�����烋�[�v
									m_fDistance += 1.0f;	// �����𒲐�

									if (m_fDistanceInit < m_fDistance)
									{// �����̒l�𒴂�����
										m_fDistance = m_fDistanceInit;	// �����𒲐�
										break;
									}

									// ���_�v�Z�p
									D3DXMATRIX mat;						// ��]
									D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
									D3DXVECTOR3	vout;					// �v�Z����

																		// ���_�v�Z
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
									D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
									m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
								}
								bArea = true;	// �G���A����
							}
						}
					}
				}
			}
		}
	}

	CScene *pScene		= NULL;		// �V�[��
	CScene *pSceneNext	= NULL;		// ���V�[��

	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);	// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// �L���X�g

				if (pMesh->ColInMesh(m_posVDest) == true)
				{// �͈̓`�F�b�N
					if (m_posVDest.y < pMesh->Getpos().y + 5.0f)
					{// ���ȉ��ɂȂ�����
						while (m_posVDest.y < pMesh->Getpos().y + 5.0f)
						{// ���ȉ��������烋�[�v
							m_fDistance -= 1.0f;		// �����𒲐�

							// ���_�v�Z�p
							D3DXMATRIX mat;						// ��]
							D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
							D3DXVECTOR3	vout;					// �v�Z����

							// ���_�v�Z
							D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
							D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
							m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
						}
					}
					else
					{// ���ȏ�ɏꍇ
						if (bArea == false)
						{// �G���A���肵�Ă��Ȃ�
							if (m_fDistance != m_fDistanceInit)
							{// �����̒l�ƈႤ�ꍇ
								while (pMesh->Getpos().y + 5.0f < m_posVDest.y)
								{// ���ȏ�ȏゾ�����烋�[�v
									m_fDistance += 1.0f;	// �����𒲐�

									if (m_fDistanceInit < m_fDistance)
									{// �����̒l�𒴂�����
										m_fDistance = m_fDistanceInit;	// �����𒲐�
										break;
									}

									// ���_�v�Z�p
									D3DXMATRIX mat;						// ��]
									D3DXVECTOR3	v(0, 0, m_fDistance);	// �����_�Ƃ̋���
									D3DXVECTOR3	vout;					// �v�Z����

									// ���_�v�Z
									D3DXMatrixRotationYawPitchRoll(&mat, m_rot.y, m_rot.x, m_rot.z);	// ��]
									D3DXVec3TransformCoord(&vout, &v, &mat);							// �����ƍ��킹��
									m_posVDest = vout + m_posRDest;		// �����_����̈ʒu
								}
							}
						}
					}
					break;
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CCameraMap ***
//=============================================================================
CCameraMap::CCameraMap() : CCamera()
{

}

//=============================================================================
// �f�X�g���N�^									(public)	*** CCameraMap ***
//=============================================================================
CCameraMap::~CCameraMap()
{

}

//=============================================================================
// ����������									(public)	*** CCameraMap ***
//=============================================================================
HRESULT CCameraMap::Init(void)
{
	CCamera::Init();	// ����������

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������										(public)	*** CCameraMap ***
//=============================================================================
void CCameraMap::Uninit(void)
{
	CCamera::Uninit();	// �I������
}

//=============================================================================
// �X�V����										(public)	*** CCameraMap ***
//=============================================================================
void CCameraMap::Update(void)
{

}

//=============================================================================
// �`�揈��										(public)	*** CCameraMap ***
//=============================================================================
void CCameraMap::SetCamera(void)
{
	CCamera::SetCamera();
}