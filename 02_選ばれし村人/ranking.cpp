//=========================================================================================================================
//
// �����L���O���� [ranking.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "ranking.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "xobject.h"
#include "shadow.h"
#include "score.h"
#include "fade.h"
#include "input.h"
#include "window.h"

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
int				g_nCntRankingTimer;

//=========================================================================================================================
// �����L���O�̏���������
//=========================================================================================================================
void InitRanking(void)
{
	g_nCntRankingTimer = 0;

	// �J����
	InitCamera();
	SetCamera(D3DXVECTOR3(100.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_RANKING);

	// ���C�g
	InitLight();

	// �t�B�[���h
	InitMeshField();

	// �V�����_�[
	InitMeshCylinder();

	// �e
	InitShadow();

	// X�I�u�W�F�N�g
	InitXObject();

	// �X�R�A
	InitScore();

	// �ݒ�
	// �t�B�[���h
	SetMeshField(0, 2);
	SetMeshField(1, 0);
	SetMeshField(2, 3);
	SetMeshField(3, 0);
	SetMeshField(4, 0);
	SetMeshField(5, 0);
	SetMeshField(6, 0);
	SetMeshField(7, 0);
	SetMeshField(8, 0);

	// �V�����_�[
	SetMeshCylinder(0);
	SetMeshCylinder(1);

	// �I�u�W�F�N�g
	SetXObject(TEXTTYPE_TREE_9);
	SetXObject(TEXTTYPE_TREE_10);
	SetXObject(TEXTTYPE_TREE_11);
	SetXObject(TEXTTYPE_TREE_12);
	SetXObject(TEXTTYPE_TREE_13);
	SetXObject(TEXTTYPE_TREE_14);

	//
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 95, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 235, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 375, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 515, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 655, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	AddScore(SCORETYPE_RANKING, 0);

	SetWindow(D3DXVECTOR3(275.0f, 130.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 400, 200, WINDOWTEXTURE_RANKING_BG, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, 95.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 180, WINDOWTEXTURE_RANKING_1, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 230.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_2, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 370.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_3, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 510.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_4, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 650.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_5, SETTEXTURE_FRONT);
}

//=========================================================================================================================
// �����L���O�̏I������
//=========================================================================================================================
void UninitRanking(void)
{
	// �J����
	UninitCamera();

	// ���C�g
	UninitLight();

	// �t�B�[���h
	UninitMeshField();

	// �V�����_�[
	UninitMeshCylinder();

	// �e
	UninitShadow();

	// X�I�u�W�F�N�g
	UninitXObject();

	// �X�R�A
	UninitScore();

	// �E�B���h�E
	DeleteWindow();
}

//=========================================================================================================================
// �����L���O�̍X�V����
//=========================================================================================================================
void UpdateRanking(void)
{
	// �J����
	UpdateCamera();

	// ���C�g
	UpdateLight();

	// �t�B�[���h
	UpdateMeshField();

	// �V�����_�[
	UpdateMeshCylinder();

	// �e
	UpdateShadow();

	// X�I�u�W�F�N�g
	UpdateXObject();

	// �X�R�A
	UpdateScore();

	g_nCntRankingTimer++;

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
		GetButtonTrigger(2) == true) || g_nCntRankingTimer > 600)
	{// �^�C�g���ֈڍs
		SetFadeMode(MODE_TITLE);
	}
}

//=========================================================================================================================
// �����L���O�̕`�揈��
//=========================================================================================================================
void DrawRanking(void)
{
	// �J����
	SetUpCamera();

	// �t�B�[���h
	DrawMeshField();

	// �V�����_�[
	DrawMeshCylinder();

	// �e
	DrawShadow();

	// X�I�u�W�F�N�g
	DrawXObject();

	// �X�R�A
	DrawScore();
}
