#include "MainWindow.h"
#include "utils.h"
#include <AUI/Platform/Entry.h>

AUI_ENTRY {
    createThemedStylesheet();
    _new<MainWindow>()->show();
    return 0;
};