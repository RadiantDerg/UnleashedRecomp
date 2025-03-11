#pragma once

#include <ui/reddog/reddog_window.h>

class MemoryInspector : public Reddog::Window
{
    // Impl Memory Region Item
    struct MemRegionItem {
        uint32_t* ptr;
        uint32_t size;
    };

    struct MemRegionXItem {
        xpointer<uint32_t>* ptr;
        uint32_t size;
    };

    bool m_IsGreyOutZeroes = true;
    bool m_IsMemoryReadOnly = true;

public:
    MemoryInspector() : Window()
    {
        Name = "Memory Inspector";
        Flags = Reddog::eWindowFlags_None;
    }

    void Draw() override;
};
