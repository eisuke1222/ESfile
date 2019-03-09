//=========================================================================================================================
//
// ���C�g���� [light.cpp]
// Author : 
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "light.h"
#include "input.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_LIGHT		(4)						// ���C�g�̍ő吔

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
D3DLIGHT9 g_Light[MAX_LIGHT];

D3DXCOLOR g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0);
//=========================================================================================================================
// ���C�g�̏���������
//=========================================================================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir[MAX_LIGHT];				// �ݒ�p�����x�N�g��
	int nCntLight;

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&g_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// ���C�g�̎�ނ�ݒ�
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(0.55f, -0.8f, 0.55f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ���C�g�̎�ނ�ݒ�
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(0.55f, -0.8f, -0.55f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ���C�g�̎�ނ�ݒ�
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.55f, -0.8f, 0.55f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ���C�g�̎�ނ�ݒ�
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.55f, -0.8f, -0.55f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_Light[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=========================================================================================================================
// ���C�g�̏I������
//=========================================================================================================================
void UninitLight(void)
{

}

//=========================================================================================================================
// ���C�g�̍X�V����
//=========================================================================================================================
void UpdateLight(void)
{
	
}

