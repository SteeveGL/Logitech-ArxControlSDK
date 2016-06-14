#include "SDK_callbacks.h"
#include "DebugConsole.h"
#include "NameGenerator.h"
#include "UI.h"

void SDK_callbacks::init(HWND hwnd)
{
	RedirectIOToConsole();
	logiArxCbContext arxContextStruct;
	arxContextStruct.arxCallBack = (logiArxCb)onCallback;
	//Using the main window handle to be notified on the main thread through PostMessage on the main handle
	arxContextStruct.arxContext = hwnd;
	name = genName(30);
	wchar_t* szName = (wchar_t *)name.c_str();
	appletInitialized = false;
	status = UNKNOWN;
	curOrientation = 0x00;

	wchar_t *szPrettyName = L"World of Snakes";

	bool retVal = LogiArxInit(szName, szPrettyName, &arxContextStruct);				
	printf("Sending hello message: %s\n", retVal?"Successful":"Unsuccessful");
	if(retVal)
	{
		status = DISCONNECTED;
		appletInitialized = true;
	}
	else
	{
		printf("Init function failed with code: %d\n", LogiArxGetLastError());
	}
	
}

void SDK_callbacks::shutdown()
{
	if(!appletInitialized)
		return;
	LogiArxShutdown();
	appletInitialized = false;
	status = UNKNOWN;
}


void SDK_callbacks::updateScore( int cur, int max)
{
	wchar_t bScore[255];
	wsprintf(bScore, L"%d/%d",cur, max);
	LogiArxSetTagContentById(L"scoreText",bScore);
}

void SDK_callbacks::close()
{
	
}

void SDK_callbacks::onCallback(int eventType, int eventValue, wchar_t * eventArg, void * context)
{
	HWND main_hwnd = (HWND)context;
	//Using PostMessage to notify main thread of callback event
	arxAppCallbackMessage *parameter = new arxAppCallbackMessage;
	parameter->eventType = eventType;
	parameter->eventValue = eventValue;
	wcsncpy_s(parameter->eventArg, eventArg, _TRUNCATE);
	PostMessage(main_hwnd, WM_ARXAPP_SDK_CALLBACK, reinterpret_cast<WPARAM> (parameter) , static_cast<LPARAM>(0));
}

std::vector<BYTE> SDK_callbacks::ReadAllBytes(char const* filename)
{
	ifstream ifs(filename, ios::binary|ios::ate);
	if(!ifs.good())
	{
		std::vector<BYTE> emptyVector;
		return emptyVector;
	}
	ifstream::pos_type pos = ifs.tellg();

	std::vector<BYTE>  result(pos);
	
	ifs.seekg(0, ios::beg);
	ifs.read((char *)&result[0], pos);

	return result;
}

void SDK_callbacks::sendFile(wchar_t* path, wchar_t* resourceName)
{
	if(!LogiArxAddFileAs(path,resourceName,L""))
		printf("Could not upload %ls with error code : %d\n",resourceName, LogiArxGetLastError());
	else
		printf("Uploaded to lgs %ls\n",resourceName);
}


int xy2d (int n, int x, int y) ;
void rot(int n, int *x, int *y, int rx, int ry);

void renderTail(BYTE*image, int width, int height, float percent)
{
	int n = 64;
	float max = n*n - 1;
	for(int i =0; i < width; i++)
	{
		for(int j =0; j < height; j++)
		{
//			if(i/4 * 4 ==i || j/4 * 4 ==j)
//			{
				int d = xy2d(n, i/8, j/8);
				//pixel[i,j]
				image[4*(i + j*width)] =   d % 255;
				image[4*(i +j*width)+1] = (d/2) % 255; 
				image[4*(i +j*width)+2] =  (d/4) % 255; 
				image[4*(i +j*width)+3] = ( d/max < percent)?255:10;
//			}
		}
	}
}


// http://en.wikipedia.org/wiki/Hilbert_curve
//convert (x,y) to d
int xy2d (int n, int x, int y) {
    int rx, ry, s, d=0;
    for (s=n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}

//rotate/flip a quadrant appropriately
void rot(int n, int *x, int *y, int rx, int ry) {
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
 
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}



void SDK_callbacks::sendApplet()
{
	//send from file
	sendFile(L"Resources\\Upload\\index.html",L"index.html");
	sendFile(L"Resources\\Upload\\genPellets.js",L"genPellets.js");
	sendFile(L"Resources\\Upload\\pellet.png",L"pellet.png");
	sendFile(L"Resources\\Upload\\player.png",L"player.png");
	sendFile(L"Resources\\Upload\\surface.png",L"surface.png");
	sendFile(L"Resources\\Upload\\bar-graph.css",L"bar-graph.css");

	//send a file read in memory.
	std::vector<BYTE> contentVector = ReadAllBytes("Resources\\spritesheet.png");
	if(contentVector.size() > 0)
	{
		BYTE *content = &contentVector[0];
		if(!LogiArxAddContentAs(content, contentVector.size(), L"spritesheet.png", L"image/png"))
			printf("lgAddContentAs failed with error code %d\n", LogiArxGetLastError());
	}

	//send string as file
	if(!LogiArxAddUTF8StringAs(L"<meta http-equiv='Content-Type' content='text/html;charset=utf-8'><h1>На берегу пустынных волн ვეპხის ტყაოსანი?,</h1><img src=spritesheet.png></img><br><img src=imageFromBitmap.png></img>",L"first.html", L""))
		printf("lgAddStringAs failed with error code %d\n", LogiArxGetLastError());

	//send image from byte array
	BYTE image[255*255*4];
	renderTail(image,255,255,.8f);
	if(!LogiArxAddImageFromBitmap(image, 255, 255, L"imageFromBitmap.png"))
		printf("lgAddImageAs failed with error code %d\n", LogiArxGetLastError());
	
	if(!LogiArxSetIndex(L"index.html"))
		printf("lgSetIndex failed with error code %d\n", LogiArxGetLastError());
	if(!LogiArxSetIndex(L"WRONGFILE.html"))
		printf("lgSetIndex failed with error code %d\n", LogiArxGetLastError());
}

void SDK_callbacks::updateMiniMap( wchar_t *numPelletsValue, wchar_t *pelletLocsNewContent )
{
	LogiArxSetTagPropertyById(L"numPellets",L"value", numPelletsValue);
	LogiArxSetTagContentById(L"pelletLocs", pelletLocsNewContent);
}

