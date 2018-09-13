#ifndef	__C_COLLISION_MAP_MAKER__
#define	__C_COLLISION_MAP_MAKER__

//	Ŭ���� ����
struct	DxFrame;
class	DxFrameMesh;
class	CCollisionMapCell;

class	CCollisionMapMaker
{
public:
			CCollisionMapMaker ();
	virtual	~CCollisionMapMaker();

private:
	BOOL	GetAllDataFromDxFrame ( DxFrameMesh* pFrameMesh );
    
private:
	DWORD				m_nMapCells;
	CCollisionMapCell*	m_pMapCells;

	DWORD			m_nVertices;
	D3DXVECTOR3*	m_pVertices;

	DxFrame**	m_ppDxFrame;	//	�̰��� �������� m_nMapCells�� ���ƾ��Ѵ�.
	DWORD		m_nDxFrame;	
};

#endif	//	__C_COLLISION_MAP_MAKER__