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
	std::cout << "PROGRAM START--->" << std::endl;
	std::cout << "INPUT VARIBLES FOR RANDOM PARAMETERS--->" << std::endl;
	RandomParameters rp;

	std::string input;
	std::cout << "Input coefficient of viscosity (0.845 * 10^-5): ";
	std::getline(std::cin, input);
	
	if (!input.empty()) {
		rp.setCoefficientOfViscosity(atof(input.c_str()) * pow(10, -5));
		input.clear();
	}

	std::cout << "Input mass of atom (3.34 * 10^-27): ";
	std::getline(std::cin, input);

	if (!input.empty()) {
		rp.setMass(atof(input.c_str()) * pow(10, -27));
		input.clear();
	}

	std::cout << "Input viscosity index (0.67): ";
	std::getline(std::cin, input);

	if (!input.empty()) {
		rp.setViscosityIndex(atof(input.c_str()));
		input.clear();
	}
	
	std::cout << "Input diameter (2.92 * 10^-10): ";
	std::getline(std::cin, input);

	if (!input.empty()) {
		rp.setDiameter(atof(input.c_str()) * pow(10,-10));
		input.clear();
	}

	std::cout << "Input temperature (273.0): ";
	std::getline(std::cin, input);

	if (!input.empty()) {
		rp.setTemperature(atof(input.c_str()));
		input.clear();
	}

	std::cout << "NUMBER OF TESTS 100--->" << std::endl;

	//TESTING RANDOM VARIABLES
	std::cout << "Random Cr of air molecules : " << rp.get_3D_Cr()[0] << " , " << rp.get_3D_Cr()[1] << " , " << rp.get_3D_Cr()[2] << std::endl;

	std::cout << "Cr_ref of hydrogen : " << rp.get_CrRef() << std::endl;

	rp.get_B();

	std::cout << "SENDING TO MODELS--->" << std::endl;

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

	//Gas Hydrogen_Gas = Gas(3.0, 3.34 * pow(10, -27), 0.845 * pow(10, -5), 0.67);
	//CollisionDynamics collDy2(0.5, 2.0, Hydrogen_Gas);

	//std::cout << "Hydrogen gas kappa : " << Hydrogen_Gas.getKappa() << std::endl;

	/*
	Starting of variable hard sphere collision
	Change funtion parameters here:
	miss-distance impact parameter : b,
	//mass : m,
	diameter : d,
	molecular velocity vector : c,
	relative speed exponent v : w - 1/2
	*/
	vhs vhs;
	
	//b , diameter , velocity
	//double d = rp.get_DRef * pow(rp.get_CrRef / rp.get_3D_Cr, rp.viscosity_index_hydrogen - 0.5);
	//vhs.addParams(rp.get_B, d, );
	//vhs.addParticle(new Particle(0.10, new Vector(0, 0), new Vector(0.019, 0.05)));
	vhs.run();

	vhs.addParams(0.02f, 1.0f, 10);
	vhs.run();

	std::cout << "PROGRAM END..." << std::endl;
	system("pause");
    return 0;
}


