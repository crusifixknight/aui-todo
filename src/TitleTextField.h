#pragma once
#include <AUI/View/ATextField.h>


class TitleTextField : public ATextField {
public:
    using ATextField::ATextField;
    void onCharEntered(AChar c) override {
        if (c == '\r') {
            AWindow::current()->focusNextView();
            return;
        }
        ATextField::onCharEntered(c);
    }

};