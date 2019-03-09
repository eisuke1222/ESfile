//=============================================================================
//
// テキスト処理 [text.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "text.h"			// モデル
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_NAME	"data\\TEXT\\SYSTEM\\system.ini"		// ファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int						CText::m_nMaxText = 0;								// テキスト数
int						CText::m_nMaxType[CText::TYPE_MAX] = { 0 };			// タイプ別テキスト数
CText::TEXT				*CText::m_Text = NULL;								// テキスト

CModel::MODEL_INFO		**CText::m_pModelInfo = NULL;						// モデル
CMotion::MOTION_INFO	**CText::m_pMotionInfo = NULL;						// モーション

int						CText::m_nMaxModel = NULL;							// モデル数
CText::MODEL_STORAGE	*CText::m_pModelStorage = NULL;						// モデル保管

//=============================================================================
// コンストラクタ								(public)	*** CText ***
//=============================================================================
CText::CText()
{

}

//=============================================================================
// デストラクタ									(public)	*** CText ***
//=============================================================================
CText::~CText()
{

}

//=============================================================================
// ロード処理									(public)	*** CText ***
//=============================================================================
HRESULT CText::Load(void)
{
	FILE *pFile = NULL;					// ファイルポインタ

	pFile = fopen(TEXT_NAME, "r");		// ファイルオープン

	if (pFile != NULL)
	{// NULL以外の場合
		int nCntText = 0;				// テキストカウンタ

		char *pStrCut;					// 文字列の先頭へのポインタ
		char aLine[256];				// 文字列読み取り用（１行分）
		char aStr[256];					// 文字列抜き出し用

		pStrCut = ReadFile(pFile, &aLine[0]);				// コメントチェック
		strcpy(aStr, pStrCut);								// 文字変換

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{// 読み込み開始
			do
			{// ループ
				pStrCut = ReadFile(pFile, &aLine[0]);					// コメントチェック
				strcpy(aStr, pStrCut);									// 文字変換

				if (memcmp(aStr, "NUM_TEXT", strlen("NUM_TEXT")) == 0)
				{// テキスト数
					pStrCut += strlen("NUM_TEXT");								// ポインタを進める
					PopString(pStrCut, aStr);									// 文字変換
					m_nMaxText = atoi(aStr);									// 整数型に変換

					if (m_Text == NULL)
					{// NULLの場合
						m_Text = new TEXT[m_nMaxText];							// テキスト

						for (int nCount = 0; nCount < m_nMaxText; nCount++)
						{// テキスト数カウント
							// 構造体初期化
							m_Text[nCount].type			= TYPE_MODEL;		// 種類
							m_Text[nCount].nTypeNum		= 0;				// 種類番号
							m_Text[nCount].pFileName[0] = '\0';				// ファイル名
							m_Text[nCount].nMax			= 0;				// 最大値
							m_Text[nCount].bLoad		= false;			// ロードしたか
						}
					}
				}

				if (nCntText < m_nMaxText)
				{// テキスト数以下の場合
					if (memcmp(aStr, "MODEL_TEXT", strlen("MODEL_TEXT")) == 0)
					{// モデル
						pStrCut += strlen("MODEL_TEXT");							// ポインタを進める
						PopString(pStrCut, aStr);									// 文字変換
						strcpy(m_Text[nCntText].pFileName, aStr);					// 文字変換
						m_Text[nCntText].type = TYPE_MODEL;							// タイプ設定
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MODEL];			// タイプ番号
						m_nMaxType[TYPE_MODEL]++;									// タイプ最大数カウント
						m_Text[nCntText].nIdx = nCntText;							// テキスト番号
						nCntText++;													// インデックスカウント
					}
					else if (memcmp(aStr, "MOTION_TEXT", strlen("MOTION_TEXT")) == 0)
					{// モーション
						pStrCut += strlen("MOTION_TEXT");							// ポインタを進める
						PopString(pStrCut, aStr);									// 文字変換
						strcpy(m_Text[nCntText].pFileName, aStr);					// 文字変換
						m_Text[nCntText].type = TYPE_MOTION;						// タイプ設定
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MOTION];		// タイプ番号
						m_nMaxType[TYPE_MOTION]++;									// タイプ最大数カウント
						m_Text[nCntText].nIdx = nCntText;							// テキスト番号
						nCntText++;													// インデックスカウント
					}
					else if (memcmp(aStr, "MESHSKIN_TEXT", strlen("MESHSKIN_TEXT")) == 0)
					{// メッシュスキン
						pStrCut += strlen("MESHSKIN_TEXT");							// ポインタを進める
						PopString(pStrCut, aStr);									// 文字変換
						strcpy(m_Text[nCntText].pFileName, aStr);					// 文字変換
						m_Text[nCntText].type = TYPE_MESHSKIN;						// タイプ設定
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MESHSKIN];		// タイプ番号
						m_nMaxType[TYPE_MESHSKIN]++;								// タイプ最大数カウント
						m_Text[nCntText].nIdx = nCntText;							// テキスト番号
						nCntText++;													// インデックスカウント
					}
					else if (memcmp(aStr, "MESHFIELD_TEXT", strlen("MESHFIELD_TEXT")) == 0)
					{// メッシュフィールド
						pStrCut += strlen("MESHFIELD_TEXT");						// ポインタを進める
						PopString(pStrCut, aStr);									// 文字変換
						strcpy(m_Text[nCntText].pFileName, aStr);					// 文字変換
						m_Text[nCntText].type = TYPE_MESHFIELD;						// タイプ設定
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MESHFIELD];		// タイプ番号
						m_Text[nCntText].nIdx = nCntText;							// テキスト番号
						m_nMaxType[TYPE_MESHFIELD]++;								// タイプ最大数カウント
						nCntText++;													// インデックスカウント
					}
				}
			} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
		}
		fclose(pFile);		// ファイルクローズ
	}

	for (int nCount = 0; nCount < m_nMaxText; nCount++)
	{// テキスト数カウント
		if (m_Text[nCount].type == TYPE_MODEL)
		{// タイプがモデル
			if (m_pModelInfo == NULL)
			{// NULLの場合
				m_pModelInfo = new CModel::MODEL_INFO*[m_nMaxType[TYPE_MODEL]];			// メモリ確保

				// 構造体初期化
				for (int nCount = 0; nCount < m_nMaxType[TYPE_MODEL]; nCount++)
				{// タイプ別テキスト数カウント
					m_pModelInfo[nCount] = NULL;	// NULLへ
				}
			}
		}

		if (m_Text[nCount].type == TYPE_MOTION)
		{// タイプがモーション
			if (m_pMotionInfo == NULL)
			{// NULLの場合
				m_pMotionInfo = new CMotion::MOTION_INFO*[m_nMaxType[TYPE_MOTION]];		// メモリ確保

				// 構造体初期化
				for (int nCount = 0; nCount < m_nMaxType[TYPE_MOTION]; nCount++)
				{// タイプ別テキスト数カウント
					m_pMotionInfo[nCount] = NULL;		// NULLへ
				}
			}
		}
	}

	// モデル数取得
	m_nMaxModel = GetMaxModel();

	if (m_pModelStorage == NULL)
	{// NULLの場合
		m_pModelStorage = new MODEL_STORAGE[m_nMaxModel];		// メモリ確保

		for (int nCount = 0; nCount < m_nMaxModel; nCount++)
		{// モデル数カウント
			m_pModelStorage[nCount].pFileName[0]	= '\0';		// ファイル名
			m_pModelStorage[nCount].pTexture		= NULL;		// テクスチャ情報へのポインタ
			m_pModelStorage[nCount].pBuffMat		= NULL;		// マテリアル情報へのポインタ
			m_pModelStorage[nCount].pMesh			= NULL;		// メッシュ情報へのポインタ
			m_pModelStorage[nCount].nNumMat			= 0;		// マテリアル情報の数
			m_pModelStorage[nCount].bLoad			= false;	// ロードしたか
		}
	}

	return S_OK;
}

