#pragma once

#include <string>

#include "./GLItem.h"

class GLPointShopMan
{
public:
typedef stdext::hash_map<std::string,POINT_SHOP> POINTSHOP_STRMAP;
typedef stdext::hash_map<DWORD,POINT_SHOP> POINTSHOP_KEYMAP;

typedef POINTSHOP_STRMAP::iterator	POINTSHOP_STRMAP_ITER;
typedef POINTSHOP_KEYMAP::iterator	POINTSHOP_KEYMAP_ITER;


public:
	enum
	{
		VERSION	= 0x0100,		
	};

public:

	POINTSHOP_STRMAP	m_mapStrPointShop;
	POINTSHOP_KEYMAP	m_mapKeyPointShop;

	std::string				m_strPATH;
	GLPointShopMan::POINTSHOP_STRMAP& GetPointShop ()	{ return m_mapStrPointShop; }
	GLPointShopMan::POINTSHOP_KEYMAP& GetPointShopz ()	{ return m_mapKeyPointShop; }
public:
	void SetPath ( std::string strPATH )	{ m_strPATH = strPATH; }
	std::string GetPath ()					{ return m_strPATH.c_str(); }

protected:
	bool	m_bServer;

public:
	HRESULT LoadFile ( const char* szFile );

	void insert ( POINT_SHOP& sPointShop );

	void SortTicketItem( POINT_SHOP& sPointShop );	//	재료아이템 정렬
	void SortInvenItem ( SINVENITEM* pInvenItem );

	const POINT_SHOP* GetPointShop( const POINT_SHOP& sPointShop );
	const POINT_SHOP* GetPointShop( DWORD dwKey );

public:
	GLPointShopMan ();

public:
	~GLPointShopMan ();
	void CleanUp();

public:
	static GLPointShopMan& GetInstance();
};

class POINTSHOP_DATA_CMP
{
public:
	bool operator() ( const POINTSHOP_DATA &lvalue, const POINTSHOP_DATA &rvalue )
	{
		if ( lvalue.sNID.dwID < rvalue.sNID.dwID ) return true;
		else if ( lvalue.sNID.dwID == rvalue.sNID.dwID ) 
		{	
			if ( lvalue.nNum < rvalue.nNum ) return true;
		}
		
		return false;
	}
};

class POINTSHOP_INVEN_CMP
{
public:
	bool operator() ( const SINVENITEM &lvalue, const SINVENITEM &rvalue )
	{
		if ( lvalue.sItemCustom.sNativeID.dwID < rvalue.sItemCustom.sNativeID.dwID ) return true;
		else if ( lvalue.sItemCustom.sNativeID.dwID == rvalue.sItemCustom.sNativeID.dwID ) 
		{	
			if ( lvalue.sItemCustom.wTurnNum < rvalue.sItemCustom.wTurnNum ) return true;
		}
		
		return false;
	}
};

