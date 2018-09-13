#include "StdAfx.h"
#include "./DxCharSham.h"
#include "./G-Logic/GLogicData.h"
#include "./G-Logic/GLItemMan.h"

#include "../EngineLib/DxCommon/DxMethods.h"
#include "../EngineLib/DxCommon/DxShadowMap.h"
#include "../EngineLib/DxCommon/EditMeshs.h"
#include "../EngineLib/DxEffect/DxEffectMan.h"
#include "../EngineLib/DxEffect/Single/DxEffGroupPlayer.h"
#include "../enginelib/Meshs/DxSkinPieceData.h"//add abl
#include "../enginelib/Meshs/DxSkinCharPiece.h"
#include "../enginelib/Meshs/DxSkinPieceRootData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxCharSham::DxCharSham(void) :
	m_pd3dDevice(NULL),
	m_pSkinChar(NULL),

	m_vDir(D3DXVECTOR3(0,0,-1)),
	m_vDirOrig(D3DXVECTOR3(0,0,-1)),
	m_vPos(D3DXVECTOR3(0,0,0)),

	m_nID(0),
	m_bSelect(FALSE),
	m_bEffect(FALSE),

	m_fAge(),
	m_Action(GLAT_IDLE),
	m_fIdleTime(0.0f),
	m_bPeaceZone(TRUE),
	m_bIdleReserv(FALSE),

	m_vMaxOrg(3,18,3),
	m_vMinOrg(-3,0,-3),

	m_vMax(3,18,3),
	m_vMin(-3,0,-3),

	m_dwRevData(NULL),
	m_bNeedData(FALSE),

	m_fDelayFreeAnim(0.0f)//add vcf
{
}

DxCharSham::~DxCharSham(void)
{
	SAFE_DELETE(m_pSkinChar);
}

void DxCharSham::GetAABB ( D3DXVECTOR3 &vMax, D3DXVECTOR3 &vMin )
{
	vMax = m_vMax;
	vMin = m_vMin;
}

HRESULT DxCharSham::SetCharInfo ( const SCHARINFO_LOBBY &CharInfo, const BOOL bThread )
{
	m_dwRevData |= RECEIVE_CHARDATA;

	m_CharInfo = CharInfo;

	//	Note : �ɸ����� ������ ����.
	StringCchCopy ( GLCHARLOGIC::m_szName, CHAR_SZNAME, m_CharInfo.m_szName );
	GLCHARLOGIC::m_emClass = m_CharInfo.m_emClass;
	GLCHARLOGIC::m_wSchool = m_CharInfo.m_wSchool;
	GLCHARLOGIC::m_wHair = m_CharInfo.m_wHair;
	GLCHARLOGIC::m_wHairColor = m_CharInfo.m_wHairColor;
	GLCHARLOGIC::m_wFace = m_CharInfo.m_wFace;

	GLCHARLOGIC::m_wLevel = m_CharInfo.m_wLevel;
	GLCHARLOGIC::m_sStats = m_CharInfo.m_sStats;

//  LOBY data ����
//	memcpy ( GLCHARLOGIC::m_PutOnItems, m_CharInfo.m_PutOnItems, sizeof(SITEMCUSTOM)*SLOT_TSIZE );

	for ( int i = 0; i < SLOT_TSIZE; ++i )
	{
		GLCHARLOGIC::m_PutOnItems[i].Assign(  m_CharInfo.m_PutOnItems[i] );
	}


	GLCHARLOGIC::INIT_DATA ( FALSE, FALSE );

	//	Note : �ɸ����� ������ ����.
	EMCHARINDEX emIndex = CharClassToIndex(m_CharInfo.m_emClass);

	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().LoadData( GLCONST_CHAR::szCharSkin[emIndex], m_pd3dDevice, bThread );
	if ( !pSkinChar )	return E_FAIL;

	SAFE_DELETE(m_pSkinChar);
	m_pSkinChar = new DxSkinChar;
	m_pSkinChar->SetCharData ( pSkinChar, m_pd3dDevice );
	if ( m_pd3dDevice )	UpdateSuit ();

	//	Note : ���ϸ��̼� �ʱ�ȭ.
	//
	m_pSkinChar->SELECTANI ( AN_PLACID, AN_SUB_NONE );

	return S_OK;
}

