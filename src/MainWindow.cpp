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

_<AView> todoPreview(const _<TodoItem>& todoItem) {
    auto stringOneLinePreview = [](const AString& s) -> AString {
        if (s.empty()) {
            return "Empty";
        }
        return s.restrictLength(100, "").replacedAll('\n', ' ');
    };

    return Vertical {
        Label { .text = AUI_REACT(stringOneLinePreview(todoItem->title)) } AUI_OVERRIDE_STYLE { FontSize { 16_pt }, ATextOverflow::ELLIPSIS },
        Label { .text = AUI_REACT(stringOneLinePreview(todoItem->description))} AUI_OVERRIDE_STYLE { Opacity { 0.7f }, ATextOverflow::ELLIPSIS }
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