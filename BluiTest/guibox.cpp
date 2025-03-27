#include "guiBox.hpp"

GuiBox::GuiBox(int initialX, int initialY) :
	x_initial(initialX), y_initial(initialY),
	x_secondary(initialX), y_secondary(initialY), color(Color{0,0,0,125})
{
}

GuiBox::GuiBox(int initialX, int initialY, int secondaryX, int secondaryY) :
	x_initial(initialX), y_initial(initialY),
	x_secondary(secondaryX), y_secondary(secondaryY), color(Color{ 0,0,0,125 })
{
}


void GuiBox::Stretch(int x, int y) 
{ 
	x_secondary = x; 
	y_secondary = y; 
}

void GuiBox::Nudge(int topLeftOffsetX, int topLeftOffsetY, int bottomRightX, int bottomRightY) 
{
	
}

int GuiBox::x() 
{ 
	return std::min(x_initial, x_secondary); 
}

int GuiBox::y() 
{
	return std::min(y_initial, y_secondary); 
}

int GuiBox::width() 
{ 
	return std::max(1, std::abs(x_initial - x_secondary)); 
}

int GuiBox::height() 
{ 
	return std::max(1, std::abs(y_initial - y_secondary));
}
