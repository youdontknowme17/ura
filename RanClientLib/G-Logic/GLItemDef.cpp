#include "stdafx.h"
#include "./GLItemDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

EMGRINDING_RS_INDEX GrindRsClassToIndex ( const EMGRINDING_RS_FLAGS emClass )
{
	switch (emClass)
	{
	case EMGRINDING_RS_FIRE:	return EMGRINDING_RS_FIRE_INDEX;
	case EMGRINDING_RS_ICE:		return EMGRINDING_RS_ICE_INDEX;
	case EMGRINDING_RS_ELECTRIC:return EMGRINDING_RS_ELECTRIC_INDEX;
	case EMGRINDING_RS_POISON:	return EMGRINDING_RS_POISON_INDEX;
	case EMGRINDING_RS_SPIRIT:	return EMGRINDING_RS_SPIRIT_INDEX;
	};

	GASSERT(0&&"�߸��� ���׾����� Ŭ�����Դϴ�.");
	return EMGRINDING_RS_FIRE_INDEX;
}

EMGRINDING_RS_FLAGS GrindIndexToRsClass ( const EMGRINDING_RS_INDEX emIndex )
{
	switch (emIndex)
	{
	case EMGRINDING_RS_FIRE_INDEX:		return EMGRINDING_RS_FIRE;
	case EMGRINDING_RS_ICE_INDEX:		return EMGRINDING_RS_ICE;
	case EMGRINDING_RS_ELECTRIC_INDEX:	return EMGRINDING_RS_ELECTRIC;
	case EMGRINDING_RS_POISON_INDEX:	return EMGRINDING_RS_POISON;
	case EMGRINDING_RS_SPIRIT_INDEX:	return EMGRINDING_RS_SPIRIT;
	};

	GASSERT(0&&"�߸��� ���׾����� Ŭ�����Դϴ�.");
	return EMGRINDING_RS_FIRE;
}

namespace COMMENT
{
	std::string ITEMLEVEL[LEVEL_NSIZE] =
	{
		"Common Item",
		"High Class Item",
		"Rare Item",
		"Heroic Item",
		"Ancient Item",
		"Legendary Item",
		"Vote Point Item",
		"Premium Item",
		"Contribution Item"
	};

	std::string ITEMTYPE[ITEM_NSIZE] =
	{
		"Weapon Tool",
		"Arrow",
		"Pharmaceutical",
		"Skill Scroll",
		"Revert's Book",
		"Authentication Book",
		"Enhancer",
		"Magic Incantations",
		"Bus Ticket",
		"Oblivion Potion (Not Used)",
		"Oblivion Potion (Not Used)",
		"Oblivion Potion",
		"Gift Box",
		"Forgetful Detergent",
		"Megaphone",
		"Firecracker",
		"Character Add Card",
		"Inventory Add Card",
		"Locker Add Card",
		"Locker Connection Card",
		"Premium Box",
		"Store Permit Card",
		"Random Box",
		"Costume Detergent",
		"Hair Style Card",
		"Face Transform Card",
		"? Item",
		"Authentication CD",
		"Friend Connection Card",
		"Summon Cluber",
		"Hair Shop Ticket",
		"Name Change Card",
		"Hair Style Card",
		"Hair Color Card",
		"Resurrection Rosary",
		"Pet Card",
		"Pet Food",
		"Pet Name Change Card",
		"Pet Color Change Card",
		"Pet Style Change Card",
		"Pet Skill Scroll",
		"SMS Send",
		"PET Resurrection Card",
		"Safe Guard Rosary",
		"Reform Card",
		"S.R.C",
		"S.R.C Fuel",
		"Item Get Card",
		"Exp Get Card",
		"Sex Change Card",
		"Garbage Card",
		"Map Pass",
		"PET Shape Change",
		"Face Change Card",
		"Taxi Card",
		"Material Item",
		"NPC Call card",
		"Neutron Bullet",
		"Lunchbox",
		"Pet Dual Skill Card",
		"Reinforcement Rosary",
		"Mitigation Rosary",
		"Bike Color Change Card",
		"Bike Boost Card",
		"New Oblivion Potion",
		"Costume Color Change Card",
		"Mailbox Card",
		"Point Card(Refundable)",
		"point card(No Refundable)",
		"Item Mix Scroll",
		"Item Summon Card",
		"Item Slot Material",
		"Item Consume",
		"Sealed Card",
		"Wrapper"
	};

