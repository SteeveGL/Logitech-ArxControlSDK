//--------------------------------------------------------------------------------------
// File: main.cpp
//
// This application displays a triangle using Direct3D 11
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include <iostream>
#include "Renderer.h"
#include "SDK_callbacks.h"
#include "Model.h"
#include "UI.h"
#include "Hiro.h"
#include "SphereWorld.h"
#include "Camera.h"
#include <time.h>
#include "Pellet.h"
#include "GUIHandler.h"

#include <windows.h>
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
#define NUM_PELLETS 150
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------

Renderer* r;
Hiro* h;
GUIHandler* goo;
float t=0;
float endTimer=0;
bool paused=false;
bool sendTail = false;


//SDK_Callbacks member
SDK_callbacks sdkCallbacks;
//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
void Render();
void Update(float dt);

void reset()
{
	t =0;
	endTimer=0;
	Pellet::resetAll();
	h->reset();
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	srand((unsigned int )time(NULL));
	
	r = Renderer::instance();
	r->init(hInstance, nCmdShow, WndProc);
	//Init game: 
    sdkCallbacks.init(r->hwnd());
	Pellet* p;
	for(int i=0; i < NUM_PELLETS; i++)
	{
		p = new Pellet(r->device());
	}
	h = new Hiro(r->device(), sdkCallbacks);
	SphereWorld::init(r->device());
	goo = new GUIHandler(r->device());
	Camera::init(r->device());


    // Main message loop
    MSG msg = {0};
	clock_t lastFrame = clock();	
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			clock_t curFrame = clock();
			float dt = (curFrame - lastFrame)/ (float)CLOCKS_PER_SEC;
			Update(dt);
            Render();
			lastFrame = curFrame;
        }
    }

    r->CleanupDevice();

    return ( int )msg.wParam;
}


int paramToButton(WPARAM wparam)
	{
		if(wparam & MK_LBUTTON)
			return 0;
		if(wparam & MK_MBUTTON)
			return 1;
		if(wparam & MK_LBUTTON)
			return 2;
		return -1;
	}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;
		case WM_KEYDOWN:
			UI::setKey(wParam,true);
			break;
		case WM_KEYUP:
			UI::setKey(wParam, false);
			break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			UI::setClick(paramToButton(wParam), true);
			return 0;
		case WM_LBUTTONUP:
			UI::setClick(paramToButton(MK_LBUTTON), false);
		case WM_MBUTTONUP:
			UI::setClick(paramToButton(MK_MBUTTON), false);
		case WM_RBUTTONUP:
			UI::setClick(paramToButton(MK_RBUTTON), false);		
			return 0;
		case WM_MOUSEMOVE:
			UI::setMousePos(MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y);
			return 0;
		case WM_ARXAPP_SDK_CALLBACK:
			{
				arxAppCallbackMessage *callbackMessageStruct = reinterpret_cast<arxAppCallbackMessage*> (wParam);
				printf("Received callback on main thread for ");
				switch(callbackMessageStruct->eventType)
				{

				case  LOGI_ARX_EVENT_FOCUS_INACTIVE:
					{
						printf("applet focus inactive \n");
						sdkCallbacks.status = SLEEPING;
					}
					break;

				case  LOGI_ARX_EVENT_FOCUS_ACTIVE:
					{
						sdkCallbacks.status = ACTIVE;
						sdkCallbacks.curOrientation = (char) callbackMessageStruct->eventValue;
						if(callbackMessageStruct->eventValue == LOGI_ARX_ORIENTATION_PORTRAIT)
						{
							printf("applet focus active in portrait mode\n");
						}
						if(callbackMessageStruct->eventValue == LOGI_ARX_ORIENTATION_LANDSCAPE)
						{
							printf("applet focus active in landscape mode\n");

						}
						break;
					}

				case  LOGI_ARX_EVENT_MOBILEDEVICE_ARRIVAL:
					{
						printf("mobile device arrival\n");
						sdkCallbacks.status = CONNECTED;
						sdkCallbacks.sendApplet();
						break;
					}

				case  LOGI_ARX_EVENT_MOBILEDEVICE_REMOVAL:
					{
						printf("mobile device removal\n");
						sdkCallbacks.status = DISCONNECTED;
						break;
					}
				case  LOGI_ARX_EVENT_TAP_ON_TAG:
					{
						wprintf(L"click on tag with id %ls\n",callbackMessageStruct->eventArg);
						if(callbackMessageStruct->eventArg)
						{
							UI::setRemoteClicked(callbackMessageStruct->eventArg);
						}
						break;
					}

				default:
					printf("unknown message %d:%d", callbackMessageStruct->eventType, callbackMessageStruct->eventValue);
					break;
				}
				return 0;
			}
			
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}

    return 0;
}