//=============================================================================
// アンロード処理									(public)	*** CText ***
//=============================================================================
void CText::Unload(void)
{
	// モデル
	if (m_pModelInfo != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// テキスト数カウント
			if (m_Text[nCount].type == TYPE_MODEL && m_Text[nCount].bLoad == true)
			{// タイプがモデル && ロードしている
				delete[] m_pModelInfo[m_Text[nCount].nTypeNum];		// メモリ開放
				m_pModelInfo[m_Text[nCount].nTypeNum] = NULL;		// NULLへ
			}
		}
		delete[] m_pModelInfo;		// メモリ開放
		m_pModelInfo = NULL;		// NULLへ
	}

	// モーション
	if (m_pMotionInfo != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// テキスト数カウント
			if (m_Text[nCount].type == TYPE_MOTION && m_Text[nCount].bLoad == true)
			{// タイプがモーション && ロードしている
				for (int nCntAnim = 0; nCntAnim < m_Text[nCount].nMax; nCntAnim++)
				{// モーション数カウント
					for (int nCntKey = 0; nCntKey < m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].nKeyMax; nCntKey++)
					{// キー数カウント
						delete[] m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo[nCntKey].aKey;	// メモリ開放
						m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo[nCntKey].aKey = NULL;		// NULLへ
					}
					delete[] m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo;		// メモリ開放
					m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo = NULL;		// NULLへ
				}
				delete[] m_pMotionInfo[m_Text[nCount].nTypeNum];	// メモリ開放
				m_pMotionInfo[m_Text[nCount].nTypeNum] = NULL;		// NULLへ
			}
		}
		delete[] m_pMotionInfo;		// メモリ開放
		m_pMotionInfo = NULL;		// NULLへ
	}

	// テキスト
	if (m_Text != NULL)
	{// NULL以外の場合
		delete[] m_Text;		// メモリ開放
		m_Text = NULL;			// NULLへ
	}

	// モデル保管
	if (m_pModelStorage != NULL)
	{// NULL以外の場合
		ReleaseXModelData();
		delete[] m_pModelStorage;
		m_pModelStorage = NULL;
	}
}

