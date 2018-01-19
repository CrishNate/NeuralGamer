#include "Neuron.h"


Neuron::Neuron(bool onlyPositive)
	: m_Value(NULL)
{
	// generating random value for bias
	if (onlyPositive)
		m_Bias = frand(0, 1);
	else
		m_Bias = frand(-1, 1);
}


Neuron::~Neuron()
{
	for (std::vector< Dendrite* >::iterator it = m_Dendrites.begin(); it != m_Dendrites.end(); it++)
		delete *it;

	m_Dendrites.clear();
}


bool Neuron::AddDendrite(Dendrite* neuron)
{
	m_Dendrites.push_back(neuron);
	return true;
}