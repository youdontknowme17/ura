#ifndef	__C_HEAP__
#define	__C_HEAP__

#include "CList.h"

template<class TYPE >
class	CHeap
{
protected:
	struct	NODE
	{
		typedef NODE* PNODE;

		TYPE	Data;		//	���� ����Ÿ

		union
		{
			struct {	NODE	*pParent; };	//	�θ�
			struct {	NODE	*pNext; };		//	Garbage ������ ����Ʈ.
		};

		NODE	*pLChild;	//	���� �ڽ�
		NODE	*pRChild;	//	������ �ڽ�
		
		NODE ()	:
			pParent(NULL),
			pLChild(NULL),
			pRChild(NULL)
		{
		}

		void GarbageCollection ( PNODE &pNode )
		{
			if ( pLChild )	pLChild->GarbageCollection ( (PNODE)pNode );
			if ( pRChild )	pRChild->GarbageCollection ( (PNODE)pNode );

			this->pNext = pNode;
			pNode = this;
		}

		~NODE ()
		{
			SAFE_DELETE(pLChild);
			SAFE_DELETE(pRChild);
		}
	};	

	//	<--	��帮��Ʈ ����
	//	<**	��帮��Ʈ�� ������ �����ڸ�,
	//		������ ��Ʈ�� ���� ������ ���� �ð�����
	//		ã���ְ� �ϴ� �Ǽ��� ����ü��.
	//		�� ����Ʈ���� Head�� �������� �Ǹ� ��Ʈ�� �Ǵ� ���̰�,
	//		������ ��������, ���� ������ �Ǵ� ���̴�.
	//		�װ��� ���� ����Ʈ�� �������� ������ ��ȭ�� ���̴�.
	//	**>
	CMList<NODE*>	m_NodeList;

	NODE			*m_pGarList;

	//	-->	��帮��Ʈ ����

	NODE	*m_pRoot;
	NODE	*m_pNode;

	//	<--	����Ÿ �� �Լ� ������
	int		(*m_pfCompare)( TYPE &DataA, TYPE &DataB );
	//	-->	����Ÿ �� �Լ� ������

public:
	CHeap();
	virtual	~CHeap();

	//	<--	����Ÿ �ڵ鸵
	void	SetCompareFunc ( void* Func );
	BOOL	Insert ( TYPE &Data );
	BOOL	Delete ( TYPE &Data );	
	//	-->	����Ÿ �ڵ鸵

	//	<--	���� �����ʹ� ������ �޽��
	BOOL	Adjust	( TYPE &Data );
	BOOL	FindNode( TYPE &Data );
	//	-->	���� �����ʹ� ������ �޽��

	void	Clear		();
	DWORD	GetAmount	() { return m_NodeList.GetAmount(); }

protected:	
	void	Swap ( TYPE &DataA, TYPE &DataB );	
};

template<class TYPE>
inline CHeap<TYPE>::CHeap () :
m_pRoot(NULL),
m_pGarList(NULL),
m_pNode(NULL)
{
	Clear ();
}

template<class TYPE>
inline CHeap<TYPE>::~CHeap ()
{
	Clear ();

	NODE *pGarCur = (NODE*) m_pGarList, *pGarNext=NULL;
	while ( pGarCur )
	{
		pGarNext = pGarCur->pNext;

		pGarCur->pLChild = NULL;
		pGarCur->pRChild = NULL;
		SAFE_DELETE(pGarCur);

		pGarCur = pGarNext;
	}
}

template<class TYPE>
inline void CHeap<TYPE>::SetCompareFunc( void* Func )
{
	m_pfCompare = ( int (__cdecl*)( TYPE &, TYPE & ) ) Func;
}

