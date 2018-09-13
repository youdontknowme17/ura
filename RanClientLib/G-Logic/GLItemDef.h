#ifndef GLITEMDEF_H_
#define GLITEMDEF_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "../../EngineLib/Meshs/DxPieceDefine.h"

namespace glold
{
	enum GLITEM_ATT_102
	{
		ITEMATT_NOTHING		= 0,	//	���빫�����.
		ITEMATT_SWORD		= 1,	//	�˷�.
		ITEMATT_REV00		= 2,	//	REV00
		ITEMATT_DAGGER		= 3,	//	�ܰ˷�.
		ITEMATT_SPEAR		= 4,	//	â��.
		ITEMATT_BOW			= 5,	//	Ȱ��.
		ITEMATT_THROW		= 6,	//	��ô��.
		ITEMATT_NOCARE		= 7,	//	Ÿ�� ��� ����.	( "GLSKILL" ���� Hand ���� ���� �Ӽ����� ����. )
		ITEMATT_NSIZE		= 7,	//	��������.
		ITEMATT_NEAR		= 4,	//	���� ����.
	};
};

namespace glold_103
{
	enum GLITEM_ATT_103
	{
		ITEMATT_NOTHING		= 0,	//	���빫�����.
		ITEMATT_SWORD		= 1,	//	��.
		ITEMATT_SABER		= 2,	//	��.
		ITEMATT_DAGGER		= 3,	//	�ܰ�.
		ITEMATT_SPEAR		= 4,	//	â.
		ITEMATT_STICK		= 5,	//	������.
		ITEMATT_BOW			= 6,	//	Ȱ.
		ITEMATT_THROW		= 7,	//	��ô.
		ITEMATT_NOCARE		= 8,	//	Ÿ�� ��� ����.	( "GLSKILL" ���� Hand ���� ���� �Ӽ����� ����. )
		ITEMATT_NSIZE		= 8,	//	��������.
		ITEMATT_NEAR		= 5,	//	���� ����.
	};
};

enum EMITEMLEVEL
{
	//same as ran gs
	LEVEL_NORMAL	= 0,
	LEVEL_HIGH_CLASS = 1, //add by CNDev	
	LEVEL_RARE		= 2,	
	LEVEL_HEROIC	= 3,
	LEVEL_ANCIENT	= 4,
	LEVEL_LEGENDARY	= 5,	
	LEVEL_VOTE_ITEM	= 6,	
	LEVEL_PREMIUM_ITEM	= 7,
	LEVEL_CONTRIBUTION_ITEM = 8, //add contributionpoint by CNDev

	LEVEL_NSIZE		= 9
};

enum EMITEM_FLAG
{
	TRADE_SALE				= 0x01,	//	??? ?? ??.
	TRADE_EXCHANGE			= 0x02,	//	??? ?? ??.
	TRADE_THROW				= 0x04,	//	??? ??? ??.
	TRADE_EVENT_SGL			= 0x08,	//	?? ??? ???? ?? ?? ? ?? ?? ??.
	ITEM_DISGUISE			= 0x10,	//	???.
	ITEM_TIMELMT			= 0x20,	//	???.
	ITEM_CHANNEL_ALL		= 0x40, //	?? ??.
	TRADE_LOCKER			= 0x80, //	?? ??
	TRADE_GARBAGE			= 0x100,
	ITEM_OWNERSHIP			= 0x200, //add ownership
	ITEM_POINT_RECOVER		= 0x400, //add point sell
	ITEM_COSTUME_COMBINE	= 0x800, //add jitem
	ITEM_NO_FAIL			= 0x1000, //add nofailups
	ITEM_COLORING			= 0x2000, //add itemcolor

	TRADE_ALL		= TRADE_SALE|TRADE_EXCHANGE|TRADE_THROW,
};

enum EMITEM_HAND
{
	HAND_RIGHT		= 0x0001,	//	������ ��.
	HAND_LEFT		= 0x0002,	//	�޼� ��.
	HAND_BOTH		= 0x0003,	//	�޼�, ������ ��� ����.
};

