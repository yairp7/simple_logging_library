CC_armv7a=~/Reverse/tools/android-21-toolchain/bin/armv7a-linux-androideabi21-clang
CC_x86=~/Reverse/tools/android-21-toolchain/bin/i686-linux-android21-clang

lib-arm32:
	$(CC_armv7a) -shared -fpic logging.c -o bin/arm32/liblogging.so 

lib-x86:
	$(CC_x86) -shared -fpic logging.c -o  bin/x86/liblogging.so

tester-arm32:
	$(CC_armv7a) -g tester.c -o bin/arm32/tester

tester-x86:
	$(CC_x86) -g tester.c -o bin/x86/tester

all: lib-x86 tester-x86 lib-arm32 tester-arm32
	