// CollisionSimulation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "vhs.h"

using namespace std;

int main()
{
	cout << "Starting program" << endl;

	cout << "Testing VHS class\n\n\n" << endl;
	//TODO add input

	vhs vhs;
	
	vhs.addParticle(new Particle(10.0f, 0, 0, new Vector(2,2)));

	vhs.setTarget(Particle(10.0f, 1, 1, new Vector(1,1)));

	vhs.showParticles();

	cout << "Program ended" << endl;
	system("pause");
    return 0;
}