//=============================================================================
// ファイル読み込み処理								(public)	*** CText ***
//=============================================================================
char *CText::ReadFile(FILE *pFile, char *pDest)
{
	fgets(pDest, 256, pFile);	// １行読み込み

	if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
	{// （＃）だったら
		ReadFile(pFile, pDest);		// 読み込み
	}
	else
	{// その他
		do
		{// ループ
			if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
			{// （＃、改行）だったら
				ReadFile(pFile, pDest);
			}
			else if (memcmp(pDest, " ", strlen(" ")) == 0)
			{// （半角スペース）だったら
				wsprintf(&pDest[0], &pDest[strlen(" ")]);
			}
			else if (memcmp(pDest, "\t", strlen("\t")) == 0)
			{// （tab）だったら
				wsprintf(&pDest[0], &pDest[strlen("\t")]);
			}
			else if (memcmp(pDest, "　", strlen("　")) == 0)
			{// （全角スペース）だったら
				wsprintf(&pDest[0], &pDest[strlen("　")]);
			}
		} while (
			memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0 ||
			memcmp(pDest, " ", strlen(" ")) == 0 || memcmp(pDest, "\t", strlen("\t")) == 0 ||
			memcmp(pDest, "　", strlen("　")) == 0);
		// それ以外だったらぬける
	}

	return pDest;	// 値を返す
}

