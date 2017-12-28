#pragma once

#ifndef PATTERNS_H
#define PATTERNS_H

#include "stdafx.h"
#include "autoco.h"
#include "Neural Network\NeuralNetwork.h"

class CPatterns {
public:
	CPatterns(const std::vector<int>& layers);
	~CPatterns();

	NeuralNetwork* GetBrain();

private:

	NeuralNetwork* m_pBrain;

};

#endif /* PATTERNS_H */
