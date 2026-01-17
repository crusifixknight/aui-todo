#pragma once

#include "Model/TodoItem.h"
#include <AUI/Platform/AWindow.h>
#include <AUI/Common/AProperty.h>
#include <AUI/Common/AVector.h>

class MainWindow: public AWindow {
public:
    MainWindow();
    ~MainWindow();

private:
    void newTodo();
    void openDetailed(const _<TodoItem>&);
    void save();
    void load();
    void deleteTodo(const _<TodoItem>&);

    AProperty<AVector<_<TodoItem>>> mTodoItems;
    _<AWindow> detailedWindow;
};