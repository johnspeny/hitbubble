#include "SceneManager.h"

using namespace ax;

SceneManager::SceneManager() :
	_scenes{}
{
}

SceneManager& SceneManager::getInstance()
{
	static SceneManager instance;
	return instance;
}

void SceneManager::runWithScene(Scene* scene)
{
	Director::getInstance()->runWithScene(scene);
	while (!_scenes.empty()) {
		_scenes.pop();
	}
	_scenes.push(scene);
}

void SceneManager::pushScene(Scene* scene) {
	_scenes.push(scene);
	Director::getInstance()->pushScene(scene);
}

void SceneManager::popScene()
{
	if (!_scenes.empty()) {
		_scenes.pop();
		if (!_scenes.empty()) {
			Director::getInstance()->popScene();
		}
	}
}

std::vector<ax::Scene*> SceneManager::getAllScenes()
{
	std::vector<Scene*> scenes;
	while (!_scenes.empty()) {
		scenes.push_back(_scenes.top());
		_scenes.pop();
	}
	for (auto it = scenes.rbegin(); it != scenes.rend(); ++it) {
		_scenes.push(*it);
	}
	return scenes;
}

void SceneManager::replaceScene(ax::Scene* scene)
{
	// Check if there is a current scene to replace
	if (!_scenes.empty()) {
		// Pop the current scene
		_scenes.pop();
	}
	// Push the new scene to the scene stack
	pushScene(scene);
}