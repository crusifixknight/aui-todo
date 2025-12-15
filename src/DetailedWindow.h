#pragma once

#include "model/TodoItem.h"
#include <AUI/Platform/AWindow.h>
#include <AUI/Common/SharedPtrTypes.h>

class DetailedWindow : public AWindow {
public:
    DetailedWindow(_<TodoItem>);
};