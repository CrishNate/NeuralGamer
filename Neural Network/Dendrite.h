#pragma once
#include <cstdlib>
#include "..\math.h"

class Dendrite
{
public:
	Dendrite(bool onlyPositive = false);
	~Dendrite();

	void SetWeight(float value);
	float GetWeight();

private:
	// fields
	float m_Weight;
};