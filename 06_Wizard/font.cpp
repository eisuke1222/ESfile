//=============================================================================
//
// �t�H���g���� [font.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "font.h"			// �t�H���g
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����

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
CFont *CFont::m_apFont[FONTNUM_MAX] = {};		// �V�[��

#ifdef _DEBUG
bool CFont::m_bDispAll = true;					// �\��
#else
bool CFont::m_bDispAll = false;					// ��\��
#endif

char CFont::m_MessageData[CScene::OBJTYPE_MAX][m_MAX_MESSAGE] = {};	// ���b�Z�[�W

//=============================================================================
// �R���X�g���N�^										(public)	*** CFont ***
//=============================================================================
CFont::CFont()
{
	for (int nCntFont = 0; nCntFont < FONTNUM_MAX; nCntFont++)
	{// �t�H���g�J�E���g
		if (m_apFont[nCntFont] == NULL)
		{// NULL�̏ꍇ
			m_apFont[nCntFont] = this;				// ��������
			m_nID = nCntFont;						// ID�L��
			m_bDisp = false;						// �\��

			// �R�}���h�A�f�o�b�O�̂ݕ\��
			if (nCntFont == FONTNUM_COMMAND || nCntFont == FONTNUM_DEBUG) 
				m_bDisp = true;

#ifdef _DEBUG	// �f�o�b�O�̂�
			// �e�X�g�̂�
			if (nCntFont == FONTNUM_TEST) 
				m_bDisp = true;
#endif
			break;
		}
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CFont ***
//=============================================================================
CFont::~CFont()
{

}

//=============================================================================
// �N���G�C�g										(public)	*** CFont ***
//=============================================================================
CFont *CFont::Create(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col)
{
	CFont *pFont = NULL;			// �|�C���^

	if (pFont == NULL)
	{// NULL�̏ꍇ
		pFont = new CFont;			// �������m��
	}

	if (pFont != NULL)
	{// NULL�ȊO�̏ꍇ
		pFont->Init(nHeight, nWidth, nWeight, rect, uFormat, col);		// ����������
	}

	return pFont;
}

//=============================================================================
// �S�J������										(public)	*** CFont ***
//=============================================================================
void CFont::ReleaseAll(void)
{
	for (int nCntFont = 0; nCntFont < FONTNUM_MAX; nCntFont++)
	{// �t�H���g�J�E���g
		if (m_apFont[nCntFont] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apFont[nCntFont]->Uninit();		// �I������
			delete m_apFont[nCntFont];			// �������J��
			m_apFont[nCntFont] = NULL;			// NULL��
		}
	}
}

//=============================================================================
// �S�`�揈��										(public)	*** CFont ***
//=============================================================================
void CFont::DrawAll(void)
{
	static int nCntPress = 0;		// �������J�E���g

	if (CManager::GetInputKeyboard()->GetPress(DIK_F1) == true)
	{// �iF1�j�L�[�������ꂽ
		nCntPress++;				// ���ԃJ�E���g
		if (nCntPress == 60)
		{// ���t���[���ɂȂ�����
			m_bDispAll = m_bDispAll ? false : true;		// �\���ؑ�
		}
	}
	else
	{// ���̑�
		nCntPress = 0;				// ���ԃJ�E���g������
	}

	if (m_bDispAll == true)
	{// �\������
		for (int nCntFont = 0; nCntFont < FONTNUM_MAX; nCntFont++)
		{// �t�H���g�J�E���g
			if (m_apFont[nCntFont] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_apFont[nCntFont]->Draw();				// �X�V����
			}
		}
	}
}

//=============================================================================
// ����������										(public)	*** CFont ***
//=============================================================================
HRESULT CFont::Init(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �t�H���g����
	D3DXCreateFont(pDevice, nHeight, nWidth, nWeight, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas", &m_pFont);

	m_rect		= rect;			// �傫��
	m_uFormat	= uFormat;		// �t�H�[�}�b�g
	m_col		= col;			// �F

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CFont ***
//=============================================================================
void CFont::Uninit(void)
{
	if (m_pFont != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pFont->Release();		// �J��
		m_pFont = NULL;			// NULL��
	}
}

//=============================================================================
// �X�V����											(public)	*** CFont ***
//=============================================================================
void CFont::Update(void)
{

}

//=============================================================================
// �`�揈��											(public)	*** CFont ***
//=============================================================================
void CFont::Draw(void)
{
	if (m_pFont != NULL)
	{// NULL�ȊO�̏ꍇ
		if (m_bDisp == true)
		{// �\�����Ă���
			// �e�L�X�g�`��
			m_pFont->DrawText(NULL, m_Message, -1, &m_rect, m_uFormat, m_col);
		}
	}
}

//=============================================================================
// �����ݒ菈��										(public)	*** CFont ***
//=============================================================================
void CFont::SetMessageData(CScene::OBJTYPE objType, SETTYPE nSetType, char *aMessage, ...)
{
	// �����擾�p
	char cMessage[m_MAX_MESSAGE], cMessageCat[m_MAX_MESSAGE + (m_MAX_MESSAGE / 2)];
	cMessage[0] = '\0';	cMessageCat[0] = '\0';

	// �ώ擾
	va_list arg;
	va_start(arg, aMessage);
	vsprintf(cMessage, aMessage, arg);
	va_end(arg);

	// ���������
	if (nSetType == SETTYPE_NEW)
	{// �㏑��
		wsprintf(&m_MessageData[objType][0], "%s", &cMessage[0]);
	}
	else if (nSetType == SETTYPE_ADD)
	{// �ǉ��i���j
		wsprintf(&cMessageCat[0], "%s", &m_MessageData[objType][0]);

		// �ǉ�
		strcat(&cMessageCat[0], &cMessage[0]);
		cMessageCat[m_MAX_MESSAGE - 1] = '\0';

		wsprintf(&m_MessageData[objType][0], "%s", &cMessageCat[0]);
	}
	else if (nSetType == SETTYPE_LOG)
	{// �ǉ��i��j
		wsprintf(&cMessageCat[0], "%s", &cMessage[0]);

		// �ǉ�
		strcat(&cMessageCat[0], &m_MessageData[objType][0]);
		cMessageCat[m_MAX_MESSAGE - 1] = '\0';

		wsprintf(&m_MessageData[objType][0], "%s", &cMessageCat[0]);
	}
}

//=============================================================================
// �����ݒ菈��											(public)	*** CFont ***
//=============================================================================
void CFont::SetMessage(SETTYPE nSetType, char *aMessage, ...)
{
	// �����擾�p
	char cMessage[m_MAX_MESSAGE], cMessageCat[m_MAX_MESSAGE + (m_MAX_MESSAGE / 2)];
	cMessage[0] = '\0';	cMessageCat[0] = '\0';

	// �ώ擾
	va_list arg;
	va_start(arg, aMessage);
	vsprintf(cMessage, aMessage, arg);
	va_end(arg);

	// ���������
	if (nSetType == SETTYPE_NEW)
	{// �㏑��
		wsprintf(&m_Message[0], "%s", &cMessage[0]);
	}
	else if (nSetType == SETTYPE_ADD)
	{// �ǉ��i���j
		wsprintf(&cMessageCat[0], "%s", &m_Message[0]);

		// �ǉ�
		strcat(&cMessageCat[0], &cMessage[0]);
		cMessageCat[m_MAX_MESSAGE - 1] = '\0';

		wsprintf(&m_Message[0], "%s", &cMessageCat[0]);
	}
	else if (nSetType == SETTYPE_LOG)
	{// �ǉ��i��j
		wsprintf(&cMessageCat[0], "%s", &cMessage[0]);

		// �ǉ�
		strcat(&cMessageCat[0], &m_Message[0]);
		cMessageCat[m_MAX_MESSAGE - 1] = '\0';

		wsprintf(&m_Message[0], "%s", &cMessageCat[0]);
	}
}

//=============================================================================
// �J������											(protected)	*** CFont ***
//=============================================================================
void CFont::Release(void)
{
	if (m_apFont[m_nID] != NULL)
	{// NUll�ȊO�̏ꍇ
		int nID = m_nID;			// ID��ۑ��i�f�[�^�̕s��h�~�j
		delete m_apFont[nID];		// �������J��
		m_apFont[nID] = NULL;		// NULL��
	}
}