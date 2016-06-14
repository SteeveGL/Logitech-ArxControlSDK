#include "UI.h"

char UI::keyCodes[] = {false};
char UI::mouseButtons[] = {false};
int UI::mouseX =0;
int UI::mouseY =0;
std::wstring UI::remoteButton = L"";
void UI::setKey(int code, bool down)
{
	if(code >=0 && code < 256)
		keyCodes[code] = (down)?0xf:0;
}

bool UI::isKeyDown(int code)
{
	if(code >=0 && code < 256)
		return keyCodes[code]!=0;
	return false;
}
bool UI::isKeyClicked(int code)
{
	if(code >=0 && code < 256)
		return keyCodes[code] > 1;	
	return false;
}

void UI::setClick(int key, bool down)
{
	if(key >= 0 && key < 3)
	{
		mouseButtons[key] = (down)?0xf:0;
	}
}

bool UI::isMouseDown(int key)
{
	if(key >= 0 && key < 3)
	{
		return mouseButtons[key]!=0;
	}
	return false;
}
bool UI::isMouseClicked(int key)
{
	if(key >= 0 && key < 3)
	{
		return mouseButtons[key] > 1;	
	}
	return false;
}

void UI::setMousePos(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void UI::getMousePos(int &x, int &y)
{
	x = mouseX;
	y = mouseY;
}

void UI::setRemoteClicked(std::wstring remoteID)
{
	remoteButton = remoteID;
}
bool UI::isRemoteClicked(std::wstring remoteID)
{
	return remoteButton.compare(remoteID) == 0;
}
void UI::update()
{
	for(int i=0; i <3; i++)
	{
		mouseButtons[i]&=1;
	}
	for(int i=0; i <255; i++)
	{
		keyCodes[i]&=1;
	}
	remoteButton = L"";
}