//	Note : ĳ���� ���ϸ��̼� ����� ������ ����.
//		ĳ���� ���ϸ��̼� �����Ҷ� ���� �����ƿ� ���� ������ ��.
//
enum GLITEM_ATT
{
	ITEMATT_NOTHING			= 0,	
	ITEMATT_SWORD			= 1,	
	ITEMATT_SABER			= 2,	
	ITEMATT_DAGGER			= 3,	
	ITEMATT_SPEAR			= 4,	
	ITEMATT_STICK			= 5,	
	ITEMATT_GAUNT			= 6,	
	ITEMATT_BOW				= 7,	
	ITEMATT_THROW			= 8,	
	ITEMATT_DUAL_GUN		= 9,
	ITEMATT_RAIL_GUN		= 10,
	ITEMATT_PORTAL_GUN		= 11,
	ITEMATT_SCYTHE			= 12,
	ITEMATT_DUALSPEAR		= 13,
	ITEMATT_SHURIKEN		= 14,
	ITEMATT_FIST			= 15,
	ITEMATT_WAND			= 16,
	ITEMATT_CUBE			= 17,
	ITEMATT_WHIP			= 18,
	ITEMATT_HAMMER			= 19,
	ITEMATT_SHIELD			= 20,
	ITEMATT_UMBRELLA		= 21,
	ITEMATT_NOCARE			= 22,
	ITEMATT_SWORDSABER		= 23,
	ITEMATT_GUN				= 24,

	//
	ITEMATT_NEAR		= 7,
	ITEMATT_NSIZE		= 25,	//	��������.
};

enum GLITEM_CONST
{
	ITEM_SZNAME		= 65,	//	item �̸� ���� ����. (NULL) ���� ���� ����.
	ITEM_SZCOMMENT	= 256,	//	item "��������" ���� ����. (NULL) ���� ���� ����.
	ITEM_SKILLS		= 4,	//	"��ų ����Ʈ ����ġ" �� ������ �ִ� ����.
};

enum EMSUIT
{
	SUIT_HEADGEAR			= 0,
	SUIT_UPPER				= 1,
	SUIT_LOWER				= 2,
	SUIT_HAND				= 3,
	SUIT_FOOT				= 4,
	SUIT_HANDHELD			= 5,
	SUIT_NECK				= 6,
	SUIT_WRIST				= 7,
	SUIT_FINGER				= 8,
	SUIT_PET_A				= 9,	// PetData
	SUIT_PET_B				= 10,
	SUIT_VEHICLE			= 11,	// Ż��
	SUIT_VEHICLE_SKIN		= 12,	// Ż�� �� ���� ��Ų
	SUIT_VEHICLE_PARTS_A	= 13,	// Ż�� �� ���� ����
	SUIT_VEHICLE_PARTS_B	= 14,
	SUIT_VEHICLE_PARTS_C	= 15,
	SUIT_VEHICLE_PARTS_D	= 16,
	SUIT_VEHICLE_PARTS_E	= 17,
	SUIT_VEHICLE_PARTS_F	= 18,
	SUIT_WAISTBAND			= 19,	// Belt
	SUIT_EARRING			= 20,	// Earring
	SUIT_ACCESSORIES		= 21,	// Accessory
	SUIT_ORNAMENT			= 22,	// Ornament

	SUIT_NSIZE				= 23

	//SUIT_NSIZE			= 16
};

//	Note : ������ġ.
enum EMSLOT
{
	SLOT_HEADGEAR	= 0,	//	�Ӹ� ����
	SLOT_UPPER		= 1,	//	��ü
	SLOT_LOWER		= 2,	//	��ü
	SLOT_HAND		= 3,	//	��
	SLOT_FOOT		= 4,	//	��
	SLOT_RHAND		= 5,	//	������ ����
	SLOT_LHAND		= 6,	//	�޼� ����
	SLOT_NECK		= 7,	//	�����
	SLOT_WRIST		= 8,	//	�ո�
	SLOT_RFINGER	= 9,	//	������ �հ���
	SLOT_LFINGER	= 10,	//	�޼� �հ���
	SLOT_RHAND_S	= 11,	//	������ ����, �ذ���
	SLOT_LHAND_S	= 12,	//	�޼� ����, �ذ���
	SLOT_VEHICLE	= 13,	//  Ż�� 
	SLOT_LEARRING		= 14,	//SLOT_LACCESSORY		= 14,	
	SLOT_LACCESSORY		= 15,	//SLOT_LEARRING		= 15,	
	SLOT_RACCESSORY		= 16,	//SLOT_WAIST			= 16,	
	SLOT_REARRING		= 17,   //SLOT_ORNAMENT
	SLOT_WAIST			= 18,	//SLOT_RACCESSORY		= 18,

	SLOT_NSIZE_2		= 19,   
	SLOT_NSIZE_S_2		= 19,	
	SLOT_HOLD			= 19,	
	SLOT_TSIZE			= 20,
};

