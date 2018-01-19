#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(float learningRate, const std::vector<int>& layers, bool onlyPositive)
	: m_LearningRate(learningRate)
	, m_NeuralNetworkType(onlyPositive)
{
	// cannot be less than 2 layers
	if (layers.size() < 2)
		return;

	CreateNNTree(layers, onlyPositive);
}


NeuralNetwork::NeuralNetwork(NeuralNetwork* nn)
	: m_Generation(nn->GetGeneration())
{
	std::vector<int> layers;
	for (int i = 0; i < nn->LayerCount(); i++)
		layers.push_back(nn->GetLayer(i)->NeuronCount());

	CreateNNTree(layers);

	for (int i = 0; i < m_Layers.size(); i++)
	{
		Layer* pLayer = m_Layers[i];

		for (int j = 0; j < pLayer->NeuronCount(); j++)
		{
			Neuron* pNeuron = pLayer->GetNeuron(j);
			pNeuron->SetBias(nn->GetLayer(i)->GetNeuron(j)->GetBias());

			for (int k = 0; k < pNeuron->DendriteCount(); k++)
			{
				Dendrite* pDendrite = pNeuron->GetDendrite(k);
				float weight = nn->GetLayer(i)->GetNeuron(j)->GetDendrite(k)->GetWeight();
				pDendrite->SetWeight(weight);
			}
		}
	}
}


NeuralNetwork::~NeuralNetwork()
{
	for (Layer* pLayrs : m_Layers)
		delete pLayrs;

	m_Layers.clear();
}


bool NeuralNetwork::Reproduce(NeuralNetwork* pParent1, NeuralNetwork* pParent2)
{
	if (pParent1->LayerCount() != pParent2->LayerCount())
	return false;

	for (int i = 0; i < pParent1->LayerCount(); i++)
	{
		Layer* pLayer1 = pParent1->GetLayer(i);
		Layer* pLayer2 = pParent2->GetLayer(i);

		if (pLayer1->NeuronCount() != pLayer2->NeuronCount())
			return false;

		if (i == 1)
			continue;

		for (int j = 0; j < pLayer1->NeuronCount(); j++)
		{
			Neuron* pNeuron1 = pLayer1->GetNeuron(j);
			Neuron* pNeuron2 = pLayer2->GetNeuron(j);

			if (pNeuron1->DendriteCount() != pNeuron2->DendriteCount())
				return false;

			for (int k = 0; k < pNeuron1->DendriteCount(); k++)
			{
				float weight = frand(0, 1) > 0 ? pNeuron1->GetDendrite(k)->GetWeight() : pNeuron2->GetDendrite(k)->GetWeight();
				m_Layers[i]->GetNeuron(j)->GetDendrite(k)->SetWeight(weight);
			}
		}
	}

	Mutate();

	return true;
}


void NeuralNetwork::CreateNNTree(const std::vector<int>& layers, bool onlyPositive)
{
	// creating layers
	for (int i = 0; i < layers.size(); i++)
	{
		Layer* pLayer = new Layer();
		m_Layers.push_back(pLayer);

		// creating neurons
		for (int j = 0; j < layers[i]; j++)
		{
			Neuron* pNeuron = new Neuron(onlyPositive);
			pLayer->AddNeuron(pNeuron);

			if (i == 0)
				pNeuron->SetBias(0);
			else
				// creating dendrites
				for (int d = 0; d < layers[i - 1]; d++)
					pNeuron->AddDendrite(new Dendrite(onlyPositive));
		}
	}
}


void NeuralNetwork::Mutate()
{
	for (int i = 1; i < m_Layers.size(); i++)
	{
		Layer* pLayer = m_Layers[i];

		for (int j = 0; j < pLayer->NeuronCount(); j++)
		{
			Neuron* pNeuron = pLayer->GetNeuron(j);

			float bias = pNeuron->GetBias();
			for (int k = 0; k < pNeuron->DendriteCount(); k++)
			{
				Dendrite* pDendrite = pNeuron->GetDendrite(k);

				float weight = pNeuron->GetDendrite(k)->GetWeight();
				float randNum = rand() % 1000;

				if (randNum <= 1)
					weight *= frand(-1, 1);

				if (randNum <= 2)
					weight *= -1;

				if (randNum <= 4)
					weight = frand(-1, 1);

				if (randNum <= 6)
					weight *= (frand(0, 1) + 1);

				if (randNum <= 8)
					bias *= (frand(0, 1) + 1);

				pDendrite->SetWeight(weight);
			}

			pNeuron->SetBias(bias);
		}
	}
}


