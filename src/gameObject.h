#pragma once
#include "gameManager.h";
#include "layout.h"

class gameObject
{
public:
	Layout* layout;

	gameObject(Layout* baseLayout) : layout(baseLayout)
	{

	}

	void Init()
	{
		if (!isInitialized)
		{
			InitOnce();
			isInitialized = true;
		}
	}

	virtual void OnResize() = 0;

	virtual void OnUpdate() = 0;

	virtual void OnRender() = 0;

protected:

	virtual void InitOnce() = 0;

public:
	bool enabled = true;
	bool isInitialized = false;
};