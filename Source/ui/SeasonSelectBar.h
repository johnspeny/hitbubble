#ifndef __SEASONSELECTBAR_H__
#define __SEASONSELECTBAR_H__

#include "axmol.h"
#include "ui/CocosGUI.h"
#include "ToolBar.h"

using namespace ax;
using namespace ui;

class SeasonSelectBar : public ToolBar
{
private:
	void layoutStructure() override;
public:
	SeasonSelectBar();
	virtual ~SeasonSelectBar() = default;
	static SeasonSelectBar* create();
	virtual bool init() override;
};

#endif // !__SEASONSELECTBAR_H__