#include "Util.h"
#include <AUI/View/ACheckBox.h>
#include <AUI/ASS/Property/BackgroundSolid.h>
#include <AUI/ASS/Property/Border.h>
#include <AUI/ASS/Property/TextColor.h>
#include <AUI/ASS/Selector/class_of.h>
#include <AUI/ASS/Selector/type_of.h>
#include <AUI/View/AButton.h>
#include <AUI/View/ATextArea.h>
#include <AUI/View/ATextField.h>
#include <AUI/ASS/AStylesheet.h>

using namespace ass;

struct Style {
    Style() {
        AColor color = isLightTheme() ? AColor::WHITE.darker(0.04) : AColor::fromRRGGBB(0x373737); 

        AStylesheet::global().addRules({
            { 
              { t<AWindow>(), t<ACheckBox::Box>() },
              BackgroundSolid { color.darker(0.08) }
            },
            {
              t<ACheckBox::Box>(),
              Border { 1_dp, color.darker(0.3) }
            },
            {
              t<AButton>(),
              BackgroundSolid { color },
              Border { 1_dp, color.darker(0.2) }
            },
            {
              t<AButton>::hover(),
              Border { 1_dp, color.lighter(0.4) }
            },
            {
              Selected(t<ACheckBox::Box>()),
              BackgroundSolid { AColor::BLUE }
            },
            { 
              { t<ATextArea>(), t<ATextField>(), t<ALabel>() }, 
              TextColor { color.readableBlackOrWhite() }
            },
            { 
              { t<ATextArea>(), t<ATextField>(), c(".darker") }, 
              BackgroundSolid { color.lighter(0.2) },
              Border { nullptr }
            }
        });
    }
} s;