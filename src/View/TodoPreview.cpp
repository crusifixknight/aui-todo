#include "TodoPreview.h"
#include "Model/TodoItem.h"
#include <AUI/ASS/Property/Expanding.h>
#include <AUI/ASS/Property/FontSize.h>
#include <AUI/ASS/Property/Opacity.h>
#include <AUI/Util/Declarative/Containers.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/ASpacerFixed.h>

using namespace declarative;

_<AView> todoPreview(const _<TodoItem>& todoItem) {
    auto stringOneLineTitlePreview = [](const AString& s) -> AString {
        if (s.empty()) {
            return "Empty";
        }
        return s.restrictLength(40, "...").replacedAll('\n', ' ');
    };

    auto stringOneLinePreview = [](const AString& s) -> AString {
        if (s.empty()) {
            return "Empty";
        }
        return s.restrictLength(60, "...").replacedAll('\n', ' ');
    };

    return Vertical::Expanding {
        Label { .text = AUI_REACT(stringOneLineTitlePreview(todoItem->title)) } AUI_OVERRIDE_STYLE {
          FontSize { 16_pt }, ATextOverflow::CLIP },
        Horizontal {
          Label { .text = AUI_REACT(stringOneLinePreview(todoItem->description)) } AUI_OVERRIDE_STYLE {
            Opacity { 0.7f }, ATextOverflow::ELLIPSIS, Expanding {} },
          Label { "Created: {:%d.%m.%Y}"_format(todoItem->date) } AUI_OVERRIDE_STYLE { Opacity { 0.7f } },
          SpacerFixed { 10_dp } }
    };
}