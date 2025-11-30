#pragma once

#include "model/TodoItem.h"
#include <AUI/Platform/AWindow.h>
#include <AUI/Common/AProperty.h>
#include <AUI/Common/AVector.h>

class MainWindow: public AWindow {
public:
    MainWindow();
private:
    void observeChangesForDirty(const _<TodoItem>&);
    void markDirty();
    void newTodo();
    void openDetailed(const _<TodoItem>&);
    void save();
    void load();

    AProperty<AVector<_<TodoItem>>> mTodoItems;
    AProperty<_<TodoItem>> mCurrentTodoItem;
    AProperty<bool> mDirty = false;
};