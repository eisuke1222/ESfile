//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "mesh3d.h"		// メッシュ3D

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MESHFIELD_TEXTURE	(2)			// テクスチャ最大数
#define MESHFIELD_SIZE_X		(30)		// 大きさ（X)
#define MESHFIELD_SIZE_Y		(30)		// 大きさ（Y)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshField : public CMesh3D
{// メッシュフィールド（親：CMesh3D）
public:
	typedef enum
	{// フィールドタイプ
		MESHFIELDTYPE_O = 0,		// 海
		MESHFIELDTYPE_G,			// 陸
		MESHFIELDTYPE_MAX
	}MESHFIELDTYPE;

	CMeshField();									// コンストラクタ
	~CMeshField();									// デストラクタ

	static HRESULT Load(void);						// ロード
	static void Unload(void);						// アンロード
	static CMeshField *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex);		// 生成

	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex);					// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_MESHFIELD_TEXTURE];		// テクスチャ情報へのポインタ

	static D3DXVECTOR2 m_posUV;						// UV位置
	MESHFIELDTYPE m_meshfieldType;					// フィールドタイプ

protected:

};

#endif