//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
    // Clear the back buffer 
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	float hitColor[4] = { .6f, 0.125f, 0.1f, 1.0f }; 
	float pauseColor[4] = {.3f, .3f, .3f, 1.0f };
	if(h->isHit)
	{
		r->clearView(hitColor);
	}
	else if(!paused)
	{
		r->clearView( ClearColor );
	}
	else
	{
		r->clearView( pauseColor );
	}
    // Clear the depth buffer to 1.0 (max depth)
    XMFLOAT4X4 mView;
    XMMATRIX xProj;

    xProj = XMMatrixPerspectiveFovLH(0.785398163f,  640.0f/480.0f, .01f, 1000.0f);
	h->getViewMatrix(&mView) ;

	Camera::instance()->setProj(xProj);
	Camera::instance()->setView(&mView);
	r->useLogo();

	//Render game objects
	SphereWorld::instance()->render(r->context());
	h->render(r->context());
	Pellet::renderAll(r->context());
	
    // Reset for the gui

	r->clearDepth();
	r->useGUI();

	goo->render(r->context());

    // Present the information rendered to the back buffer to the front buffer (the screen)

	r->present();
}


void Update(float dt ){
	Camera::instance()->setColor(XMFLOAT4(0,0,0,0));

	if(!paused){
		goo->printStatus(sdkCallbacks.getStatus(), sdkCallbacks.getOrientation());
		goo->printScore(h->getCaptured(),h->left);
		goo->printFPS((float)h->score);

		t+= dt;
		if(UI::isKeyClicked(UI::KEY_SPACE) )
		{
			sendTail = !sendTail;
		}
		if(t > .1f && sendTail)
		{
			t=0; 
			h->drawTail();
		}

		if(h->isHit || h->left == 0)
		{
			if(h->isHit)
				Camera::instance()->setColor(XMFLOAT4(min(endTimer,1),0,0,.3f));
			else
				Camera::instance()->setColor(XMFLOAT4(0,min(endTimer,1),0,.3f));
			endTimer += dt;
			dt *= 1 - endTimer; 
		}
		if(endTimer < 1)
		{
			h->update(dt);
			Pellet::updateAll(dt);
		}
		if(endTimer >5)
		{
			reset();
		}

		if(UI::isMouseClicked(paramToButton(MK_LBUTTON)))
		{
			int x,y;
			UI::getMousePos(x,y);
			printf("Left mouse is clicked, (%d,%d)\n",x,y);

		}
	}
	else
	{
		Camera::instance()->setColor(XMFLOAT4(.3f,.3f,.3f,.6f));
	}

	if(UI::isKeyClicked('P')|| UI::isRemoteClicked(L"pause"))
	{
		paused = !paused;
	}
	if(UI::isKeyDown('Q') && UI::isKeyDown(UI::KEY_SHIFT) && sdkCallbacks.appletInitialized)
	{
		printf("shutting down applet");
		sdkCallbacks.shutdown();
	}
	if(UI::isKeyDown('I') && UI::isKeyDown(UI::KEY_SHIFT) && !(sdkCallbacks.appletInitialized))
	{
		printf("re-initializing applet");
		sdkCallbacks.init(r->hwnd());
		sdkCallbacks.sendApplet();
	}
	UI::update();
}