#include "PK3.hpp"
#include "langStrings.hpp"
#include "random.hpp"
#include "speciesConverter.hpp"

void PK3::shuffleArray(u8 sv) {
	static const int blockLength = 32;
	u8 index                     = sv * 4;

	u8 cdata[length];
	std::copy(data, data + length, cdata);

	for(u8 block = 0; block < 4; block++) {
		u8 ofs = blockPosition(index + block);
		std::copy(cdata + 8 + blockLength * ofs, cdata + 8 + blockLength * ofs + blockLength, data + 8 + blockLength * block);
	}
}

void PK3::crypt(void) {
	u32 seed = checksum();

	for(int i = 0x08; i < 136; i += 2) {
		seed = seedStep(seed);
		data[i] ^= (seed >> 16);
		data[i + 1] ^= (seed >> 24);
	}

	seed = PID();
	for(u32 i = 136; i < length; i += 2) {
		seed = seedStep(seed);
		data[i] ^= (seed >> 16);
		data[i + 1] ^= (seed >> 24);
	}
}

int PK3::swapBits(int value, int p1, int p2) const {
	int bit1 = (value >> p1) & 1;
	int bit2 = (value >> p2) & 1;
	int x = bit1 ^ bit2;
	x = (x << p1) | (x << p2);
	return value ^ x;
}

bool PK3::japanese(void) const {
	return (egg() || language() == 1);
}

PK3::PK3(u8* dt, bool ekx, bool party) {
	length = party ? 100 : 80;
	data = new u8[length];
	std::fill_n(data, length, 0);

	std::copy(dt, dt + length, data);
	if(ekx)	decrypt();
}

std::shared_ptr<PKX> PK3::clone(void) {
	return std::make_shared<PK3>(data, false, length == 100);
}

Generation PK3::generation(void) const {
	return Generation::THREE;
}

u32 PK3::encryptionConstant(void) const {
	return PID();
}

void PK3::encryptionConstant(u32 v) {
	(void)v;
}

u8 PK3::currentFriendship(void) const {
	return otFriendship();
}
void PK3::currentFriendship(u8 v) {
	otFriendship(v);
}

u8 PK3::currentHandler(void) const {
	return 0;
}
void PK3::currentHandler(u8 v) {
	(void)v;
}

u8 PK3::abilityNumber(void) const {
	return 1 << ((PID() >> 16) & 1);
}
void PK3::abilityNumber(u8 v) {
	if(shiny()) {
		do {
			PID(PKX::getRandomPID(species(), gender(), version(), nature(), alternativeForm(), v, PID(), generation()));
		} while(!shiny());
	} else {
		do {
			PID(PKX::getRandomPID(species(), gender(), version(), nature(), alternativeForm(), v, PID(), generation()));
		} while(shiny());
	}
}

u32 PK3::PID(void) const {
	return *(u32*)(data);
}
void PK3::PID(u32 v) {
	*(u32*)(data) = v;
}

u16 PK3::TID(void) const {
	return *(u16*)(data + 0x04);
}
void PK3::TID(u16 v) {
	*(u16*)(data + 0x04) = v;
}

u16 PK3::SID(void) const {
	return *(u16*)(data + 0x06);
}
void PK3::SID(u16 v) {
	*(u16*)(data + 0x06) = v;
}

std::string PK3::nickname(void) const {
	return StringUtils::getString3(data, 0x08, 10, japanese());
}
void PK3::nickname(const std::string& v) {
	StringUtils::setString(data, v, 0x08, 10, japanese());
}

u8 PK3::language(void) const {
	return data[0x12];
}
void PK3::language(u8 v) {
	data[0x12] = v;
}

bool PK3::flagIsBadEgg(void) const {
	return (data[0x13] & 1) == 1;
}
void PK3::flagIsBadEgg(bool v) {
	data[0x13] = (u8)((data[0x13] & ~2) | (v ? 1 : 0));
}

bool PK3::flagHasSpecies(void) const {
	return (data[0x13] & 2) == 1;
}
void PK3::flagHasSpecies(bool v) {
	data[0x13] = (u8)((data[0x13] & ~2) | (v ? 1 : 0));
}

bool PK3::flagIsEgg(void) const {
	return (data[0x13] & 4) == 1;
}
void PK3::flagIsEgg(bool v) {
	data[0x13] = (u8)((data[0x13] & ~4) | (v ? 1 : 0));
}

