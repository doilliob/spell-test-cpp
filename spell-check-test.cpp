// spell-check-test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "IDictionary.h"
#include "Dictionary.h"
#include "Processor.h"

#include "CompareAlgorithm.h"

using namespace spellchecker;

int main()
{
	IDictionaryPtr dictPtr(new Dictionary());
	Processor processor(dictPtr);
	processor.Start();

	return 0;
}
