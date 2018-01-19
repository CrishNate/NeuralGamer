#include "Layer.h"


Layer::Layer()
{ }


Layer::~Layer()
{
	for (std::vector< Neuron* >::iterator it = m_Neurons.begin(); it != m_Neurons.end();it++)
		delete *it;

	m_Neurons.clear();
}


int Layer::NeuronCount()
{ 
	return m_Neurons.size(); 
}


bool Layer::AddNeuron(Neuron* neuron)
{
	m_Neurons.push_back(neuron);
	return true;
}


Neuron* Layer::GetNeuron(int neuronInx) 
{ 
	return m_Neurons[neuronInx]; 
}


std::vector<Neuron*> Layer::GetNeurons()
{ 
	return m_Neurons; 
}