std::string PK3::otName(void) const {
	return StringUtils::getString3(data, 0x14, 7, japanese());
}
void PK3::otName(const std::string& v) {
	StringUtils::setString3(data, v, 0x14, 7, japanese());
}

u16 PK3::markValue(void) const {
	return swapBits(data[0x1B], 1, 2);
}
void PK3::markValue(u16 v) {
	data[0x1B] = swapBits(v, 1, 2);
}

u16 PK3::checksum(void) const {
	return *(u16*)(data + 0x1C);
}
void PK3::checksum(u16 v) {
	*(u16*)(data + 0x1C) = v;
}

u16 PK3::sanity(void) const {
	return *(u16*)(data + 0x1E);
}
void PK3::sanity(u16 v) {
	*(u16*)(data + 0x1E) = v;
}

u16 PK3::speciesID3(void) const {
	return *(u16*)(data + 0x20);
}
void PK3::speciesID3(u16 v) {
	*(u16*)(data + 0x20) = v;
}

u16 PK3::species(void) const {
	return SpeciesConverter::getG3Species(speciesID3());
}
void PK3::species(u16 v) {
	speciesID3(SpeciesConverter::setG3Species(v));
	flagHasSpecies(species() > 0);
}

u16 PK3::heldItem(void) const {
	return *(u16*)(data + 0x22);
}
void PK3::heldItem(u16 v) {
	*(u16*)(data + 0x22) = v;
}

u32 PK3::experience(void) const {
	return *(u32*)(data + 0x24);
}
void PK3::experience(u32 v) {
	*(u16*)(data + 0x24) = v;
}

u8 PK3::PPUp(u8 move) const {
	return (data[0x24] >> (move*2) & 3);
}
void PK3::PPUp(u8 move, u8 v) {
	data[0x24] = (u8)((data[0x24] & ~(3 << (move*2))) | v << (move*2));
}

u8 PK3::otFriendship(void) const {
	return data[0x29];
}
void PK3::otFriendship(u8 v) {
	data[0x29] = v;
}

u16 PK3::move(u8 move) const {
	return *(u16*)(data + 0x2C + move * 2);
}
void PK3::move(u8 move, u16 v) {
	*(u16*)(data + 0x2C + move * 2) = v;
}

u8 PK3::PP(u8 move) const {
	return data[0x34 + move];
}
void PK3::PP(u8 move, u8 v) {
	data[0x34 + move] = v;
}

u8 PK3::ev(u8 ev) const {
	return data[0x38 + ev];
}
void PK3::ev(u8 ev, u8 v) {
	data[0x38 + ev] = v;
}

u8 PK3::contest(u8 contest) const {
	return data[0x3E + contest];
}
void PK3::contest(u8 contest, u8 v) {
	data[0x3E + contest] = v;
}

u8 PK3::pkrs(void) const {
	return data[0x44];
}
void PK3::pkrs(u8 v) {
	data[0x44] = v;
}

u8 PK3::pkrsDays(void) const {
	return data[0x44] & 0xF;
}
void PK3::pkrsDays(u8 v) {
	data[0x44] = (u8)((data[0x44] & ~0xF) | v);
}

u8 PK3::pkrsStrain(void) const {
	return data[0x44] >> 4;
}
void PK3::pkrsStrain(u8 v) {
	data[0x44] = (u8)((data[0x44] & 0xF) | v << 4);
}

u16 PK3::metLocation(void) const {
	return data[0x45];
}
void PK3::metLocation(u16 v) {
	data[0x45] = (u8)v;
}

u8 PK3::metLevel(void) const {
	return *(u16*)(data + 0x2C) & 0x7F;
}
void PK3::metLevel(u8 v) {
	*(u16*)(data + 0x2C) = (u16)((*(u16*)(data + 0x2C) & ~0x7F) | v);
}

u8 PK3::version(void) const {
	return (*(u16*)(data + 0x2C) >> 7) & 0xF;
}
void PK3::version(u8 v) {
	*(u16*)(data + 0x2C) = (u16)((*(u16*)(data + 0x2C) & ~0x780) | ((v & 0xF) << 7));
}

u8 PK3::ball(void) const {
	return (*(u16*)(data + 0x2C) >> 11) & 0xF;
}
void PK3::ball(u8 v) {
	*(u16*)(data + 0x2C) = (u16)((*(u16*)(data + 0x2C) & ~0x7800) | ((v & 0xF) << 11));
}

