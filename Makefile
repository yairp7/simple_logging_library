CC_arm=${NDK_HOME}/android-21-arm-toolchain/bin/arm-linux-androideabi-clang
CC_x86=${NDK_HOME}/android-21-x86-toolchain/bin/i686-linux-android-clang

mac-lib-arm32:
	$(CC_arm) -shared -fpic logging.c -o bin/arm32/liblogging.so

mac-lib-x86:
	$(CC_x86) -shared -fpic logging.c -o  bin/x86/liblogging.so

mac-tester-arm32:
	$(CC_arm) -g tester.c -o bin/arm32/tester

mac-tester-x86:
	$(CC_x86) -g tester.c -o bin/x86/tester

setup:
	mkdir bin
	mkdir bin/x86
	mkdir bin/arm32

linux-lib-arm32:
	$(CC_arm) -shared -fpic logging.c -o bin/arm32/liblogging.so

linux-lib-x86:
	$(CC_x86) -shared -fpic logging.c -o  bin/x86/liblogging.so

linux-tester-arm32:
	$(CC_arm) -g tester.c -o bin/arm32/tester

linux-tester-x86:
	$(CC_x86) -g tester.c -o bin/x86/tester

mac: mac-lib-x86 mac-tester-x86 mac-lib-arm32 mac-tester-arm32

linux: linux-lib-x86 linux-tester-x86 linux-lib-arm32 linux-tester-arm32

all: mac linux