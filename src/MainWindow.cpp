#include <AUI/Util/UIBuildingHelpers.h>
#include "MainWindow.h"
#include "model/TodoItem.h"
#include "DetailedWindow.h"
#include <AUI/Util/AMetric.h>
#include <AUI/View/ALabel.h>
#include <AUI/View/AButton.h>
#include <AUI/View/ASpacerFixed.h>
#include <AUI/View/AListView.h>
#include <AUI/View/AForEachUI.h>
#include <AUI/Common/APropertyPrecomputed.h>
#include <AUI/ASS/AStylesheet.h>
#include <AUI/Platform/AWindow.h>
#include <AUI/ASS/Property/Padding.h>
#include <AUI/Json/Conversion.h>
#include <AUI/IO/AFileInputStream.h>
#include <AUI/View/ACheckBox.h>
#include <AUI/Util/AWordWrappingEngineImpl.h>
#include <AUI/Platform/AMessageBox.h>
#include <AUI/View/ADrawableView.h>
#include <AUI/Util/kAUI.h>
#include <chrono>
#include <format>

using namespace declarative;
using namespace ass;

static constexpr auto LOG_TAG = "Todo's";

AJSON_FIELDS(TodoItem, AJSON_FIELDS_ENTRY(title) AJSON_FIELDS_ENTRY(description) AJSON_FIELDS_ENTRY(isCompleted))



template <typename T1, typename T2>
struct AJsonConv<std::chrono::time_point<T1, T2>> {
    static AJson toJson(std::chrono::time_point<T1, T2> v) { return v.time_since_epoch().count(); }
    static void fromJson(const AJson& json, std::chrono::time_point<T1, T2>& dst) {
        dst = std::chrono::time_point<T1, T2>(T2(json.asLongInt()));
    }
};

MainWindow::MainWindow() : AWindow("Todo Application", 900_dp, 800_dp) {
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
                    Button{ Horizontal{ Icon {":img/icon.svg"}, SpacerFixed {5_dp}, Label { "New Note" }}, { me::newTodo } },
                    SpacerFixed { 20_dp },
                    Button{ Horizontal{ Icon {":img/save.svg"}, SpacerFixed {5_dp},Label{ "Save" }}, { me::save } }
                }
              },
                AScrollArea::Builder()
                  .withContents(
                    AUI_DECLARATIVE_FOR(todoItem, *mTodoItems, AVerticalLayout){ 
                        return Vertical{
                            SpacerFixed { 10_dp },
                            Horizontal::Expanding{
                                Vertical{
                                    Centered{
                                        CheckBox{
                                        .checked = AUI_REACT(todoItem->isCompleted),
                                        .onCheckedChange = [todoItem](bool checked) { todoItem->isCompleted = checked; }
                                        } 
                                    },SpacerFixed{10_dp}, Label{std::format("{:%Y.%m.%d}", todoItem->date)}
                                },
                                SpacerFixed { 10_dp }, 
                                todoPreview(todoItem) AUI_LET { connect(it->clicked, [this, todoItem] { openDetailed(todoItem); }), Expanding {}; },
                                Button { Horizontal {Icon {":img/trash.svg"}, SpacerFixed {5_dp}, Label { "Delete" }} } AUI_LET { connect(it->clicked, [this, todoItem] { deleteTodo(todoItem); }); }
                            } AUI_OVERRIDE_STYLE { Padding { 10_dp }, BackgroundSolid { AColor::WHITE } }
                        };
                  }).build() 
            }
    );
}

MainWindow::~MainWindow() {
    if (detailedWindow != nullptr) {
        detailedWindow->close();
    }
    MainWindow::save();
}

void MainWindow::newTodo() {
    const auto now { std::chrono::system_clock::now() };
    auto todo = aui::ptr::manage_shared(new TodoItem { .title = "Untitled",.date = floor<std::chrono::days>(std::chrono::system_clock::now()), .isCompleted = false});
    mTodoItems.writeScope()->push_back(todo);
    openDetailed(todo);
}

void MainWindow::openDetailed(const _<TodoItem>& todoItem) 
{
    if (todoItem->isCompleted)
    {
        AMessageBox::show(this, "Task is marked as closed", "Task {} is marked as closed and can't be changed"_format(todoItem->title));
        return;
    }
    if (detailedWindow != nullptr) {
        detailedWindow->close();
        detailedWindow = nullptr;
    }
    detailedWindow = _new<DetailedWindow>(todoItem);
    detailedWindow->show();
}

_<AView> todoPreview(const _<TodoItem> todoItem) {
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
            Label { .text = AUI_REACT(stringOneLineTitlePreview(todoItem->title)) } AUI_OVERRIDE_STYLE { FontSize { 16_pt }, ATextOverflow::CLIP },
            Label { .text = AUI_REACT(stringOneLinePreview(todoItem->description)) } AUI_OVERRIDE_STYLE { Opacity { 0.7f }, ATextOverflow::ELLIPSIS }
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

void MainWindow::deleteTodo(const _<TodoItem> todoItem) {
    auto it = ranges::find(*mTodoItems, todoItem);
    it = mTodoItems.writeScope()->erase(it);
}