inline EMSUIT SLOT_2_SUIT ( EMSLOT emSlot )
{
	switch( emSlot )
	{
	case SLOT_HEADGEAR: return SUIT_HEADGEAR;
	case SLOT_UPPER:	return SUIT_UPPER;
	case SLOT_LOWER:	return SUIT_LOWER;
	case SLOT_HAND:		return SUIT_HAND;
	case SLOT_FOOT:		return SUIT_FOOT;

	case SLOT_RHAND:	
	case SLOT_RHAND_S:
	case SLOT_LHAND:	
	case SLOT_LHAND_S:	return SUIT_HANDHELD;

	case SLOT_NECK:				return SUIT_NECK;
	case SLOT_WRIST:			return SUIT_WRIST;

	case SLOT_RFINGER:
	case SLOT_LFINGER:			return SUIT_FINGER;

	case SLOT_VEHICLE:			return SUIT_VEHICLE;
	case SLOT_WAIST:			return SUIT_WAISTBAND;

	case SLOT_LEARRING:			return SUIT_EARRING;
	//case SLOT_REARRING:		return SUIT_EARRING;

	case SLOT_LACCESSORY:		
	case SLOT_RACCESSORY:		return SUIT_ACCESSORIES;
	case SLOT_REARRING:		    return SUIT_ORNAMENT;

	

	default:			return SUIT_NSIZE;
	}
}

inline EMPIECECHAR SLOT_2_PIECE ( EMSLOT emSlot )
{
	switch( emSlot )
	{
	case SLOT_HEADGEAR: return PIECE_HEADGEAR;
	case SLOT_UPPER:	return PIECE_UPBODY;
	case SLOT_LOWER:	return PIECE_LOBODY;
	case SLOT_HAND:		return PIECE_GLOVE;
	case SLOT_FOOT:		return PIECE_FOOT;

	case SLOT_RHAND:	
	case SLOT_RHAND_S:	return PIECE_RHAND;
	case SLOT_LHAND:	
	case SLOT_LHAND_S:	return PIECE_LHAND;
	case SLOT_NECK:				return PIECE_NECK;
	case SLOT_WRIST:			return PIECE_WRIST;

	case SLOT_RFINGER:			return PIECE_RFINGER;
	case SLOT_LFINGER:			return PIECE_LFINGER;

	case SLOT_VEHICLE:			return PIECE_VEHICLE;

	case SLOT_WAIST:			return PIECE_WAIST; 

	case SLOT_REARRING:			return PIECE_REARRING;
	case SLOT_LEARRING:			return PIECE_LEARRING; 
	case SLOT_RACCESSORY:		return PIECE_RACCESSORY; 
	case SLOT_LACCESSORY:		return PIECE_LACCESSORY;
	default:					return PIECE_SIZE;
	}
}

inline EMSLOT PIECE_2_SLOT ( EMPIECECHAR emPiece )
{
	switch( emPiece )
	{
	case PIECE_HEAD:		return SLOT_HEADGEAR;
	case PIECE_UPBODY:		return SLOT_UPPER;
	case PIECE_LOBODY:		return SLOT_LOWER;
	case PIECE_GLOVE:		return SLOT_HAND;
	case PIECE_RHAND:		return SLOT_RHAND;
	case PIECE_LHAND:		return SLOT_LHAND;
	case PIECE_FOOT:		return SLOT_FOOT;
	case PIECE_VEHICLE:		return SLOT_VEHICLE;
	case PIECE_HAIR:
	case PIECE_HEADGEAR:	return SLOT_TSIZE;

	case PIECE_NECK:		return SLOT_NECK;
	case PIECE_WRIST:		return SLOT_WRIST;
	case PIECE_RFINGER:		return SLOT_RFINGER;
	case PIECE_LFINGER:		return SLOT_LFINGER;

	case PIECE_REV01:
	case PIECE_REV02:
	case PIECE_REV03:
	case PIECE_SIZE:		return SLOT_TSIZE;

	case PIECE_WAIST:			return SLOT_WAIST;
	case PIECE_REARRING:		return SLOT_REARRING;
	case PIECE_LEARRING:		return SLOT_LEARRING;
	case PIECE_RACCESSORY:		return SLOT_RACCESSORY;
	case PIECE_LACCESSORY:		return SLOT_LACCESSORY;
	default:				return SLOT_TSIZE;
	}
}
/////
//add item preview
inline EMPIECECHAR SUIT_2_PIECE ( EMSUIT emSuit )
{
	switch( emSuit )
	{
	case SUIT_HEADGEAR:		return PIECE_HEAD;
	case SUIT_UPPER:		return PIECE_UPBODY;
	case SUIT_LOWER:		return PIECE_LOBODY;
	case SUIT_HAND:			return PIECE_GLOVE;
	case SUIT_FOOT:			return PIECE_FOOT;
	case SUIT_HANDHELD:		return PIECE_RHAND;
	case SUIT_NECK:			return PIECE_NECK;
	case SUIT_WRIST:		return PIECE_WRIST;
	case SUIT_FINGER:		return PIECE_RFINGER;

	default:			return PIECE_SIZE;
	}
}

