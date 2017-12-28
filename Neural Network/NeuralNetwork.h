#pragma once
#include "Layer.h"
#include <iostream>
#include <time.h>

#define NeuronScale 10
#define DistanceBetweenNeuronsX 100
#define DistanceBetweenNeuronsY 25

inline float Sigmoid(float x)
{
	return 1.0f / (1.0f + exp(-x));
}

class NeuralNetwork
{
public:
	NeuralNetwork(float learningRate, const std::vector<int>& layers, bool onlyPositive = false);

	// Clone Neural Network
	NeuralNetwork(NeuralNetwork* nn);

	bool Reproduce(NeuralNetwork* pParent1, NeuralNetwork* pParent2);

	void CreateNNTree(const std::vector<int>& layers, bool onlyPositive = false);

	void Mutate();

	std::vector<float> Run(const std::vector<float>& input);

	bool Train(std::vector<float> input, std::vector<float> output);

	/*
		Compare two networks, 
		returns 1 if first neural network fitness is greather then second
		-1 if first fitness is less then second
		0 if its fitnesses is similar
	*/
	int Compare(NeuralNetwork* nn);

	Layer* GetLayer(int layer) { return m_Layers[layer]; }
	std::vector<Layer*> GetLayers() { return m_Layers; }

	// generation
	void AddGeneration(int toAdd) { m_Generation += toAdd; }
	void SetGeneration(int generation) { m_Generation = generation; }
	int GetGeneration() { return m_Generation; }

	// fitness
	void AddFitness(float toAdd) { m_Fitness += toAdd; }
	void SetFitness(float fitness) { m_Fitness = fitness; }
	float GetFitness() { return m_Fitness; }

	int LayerCount()
	{
		return m_Layers.size();
	}

private:
	// fields
	std::vector<Layer*> m_Layers;
	float m_LearningRate;
	int m_Generation;
	float m_Fitness;
	_Uint32t m_NeuralNetworkType;
};