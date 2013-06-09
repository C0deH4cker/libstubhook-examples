/*
This example mod is a working mod for the app Minecraft: Pocket
Edition. It changes the physics of sand/gravel so that placing
one of these blocks will create a pillar from the placed position
to the next block below it.

Compile this like so:
gcc -dynamiclib -o sandgravel.dylib sandgravel.c -lstubhook

Make a substrate filter plist and set the bundle id to
com.mojang.minecraftpe. This and the filter plist should be
installed to /Library/MobileSubstrate/DynamicLibraries/ just like
a normal tweak. However, libstubhook mods do not need to link
against libsubstrate.dylib. Also, libstubhook mods
obviously need to have libstubhook as a depend in the control
file, so don't forget that. Although libstubhook mods don't link
against libsubstrate, they still (for now, might be changed
later) depend on having substrate the package installed, so list
that as a depend in the control file as well.
*/
#include <stubhook.h>

/*
Define a few constants to use later in the program. These are
the block ids in MCPE.
*/
enum {
	AIR = 0,
	SAND = 12,
	GRAVEL
};


/*
Define function pointers for internal MCPE methods we want to
call later. We will set the pointer in the constructor.
*/
static int (*Level$$getTile)(void* self, int x, int y, int z);
static void (*Level$$setTile)(void* self, int x, int y, int z, int block);

/*
Hook the method that gets called when a block is either placed by
a player or just changed normally in the world (like a sheep
eating grass calls this with to change from grass to dirt). I
think it calls Level::setTile() first, but that method ends up
calling this one and sending out an update notification to the
level listener afterwards. Best to hook this method.
*/
SHHook(void, Level$$setTileNoUpdate, void* self, int x, int y, int z, int new) {
	/* Grab the old tile's block id */
	int old = Level$$getTile(self, x, y, z);
	
	/* If either sand or gravel was replaced by air */
	if(new == AIR && (old == SAND || old == GRAVEL)) {
		/*
		Instead of setting the block to air, set the block to
		the opposite of sand or gravel. So, if the old block was
		sand, set it to gravel. Otherwise, set it to sand. Useful
		to make the pattern and to help it fall.
		*/
		return Level$$setTile(self, x, y, z, old ^ 1);
	}
	
	/*
	If this is reached, then it means we don't want to change the
	behavior. Just invoke the original behavior.
	*/
	return _Level$$setTileNoUpdate(self, x, y, z, new);
}

/* This is executed as soon as the dylib is loaded into MCPE. */
SHConstructor {
	/*
	Set the address of the function pointers while handling ASLR.
	SHAddr(address) calculates the address with ASLR handled, and
	SHStub sets the function pointer to the address in the first
	parameter.
	*/
	SHAppVersion("0.6.1.0") {
		/*
		If the app's Info.plist has "0.6.1.0" as the value for
		CFBundleVersion, execute this code block.
		*/
		SHStub(SHAddr(0xdde9c), Level$$setTile);
		SHStub(SHAddr(0xdc6d4), Level$$getTile);
		SHStub(SHAddr(0xdc7a8), Level$$setTileNoUpdate);
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
	SHApplyHook(Level$$setTileNoUpdate);
}

