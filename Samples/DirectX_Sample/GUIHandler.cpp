#include "GUIHandler.h"
#include <iostream>
#include "Camera.h"
#include "UI.h"
//#include "SDK_callbacks.h"


XMFLOAT4 numToGlyph(int cell, int dim)
{
	int row = cell / dim;
	int col =  3- cell % dim;
	float dt = 1.0f/ dim;
	return XMFLOAT4(4*col*dt, 4*row*dt, dt, dt);
}

GUIHandler::GUIHandler(ID3D11Device*  device)
:fps(0)
{
	facet = new Model(device, 2, 1);

}

GUIHandler::~GUIHandler(void)
{
}


void GUIHandler::printScore(int cur, int max)
{
	XMVECTOR dx = XMVectorSet(.139,0,0,0);

	XMMATRIX scale = XMMatrixScaling(-.12,-.25,1);
	XMMATRIX translate;		
	XMVECTOR pos=XMVectorSet(.934, -.857,1,0);

	Letter digit;
	int i;
	// draw "MAX"
	do{
		i = max % 10;
		max /=10;
		translate = XMMatrixTranslationFromVector(pos);
		
		digit.glyph = numToGlyph(i,4);
		XMStoreFloat4x4(&(digit.pos) , scale * translate);
		letters.push_back(digit);
		pos -= dx;
	}	while(max > 0);
	// Add / 

		translate = XMMatrixTranslationFromVector(pos);
		digit.glyph = numToGlyph(10,4);
		XMStoreFloat4x4(&(digit.pos) , scale * translate);
		letters.push_back(digit);
		pos -= dx;
	//Draw current
	do {
		i = cur % 10;
		cur/=10;
		translate = XMMatrixTranslationFromVector(pos);
		
		digit.glyph = numToGlyph(i,4);
		XMStoreFloat4x4(&(digit.pos) , scale * translate);
		letters.push_back(digit);
		pos -= dx;
	}
	while(cur > 0);

}

void GUIHandler::printStatus(CONNECT_STATUS status, char orientation)
{
	XMMATRIX scale= XMMatrixScaling(-.25,-.5,1);
	XMMATRIX translate = XMMatrixTranslation( -.85, .7,1);
	XMMATRIX rotate = XMMatrixIdentity();
	if(orientation == LOGI_ARX_ORIENTATION_LANDSCAPE) //Landscape
	{
		rotate = XMMatrixRotationZ( -90 * (0.0174532925)); //deg to rad
		translate = XMMatrixTranslation(-.7, .7,1);
	}
	XMStoreFloat4x4(&(statusIcon.pos),  scale * rotate * translate);
	statusIcon.glyph = numToGlyph((int)status,4);
}

void GUIHandler::render(ID3D11DeviceContext*  context)
{
	XMMATRIX ident= XMMatrixIdentity();
	Camera::instance()->setView(ident);
	Camera::instance()->setProj(ident);
	for(unsigned int i=0; i < letters.size(); i++)
	{
		Letter l = letters[i];
		Camera::instance()->setWorld(&(l.pos));
		Camera::instance()->setTexTransform(l.glyph);
			Camera::instance()->flushMatricies(context);
		facet->render(context);
	}

	Camera::instance()->setWorld(&(statusIcon.pos));
	Camera::instance()->setTexTransform(statusIcon.glyph);
	Camera::instance()->flushMatricies(context);
	facet->render(context);

	Camera::instance()->setTexTransform(XMFLOAT4(0,0,1,1));

	letters.clear();

}

//0 is all old value, 1 is all new value
float weightedAppend(float oldVal, float newVal, float weight)
{
	return (weight * newVal) + (1-weight) * oldVal;
}
float getAxis(int positive, int negative, float dist)
{
	return UI::isKeyDown(positive)*dist - UI::isKeyDown(negative)*dist;
}
void GUIHandler::printFPS(float dt)
{

	XMMATRIX scale = XMMatrixScaling(-.06,-.125,1);
	XMMATRIX translate;
	XMVECTOR pos = XMVectorSet(.854f,.822f,1,0);	
	XMVECTOR dx = XMVectorSet(.0645f,.0,0,0);
	Letter digit;
	int i;
	int fpsDigits = (int)dt;
	// draw "FPS"
	do
	{
		i = fpsDigits % 10;
		fpsDigits /=10;
		
		translate = XMMatrixTranslationFromVector(pos);
		digit.glyph = numToGlyph(i,4);
		XMStoreFloat4x4(&(digit.pos) , scale * translate);
		letters.push_back(digit);
		pos -= dx;
	}	while(fpsDigits > 0);
}