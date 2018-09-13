#include "stdafx.h"
#include "DxOctreeCollisionMan.h"
#include "DxOctree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

///////////////////////////////////////////////////////////////////////////////
//	class	DxOctreeCollisoin	--	Begin
///////////////////////////////////////////////////////////////////////////////
DxOctreeCollision::DxOctreeCollision () :
	m_vMax ( -FLT_MAX, -FLT_MAX, -FLT_MAX ),
    m_vMin (  FLT_MAX,  FLT_MAX,  FLT_MAX ),
	m_bSubDivided ( TRUE ),
	m_pVertexArray ( NULL ),
	m_nVertexArray ( 0 )
{
	for ( int i = 0; i < 8; i++ )
	{
		m_pNodes[i] = NULL;
	}
}

DxOctreeCollision::~DxOctreeCollision()
{
	DelVertexArray ();
	for ( int i = 0; i < 8; i++ )
	{
		DelChild ( i );
	}
}

BOOL DxOctreeCollision::OneTimeSceneInit ()
{
	m_vMax = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	m_vMin = D3DXVECTOR3 (  FLT_MAX,  FLT_MAX,  FLT_MAX );
	m_bSubDivided = TRUE;
	DelVertexArray ();

	for ( int i = 0; i < 8; i++ )
	{
		m_pNodes[i] = NULL;
	}

	return TRUE;
}

BOOL DxOctreeCollision::FinalCleanup ()
{
	m_vMax = D3DXVECTOR3 ( -FLT_MAX, -FLT_MAX, -FLT_MAX );
	m_vMin = D3DXVECTOR3 (  FLT_MAX,  FLT_MAX,  FLT_MAX );
	m_bSubDivided = TRUE;
	DelVertexArray ();
	for ( int i = 0; i < 8; i++ )
	{
		DelChild ( i );
	}

	return TRUE;
}

BOOL DxOctreeCollision::SetVertexArray ( D3DXVECTOR3* pVertexArray, DWORD nVertexArray )
{
	if ( m_pVertexArray || m_nVertexArray )
	{
		_ASSERT ( 0 && "�̹� ���ؽ��� ��� �ֽ��ϴ�." );
		return FALSE;
	}

	if ( !pVertexArray || ( nVertexArray % 3 ) )
	{
		_ASSERT ( 0 && "����Ÿ�� �����ϴ�." );
		return FALSE;
	}

	m_nVertexArray = nVertexArray;
	m_pVertexArray = new D3DXVECTOR3 [ m_nVertexArray ];
	memcpy ( m_pVertexArray, pVertexArray, sizeof ( D3DXVECTOR3 ) * m_nVertexArray );

    return TRUE;
}

BOOL DxOctreeCollision::GetVertexArray ( D3DXVECTOR3* pVertexArray, DWORD* pnVertexArray ) const
{
	if ( !m_pVertexArray || ( m_nVertexArray % 3 ) )
	{
		_ASSERT ( 0 && "����Ÿ�� �����ϴ�." );
		return FALSE;
	}

	if ( !pVertexArray )
	{
		*pnVertexArray = m_nVertexArray;
		return FALSE;
	}

	if ( m_nVertexArray < *pnVertexArray )
	{
		_ASSERT ( 0 && "���� ����Ÿ �̻����� �������ϰ� �ֽ��ϴ�." );
		return FALSE;
	}

	memcpy ( pVertexArray, m_pVertexArray, sizeof ( D3DXVECTOR3 ) * *pnVertexArray );

	return TRUE;
}

BOOL DxOctreeCollision::DelVertexArray ()
{
	SAFE_DELETE_ARRAY ( m_pVertexArray );
	m_nVertexArray = 0;

	return TRUE;
}

BOOL DxOctreeCollision::NewChild ( int nIndex )
{
	if ( nIndex < 0 || 7 < nIndex )
	{
		_ASSERT ( 0 && "�߸��� �ε����Դϴ�." );
		return FALSE;
	}

	if ( m_pNodes[nIndex] )
	{
		_ASSERT ( 0 && "�̹� ������� ����Դϴ�." );
		return FALSE;
	}

	m_pNodes[nIndex] = new DxOctreeCollision;

	return TRUE;
}

BOOL DxOctreeCollision::DelChild ( int nIndex )
{
	if ( nIndex < 0 || 7 < nIndex )
	{
		_ASSERT ( 0 && "�߸��� �ε����Դϴ�." );
		return FALSE;
	}

	SAFE_DELETE ( m_pNodes[nIndex] );

	return TRUE;
}

