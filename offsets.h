#pragma once

DWORD gameBase;
DWORD powerOffset = gameBase + 0x29D4B0;
DWORD scoreOffset = gameBase + 0x29BCA0;
DWORD timeOffset  = gameBase + 0x29BC4C;
DWORD grazeOffset = gameBase + 0x29BCB4;
DWORD usedBombsOffset  = gameBase + 0x69BCC4;
DWORD bossSecondLifeOffsetView = gameBase + 0x29BC44;
DWORD pointItemsOffset = gameBase + 0x29D4B4;
DWORD bossLifeOffset   = gameBase + 0xB957C; // 100000 = max value
DWORD bombCountOffset  = gameBase + 0x29D4BA; // 100000 = max value
DWORD lifeCountOffset  = gameBase + 0x29D4BA; // ... 772 = 4 life; 771 = 3 life; 770 = 2 lifes; 769 = 1 life
DWORD xPositionOffset  = gamebase + 0x2CAAC8; // maybe
