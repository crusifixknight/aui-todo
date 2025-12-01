#pragma once

#include <AUI/Common/AProperty.h>
#include <AUI/Common/AObject.h>

struct TodoItem{
    AProperty<AString> title;
    AProperty<AString> description;
    AProperty<bool> isCompleted=false;
};