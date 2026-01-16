#include "utils.h"
#include <vector>
#include <Windows.h>
#include <string>
#include <stdexcept>
#include <AUI/Platform/AWindow.h>
#include <AUI/ASS/ASS.h>
#include <AUI/View/AButton.h>
#include <AUI/View/ATextArea.h>
#include <AUI/View/ACheckBox.h>
#include <AUI/View/ATextField.h>

using namespace ass;

bool isLightTheme() {
    auto buffer = std::vector<char>(4);
    auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
    auto res = RegGetValueW(
        HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme",
        RRF_RT_REG_DWORD,   
        nullptr, buffer.data(), &cbData);

    if (res != ERROR_SUCCESS) {
        throw std::runtime_error("Error: error_code=" + std::to_string(res));
    }

    auto i = int(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);

    return i == 1;
}

void createThemedStylesheet()
{
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