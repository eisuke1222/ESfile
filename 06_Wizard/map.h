//=============================================================================
//
// �}�b�v���� [map.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scenemesh.h"	// �V�[�����b�V��
#include "model.h"		// ���f��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene3D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMap : public CScene
{// �}�b�v�i�e�FCScene�j
public:
	const static int m_ROUTEPOS_NUM = 4;
	const static int m_GRASS_NUM = 4;
	const static int m_GRASS_LINE = 3;

	typedef enum
	{// ���
		TYPE_NO_00 = 0,	// �}�b�v_00
		TYPE_NO_01,		// �}�b�v_01
		TYPE_NO_02,		// �}�b�v_02
		TYPE_NO_03,		// �}�b�v_03
		TYPE_NO_04,		// �}�b�v_04
		TYPE_MAX		// �ő吔
	}TYPE;

	typedef enum
	{// �G���A
		AREA_START = 0,
		AREA_END,
		AREA_NORMAL,
		AREA_BATTLE,
		AREA_ITEM,
		AREA_MAX
	}AREA;

	typedef struct
	{// �G���A�͈�
		AREA		nArea;
		D3DXVECTOR3 pos;
		float		fRange;
	}AREA_RANGE;

	typedef struct
	{// ���[�g�͈�
		int nAreaNumStart;
		int nAreaNumEnd;
		D3DXVECTOR3 posCenter;
		D3DXVECTOR3 posStart;
		D3DXVECTOR3 posEnd;
		D3DXVECTOR3 pos[m_ROUTEPOS_NUM];
		D3DXVECTOR3 posCol[m_ROUTEPOS_NUM];
		float		fAngle;
		float		fLenght;
		float		fRange;
	}ROUTE_RANGE;

	typedef struct
	{// ���[�g�p�^�[��
		int nMaxPattern;
		int *nPattern;
		bool bStart;
		bool bEnd;
	}ROUTE_PATTERN;

	typedef struct
	{// ���[�v
		int nPattern_1;
		int nPattern_2;
		int nArea_1;
		int nArea_2;
	}WARP;

	typedef struct
	{// ��
		int nCounter;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 posCenter[m_GRASS_NUM][m_GRASS_NUM];
		D3DXVECTOR3 posVtx[m_GRASS_NUM][m_GRASS_NUM][2];
		CScene3D *pGrass[m_GRASS_NUM][m_GRASS_NUM];
		bool bFall[m_GRASS_NUM][m_GRASS_NUM];
		int nFallCounter[m_GRASS_NUM][m_GRASS_NUM];
	}GRASS;

	CMap();												// �R���X�g���N�^
	virtual ~CMap();									// �f�X�g���N�^

	static HRESULT Load(void);							// ���[�h
	static void Unload(void);							// �A�����[�h

	static CMap *Create(TYPE type);						// ����

	virtual HRESULT Init(void) = 0;						// ����������
	virtual void Uninit(void);							// �I������
	virtual void Update(void) = 0;						// �X�V����
	virtual void Draw(void) = 0;						// �`�揈��

private:

protected:

};

class CMap_00 : public CMap
{// �}�b�v_00�i�e�FCMap�j
public:
	typedef enum
	{// ��
		SKY_NO_0 = 0,	// ��_00
		SKY_NO_1,		// ��_01
		SKY_MAX			// �ő吔
	}SKY;

	CMap_00();											// �R���X�g���N�^
	~CMap_00();											// �f�X�g���N�^

	static HRESULT Load(void);							// ���[�h
	static void Unload(void);							// �A�����[�h

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

private:
	CSceneMesh *m_pField;								// �t�B�[���h
	CSceneMesh *m_pSky[SKY_MAX];						// ��

	D3DXVECTOR2 m_SkyRot[SKY_MAX];						// ���]

protected:

};

class CMap_01 : public CMap
{// �}�b�v_01�i�e�FCMap�j
public:
	const static int m_GRASS_MAX = 20;

	typedef enum
	{// ��
		SKY_NO_0 = 0,	// ��_00
		SKY_NO_1,		// ��_01
		SKY_MAX			// �ő吔
	}SKY;

	CMap_01();											// �R���X�g���N�^
	~CMap_01();											// �f�X�g���N�^

	static HRESULT Load(void);							// ���[�h
	static void Unload(void);							// �A�����[�h

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

private:
	static int					m_nMaxModel;			// ���f����

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh;				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_nNumMat;				// �}�e���A�����̐�

	static CModel::MODEL_INFO	*m_ModelInfo;			// ���f���C���t�H

