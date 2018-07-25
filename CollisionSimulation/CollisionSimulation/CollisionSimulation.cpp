// CollisionSimulation.cpp : Defines the entry point for the console application.
//

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

double getFunction(double x, void *params) {
	return x + 6;
}

int main()
{
	double a = 0., b = 100; // limits of integration
	double abserr = 0., relerr = 1.e-7; // requested errors
	double result; // the integral value
	double error; // the error estimate
	double x = 5;

	size_t np = 1000; // work area size
	gsl_integration_workspace *w = gsl_integration_workspace_alloc(np);

	gsl_function F2;

	//TODO fix this
	F2.function = &getFunction;
	//F2.params = &x;

	gsl_integration_qag(&F2, a, b, abserr, relerr, np, GSL_INTEG_GAUSS15, w, &result, &error);

	printf("result = % .18f\n", result);
	printf("estimated error = % .18f\n", error);
	printf("intervals = %zu\n", w->size);

	gsl_integration_workspace_free(w);

	std::cout << result << std::endl;

	printf("Starting root finding test");



	//TODO add input

	vhs vhs;
	
	vhs.addParticle(new Particle(1.0f, new Point(0,0), new Vector(2,0)));

	vhs.addParticle(new Particle(1.0f, new Point(10,10), new Vector(2,0)));
	
	vhs.setTarget(Particle(10.0f, new Point(0,5), new Vector(0,0)));

	vhs.run();

	//vhs.showParticles();

	std::cout << "\n\n\nProgram ended" << std::endl;
	system("pause");
    return 0;
}


