#include "GameUtils.h"
#include "Constants.h"
#include "helpers/string_manipulation/base64.h"
#include "helpers/string_manipulation/zlibString.h"
#include <regex>

using namespace ax;

namespace GameUtils {
	// init PPM
	float PPM::kPpm = 0;
	float PPM::metersHeight = 10;
	void PPM::initVars()
	{
		kPpm = ax::Director::getInstance()->getVisibleSize().height / metersHeight;
	}

	// bg image
	void setBackgrounImage(ax::Node* scene, std::string_view file_name)
	{
		float kWIDTH{ ax::Director::getInstance()->getVisibleSize().width };
		float kHEIGHT{ ax::Director::getInstance()->getVisibleSize().height };

		auto backgroundSprite = Sprite::create(file_name);
		backgroundSprite->setPosition(Vec2(kWIDTH * 0.5f, kHEIGHT * 0.5f));
		float scaleX{ kWIDTH / backgroundSprite->getContentSize().width };
		float scaleY{ kHEIGHT / backgroundSprite->getContentSize().height };

		// Choose the smaller scaling factor to ensure the logo fits entirely within the screen
		//float scale = std::min(scaleX, scaleY);

		backgroundSprite->setScale(scaleX, scaleY);
		scene->addChild(backgroundSprite, -10);
	}

	bool TestPoint(const b2CircleShape& circleShape, const b2Transform& transform, const b2Vec2& point)
	{
		b2Vec2 center = transform.p + b2Mul(transform.q, circleShape.m_p);
		b2Vec2 d = point - center;
		return b2Dot(d, d) <= circleShape.m_radius * circleShape.m_radius;
	}

	ax::ActionInterval* createShakeAction(int numOfShakes, float amplitude, float shakeDuration, float initialRotationAngle, float rotationAmplitude)
	{
		// Calculate the duration of each shake (time = total_duration / number_of_shakes)
		int shakes = numOfShakes * 2; // Multiply by 2 to account for the back-and-forth movement

		// Create the shaking sequence
		ax::Vector<ax::FiniteTimeAction*> actions;
		//for (int i = 0; i < shakes / 2; i++) {
		//	// Move the button to the right
		//	actions.pushBack(ax::MoveBy::create(shakeDuration, ax::Vec2(amplitude, 0)));
		//	// Move the button to the left
		//	actions.pushBack(ax::MoveBy::create(shakeDuration, ax::Vec2(-amplitude, 0)));
		//}

		// Calculate the initial rotation angle (45 degrees)
		//float initialRotationAngle = -15.0f;

		// Add initial rotation action (rotate to 45 degrees)
		actions.pushBack(ax::RotateTo::create(0.0f, initialRotationAngle));

		// Add rotation actions
		//float rotationAmplitude = 15.0f; // The angle of rotation (in degrees)
		for (int i = 0; i < shakes; i++) {
			// Rotate the button clockwise
			actions.pushBack(ax::RotateBy::create(shakeDuration, rotationAmplitude));
			// Rotate the button counterclockwise
			actions.pushBack(ax::RotateBy::create(shakeDuration, -rotationAmplitude));
		}

		// Create the repeating sequence (RepeatForever to keep the shaking effect until you stop it)
		auto shakingSequence = ax::Sequence::create(actions);
		return ax::RepeatForever::create(shakingSequence);
	}

	ax::ActionInterval* createInfiniteZoomAction(float zoomDuration, float zoomInScale, float originalScale)
	{
		// The duration for each zooming effect (in seconds)
		// float zoomDuration = 1.5f;
		// The scale factor to which the label will zoom in
		// float zoomInScale = 0.51f;
		// The original scale factor of the label
		// float originalScale = 0.5f;
		// Create a sequence of actions for zooming in and out
		auto zoomInAction = ax::ScaleTo::create(zoomDuration, zoomInScale);
		auto zoomOutAction = ax::ScaleTo::create(zoomDuration, originalScale);

		// Combine the actions in a sequence
		auto zoomSequence = ax::Sequence::create(zoomInAction, zoomOutAction, nullptr);

		// Create the infinite sequence by repeating the zoom sequence forever
		auto infiniteZoomAction = ax::RepeatForever::create(zoomSequence);

		return infiniteZoomAction;
	}

	void saveItemDataToJson(rapidjson::Document& document, const std::string& outputFile)
	{
		// Convert the modified JSON back to a string
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		auto modifiedJson = buffer.GetString();

		// compress the data
		auto enc = zlibString::compress_string(modifiedJson);
		auto encodedModifiedData = Strings::to_base64(enc);

		// Save the encoded modified data to a writable file
		std::string outputPath = FileUtils::getInstance()->getWritablePath() + outputFile;
		FILE* file = fopen(outputPath.c_str(), "wb");
		if (file) {
			fwrite(encodedModifiedData.c_str(), sizeof(char), encodedModifiedData.size(), file);
			fclose(file);
		}
		else
		{
			std::cout << "Error opening file for writing." << std::endl;
		}
	}

	double sumVersionNumbers(const std::string& filePath)
	{
		// Read the content of the file
		std::string content;
		FileUtils::getInstance()->setSearchPaths({});

		if (FileUtils::getInstance()->isFileExist(filePath))
		{
			content = FileUtils::getInstance()->getStringFromFile(filePath);
		}
		else {
			//AXLOG("File not found: %s", filePath.c_str());
			return 0.0; // Return 0 if the file doesn't exist
		}

		// Use a regular expression to find version numbers in the text
		std::regex versionRegex("\\d+\\.\\d+\\.\\d+");
		std::smatch matches;
		std::vector<std::string> extractedVersions; // Container to store extracted versions

		while (std::regex_search(content, matches, versionRegex)) {
			for (auto match : matches) {
				extractedVersions.push_back(match.str());
			}
			content = matches.suffix();
		}

		// Calculate the sum of extracted version numbers
		double sum = 0.0;

		for (const std::string& version : extractedVersions) {
			// Convert the version string to a numeric value and add it to the sum
			double numericVersion = std::stod(version);
			sum += numericVersion;
		}

		return sum;
	}

	bool areSpritesIntersecting(ax::Sprite* sprite1, ax::Sprite* sprite2)
	{
		// Assuming the sprites have a position, content size, and scale property
		ax::Vec2 center1 = sprite1->getPosition();
		ax::Vec2 size1 = sprite1->getContentSize();
		float scale1 = sprite1->getScale();

		ax::Vec2 center2 = sprite2->getPosition();
		ax::Vec2 size2 = sprite2->getContentSize();
		float scale2 = sprite2->getScale();

		// Adjust the dimensions based on the scale
		size1 *= scale1;
		size2 *= scale2;

		// Calculate the distance between the centers of the scaled sprites
		float distance = std::sqrt(
			std::pow(center2.x - center1.x, 2) + std::pow(center2.y - center1.y, 2));

		// Assuming the radius is determined by the half of the sprite's width
		float radius1 = size1.x / 2;
		float radius2 = size2.x / 2;

		// Check if the distance is less than the sum of the scaled radii
		return distance < (radius1 + radius2);
	}
} // GameUtils