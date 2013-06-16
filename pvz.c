/*
Version: 1.9.6
Device: iPad
Author: HackJack
App: Plants vs Zombies 
How to compile:
gcc -dynamiclib -o PvZ.dylib pvz.c -lstubhook
 
Make a substrate filter plist and set the bundle id to 
com.popcap.PvZiPad. This Dylib and the filter plist should be
installed to /Library/MobileSubstrate/DynamicLibraries/ just like
a normal tweak. However, libstubhook mods do not need to link
against libsubstrate.dylib. Also, libstubhook mods
obviously need to have libstubhook as a depend in the control
file, so don't forget that.
*/

//include stubhook header to be able to use stubhook
#include <stubhook.h>

typedef void SeedType;
typedef void StoreItem;
typedef void Plant;

static SeedType *theSeedType;
static StoreItem *storeItem;

//define function pointers to the internal PvZ methods we want to call later.
static int (*Plant$$getCost)(void *self, SeedType *seedType, SeedType *seedTypeX);
static void (*Plant$$setSleeping)(void *self, bool sleeping);
static bool (*Plant$$isNocturnal)(void *self);
static void (*Zombie$$ApplyBurn)(void *self);
static void (*Coin$$Collect)(void *self);
static int (*Store$$getItemCost)(void *self, StoreItem *storeItem);
static int (*Plant$$GetRefreshTime)(void *self, SeedType *seedType, SeedType *seedTypeX);
static bool (*LawnApp$$CanShowStore)(void *self);
static bool (*LawnApp$$CanShowAlmanac)(void *self);
static bool (*LawnApp$$CanShowStore)(void *self);
static bool (*StoreScreen$$CanAffordItem)(void *self, StoreItem *storeitem);
static bool (*StoreScreen$$isItemSoldOut)(void *self, StoreItem *storeItem);
static void (*Board$$AddSunMoney)(void *self, int sunMoney);
static void (*Board$$DrawFog)(void *self, void *graphics);
static void (*Board$$AddACrater)(void *self, int x, int y);
static void (*Board$$AddALadder)(void *self, int x, int y);

//hooks the method that is called to get the Plant cost. We'll set the return value to 0, which means the Plant cost is 0
SHHook(int, Plant$$getCost, void *self, SeedType *seedType, SeedType *seedTypeX) {
    
    return 0;
    
}

//The two hooks above check if the plant is sleeping or a night plant, both set to false effects that you can plant night plants at day and they still do their job and do not sleep.
SHHook(void, Plant$$setSleeping, void *self, bool sleeping) {
    return _Plant$$setSleeping(self, false);
}

SHHook(bool, Plant$$isNocturnal, void *self) {
    
    return false;
}

//this hook instantly kills the zombie when it starts eating. You can either let zombies die, freeze them, slow them
SHHook(void, Zombie$$startEating, void *self) {
    
    Zombie$$ApplyBurn(self);
    return _Zombie$$startEating(self);
}

//this hook calls the collect method when a sun or a coin gets dropped. this effects an auto-collect hook
SHHook(void, Coin$$UpdateFall, void *self) {
    
    Coin$$Collect(self);
    return _Coin$$UpdateFall(self);
}

//sets the store cost to 0
SHHook(int, Store$$getItemCost, void *self, StoreItem *storeItem) {
    
    return 0;
}

//the refresh time is how long a plant needs till you can plant it again. settingg it to 0 effects instant planting
SHHook(int, Plant$$GetRefreshTime, void *self, SeedType *seedType, SeedType *seedTypeX) {
    
    return 0;
}

//the three hooks beneath show the store, almanac and zen garden.
SHHook(bool, LawnApp$$CanShowStore, void *self) {
    
    return true;
    
}

SHHook(bool, LawnApp$$CanShowAlmanac, void *self) {
    
    return true;
}

SHHook(bool, LawnApp$$CanShowZen, void *self) {
    
    return true;
}