u8 PK3::otGender(void) const {
	return (*(u16*)(data + 0x2C) >> 15) & 1;
}
void PK3::otGender(u8 v) {
	*(u16*)(data + 0x2C) = (u16)((*(u16*)(data + 0x2C) & ~(1 << 15)) | ((v & 1) << 15));
}

u8 PK3::iv(u8 stat) const {
	u32 buffer = *(u32*)(data + 0x48);
	return (u8)((buffer >> 5 * stat) & 0x1F);
}
void PK3::iv(u8 stat, u8 v) {
	u32 buffer = *(u32*)(data + 0x48);
	buffer &= ~(0x1F << 5 * stat);
	buffer |= v << (5 * stat);
	*(u32*)(data + 0x38) = buffer;
}

bool PK3::egg() const {
	return ((*(u32*)(data + 0x48) >> 30) & 1) == 1;
}
void PK3::egg(bool v) {
	*(u32*)(data + 0x48) = (u32)((*(u32*)(data + 0x48) & ~0x40000000) | (u32)(v ? 0x40000000 : 0));
	flagIsEgg(v);
	if(v) {
		nickname("タマゴ");
		language(1);
	}
}

bool PK3::abilityBit(void) const {
	return (*(u32*)(data + 0x48) >> 31) == 1;
}
void PK3::abilityBit(bool v) {
	*(u32*)(data + 0x48) = (*(u32*)(data + 0x48) & 0x7FFFFFFF) | (v ? 1u << 31 : 0u);
}

/* 
 * Ribbons go here
 * I didn't do them yet as they seem to be done differently
 * in gen 3 than gen 4 in PKHeX and so I'll do them later
 */

bool PK3::fatefulEncounter(void) const {
	return (*(u32*)(data + 0x4C) >> 31) == 1;
}
void PK3::fatefulEncounter(bool v) {
	*(u32*)(data + 0x4C) = (*(u32*)(data + 0x4C) & ~(1 << 31)) | (u32)(v ? 1 << 31 : 0);
}

int PK3::partyLevel() const {
	if(length == 136) {
		return -1;
	}
	return *(data + 0x54);
}
void PK3::partyLevel(u8 v) {
	if(length != 80) {
		*(data + 0x54) = v;
	}
}

int PK3::partyCurrHP(void) const {
	if(length == 80) {
		return -1;
	}
	return *(u16*)(data + 0x56);
}
void PK3::partyCurrHP(u16 v) {
	if(length != 80) {
		*(u16*)(data + 0x56) = v;
	}
}

int PK3::partyStat(const u8 stat) const {
	if(length == 80) {
		return -1;
	}
	return *(u16*)(data + 0x58 + stat * 2);
}
void PK3::partyStat(const u8 stat, u16 v) {
	if(length != 80) {
		*(u16*)(data + 0x58 + stat * 2) = v;
	}
}

