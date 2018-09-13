#include "stdafx.h"
#include "CollisionMapMaker.h"
#include "CollisionMapCell.h"
#include "DxFrameMesh.h"


CCollisionMapMaker::CCollisionMapMaker () :
	m_nMapCells ( 0 ),
	m_pMapCells ( NULL ),
	m_nVertices ( 0 ),
	m_pVertices ( NULL ),
	m_nDxFrame ( 0 ),
	m_ppDxFrame ( NULL )
{
}

CCollisionMapMaker::~CCollisionMapMaker()
{
	SAFE_DELETE_ARRAY ( m_pMapCells );
	SAFE_DELETE_ARRAY ( m_pVertices );
	SAFE_DELETE_ARRAY ( m_ppDxFrame );

    m_nMapCells = 0;
	m_nVertices = 0;
	m_nDxFrame = 0;
}

BOOL CCollisionMapMaker::GetAllDataFromDxFrame ( DxFrameMesh* pFrameMesh )
{
	D3DXVECTOR3		dwTreeCenter	=	pFrameMesh->GetTreeCenter();
	DWORD			dwTriangles		=	pFrameMesh->GetTriangles();
	DWORD			dwVertices		=	pFrameMesh->GetVertices();	

	DWORD			*pIndices = new DWORD [ dwTriangles * 3 ];
	DWORD			dwIndexCount = 0;
	memset ( pIndices, 0, dwTriangles * 3 * sizeof ( DWORD ) );

	OCTREEVERTEX	*pVertices = new OCTREEVERTEX [ dwVertices ];
	DWORD			dwVertexCount = 0;
	memset ( pVertices, 0, dwVertices * sizeof ( OCTREEVERTEX ) );	
	
	m_nDxFrame = 0;
	m_ppDxFrame = new DxFrame *[ dwTriangles ];

	//	Note	:	����Ÿ �ܾ����.	
	pFrameMesh->GetFrameRoot()->GetAllNodeData( pIndices, &dwIndexCount,
		pVertices, &dwVertexCount, m_ppDxFrame, &m_nDxFrame, pFrameMesh->GetFrameRoot() );	

	///////////////////////////////////////////////////////////////////////////////
	//	���� ����Ÿ �����
	m_nMapCells = dwTriangles;
	m_pMapCells = new CCollisionMapCell [ m_nMapCells ];	

	m_nVertices = dwVertices;
	m_pVertices = new D3DXVECTOR3 [ m_nVertices ];
	for ( DWORD i = 0; i < m_nVertices; i++ )
	{
		m_pVertices[i] = pVertices[i].vPos;
	}
	///////////////////////////////////////////////////////////////////////////////

	//	AABB Ʈ�� ����, ȭ �Ϸ�
	//

	//	������� AABB�� �̿���, Octree �ۼ�.. �� �Ϸ�
	//
    
	SAFE_DELETE_ARRAY ( pIndices );
	SAFE_DELETE_ARRAY ( pVertices );

	return 0;
}