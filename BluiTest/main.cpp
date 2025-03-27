#include <algorithm>
#include "raylib.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "guiBox.hpp"
#include "tinyxml2.h"

using namespace tinyxml2;

int initialX = 0, initialY = 0;
int wWidth = 480, wHeight = 320;
int buttonHeight = 50;
Color bgColor = WHITE;
const char* name = "blui";
bool buttonDown = false;
std::vector<GuiBox> boxes;

uint32_t packColor(const Color& color) {
	return (static_cast<uint32_t>(color.r) << 24) |
		(static_cast<uint32_t>(color.g) << 16) |
		(static_cast<uint32_t>(color.b) << 8) |
		static_cast<uint32_t>(color.a);
}

Color unpackColor(uint32_t packedColor) {
	Color color;
	color.r = (packedColor >> 24) & 0xFF;
	color.g = (packedColor >> 16) & 0xFF;
	color.b = (packedColor >> 8) & 0xFF;
	color.a = packedColor & 0xFF;
	return color;
}

void DrawExportButton()
{

	if (GuiButton(Rectangle{ 0, (float)wHeight, (float)wWidth*.5f, (float)buttonHeight }, "Save"))
	{
		std::ofstream outFile("saveFile.xml");

		if (outFile.is_open()) {
			outFile.clear();
			outFile << "<bml>\n";
			outFile << "\t<width>" << wWidth << "</width>\n";
			outFile << "\t<height>" << wHeight << "</height>\n";
			outFile << "\t<bgColour>" << packColor(bgColor) << "</bgColour>\n";
			for (int i = 0; i < boxes.size(); i++)
			{
				GuiBox b = boxes[i];
				outFile << "\t<node>\n";
				outFile << "\t\t<name>object_" << i << "</name>\n";
				outFile << "\t\t<type>" << "Rect" << "</type>\n";
				outFile << "\t\t<x>" << b.x() << "</x>\n";
				outFile << "\t\t<y>" << b.y() << "</y>\n";
				outFile << "\t\t<width>" << b.width() << "</width>\n";
				outFile << "\t\t<height>" << b.height() << "</height>\n";
				outFile << "\t\t<colour>" << packColor(BLACK) << "</colour>\n";
				outFile << "\t</node>" << "\n";
			}
			outFile << "</bml>";
			
			outFile.close();
		}
	}
}

void StretchCurrentBox()
{
	if (boxes.size() == 0)
		return;

	boxes.back().Stretch(GetMouseX(), GetMouseY());
}

void DrawAllBoxes()
{
	for (int i = 0; i < boxes.size(); i++)
	{
		GuiBox b = boxes[i];
		DrawRectangle(b.x(), b.y(), b.width(), b.height(), b.color);
		std::string text = "[x:" + std::to_string(b.x()) + ", y:" + std::to_string(b.y())
								+ ", w:" + std::to_string(b.width()) + ", h:" + std::to_string(b.height()) + "]";
		DrawText(text.c_str(), b.x(), b.y() - 12, 5, b.color);
	}
}
void unpackNode(XMLElement* node)
{
	int x, y, width, height;
	node->FirstChildElement("x")->QueryIntText(&x);
	node->FirstChildElement("y")->QueryIntText(&y);
	node->FirstChildElement("width")->QueryIntText(&width);
	node->FirstChildElement("height")->QueryIntText(&height);
	GuiBox box{ x, y, x+width, y+height };
	boxes.push_back(box);
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		const char* filePath = argv[1];
		XMLDocument doc;
		XMLError result = doc.LoadFile(filePath);
		if (result != XML_SUCCESS) {
			std::cerr << "Error loading file: " << result << std::endl;
			return result;
		}

		uint64_t packedColor;
		XMLElement* root = doc.FirstChildElement("bml");
		root->FirstChildElement("width")->QueryIntText(&wWidth);
		root->FirstChildElement("height")->QueryIntText(&wHeight);
		root->FirstChildElement("bgColour")->QueryUnsigned64Text(&packedColor);
		XMLElement* nodeElement = root->FirstChildElement("node");
		while (nodeElement)
		{
			unpackNode(nodeElement);
			nodeElement = nodeElement->NextSiblingElement("node");
		}
		bgColor = unpackColor(packedColor);
	}

	InitWindow(wWidth, wHeight + buttonHeight, name);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		ClearBackground(bgColor);

		if (IsMouseButtonDown(0) && GetMouseY() < wHeight)
		{
			if (!buttonDown)
			{
				GuiBox box{ GetMouseX(), GetMouseY() };
				boxes.push_back(box);
				buttonDown = true;
			}
			StretchCurrentBox();
		}
		else
		{
			buttonDown = false;
		}
		DrawAllBoxes();
		DrawExportButton();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}