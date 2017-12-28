#pragma once
#include <cstdlib>
#include "..\math.h"
class Dendrite
{
public:
	Dendrite(bool onlyPositive = false)
		: m_Weight(NULL)
	{
		// randomizing weight
		if (onlyPositive)
			m_Weight = frand(0, 1);
		else
			m_Weight = frand(-1, 1);
	}

	void SetWeight(float value) { m_Weight = value; }
	float GetWeight() { return m_Weight; }

private:
	// fields
	float m_Weight;
};