//=============================================================================
//
// オブジェクト処理 [object3d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "model3d.h"	// モデル3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CObject3D : public CModel3D
{// オブジェクト（親：CModel3D）
public:
	typedef enum
	{// モデルタイプ
		OBJECTTYPE_O_G = 0,		// 海陸境界
		OBJECTTYPE_BILL_1,		// ビル1
		OBJECTTYPE_BILL_2,		// ビル2
		OBJECTTYPE_BILL_3,		// ビル3
		OBJECTTYPE_LIFE,		// 
		OBJECTTYPE_MAX
	}OBJECTTYPE;

	typedef enum
	{
		BREAKTYPE_NORMAL = 0,
		BREAKTYPE_FALL,
		BREAKTYPE_MAX
	}BREAKTYPE;

	CObject3D();									// コンストラクタ
	~CObject3D();									// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, OBJECTTYPE objectType);					// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	OBJECTTYPE GetobjectType(void) { return m_objectType; }			// オブジェクトタイプ取得

	bool HitDamage(int nDamage, BREAKTYPE breakType, float breakrot, int nShotIdx)
	{// ヒットダメージ
		if (m_bDelete != true)
		{// 破棄してなかったら
			m_nLife -= nDamage;						// 体力を減らす
			m_breakType = breakType;				// 破壊タイプ
			m_breakrot = breakrot;					// 破壊向き
			m_breakrotDiff = D3DX_PI * 0.8f;		// 破壊向き残り
			m_nShotIdx = nShotIdx;					// 発射ID

			if (m_nLife < 1) m_bDelete = true;		// 破棄開始
		}

		return m_bDelete;
	}

	bool GetDelete(void) { return m_bDelete; }		// 破壊済み

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[OBJECTTYPE_MAX];		// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh[OBJECTTYPE_MAX];			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[OBJECTTYPE_MAX];			// マテリアル情報へのポインタ
	static DWORD				m_nNumMat[OBJECTTYPE_MAX];			// マテリアル情報の数

	OBJECTTYPE	m_objectType;						// モデルタイプ
	BREAKTYPE	m_breakType;						// 破壊タイプ
	bool		m_bDelete;							// 破棄
	int			m_nLife;							// 体力
	int			m_nCntTimer;						// 時間カウンタ

	float		m_breakrot;							// 破壊向き
	float		m_breakrotDiff;						// 破壊向き残り
	int			m_nCntBreak;						// 破壊カウンタ

	int			m_nShotIdx;							// 発射ID

protected:

};

#endif