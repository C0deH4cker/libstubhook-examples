#include <stubhook.h>

/* Zombies move twice as fast. Scary. */
SHHook(float, Zombie$$getWalkingSpeedModifier, void* self) {
	return 2 * _Zombie$$getWalkingSpeedModifier(self);
}

/* One does not simply harm the undead. */
SHHook(int, Zombie$$getDamageAfterArmorAbsorb, void* self, int amount) {
	return 0;
}

SHConstructor {
	SHAppVersion("0.7.1.0") {
		SHPtrHook(SHAddr(0x2045bc), Zombie$$getWalkingSpeedModifier);
		SHPtrHook(SHAddr(0x2045c0), Zombie$$getDamageAfterArmorAbsorb);
	}
	SHOtherVersion {
		return;
	}
}