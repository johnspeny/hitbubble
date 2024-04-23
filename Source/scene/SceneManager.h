#pragma once

#include "axmol.h"

class SceneManager final {
public:
	static SceneManager& getInstance();

	void runWithScene(ax::Scene* scene);
	void pushScene(ax::Scene* scene);
	void popScene();

	template <typename T = ax::Scene>
	T* getCurrentScene() const {
		if (!_scenes.empty()) {
			return dynamic_cast<T*>(_scenes.top());
		}
		return nullptr;
	}

	std::vector<ax::Scene*> getAllScenes();
	void replaceScene(ax::Scene* scene);

private:
	SceneManager();
	std::stack<ax::Scene*> _scenes;
};
