#pragma once

#include "model/TodoItem.h"
#include <AUI/View/ATextArea.h>
#include <AUI/Platform/AWindow.h>

class DetailedWindow : public AWindow {
public:
    DetailedWindow(_<TodoItem> todoItem);
};