#pragma once
#include <string>

class UI
{
public:
static void update();
static void setKey(int code, bool down);
static bool isKeyDown(int code);
static bool isKeyClicked(int code);

static void setMousePos(int x, int y);
static void getMousePos(int &x, int &y);

static void setClick(int key, bool down);
static bool isMouseDown(int key);
static bool isMouseClicked(int key);

static void setRemoteClicked(std::wstring remoteID);
static bool isRemoteClicked(std::wstring remoteID);

static enum KEYS{KEY_BACKSPACE = 0x8,KEY_TAB,KEY_ENTER= 0x0D,KEY_SHIFT = 0x10,KEY_CONTROL,KEY_ALT,KEY_ESCAPE = 0x1B,KEY_SPACE = 0x20,KEY_PAGE_UP,KEY_PAGE_DOWN,KEY_END,KEY_HOME,KEY_LEFT,KEY_UP,KEY_RIGHT,KEY_DOWN};
	
private:

	static char keyCodes[255];
	static char mouseButtons[3]; 
	static int mouseX;
	static int mouseY;
	static std::wstring remoteButton;
};
