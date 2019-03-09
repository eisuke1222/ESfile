//=========================================================================================================================
// 
// �������� [explosion.h]
// Author : Einsuke Sakagawa
// 
//=========================================================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitExplosion(void);			// ��������������
void UninitExplosion(void);			// �����I������
void UpdateExplosion(void);			// �����X�V����
void DrawExplosion(void);			// �����`�揈��

void SetExplosion(D3DXVECTOR3 pos);		// �����̐ݒ菈��

#endif