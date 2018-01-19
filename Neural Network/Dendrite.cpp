#include "Dendrite.h"

Dendrite::Dendrite(bool onlyPositive)
	: m_Weight(NULL)
{
	// randomizing weight
	if (onlyPositive)
		m_Weight = frand(0, 1);
	else
		m_Weight = frand(-1, 1);
}


Dendrite::~Dendrite()
{ }


void Dendrite::SetWeight(float value) 
{ 
	m_Weight = value; 
}


float Dendrite::GetWeight()
{ 
	return m_Weight; 
}
