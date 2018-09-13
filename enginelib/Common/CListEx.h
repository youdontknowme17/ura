#ifndef _CListEx_H_
#define _CListEx_H_

#include <windows.h>

//-----------------------------------------------
//
//	���� �Ҵ�� ����Ÿ ������ ����.
//	����Ÿ �����Ҵ��� ����.
//	�Ҵ�� ����Ÿ ����.
//
//	����Ʈ  Ž�� �Լ� ����.
//
//	BOOL (*FindFunc)( void*, LPTYPE )
//	
//	void* -- Ž������ �񱳰�.
//	LPTYPE -- Ž���� ����� �ּ�.
//
//-----------------------------------------------
//
//	CListEX::FindNode ( void *pValue, LPTYPE &pData ) �Լ� �߰�
//	�� �Լ��� CListEX::pCurrent �� ���� �ٲ��� �ʴ´�.
//
//	Last Update - 2000/07/12 -jdh.
//

template<class TYPE>
class CListEx
{
protected:
	class NODE
	{
	public:
		NODE *Before, *Next;
		TYPE *pData;

	public:
		NODE() { pData=NULL;}
		~NODE() { if ( pData!=NULL) delete pData; }
	};
	typedef TYPE* LPTYPE;
	
	static BOOL DefaultFindFunc ( void* value, LPTYPE pData);
	static BOOL DefaultAddFunc( void* value, LPTYPE pData,CListEx<TYPE>* pList);

	BOOL (*FindFunc) ( void*, LPTYPE );
	BOOL (*AddFunc) (void*, LPTYPE,CListEx<TYPE>*);

	NODE Head, Tail;	// ����, ��
	NODE *pCurrent;		// Ž�� ��ġ.
	int	 Amount;
	// ��� ����, �ı�.
	NODE* NewNode () { return new NODE;}
	void DelNode ( NODE *pNode );

public:
	// ������, �ı���, �ʱ�ȭ
	CListEx ();
	CListEx ( void* Func );
	~CListEx ();
	void SetFindFunc( void* Func );
	
	//2002.05.10�߰�
	void SetAddFunc( void* Func);

	// ���°˻�.
	BOOL IsEmpty ();
	BOOL IsEnd ();

	// ����Ÿ ����
	void AddHead ( LPTYPE pData );
	void AddTail ( LPTYPE pData );
	void AddCurrent ( LPTYPE pData );
	
	// ����Ÿ ����
	BOOL DelHead ();
	BOOL DelTail ();
	BOOL DelCurrent ();
	void RemoveAll ();

	// ����Ÿ ��Ȯ��.
	BOOL GetHead ( LPTYPE &pData );
	BOOL GetTail ( LPTYPE &pData );
	BOOL GetCurrent ( LPTYPE &pData );
	LPTYPE GetCurrent();
	// ����Ÿ Ž��.
	void SetHead ();
	void GoNext ();
	void GoPrev();
	BOOL FindNode ( void *pValue );
	BOOL FindNode ( void *pValue, LPTYPE &pData );
	
	//2002.06.07�߰�
	int GetAmount(){return Amount;}
};

//-------------------- ��� ����, �ı�.
template<class TYPE>
inline void CListEx<TYPE>::DelNode ( NODE *pNode )
{
	if ( pCurrent==pNode )
	{
		if ( pCurrent->Next!=&Tail ) pCurrent = pCurrent->Next;
		else pCurrent = pCurrent->Before;
	}

	if ( pNode ) { delete pNode; pNode = NULL; }
}

//-------------------- ������, �ı���, �ʱ�ȭ
template<class TYPE>
CListEx<TYPE>::CListEx ()
{
	Head.Before = &Tail;
	Head.Next = &Tail;

	Tail.Before = &Head;
	Tail.Next = &Head;
	
	pCurrent = Head.Next;

	FindFunc = DefaultFindFunc;
	//
	AddFunc = DefaultAddFunc;
	Amount=0;
}

