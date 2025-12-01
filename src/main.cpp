#include "MainWindow.h"
#include <AUI/Platform/Entry.h>

AUI_ENTRY {
    _new<MainWindow>()->show();
    return 0;
};