enum EMITEM_TYPE
{
	ITEM_SUIT				= 0,
	ITEM_ARROW				= 1,	//	ȭ��.
	ITEM_CURE				= 2,	//	��ǰ.
	ITEM_SKILL				= 3,	//	��ų ���� ����.
	ITEM_RECALL				= 4,	//	��ȯ��.
	ITEM_KEY				= 5,	//	������ ( ���� ).
	ITEM_GRINDING			= 6,	//	������
	ITEM_CHARM				= 7,	//	����.
	ITEM_TICKET				= 8,	//	������.
	ITEM_SKP_RESET			= 9,	//	��ų ����Ʈ ����.
	ITEM_STAT_RESET			= 10,	//	���� ����Ʈ ����.
	ITEM_SKP_STAT_RESET		= 11,	//	��ų, ���� ����Ʈ ����.
	ITEM_BOX				= 12,	//	���� ����.
	ITEM_CLEANSER			= 13,	//	��Ź��.
	ITEM_LOUDSPEAKER		= 14,	//	Ȯ����.
	ITEM_FIRECRACKER		= 15,	//	����.
	ITEM_CHARACTER_CARD		= 16,	//	ĳ���� �߰� ī��.
	ITEM_INVEN_CARD			= 17,	//	�κ� Ȯ�� ī��.
	ITEM_STORAGE_CARD		= 18,	//	â�� Ȯ�� ī��.
	ITEM_STORAGE_CONNECT	= 19,	//	â�� ���� ī��.
	ITEM_PREMIUMSET			= 20,	//	�����̾� ��Ʈ.
	ITEM_PRIVATEMARKET		= 21,	//	���λ��� ���� �Ǹ�.
	ITEM_RANDOMITEM			= 22,	//	���� ������.
	ITEM_DISJUNCTION		= 23,	//	�ڽ��� �и�.
	ITEM_HAIR				= 24,	//	��Ÿ�� ����.
	ITEM_FACE				= 25,	//	�� ����.
	ITEM_QITEM				= 26,	//	? ������.
	ITEM_CD					= 27,	//	CD.
	ITEM_2FRIEND			= 28,	//	ģ������.
	ITEM_CLUBCALL			= 29,	//	Ŭ��ȣ��.
	ITEM_HAIRSHOP			= 30,	//	���� �̿��.
	ITEM_RENAME				= 31,   //  �̸����� ī��.
	ITEM_HAIR_STYLE			= 32,	//  ��Ÿ��
	ITEM_HAIR_COLOR			= 33,   //  ����÷�
	ITEM_REVIVE				= 34,	//  ��ȥ��
	ITEM_PET_CARD			= 35,	//	�� ī��	// PetData
	ITEM_PET_FOOD			= 36,	//	�� ����
	ITEM_PET_RENAME			= 37,	//	�� �̸�����
	ITEM_PET_COLOR			= 38,	//	�� �÷�����
	ITEM_PET_STYLE			= 39,	//	�� ��Ÿ�Ϻ���
	ITEM_PET_SKILL			= 40,	//	�� ��ų
	ITEM_SMS				= 41,	//	SMS���� �߼�
	ITEM_PET_REVIVE			= 42,	//	�� ��Ȱī��
	ITEM_ANTI_DISAPPEAR 	= 43,	//	�ҹ���(�Ҹ����)
	ITEM_REMODEL			= 44,	//	���� ���
	ITEM_VEHICLE			= 45,	//	Ż��
	ITEM_VEHICLE_OIL		= 46,	//  Ż�� �⸧
	ITEM_VIETNAM_ITEMGET	= 47,	//  ��Ʈ�� Ž�� ���� ������ ȹ��
	ITEM_VIETNAM_EXPGET		= 48,	//  ��Ʈ�� Ž�� ���� ����ġ ȹ��	
	ITEM_GENDER_CHANGE		= 49,	//  ���� ���� ī�� 
	ITEM_GARBAGE_CARD		= 50,	//	������ ī��	
	ITEM_TELEPORT_CARD		= 51,	//	��ȯ��	
	ITEM_PET_SKIN_PACK  	= 52,	//  �� ��Ų ��
	ITEM_FACE_STYLE			= 53,	//	�󱼽�Ÿ��
	ITEM_TAXI_CARD			= 54,	//	�ý�ī��
	ITEM_MATERIALS			= 55,	//	��������
	ITEM_NPC_RECALL			= 56,	//	NPC��ȯ ī��
	ITEM_BULLET				= 57,	//	add bullet