	std::string ITEMSUIT[SUIT_NSIZE] =
	{
		"Hats",
		"Top",
		"Pants",
		"Glove",
		"Shoes",
		"Arms",
		"Necklace",
		"Bracelet",
		"Ring",
		"Pet Costume A",
		"Pet Costume B",
		"S.R.C",
		"S.R.C Skin",
		"S.R.C Parts A",
		"S.R.C Parts B",
		"S.R.C Parts C",
		"S.R.C Parts D",
		"S.R.C Parts E",
		"S.R.C Parts F",
		"Waistband",
		"Earring",
		"Accessories",
	    "Ornament"
	};

	std::string ITEMDRUG[ITEM_DRUG_SIZE] =
	{
		"NONE",
		"HP Recovery",
		"MP Recovery",
		"SP Recovery",
		"HP+MP ReCovery",
		"HP+SP Recovery",
		"HP+MP+SP Recovery",
		"Extraodinary Treatment",
		"Back to School",
		"Back to Starting Point",
		"Back to Before",
		"Revive function",
		"HP Recovery+Extraodinary Treatment",
		"HP+MP+SP Recovery+Extraodinary Treatment",
		"Move to Specific Location",
		"CP Recovery",
		"Emergency Potion",
	};

	//std::string ITEMATTACK[ITEMATT_NSIZE+1] =
	std::string ITEMATTACK[ITEMATT_NSIZE] =
	{
		"Don't Install",
		"Sword",
		"Blade",
		"Dagger",
		"Spear",
		"Club",
		"Gauntlet",
		"Bow",
		"Throw",
		"Dual Gun",
		"Rail Gun",
		"Portal Gun",
		"Scythe",
		"Dual Spear",
		"Shuriken",
		"Fist",
		"Wand",
		"Cube",
		"Whip",
		"Hammer",
		"Shield",
		"Umbrella",
		"No Type Check",
		"Sword/Blade",
		"Portal/Rail Gun",
	};

	std::string ITEMADDON[EMADD_SIZE] =
	{
		"None",
		"Hit Rate",
		"Avoid Rate",
		"Damage",
		"Defence",
		"(HP)Physical Strength",
		"(MP)Magic Power",
		"(SP)Durability",
		"STATS Power",
		"STATS Strength",
		"STATS Spirit",
		"STAT Agility(Dex)",
		"STAT Intelligence",
		"STAT Endurance(SP)",
		"Melee Attack Value",
		"Shoot Attack Value",
		"Spirit Attack Value",
		"(CP)CombatPoint",
	};

	std::string ITEMVAR[EMVAR_SIZE] =
	{
		"None",
		"HP Recovery",
		"MP Recovery",
		"SP Recovery",
		"HP+MP+SP Recovery",
		"Movement Speed",
		"Attack Speed",
		"Critical Rate",
		"Crushing Blow",
		"CP Recovery",
		"Critical Damage",//add critdmg
		"Crushing Blow Damage"//add blowdmg
	};

	std::string ITEMVOL[EMVAR_SIZE] = // by ���
	{
		"None",
		"HP Recovery",
		"MP Recovery",
		"SP Recovery",
		"HP+MP+SP Recovery",
		"Movement Speed",
		"Attack Speed",
		"Critical Rate",
		"Crushing Blow",
		"CP Recovery",
		"Critical Damage",//add critdmg
		"Crushing Blow Damage"//add blowdmg
	};