HRESULT DxCharSham::Create ( D3DXVECTOR3* pvPos, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, BOOL bNeedData, int nID )
{
	m_pd3dDevice = pd3dDevice;

	m_nID = nID;
	m_bNeedData = bNeedData;

	//	Note : �ɸ����� ������ ����.
	EMCHARINDEX emIndex = CharClassToIndex(m_CharInfo.m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().LoadData( GLCONST_CHAR::szCharSkin[emIndex], pd3dDevice, bThread );
	GASSERT(pSkinChar&&"DxCharSham::Create()->DxSkinCharData::Load()");
	if ( !pSkinChar )	return E_FAIL;

	SAFE_DELETE(m_pSkinChar);
	m_pSkinChar = new DxSkinChar;

	m_pSkinChar->SetCharData( pSkinChar, pd3dDevice );
	UpdateSuit ();

	//	Note : �̵� ���� �ʱ�ȭ.
	if ( pvPos )	m_vPos = *pvPos;

	return S_OK;
}

HRESULT DxCharSham::UpdateSuit ()
{
	GASSERT(m_pd3dDevice);

	if ( !m_pSkinChar )										
		return S_FALSE;

	EMCHARINDEX emIndex = CharClassToIndex(m_emClass);
	DxSkinCharData* pSkinChar = DxSkinCharDataContainer::GetInstance().FindData ( GLCONST_CHAR::szCharSkin[emIndex] );
	if ( !pSkinChar )	return E_FAIL;

	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

		if ( sCONST.dwHEADNUM > m_wFace )//add abl
		{
			std::string strHEAD_CPS = sCONST.strHEAD_CPS[m_wFace];
		
			PDXCHARPART pCharPart = NULL;
			pCharPart = m_pSkinChar->GetPiece(PIECE_HEAD);		//	���� ���� ��Ų.

			if ( pCharPart && strcmp(strHEAD_CPS.c_str(),pCharPart->m_szFileName) )
			{//add abl
				DxSkinPieceRootData* pPieceDataRoot;
				pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( strHEAD_CPS.c_str() , m_pd3dDevice , TRUE );
				if ( pPieceDataRoot )
				{
					if ( pPieceDataRoot->GetUseCPS() )
					{
						m_pSkinChar->SetPiece( pPieceDataRoot->GetCpsName().c_str(), m_pd3dDevice, 0x0, 0, TRUE );
					}
					m_pSkinChar->SetCharPieceData( pPieceDataRoot->GetData()
													, m_pd3dDevice
													, pPieceDataRoot->GetBoneLink()
													, pPieceDataRoot->GetWeaponSlot()
													, pPieceDataRoot->GetType()
													, 0x0
													, 0
													, TRUE );
				}else{
					m_pSkinChar->SetPiece ( strHEAD_CPS.c_str(), m_pd3dDevice, 0x0, 0, TRUE );
				}
			}
		}
	

	if ( sCONST.dwHAIRNUM > m_wHair )
		{//add abl
			std::string strHAIR_CPS = sCONST.strHAIR_CPS[m_wHair];
		
			PDXCHARPART pCharPart = NULL;
			pCharPart = m_pSkinChar->GetPiece(PIECE_HAIR);		//	���� ���� ��Ų.

			if ( pCharPart && strcmp(strHAIR_CPS.c_str(),pCharPart->m_szFileName) )
			{
				DxSkinPieceRootData* pPieceDataRoot;
				pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( strHAIR_CPS.c_str() , m_pd3dDevice , TRUE );
				if ( pPieceDataRoot )
				{
					if ( pPieceDataRoot->GetUseCPS() )
					{
						m_pSkinChar->SetPiece( pPieceDataRoot->GetCpsName().c_str(), m_pd3dDevice, 0x0, 0, TRUE );
					}
					m_pSkinChar->SetCharPieceData( pPieceDataRoot->GetData()
													, m_pd3dDevice
													, pPieceDataRoot->GetBoneLink()
													, pPieceDataRoot->GetWeaponSlot()
													, pPieceDataRoot->GetType()
													, 0x0
													, 0
													, TRUE );
				}else{
					m_pSkinChar->SetPiece ( strHAIR_CPS.c_str(), m_pd3dDevice, 0x0, 0, TRUE );
				}
			}
		}

	// ����÷� ����
		m_pSkinChar->SetHairColor( m_wHairColor );

	for ( int i=0; i<SLOT_NSIZE_S_2; i++ )
	{	
		// ���� �������� ���Ⱑ �ƴϸ� �Ѿ��.
		if( !IsCurUseArm( EMSLOT(i) ) ) continue;

		SITEMCUSTOM &ItemCustom = m_PutOnItems[i];

		SNATIVEID nidITEM = ItemCustom.nidDISGUISE;
		if ( nidITEM==SNATIVEID(false) )	nidITEM = ItemCustom.sNativeID;
		if ( m_pSkinChar->m_pSkinVehicle  )//add vcf
		{
			if ( !m_bVehicle )
			{
				m_pSkinChar->ResetVCF();
			}
		}


		if ( nidITEM == SNATIVEID(false) )
		{
			//	Note : �⺻ ��Ų�� ���� ������ ��Ų�� Ʋ�� ���.
			//	SLOT->PIECE.
			PDXSKINPIECE pSkinPiece = NULL;	//	�⺻ ��Ų.
			PDXCHARPART pCharPart = NULL;	//	���� ���� ��Ų.

			EMPIECECHAR emPiece = SLOT_2_PIECE(EMSLOT(i));
			if ( emPiece!=PIECE_SIZE )
			{
				pSkinPiece = pSkinChar->GetPiece(emPiece);	//	�⺻ ��Ų.
				pCharPart = m_pSkinChar->GetPiece(emPiece);	//	���� ���� ��Ų.
			}

			if ( pSkinPiece )
			{
				if ( pCharPart && strcmp(pSkinPiece->m_szFileName,pCharPart->m_szFileName) )
					{//add abl
					DxSkinPieceRootData* pPieceDataRoot;
					pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( pSkinPiece->m_szFileName , m_pd3dDevice , TRUE );
					if ( pPieceDataRoot )
					{
						if ( pPieceDataRoot->GetUseCPS() )
						{
							m_pSkinChar->SetPiece( pPieceDataRoot->GetCpsName().c_str(), m_pd3dDevice, 0X0, 0, TRUE );
						}
						else
						{
							EMPIECECHAR emPiece_a = SLOT_2_PIECE(EMSLOT(i));
							PDXSKINPIECE pSkinPiece_a  = NULL;	
							if ( emPiece_a!=PIECE_SIZE )
							{
								pSkinPiece_a  = pSkinChar->GetPiece(emPiece_a );	//
							}
							m_pSkinChar->SetPiece ( pSkinPiece_a->m_szFileName, m_pd3dDevice, 0X0, 0, TRUE );
						}
						m_pSkinChar->SetCharPieceData( pPieceDataRoot->GetData()
														, m_pd3dDevice
														, pPieceDataRoot->GetBoneLink()
														, pPieceDataRoot->GetWeaponSlot()
														, pPieceDataRoot->GetType()
														, 0x0
														, 0
														, TRUE );
					}
					else
					{
						m_pSkinChar->SetPiece ( pSkinPiece->m_szFileName, m_pd3dDevice, 0X0, 0, TRUE );
						m_pSkinChar->ResetABL ( emPiece ); //clear abl when you remove an item
					}
				}
				else
				{
					m_pSkinChar->ResetABL ( emPiece ); //clear abl when you remove an item
				}
			}
			else
			{
				if ( pCharPart )
				{
					if ( m_pSkinChar->m_pSkinCharPiece[PIECE_RHAND] && emPiece != PIECE_LHAND )
					{
						m_pSkinChar->ResetPiece(emPiece);
					}
					else
					{
						if ( emPiece != PIECE_LHAND )
							m_pSkinChar->ResetPiece(emPiece);
						EMSLOT emRHand = GetCurRHand();
						SITEM* pRHAND = GET_SLOT_ITEMDATA ( emRHand );
						if ( pRHAND )// dual cps fix
						{
							EMCHARCLASS emSex = CharClassGetSex( emIndex );
							EMCPSINDEX	emLeft = CPSIND_NULL;
							if ( emSex == GLCC_MAN )
							{
								emLeft = CPSIND_M_LEFT;
							}else{
								emLeft = CPSIND_W_LEFT;
							}

							if (strcmp(pRHAND->GetCpsFile(emLeft), "") == 0)
							{
								m_pSkinChar->ResetPiece(emPiece);
							}
						}
						else
						{
							if ( !m_pSkinChar->m_pSkinCharPiece[PIECE_RHAND] && emPiece == PIECE_LHAND )
								m_pSkinChar->ResetPiece(emPiece);
						}

					}
				}
			}
		}
		// ��ȿ�ϸ�
		else
		{
			SITEM* pItem = GLItemMan::GetInstance().GetItem ( nidITEM );
			if ( pItem ) 
			{
					EMCHARCLASS emSex = CharClassGetSex( emIndex );
				EMCPSINDEX	emLeft = CPSIND_NULL;
				EMCPSINDEX	emRight = CPSIND_NULL;
				//add abl element by CNDev
				EMSTATE_BLOW emBlow  = pItem->sSuitOp.sBLOW.emTYPE;
				std::string strElement[EMBLOW_SIZE] =
				{
					"none.effskin",
					"elect.effskin",
					"elect.effskin",
					"special.effskin",
					"fire.effskin",
					"ice.effskin",
					"none.effskin",
					"poison.effskin",
					"none.effskin",
				};
				if ( emSex == GLCC_MAN )
				{
					emLeft = CPSIND_M_LEFT;
					emRight = CPSIND_M_RIGHT;
				}else{
					emLeft = CPSIND_W_LEFT;
					emRight = CPSIND_W_RIGHT;
				}
				//if ( emRight != CPSIND_NULL )
				{
					DxSkinPieceRootData* pPieceDataRoot;
					pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( pItem->GetCpsFile(emRight) , m_pd3dDevice , TRUE );
					if ( pPieceDataRoot )
					{
						if ( pPieceDataRoot->GetUseCPS() )
						{
							m_pSkinChar->SetPiece( pPieceDataRoot->GetCpsName().c_str(), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
						}
						else
						{
							m_pSkinChar->ResetCPS (  PIECE_RHAND );//remove existing cps when the abl data dont have cps
						}
						m_pSkinChar->SetCharPieceData( pPieceDataRoot->GetData()
														, m_pd3dDevice
														, pPieceDataRoot->GetBoneLink()
														, pPieceDataRoot->GetWeaponSlot()
														, (DWORD) PIECE_RHAND
														, NULL
														, ItemCustom.GETGRADE_EFFECT()
														, TRUE 
														//add abl element by CNDev
														, strElement[emBlow].c_str() );
						//m_pSkinChar->Clear( PIECE_LHAND );
					}else{
						m_pSkinChar->SetPiece ( pItem->GetCpsFile(emRight), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
					}
				}
				//if ( emLeft != CPSIND_NULL )
				{
					DxSkinPieceRootData* pPieceDataRoot;
					pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( pItem->GetCpsFile(emLeft) , m_pd3dDevice , TRUE );
					if ( pPieceDataRoot )
					{
						if ( pPieceDataRoot->GetUseCPS() )
						{
							m_pSkinChar->SetPiece( pPieceDataRoot->GetCpsName().c_str(), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
						}
						else
						{
							m_pSkinChar->ResetCPS (  PIECE_LHAND );//remove existing cps when the abl data dont have cps
						}
						m_pSkinChar->SetCharPieceData( pPieceDataRoot->GetData()
														, m_pd3dDevice
														, pPieceDataRoot->GetBoneLink()
														, pPieceDataRoot->GetWeaponSlot()
														, (DWORD) PIECE_LHAND
														, NULL
														, ItemCustom.GETGRADE_EFFECT()
														, TRUE 
														//add abl element by CNDev
														, strElement[emBlow].c_str() );
					}else{
						m_pSkinChar->SetPiece ( pItem->GetCpsFile(emLeft), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
					}
				}

				//if ( !m_pSkinChar->m_pSkinCharPiece[ PIECE_LHAND ] && !m_pSkinChar->m_pSkinCharPiece[ PIECE_RHAND ] )
				{
					if ( pItem->sBasicOp.emItemType != ITEM_VEHICLE )
					{
						{
						DxSkinPieceRootData* pPieceDataRoot;
						pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( pItem->GetWearingFile(emIndex) , m_pd3dDevice , TRUE );
						if ( pPieceDataRoot )
						{
							if ( pPieceDataRoot->GetUseCPS() )
							{
								m_pSkinChar->SetPiece( pPieceDataRoot->GetCpsName().c_str(), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
							}
							m_pSkinChar->SetCharPieceData( pPieceDataRoot->GetData()
															, m_pd3dDevice
															, pPieceDataRoot->GetBoneLink()
															, pPieceDataRoot->GetWeaponSlot()
															, pPieceDataRoot->GetType()
															, NULL
															, ItemCustom.GETGRADE_EFFECT()
															, TRUE 
															//add abl element by CNDev
															, strElement[emBlow].c_str() );
						}else{
							m_pSkinChar->SetPiece ( pItem->GetWearingFile(emIndex), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
						}
						}
						//LEFT HAND
						{
						DxSkinPieceRootData* pPieceDataRoot;
						pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( pItem->GetWearingFile2(emIndex) , m_pd3dDevice , TRUE );
						if ( pPieceDataRoot )
						{
							if ( pPieceDataRoot->GetUseCPS() )
							{
								m_pSkinChar->SetPiece( pPieceDataRoot->GetCpsName().c_str(), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
							}
							m_pSkinChar->SetCharPieceData( pPieceDataRoot->GetData()
														, m_pd3dDevice
														, pPieceDataRoot->GetBoneLink()
														, pPieceDataRoot->GetWeaponSlot()
														, pPieceDataRoot->GetType()
														, NULL
														, ItemCustom.GETGRADE_EFFECT()
														, TRUE );
						}else{
							m_pSkinChar->SetPiece ( pItem->GetWearingFile2(emIndex), m_pd3dDevice, NULL, ItemCustom.GETGRADE_EFFECT(), TRUE );
						}
						}
					}
				}
				m_pSkinChar->SetColorAll( 32767 );
			}
		}
	}

	return S_OK;
}

void DxCharSham::SetSelect ( BOOL bSel, BOOL bReserv )	
{ 
	m_bSelect = bSel;

	if ( m_bSelect )
	{
		if( m_Action == GLAT_IDLE )
			TurnAction ( GLAT_CONFT_END );

		m_bIdleReserv = FALSE;
	}
	else
	{
		if( bReserv )
			m_bIdleReserv = TRUE;
		else
			TurnAction ( GLAT_IDLE );
	}
}

void DxCharSham::TurnAction ( EMACTIONTYPE toAction )
{
	//	Note : ���� �׼� ���.
	//
	//switch ( m_Action )
	//{
	//case GLAT_IDLE:			break;
	//case GLAT_CONFT_END:	break;
	//case GLAT_ATTACK:		break;
	//default:				break;
	//};

	m_Action = toAction;

	//	Note : �׼� �ʱ�ȭ.
	//
	switch ( m_Action )
	{
	case GLAT_IDLE:
		m_bIdleReserv = FALSE;
		m_bPeaceZone = TRUE;
		break;

	case GLAT_CONFT_END:
		m_bPeaceZone = FALSE;
		break;

	case GLAT_ATTACK:
		m_bPeaceZone = FALSE;
		m_fIdleTime = 0.0f;
		break;

	default:
		break;
	};
}

HRESULT DxCharSham::FrameMove ( float fTime, float fElapsedTime )
{
	if ( !m_pSkinChar )										
		return S_FALSE;

	switch ( m_Action )
	{
	case GLAT_IDLE:
		{
			m_fIdleTime += fElapsedTime;

			if ( m_pSkinChar->GETCURMTYPE() == AN_GESTURE )//add vcf
			{
				if ( m_pSkinChar->ISENDANIM() )
					m_pSkinChar->SELECTANI ( AN_PLACID, AN_SUB_NONE );
			}else{
				if( m_pSkinChar->GETCURMTYPE() != AN_PLACID )
					m_pSkinChar->SELECTANI ( AN_PLACID, AN_SUB_NONE );
			}
		}
		break;

	case GLAT_CONFT_END:
		{
			if( m_pSkinChar->GETCURMTYPE() != AN_CONFT_WIN && 
				m_pSkinChar->GETCURMTYPE() != AN_GUARD_N )
				m_pSkinChar->SELECTANI ( AN_CONFT_WIN, AN_SUB_NONE );

			if( m_pSkinChar->ISENDANIM() )
				TurnAction ( GLAT_ATTACK );
		}
		break;

	case GLAT_ATTACK:
		{
			if( m_pSkinChar->GETCURMTYPE() != AN_GUARD_N )		
				m_pSkinChar->SELECTANI ( AN_GUARD_N, GLCHARLOGIC::m_emANISUBTYPE );

			if( m_fIdleTime == m_pSkinChar->GETENDTIME() && m_bIdleReserv )
			{
				TurnAction ( GLAT_IDLE );
				break;
			}

			if( (m_fIdleTime*1000.0f) > (m_pSkinChar->GETENDTIME()/2.0f) )
			{
				m_fIdleTime = m_pSkinChar->GETENDTIME();
			}
			else
			{
				m_fIdleTime += fElapsedTime;
			}
		}
		break;
	};

	if ( m_fDelayFreeAnim > 500 && m_bSelect == FALSE )//add vcf
	{
		switch( RandomNumber( 0 , 10 ) )
		{
		case 0:
			GetSkinChar()->SELECTANI( AN_GESTURE , (EMANI_SUBTYPE) 38 );
			break;
		case 1:
			GetSkinChar()->SELECTANI( AN_GESTURE , (EMANI_SUBTYPE) 46 );
			break;
		case 2:
			GetSkinChar()->SELECTANI( AN_GESTURE , (EMANI_SUBTYPE) 41 );
			break;
		}
		m_fDelayFreeAnim = 0;
	}

	if ( m_bSelect )
	{
		STARGETID sTARID(CROW_PC,m_nID);
		DxEffGroupPlayer::GetInstance().PassiveEffect ( GLCONST_CHAR::strSELECT_CHAR.c_str(), m_matTrans, sTARID );
	}
	else
	{
		if( m_pSkinChar->GETCURMTYPE() == AN_PLACID )//add vcf
			m_fDelayFreeAnim += RandomNumber( 0.0f , 10.0f );
		m_bEffect = FALSE;
		STARGETID sTARID(CROW_PC,m_nID);
		DxEffGroupPlayer::GetInstance().DeletePassiveEffect ( GLCONST_CHAR::strSELECT_CHAR.c_str(), sTARID );
	}

	//	Note : ��Ų ������Ʈ.
	//
	m_pSkinChar->SetPosition ( m_vPos );
	m_pSkinChar->FrameMove ( fTime, fElapsedTime, TRUE, FALSE, !m_bPeaceZone );

	//	Note : ���� ��ġ�� �������� Transform ��Ʈ���� ���.
	//
	D3DXMATRIX matTrans, matYRot;
	D3DXMatrixTranslation ( &matTrans, m_vPos.x, m_vPos.y, m_vPos.z );
	float fThetaY = DXGetThetaYFromDirection ( m_vDir, m_vDirOrig );
	D3DXMatrixRotationY ( &matYRot, fThetaY );

	m_matTrans = matYRot * matTrans;

	//	Note : AABB ���.
	//
	m_vMax = m_vPos + m_vMaxOrg;
	m_vMin = m_vPos + m_vMinOrg;

	return S_OK;
}

HRESULT DxCharSham::Render ( LPDIRECT3DDEVICEQ pd3dDevice, CLIPVOLUME &cv, BOOL bRendAABB )
{
	if ( m_bNeedData && !(m_dwRevData&RECEIVE_CHARDATA) )	return S_FALSE;
	if ( !m_pSkinChar )										return S_FALSE;

	if ( bRendAABB ) EDITMESHS::RENDERAABB ( pd3dDevice, m_vMax, m_vMin );	//	�׽�Ʈ��.

	HRESULT hr;
	hr = m_pSkinChar->Render ( pd3dDevice, m_matTrans );
	if ( FAILED(hr) )	return hr;

	return S_OK;
}

HRESULT DxCharSham::RenderShadow ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( m_bNeedData && !(m_dwRevData&RECEIVE_CHARDATA) )	return S_FALSE;
	if ( !m_pSkinChar )										return S_FALSE;

	//	Note : �׸��� ������.
	//
	DxShadowMap::GetInstance().RenderShadowCharMob ( m_pSkinChar, m_matTrans, pd3dDevice );

	return S_OK;
}

HRESULT DxCharSham::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if ( m_pSkinChar )	
		m_pSkinChar->RestoreDeviceObjects ( pd3dDevice );

	return S_OK;
}

HRESULT DxCharSham::InvalidateDeviceObjects ()
{
	if ( m_pSkinChar )	
		m_pSkinChar->InvalidateDeviceObjects ();

	return S_OK;
}