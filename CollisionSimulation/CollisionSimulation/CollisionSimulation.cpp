// CollisionSimulation.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <string>
#include <chrono>
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

	//std::cout << "NUMBER OF TESTS 100--->" << std::endl;

	double cr1[3];
	double cr2[3];
	rp.get_3D_Cr(cr1);
	rp.get_3D_Cr(cr2);
	double aCoord[3];
	double bCoord[3];
	rp.get_coordinates(aCoord);
	rp.get_coordinates(bCoord);

	//Changing frame of reference to a
	double newV[3] = { cr1[0] - cr2[0] , cr1[1] - cr2[1] , cr1[2] - cr2[2] };
	std::cout << "newV : " << newV[0] << " " << newV[1] << " " << newV[2] << std::endl;

	std::cout << "1st Cr of air molecules : " << *(cr1 + 0) << " , " << *(cr1 + 1) << " , " << *(cr1 + 2) << std::endl;
	std::cout << "Coordinates : " << *(aCoord + 0) << " , " << *(aCoord + 1) << " , " << *(aCoord + 2) << std::endl;
	std::cout << "2nd Cr of air molecules : " << *(cr2 + 0) << " , " << *(cr2 + 1) << " , " << *(cr2 + 2) << std::endl;
	std::cout << "Coordinates : " << *(bCoord + 0) << " , " << *(bCoord + 1) << " , " << *(bCoord + 2) << std::endl;

	std::cout << "Cr_ref of hydrogen : " << rp.get_CrRef() << std::endl;

	std::cout << "B of particles : " << rp.get_B(aCoord, bCoord, newV) << std::endl;

	std::cout << "SENDING TO MODELS--->" << std::endl;

	system("pause");

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

	/*********************************
	Variable hard sphere collision
	b : miss-distance impact parameter 
	m : mass
	d : diameter
	c : molecular velocity vector
	v : w - 1/2 relative speed exponent
	**********************************/
	//double d = rp.get_DRef() * pow(rp.get_CrRef() / newV, rp.viscosity_index - 0.5);
	//TODO FIX
	//vhs vhs(rp.get_B(aCoord, bCoord, newV), d, newV);

	std::cout << "PROGRAM END..." << std::endl;
	system("pause");
    return 0;
}


