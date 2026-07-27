#ifndef PADDED_IMENU_H
#define PADDED_IMENU_H

#include "RE/Skyrim.h"

namespace Scaleform {
    struct PaddedIMenu : public RE::IMenu {
#ifndef EXCLUSIVE_SKYRIM_VR
        RE::IMenu::VR_RUNTIME_DATA vr_pad;
        void* vr_pad2 = nullptr; // VR IMenu seems to be 0x48 bytes and accesses offset 0x40
#endif
    };
}

#endif // PADDED_IMENU_H
