#include "Util.h"
#include <vector>
#include <Windows.h>
#include <string>
#include <stdexcept>

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