template<class TYPE>
inline BOOL	CHeap<TYPE>::Insert ( TYPE &Data )
{
	static NODE* pNewNode;

	if ( m_pGarList )
	{
		pNewNode = m_pGarList;
		m_pGarList = m_pGarList->pNext;

		pNewNode->pParent = NULL;
		pNewNode->pLChild = NULL;
		pNewNode->pRChild = NULL;
	}
	else
	{
		pNewNode = new NODE;
	}

	pNewNode->Data = Data;
	m_NodeList.AddTail ( pNewNode );
	    
	//	<--	�θ� ��������
	if ( m_NodeList.GetAmount() == 1 )
	{
		m_pRoot = pNewNode;
		m_NodeList.SetHead();
		return TRUE;
	}

	static NODE	*pParent = NULL;
	m_NodeList.GetCurrent ( pParent );
	if ( pParent->pLChild && pParent->pRChild )
	{
		m_NodeList.GoNext ();
		m_NodeList.GetCurrent ( pParent );
	}
	//	-->	�θ� ��������

	//	<--	�θ� �ڽ� ����
	if ( !pParent->pLChild )
	{
		pParent->pLChild = pNewNode;
	}
	else
	{
		pParent->pRChild = pNewNode;
	}
	pNewNode->pParent = pParent;
	//	-->	�θ� �ڽ� ����

	//	<--	�ڷ� ����
	pParent = NULL;
	static  NODE	*pChild = NULL;

	m_NodeList.GetTail ( pChild );
	pParent = pChild->pParent;

	while ( pParent )
	{
		if ( 0 < m_pfCompare ( pParent->Data , pChild->Data ) )
		{
			Swap ( pParent->Data, pChild->Data );
		}

		pChild = pParent;
		pParent = pChild->pParent;
	}
	//	-->	�ڷ� ����

	return TRUE;
}

template<class TYPE>
inline BOOL	CHeap<TYPE>::Delete ( TYPE &Data )
{
	static NODE* pLeaf;

	//	<--	��Ʈ ���� ( ����Ÿ�� )
	Data = m_pRoot->Data;
	//	-->	��Ʈ ����

	if ( m_NodeList.GetAmount() == 1 )
	{
		Clear ();

		return TRUE;
	}
	
	//	<--	��Ʈ�� ���� ���� �ֱ� ( ����Ÿ�� )
	pLeaf = NULL;
	m_NodeList.GetTail ( pLeaf );
	m_pRoot->Data = pLeaf->Data;
	//	<--	��Ʈ�� ���� ���� �ֱ� ( ����Ÿ�� )

	//	<--	�ڷ� ����
	static NODE	*pParent = NULL;
	static NODE	*pChild	= NULL;
	static NODE	*pLChild = NULL;
	static NODE	*pRChild = NULL;
    
	pParent = m_pRoot;
	pLChild = pParent->pLChild;
	pRChild = pParent->pRChild;
	while ( pLChild || pRChild )
	{
		//	<--	�ڽ��� ���� ���� ����
		if ( pRChild )		//	�ڽ� �� �� ���� ��
		{
			if ( 0 < m_pfCompare ( pLChild->Data, pRChild->Data ) )
			{
				pChild = pRChild;
			}
			else
			{
				pChild = pLChild;
			}
		}
		else if ( pLChild )	//	�ڽ� ���ʸ� ���� ��
		{
			pChild = pLChild;
		}
		//	-->	�ڽ��� ū ���� ����

		//	<--	�ڽİ� �θ��� ��
		if ( 0 < m_pfCompare ( pParent->Data, pChild->Data ) )
		{
			Swap ( pParent->Data, pChild->Data );
		}
		//	-->	�ڽİ� �θ��� ��

		pParent = pChild;
		pLChild = pParent->pLChild;
		pRChild = pParent->pRChild;
	}
	//	-->	�ڷ� ����

	//	<--	���� ��� ����
	pParent = pLeaf->pParent;
	if ( pParent->pRChild )
	{
		pParent->pRChild = NULL;
	}
	else if ( pParent->pLChild )
	{
        pParent->pLChild = NULL;
	}

	//	Note : �޸� ��� ���
	//
	pLeaf->pNext = m_pGarList;
	m_pGarList = pLeaf;
	pLeaf = NULL;

	m_NodeList.DelTail ();
	//	-->	���� ��� ����

	return TRUE;
}

