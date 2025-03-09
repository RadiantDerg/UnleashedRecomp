#pragma once

#include <ui/reddog/reddog_window.h>

class ParamEditorWindowTest : public Reddog::Window
{
public:
    ParamEditorWindowTest() : Window()
    {
        Name = "Parameter Editor";
        Flags = Reddog::eWindowFlags_NoResize;
    }

    void Draw() override;
};
