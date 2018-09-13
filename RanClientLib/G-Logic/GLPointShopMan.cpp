#include "stdafx.h"
#include "./GLPointShopMan.h"
#include "./GLogicData.h"
#include "../../EngineLib/Common/IniLoader.h"
#include "../../EngineLib/G-Logic/GLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int nKeysize = (sizeof( DWORD )+sizeof( BYTE ) ) * POINTSHOP_ITEMNUM * 2;

GLPointShopMan& GLPointShopMan::GetInstance()
{
	static GLPointShopMan Instance;
	return Instance;
}


GLPointShopMan::GLPointShopMan () 
//: m_bServer ( false )
{
}

GLPointShopMan::~GLPointShopMan ()
{
	CleanUp();
}

HRESULT GLPointShopMan::LoadFile ( const char* szFile )
{
	//	Note : 이전 정보 삭제.
	CleanUp();

	//m_bServer = bServer;

	std::string strLIST_FILE = m_strPATH + szFile;

	CIniLoader cFILE;

	if( GLOGIC::bGLOGIC_ZIPFILE )
		cFILE.SetZipFile( GLOGIC::strGLOGIC_ZIPFILE );

	std::string strSep( ",[]\t" );
	cFILE.reg_sep( strSep );

	if( !cFILE.open( strLIST_FILE, true, GLOGIC::bGLOGIC_PACKFILE ) )
	{
		CDebugSet::ToLogFile ( "ERROR : GLPointShopMan::LoadFile(), File Open %s", szFile );
		return false;
	}

	INT nPointShopKeySize(0);

	cFILE.getflag( "POINT_SHOP_INFO", "PointShopKeySize", 0, 1, nPointShopKeySize );

	DWORD dwNUM = cFILE.GetKeySize( "POINT_SHOP_LIST", "POINT_SHOP" );
	for ( DWORD i=0; i<dwNUM; ++i )
	{
		POINT_SHOP	sPointShop;

		cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", 0, nPointShopKeySize, sPointShop.dwKey );


		for ( int j = 0; j < POINTSHOP_ITEMNUM; ++j )
		{
			cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", (j*3)+1, nPointShopKeySize, sPointShop.sTicketItem[j].sNID.wMainID );
			cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", (j*3)+2, nPointShopKeySize, sPointShop.sTicketItem[j].sNID.wSubID );		
			cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", (j*3)+3, nPointShopKeySize, sPointShop.sTicketItem[j].nNum );
		}

		cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", 16, nPointShopKeySize, sPointShop.sResultItem.sNID.wMainID );
		cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", 17, nPointShopKeySize, sPointShop.sResultItem.sNID.wSubID );
		cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", 18, nPointShopKeySize, sPointShop.sResultItem.nNum );

		cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", 19, nPointShopKeySize, sPointShop.dwRate );
		cFILE.getflag( i, "POINT_SHOP_LIST", "POINT_SHOP", 20, nPointShopKeySize, sPointShop.dwPrice );
		//add dummy item mix flag for geger POINT_SHOP

		insert( sPointShop );
	}

	return S_OK;
}

void GLPointShopMan::CleanUp()
{
	m_mapStrPointShop.clear();
	m_mapKeyPointShop.clear();

	//m_bServer = false;
}

void GLPointShopMan::insert ( POINT_SHOP& sPointShop )
{
	//if ( m_bServer )
	//{
	//	SortMeterialItem( sItemMix );
	//	m_mapKeyItemMix.insert( std::make_pair( sItemMix.dwKey, sItemMix ));
	//}
	//else
	//{
	    SortTicketItem( sPointShop );

		m_mapKeyPointShop.insert( std::make_pair( sPointShop.dwKey, sPointShop ));

		TCHAR szKey[nKeysize+1] = {0};

		SortTicketItem( sPointShop );

		for ( int i = 0; i < POINTSHOP_ITEMNUM; ++i )
		{
			_stprintf( szKey, "%s%0.8x%0.2x", szKey, sPointShop.sTicketItem[i].sNID.dwID, sPointShop.sTicketItem[i].nNum );
		}

		std::string strKey = szKey;

		m_mapStrPointShop.insert( std::make_pair( strKey, sPointShop ));		
		
	//}

	return;
}

const POINT_SHOP* GLPointShopMan::GetPointShop( const POINT_SHOP& sPointShop )
{
	//if ( m_bServer )	return NULL;

	TCHAR szKey[nKeysize+1] = {0};


	for ( int i = 0; i < POINTSHOP_ITEMNUM; ++i )
	{
		_stprintf( szKey, "%s%0.8x%0.2x", szKey, sPointShop.sTicketItem[i].sNID.dwID, sPointShop.sTicketItem[i].nNum );
	}

	std::string strKey = szKey;

	POINTSHOP_STRMAP_ITER pos_beg = m_mapStrPointShop.find( strKey );
	POINTSHOP_STRMAP_ITER pos_end = m_mapStrPointShop.end();
	if ( pos_beg == pos_end )
	{
		return NULL;
	}

	return &(*pos_beg).second;
}

const POINT_SHOP* GLPointShopMan::GetPointShop( DWORD dwKey )
{
	//if ( !m_bServer ) return NULL;
	
	POINTSHOP_KEYMAP_ITER pos = m_mapKeyPointShop.find(dwKey);
	if ( pos==m_mapKeyPointShop.end() )		return NULL;

	return &(*pos).second;
}

void GLPointShopMan::SortTicketItem( POINT_SHOP& sPointShop )
{
	std::vector< POINTSHOP_DATA >	vecTemp;
	for( int i = 0; i < POINTSHOP_ITEMNUM; ++i ) 
	{
		vecTemp.push_back( sPointShop.sTicketItem[i] );
	}

	std::sort( vecTemp.begin(), vecTemp.end(), POINTSHOP_DATA_CMP() );

	std::vector< POINTSHOP_DATA >::iterator pos = vecTemp.begin();
	for( int i = 0; i < POINTSHOP_ITEMNUM; ++i ) 
	{
		sPointShop.sTicketItem[i] = (POINTSHOP_DATA)(*pos);
		pos++;
	}
}

void GLPointShopMan::SortInvenItem ( SINVENITEM* pInvenItem )
{
	std::vector< SINVENITEM >	vecTemp;
	for( int i = 0; i < POINTSHOP_ITEMNUM; ++i ) 
	{
		vecTemp.push_back( pInvenItem[i] );
	}

	std::sort( vecTemp.begin(), vecTemp.end(), POINTSHOP_INVEN_CMP() );

	std::vector< SINVENITEM >::iterator pos = vecTemp.begin();
	for( int i = 0; i < POINTSHOP_ITEMNUM; ++i ) 
	{
		pInvenItem[i] = (SINVENITEM)(*pos);
		pos++;
	}

	return;
}