	ITEM_DIGEST				= 58,	//jdev pending
	ITEM_DUAL_PETCARD		= 59,
	ITEM_ROSSARY_RE			= 60,
	ITEM_ROSSART_MI			= 61,
	ITEM_BIKE_COLOR			= 62,
	ITEM_BIKE_BOOST			= 63,
	ITEM_NEW_OBLI			= 64,
	ITEM_ITEM_COLOR			= 65,
	ITEM_MAIL_CARD			= 66,
	ITEM_POINT_CARD_A		= 67,
	ITEM_POINT_CARD_B		= 68,
	ITEM_POINT_SHOP_BOOK		= 69,
	ITEM_SUMMON_CARD		= 70,	//	add summoncard
	ITEM_SLOTMATERIAL		= 71,	//	add itemslot
	ITEM_ATTACK_CON			= 72,	//	add itemslot
	ITEM_SEALED_CARD		= 73,	//  add sealcard
	ITEM_WRAPPER			= 74,   //	add wrapper
	ITEM_NSIZE				= 75,	//	��������.
};

enum EMITEM_DRUG
{
	ITEM_DRUG_NUNE			= 0,
	ITEM_DRUG_HP			= 1,	//	ä��
	ITEM_DRUG_MP			= 2,	//	���
	ITEM_DRUG_SP			= 3,	//	Ȱ��
	ITEM_DRUG_HP_MP			= 4,	//	ü��+���
	ITEM_DRUG_MP_SP			= 5,	//	ü��+Ȱ��
	ITEM_DRUG_HP_MP_SP		= 6,	//	ü��+���+Ȱ��
	ITEM_DRUG_CURE			= 7,	//	���� �̻� ġ��.
	ITEM_DRUG_CALL_SCHOOL	= 8,	//	�б���ȯ.
	ITEM_DRUG_CALL_REGEN	= 9,	//	���۱�ȯ.
	ITEM_DRUG_CALL_LASTCALL	= 10,	//	������ȯ.
	ITEM_DRUG_CALL_REVIVE	= 11,	//	��Ȱ���.
	ITEM_DRUG_HP_CURE		= 12,	// ü��+�̻�ġ��
	ITEM_DRUG_HP_MP_SP_CURE	= 13,	// ü��+���+Ȱ��+�̻�ġ��
	ITEM_DRUG_CALL_TELEPORT	= 14,	// Ư�������̵�
	ITEM_DRUG_CP			= 15,
	ITEM_DRUG_EMERGENCY_POTION	= 16, //added by CNDev

	ITEM_DRUG_SIZE			= 17
};

enum EMITEM_ADDON
{
	EMADD_NONE		= 0,
	EMADD_HITRATE	= 1,	//	������
	EMADD_AVOIDRATE	= 2,	//	ȸ����.
	EMADD_DAMAGE	= 3,	//	���ݷ�.
	EMADD_DEFENSE	= 4,	//	����.
	EMADD_HP		= 5,	//	ä�� ����.
	EMADD_MP		= 6,	//	���� ����.
	EMADD_SP		= 7,	//	���׹̳� ����.
	EMADD_STATS_POW	= 8,	//	��.
	EMADD_STATS_STR	= 9,	//	ü��.
	EMADD_STATS_SPI	= 10,	//	����.
	EMADD_STATS_DEX	= 11,	//	��ø.
	EMADD_STATS_INT	= 12,	//	����.
	EMADD_STATS_STA	= 13,	//	�ٷ�.
	EMADD_PA		= 14,	//	����ġ.
	EMADD_SA		= 15,	//	���ġ.
	EMADD_MA		= 16,	//	����ġ.
	EMADD_CP		= 17,	//add cp

	EMADD_SIZE		= 18,
};

enum EMITEM_VAR
{
	EMVAR_NONE			= 0,
	EMVAR_HP			= 1,
	EMVAR_MP			= 2,
	EMVAR_SP			= 3,
	EMVAR_AP			= 4,
	EMVAR_MOVE_SPEED	= 5,
	EMVAR_ATTACK_SPEED	= 6,
	EMVAR_CRITICAL_RATE	= 7,
	EMVAR_CRUSHING_BLOW	= 8, 
	EMVAR_CP			= 9,	//add cp
	EMVAR_CRIT_DMG		= 10,	//add critdgmc
	EMVAR_BLOW_DMG		= 11,	//add blowdmg

	EMVAR_SIZE			= 12,
};

enum EMITEM_QUESTION
{
	QUESTION_NONE			= 0,
	QUESTION_SPEED_UP		= 1,
	QUESTION_CRAZY			= 2,
	QUESTION_ATTACK_UP		= 3,
	QUESTION_EXP_UP			= 4,
	QUESTION_EXP_GET		= 5,
	QUESTION_LUCKY			= 6,
	QUESTION_BOMB			= 7,
	QUESTION_MOBGEN			= 8,
	QUESTION_SPEED_UP_M		= 9,
	QUESTION_MADNESS		= 10,
	QUESTION_ATTACK_UP_M	= 11,
	QUESTION_HEAL			= 12,

	QUESTION_SIZE			= 13
};

