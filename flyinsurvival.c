#include <stubhook.h>

typedef struct Abilities {
	bool a;
	bool b;
	bool canFly;
	bool d;
} Abilities;

SHHook(void, SurvivalMode$$initAbilities, void* self, Abilities* abilities) {
	_SurvivalMode$$initAbilities(self, abilities);
	abilities->canFly = true;
}

SHHook(void, LocalPlayer$$causeFallDamage, void* self, float damage) {
	/* Don't cause fall damage. */
	return;
}

SHConstructor {
	SHAppVersion("0.7.2.0") {
		SHPtrHook(SHAddr(0x20c410), SurvivalMode$$initAbilities);
		SHPtrHook(SHAddr(0x206d3c), LocalPlayer$$causeFallDamage);
	}
	SHAppVersion("0.7.1.0") {
		SHPtrHook(SHAddr(0x2053d0), SurvivalMode$$initAbilities);
		SHPtrHook(SHAddr(0x1ffd20), LocalPlayer$$causeFallDamage);
	}
	SHOtherVersion {
		return;
	}
}