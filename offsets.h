#pragma once


struct OffsetStruct {
	DWORD gameBase;
	DWORD powerOffset = 0x29D4B0;      // 1 byte
	DWORD scoreOffset = 0x29BCA0;
	DWORD timeOffset = 0x29BC4C;
	DWORD grazeOffset = 0x29BCB4;
	DWORD usedBombsOffset = 0x69BCC4;
	DWORD bossSecondLifeOffsetView = 0x29BC44;
	DWORD pointItemsOffset = 0x29D4B4;
	DWORD bossLifeOffset = 0xB957C;  // 2 bytes
	DWORD bombCountOffset = 0x69D4BB; // 1 byte 
	DWORD lifeCountOffset = 0x69D4BA; // 1 byte 
	DWORD xPositionOffset = 0x2CAAC8; // maybe
};
OffsetStruct offsets;
