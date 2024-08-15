#include "item.h"

#include <iostream>

#include "../itemdat.h"
#include "../items.h"
#include "../monster.h"
#include "../objects.h"

static void CreateItemsFromObject(int oid)
{
	switch (object[oid]._otype) {
	case OBJ_CHEST1:
	case OBJ_CHEST2:
	case OBJ_CHEST3:
	case OBJ_TCHEST1:
	case OBJ_TCHEST2:
	case OBJ_TCHEST3:
		SetRndSeed(object[oid]._oRndSeed);
		if (setlevel) {
			for (int j = 0; j < object[oid]._oVar1; j++) {
				CreateRndItem(object[oid]._ox, object[oid]._oy, TRUE, TRUE, FALSE);
			}
		} else {
			for (int j = 0; j < object[oid]._oVar1; j++) {
				if (object[oid]._oVar2 != 0)
					CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
				else
					CreateRndUseful(0, object[oid]._ox, object[oid]._oy, TRUE);
			}
		}
		break;
	case OBJ_SARC:
		SetRndSeed(object[oid]._oRndSeed);
		if (object[oid]._oVar1 <= 2)
			CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
		break;
	case OBJ_DECAP:
		SetRndSeed(object[oid]._oRndSeed);
		CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
		break;
	case OBJ_BARREL:
		SetRndSeed(object[oid]._oRndSeed);
		if (object[oid]._oVar2 <= 1) {
			if (object[oid]._oVar3 == 0)
				CreateRndUseful(0, object[oid]._ox, object[oid]._oy, TRUE);
			else
				CreateRndItem(object[oid]._ox, object[oid]._oy, FALSE, TRUE, FALSE);
		}
		break;
	case OBJ_SKELBOOK:
	case OBJ_BOOKSTAND:
		SetRndSeed(object[oid]._oRndSeed);
		if (random_(161, 5) != 0)
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_MISC, IMISC_SCROLL, TRUE, FALSE);
		else
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_MISC, IMISC_BOOK, TRUE, FALSE);
		break;
	case OBJ_BOOKCASEL:
	case OBJ_BOOKCASER:
		SetRndSeed(object[oid]._oRndSeed);
		CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_MISC, IMISC_BOOK, TRUE, FALSE);
		break;
	case OBJ_ARMORSTAND:
	case OBJ_WARARMOR: {
		SetRndSeed(object[oid]._oRndSeed);
		BOOL uniqueRnd = random_(0, 2);
		if (currlevel <= 5) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, TRUE, ITYPE_LARMOR, IMISC_NONE, TRUE, FALSE);
		} else if (currlevel >= 6 && currlevel <= 9) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, uniqueRnd, ITYPE_MARMOR, IMISC_NONE, TRUE, FALSE);
		} else if (currlevel >= 10 && currlevel <= 12) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, ITYPE_HARMOR, IMISC_NONE, TRUE, FALSE);
		} else if (currlevel >= 13 && currlevel <= 16) {
			CreateTypeItem(object[oid]._ox, object[oid]._oy, TRUE, ITYPE_HARMOR, IMISC_NONE, TRUE, FALSE);
		}
	} break;
	case OBJ_WARWEAP:
	case OBJ_WEAPONRACK: {
		SetRndSeed(object[oid]._oRndSeed);
		int weaponType;

		switch (random_(0, 4) + ITYPE_SWORD) {
		case ITYPE_SWORD:
			weaponType = ITYPE_SWORD;
			break;
		case ITYPE_AXE:
			weaponType = ITYPE_AXE;
			break;
		case ITYPE_BOW:
			weaponType = ITYPE_BOW;
			break;
		case ITYPE_MACE:
			weaponType = ITYPE_MACE;
			break;
		}

		if (leveltype > 1)
			CreateTypeItem(object[oid]._ox, object[oid]._oy, TRUE, weaponType, IMISC_NONE, TRUE, FALSE);
		else
			CreateTypeItem(object[oid]._ox, object[oid]._oy, FALSE, weaponType, IMISC_NONE, TRUE, FALSE);
	} break;
	}
}

static void DropAllItems()
{
	MonsterItems = numitems;
	for (int i = 0; i < nummonsters; i++) {
		int mid = monstactive[i];
		if (monster[mid].MType->mtype == MT_GOLEM)
			continue;
		SetRndSeed(monster[mid]._mRndSeed);
		SpawnItem(mid, monster[mid]._mx, monster[mid]._my, TRUE);
	}

	ObjectItems = numitems;
	for (int i = 0; i < nobjects; i++) {
		int oid = objectactive[i];
		CreateItemsFromObject(oid);
	}
}

