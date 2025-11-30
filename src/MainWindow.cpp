#include "MainWindow.h"
#include <AUI/Util/AMetric.h>
#include <AUI/Util/UIBuildingHelpers.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/AButton.h>
#include <AUI/View/ASpacerFixed.h>
#include <AUI/View/AListView.h>
#include <AUI/View/AForEachUI.h>
#include <AUI/Common/APropertyPrecomputed.h>
#include <AUI/ASS/AStylesheet.h>
#include <AUI/Platform/AWindow.h>
#include <AUI/ASS/Property/Padding.h>
#include "DetailedWindow.h"
#include "model/TodoItem.h"
#include<AUI/Json/Conversion.h>
#include <AUI/IO/AFileInputStream.h>
#include <AUI/View/ACheckBox.h>

using namespace declarative;

static constexpr auto LOG_TAG = "Todo's";

AJSON_FIELDS(TodoItem, AJSON_FIELDS_ENTRY(title) AJSON_FIELDS_ENTRY(description) AJSON_FIELDS_ENTRY(date) AJSON_FIELDS_ENTRY(isCompleted))

MainWindow::MainWindow() : AWindow("Todo Application", 700_dp, 600_dp) {
    setExtraStylesheet(AStylesheet {
      {
        t<AWindow>(),
        Padding { 10_dp },
      },
    });
    load();

    setContents( 
            Vertical{
              Centered{
                Horizontal{
                    Button{ Label { "New Note" }, { me::newTodo } },
                    Button{ Label{ "Save" }, { me::save } }
                }
              },
                AScrollArea::Builder()
                  .withContents(
                    AUI_DECLARATIVE_FOR(todoItem, *mTodoItems, AVerticalLayout){ 
                       return Vertical{
                           SpacerFixed { 10_dp },
                           todoPreview(todoItem) AUI_LET { connect(it->clicked, [this, todoItem] {
                               mCurrentTodoItem = todoItem;
                               openDetailed(todoItem);
                           });
                           it& mCurrentTodoItem > [todoItem](AView& view, const _<TodoItem>& currentTodoItem) {
                               ALOG_DEBUG(LOG_TAG)
                                   << "currentTodoItem == todoItem " << currentTodoItem << " == " << todoItem;
                           };
                           }
                       };
                    }
                  ).build() 
            }
    );

}

void MainWindow::newTodo() {
    auto todo = aui::ptr::manage_shared(new TodoItem { .title = "Untitled", .isCompleted = false});
    mTodoItems.writeScope()->push_back(todo);
    openDetailed(todo);
}

void MainWindow::openDetailed(const _<TodoItem>& todoItem) 
{
    _new<DetailedWindow>(todoItem)->show();
}

_<AView> todoPreview(const _<TodoItem> todoItem) {
    auto stringOneLinePreview = [](const AString& s) -> AString {
        if (s.empty()) {
            return "Empty";
        }
        return s.restrictLength(100, "").replacedAll('\n', ' ');
    };

    auto check = _new<AProperty<bool>>(todoItem->isCompleted);
    return Horizontal::Expanding {
        Horizontal {
            Centered { CheckBox {
              .checked = AUI_REACT(todoItem->isCompleted),
              .onCheckedChange = [todoItem](bool checked) { todoItem->isCompleted = checked; } } },
                SpacerFixed { 10_dp },
                Vertical::Expanding {
                Label { .text = AUI_REACT(stringOneLinePreview(todoItem->title)) } AUI_OVERRIDE_STYLE {
                    FontSize { 16_pt }, ATextOverflow::ELLIPSIS
                },
                    Label { .text = AUI_REACT(stringOneLinePreview(todoItem->description)) } AUI_OVERRIDE_STYLE {
                    Opacity { 0.7f }, ATextOverflow::ELLIPSIS
                }
            }
        }
    }
    AUI_OVERRIDE_STYLE { Padding { 10_dp }, BackgroundSolid { AColor::WHITE } };
}


_<AView> minimalCheckBox(_<AProperty<bool>> state) {
    return CheckBox {
        .checked = AUI_REACT(*state),
        .onCheckedChange = [state](bool checked) { *state = checked; },
        .content = Label { "Minimal checkbox" },
    };
}

void MainWindow::save()
{
    AFileOutputStream("todo.json") << aui::to_json(*mTodoItems);
}

void MainWindow::load() {
    try {
        if (!"todo.json"_path.isRegularFileExists()) {
            return;
        }
        aui::from_json(AJson::fromStream(AFileInputStream("todo.json")), mTodoItems);
    } catch (const AException& e) {
        ALogger::info(LOG_TAG) << "Can't load todo list: " << e;
    }
}

