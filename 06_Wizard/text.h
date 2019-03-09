//=============================================================================
//
// テキスト処理 [text.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TEXT_H_
#define _TEXT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "model.h"		// モデル
#include "motion.h"		// モーション

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CText
{// テキスト
public:
	typedef enum
	{// タイプ
		TYPE_MODEL = 0,				// モデル
		TYPE_MOTION,				// モーション
		TYPE_MESHSKIN,				// スキン
		TYPE_MESHFIELD,				// フィールド
		TYPE_MAX					// 最大数
	}TYPE;

	typedef struct
	{// テキスト
		int		nIdx;				// テキスト番号
		TYPE	type;				// タイプ
		int		nTypeNum;			// タイプ番号
		char	pFileName[256];		// ファイル名
		int		nMax;				// 最大値
		bool	bLoad;				// ロードしたか
	}TEXT;

	typedef struct
	{// モデル
		char				pFileName[256];		// ファイル名
		LPDIRECT3DTEXTURE9 *pTexture;			// テクスチャ情報へのポインタ
		LPD3DXMESH			pMesh;				// メッシュ情報へのポインタ
		LPD3DXBUFFER		pBuffMat;			// マテリアル情報へのポインタ
		DWORD				nNumMat;			// マテリアル情報の数
		bool				bLoad;				// ロードしたか
	}MODEL_STORAGE;

	CText();														// コンストラクタ
	~CText();														// デストラクタ

	static HRESULT Load(void);										// ロード
	static void Unload(void);										// アンロード

	static char *ReadFile(FILE *pFile, char *pDest);				// ファイル読み込み
	static void PopString(char *pStrCur, char *pStr);				// 読み込み調整

	static CModel::MODEL_INFO	*GetModel(char *cName, int *nMax);	// モデル取得
	static CMotion::MOTION_INFO	*GetMotion(char *cName);			// モーション取得

																	// モデルデータ取得
	static void GetXModelData(char *cName, LPDIRECT3DTEXTURE9 **pTexture, LPD3DXMESH *pMesh, LPD3DXBUFFER *pBuffMat, DWORD *nNumMat);
	static void ReleaseXModelData(void);							// モデルデータ開放

private:
	static void LoadModelText(char *cName, int *nMax, int nNum);	// モデルロード
	static void LoadMotionText(char *cName, int *nMax, int nNum);	// モーションロード

	static int GetMaxModel(void);									// モデル数取得
	static void LoadXModelData(char *cName, int nNum);				// モデルデータロード

	static int					m_nMaxText;							// テキスト数
	static int					m_nMaxType[TYPE_MAX];				// タイプ別テキスト数

	static TEXT					*m_Text;							// テキスト
	static CModel::MODEL_INFO	**m_pModelInfo;						// モデル
	static CMotion::MOTION_INFO	**m_pMotionInfo;					// モーション

	static int					m_nMaxModel;						// モデル数
	static MODEL_STORAGE		*m_pModelStorage;					// モデル保管

protected:

};

#endif