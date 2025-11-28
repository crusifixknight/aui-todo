#include "MainWindow.h"
#include <AUI/Util/UIBuildingHelpers.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/AButton.h>
#include <AUI/Platform/AWindow.h>
#include <AUI/View/AHDividerView.h>
#include <AUI/View/ATextField.h>
#include <AUI/View/ASpacerFixed.h>
#include <AUI/View/AListView.h>

using namespace declarative;


MainWindow::MainWindow() : AWindow("Todo Application", 700_dp, 600_dp) {
    setContents(
        Stacked { 
          Vertical::Expanding {
            Centered{
              Label { "TODO List" } AUI_WITH_STYLE { FontSize { 25_dp } }
            },
            _new<ASpacerFixed>(30_dp),
            _new<AHDividerView>,
            _new<ASpacerFixed>(15_dp),
            Horizontal{
              _new<ATextField>() AUI_WITH_STYLE { Expanding {1} },
              _new<AButton>("ADD"),
            },
            _new<AListView>() ,
          }

      }
    );
}