static bool AffixValueMatches(ItemStruct &item, int affix, bool isPrefix, int value)
{
	int power = isPrefix ? PL_Prefix[affix].PLPower : PL_Suffix[affix].PLPower;

	switch (power) {
	case IPL_TOHIT:
		return item._iPLToHit == value;
	case IPL_TOHIT_CURSE:
		return item._iPLToHit == value;
	case IPL_DAMP:
		return item._iPLDam == value;
	case IPL_DAMP_CURSE:
		return item._iPLDam == value;
	case IPL_TOHIT_DAMP:
		return item._iPLDam == value;
		// if (item._iPLToHit)
	case IPL_TOHIT_DAMP_CURSE:
		return item._iPLDam == value;
		// if (item._iPLToHit)
	case IPL_ACP:
		return item._iPLAC == value;
	case IPL_ACP_CURSE:
		return item._iPLAC == value;
	case IPL_SETAC:
		return item._iAC == value;
	case IPL_AC_CURSE:
		return item._iAC == value;
	case IPL_FIRERES:
		return item._iPLFR == value;
	case IPL_LIGHTRES:
		return item._iPLLR == value;
	case IPL_MAGICRES:
		return item._iPLMR == value;
	case IPL_ALLRES:
		return item._iPLFR == value
		    && item._iPLLR == value
		    && item._iPLMR == value;
	case IPL_SPLLVLADD:
		return item._iSplLvlAdd == value;
	case IPL_CHARGES:
		return item._iMaxCharges == value;
	case IPL_SPELL:
		return item._iSpell == value;
	case IPL_FIREDAM:
		return item._iFMinDam == value;
	case IPL_LIGHTDAM:
		return item._iLMinDam == value;
	case IPL_STR:
		return item._iPLStr == value;
	case IPL_STR_CURSE:
		return item._iPLStr == value;
	case IPL_MAG:
		return item._iPLMag == value;
	case IPL_MAG_CURSE:
		return item._iPLMag == value;
	case IPL_DEX:
		return item._iPLDex == value;
	case IPL_DEX_CURSE:
		return item._iPLDex == value;
	case IPL_VIT:
		return item._iPLVit == value;
	case IPL_VIT_CURSE:
		return item._iPLVit == value;
	case IPL_ATTRIBS:
		return item._iPLStr == value
		    && item._iPLMag == value
		    && item._iPLDex == value
		    && item._iPLVit == value;
	case IPL_ATTRIBS_CURSE:
		return item._iPLStr == value
		    && item._iPLMag == value
		    && item._iPLDex == value
		    && item._iPLVit == value;
	case IPL_GETHIT_CURSE:
		return item._iPLGetHit == value;
	case IPL_GETHIT:
		return item._iPLGetHit == value;
	case IPL_LIFE:
		return item._iPLHP == value;
	case IPL_LIFE_CURSE:
		return item._iPLHP == value;
	case IPL_MANA:
		return item._iPLMana == value;
	case IPL_MANA_CURSE:
		return item._iPLMana == value;
	case IPL_DUR:
		return item._iMaxDur == value;
	case IPL_DUR_CURSE:
		return item._iMaxDur == value;
	case IPL_INDESTRUCTIBLE:
		return true;
	case IPL_LIGHT:
		return item._iPLLight == value;
	case IPL_LIGHT_CURSE:
		return item._iPLLight == value;
	case IPL_FIRE_ARROWS:
		return item._iFMinDam == value;
	case IPL_LIGHT_ARROWS:
		return item._iLMinDam == value;
	case IPL_THORNS:
	case IPL_NOMANA:
	case IPL_NOHEALPLR:
	case IPL_ABSHALFTRAP:
	case IPL_KNOCKBACK:
	case IPL_3XDAMVDEM:
	case IPL_ALLRESZERO:
	case IPL_NOHEALMON:
		return true;
	case IPL_STEALMANA:
		return (value == 3 && item._iFlags & ISPL_STEALMANA_3)
		    || (value == 5 && item._iFlags & ISPL_STEALMANA_5);
	case IPL_STEALLIFE:
		return (value == 3 && item._iFlags & ISPL_STEALLIFE_3)
		    || (value == 5 && item._iFlags & ISPL_STEALLIFE_5);
	case IPL_TARGAC:
		return item._iPLEnAc == value;
	case IPL_FASTATTACK:
		return (value == 1 && (item._iFlags & ISPL_QUICKATTACK))
		    || (value == 2 && (item._iFlags & ISPL_FASTATTACK))
		    || (value == 3 && (item._iFlags & ISPL_FASTERATTACK))
		    || (value == 4 && (item._iFlags & ISPL_FASTESTATTACK));
	case IPL_FASTRECOVER:
		return (value == 1 && (item._iFlags & ISPL_FASTRECOVER))
		    || (value == 2 && (item._iFlags & ISPL_FASTERRECOVER))
		    || (value == 3 && (item._iFlags & ISPL_FASTESTRECOVER));
	case IPL_FASTBLOCK:
		return true;
	case IPL_DAMMOD:
		return item._iPLDamMod == value;
	case IPL_RNDARROWVEL:
	case IPL_SETDAM:
	case IPL_SETDUR:
	case IPL_FASTSWING:
	case IPL_ONEHAND:
	case IPL_DRAINLIFE:
	case IPL_RNDSTEALLIFE:
	case IPL_INFRAVISION:
	case IPL_NOMINSTR:
	case IPL_INVCURS:
	case IPL_ADDACLIFE:
	case IPL_ADDMANAAC:
	case IPL_FIRERESCLVL:
		return true;
	default:
		return false;
	}

	return false;
}

