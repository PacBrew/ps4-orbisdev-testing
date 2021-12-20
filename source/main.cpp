#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ps4sdk.h>
#include <debugnet.h>

#include <zlib.h>

extern "C" int sceSystemServiceLoadExec(const char *path, const char *args[]);

bool loadModules() {

    int ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
    if (ret != 0) {
        return false;
    }

    ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_NET);
    if (ret != 0) {
        return false;
    }

    ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_USER_SERVICE);
    if (ret != 0) {
        return false;
    }

    ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PAD);
    if (ret != 0) {
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {

    SceUserServiceUserId userId;
    ScePadData padDataCurrent;
    ScePadData padDataLast;
    int padHandle, ret;

    loadModules();
    debugNetInit("192.168.0.187", 18194, 3);

    int param = 700;
    ret = sceUserServiceInitialize(&param);
    debugNetPrintf(DEBUGNET_INFO, "sceUserServiceInitialize: 0x%8x\n", ret);

    ret = scePadInit();
    debugNetPrintf(DEBUGNET_INFO, "scePadInit: 0x%8x\n", ret);

    ret = sceUserServiceGetInitialUser(&userId);
    debugNetPrintf(DEBUGNET_INFO, "sceUserServiceGetInitialUser: 0x%8x\n", ret);

    padHandle = scePadOpen(userId, 0, 0, nullptr);
    debugNetPrintf(DEBUGNET_INFO, "padHandle: 0x%8x\n", ret);

    // test zlib
    debugNetPrintf(DEBUGNET_INFO, "testing zlib...\n");
    gzFile fi = gzopen("/data/file.gz", "wb");
    ret = gzwrite(fi, "ps4-orbisdev-zlib-test", strlen("ps4-orbisdev-zlib-test"));
    gzclose(fi);
    debugNetPrintf(DEBUGNET_INFO, "testing zlib: wrote %i bytes\n", ret);
    // test zlib

    memset(&padDataCurrent, 0, sizeof(ScePadData));
    memset(&padDataLast, 0, sizeof(ScePadData));

    while (true) {
        memcpy(&padDataLast, &padDataCurrent, sizeof(ScePadData));
        ret = scePadReadState(padHandle, &padDataCurrent);
        if (ret == 0) {
            if (padDataCurrent.buttons != padDataLast.buttons) {
                ret = sceSystemServiceLoadExec("/data/self/system/common/lib/ps4_orbisdev_test.self", nullptr);
                debugNetPrintf(DEBUGNET_INFO, "sceSystemServiceLoadExec: 0x%8x\n", ret);
                sleep(1);
                break;
            }
        }
    }

    scePadClose(padHandle);
    debugNetFinish();

    sleep(3);
    exit(EXIT_SUCCESS);
}