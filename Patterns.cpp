#include "Patterns.h"


CPatterns::CPatterns(const std::vector<int>& layers)
{ 
	m_pBrain = new NeuralNetwork(1, layers, true);
}


CPatterns::~CPatterns() 
{
	delete m_pBrain;
}


NeuralNetwork* CPatterns::GetBrain()
{
	return m_pBrain;
}