//add pk streak
enum EMPKSTREAK
{
	PM_STREAK_NON			= 0,
	PK_FIRST_BLOOD			= 1,
	PK_DOUBLE_KILL			= 2,
	PK_TRIPLE_KILL			= 3,
	PK_ULTRA_KILL			= 4,
	PK_RAMPAGE				= 5,
	PK_KILLING_SPREE		= 6,
	PK_DOMINATING			= 7,
	PK_MEGA_KILL			= 8,
	PK_UNSTOPPABLE			= 9,
	PK_WICKED_SICK			= 10,
	PK_MONSTER_KILL			= 11,
	PK_GODLIKE				= 12,
	PK_HOLY_SHIT			= 13,
	PK_OWNAGE				= 14,

	PK_STREAK_SIZE			= 15
};
enum EMCTFSTREAK
{
	CTF_CAPTURED_SG			= 0,
	CTF_CAPTURED_MP			= 1,
	CTF_CAPTURED_PHX		= 2,

	CTF_STREAK_SIZE			= 3
};
enum TOWERDOWN
{
	TOWER_CAPTURED_SG	=	0,
	TOWER_CAPTURED_PHX	=	1,
	TOWER_CAPTURED_MP	=	2,

	TOWER_DOWN_SIZE		=	3
};
enum EMRANDOM_OPT //add directval
{
	EMR_OPT_NULL				= 0,
	EMR_OPT_DAMAGE_RATE			= 1,	//	���ݷ�.
	EMR_OPT_DEFENSE_RATE		= 2,	//	����.
	EMR_OPT_HIT_RATE			= 3,	//	������.
	EMR_OPT_AVOID_RATE			= 4,	//	ȸ����.
	EMR_OPT_HP					= 5,	//	hp ����.
	EMR_OPT_MP					= 6,	//	mp ����.
	EMR_OPT_SP					= 7,	//	sp ����.
	EMR_OPT_HP_INC_RATE			= 8,	//	hp ������.
	EMR_OPT_MP_INC_RATE			= 9,	//	mp ������.
	EMR_OPT_SP_INC_RATE			= 10,	//	sp ������.
	EMR_OPT_HMS_INC_RATE		= 11,	//	hp, mp, sp ������.
	EMR_OPT_GRIND_DAMAGE		= 12,	//	���ݷ� ����.
	EMR_OPT_GRIND_DEFENSE		= 13,	//	���� ����.
	EMR_OPT_RANGE				= 14,	//	���� ����.
	EMR_OPT_DIS_SP				= 15,	//	sp �Ҹ�.
	EMR_OPT_RESIST				= 16,	//	����.
	EMR_OPT_MOVE_SPEED			= 17,	// �̵��ӵ� ����

	//add directval
	EMR_OPT_DAMAGE				= 18,
	EMR_OPT_DEFENSE				= 19,
	EMR_OPT_HIT					= 20,
	EMR_OPT_AVOID				= 21,
	EMR_OPT_POW					= 22,
	EMR_OPT_VIT					= 23,
	EMR_OPT_INT					= 24,
	EMR_OPT_DEX					= 25,
	EMR_OPT_STM					= 26,
	EMR_OPT_MELEE				= 27,
	EMR_OPT_MESSILE				= 28,
	EMR_OPT_ENERGY				= 29,
	EMR_OPT_HP_POTION			= 30,
	EMR_OPT_MP_POTION			= 31,
	EMR_OPT_SP_POTION			= 32,
	EMR_OPT_CP_GAIN				= 33,	//add cp
	EMR_OPT_CP					= 34,	//add cp
	EMR_OPT_CRIT_DMG			= 35,	//add critdmg
	EMR_OPT_BLOW_DMG			= 36,	//add blowdmg
	EMR_OPT_CRIT_RATE			= 37,	//add critdmg
	EMR_OPT_BLOW_RATE			= 38,	//add blowdmg
	EMR_OPT_SIZE				= 39
};

enum EMCOOL_TYPE
{
	EMCOOL_ITEMID	= 0,	// MID/SID
	EMCOOL_ITEMTYPE	= 1,	// ������ Ÿ��
	EMCOOL_SIZE		= 2,	// ������
};

struct SRANDOM_OPT
{
	WORD	wTYPE;
	short	nVALUE;

	SRANDOM_OPT()
		: wTYPE( EMR_OPT_NULL )
		, nVALUE( 0 )
	{
	}

	float get_float()	{ return nVALUE * 0.01f; }
	float get_int()		{ return nVALUE; }
};