//=============================================================================
// 読み込み調整処理									(public)	*** CText ***
//=============================================================================
void CText::PopString(char *pStrCur, char *pStr)
{
	do
	{// ループ
		if (memcmp(pStrCur, "=", strlen("=")) == 0)
		{// （＝）だったら
			wsprintf(&pStrCur[0], &pStrCur[strlen("=")]);
		}
		else if (memcmp(pStrCur, " ", strlen(" ")) == 0)
		{// （半角スペース）だったら
			wsprintf(&pStrCur[0], &pStrCur[strlen(" ")]);
		}
		else if (memcmp(pStrCur, "\t", strlen("\t")) == 0)
		{// （tab）だったら
			wsprintf(&pStrCur[0], &pStrCur[strlen("\t")]);
		}
		else if (memcmp(pStrCur, "　", strlen("　")) == 0)
		{// （全角スペース）だったら
			wsprintf(&pStrCur[0], &pStrCur[strlen("　")]);
		}
	} while (
		memcmp(pStrCur, "=", strlen("=")) == 0 ||
		memcmp(pStrCur, " ", strlen(" ")) == 0 || memcmp(pStrCur, "\t", strlen("\t")) == 0 ||
		memcmp(pStrCur, "　", strlen("　")) == 0);
	// それ以外だったらぬける

	int nCntEnd = 0;			// 読み込み終了いち番号
	strcpy(pStr, pStrCur);		// 上書き

	do
	{// ループ
		if (memcmp(pStr, "#", strlen("#")) == 0)
		{// （＃）だったら
			wsprintf(&pStr[0], &pStr[strlen("#")]);
		}
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{// （改行）だったら
			wsprintf(&pStr[0], &pStr[strlen("\n")]);
		}
		else if (memcmp(pStr, " ", strlen(" ")) == 0)
		{// （半角スペース）だったら
			wsprintf(&pStr[0], &pStr[strlen(" ")]);
		}
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{// （tab）だったら
			wsprintf(&pStr[0], &pStr[strlen("\t")]);
		}
		else if (memcmp(pStr, "　", strlen("　")) == 0)
		{// （全角スペース）だったら
			wsprintf(&pStr[0], &pStr[strlen("　")]);
		}
		else
		{// それ以外だったら
			wsprintf(&pStr[0], &pStr[1]);
		}

		nCntEnd++;		// カウント
	} while (
		memcmp(pStr, "#", strlen("#")) != 0 && memcmp(pStr, "\n", strlen("\n")) != 0 &&
		memcmp(pStr, " ", strlen(" ")) != 0 && memcmp(pStr, "\t", strlen("\t")) != 0 &&
		memcmp(pStr, "　", strlen("　")) != 0);
	// それ以外だったらぬける

	strcpy(pStr, pStrCur);						// 上書き
	wsprintf(&pStrCur[0], &pStrCur[nCntEnd]);	// 上書き
	pStr[nCntEnd] = '\0';						// null文字
}

//=============================================================================
// モデル取得処理										(public)	*** CText ***
//=============================================================================
CModel::MODEL_INFO	*CText::GetModel(char *cName, int *nMax)
{
	if (m_pModelInfo != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// テキスト数カウント
			if (m_Text[nCount].type == TYPE_MODEL && strcmp(m_Text[nCount].pFileName, cName) == 0)
			{// タイプがモデル && タイプ番号と一致
				if (m_Text[nCount].bLoad == false)
				{// ロードしていない
					LoadModelText(&m_Text[nCount].pFileName[0], &m_Text[nCount].nMax, m_Text[nCount].nTypeNum);	// モデルロード
					m_Text[nCount].bLoad = true;	// ロードした
				}
				*nMax = m_Text[nCount].nMax;		// 最大値を設定
			
				return m_pModelInfo[m_Text[nCount].nTypeNum];		// 値を返す

				break;
			}
		}
	}

	MessageBox(0, "CText::GetModel() nNum Error", "警告", MB_OK);
	return m_pModelInfo[m_nMaxType[TYPE_MODEL]];	// 値を返す
}

//=============================================================================
// モーション取得処理									(public)	*** CText ***
//=============================================================================
CMotion::MOTION_INFO	*CText::GetMotion(char *cName)
{
	if (m_pMotionInfo != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// テキスト数カウント
			if (m_Text[nCount].type == TYPE_MOTION && strcmp(m_Text[nCount].pFileName, cName) == 0)
			{// タイプがモーション && タイプ番号と一致
				if (m_Text[nCount].bLoad == false)
				{// ロードしていない
					LoadMotionText(&m_Text[nCount].pFileName[0], &m_Text[nCount].nMax, m_Text[nCount].nTypeNum);		// モーションロード
					m_Text[nCount].bLoad = true;	// ロードした
				}
		
				return m_pMotionInfo[m_Text[nCount].nTypeNum];		// 値を返す

				break;
			}
		}
	}

	MessageBox(0, "CText::GetMotion() nNum Error", "警告", MB_OK);
	return m_pMotionInfo[m_nMaxType[TYPE_MOTION]];		// 値を返す
}