	CModel						*m_pModel;				// ���f��

	CSceneMesh					*m_pField;				// �t�B�[���h
	CSceneMesh					*m_pSky[SKY_MAX];		// ��

	GRASS						*m_pGrass;				// ��

	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// ���]

protected:

};

class CMap_02 : public CMap
{// �}�b�v_02�i�e�FCMap�j
public:
	const static int m_WARPCYLINDER_NUM = 6;

	typedef enum
	{// ��
		SKY_NO_0 = 0,	// ��_00
		SKY_NO_1,		// ��_01
		SKY_MAX			// �ő吔
	}SKY;

	CMap_02();											// �R���X�g���N�^
	~CMap_02();											// �f�X�g���N�^

	static HRESULT Load(void);							// ���[�h
	static void Unload(void);							// �A�����[�h

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	void SetField(D3DXVECTOR3 pos);						// �t�B�[���h�ݒ�
	void SetObject(void);								// �I�u�W�F�N�g�ݒ�
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange);
	bool CollisionInOut(D3DXVECTOR3 pos);
	bool CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp);
	bool CollisionGoal(D3DXVECTOR3 *pPos);
	bool CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

private:
	static int					m_nMaxModel;			// ���f����

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh;				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_nNumMat;				// �}�e���A�����̐�

	static CModel::MODEL_INFO	*m_ModelInfo;			// ���f���C���t�H

	int							m_nModelWarp;			// ���f�����[�v��
	CModel						*m_pModelWarp;			// ���f�����[�v

	int							m_nModelObject;			// ���f���I�u�W�F�N�g��
	CModel						*m_pModelObject;		// ���f���I�u�W�F�N�g

	CSceneMesh					*m_pField;				// �t�B�[���h
	CSceneMesh					*m_pSky[SKY_MAX];		// ��

	int							m_nGrassMax;			// �ő呐
	GRASS						*m_pGrass;				// ��
	
	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// ���]

	int							m_nAreaMax;				// �G���A�ő吔
	int							m_nAreaNum[AREA_MAX];	// �G���A��
	AREA_RANGE					*m_pArea;				// �G���A

	int							m_nRouteMax;				// ���[�g�ő吔
	int							m_nRouteNum[AREA_MAX];	// ���[�g��
	ROUTE_RANGE					*m_pRoute;				// ���[�g

	int							m_nPatternMax;			// �p�^�[���ő吔
	ROUTE_PATTERN				*m_pPattern;			// �p�^�[��

	WARP						*m_pWarp;				// ���[�v
	//CSceneMesh					*m_pWarpCylinder;		// ���[�v�V�����_�[
	CSceneMesh					*m_pWarpCylinder[m_WARPCYLINDER_NUM];		// ���[�v�V�����_�[
	D3DXVECTOR2					m_WarpCylinderUV;		// ���[�v�V�����_�[UV

	CSceneMesh					*m_pGoalCylinder;		// �S�[���V�����_�[

	bool						m_bDrawIcon;			// �A�C�R���`��
	CScene3D					*m_pIcon;				// �A�C�R��

protected:

};

class CMap_03 : public CMap
{// �}�b�v_03�i�e�FCMap�j
public:
	const static int m_WARPCYLINDER_NUM = 6;

	typedef enum
	{// ��
		SKY_NO_0 = 0,	// ��_00
		SKY_NO_1,		// ��_01
		SKY_MAX			// �ő吔
	}SKY;

	CMap_03();											// �R���X�g���N�^
	~CMap_03();											// �f�X�g���N�^

	static HRESULT Load(void);							// ���[�h
	static void Unload(void);							// �A�����[�h

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	void SetField(D3DXVECTOR3 pos);						// �t�B�[���h�ݒ�
	void SetObject(void);								// �I�u�W�F�N�g�ݒ�
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange);
	bool CollisionInOut(D3DXVECTOR3 pos);
	bool CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp);
	bool CollisionGoal(D3DXVECTOR3 *pPos);
	bool CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

