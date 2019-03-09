//=============================================================================
//
// 敵(巨人)処理 [enemy.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン
#include "motion.h"		// モーション

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;			// モデル
class CMotion;			// モーション

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CScene
{// 敵（親：CScene）
public:
	typedef struct
	{// モデル情報
		char cFileName[256];		// ファイル名
		int nIndex;					// ID番号
		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR3 rot;			// 角度
	}MODEL_INFO;

	CEnemy();										// コンストラクタ
	~CEnemy();										// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// 生成

	HRESULT Init(void);											// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	// 位置（設定、取得）
	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 Getpos(void) { return m_pos; }

	// 敵の当たり判定
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);

	// ファイル読み込み
	static char *ReadFile(FILE *pFile, char *pDest)
	{
		fgets(pDest, 256, pFile);

		if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
		{
			ReadFile(pFile, pDest);
		}
		else
		{
			do
			{
				if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
				{
					ReadFile(pFile, pDest);
				}
				else if (memcmp(pDest, " ", strlen(" ")) == 0)
				{
					wsprintf(&pDest[0], &pDest[strlen(" ")]);
				}
				else if (memcmp(pDest, "\t", strlen("\t")) == 0)
				{
					wsprintf(&pDest[0], &pDest[strlen("\t")]);
				}
				else if (memcmp(pDest, "　", strlen("　")) == 0)
				{
					wsprintf(&pDest[0], &pDest[strlen("　")]);
				}
			} while (
				memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0 ||
				memcmp(pDest, " ", strlen(" ")) == 0 || memcmp(pDest, "\t", strlen("\t")) == 0 ||
				memcmp(pDest, "　", strlen("　")) == 0);
		}

		return pDest;
	}
	static void PopString(char *pStrCur, char *pStr)
	{
		do
		{
			if (memcmp(pStrCur, "=", strlen("=")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen("=")]);
			}
			else if (memcmp(pStrCur, " ", strlen(" ")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen(" ")]);
			}
			else if (memcmp(pStrCur, "\t", strlen("\t")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen("\t")]);
			}
			else if (memcmp(pStrCur, "　", strlen("　")) == 0)
			{
				wsprintf(&pStrCur[0], &pStrCur[strlen("　")]);
			}
		} while (
			memcmp(pStrCur, "=", strlen("=")) == 0 ||
			memcmp(pStrCur, " ", strlen(" ")) == 0 || memcmp(pStrCur, "\t", strlen("\t")) == 0 ||
			memcmp(pStrCur, "　", strlen("　")) == 0);

		int nCntEnd = 0;
		strcpy(pStr, pStrCur);

		do
		{
			if (memcmp(pStr, "#", strlen("#")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("#")]);
			}
			else if (memcmp(pStr, "\n", strlen("\n")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("\n")]);
			}
			else if (memcmp(pStr, " ", strlen(" ")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen(" ")]);
			}
			else if (memcmp(pStr, "\t", strlen("\t")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("\t")]);
			}
			else if (memcmp(pStr, "　", strlen("　")) == 0)
			{
				wsprintf(&pStr[0], &pStr[strlen("　")]);
			}
			else
			{
				wsprintf(&pStr[0], &pStr[1]);
			}

			nCntEnd++;
		} while (
			memcmp(pStr, "#", strlen("#")) != 0 && memcmp(pStr, "\n", strlen("\n")) != 0 &&
			memcmp(pStr, " ", strlen(" ")) != 0 && memcmp(pStr, "\t", strlen("\t")) != 0 &&
			memcmp(pStr, "　", strlen("　")) != 0);

		strcpy(pStr, pStrCur);
		wsprintf(&pStrCur[0], &pStrCur[nCntEnd]);
		pStr[nCntEnd] = '\0';
	}

private:
	static int					m_MaxModel;			// モデル数
	static int					m_MaxAnim;			// アニメーション数

	static LPDIRECT3DTEXTURE9	**m_pTexture;		// テクスチャ情報へのポインタ
	static LPD3DXMESH			*m_pMesh;			// メッシュ情報へのポインタ
	static LPD3DXBUFFER			*m_pBuffMat;		// マテリアル情報へのポインタ
	static DWORD				*m_nNumMat;			// マテリアル情報の数

	static MODEL_INFO			*m_ModelInfo;
	static CMotion::ANIM_INFO	*m_AnimInfo;

	D3DXMATRIX					m_mtxWorld;			// ワールドマトリックス
	CModel						*m_pModel;			// モデル
	CMotion						*m_pMotion;			// モーション

	bool						m_bStart;			// スタート
	int							m_nCntStart;

	D3DXVECTOR3					m_vtxMax;			// 大きさ（最大）
	D3DXVECTOR3					m_vtxMin;			// 大きさ（最小）
	D3DXVECTOR3					m_pos;				// 位置現在
	D3DXVECTOR3					m_posOld;			// 位置前回
	D3DXVECTOR3					m_rot;				// 向き
	D3DXVECTOR3					m_move;				// 移動量
	float						m_frotDest;			// 目標向き
	bool						m_bJump;			// ジャンプ
	bool						m_bSlip;			// 滑る
	float						m_fSliprot;			// 滑る角度
	float						m_fDownSpeed;		// 減速速度
	int							m_nWallJump;		// 壁キックカウント
	bool						m_bWallJump;		// 壁キックしているか
	float						m_bWallJumpmove;	// 壁キック移動量

	bool						m_bmoveUp;			// アイテムでスピードが上がっていいるかどうか
	int							m_nMoveCounter;		// スピードUPする時間のカウンター
	FLOAT						m_fScaleX = 1.0f, m_fScaleY = 1.0f, m_fScaleZ = 1.0f;   //拡大縮小用変数

	float						m_shadowHeight;		// 影高さ

protected:

};

#endif