#include "model/TodoItem.h"
#include "DetailedWindow.h"
#include "TitleTextArea.h"
#include <AUI/View/Dynamic.h>
#include <AUI/View/AScrollArea.h>
#include <AUI/Util/kAUI.h>
#include <AUI/Util/AMetric.h>
#include <AUI/View/ASpacerFixed.h>



using namespace declarative;

_<AView> todoEditor(const _<TodoItem>& todoItem) {
    return AScrollArea::Builder().withContents(
        Vertical{
            _new<TitleTextArea>("Untitled") AUI_LET {
                it->setCustomStyle({ FontSize { 20_pt }, Expanding {1, 0}, BackgroundSolid { AColor::WHITE } });
                AObject::biConnect(todoItem->title, it->text());
                if (todoItem->description.raw.empty()) {
                    it->focus();
                }
            },
            SpacerFixed{ 10_dp },
            _new<ATextArea>("Text") AUI_OVERRIDE_STYLE { Expanding(), BackgroundSolid { AColor::WHITE } } && todoItem->description
        } AUI_OVERRIDE_STYLE { Padding { 8_dp, 16_dp }
    });
}

DetailedWindow::DetailedWindow(_<TodoItem> todoItem) : AWindow(todoItem->title) {
    setContents(
        Vertical::Expanding{
            experimental::Dynamic { AUI_REACT(todoEditor(todoItem)) } AUI_OVERRIDE_STYLE { Expanding() }
        } 
    );
}