#include "DetailedWindow.h"
#include "View/TodoEditor.h"
#include <AUI/View/Dynamic.h>
#include <AUI/Util/Declarative/Containers.h>


using namespace declarative;

DetailedWindow::DetailedWindow(const _<TodoItem>& todoItem) : AWindow(todoItem->title) {
    setContents(
        Vertical::Expanding{
            experimental::Dynamic { AUI_REACT(todoEditor(todoItem)) } AUI_OVERRIDE_STYLE { Expanding() }
        } 
    );
}