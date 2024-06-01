#pragma once

#include "axmol.h"
#include "ToolBar.h"

using namespace ax;
using namespace ui;

class BottomNavigationBar : public ToolBar
{
private:
    void layoutStructure() override;

public:
    BottomNavigationBar();
    virtual ~BottomNavigationBar() = default;
    static BottomNavigationBar* create();
    virtual bool init() override;
};
