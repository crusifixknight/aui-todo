#pragma once

#include <AUI/Common/AProperty.h>
#include <AUI/Common/AObject.h>
#include <chrono>

struct TodoItem{
    using Timepoint = std::chrono::time_point<std::chrono::system_clock, std::chrono::days>;
    AProperty<AString> title = "Untitled";
    AProperty<AString> description;
    Timepoint date;
    AProperty<bool> isCompleted = false;
};
