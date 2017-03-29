#ifndef _QUADRANGLEGENERATOR_H_
#define _QUADRANGLEGENERATOR_H_

#include "Global.h"
#include "Line_Utils.h"

class QuadrangleGenerator{
public:
	QuadrangleGenerator()
	{

	}

	Quadrangle Generate(const Line &leftLine, const Line &topLine, const Line &rightLine, const Line &bottomLine) const;


private:
	DISALLOW_CLASS_COPY_AND_ASSIGN(QuadrangleGenerator);
};




#endif // _QUADRANGLEGENERATOR_H_
