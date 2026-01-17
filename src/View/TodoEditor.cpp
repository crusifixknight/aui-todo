#include "TodoEditor.h"
#include "TitleTextField.h"
#include <AUI/View/ATextArea.h>
#include <AUI/View/AScrollArea.h>
#include <AUI/View/ASpacerFixed.h>

using namespace declarative;

_<AView> todoEditor(const _<TodoItem>& todoItem) {
    return AScrollArea::Builder().withContents(
        Vertical{
            _new<TitleTextField>() AUI_LET {
                it->setCustomStyle({ FontSize { 20_pt }, Expanding {1, 0}, MinSize {28_pt} });
                AObject::biConnect(todoItem->title, it->text());
                if (todoItem->description.raw.empty()) {
                    it->focus();
                }
            },
            SpacerFixed{ 10_dp },
            _new<ATextArea>() AUI_OVERRIDE_STYLE { Expanding(), Padding { 4_dp } } && todoItem->description 
        } AUI_OVERRIDE_STYLE { Padding { 8_dp, 16_dp }
    });
}