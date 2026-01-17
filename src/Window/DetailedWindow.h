#pragma once

#include "Model/TodoItem.h"
#include <AUI/Platform/AWindow.h>

class DetailedWindow : public AWindow {
public:
    DetailedWindow(const _<TodoItem>&);
};