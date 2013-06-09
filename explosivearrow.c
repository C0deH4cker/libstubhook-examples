#include <stubhook.h>
#include <stdbool.h>
#include <stddef.h>

typedef void Level;
typedef void User;
typedef void Minecraft;

typedef struct {
	void* vt;
	float x; // 0x4
	float y; // 0x8
	float z; // 0xc
} Entity;

static Level* level;
static bool inArrowTick, arrowHit;


static void (*Entity$$remove)(void* self);
static void (*Level$$explode)(void* self, void* entity, float x, float y, float z, float r);


SHHook(void*, Arrow$$tick, Entity* self) {
	inArrowTick = true;
	void* ret = _Arrow$$tick(self);
	inArrowTick = false;
	
	if(arrowHit) {
		arrowHit = false;
		// 3.1 is size of TNT explosion
		Level$$explode(level, NULL, self->x, self->y, self->z, 3.1 * 10);
		Entity$$remove(self);
	}
	
	return ret;
}

SHHook(void*, Level$$playSound, Level* self, Entity* entity, void* string, float a, float b) {
	if(inArrowTick)
		arrowHit = true;
	
	return _Level$$playSound(self, entity, string, a, b);
}

SHHook(Entity*, LocalPlayer$$LocalPlayer, Entity* self, Minecraft* minecraft, Level* newLevel, User* user, int a, bool b) {
	level = newLevel;
	return _LocalPlayer$$LocalPlayer(self, minecraft, newLevel, user, a, b);
}

SHConstructor {
	SHAppVersion("0.6.1.0") {
		SHStub(SHAddr(0xa2e58), Entity$$remove);
		SHStub(SHAddr(0xdf8a0), Level$$explode);
		SHStub(SHAddr(0xb8e3c), Arrow$$tick);
		SHStub(SHAddr(0xddae8), Level$$playSound);
		SHStub(SHAddr(0x52ca0), LocalPlayer$$LocalPlayer);
	}
	SHAppVersion("0.7.1.0") {
		SHStub(SHAddr(0xb1a64), Entity$$remove);
		SHStub(SHAddr(0xf0e48), Level$$explode);
		SHStub(SHAddr(0xc87e0), Arrow$$tick);
		SHStub(SHAddr(0xeec88), Level$$playSound);
		SHStub(SHAddr(0x5abc0), LocalPlayer$$LocalPlayer);
	}
	SHOtherVersion {
		return;
	}
	
	SHApplyHook(Arrow$$tick);
	SHApplyHook(Level$$playSound);
	SHApplyHook(LocalPlayer$$LocalPlayer);
}