template<class TYPE>
inline void	CHeap<TYPE>::Clear ()
{	
	if ( m_pRoot )
	{
		m_pRoot->GarbageCollection ( m_pGarList );
		m_pRoot	= NULL;
	}

	m_pNode = NULL;

	m_NodeList.RemoveAll ();
}

template<class TYPE>
inline void	CHeap<TYPE>::Swap ( TYPE &DataA, TYPE &DataB )
{
	TYPE	Temp;
	
	Temp = DataA;
	DataA = DataB;
	DataB = Temp;
}

template<class TYPE>
inline BOOL CHeap<TYPE>::FindNode ( TYPE &Data )
{
	if ( !m_pRoot )
	{
		return FALSE;
	}

	NODE **pStack = new NODE*[ m_NodeList.GetAmount() + 1 ];	//	0���� ���� �����Ƿ� �� �� �� ��´�.

	pStack[0] = NULL;
	pStack[1] = m_pRoot;
	DWORD	StackCount = 1;	

    while ( StackCount )
	{		
		//	<--	Pop
		NODE *pNODE = pStack[StackCount];
		StackCount--;
		//	-->	Pop

		if ( !m_pfCompare ( pNODE->Data, Data ) )
		{
			delete [] pStack;
			pStack = NULL;

			m_pNode = pNODE;

			return TRUE;
		}

		//	<--	Push
		if ( pNODE->pLChild && ( 0 < m_pfCompare ( pNODE->pLChild->Data, Data ) ) )
		{
			StackCount++;
			pStack[StackCount] = pNODE->pLChild;

		}
		if ( pNODE->pRChild && ( 0 < m_pfCompare ( pNODE->pRChild->Data, Data ) ) )
		{
			StackCount++;
			pStack[StackCount] = pNODE->pRChild;			
		}
		//	-->	Push
	}
    
	delete [] pStack;
	pStack = NULL;

	m_pNode = NULL;

	return FALSE;
}

template<class TYPE>
inline BOOL CHeap<TYPE>::Adjust ( TYPE &Data )
{
	if ( m_pNode )
	{
		//	<--	���� ����ִ°ͺ��� ū ���� ���� ���
		if ( 0 < m_pfCompare ( m_pNode->Data, Data ) )
		{
			m_pNode->Data = Data;

			//	<--	�ڷ� ����
			NODE	*pParent = m_pNode->pParent;	
			NODE	*pChild = m_pNode;

			while ( pParent )
			{
				if ( 0 < m_pfCompare ( pParent->Data , pChild->Data ) )
				{
					Swap ( pParent->Data, pChild->Data );
				}

				pChild = pParent;
				pParent = pChild->pParent;
			}
			//	-->	�ڷ� ����

			return TRUE;
		}
		//	-->	���� ����ִ°ͺ��� ū ���� ���� ���
		else
		{
			m_pNode->Data = Data;

			//	<--	�ڷ� ����
			NODE	*pParent = NULL;
			NODE	*pChild	= NULL;
			NODE	*pLChild = NULL;
			NODE	*pRChild = NULL;
		    
			pParent = m_pNode;
			pLChild = pParent->pLChild;
			pRChild = pParent->pRChild;
			while ( pLChild || pRChild )
			{
				//	<--	�ڽ��� ū ���� ����
				if ( pRChild )		//	�ڽ� �� �� ���� ��
				{
					if ( 0 < m_pfCompare ( pLChild->Data, pRChild->Data ) )
					{
						pChild = pRChild;
					}
					else
					{
						pChild = pLChild;
					}
				}
				else if ( pLChild )	//	�ڽ� ���ʸ� ���� ��
				{
					pChild = pLChild;
				}
				//	-->	�ڽ��� ū ���� ����

				//	<--	�ڽİ� �θ��� ��
				if ( 0 < m_pfCompare ( pParent->Data, pChild->Data ) )
				{
					Swap ( pParent->Data, pChild->Data );
				}
				//	-->	�ڽİ� �θ��� ��

				pParent = pChild;
				pLChild = pParent->pLChild;
				pRChild = pParent->pRChild;
			}
			//	-->	�ڷ� ����

			return TRUE;            
		}
	}

	return FALSE;
}



#endif	//	__C_HEAP__