#ifndef Popup_h
#define Popup_h

#include <stdio.h>
#include "axmol.h"
#include "ui/CocosGUI.h"

/*
 -Displays a popup
 -Resizes itself
 -Automatically swallows rest of the touches
 -Exit is called if touch is outside Bg
 */

class GameScene;

namespace UICustom
{
	class Popup : public ax::LayerColor
	{
	private:
		int m_audioID;

		bool m_isAnimated;
		void initBg();
		GameScene* m_gameScene;

	public:
		explicit Popup(GameScene* gameScene);
		static Popup* create(GameScene* gameScene);
		virtual void show(const bool animated);

	protected:
		virtual bool init() override;
		virtual void setUpTouches();

		virtual void dismiss(const bool animated);
		inline void setAnimated(const bool animated) { m_isAnimated = animated; }

		ax::ui::ImageView* m_bg;
	};
}

#endif /* Popup_h */