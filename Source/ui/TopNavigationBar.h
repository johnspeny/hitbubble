#ifndef __TopNavigationBar_H__
#define __TopNavigationBar_H__

#include "axmol.h"
#include "ui/CocosGUI.h"
#include "ToolBar.h"

using namespace ax;
using namespace ui;

class TopNavigationBar : public ToolBar
{
private:
	void layoutStructure() override;
public:
	TopNavigationBar();
	virtual ~TopNavigationBar() = default;
	static TopNavigationBar* create();
	virtual bool init() override;
};

#endif // !__TopNavigationBar_H__