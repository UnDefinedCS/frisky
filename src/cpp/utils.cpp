#include <utils.hpp>
#include <windows.h>
#include <intrin.h>

void initialize() {
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
    if (int cpuInfo[4] = {0}; true) {
        __cpuid(cpuInfo, 1);
        bool hypervisorPresent = (cpuInfo[2] & (1 << 31)) != 0;
        if (hypervisorPresent) {
            MessageBox(
                NULL, // Owner window (NULL = no owner)
                "I hate Hypervisors as much as I hate Supervision! >:(", // Message text
                "I don't think so!", // Title of the window
                MB_OK
            );
            return;
        }
    }

    // check bios for VM signatures
    if (char buffer[128]; true) {
        DWORD size = sizeof(buffer);
        if (GetSystemFirmwareTable('RSMB', 0, buffer, size) > 0) {
            std::string bios(buffer);
            if (bios.find("VirtualBox") != std::string::npos ||
                bios.find("VMware") != std::string::npos ||
                bios.find("Hyper-V") != std::string::npos) {
                MessageBox(
                    NULL, // Owner window (NULL = no owner)
                    "Run me on a real machine punk! >:(", // Message text
                    "I don't think so!", // Title of the window
                    MB_OK
                );
                return;
            }
        }
    }

    // env is safe to operate in
    prepLoader();
    runner();

}

void prepLoader() {
    
}
void runner() {
    
}