#define USAGE_RANDOM_OPT_SE
#ifdef USAGE_RANDOM_OPT_SE
//#ifdef DAUMTESTPARAM	// �ѱ� �׼������� 2�� ���� �ɼ� ���� ��� ( ���� )
	struct SRANDOM_OPT_SECTION
	{
		float	fRATE;	// ������ �߻� Ȯ��
		float	fHIGH;	// �� �ִ�
		float	fLOW;	// �� �ּ�

		SRANDOM_OPT_SECTION()
			: fRATE( 0.0f )
			, fHIGH( 0.0f )
			, fLOW( 0.0f )
		{
		}
	};

	struct SRANDOM_OPT_SET
	{
		float					fRATE;	// ������ �ɼǴ� �߻� Ȯ��
		WORD					wTYPE;	// �ɼ� ����
		WORD					wSECT;	// ���� ��
		SRANDOM_OPT_SECTION*	pSECT;	// ����

		SRANDOM_OPT_SET()
			: fRATE( 0.0f )
			, wTYPE( EMR_OPT_NULL )
			, wSECT( 0 )
			, pSECT( NULL )
		{
		}

		~SRANDOM_OPT_SET()
		{
			SAFE_DELETE_ARRAY( pSECT );
		}
	};

	struct SRANDOM_GEN
	{
		enum { MAX_SET = 10, MAX_OPT = 4 };

		float			fRATE;				// ���� �ɼ� �߻� Ȯ��
		DWORD			dwSET_NUM;			// ������ ���� �ɼ� ����
		float			fD_point;			// ���� �� �Ҹ��� �� �ִ� Ȯ��
		float			fS_value;			// ���� �� �ʿ��� �ݾ� ���� ( ������ ���ݿ� ���� )
		SRANDOM_OPT_SET	sSET[ MAX_SET ];	// ������ �ɼ� ( ó�� ���� �� ��� )
		SRANDOM_OPT_SET	sSETR[ MAX_SET ];	// ������ �ɼ� ( ���� ���� �� ��� )

		SRANDOM_GEN()
			: fRATE( 0.0f )
			, dwSET_NUM( 0 )
			, fD_point( 0.0f )
			, fS_value( 1.0f )
		{
		}
	};
	#undef USAGE_RANDOM_OPT_SE
#else
	struct SRANDOM_OPT_SET
	{
		float	fRATE;	// ������ �ɼǴ� �߻� Ȯ��
		WORD	wTYPE;	// �ɼ� ����
		float	fLOW;	// �� �ִ�
		float	fHIGH;	// �� �ּ�

		SRANDOM_OPT_SET()
            : fRATE( 0 )
            , wTYPE( EMR_OPT_NULL )
			, fLOW( 0 )
			, fHIGH( 0 )
		{
		}
	};

	struct SRANDOM_GEN
	{
		enum { MAX_SET = 10, MAX_OPT = 4 };

		float			fRATE;			// ���� �ɼ� �߻� Ȯ��
		DWORD			dwSET_NUM;		// ������ ���� �ɼ� ����
		SRANDOM_OPT_SET	sSET[MAX_SET];	// ������ �ɼ�

		float			fD_point;		// DAUMTESTPARAM �ƴϸ� ������ ����
		float			fS_value;		// DAUMTESTPARAM �ƴϸ� ������ ����

		SRANDOM_GEN()
			: fRATE( 0 )
			, dwSET_NUM( 0 )

			, fD_point( 0.0f )			// DAUMTESTPARAM �ƴϸ� ������ ����
			, fS_value( 1.0f )			// DAUMTESTPARAM �ƴϸ� ������ ����
		{
		}
	};
#endif

struct SRANDOM_DATA
{
	enum { NAME_LEN = 64 };
	char			szNAME[ NAME_LEN ];
	SRANDOM_GEN*	pSET;

	SRANDOM_DATA()
		: pSET( NULL )
	{
		memset( szNAME, 0, sizeof(char) * NAME_LEN );
	}
};

struct SRANDOM_DATA_OPER
{
	bool operator() ( const SRANDOM_DATA &lvalue, const SRANDOM_DATA &rvalue )
	{
		return strcmp(lvalue.szNAME,rvalue.szNAME)<0;
	}

	bool operator() ( const SRANDOM_DATA &lvalue, const char *szNAME )
	{
		return strcmp(lvalue.szNAME,szNAME)<0;
	}

	bool operator() ( const char *szNAME, const SRANDOM_DATA &rvalue )
	{
		return strcmp(szNAME,rvalue.szNAME)<0;
	}
};

enum EMGRINDING_CLASS
{
	EMGRINDING_CLASS_ARM	= 0,
	EMGRINDING_CLASS_CLOTH	= 1,

	EMGRINDING_CLASS_SIZE	= 2,
};

enum EMGRINDING_TYPE
{
	EMGRINDING_NUNE				= 0,

	EMGRINDING_DAMAGE			= 1,
	EMGRINDING_DEFENSE			= 2,