//=============================================================================
// モデル取得処理										(public)	*** CText ***
//=============================================================================
void CText::GetXModelData(char *cName, LPDIRECT3DTEXTURE9 **pTexture, LPD3DXMESH *pMesh, LPD3DXBUFFER *pBuffMat, DWORD *nNumMat)
{
	if (m_pModelStorage != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nMaxModel; nCount++)
		{// テキスト数カウント
			if (m_pModelStorage[nCount].bLoad == false)
			{// ロードしていない
				strcpy(m_pModelStorage[nCount].pFileName, cName);		// モデル名保存
				LoadXModelData(cName, nCount);							// モデル読み込み

				*pTexture	= m_pModelStorage[nCount].pTexture;			// テクスチャ情報
				*pMesh		= m_pModelStorage[nCount].pMesh;			// メッシュ情報
				*pBuffMat	= m_pModelStorage[nCount].pBuffMat;			// マテリアル情報
				*nNumMat	= m_pModelStorage[nCount].nNumMat;			// マテリアル数

				break;
			}
			else if (m_pModelStorage[nCount].bLoad == true && strcmp(m_pModelStorage[nCount].pFileName, cName) == 0)
			{// ロードしてる
				*pTexture	= m_pModelStorage[nCount].pTexture;			// テクスチャ情報
				*pMesh		= m_pModelStorage[nCount].pMesh;			// メッシュ情報
				*pBuffMat	= m_pModelStorage[nCount].pBuffMat;			// マテリアル情報
				*nNumMat	= m_pModelStorage[nCount].nNumMat;			// マテリアル数

				break;
			}
		}
	}
}

//=============================================================================
// モデルデータ破棄処理								(public)	*** CText ***
//=============================================================================
void CText::ReleaseXModelData(void)
{
	if (m_pModelStorage != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nMaxModel; nCount++)
		{// テキスト数カウント
			if (m_pModelStorage[nCount].bLoad == true)
			{// ロードしてる
				m_pModelStorage[nCount].pFileName[0] = '\0';		// 初期化

				if (m_pModelStorage[nCount].pTexture != NULL)
				{// NULL以外の場合
					for (int nCntMat = 0; nCntMat < (int)m_pModelStorage[nCount].nNumMat; nCntMat++)
					{// マテリアル数カウント
						if (m_pModelStorage[nCount].pTexture[nCntMat] != NULL)
						{// NULL以外の場合
							m_pModelStorage[nCount].pTexture[nCntMat]->Release();	// 開放
							m_pModelStorage[nCount].pTexture[nCntMat] = NULL;		// NULLへ
						}
					}

					delete m_pModelStorage[nCount].pTexture;		// 破棄
					m_pModelStorage[nCount].pTexture = NULL;		// NULLへ
				}

				if (m_pModelStorage[nCount].pBuffMat != NULL)
				{// NULL以外の場合
					m_pModelStorage[nCount].pBuffMat->Release();	// 開放
					m_pModelStorage[nCount].pBuffMat = NULL;		// NULLへ
				}

				if (m_pModelStorage[nCount].pMesh != NULL)
				{// NULL以外の場合
					m_pModelStorage[nCount].pMesh->Release();		// 開放
					m_pModelStorage[nCount].pMesh = NULL;			// NULLへ
				}

				m_pModelStorage[nCount].nNumMat = 0;				// 初期化
				m_pModelStorage[nCount].bLoad = false;				// 初期化
			}
		}
	}
}

