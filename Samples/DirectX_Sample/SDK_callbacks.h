#pragma once

#pragma comment(lib, "LogitechGArxControlLib.lib")
#include <stddef.h>
#include "LogitechGArxControlLib.h"
#include <windows.h>
#include <string>
#include <vector>

typedef struct arxAppCallbackMessage
{
	unsigned __int32 eventType;
	unsigned __int32 eventValue;
	wchar_t eventArg[128]; //Max length for this callback param (see documentation for more details)
}arxAppCallbackMessage;


#define WM_ARXAPP_SDK_CALLBACK WM_APP + 1

typedef enum{UNKNOWN=11, CONNECTED, SLEEPING, DISCONNECTED, ACTIVE, INACTIVE, ALERT} CONNECT_STATUS;

class SDK_callbacks
{
public:
	void init(HWND hwnd);
	void close();
	void sendApplet();
	void shutdown();
	void sendFile(wchar_t* path, wchar_t* resourceName);
	void updateScore(int cur, int max);
	void updateMiniMap(wchar_t *numPelletsValue, wchar_t *pelletLocsNewContent);

    static void __cdecl onCallback(int eventType, int eventValue, wchar_t * eventArg, void *context);
	CONNECT_STATUS getStatus(){ return status;};
	char getOrientation(){ return curOrientation;};
	std::wstring getName(){ return name;};
	CONNECT_STATUS status;
	std::wstring name;
	char curOrientation;
	bool appletInitialized;
private:
	HMODULE GamepanelDLL;
	std::vector<BYTE> ReadAllBytes(char const* filename);
};