//	BasicScrollBarEx Ŭ����
//
//	���� �ۼ��� : ���⿱
//	���� ������ :
//	�α�
//		[2003.12.1]
//			@ ��������

#pragma	once

#include "../EngineUIlib/GUInterface/BasicTree.h"

class	CBasicTreeEx : public CBasicTree
{
public:
	CBasicTreeEx ();
	virtual	~CBasicTreeEx ();

public:
 //   void	CreateBaseTree ( char* szBaseTree );

public:
	CBasicTreeNode*	InsertNode ( CBasicTreeNode* pParent, CString strNodeName );

private:
	UIGUID	m_NodeWndID;
};