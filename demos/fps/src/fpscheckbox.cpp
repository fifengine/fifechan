/*
 * Guichan FPS Demo
 *
 * This is a demonstration of the Guichan GUI library and what it
 * is capable of.
 *
 * For more information about Guichan visit: http://guichan.sourceforge.net
 */
#include "fpscheckbox.hpp"

void FPSCheckBox::draw(gcn::Graphics* graphics)
{
	graphics->setFont(getFont());
	graphics->setColor(getForegroundColor());

	// First draw the caption
	graphics->drawText(mCaption, 0, 0);

	// Calculate the x coordinate for the box
	int x = getFont()->getWidth(mCaption) + getHeight() / 2;

	// Push a clip area where the box should draw itself
	graphics->pushClipArea(gcn::Rectangle(x, 0, getWidth() - x, getHeight())); 
	drawBox(graphics);
	// Pop the cliparea. VERY IMPORTANT!
	graphics->popClipArea();
}