//=============================================================================
// モデルロード処理									(public)	*** CText ***
//=============================================================================
void CText::LoadModelText(char *cName, int *nMax, int nNum)
{
	int nModelMax = 0;					// モデル数

	FILE *pFile = NULL;					// ファイルポインタ

	pFile = fopen(cName, "r");			// ファイルオープン

	if (pFile != NULL)
	{// NULL以外の場合
		int nCntModel = 0;				// モデルカウンタ

		char *pStrCut;					// 文字列の先頭へのポインタ
		char aLine[256];				// 文字列読み取り用（１行分）
		char aStr[256];					// 文字列抜き出し用

		pStrCut = ReadFile(pFile, &aLine[0]);				// コメントチェック
		strcpy(aStr, pStrCut);								// 文字変換

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{// 読み込み開始
			do
			{// ループ
				pStrCut = ReadFile(pFile, &aLine[0]);			// コメントチェック
				strcpy(aStr, pStrCut);							// 文字変換

				if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
				{// モデル数
					pStrCut += strlen("NUM_MODEL");						// ポインタを進める
					PopString(pStrCut, aStr);							// 文字変換
					nModelMax = atoi(aStr);								// 整数型に変換

					if (m_pModelInfo[nNum] == NULL)
					{// NULLの場合
						m_pModelInfo[nNum] = new CModel::MODEL_INFO[nModelMax];	// メモリ確保

						for (int nCount = 0; nCount < nModelMax; nCount++)
						{
							m_pModelInfo[nNum][nCount].cFileName[0] = '\0';
							m_pModelInfo[nNum][nCount].nIndex = -1;
							m_pModelInfo[nNum][nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							m_pModelInfo[nNum][nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
				else if (memcmp(aStr, "MODEL_FILENAME", strlen("MODEL_FILENAME")) == 0)
				{// モデルファイル名
					pStrCut += strlen("MODEL_FILENAME");						// ポインタを進める
					PopString(pStrCut, aStr);									// 文字変換
					strcpy(m_pModelInfo[nNum][nCntModel].cFileName, aStr);		// ファイル名を保存

					nCntModel++;		// モデルカウント
				}
				else if (memcmp(aStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
				{// キャラクター設定
					nCntModel = 0;		// モデルカウンタを初期化

					do
					{// ループ
						pStrCut = ReadFile(pFile, &aLine[0]);						// コメントチェック
						strcpy(aStr, pStrCut);										// 文字変換

						if (memcmp(aStr, "PARTSSET", strlen("PARTSSET")) == 0)
						{// パーツ設定
							do
							{// ループ
								pStrCut = ReadFile(pFile, &aLine[0]);					// コメントチェック
								strcpy(aStr, pStrCut);									// 文字変換

								if (memcmp(aStr, "INDEX", strlen("INDEX")) == 0)
								{// モデル番号
									pStrCut += strlen("INDEX");									// ポインタを進める
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].nIndex = atoi(aStr);			// 整数型に変換
								}
								else if (memcmp(aStr, "PARENT", strlen("PARENT")) == 0)
								{// 親モデル番号
									pStrCut += strlen("PARENT");								// ポインタを進める
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].nIndex = atoi(aStr);			// 整数型に変換
								}
								else if (memcmp(aStr, "POS", strlen("POS")) == 0)
								{// 位置
									pStrCut += strlen("POS");									// ポインタを進める
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].pos.x = (float)atof(aStr);	// 整数型に変換
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].pos.y = (float)atof(aStr);	// 整数型に変換
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].pos.z = (float)atof(aStr);	// 整数型に変換
								}
								else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
								{// 向き
									pStrCut += strlen("ROT");									// ポインタを進める
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].rot.x = (float)atof(aStr);	// 整数型に変換
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].rot.y = (float)atof(aStr);	// 整数型に変換
									PopString(pStrCut, aStr);									// 文字変換
									m_pModelInfo[nNum][nCntModel].rot.z = (float)atof(aStr);	// 整数型に変換
								}
							} while (memcmp(aStr, "END_PARTSSET", strlen("END_PARTSSET")) != 0);
							nCntModel++;	// モデルカウント
						}
					} while (memcmp(aStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) != 0);
				}
			} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
		}
		fclose(pFile);					// ファイルクローズ
	}

	*nMax = nModelMax;		// モデル数を設定
}

