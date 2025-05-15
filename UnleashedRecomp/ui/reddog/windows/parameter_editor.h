#pragma once

#include <ui/reddog/reddog_window.h>

class ParameterEditorWindow : public Reddog::Window
{
public:
    ParameterEditorWindow() : Window()
    {
        Name = "Parameter Editor";
        Flags = Reddog::eWindowFlags_None;
    }

    void Draw() override;
};
