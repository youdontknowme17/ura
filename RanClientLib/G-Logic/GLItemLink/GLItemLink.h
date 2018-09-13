///////////////////////////////////////////////////////////////////////////////
// GLItemLink.h
//
// struct SNETITEM
// itemlink item structure
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GLITEMLINK_H_
#define GLITEMLINK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//originally sitemcustom
//changes in sitemcustom must be applied here too
#include "../EngineLib/G-Logic/GLDefine.h"

enum 
{
	MAX_ITEMLINK_NAME = 70,
};

struct SITEMLINK
{
public:
	bool		bUSE;
	SNATIVEID	sNativeID;
	SNATIVEID	nidDISGUISE;
	__time64_t	tBORNTIME;
	__time64_t	tDISGUISE;

	LONGLONG	lnGenNum;
	WORD		wTurnNum;
	BYTE		cGenType;
	BYTE		cChnID;

	BYTE		cFieldID;

public:
	BYTE		cDAMAGE;
	BYTE		cDEFENSE;
	BYTE		cRESIST_FIRE;
	BYTE		cRESIST_ICE;
	BYTE		cRESIST_ELEC;
	BYTE		cRESIST_POISON;
	BYTE		cRESIST_SPIRIT;

public:
	BYTE		cOptTYPE1;
	BYTE		cOptTYPE2;
	BYTE		cOptTYPE3;
	BYTE		cOptTYPE4;

	short		nOptVALUE1;
	short		nOptVALUE2;
	short		nOptVALUE3;
	short		nOptVALUE4;

public:
	DWORD		dwPetID;
	DWORD		dwVehicleID;

	bool		bVietnamGainItem;

	char		szNAME[MAX_ITEMLINK_NAME];

	SITEMLINK()
		: bUSE( false )
		,sNativeID(false)
		, nidDISGUISE(false)
		, tBORNTIME(0)
		, tDISGUISE(0)
		, lnGenNum(0)
		, wTurnNum(1)
		, cGenType(0)
		, cChnID(0)

		, cFieldID(0)

		, cDAMAGE(0)
		, cDEFENSE(0)
		, cRESIST_FIRE(0)

		, cRESIST_ICE(0)
		, cRESIST_ELEC(0)
		, cRESIST_POISON(0)
		, cRESIST_SPIRIT(0)

		, cOptTYPE1(0)
		, cOptTYPE2(0)
		, cOptTYPE3(0)
		, cOptTYPE4(0)

		, nOptVALUE1(0)
		, nOptVALUE2(0)
		, nOptVALUE3(0)
		, nOptVALUE4(0)

		, dwPetID(0)
		, dwVehicleID( 0 )
		, bVietnamGainItem( false )
	{
		StringCchCopy( szNAME, MAX_ITEMLINK_NAME, "" );
	}

	void	BuildFilter( CString strITEMNAME )
	{
		CString str;
		str += "<";
		str += strITEMNAME.GetString();
		str += ">";

		StringCchCopy( szNAME, MAX_ITEMLINK_NAME, str.GetString() );
	}

	CString GetFilter()
	{
		CString str = szNAME;
		return str;
	}

	SITEMLINK& operator= ( const SITEMLINK &value )
	{
		bUSE			= value.bUSE;
		sNativeID		= value.sNativeID;
		nidDISGUISE		= value.nidDISGUISE;
		tBORNTIME		= value.tBORNTIME;
		tDISGUISE		= value.tDISGUISE;

		lnGenNum		= value.lnGenNum;
		wTurnNum		= value.wTurnNum;
		cGenType		= value.cGenType;
		cChnID			= value.cChnID;

		cFieldID		= value.cFieldID;

		cDAMAGE			= value.cDAMAGE;
		cDEFENSE		= value.cDEFENSE;
		cRESIST_FIRE	= value.cRESIST_FIRE;
		cRESIST_ICE		= value.cRESIST_ICE;
		cRESIST_ELEC	= value.cRESIST_ELEC;
		cRESIST_POISON	= value.cRESIST_POISON;
		cRESIST_SPIRIT	= value.cRESIST_SPIRIT;

		cOptTYPE1		= value.cOptTYPE1;
		cOptTYPE2		= value.cOptTYPE2;
		cOptTYPE3		= value.cOptTYPE3;
		cOptTYPE4		= value.cOptTYPE4;

		nOptVALUE1		= value.nOptVALUE1;
		nOptVALUE2		= value.nOptVALUE2;
		nOptVALUE3		= value.nOptVALUE3;
		nOptVALUE4		= value.nOptVALUE4;

		dwPetID			= value.dwPetID;
		dwVehicleID		= value.dwVehicleID;

		bVietnamGainItem	= value.bVietnamGainItem;

		StringCchCopy( szNAME, MAX_ITEMLINK_NAME, value.szNAME );

		return *this;

	}
};


#endif // GLITEMLINK_H_




//                                      /|
//                                      ||
//                                      ||
//                                      ||
//                                      ||
//       .--._                          ||
//  _.-"" (   /                         ||                        .-.
// (_.-(   `. \                         ||                        )  `-._
//(  (   __.   \                        ||                   __.-' ` .)._)
//(  (  _.-'  `.`-._                    ||               _.-"    )  )     )
// (  (   (         `.                  ||            .-'   .'  `.   )` - .)
// ((  .-'   .-'  `.  `-.               ||          .'   .'     )  ) - . )  )
//  (  ( .-'       `-.   \            __/\__       / .'     `-.   `. )     )
//   `. ( _.-'         `. `.          _(  |       :     `-.       )   `-.  )
//    (   (   .'.-'         `.       /  ) /     .'  .'`.     `-.     ) _.-'
//    (.-' (      .-'    `.   `.    | ,'| |   .' .'                )  )
//     (     (     .'        `. `.,' /  |  \.'  : `-._`-.  `-.  `.)`.)
//      `-._  (       .'  .     ,'  /    \  \ .'               )`.  )
//          (      (       .'  /   ,-""-./\  \ `.  `-.  )   )`-._.-'
//           `-.      .'       |  / __.. `|  /               .-'
//              \   .'     .'  | | /_  _\/  / `-. `.    ) .-'
//               `-._.'        | \ )    (|  |            /
//                   `-._   ,; |.' \    /   |   ` .  _.-'
//                       ""-._ /   )`--'|   |  `._.-'
//                            /   /`-..-'  /_..-'
//                           ;  _/        |
//                            ,'          `.
//                            |            |
//                            `.__.-' '.__.'
//                               `.       |
//                                 \       `.
//                                  |         .
//                                  |          \
//                                  |           .
//                                 ,'           |
//                                .'            |
//                               ,'             |
//                              .'             /
//                              |              /
//                              |             |
//                              |             /
//                             |             |
//                             |             |
//                            /               \
//                           /    /            |
//                          /           .:     |
//                         /    .      .:      |
//                        /    .:      .:     ,'
//                      ,'    .:      .::__.-|
//                     -..__   : __,,..-' \  |
//                       /  |''''         /  (
//                     .'  ,'             \_  `.
//                     `..'                 `.__;