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
		if (item._iPLToHit == value)
			return true;
	case IPL_TOHIT_CURSE:
		if (item._iPLToHit == value)
			return true;
	case IPL_DAMP:
		if (item._iPLDam == value)
			return true;
	case IPL_DAMP_CURSE:
		if (item._iPLDam == value)
			return true;
	case IPL_TOHIT_DAMP:
		if (item._iPLDam == value)
			// if (item._iPLToHit)
			return true;
	case IPL_TOHIT_DAMP_CURSE:
		if (item._iPLDam == value)
			// if (item._iPLToHit)
			return true;
	case IPL_ACP:
		if (item._iPLAC == value)
			return true;
	case IPL_ACP_CURSE:
		if (item._iPLAC == value)
			return true;
	case IPL_SETAC:
		if (item._iAC == value)
			return true;
	case IPL_AC_CURSE:
		if (item._iAC == value)
			return true;
	case IPL_FIRERES:
		if (item._iPLFR == value)
			return true;
	case IPL_LIGHTRES:
		if (item._iPLLR == value)
			return true;
	case IPL_MAGICRES:
		if (item._iPLMR == value)
			return true;
	case IPL_ALLRES:
		if (item._iPLFR == value && item._iPLLR == value && item._iPLMR == value)
			return true;
	case IPL_SPLLVLADD:
		if (item._iSplLvlAdd == value)
			return true;
	case IPL_CHARGES:
		if (item._iMaxCharges == value)
			return true;
	case IPL_SPELL:
		if (item._iSpell == value)
			return true;
	case IPL_FIREDAM:
		if (item._iFMinDam == value)
			return true;
	case IPL_LIGHTDAM:
		if (item._iLMinDam == value)
			return true;
	case IPL_STR:
		if (item._iPLStr == value)
			return true;
	case IPL_STR_CURSE:
		if (item._iPLStr == value)
			return true;
	case IPL_MAG:
		if (item._iPLMag == value)
			return true;
	case IPL_MAG_CURSE:
		if (item._iPLMag == value)
			return true;
	case IPL_DEX:
		if (item._iPLDex == value)
			return true;
	case IPL_DEX_CURSE:
		if (item._iPLDex == value)
			return true;
	case IPL_VIT:
		if (item._iPLVit == value)
			return true;
	case IPL_VIT_CURSE:
		if (item._iPLVit == value)
			return true;
	case IPL_ATTRIBS:
		if (item._iPLStr == value && item._iPLMag == value && item._iPLDex == value && item._iPLVit == value)
			return true;
	case IPL_ATTRIBS_CURSE:
		if (item._iPLStr == value && item._iPLMag == value && item._iPLDex == value && item._iPLVit == value)
			return true;
	case IPL_GETHIT_CURSE:
		if (item._iPLGetHit == value)
			return true;
	case IPL_GETHIT:
		if (item._iPLGetHit == value)
			return true;
	case IPL_LIFE:
		if (item._iPLHP == value)
			return true;
	case IPL_LIFE_CURSE:
		if (item._iPLHP == value)
			return true;
	case IPL_MANA:
		if (item._iPLMana == value)
			return true;
	case IPL_MANA_CURSE:
		if (item._iPLMana == value)
			return true;
	case IPL_DUR:
		if (item._iMaxDur == value)
			return true;
	case IPL_DUR_CURSE:
		if (item._iMaxDur == value)
			return true;
	case IPL_INDESTRUCTIBLE:
		return true;
	case IPL_LIGHT:
		if (item._iPLLight == value)
			return true;
	case IPL_LIGHT_CURSE:
		if (item._iPLLight == value)
			return true;
	case IPL_FIRE_ARROWS:
		if (item._iFMinDam == value)
			return true;
	case IPL_LIGHT_ARROWS:
		if (item._iLMinDam == value)
			return true;
	case IPL_THORNS:
		return true;
	case IPL_NOMANA:
		return true;
	case IPL_NOHEALPLR:
		return true;
	case IPL_ABSHALFTRAP:
		return true;
	case IPL_KNOCKBACK:
		return true;
	case IPL_3XDAMVDEM:
		return true;
	case IPL_ALLRESZERO:
		return true;
	case IPL_NOHEALMON:
		return true;
	case IPL_STEALMANA:
		if ((item._iFlags |= ISPL_STEALMANA_3 && value == 3) || (item._iFlags |= ISPL_STEALMANA_5 && value == 5))
			return true;
	case IPL_STEALLIFE:
		if ((item._iFlags |= ISPL_STEALLIFE_3 && value == 3) || (item._iFlags |= ISPL_STEALLIFE_5 && value == 5))
			return true;
	case IPL_TARGAC:
		if (item._iPLEnAc == value)
			return true;
	case IPL_FASTATTACK:
		if (item._iFlags |= ISPL_QUICKATTACK && value == 1)
			return true;
		else if (item._iFlags |= ISPL_FASTATTACK && value == 2)
			return true;
		else if (item._iFlags |= ISPL_FASTERATTACK && value == 3)
			return true;
		else if (item._iFlags |= ISPL_FASTESTATTACK && value == 4)
			return true;
	case IPL_FASTRECOVER:
		if (item._iFlags |= ISPL_FASTRECOVER && value == 1)
			return true;
		else if (item._iFlags |= ISPL_FASTERRECOVER && value == 2)
			return true;
		else if (item._iFlags |= ISPL_FASTESTRECOVER && value == 3)
			return true;
	case IPL_FASTBLOCK:
		return true;
	case IPL_DAMMOD:
		if (item._iPLDamMod == value)
			return true;
	case IPL_RNDARROWVEL:
		return true;
	case IPL_SETDAM:
		return true;
	case IPL_SETDUR:
		return true;
	case IPL_FASTSWING:
		return true;
	case IPL_ONEHAND:
		return true;
	case IPL_DRAINLIFE:
		return true;
	case IPL_RNDSTEALLIFE:
		return true;
	case IPL_INFRAVISION:
		return true;
	case IPL_NOMINSTR:
		return true;
	case IPL_INVCURS:
		return true;
	case IPL_ADDACLIFE:
		return true;
	case IPL_ADDMANAAC:
		return true;
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
