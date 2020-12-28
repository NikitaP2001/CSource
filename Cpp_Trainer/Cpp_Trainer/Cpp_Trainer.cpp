#include "MemeReader.h"
#include <iostream>
int main()
{
	MemeReader* proc = new MemeReader();
	try {
		proc->Open();
		std::cout << "Succsesful. pID = " << proc->getPID() << "\n";
	}
	catch(_exception e) {
		std::cout << e.name;
	}
}
