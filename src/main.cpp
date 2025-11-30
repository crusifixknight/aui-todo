#include "MainWindow.h"
#include <AUI/Platform/Entry.h>

AUI_ENTRY {
    auto w = _new<MainWindow>();
    w->show();
    return 0;
};