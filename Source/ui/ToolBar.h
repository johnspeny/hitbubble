#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#include "axmol.h"
#include "ui/CocosGUI.h"

using namespace ax;
using namespace ui;

class ToolBar : public ui::Layout
{
protected:
	Widget* m_leftItem;
	Widget* m_rightItem;
	Widget* m_middleItem;
	const float kLayoutHeight = 50.0f;
	const float kPadding = 50.0f;
	Size m_visibleSize;
	Vec2 m_origin;

	virtual void layoutStructure();
	virtual void layoutStructure(const Vec2& pos, const Color3B& color);
public:
	ToolBar();
	virtual ~ToolBar() = default;
	static ToolBar* create();
	virtual bool init() override;
	virtual void addLeftComponent(Widget* component, float scale);
	virtual void addRightComponent(Widget* component, float scale);
	virtual void addMiddleComponent(Widget* component, float scale);
	virtual void addComponentAtCenter(Widget* component);
};

#endif // !__TOOLBAR_H__
