#pragma once
#include "Neuron.h"

class Layer
{
public:
	Layer();
	~Layer();

	int NeuronCount();
	bool AddNeuron(Neuron* neuron);
	Neuron* GetNeuron(int neuronInx);
	std::vector<Neuron*> GetNeurons();

private:
	// fields
	std::vector<Neuron*> m_Neurons;
};