#pragma once

#include "model/TodoItem.h"
#include <AUI/Platform/AWindow.h>

class MainWindow: public AWindow {
public:
    MainWindow();
private:
    void observeChangesForDirty(const _<TodoItem>&);
    void markDirty();
    void newTodo();
    void openDetailed(const _<TodoItem>&);

    AProperty<AVector<_<TodoItem>>> mTodoItems;
    AProperty<_<TodoItem>> mCurrentTodoItem;
    AProperty<bool> mDirty = false;
};