	std::string ITEM_QUE_TYPE[QUESTION_SIZE] =
	{
		"none",
		"Speed Up",
		"Crazy",
		"Attack Up",
		"Exp Up",
		"Exp Get",
		"Lucky",
		"Bomb!!",
		"Mob Gen",
		"Speed Up Max",
		"Madness",
		"Attack Up Max",
		"Heal",
	};

	std::string ITEM_QUE_VAR1[QUESTION_SIZE] =
	{
		"None",
		"Speed Movement",
		"Speed Movement",
		"Damage",
		"Experience",
		"No",
		"Occurred Rate",
		"Damage",
		"MID",
		"Speed Movement",
		"Speed Movement",
		"Damage",
		"hp"
	};

	std::string ITEM_QUE_VAR2[QUESTION_SIZE] =
	{
		"No",
		"No",
		"Attack Speed",
		"No",
		"No",
		"No",
		"No",
		"No",
		"SID",
		"No",
		"Attack Speed",
		"No",
		"No"
	};

	float ITEMVAR_SCALE[EMVAR_SIZE] =
	{
		1.0f,
		100.0f,
		100.0f,
		100.0f,
		100.0f
	};

	bool IsITEMVAR_SCALE ( EMITEM_VAR emITEM_VAR )
	{
		return ITEMVAR_SCALE[emITEM_VAR]==100.0f;
	};

	std::string GRINDING_TYPE[EMGRINDING_NSIZE] =
	{
		"No",

		"Attack Power",
		"Defend Power",

		"Resistance(Fire)",
		"ResistanceI(Ice)",
		"Resistance(Eletric)",
		"Resistance(Poison)",
		"Resistance(air)",
	};

	std::string GRINDING_LEVEL[EMGRINDER_SIZE] =
	{
		"Low",
		"High",
		"Highest",
	};

	std::string GRINDING_RESIST[EMGRINDING_RS_NSIZE] =
	{
		"Fire",
		"Ice",
		"Electric",
		"Poison",
		"Air",
	};

	//modified as part of add directval project
	std::string ITEM_RANDOM_OPT[EMR_OPT_SIZE] =
	{
		"NULL",
		"DAMAGE(%)",
		"DEFENSE(%)",
		"HITRATE(+)",
		"AVOIDRATE(+)",
		"HP(+)",
		"MP(+)",
		"SP(+)",
		"HP_INC(%)",
		"MP_INC(%)",
		"SP_INC(%)",
		"HMS_INC(%)",
		"GRIND_DAMAGE",
		"GRIND_DEFENSE",
		"ATTACK_RANGE",
		"DIS_SP",
		"RESIST",
		"MoveSpeed",

		"Damage(+)",
		"Defense(+)",
		"HitRate(%)",
		"AvoidRate(%)",
		"Pow(+)",
		"Vit(+)",
		"Int(+)",
		"Dex(+)",
		"Stm(+)",
		"Melee(+)",
		"Messile(+)",
		"Energy(+)",
		"HP Potion(+)",
		"MP Potion(+)",
		"SP Potion(+)",
		"CP Gain(+)",
		"CP(+)",
		"Critical Damage(+)", //add critdmg
		"Crushing Blow Damage(+)", //add blowdmg
		"Critical Rate(+)", //add critdmg
		"Crushing Blow Rate(+)" //add blowdmg

	};


	DWORD	ITEMCOLOR[LEVEL_NSIZE] = 
	{
		0xffff0000,
		0xff00ff00,
		0xff0000ff,
		0xffffff00,
		0xffff00ff
	};


	std::string COOLTYPE[EMCOOL_SIZE] =
	{
		"������Ÿ��",
		"������MID/SID"
	};
	/*std::string SUMMONTYPE[SUMMON_TYPE_SIZE] =
	{
		"SummonType_A"
	};*/
};