//=============================================================================
// モーションロード処理								(public)	*** CText ***
//=============================================================================
void CText::LoadMotionText(char *cName, int *nMax, int nNum)
{
	int nModelMax = 0;					// モデル数
	int nMotionMax = 0;					// モーション数

	FILE *pFile = NULL;					// ファイルポインタ

	pFile = fopen(cName, "r");			// ファイルオープン

	if (pFile != NULL)
	{// NULL以外の場合
		int nCntModel = 0;				// モデルカウンタ
		int nCntMotion = 0;				// モーションカウンタ
		int nCntKey = 0;				// キーカウンタ

		char *pStrCut;					// 文字列の先頭へのポインタ
		char aLine[256];				// 文字列読み取り用（１行分）
		char aStr[256];					// 文字列抜き出し用

		pStrCut = ReadFile(pFile, &aLine[0]);				// コメントチェック
		strcpy(aStr, pStrCut);								// 文字変換

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{// 読み込み開始
			do
			{// ループ
				pStrCut = ReadFile(pFile, &aLine[0]);			// コメントチェック
				strcpy(aStr, pStrCut);							// 文字変換

				if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
				{// モデル数
					pStrCut += strlen("NUM_MODEL");						// ポインタを進める
					PopString(pStrCut, aStr);							// 文字変換
					nModelMax = atoi(aStr);								// 整数型に変換					
				}
				else if (memcmp(aStr, "NUM_MOTION", strlen("NUM_MOTION")) == 0)
				{// モーション数
					pStrCut += strlen("NUM_MOTION");					// ポインタを進める
					PopString(pStrCut, aStr);							// 文字変換
					nMotionMax = atoi(aStr);							// 整数型に変換
					
					if (m_pMotionInfo[nNum] == NULL)
					{// NULLの場合
						m_pMotionInfo[nNum] = new CMotion::MOTION_INFO[nMotionMax];	// メモリ確保

						for (int nCount = 0; nCount < nMotionMax; nCount++)
						{// アニメーションカウント
							m_pMotionInfo[nNum][nCount].aKeyInfo = NULL;			// NULLへ
						}
					}
				}
				else if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{// モーション設定
					nCntKey = 0;	// キーカウンタ初期化

					do
					{// ループ
						pStrCut = ReadFile(pFile, &aLine[0]);						// コメントチェック
						strcpy(aStr, pStrCut);										// 文字変換

						if (memcmp(aStr, "LOOP", strlen("LOOP")) == 0)
						{// ループするか
							pStrCut += strlen("LOOP");										// ポインタを進める
							PopString(pStrCut, aStr);										// 文字変換
							m_pMotionInfo[nNum][nCntMotion].bLoop = atoi(aStr);				// 整数型に変換
						}
						else if (memcmp(aStr, "NUM_KEY", strlen("NUM_KEY")) == 0)
						{// キー数
							pStrCut += strlen("NUM_KEY");									// ポインタを進める
							PopString(pStrCut, aStr);										// 文字変換
							m_pMotionInfo[nNum][nCntMotion].nKeyMax = atoi(aStr);			// 整数型に変換

							if (m_pMotionInfo[nNum][nCntMotion].aKeyInfo == NULL)
							{// NULLの場合
								m_pMotionInfo[nNum][nCntMotion].aKeyInfo = new CMotion::KEY_INFO[m_pMotionInfo[nNum][nCntMotion].nKeyMax];	// メモリ確保

								for (int nCount = 0; nCount < m_pMotionInfo[nNum][nCntMotion].nKeyMax; nCount++)
								{// キーカウント
									m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCount].aKey = NULL;		// NULLへ
								}
							}
						}
						else if (memcmp(aStr, "BLEND_FRAME", strlen("BLEND_FRAME")) == 0)
						{// ブレンドフレーム
							pStrCut += strlen("BLEND_FRAME");								// ポインタを進める
							PopString(pStrCut, aStr);										// 文字変換
							m_pMotionInfo[nNum][nCntMotion].nBrendFrame = atoi(aStr);		// 整数型に変換
						}
						else if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
						{// キー設定
							nCntModel = 0;		// モデルカウンタ初期化

							if (nCntKey < m_pMotionInfo[nNum][nCntMotion].nKeyMax)
							{// キー最大数まで
								if (m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey == NULL)
								{// NULLの場合
									m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey = new CMotion::KEY[nModelMax];		// メモリ確保
								}
							}

							do
							{// ループ
								pStrCut = ReadFile(pFile, &aLine[0]);				// コメントチェック
								strcpy(aStr, pStrCut);								// 文字変換

								if (memcmp(aStr, "FRAME", strlen("FRAME")) == 0)
								{// フレーム
									pStrCut += strlen("FRAME");												// ポインタを進める
									PopString(pStrCut, aStr);												// 文字変換
									m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(aStr);	// 整数型に変換
								}
								else if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
								{// キーモデル設定
									do
									{// ループ
										pStrCut = ReadFile(pFile, &aLine[0]);		// コメントチェック
										strcpy(aStr, pStrCut);						// 文字変換

										if (memcmp(aStr, "POS", strlen("POS")) == 0)
										{// 位置
											pStrCut += strlen("POS");																		// ポインタを進める
											PopString(pStrCut, aStr);																		// 文字変換
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x = (float)atof(aStr);	// 整数型に変換
											PopString(pStrCut, aStr);																		// 文字変換
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y = (float)atof(aStr);	// 整数型に変換
											PopString(pStrCut, aStr);																		// 文字変換
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z = (float)atof(aStr);	// 整数型に変換
										}
										else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
										{// 向き
											pStrCut += strlen("ROT");																		// ポインタを進める
											PopString(pStrCut, aStr);																		// 文字変換
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x = (float)atof(aStr);	// 整数型に変換
											PopString(pStrCut, aStr);																		// 文字変換
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y = (float)atof(aStr);	// 整数型に変換
											PopString(pStrCut, aStr);																		// 文字変換
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z = (float)atof(aStr);	// 整数型に変換
										}
									} while (memcmp(aStr, "END_KEY", strlen("END_KEY")) != 0);
									nCntModel++;	// モデルカウンタ
								}
							} while (memcmp(aStr, "END_KEYSET", strlen("END_KEYSET")) != 0);
							nCntKey++;		// キーカウンタ
						}
					} while (memcmp(aStr, "END_MOTIONSET", strlen("END_MOTIONSET")) != 0);
					nCntMotion++;		// モーションカウンタ
				}
			} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
		}
		fclose(pFile);					// ファイルクローズ
	}

	*nMax = nMotionMax;		// モーション数を設定
}