template<class TYPE>
CListEx<TYPE>::CListEx ( void* Func )
{
	Head.Before = &Tail;
	Head.Next = &Tail;

	Tail.Before = &Head;
	Tail.Next = &Head;
	
	pCurrent = Head.Next;

	FindFunc = ( BOOL (__cdecl*)( void*, LPTYPE ) ) Func;
	//
	AddFunc = DefaultAddFunc;
	Amount=0;
}

template<class TYPE>
CListEx<TYPE>::~CListEx ()
{
	RemoveAll ();
}

template<class TYPE >
void CListEx<TYPE>::SetFindFunc( void* Func )
{
	FindFunc = ( BOOL (__cdecl*)( void*, LPTYPE ) ) Func;
}

template<class TYPE>
void CListEx<TYPE>::SetAddFunc(void* Func)
{
	AddFunc= ( BOOL (__cdecl*)( void*, LPTYPE ,CListEx<TYPE>*) ) Func;

}

//-------------------- ���°˻�.
template<class TYPE>
inline BOOL CListEx<TYPE>::IsEmpty ()
{
	if ( Head.Next == &Tail ) return TRUE;
	return FALSE;
}

template<class TYPE>
inline BOOL CListEx<TYPE>::IsEnd ()
{
	if ( pCurrent == &Tail || pCurrent == &Head ) return TRUE;
	return FALSE;
}

template<class TYPE>
inline BOOL CListEx<TYPE>::DefaultFindFunc ( void* value, LPTYPE pData)
{
//	LPTYPE pValue = (TYPE*) value;
//	if ( (*pValue)==(*pData) ) return TRUE;
	return FALSE;
}
template<class TYPE>
inline BOOL CListEx<TYPE>::DefaultAddFunc ( void* value, LPTYPE pData,CListEx<TYPE>* pList)
{
//	LPTYPE pValue = (TYPE*) value;
//	if ( (*pValue)==(*pData) ) return TRUE;
	return FALSE;
}
//-------------------- ����Ÿ ����.
template<class TYPE>
void CListEx<TYPE>::AddHead ( LPTYPE pData )
{
	NODE *pBackNode = Head.Next;
	
	NODE *pNewNode = NewNode ();	// �� ��� ����.
	pNewNode->pData = pData;		// �� �Է�.
	
	pNewNode->Next = pBackNode;		// ����.
	pNewNode->Before = &Head;

	Head.Next = pNewNode;
	pBackNode->Before = pNewNode;

	//
	//
	Amount++;
}

template<class TYPE>
void CListEx<TYPE>::AddTail ( LPTYPE pData )
{
	NODE *pBackNode = Tail.Before;

	NODE *pNewNode = NewNode ();	// �� ��� ����.
	pNewNode->pData = pData;			// �� �Է�.

	pNewNode->Next = &Tail;			// ����.
	pNewNode->Before = pBackNode;

	Tail.Before = pNewNode;
	pBackNode->Next = pNewNode;
	//
	//
	Amount++;
}

template<class TYPE>
void CListEx<TYPE>::AddCurrent ( LPTYPE pData )
{
	GASSERT ( pCurrent!=&Tail );

	NODE *pNextNode = pCurrent->Next;

	NODE *pNewNode = NewNode ();	// �� ��� ����.
	pNewNode->pData = pData;			// �� �Է�.
	
	pNewNode->Next = pNextNode;		// ����.
	pNewNode->Before = pCurrent;

	pNextNode->Before = pNewNode;
	pCurrent->Next = pNewNode;

	pCurrent = pNewNode;
	//
	//
	Amount++;
}

//-------------------- ����Ÿ ����.
template<class TYPE>
BOOL CListEx<TYPE>::DelHead ()
{
	if ( IsEmpty () ) return FALSE;

	NODE *pDelNode = Head.Next;
	
	Head.Next = pDelNode->Next;		// ��� ���� ����.
	pDelNode->Next->Before = &Head;

	DelNode ( pDelNode );			// ��� ����.
	//
	//
	Amount--;
	return TRUE;
}

