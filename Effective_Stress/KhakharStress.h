#ifndef KHAKHAR_H
#define KHAKHAR_H
#include <vector>
#include <map>

using namespace std;

class KhakharStress
{
	public:
		KhakharStress(double&,double,vector<double>&,double,double,vector<double>,double,double);
		double GetTotalNormalStress();
		double GetTotalShearStress();
		double GetFrictionForce();
	protected:
		double bulkDensity;
		double reposeAngle;
		double baseAreaFraction;
		double WallFriction;
		double RDwidth;
		vector<double> volFraction;
		double totalNormalStress;
		double totalShearStress;
		double firstTotalNormalStress;
		double firstTotalShearStress;
		map<double,double> kineticNormalStress;
		map<double,double> kineticShearStress;
		double frictionForce;

};

#endif

