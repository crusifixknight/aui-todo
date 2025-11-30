#pragma once

#include <AUI/Common/AProperty.h>
#include <AUI/Common/AObject.h>
#include <AUI/Json/Conversion.h>

struct TodoItem{
    AProperty<AString> title;
    AProperty<AString> description;
    AProperty<AString> date;
    AProperty<bool> isCompleted=false;
};