template<class TYPE>
BOOL CListEx<TYPE>::DelTail ()
{
	if ( IsEmpty () ) return FALSE;

	NODE *pDelNode = Tail.Before;
	
	Tail.Before = pDelNode->Before;	// ��� ���� ����.
	pDelNode->Before->Next = &Tail;

	DelNode ( pDelNode );			// ��� ����.
	//
	//
	Amount--;
	return TRUE;
}

template<class TYPE>
BOOL CListEx<TYPE>::DelCurrent ()
{
	if ( pCurrent == &Head || pCurrent == &Tail ) return FALSE;

	NODE *pBeforeNode = pCurrent->Before;
	NODE *pNextNode = pCurrent->Next;

	pBeforeNode->Next = pNextNode;
 	pNextNode->Before = pBeforeNode;

	DelNode ( pCurrent );			// ��� ����.

	pCurrent = pNextNode;
	//
	//
	Amount--;
	return TRUE;
}

template<class TYPE>
void CListEx<TYPE>::RemoveAll ()
{
	for( SetHead(); !IsEmpty() ; )
	{
		DelCurrent();
	}
	//
	//
	Amount=0;
}

//-------------------- ����Ÿ ��Ȯ��.
template<class TYPE>
BOOL CListEx<TYPE>::GetHead ( LPTYPE &pData)
{
	if ( IsEmpty () ) return FALSE;

	pData = Head.Next->pData;		// �� ���.

	return TRUE;
}

template<class TYPE>
BOOL CListEx<TYPE>::GetTail ( LPTYPE &pData )
{
	if ( IsEmpty () ) return FALSE;

	pData = Tail.Before->pData;		// �� ���.

	return TRUE;
}

template<class TYPE>
BOOL CListEx<TYPE>::GetCurrent ( LPTYPE &pData )
{
	if ( pCurrent == &Head || pCurrent == &Tail ) return FALSE;

	pData = pCurrent->pData;		// �� ���.

	return TRUE;
}
template<class TYPE>
TYPE* CListEx<TYPE>::GetCurrent ()
{
	if ( pCurrent == &Head || pCurrent == &Tail ) return FALSE;
	return pCurrent->pData;
}

//-------------------- ����Ÿ Ž��
template<class TYPE>
inline void CListEx<TYPE>::SetHead ()
{
	pCurrent = Head.Next;
}

template<class TYPE>
inline void CListEx<TYPE>::GoNext ()
{
	GASSERT ( pCurrent != &Tail && pCurrent != &Head );

	pCurrent = pCurrent->Next;
}

template<class TYPE>
inline void CListEx<TYPE>::GoPrev ()
{
	GASSERT ( pCurrent != &Tail && pCurrent != &Head );

	pCurrent = pCurrent->Before;
}

template<class TYPE>
BOOL CListEx<TYPE>::FindNode ( void *pValue )
{
	if ( FindFunc == NULL ) return FALSE;

	BOOL IsFind = FALSE;
	LPTYPE pFindData=NULL;
	for( SetHead(); !IsEnd(); GoNext() )
	{
		GetCurrent ( pFindData );
		if( FindFunc ( pValue, pFindData) )
		{
			IsFind = TRUE;
			break;
		}
	}
	
	return (IsFind);
}

template<class TYPE>
BOOL CListEx<TYPE>::FindNode ( void *pValue, LPTYPE &pData )
{
	if ( FindFunc == NULL ) return FALSE;

	// Ž������ Current �� ���
	NODE *pBackCurrent = pCurrent;

	BOOL IsFind = FALSE;
	LPTYPE pFindData=NULL;
	for( SetHead(); !IsEnd(); GoNext() )
	{
		GetCurrent ( pFindData );
		if( FindFunc ( pValue, pFindData) )
		{
			pData = pFindData;
			IsFind = TRUE;
			break;
		}
	}
	
	// Ŀ��Ʈ �� ����
	pCurrent = pBackCurrent;

	return (IsFind);
}
#endif //  _CListEx_H_