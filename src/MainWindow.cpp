#include "MainWindow.h"
#include <AUI/Util/UIBuildingHelpers.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/AButton.h>
#include <AUI/View/AHDividerView.h>
#include <AUI/View/ATextField.h>
#include <AUI/View/ASpacerFixed.h>
#include <AUI/View/AListView.h>
#include <AUI/View/ASplitter.h>
#include <AUI/View/ACheckBox.h>
#include <AUI/View/AForEachUI.h>
#include <AUI/Reflect/for_each_field.h>
#include <AUI/Common/APropertyPrecomputed.h>
#include <AUI/ASS/AStylesheet.h>
#include <AUI/Platform/AWindow.h>
#include <AUI/Util/AMetric.h>
#include <AUI/ASS/Property/Padding.h>
#include <AUI/ASS/Property/BackgroundSolid.h>
#include "DetailedWindow.h"


using namespace declarative;

static constexpr auto LOG_TAG = "Todo's";

MainWindow::MainWindow() : AWindow("Todo Application", 700_dp, 600_dp) {
    setExtraStylesheet(AStylesheet {
      {
        c(".plain_bg"),
        BackgroundSolid { AColor::WHITE },
      },
      {
        t<AWindow>(),
        Padding { 0 },
      },
    });

    setContents( 
            Vertical{
              Centered{
                Button{ Label { "New Note" }, { me::newTodo } },
              },
                AScrollArea::Builder()
                  .withContents(
                    AUI_DECLARATIVE_FOR(todoItem, *mTodoItems, AVerticalLayout){ 
                       observeChangesForDirty(todoItem);
                       return todoPreview(todoItem) AUI_LET 
                       { 
                           connect(it->clicked, [this, todoItem] {
                               mCurrentTodoItem = todoItem;
                               openDetailed(todoItem);
                           });
                           it& mCurrentTodoItem > [todoItem](AView& view, const _<TodoItem>& currentTodoItem) {
                               ALOG_DEBUG(LOG_TAG) << "currentTodoItem == todoItem " << currentTodoItem << " == " << todoItem; 
                               view.setAssName(".plain_bg", currentTodoItem == todoItem);
                           };
                       };
                    }
                  ).build() 
            }
    );

}

void MainWindow::observeChangesForDirty(const _<TodoItem>& todeItem) {
    aui::reflect::for_each_field_value(
        *todeItem,
        aui::lambda_overloaded {
          [&](auto& field) {},
          [&](APropertyReadable auto& field) {

              AObject::connect(field.changed, me::markDirty);
          },
        });
}

void MainWindow::markDirty() { mDirty = true; }

void MainWindow::newTodo() {
    auto todo = aui::ptr::manage_shared(new TodoItem { .title = "Untitled", .isCompleted = false});
    openDetailed(todo);
}
//_<AView> todoPreview(const _<TodoItem>& todoItem){
//    return Horizontal{
//      CheckBox { // checkBox for complete task
//        .checked = AUI_REACT(todoItem->isCompleted),
//        .onCheckedChange = [todoItem](bool checked) { todoItem->isCompleted = checked; }
//      },
//      Vertical{
//        Label { AUI_REACT(todoItem->title) } AUI_OVERRIDE_STYLE { FontSize {10_pt}, ATextOverflow::ELLIPSIS },
//        Label { AUI_REACT(todoItem->description) } AUI_OVERRIDE_STYLE { Opacity { 0.7f }, ATextOverflow::ELLIPSIS },
//      }
//    };
//}

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
        Label { .text = AUI_REACT(stringOneLinePreview(todoItem->title)) },
        Label { .text = AUI_REACT(stringOneLinePreview(todoItem->description)) }
    };
}