std::vector<float> NeuralNetwork::Run(const std::vector<float>& input)
{
	// input data size must equal to input layer neurons count
	if (input.size() != m_Layers[0]->NeuronCount()) return std::vector<float>();

	for (int i = 0; i < m_Layers.size(); i++)
	{
		Layer* pLayer = m_Layers[i];

		for (int j = 0; j < pLayer->NeuronCount(); j++)
		{
			Neuron* neuron = pLayer->GetNeuron(j);

			// setting values to input layer
			if (i == 0)
				neuron->SetValue(input[j]);
			else
			{
				neuron->SetValue(0);
				for (int np = 0; np < m_Layers[i - 1]->NeuronCount(); np++)
				{
					float weight = neuron->GetDendrite(np)->GetWeight();
					float prevValue = neuron->GetValue();
					float prevLayerNeuronValue = m_Layers[i - 1]->GetNeuron(np)->GetValue();
					neuron->SetValue(prevValue + prevLayerNeuronValue * weight);
				}

				neuron->SetValue(Sigmoid(neuron->GetValue() + neuron->GetBias()));
				//neuron->SetValue(std::tanh(neuron->GetValue() + neuron->GetBias()));
			}
		}
	}

	Layer* pOutputLayer = m_Layers[m_Layers.size() - 1];
	int numOutput = pOutputLayer->NeuronCount();
	std::vector<float> output;
	for (int i = 0; i < pOutputLayer->NeuronCount(); i++)
		output.push_back(pOutputLayer->GetNeuron(i)->GetValue());

	return output;
}


bool NeuralNetwork::Train(std::vector<float> input, std::vector<float> output)
{
	if ((input.size() != m_Layers[0]->NeuronCount()) || (output.size() != m_Layers[m_Layers.size() - 1]->NeuronCount())) return false;

	Run(input);

	for (int i = 0; i < m_Layers[m_Layers.size() - 1]->NeuronCount(); i++)
	{
		Neuron* pNeuron = m_Layers[m_Layers.size() - 1]->GetNeuron(i);

		pNeuron->SetDelta(pNeuron->GetValue() * (1.0f - pNeuron->GetValue()) * (output[i] - pNeuron->GetValue()));

		// calculating delta for all hidden layers
		for (int j = m_Layers.size() - 2; j > 0; j--)
		{
			for (int k = 0; k < m_Layers[j]->NeuronCount(); k++)
			{
				Neuron* pN = m_Layers[j]->GetNeuron(k);

				pN->SetDelta(pN->GetValue() *
					(1 - pN->GetValue()) *
					m_Layers[j + 1]->GetNeuron(i)->GetDendrite(k)->GetWeight() *
					m_Layers[j + 1]->GetNeuron(i)->GetDelta());
			}
		}
	}

	for (int i = m_Layers.size() - 1; i > 0; i--)
	{
		for (int j = 0; j < m_Layers[i]->NeuronCount(); j++)
		{
			Neuron* n = m_Layers[i]->GetNeuron(j);
			n->SetBias(n->GetBias() + (m_LearningRate * n->GetDelta()));

			for (int k = 0; k < n->DendriteCount(); k++)
				n->GetDendrite(k)->SetWeight(n->GetDendrite(k)->GetWeight() + (m_LearningRate * (m_Layers[i - 1]->GetNeuron(k)->GetValue() * n->GetDelta())));
		}
	}

	return true;
}


int NeuralNetwork::Compare(NeuralNetwork* nn)
{
	if (m_Fitness > nn->GetFitness())
	{
		return 1;
	}
	else if (m_Fitness < nn->GetFitness())
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


int NeuralNetwork::LayerCount()
{
	return m_Layers.size();
}