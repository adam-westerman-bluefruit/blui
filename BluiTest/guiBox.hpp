#pragma once
#include <algorithm>
#include <raylib.h>

class GuiBox
{
public:
	GuiBox(int initialX, int initialY);
	GuiBox(int initialX, int initialY, int secondaryX, int secondaryY);
	void Stretch(int x, int y);
	void Nudge(int topLeftOffsetX, int topLeftOffsetY, 
		int bottomRightX, int bottomRightY);
	int x();
	int y();
	int width();
	int height();
	Color color;
	private:
	int x_initial;
	int y_initial;
	int x_secondary;
	int y_secondary;
};
