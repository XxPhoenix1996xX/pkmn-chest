#include "PK3.hpp"
#include "random.hpp"
#include "speciesConverter.hpp"

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
