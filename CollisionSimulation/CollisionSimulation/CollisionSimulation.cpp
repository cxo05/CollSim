// CollisionSimulation.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <string>
#include "vhs.h"
#include "Point.h"
#include "Ipl.h"
#include <stdio.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>
#include "CollisionDynamics.h"
#include "Gas.h"
#include <math.h>
#include "RandomParameters.h"

int main()
{
	std::cout << "\n\n\n//////////////PROGRAM START///////////////" << std::endl;
	/*
	Starting of collision dynamics (finding deflection angle) testing 
	Change function parameters here: 
	miss-distance impact parameter : b, 
	mass : m,
	molecular velocity vector : c,
	constant kappa : k,
	exponent eta : n
	*/
	Gas g = Gas();
	CollisionDynamics collDy(0.5, 2.0, 2.0, 1.0, 500.0, g);

	//std::cout << "\n\n\n//////////////GAS TEST START///////////////" << std::endl;

	Gas Hydrogen_Gas = Gas(3.0, 3.34 * pow(10, -27), 0.845 * pow(10, -5), 0.67);
	//CollisionDynamics collDy2(0.5, 2.0, Hydrogen_Gas);

	std::cout << "Hydrogen gas kappa : " << Hydrogen_Gas.getKappa() << std::endl;

	/*
	Starting of variable hard sphere collision
	Change funtion parameters here:
	miss-distance impact parameter : b,
	//mass : m,
	diameter : d;
	molecular velocity vector : c,
	relative speed exponent v : w - 1/2
	*/
	vhs vhs;
	//vhs.setTarget(Particle(0.10, new Vector(1, 2.5), new Vector(0, 0)));
	//b , diameter , velocity
	vhs.addParams(0.01f, 0.1f, 10);
	//vhs.addParticle(new Particle(0.10, new Vector(0, 0), new Vector(0.019, 0.05)));
	vhs.run();

	vhs.addParams(0.02f, 1.0f, 10);
	vhs.run();

	std::cout << "\n\n\n//////////////PROGRAM END///////////////" << std::endl;

	std::cout << "//////////////RANDOM PARAMETERS///////////////" << std::endl;
	
	RandomParameters rp;
	for (int i = 0;i < 10;i++) {
		std::cout << "Random Cr of air molecules : "<< rp.get_Cr() << std::endl;
	}
	
	std::cout << "Cr_ref of hydrogen : " << rp.get_CrRef() << std::endl;
	
	rp.get_B();

	system("pause");
    return 0;
}