//=============================================================================
// モデル数処理										(public)	*** CText ***
//=============================================================================
int CText::GetMaxModel(void)
{
	int nMaxModel = 0;

	if (m_pModelInfo != NULL)
	{// NULL以外の場合
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// テキスト数カウント
			if (m_Text[nCount].type == TYPE_MODEL)
			{// タイプがモデル
				FILE *pFile = NULL;					// ファイルポインタ

				pFile = fopen(m_Text[nCount].pFileName, "r");	// ファイルオープン

				if (pFile != NULL)
				{// NULL以外の場合
					int nCntModel = 0;				// モデルカウンタ

					char *pStrCut;					// 文字列の先頭へのポインタ
					char aLine[256];				// 文字列読み取り用（１行分）
					char aStr[256];					// 文字列抜き出し用

					pStrCut = ReadFile(pFile, &aLine[0]);				// コメントチェック
					strcpy(aStr, pStrCut);								// 文字変換

					if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
					{// 読み込み開始
						do
						{// ループ
							pStrCut = ReadFile(pFile, &aLine[0]);			// コメントチェック
							strcpy(aStr, pStrCut);							// 文字変換

							if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
							{// モデル数
								pStrCut += strlen("NUM_MODEL");						// ポインタを進める
								PopString(pStrCut, aStr);							// 文字変換
								nMaxModel += atoi(aStr);							// 整数型に変換

								break;
							}
						} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
					}
				}
			}
		}
	}

	return nMaxModel;
}

//=============================================================================
// モデルデータロード処理								(public)	*** CText ***
//=============================================================================
void CText::LoadXModelData(char *cName, int nNum)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	if (m_pModelStorage != NULL)
	{// NULL以外の場合
		if (m_pModelStorage[nNum].bLoad == false)
		{
			D3DXLoadMeshFromX(cName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pModelStorage[nNum].pBuffMat, NULL, &m_pModelStorage[nNum].nNumMat, &m_pModelStorage[nNum].pMesh);

			if (m_pModelStorage[nNum].pBuffMat != NULL)
			{// NULL以外の場合
				D3DXMATERIAL *pmat;								// マテリアルデータへのポインタ
				D3DMATERIAL9 *matDef;							// 現在のマテリアル保存用
				pmat = (D3DXMATERIAL*)m_pModelStorage[nNum].pBuffMat->GetBufferPointer();			// マテリアルを取得
				matDef = new D3DMATERIAL9[m_pModelStorage[nNum].nNumMat];			// マテリアル数のメモリ確保
				m_pModelStorage[nNum].pTexture = new LPDIRECT3DTEXTURE9[m_pModelStorage[nNum].nNumMat];	// マテリアル数のメモリ確保

				for (DWORD tex = 0; tex < m_pModelStorage[nNum].nNumMat; tex++)
				{// マテリアル数カウント
					matDef[tex] = pmat[tex].MatD3D;
					matDef[tex].Ambient = matDef[tex].Diffuse;
					m_pModelStorage[nNum].pTexture[tex] = NULL;

					if (pmat[tex].pTextureFilename != NULL && lstrlen(pmat[tex].pTextureFilename) > 0)
					{// テクスチャを使用している
						if (FAILED(D3DXCreateTextureFromFile(pDevice, pmat[tex].pTextureFilename, &m_pModelStorage[nNum].pTexture[tex])))
							MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
					}
				}

				delete[] matDef;	// メモリ開放
				matDef = NULL;		// NULLへ
			}

			m_pModelStorage[nNum].bLoad = true;
		}
	}
}