DxOctreeCollision* DxOctreeCollision::GetChild ( int nIndex )
{
	if ( nIndex < 0 || 7 < nIndex )
	{
		_ASSERT ( 0 && "�߸��� �ε����Դϴ�." );
		return FALSE;
	}

	return m_pNodes[nIndex];
}

void DxOctreeCollision::SetMinMaxPoint ( D3DXVECTOR3 vMin, D3DXVECTOR3 vMax )
{
	m_vMin = vMin;
	m_vMax = vMax;
}

void DxOctreeCollision::GetMinMaxPoint ( D3DXVECTOR3* pvMin, D3DXVECTOR3* pvMax )
{
	if ( !pvMin || !pvMax )
	{
		_ASSERT ( 0 && "NULL�� �Ѿ�Խ��ϴ�." );
		return;
	}

	*pvMin = m_vMin;
	*pvMax = m_vMax;
}

void DxOctreeCollision::SetSubDivided	()
{
	m_bSubDivided = TRUE;
}

void DxOctreeCollision::ResetSubDivided	()
{
	m_bSubDivided = FALSE;
}

BOOL DxOctreeCollision::IsSubDivided	()
{
	return m_bSubDivided;
}

///////////////////////////////////////////////////////////////////////////////
//	class	DxOctreeCollisoin	--	End
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//	class	DxOctreeCollisionMan	--	Begin
///////////////////////////////////////////////////////////////////////////////
DxOctreeCollisionMan::DxOctreeCollisionMan () :
	m_pRootNode ( NULL )
{
}

DxOctreeCollisionMan::~DxOctreeCollisionMan()
{
	SAFE_DELETE ( m_pRootNode );
}

BOOL DxOctreeCollisionMan::Import ( DxOctree* pOctree )
{
/*
	if ( !pOctree )
	{
		_ASSERT ( 0 && "����Ÿ ������ �߸��Ǿ����ϴ�." );
		return FALSE;
	}

	if ( !ExtractVertexFromOctree ( pOctree, m_pRootNode ) )
	{
		_ASSERT ( 0 && "��Ʈ���κ��� �������⿡ �����Ͽ����ϴ�." );
		return FALSE;
	}

	if ( !CheckIntegrity ( pOctree ) )
	{
		return FALSE;
	}    
*/
	return TRUE;
}

BOOL DxOctreeCollisionMan::OneTimeSceneInit ()
{
	m_pRootNode = new DxOctreeCollision;

	return TRUE;
}

BOOL DxOctreeCollisionMan::FinalCleanup ()
{
	SAFE_DELETE ( m_pRootNode );

	return TRUE;
}

BOOL DxOctreeCollisionMan::ExtractVertexFromOctree ( DxOctree* pOctree, DxOctreeCollision*	pRootNode )
{
	if ( !pOctree || !pRootNode )
	{
		_ASSERT ( 0 && "����Ÿ ������ �߸��Ǿ����ϴ�." );
		return FALSE;
	}

	//	Note : �⺻ ����Ÿ ����
	//
	pRootNode->SetSubDivided ();
	pRootNode->SetMinMaxPoint ( pOctree->GetMin(), pOctree->GetMax () );

	//	Note : ������� ����Ÿ ����
	//
	if ( !pOctree->IsSubDivision () )
	{
		pRootNode->ResetSubDivided ();
		return GetNodeVertex ( pOctree->GetDxFrameHead(), pRootNode );
	}

	for ( int i = 0; i < 8; i++ )
	{
		if ( pOctree->GetOctreeNode ( i ) )
		{
			//	���ϰ��� üũ���� �ʾƵ� �������.
			//	�ֳ��ϸ�, ���ýú� �Լ��̱� ������,
			//	�ٸ� �κп��� ��� üũ�� �����Ѵ�.
			if ( !pRootNode->NewChild ( i ) )
			{
				_ASSERT ( 0 && "���ο� �ڽ��� ����µ� �����Ͽ����ϴ�." );
				return FALSE;
			}
			ExtractVertexFromOctree ( pOctree->GetOctreeNode ( i ), pRootNode->GetChild ( i ) );
		}	
	}

	return TRUE;
}