bool ScannerItem::skipLevel(int level)
{
	return level != *Config.target;
}

static inline bool FindPrefix()
{
	return *Config.prefix;
}

static inline bool FindSuffix()
{
	return *Config.suffix;
}

static inline bool FindUnique()
{
	return *Config.uid;
}

bool LocateItem()
{
	DropAllItems();

	POI = { -1, -1 };

	for (int i = 0; i < numitems; i++) {
		int ii = itemactive[i];
		ItemStruct &searchItem = item[ii];
		if (FindUnique) { // Puzzler = 60
			if (searchItem._iMagical != ITEM_QUALITY_UNIQUE || searchItem._iUid != *Config.uid)
				return false;
		} else if (FindPrefix() || FindSuffix()) {
			if (FindPrefix()) {
				uint8_t prefix = *Config.prefix;
				uint32_t prefixval = Config.prefixval.value_or(0);

				if (searchItem._iPrePower != prefix || !AffixValueMatches(searchItem, prefix, true, prefixval))
					return false;
			}

			if (FindSuffix()) {
				uint8_t suffix = *Config.suffix;
				uint32_t suffixval = Config.suffixval.value_or(0);

				if (searchItem._iSufPower != suffix || !AffixValueMatches(searchItem, suffix, false, suffixval))
					return false;
			}
		} else {
			return false;
		}

		POI = { searchItem._ix, searchItem._iy };
		return true;
	}
}

bool ScannerItem::levelMatches(std::optional<uint32_t> levelSeed)
{
	if (!LocateItem())
		return false;

	if (Config.verbose) {
		std::cerr << "Monster Count: " << nummonsters << std::endl;
		for (int i = 0; i < nummonsters; i++) {
			std::cerr << "Monster " << i << ": " << monster[monstactive[i]].mName << " (" << monster[monstactive[i]]._mRndSeed << ")" << std::endl;
		}
		std::cerr << std::endl;
		std::cerr << "Object Count: " << nobjects << std::endl;
		for (int i = 0; i < nobjects; i++) {
			int oid = objectactive[i];
			char objstr[50];
			GetObjectStr(oid, objstr);
			std::cerr << "Object " << i << ": " << objstr << " (" << object[oid]._oRndSeed << ")" << std::endl;
		}
		std::cerr << std::endl;
		std::cerr << "Item Count: " << numitems << std::endl;
		for (int i = 0; i < numitems; i++) {
			std::string prefix = "";
			if (i >= ObjectItems)
				prefix = "Object ";
			else if (i >= MonsterItems)
				prefix = "Monster ";
			std::cerr << prefix << "Item " << i << ": " << item[itemactive[i]]._iIName << " (" << item[itemactive[i]]._iSeed << ")" << std::endl;
		}
	}

	std::cout << sgGameInitInfo.dwSeed << std::endl;

	return true;
}