//does not matter if you have enough money, you can buy items in store, because we set CanAffordItem to always true.
SHHook(bool, StoreScreen$$CanAffordItem, void *self, StoreItem *storeItem) {
    
    return true;
}

//items are never sold out
SHHook(bool, StoreScreen$$isItemSoldOut, void *self, StoreItem *storeitem) {
    
    return false;
}

//this calls the addsunmoney when it normally subtract the money, so you get the money you spent for plants
SHHook(void, Board$$TakeSunMoney, void *self, int sunMoney) {
    
    Board$$AddSunMoney(self,sunMoney);
}

//empty hook means, that the method does not do anything. so the fog will not be drawn
SHHook(void, Board$$DrawFog, void *self, void *graphics) {
    
    
}

//same here, neither craters nor ladders will be added to the board
SHHook(void, Board$$AddACrater, void *self, int x, int y) {
    
    
}

SHHook(void, Board$$AddALadder, void *self, int x, int y) {
    
    
}


SHConstructor {
    
       /*
    if the app version is correct (1.9.6 = CFBundleShortVersion, 4111.0.0 = Version) it will actually stub the function pointer.
 
     Set the address of the function pointers while handling ASLR.
        SHAddr(address) calculates the address with ASLR handled, and
        SHStub sets the function pointer to the address in the first
        parameter.
     */
    SHAppVersion("4111.0.0") {
        
        SHStub(SHAddr(0x11C6C0), Plant$$getCost);
        SHStub(SHAddr(0x124F74), Plant$$setSleeping);
        SHStub(SHAddr(0x11C908), Plant$$isNocturnal);
        
        SHStub(SHAddr(0x1950E0), Zombie$$ApplyBurn);
        SHStub(SHAddr(0x1842A0), Zombie$$startEating);
        
        SHStub(SHAddr(0xB06D8), Coin$$Collect);
        SHStub(SHAddr(0xB29D8), Coin$$UpdateFall);
        
        SHStub(SHAddr(0x165218), Store$$getItemCost);
        
        SHStub(SHAddr(0x11D2EC), Plant$$GetRefreshTime);
    
        SHStub(SHAddr(0xE3308), LawnApp$$CanShowStore);
        SHStub(SHAddr(0xE3360), LawnApp$$CanShowAlmanac);
        SHStub(SHAddr(0xE33E0), LawnApp$$CanShowZen);
        SHStub(SHAddr(0x166F28), StoreScreen$$CanAffordItem);
        SHStub(SHAddr(0x168284), StoreScreen$$isItemSoldOut);
        
        SHStub(SHAddr(0x17AC0), Board$$AddSunMoney);
        SHStub(SHAddr(0x9778), Board$$TakeSunMoney);
        SHStub(SHAddr(0xA484), Board$$DrawFog);
        
        SHStub(SHAddr(0x260C8), Board$$AddACrater);
        SHStub(SHAddr(0x26108), Board$$AddALadder);
        
    }
    
    SHOtherVersion {
        
        /*
         If this is reached, the app is running an unknown
         version. In that case, we should bail out since we don't
         know the correct addresses.
         */
        
        return;
        
    }
    
    /* Now that we set the address, actually apply the hook! */
    SHApplyHook(Plant$$getCost);
    SHApplyHook(Plant$$setSleeping);
    SHApplyHook(Plant$$isNocturnal);
    SHApplyHook(Zombie$$startEating);
    SHApplyHook(Coin$$UpdateFall);
    SHApplyHook(Store$$getItemCost);
    SHApplyHook(Plant$$GetRefreshTime);
    SHApplyHook(LawnApp$$CanShowStore);
    SHApplyHook(LawnApp$$CanShowAlmanac);
    SHApplyHook(LawnApp$$CanShowZen);
    SHApplyHook(StoreScreen$$CanAffordItem);
    SHApplyHook(StoreScreen$$isItemSoldOut);
    SHApplyHook(Board$$TakeSunMoney);
    SHApplyHook(Board$$DrawFog);
    SHApplyHook(Board$$AddACrater);
    SHApplyHook(Board$$AddALadder);
    
}
