#pragma once

#include "axmol.h"
#include "box2d/box2d.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>

namespace GameUtils
{
	class PPM {
	public:
		// b2d world defaults
		static float kPpm;
		static float metersHeight;
		static void initVars();
	};

	//constexpr float metersHeight = 10.0f;
	void setBackgrounImage(ax::Node* scene, std::string_view file_name);

	// test two circles are intersecting
	bool TestPoint(const b2CircleShape& circleShape, const b2Transform& transform, const b2Vec2& point);

	// shake function
	ax::ActionInterval* createShakeAction(int numOfShakes, float amplitude, float shakeDuration, float initialRotationAngle, float rotationAmplitude);

	// zoom in and out basically pop up n in
	ax::ActionInterval* createInfiniteZoomAction(float zoomDuration, float zoomInScale, float originalScale);

	void saveItemDataToJson(rapidjson::Document& document, const std::string& outputFile);

	double sumVersionNumbers(const std::string& filePath);

	bool areSpritesIntersecting(ax::Sprite* sprite1, ax::Sprite* sprite2);
}