BOOL DxOctreeCollisionMan::GetNodeVertex ( DxFrame* pDxFrame, DxOctreeCollision* pNode )
{
	if ( !pDxFrame )
	{
		_ASSERT ( 0 && "����Ÿ ������ �߸��Ǿ����ϴ�." );
		return FALSE;
	}

	DWORD dwIndices = 0x00000000;
	DWORD dwVertices = 0x00000000;

	pDxFrame->GetDataFromOctreeMesh ( NULL, &dwIndices, NULL, &dwVertices );	

	if ( !dwIndices || !dwVertices )
	{
		_ASSERT ( 0 && "�ε����� ���ؽ��� ��� �ֽ��ϴ�." );
		return FALSE;
	}

	DWORD* pIndices = new DWORD [ dwIndices ];
	OCTREEVERTEX* pVertices = new OCTREEVERTEX [ dwVertices ];

	dwIndices = 0x00000000;
	dwVertices = 0x00000000;
    pDxFrame->GetDataFromOctreeMesh ( pIndices, &dwIndices, pVertices, &dwVertices );

	//////////////////////////////////////////////////////////////////////////
	//	octreevertex --> d3dxvertex3
	//
	D3DXVECTOR3* pVertexArray = new D3DXVECTOR3 [ dwIndices ];
	for ( DWORD i = 0; i < dwIndices; i++ )
	{
		pVertexArray[i] = pVertices[pIndices[i]].vPos;
	}

	if ( !pNode->SetVertexArray ( pVertexArray, dwIndices ) )
	{
		SAFE_DELETE_ARRAY ( pVertexArray );
		return FALSE;
	}
	SAFE_DELETE_ARRAY ( pVertexArray );
	//////////////////////////////////////////////////////////////////////////
        
	SAFE_DELETE_ARRAY ( pIndices );
	SAFE_DELETE_ARRAY ( pVertices );

	return TRUE;
}

BOOL DxOctreeCollisionMan::CheckIntegrity ( DxOctree* pOctree )
{
	if ( !pOctree )
	{
		_ASSERT ( 0 && "����Ÿ ������ �߸��Ǿ����ϴ�." );
		return FALSE;
	}

	DWORD OriginNodeCount, OriginIndicesCount, OriginVerticesCount;
	DWORD ConvertNodeCount, ConvertIndicesCount, ConvertVerticesCount;

	OriginNodeCount = ConvertNodeCount = OriginIndicesCount = ConvertIndicesCount = OriginVerticesCount = ConvertVerticesCount = 0;
	if ( !GetOctreeMeshInfo ( pOctree, &OriginNodeCount, &OriginIndicesCount, &OriginVerticesCount ) )
	{
		return FALSE;
	}
	if ( !GetOctreeCollisioinInfo ( m_pRootNode, &ConvertNodeCount, &ConvertIndicesCount, &ConvertVerticesCount ) )
	{
		return FALSE;
	}

	if ( OriginNodeCount != ConvertNodeCount &&
		 OriginIndicesCount != ConvertVerticesCount )
	{
		_ASSERT ( 0 && "����Ÿ�� �߸� ����Ǿ����ϴ�." );
		return FALSE;
	}

	return TRUE;
}

BOOL DxOctreeCollisionMan::GetOctreeMeshInfo ( DxOctree* pOctree, DWORD* pNodeCount, DWORD* pIndicesCount, DWORD* pVerticesCount )
{
	if ( !pOctree )
	{
		_ASSERT ( 0 && "����Ÿ ������ �߸��Ǿ����ϴ�." );
		return FALSE;
	}

	if ( !pOctree->IsSubDivision () )
	{
		//	���� �ܱ�
		pOctree->GetDxFrameHead()->GetAllNodeData ( NULL, pIndicesCount, NULL, pVerticesCount );
		(*pNodeCount)++;
		return TRUE;
	}

	for ( int i = 0; i < 8; i++ )
	{
		if ( pOctree->GetOctreeNode ( i ) )
		{
			GetOctreeMeshInfo ( pOctree->GetOctreeNode ( i ), pNodeCount, pIndicesCount, pVerticesCount );
		}	
	}

	return TRUE;
}

BOOL DxOctreeCollisionMan::GetOctreeCollisioinInfo ( DxOctreeCollision* pNode, DWORD* pNodeCount, DWORD* pIndicesCount, DWORD* pVerticesCount )
{
	if ( !pNode )
	{
		_ASSERT ( 0 && "����Ÿ ������ �߸��Ǿ����ϴ�." );
		return FALSE;
	}

	if ( !pNode->IsSubDivided () )
	{
		DWORD nVertexArray = 0;
		if ( pNode->GetVertexArray ( NULL, &nVertexArray ) )
		{
			return FALSE;
		}

		//	���� �ܱ�
		(*pVerticesCount) += nVertexArray;
		(*pNodeCount)++;
		return TRUE;
	}

	for ( int i = 0; i < 8; i++ )
	{
		if ( pNode->GetChild ( i ) )
		{
			GetOctreeCollisioinInfo ( pNode->GetChild ( i ), pNodeCount, pIndicesCount, pVerticesCount );
		}	
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	class	DxOctreeCollisionMan	--	End
///////////////////////////////////////////////////////////////////////////////