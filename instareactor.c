#include <stubhook.h>
#include "blocks.h"
/*
blocks.h is just a bunch of enums with block ids.
*/

static int (*Level$$getTile)(void* self, int x, int y, int z);
static void (*Level$$setTile)(void* self, int x, int y, int z, int new);

#define PLACE(_x, _y, _z, _block) Level$$setTile(level, _x, _y, _z, _block)
SHHook(void, EntityTile$$onPlace, void* self, void* level, int x, int y, int z) {
	_EntityTile$$onPlace(self, level, x, y, z);
	
	int new = Level$$getTile(level, x, y, z);
	if(new == NETHER_REACTOR_CORE) {
		PLACE(x-1, y-1, z-1, GOLD_BLOCK);
		PLACE(x-1, y-1, z+1, GOLD_BLOCK);
		PLACE(x+1, y-1, z-1, GOLD_BLOCK);
		PLACE(x+1, y-1, z+1, GOLD_BLOCK);
		PLACE(x-1, y-1, z, COBBLESTONE);
		PLACE(x+1, y-1, z, COBBLESTONE);
		PLACE(x, y-1, z-1, COBBLESTONE);
		PLACE(x, y-1, z+1, COBBLESTONE);
		PLACE(x-1, y, z-1, COBBLESTONE);
		PLACE(x-1, y, z+1, COBBLESTONE);
		PLACE(x+1, y, z-1, COBBLESTONE);
		PLACE(x+1, y, z+1, COBBLESTONE);
		PLACE(x-1, y+1, z, COBBLESTONE);
		PLACE(x+1, y+1, z, COBBLESTONE);
		PLACE(x, y+1, z-1, COBBLESTONE);
		PLACE(x, y+1, z+1, COBBLESTONE);
		PLACE(x, y+1, z, COBBLESTONE);
		PLACE(x-1, y, z, AIR);
		PLACE(x+1, y, z, AIR);
		PLACE(x, y, z-1, AIR);
		PLACE(x, y, z+1, AIR);
		PLACE(x-1, y+1, z-1, AIR);
		PLACE(x-1, y+1, z+1, AIR);
		PLACE(x+1, y+1, z-1, AIR);
		PLACE(x+1, y+1, z+1, AIR);
	}
}

SHConstructor {
	SHAppVersion("0.6.1.0") {
		SHStub(SHAddr(0xdc6d4), Level$$getTile);
		SHStub(SHAddr(0xdde9c), Level$$setTile);
		SHStub(SHAddr(0x15e164), EntityTile$$onPlace);
	}
	SHOtherVersion {
		return;
	}
	
	SHApplyHook(EntityTile$$onPlace);
}