private:
	static int					m_nMaxModel;			// ���f����

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh;				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_nNumMat;				// �}�e���A�����̐�

	static CModel::MODEL_INFO	*m_ModelInfo;			// ���f���C���t�H

	int							m_nModelWarp;			// ���f�����[�v��
	CModel						*m_pModelWarp;			// ���f�����[�v

	int							m_nModelObject;			// ���f���I�u�W�F�N�g��
	CModel						*m_pModelObject;		// ���f���I�u�W�F�N�g

	CSceneMesh					*m_pField;				// �t�B�[���h
	CSceneMesh					*m_pSky[SKY_MAX];		// ��

	int							m_nGrassMax;			// �ő呐
	GRASS						*m_pGrass;				// ��

	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// ���]

	int							m_nAreaMax;				// �G���A�ő吔
	int							m_nAreaNum[AREA_MAX];	// �G���A��
	AREA_RANGE					*m_pArea;				// �G���A

	int							m_nRouteMax;				// ���[�g�ő吔
	int							m_nRouteNum[AREA_MAX];	// ���[�g��
	ROUTE_RANGE					*m_pRoute;				// ���[�g

	int							m_nPatternMax;			// �p�^�[���ő吔
	ROUTE_PATTERN				*m_pPattern;			// �p�^�[��

	WARP						*m_pWarp;				// ���[�v
	//CSceneMesh					*m_pWarpCylinder;		// ���[�v�V�����_�[
	CSceneMesh					*m_pWarpCylinder[m_WARPCYLINDER_NUM];		// ���[�v�V�����_�[
	D3DXVECTOR2					m_WarpCylinderUV;		// ���[�v�V�����_�[UV

	CSceneMesh					*m_pGoalCylinder;		// �S�[���V�����_�[

	bool						m_bDrawIcon;			// �A�C�R���`��
	CScene3D					*m_pIcon;				// �A�C�R��

protected:

};

class CMap_04 : public CMap
{// �}�b�v_04�i�e�FCMap�j
public:
	const static int m_WARPCYLINDER_NUM = 6;

	typedef enum
	{// ��
		SKY_NO_0 = 0,	// ��_00
		SKY_NO_1,		// ��_01
		SKY_MAX			// �ő吔
	}SKY;

	CMap_04();											// �R���X�g���N�^
	~CMap_04();											// �f�X�g���N�^

	static HRESULT Load(void);							// ���[�h
	static void Unload(void);							// �A�����[�h

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	void SetField(D3DXVECTOR3 pos);						// �t�B�[���h�ݒ�
	void SetObject(void);								// �I�u�W�F�N�g�ݒ�
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, float fRange);
	bool CollisionInOut(D3DXVECTOR3 pos);
	bool CollisionWarp(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosWarp);
	bool CollisionGoal(D3DXVECTOR3 *pPos);
	bool CollisionCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

	void SetGoal(bool bGoal)
		{ m_bGoal = bGoal; }
	bool GetGoal(void)
		{ return m_bGoal; }

private:
	static int					m_nMaxModel;				// ���f����

	static LPDIRECT3DTEXTURE9	**m_pTexture;			// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh;				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_nNumMat;				// �}�e���A�����̐�

	static CModel::MODEL_INFO	*m_ModelInfo;			// ���f���C���t�H

	int							m_nModelWarp;			// ���f�����[�v��
	CModel						*m_pModelWarp;			// ���f�����[�v

	int							m_nModelObject;			// ���f���I�u�W�F�N�g��
	CModel						*m_pModelObject;		// ���f���I�u�W�F�N�g

	CSceneMesh					*m_pField;				// �t�B�[���h
	CSceneMesh					*m_pSky[SKY_MAX];		// ��

	int							m_nGrassMax;			// �ő呐
	GRASS						*m_pGrass;				// ��

	D3DXVECTOR2					m_SkyRot[SKY_MAX];		// ���]

	int							m_nAreaMax;				// �G���A�ő吔
	int							m_nAreaNum[AREA_MAX];	// �G���A��
	AREA_RANGE					*m_pArea;				// �G���A

	int							m_nRouteMax;				// ���[�g�ő吔
	int							m_nRouteNum[AREA_MAX];	// ���[�g��
	ROUTE_RANGE					*m_pRoute;				// ���[�g

	int							m_nPatternMax;			// �p�^�[���ő吔
	ROUTE_PATTERN				*m_pPattern;			// �p�^�[��

	WARP						*m_pWarp;				// ���[�v
	//CSceneMesh					*m_pWarpCylinder;		// ���[�v�V�����_�[
	CSceneMesh					*m_pWarpCylinder[m_WARPCYLINDER_NUM];		// ���[�v�V�����_�[
	D3DXVECTOR2					m_WarpCylinderUV;		// ���[�v�V�����_�[UV

	bool						m_bGoal;				// �S�[���ł��邩
	CSceneMesh					*m_pGoalCylinder;		// �S�[���V�����_�[

	bool						m_bDrawIcon;			// �A�C�R���`��
	CScene3D					*m_pIcon;				// �A�C�R��

protected:

};

#endif