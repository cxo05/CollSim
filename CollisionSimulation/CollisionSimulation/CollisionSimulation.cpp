// CollisionSimulation.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>

#include "VHS.h"
#include "CollisionDynamics.h"
#include "Gas.h"
#include "RandomParameters.h"
#include "A2eta.h"

#include <gsl/gsl_integration.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

#include <Eigen/Dense>

bool collDyFlag;

int main()
{
	std::cout << "PROGRAM START--->" << std::endl;
	
	//Data file initialization
	std::ofstream dataFile, dataFile_difference;
	dataFile.open("data.csv");
	dataFile << "Initial Pos 1,Initial Velocity 1,Initial Pos 2,Initial Velocity 2,Time,VHS_d,B,VHS Deflection Angle,VHS Final Velocity 1, VHS Final Velocity 2, IPL Deflection Angle, IPL Final Velocity 1, IPL Final Velocity 2, Initial energy, Final energy vhs, Final energy ipl e" << std::endl;
	dataFile_difference.open("data2.csv");
	dataFile_difference << "Temperature, Difference" << std::endl;

	std::cout << "INPUT VARIBLES FOR RANDOM PARAMETERS--->" << std::endl;

	RandomParameters rp;

	//Get A_2(eta)
	/*std::string etaInput;
	std::cout << "Input eta to get A_2(eta) : ";
	std::getline(std::cin, etaInput);
	double a2eta = A2eta::getA2eta(atof(etaInput.c_str()));
	std::cout << "A_2(eta) : " << a2eta << std::endl;*/

	//double cov = (5 * rp.getMass() * pow(8.314 * rp.getTemperature() / 3.1415, 0.5) * pow(2 * rp.getMass() * 8.314 * rp.getTemperature() / 1.403/*kappa*/, 2/(eta-1)))/(8 * a2eta * tgamma(4-2/(eta-1)));
	//rp.setCoefficientOfViscosity(cov);
	
	Gas Hydrogen_Gas = Gas(0.333, 3.34e-27, 273.0, 0.845e-5, 0.67, 2.92e-10);
	Gas Argon_Gas = Gas(0.333, 66.3e-27, 273.0, 2.117e-5, 0.81, 4.11e-10);
	Gas Xenon_Gas = Gas(0.333, 218e-27, 273.0, 2.107e-5, 0.85, 5.74e-10);
	Gas Gas = Xenon_Gas;

	rp.setCoefficientOfViscosity(Gas.getViscosity_co());
	rp.setMass(Gas.getMol_mass());
	rp.setDiameter(Gas.getDiameter());
	rp.setTemperature(Gas.getTemperature());
	rp.setViscosityIndex(Gas.getViscosity_index());
	rp.setV();

	Eigen::IOFormat CommaInitFmt(6, Eigen::DontAlignCols, "|", "|", "", "", "", "");

	double difference = 0;
	int iterations = 2000;
	
	for (int u = 100; u < 450; u++) {
		rp.setTemperature(u);
		//std::cout << u << std::endl;
		for (int i = 0; i < iterations; i++) {
			collDyFlag = false;
			//std::cout << "GENERATING DATA SET--->" << std::endl;

			Eigen::Vector3d cr1 = rp.get_3D_Cr();
			Eigen::Vector3d cr2 = rp.get_3D_Cr();

			//Changing frame of reference
			Eigen::Vector3d newV = cr1 - cr2;

			double VHS_d = rp.getDiameter() * pow(rp.get_CrRef() / newV.norm(), rp.viscosity_index - 0.5);

			Eigen::Vector3d aCoord(0, 0, 0);
			Eigen::Vector3d bCoord = rp.get_coordinates_at_contact(rp.getDiameter(), cr1, cr2);

			double b = rp.get_B(aCoord, bCoord, newV);

			//std::cout << "SENDING TO MODELS--->" << std::endl;

			/*********************************
			Collision dynamics (Deflection angle)
			b : miss-distance impact parameter
			m : mass
			c : molecular velocity vector
			k : constant kappa
			n : exponent eta
			**********************************/
			Eigen::Vector3d xAxis(1, 0, 0);
			Eigen::Vector3d yPrime(0, newV.data()[2] * pow(pow(newV.data()[1], 2) + pow(newV.data()[2], 2), -0.5), -newV.data()[1] * pow(pow(newV.data()[1], 2) + pow(newV.data()[2], 2), -0.5));
			Eigen::Vector3d collisionPlane;
			double epsilon;

			// If cr is in the same direction as the axis, our collision plane will be 0
			// hence an if else case is required for this particular case
			if ((aCoord - bCoord).cross(newV).norm() == 0) {
				Eigen::Vector3d shifted_cr1(cr1.data()[0] + 0.1, cr1.data()[1], cr1.data()[2]);
				collisionPlane = (aCoord - bCoord).cross(shifted_cr1 - cr2);
			}
			else {
				collisionPlane = (aCoord - bCoord).cross(newV);
			}

			// If epsilon is < 0 (wrong direction), we will take -epsilon instead
			if (yPrime.cross(collisionPlane).dot(newV) < 0) {
				epsilon = acos(yPrime.dot(collisionPlane) / (yPrime.norm() * collisionPlane.norm()));
			}
			else {
				epsilon = -acos(yPrime.dot(collisionPlane) / (yPrime.norm() * collisionPlane.norm()));
			}

			CollisionDynamics collDy(b, newV.norm(), Hydrogen_Gas, cr1, cr2, newV, epsilon);

			if (collDyFlag) {
				iterations--;
				continue;
			}

			/*********************************
			Variable hard sphere collision
			b : miss-distance impact parameter
			d : diameter
			c : molecular velocity vector
			v : w - 1/2 relative speed exponent
			**********************************/
			double angle = acos(b / VHS_d) * 180 / M_PI;
			if (std::isnan(angle)) {
				angle = 0;
			}

			VHS vhs;
			std::vector<Eigen::Vector3d> vhsFinalVelocities = vhs.run(b, VHS_d, aCoord, bCoord, cr1, cr2);

			Eigen::Vector3d iplFinalV1 = collDy.getFinalV1();
			Eigen::Vector3d iplFinalV2 = collDy.getFinalV2();

			//Writing to file
			dataFile << aCoord.data()[0] << "|" << aCoord.data()[1] << "|" << aCoord.data()[2] << ","
				<< cr1.data()[0] << "|" << cr1.data()[1] << "|" << cr1.data()[2] << "|" << ","
				<< bCoord.data()[0] << "|" << bCoord.data()[1] << "|" << bCoord.data()[2] << ","
				<< cr2.data()[0] << "|" << cr2.data()[1] << "|" << cr2.data()[2] << "|" << ","
				<< rp.getTimeToCollision() << ","
				<< VHS_d << ","
				<< b << ","
				<< angle * 2 << ","
				<< vhsFinalVelocities.at(0).norm() << ","
				<< vhsFinalVelocities.at(1).norm() << "|" << ","
				<< collDy.getDeflectionAngle() << ","
				<< iplFinalV1.norm() << ","
				<< iplFinalV2.norm() << ","
				<< (cr1.norm() * cr1.norm()) + (cr2.norm() * cr2.norm()) << ","
				<< vhsFinalVelocities.at(0).norm() * vhsFinalVelocities.at(0).norm() + vhsFinalVelocities.at(1).norm() * vhsFinalVelocities.at(1).norm() << ","
				<< iplFinalV1.norm() * iplFinalV1.norm() + iplFinalV2.norm() * iplFinalV2.norm() << ","
				<< std::endl;*/
		
			Eigen::Vector3d cr1_difference = vhsFinalVelocities.at(0) - iplFinalV1;
			Eigen::Vector3d cr2_difference = vhsFinalVelocities.at(1) - iplFinalV2;
			if (!std::isnan(pow(cr1_difference.norm(), 2) + pow(cr2_difference.norm(), 2))){
				//difference += pow(cr1_difference.norm(), 2) + pow(cr2_difference.norm(), 2)/ iterations;
				difference += sqrt(pow(cr1_difference.norm(), 2) + pow(cr2_difference.norm(), 2));
			}
			else {
				iterations--;
			}
		}
		std::cout << "Difference = " << difference << std::endl;
		std::cout << "Iterations = " << iterations << std::endl;
		dataFile_difference << u << "," << difference/iterations << std::endl;
	}

	std::cout << "PROGRAM END..." << std::endl;
	system("pause");
    return 0;
}