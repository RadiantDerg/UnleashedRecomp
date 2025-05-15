#pragma once

#include <ui/reddog/reddog_window.h>
#include <SWA.h>
class SequenceMenu : public Reddog::Window
{
public:
    static inline  std::vector<std::string> sequenceLogs;
    SequenceMenu() : Window()
    {
        Name = "Sequence Menu";
    }

    void Draw() override;
};