std::shared_ptr<PKX> PK3::next(void) const {
	time_t t = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&t);

	std::shared_ptr<PKX> pk4 = std::make_shared<PK4>();

	pk4->PID(PID());
	pk4->species(species());
	pk4->TID(TID());
	pk4->SID(SID());
	// pk4->experience(egg() ? Experience.GetEXP(5, Species, 0) : experience()),
	pk4->egg(false);
	pk4->otFriendship(70);
	pk4->markValue(markValue());
	pk4->language(language());
	pk4->ev(0, ev(0));
	pk4->ev(1, ev(1));
	pk4->ev(2, ev(2));
	pk4->ev(3, ev(3));
	pk4->ev(4, ev(4));
	pk4->ev(5, ev(5));
	pk4->contest(0, contest(0));
	pk4->contest(1, contest(1));
	pk4->contest(2, contest(2));
	pk4->contest(3, contest(3));
	pk4->contest(4, contest(4));
	pk4->contest(5, contest(5));
	pk4->move(0, move(0));
	pk4->move(1, move(1));
	pk4->move(2, move(2));
	pk4->move(3, move(3));
	pk4->PPUp(0, PPUp(0));
	pk4->PPUp(1, PPUp(1));
	pk4->PPUp(2, PPUp(2));
	pk4->PPUp(3, PPUp(3));
	pk4->iv(0, iv(0));
	pk4->iv(1, iv(1));
	pk4->iv(2, iv(2));
	pk4->iv(3, iv(3));
	pk4->iv(4, iv(4));
	pk4->iv(5, iv(5));
	pk4->ability(ability());
	pk4->version(version());
	pk4->ball(ball());
	pk4->pkrsStrain(pkrsStrain());
	pk4->pkrsDays(pkrsDays());
	pk4->otGender(otGender());
	pk4->metYear(timeStruct->tm_year - 100);
	pk4->metMonth(timeStruct->tm_mon + 1);
	pk4->metDay(timeStruct->tm_mday);
	pk4->metLocation(0x37); // Pal Park

	// RibbonChampionG3Hoenn = RibbonChampionG3Hoenn,
	// RibbonWinning = RibbonWinning,
	// RibbonVictory = RibbonVictory,
	// RibbonArtist = RibbonArtist,
	// RibbonEffort = RibbonEffort,
	// RibbonChampionBattle = RibbonChampionBattle,
	// RibbonChampionRegional = RibbonChampionRegional,
	// RibbonChampionNational = RibbonChampionNational,
	// RibbonCountry = RibbonCountry,
	// RibbonNational = RibbonNational,
	// RibbonEarth = RibbonEarth,
	// RibbonWorld = RibbonWorld,

	pk4->fatefulEncounter(fatefulEncounter());

	// pk4.RibbonG3Cool			|= RibbonCountG3Cool > 0;
	// pk4.RibbonG3CoolSuper	|= RibbonCountG3Cool > 1;
	// pk4.RibbonG3CoolHyper	|= RibbonCountG3Cool > 2;
	// pk4.RibbonG3CoolMaster	|= RibbonCountG3Cool > 3;
	// pk4.RibbonG3Beauty		|= RibbonCountG3Beauty > 0;
	// pk4.RibbonG3BeautySuper	|= RibbonCountG3Beauty > 1;
	// pk4.RibbonG3BeautyHyper	|= RibbonCountG3Beauty > 2;
	// pk4.RibbonG3BeautyMaster	|= RibbonCountG3Beauty > 3;
	// pk4.RibbonG3Cute			|= RibbonCountG3Cute > 0;
	// pk4.RibbonG3CuteSuper	|= RibbonCountG3Cute > 1;
	// pk4.RibbonG3CuteHyper	|= RibbonCountG3Cute > 2;
	// pk4.RibbonG3CuteMaster	|= RibbonCountG3Cute > 3;
	// pk4.RibbonG3Smart		|= RibbonCountG3Smart > 0;
	// pk4.RibbonG3SmartSuper	|= RibbonCountG3Smart > 1;
	// pk4.RibbonG3SmartHyper	|= RibbonCountG3Smart > 2;
	// pk4.RibbonG3SmartMaster	|= RibbonCountG3Smart > 3;
	// pk4.RibbonG3Tough		|= RibbonCountG3Tough > 0;
	// pk4.RibbonG3ToughSuper	|= RibbonCountG3Tough > 1;
	// pk4.RibbonG3ToughHyper	|= RibbonCountG3Tough > 2;
	// pk4.RibbonG3ToughMaster	|= RibbonCountG3Tough > 3;


	// Yay for reusing string buffers!
	// var trash = StringConverter345.G4TransferTrashBytes;
	// if (pk4.Language < trash.Length)
	// 	trash[pk4.Language].CopyTo(pk4.Data, 0x48 + 4);
	pk4->nickname(egg() ? Lang::species[species()] : nickname());
	memcpy(pk4->rawData()+0x68, data+0x48, 0x10);
	pk4->otName(otName());

	// Set Final Data
	pk4->metLevel(level());
	// pk4.Gender = PKX.GetGenderFromPID(pk4.Species, pk4.PID);
	pk4->gender(gender());
	pk4->nicknamed(nicknamed());

	// Unown Form
	pk4->alternativeForm(alternativeForm());

	// if(HeldItem > 0) {
	// 	ushort item = ItemConverter.GetG4Item((ushort)HeldItem);
	// 	if (ItemConverter.IsItemTransferable34(item))
	// 		pk4.HeldItem = item;
	// }

	// Remove HM moves
	u16 moves[4] = {move(0), move(1), move(2), move(3)};
	for (int i = 0; i < 4; i++) {
		// if(std::find(banned, banned + 8, moves[i]) != banned + 8) {
		// 	moves[i] = 0;
		// }
		pk4->move(i, moves[i]);
	}
	pk4->fixMoves();

	pk4->refreshChecksum();
	return pk4;
}

