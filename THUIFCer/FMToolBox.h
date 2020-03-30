#pragma once
#include <fstream>
#include "matlib.h"
#include "rescale.h"
using namespace std;
#define GRIDNUM 100

class FMToolBox
{
public:
	FMToolBox(void);
public:
	~FMToolBox(void);

public:
	void PathPlanning();
};