	EMGRINDING_RESIST_FIRE		= 3,
	EMGRINDING_RESIST_ICE		= 4,
	EMGRINDING_RESIST_ELEC		= 5,
	EMGRINDING_RESIST_POISON	= 6,
	EMGRINDING_RESIST_SPIRIT	= 7,
	
	EMGRINDING_NSIZE			= 8
};

enum EMGRINDER_TYPE
{
	EMGRINDER_NORMAL			= 0,
	EMGRINDER_HIGH				= 1,
	EMGRINDER_TOP				= 2,

	EMGRINDER_SIZE				= 3,
};

enum EMGRINDING_RS_FLAGS
{
	EMGRINDING_RS_FIRE		= 0x01,
	EMGRINDING_RS_ICE		= 0x02,
	EMGRINDING_RS_ELECTRIC	= 0x04,
	EMGRINDING_RS_POISON	= 0x08,
	EMGRINDING_RS_SPIRIT	= 0x10,

	EMGRINDING_RS_NSIZE		= 5,
	EMGRINDING_NO			= 2,
	EMGRINDING_RS_ALL		= (EMGRINDING_RS_FIRE|EMGRINDING_RS_ICE|EMGRINDING_RS_ELECTRIC|EMGRINDING_RS_POISON|EMGRINDING_RS_SPIRIT)
};

enum EMGRINDING_RS_INDEX
{
	EMGRINDING_RS_FIRE_INDEX		= 0,
	EMGRINDING_RS_ICE_INDEX			= 1,
	EMGRINDING_RS_ELECTRIC_INDEX	= 2,
	EMGRINDING_RS_POISON_INDEX		= 3,
	EMGRINDING_RS_SPIRIT_INDEX		= 4,

	EMGRINDING_RS_INDEX_NSIZE		= 5
};

EMGRINDING_RS_INDEX	GrindRsClassToIndex ( const EMGRINDING_RS_FLAGS emClass );
EMGRINDING_RS_FLAGS	GrindIndexToRsClass ( const EMGRINDING_RS_INDEX	emIndex );

struct SGRIND
{
	EMGRINDING_TYPE	emTYPE;

	DWORD			dwRESIST;
	int				nMin;
	int				nMax;

	SGRIND () :
		emTYPE(EMGRINDING_NUNE),
		dwRESIST(NULL),
		nMin(0),
		nMax(0)
	{
	}
};

struct ITEM_COOLTIME
{
	enum { VERSION = 0x0001 };

	DWORD		dwID;		//	����� ������ ID
	DWORD		dwCoolID;	//	��Ÿ�� ID ( Ÿ�Կ� ���� �ٸ� ) 
	__time64_t	tUseTime;	//	���ð�
	__time64_t	tCoolTime;	//	��밡���� �ð�

	ITEM_COOLTIME()
		: dwID( 0 )
		, dwCoolID ( 0 )
		, tUseTime ( 0 )
		, tCoolTime ( 0 )
	{
	}
};


namespace COMMENT
{
	extern std::string ITEMLEVEL[LEVEL_NSIZE];
	extern std::string ITEMTYPE[ITEM_NSIZE];

	extern std::string ITEMSUIT[SUIT_NSIZE];
	//extern std::string ITEMSLOT[SLOT_NSIZE_S]; // ������.
	//extern std::string ITEMATTACK[ITEMATT_NSIZE+1];
	extern std::string ITEMATTACK[ITEMATT_NSIZE];

	extern std::string ITEMDRUG[ITEM_DRUG_SIZE];

	extern std::string ITEMADDON[EMADD_SIZE];
	
	extern std::string ITEMVAR[EMVAR_SIZE];

	extern std::string ITEMVOL[EMVAR_SIZE]; // by ���
	extern std::string ITEM_QUE_TYPE[QUESTION_SIZE]; // Question Item
	extern std::string ITEM_QUE_VAR1[QUESTION_SIZE];
	extern std::string ITEM_QUE_VAR2[QUESTION_SIZE];

	extern float ITEMVAR_SCALE[EMVAR_SIZE];
	bool IsITEMVAR_SCALE ( EMITEM_VAR emITEM_VAR );

	extern std::string GRINDING_TYPE[EMGRINDING_NSIZE];
	extern std::string GRINDING_RESIST[EMGRINDING_RS_NSIZE];
	extern std::string GRINDING_LEVEL[EMGRINDER_SIZE];

	extern std::string ITEM_RANDOM_OPT[EMR_OPT_SIZE];

	extern DWORD	ITEMCOLOR[LEVEL_NSIZE];

	extern std::string COOLTYPE[EMCOOL_SIZE];

	/*extern std::string SUMMONTYPE[SUMMON_TYPE_SIZE];*/
};

#endif // GLITEMDEF_H_
