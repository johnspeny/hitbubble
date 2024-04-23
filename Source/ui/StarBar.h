#ifndef __STARBAR_H__
#define __STARBAR_H__

#include "axmol.h"
#include "ui/CocosGUI.h"
#include "ToolBar.h"
#include "ui/SeasonSelectBar.h"

using namespace ax;
using namespace ui;

//i have 3 wives each has 2 children, one child dies and one of my wives announces that 3 children do not belong to me.I go a head and impregnant one of my wives who delivers triplets, how many wives and children do I have

class StarBar : public ToolBar
{
private:
	const SeasonSelectBar& m_seasonSeleBar;
public:
	StarBar(const SeasonSelectBar& seasonSeleBar);
	virtual ~StarBar() = default;
	static StarBar* create(const SeasonSelectBar& seasonSeleBar);
	virtual bool init() override;
};

#endif // !__STARBAR_H__
