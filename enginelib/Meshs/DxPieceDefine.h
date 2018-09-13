#ifndef DXPIECEDEFINE_H_
#define DXPIECEDEFINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DxPieceDefine.h"

enum EMPIECECHAR
{
	PIECE_HEAD		= 0,
	PIECE_UPBODY	= 1,
	PIECE_LOBODY	= 2,
	PIECE_GLOVE		= 3,
	PIECE_RHAND		= 4,
	PIECE_LHAND		= 5,
	PIECE_FOOT		= 6,
	PIECE_HAIR		= 7,
	PIECE_HEADGEAR	= 8,
	PIECE_VEHICLE	= 9,
	PIECE_NECK		= 10,
	PIECE_WRIST		= 11,
	PIECE_RFINGER	= 12,
	PIECE_LFINGER	= 13,
	PIECE_REBOOT_RHAND	=	14,//Extreme Reboot
	PIECE_REBOOT_LHAND	=	15,//Extreme Reboot
	PIECE_LACCESSORY		= 16, //add wing
	PIECE_LEARRING		= 17, //add aura
	PIECE_WAIST		= 18,//add newitem
	PIECE_REARRING	= 19,//add newitem
	PIECE_RACCESSORY	= 20,//add newitem
	PIECE_CON		= 21,//add newitem



	//add upgradeeffect
	PIECE_EFF_UPBODY	= 22,
	PIECE_EFF_LOBODY	= 23,
	PIECE_EFF_GLOVE		= 24,
	PIECE_EFF_FOOT		= 25,
	PIECE_REV01		= 26,
	PIECE_REV02		= 27,
	PIECE_REV03		= 28,
	PIECE_SIZE		= 29
};

enum EMPEACEZONEWEAPON
{
	EMPEACE_WEAPON_RSLOT = 0,
	EMPEACE_WEAPON_WHAL_SLOT = 1,
	EMPEACE_WEAPON_STICK_RSLOT = 2,

	EMPEACE_WEAPON_RGUN_SLOT = 3,
	EMPEACE_WEAPON_LGUN_SLOT = 4,
	EMPEACE_WEAPON_GUN_SLOT = 5,
	EMPEACE_WEAPON_RSPEAR_SLOT = 6,
	EMPEACE_WEAPON_LSPEAR_SLOT = 7,
	EMPEACE_WEAPON_SIDE_SLOT = 8,
	EMPEACE_WEAPON_KUNAI_SLOT = 9,

	EMPEACE_WEAPON_USER_SLOT = 10,
	EMPEACE_WEAPON_WHIP_SLOT	=	11,
	EMPEACE_WEAPON_BOX_SLOT	=	12,

	EMPEACE_WEAPON_SIZE = 13,
};

namespace SKINTYPE_STING
{
	const TCHAR	m_szWHEREBACKTYPE[EMPEACE_WEAPON_SIZE][64] =
	{
		_T("slot_rhand"),
		_T("whal_slot"),
		_T("stick_rslot"),

		_T("r_gun_slot"),
		_T("l_gun_slot"),
		_T("gun_slot"),
		_T("r_spear_slot"),
		_T("l_spear_slot"),
		_T("side_slot"),
		_T("kunai_slot"),
		_T("user_slot"),
		_T("whip_slot"),
		_T("box_slot"),
	};

	const TCHAR	m_szPIECETYPE[PIECE_SIZE][64] =
	{
		_T("Piece Head"),
		_T("Piece Upbody"),
		_T("Piece LowBody"),
		_T("Piece Glove"),
		_T("Piece R-Hand"),
		_T("Piece L-Hand"),
		_T("Piece Foot"),
		_T("Pisce Hair"),
		_T("Piece Headgear"),
		_T("Piece Vehicle"),
		_T("Piece Neck"),
		_T("Piece Wrist"),
		_T("Piece R-Finger"),
		_T("Piece L-Finger"),
		_T("Piece Wing"),
		_T("Piece Aura"),
		_T("Piece Belt"),
		_T("Piece Earring"),
		_T("Piece Accesory"),
		_T("Piece Consumable"),
		_T("Piece REV01"),
		_T("Piece REV02"),
		_T("Piece REV03"),
	};
};

#endif // DXPIECEDEFINE_H_