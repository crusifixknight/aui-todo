#include "MainWindow.h"
#include <AUI/Util/UIBuildingHelpers.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/AButton.h>
#include <AUI/Platform/APlatform.h>
#include <AUI/View/ADrawableView.h>
#include <AUI/View/AProgressBar.h>
#include <AUI/Platform/AWindow.h>

using namespace declarative;

MainWindow::MainWindow() : AWindow("Hello World", 300_dp, 200_dp) {
    setContents(
        Centered {
            Vertical {
                Centered { Label { "Hello world" },
                }
            }
        }
    );
}
