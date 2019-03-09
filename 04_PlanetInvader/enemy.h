//=============================================================================
//
// 敵処理 [enemy.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
class CEnemy : public CModel3D
{// 敵（親：CModel3D）
public:
	typedef enum
	{// 敵タイプ
		ENEMYTYPE_S_1 = 0,		// 空01
		ENEMYTYPE_S_2,			// 空02
		ENEMYTYPE_G_1,			// 陸01
		ENEMYTYPE_G_2,			// 陸02
		ENEMYTYPE_O_1,			// 海01
		ENEMYTYPE_O_2,			// 海02
		ENEMYTYPE_B_1,			// 海02
		ENEMYTYPE_B_2,			// 海02
		ENEMYTYPE_B_3,			// 海02
		ENEMYTYPE_MAX
	}ENEMYTYPE;

	CEnemy();									// コンストラクタ
	~CEnemy();									// デストラクタ

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming);	// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming);			// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	bool HitDamage(int nDamage) { m_nLife -= nDamage; if (m_nLife < 1)return true; return false; }
	ENEMYTYPE GetenemyType(void) { return m_enemyType; }

private:
	static LPDIRECT3DTEXTURE9			*m_pTexture[ENEMYTYPE_MAX];			// テクスチャ情報へのポインタ
	static LPD3DXMESH					m_pMesh[ENEMYTYPE_MAX];				// メッシュ情報へのポインタ
	static LPD3DXBUFFER					m_pBuffMat[ENEMYTYPE_MAX];			// マテリアル情報へのポインタ
	static DWORD						m_nNumMat[ENEMYTYPE_MAX];			// マテリアル情報の数

	ENEMYTYPE m_enemyType;						// 種類
	D3DXVECTOR3 m_pos;							// 位置
	D3DXVECTOR3 m_move;							// 移動量
	int m_nCntTimer;							// 時間カウンタ
	int m_nLife;								// 体力
	int m_nShotTiming;							// 発射タイミング

	int m_nNumPattern;							// パターン番号

protected:

};

#endif