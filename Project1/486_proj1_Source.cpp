#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<math.h>
#include "math.h"

using namespace std;

//functions made
void pause();
double calcMagnitude(gfx::Vector a);
vector<double> returnNumbers(int i, string); //returns numbers from a line within a string.
int returnDigit(char digit);
double calcDistance(gfx::Vector a, gfx::Vector b);

struct output
{
	string description;
	vector<double> data;
};
output scan(string);
using namespace std;


struct Triangle {
	int id = 0;
	gfx::Vector pts[3];
};

struct Camera {
	gfx::Vector eye;
	int id = 9;
	double d_max = 100;
	double min_sub_pt = 10; //d_min
	double v_width = 5.7735;
	double v_height = 5.7735;
	//Frustrum Vertices that are Near
	gfx::Vector frust_Vbl;
	gfx::Vector frust_Vtl;
	gfx::Vector frust_Vbr;
	gfx::Vector frust_Vtr;
	//Frustrum Vertices that are Far
	gfx::Vector frust_Wbl;
	gfx::Vector frust_Wtl;
	gfx::Vector frust_Wbr;
	gfx::Vector frust_Wtr;
	//Frustrum Plane Normals
	gfx::Vector frust_Nn;
	gfx::Vector frust_Nf;
	gfx::Vector frust_Nl;
	gfx::Vector frust_Nr;
	gfx::Vector frust_Nb;
	gfx::Vector frust_Nt;
};

gfx::Vector calcPoint(double azimuth, double elevation, gfx::Vector turn, gfx::Vector worldSpacePlace)
{
	gfx::Vector result;
	double pie = 3.14159;
	double degrees = azimuth*(pie / 180);
	gfx::Vector v1(cos(degrees), -sin(degrees), 0,turn[0]);
	gfx::Vector v2(sin(degrees), cos(degrees), 0, turn[1]);
	gfx::Vector v3(0, 0, 1, turn[2]);
	gfx::Vector v4(0, 0, 0, 1);

	degrees = elevation * (pie / 180);

	gfx::Vector v5(cos(degrees), 0, -sin(degrees), 0);
	gfx::Vector v6(0, 1, 0, 0);
	gfx::Vector v7(sin(degrees), 0, cos(degrees), 0);
	gfx::Vector v8(0, 0, 0, 1);
	

	double result1[4];
	result1[0] = v5.Dot(worldSpacePlace);
	result1[1] = v6.Dot(worldSpacePlace);
	result1[2] = v7.Dot(worldSpacePlace);
	result1[3] = v8.Dot(worldSpacePlace);

	gfx::Vector resultPart(result1[0], result1[1], result1[2], result1[3]);

	double result2[4];
	result2[0] = v1.Dot(resultPart);
	result2[1] = v2.Dot(resultPart);
	result2[2] = v3.Dot(resultPart);
	result2[3] = v4.Dot(resultPart);
	gfx::Vector resultPart2(result2[0], result2[1], result2[2], result2[3]);

	result = resultPart2;
	return (result);
}

//Prototypes:
void toWorldSpace(Triangle, gfx::Vector, double, double);
void calcNearFrustrum(Camera);
void calcFarFrustrum(Camera);
void calcFrustPlanes(Camera);
void printTri(Triangle);
int main()
{
	ifstream myfile("test.txt");
	ofstream myfile2("cslg-p1-output-1.txt");
	if (myfile.fail())
	{
		cout << "\n** ERROR - the file cannot be found! One has been created. Please place the data in test.txt";
		ofstream myfile3("test.txt");
		pause();
		return 0;
	}
	else {
		string line;
		vector<output> vec;
		vector<output> determineVector;
		while (getline(myfile, line))
		{

			if (scan(line).description == "NULL")
			{
				for (int i = 0; i < scan(line).data.size(); i++)
				{
					//cout << scan(line).data.at(i) << endl;
				vec.at(vec.size() - 1).data.push_back(scan(line).data.at(i));
				}
			}
			else
				vec.push_back(scan(line));
		}
		gfx::Vector a(0.0, 0.0, 0.0);
		gfx::Vector c(12, -7, -2);
		gfx::Vector b(-3, -3, -13);
		cout << calcDistance(c, b);
		cout << "vector of size : " << vec.size() << endl;
		for (int i = 0; i < vec.size(); i++)
		{
			cout << vec.at(i).description << endl;
			for (int j = 0; j < vec.at(i).data.size(); j++)
			{
				cout << vec.at(i).data.at(j) << endl;
			}
		}
		pause();

	}
	//input should be: Model Space, World Space, and ID of cam
	int clip_cam = 9; //cam to clip against

	Camera main_cam;
	//main_cam.eye = gfx::Vector(0, 0, 5);

	Triangle test;
	test.pts[0] = gfx::Vector(-11.8014, -0.6047, -7.7693);
	test.pts[1] = gfx::Vector(1.9975, 13.3936, -4.0767);
	//test.pts[2] = (0,0,0);
	printTri(test);

	// 1. Convert to world space from model space
	//ex) try loc(-70,0,80) with az -90 and elv -30
	toWorldSpace(test, gfx::Vector(-70, 0, 80), -90, -30);

	// 2. Calculate view frustrum (6 panels, 8 points)
	calcNearFrustrum(main_cam); // near frustrum vertices
	calcFarFrustrum(main_cam); // far frustrum vertices
	calcFrustPlanes(main_cam); // frustrum planes

	// 3. Cull/Clip: Check if triangles fall in, out, or partial of frustrum.


	pause();
	return 0;
}

void toWorldSpace(Triangle tri, gfx::Vector vec, double azm, double elev) {
	
}

void calcNearFrustrum(Camera cam) {
	// D U R
	gfx::Vector temp_D(0, 0, 1);
	gfx::Vector temp_U(0, 1, 0);
	gfx::Vector temp_R(1, 0, 0);
	cout << "Near Frustrums:\n";

	// Vbl = E + dmin*D + umin*U + rmin*R
	cam.frust_Vbl = (cam.eye + cam.min_sub_pt*temp_D - (cam.v_height / 2)*temp_U - (cam.v_width / 2)*temp_R);
	cout << "Vbl: " << cam.frust_Vbl << endl;

	// Vtl = E + dmin*D + umax*U + rmin*R
	cam.frust_Vtl = (cam.eye + cam.min_sub_pt*temp_D + (cam.v_height / 2)*temp_U - (cam.v_width / 2)*temp_R);
	cout << "Vtl: " << cam.frust_Vtl << endl;

	// Vbr = E + dmin*D + umin*U + rmax*R
	cam.frust_Vbr = (cam.eye + cam.min_sub_pt*temp_D - (cam.v_height / 2)*temp_U + (cam.v_width / 2)*temp_R);
	cout << "Vbr: " << cam.frust_Vbr << endl;

	// Vtr = E + dmin*D + umax*U + rmax*R
	cam.frust_Vtr = (cam.eye + cam.min_sub_pt*temp_D + (cam.v_height / 2)*temp_U + (cam.v_width / 2)*temp_R);
	cout << "Vtr: " << cam.frust_Vtr << endl;

	cout << endl;
}


bool isZero(gfx::Vector a)
{
	return ((a[0] == 0) && (a[1] == 0) && (a[2] == 0));
}
double calcMagnitude(gfx::Vector a)
{
	return sqrt(pow(a[0],2)+pow(a[1],2)+pow(a[2],2));
}
double calcDistance(gfx::Vector b, gfx::Vector a)
{
	double result=0.0;
	double dotProduct=0.0;

	dotProduct=a.Dot(b);
	double top = dotProduct;
	double bot = calcMagnitude(a)*calcMagnitude(b);

	result=acos(top / bot)*(180/3.14);

	return result;

}
gfx::Vector azimuthchange(double degree, gfx::Vector a, gfx::Vector b)
{
	gfx::Vector result(a[0],a[1],a[2]);
	double botTop= cos(degree);
	double length = a[2];
	//cos x/r
	cos(degree);
	

	return result;
	

	

}

void calcFarFrustrum(Camera cam) {
	// D U R
	gfx::Vector temp_D(0, 0, 1);
	gfx::Vector temp_U(0, 1, 0);
	gfx::Vector temp_R(1, 0, 0);
	double temp_dist = cam.d_max / cam.min_sub_pt;
	cout << "Far Frustrums:\n";

	// Wbl = E + dmax/dmin(dmin*D + umin*U + rmin*R)
	cam.frust_Wbl = (cam.eye + temp_dist * (cam.min_sub_pt*temp_D - (cam.v_height / 2)*temp_U - (cam.v_width / 2)*temp_R));
	cout << "Wbl: " << cam.frust_Wbl << endl;

	// Wtl = E + dmin*D + umax*U + rmin*R
	cam.frust_Wtl = (cam.eye + temp_dist * (cam.min_sub_pt*temp_D + (cam.v_height / 2)*temp_U - (cam.v_width / 2)*temp_R));
	cout << "Wtl: " << cam.frust_Wtl << endl;

	// Wbr = E + dmin*D + umin*U + rmax*R
	cam.frust_Wbr = (cam.eye + temp_dist * (cam.min_sub_pt*temp_D - (cam.v_height / 2)*temp_U + (cam.v_width / 2)*temp_R));
	cout << "Wbr: " << cam.frust_Wbr << endl;

	// Wtr = E + dmin*D + umax*U + rmax*R
	cam.frust_Wtr = (cam.eye + temp_dist * (cam.min_sub_pt*temp_D + (cam.v_height / 2)*temp_U + (cam.v_width / 2)*temp_R));
	cout << "Wtr: " << cam.frust_Wtr << endl;

	cout << endl;
}

void calcFrustPlanes(Camera cam) {
	// D U R
	gfx::Vector temp_D(0, 0, 1);
	gfx::Vector temp_U(0, 1, 0);
	gfx::Vector temp_R(1, 0, 0);
	cout << "Frustrum Planes:\n";

	// NearPlane D.E + dmin
	cam.frust_Nn = cam.frust_Nn.Dot(temp_D, cam.eye) + cam.min_sub_pt;
	cout << "Nn: " << cam.frust_Nn << endl;

	// FarPlane
	cam.frust_Nf = -(cam.frust_Nf.Dot(temp_D, cam.eye) + cam.min_sub_pt);
	cout << "Nf: " << cam.frust_Nf << endl;

	// LeftPlane
	cam.frust_Nl = (cam.min_sub_pt*temp_R - (-cam.v_width / 2) *temp_D) * (1 / sqrt(cam.min_sub_pt*cam.min_sub_pt + (cam.v_width / 2)*(cam.v_width / 2)));
	//cam.frust_Nl = (cam.v_height)*(cam.min_sub_pt*temp_R - cam.v_width*temp_D);
	cout << "Nl: " << cam.frust_Nl << endl;

	// RightPlane
	cam.frust_Nr = (-cam.min_sub_pt*temp_R + (cam.v_width / 2) *temp_D) * (1 / sqrt(cam.min_sub_pt*cam.min_sub_pt + (-cam.v_width / 2)*(-cam.v_width / 2)));
	cout << "Nr: " << cam.frust_Nr << endl;
	// BotPlane
	cam.frust_Nb = (cam.min_sub_pt*temp_U - (-cam.v_width / 2) *temp_D) * (1 / sqrt(-cam.min_sub_pt*-cam.min_sub_pt + (-cam.v_width / 2)*(-cam.v_width / 2)));
	cout << "Nb: " << cam.frust_Nb << endl;
	// TopPlane
	cam.frust_Nt = (-cam.min_sub_pt*temp_U + (cam.v_width / 2) *temp_D) * (1 / sqrt(-cam.min_sub_pt*-cam.min_sub_pt + (cam.v_width / 2)*(cam.v_width / 2)));
	cout << "Nt: " << cam.frust_Nt << endl;

	cout << endl;
}

void printTri(Triangle tri) {
	cout << "Triangle " << tri.id << endl;
	for (int i = 0; i < 3; i++) {
		cout << "point " << i << ": " << tri.pts[i] << endl;
	}
}



void pause() {//simple pausing function for readability
	string junk;
	cout << "Press Enter to continue...";
	getline(cin, junk);
}
output scan(string line)
{
	output result;
	result.description = "NULL";
	vector<double> info;

	//we know it's not a comment
	//line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
	//Does this describe the camera?
	for (int i = 0; i < line.length(); i++)
	{
		if (line.at(i) == 'c' && (i + 6) < line.length()) {
			if (line.at(i + 1) == 'a')
				if (line.at(i + 2) == 'm')
					if (line.at(i + 3) == 'e')
						if (line.at(i + 4) == 'r')
							if (line.at(i + 5) == 'a')
							{
								//line is about camera
								i += 5;
								result.description = "Camera space";
								//now we seek 3 numbers associated with camera
								info = returnNumbers(i, line);
								result.data = info;
							}
		}
		else if (line.at(i) == 't' && (i + 2) < line.length()) {
			if (line.at(i + 1) == 'r')
				if (line.at(i + 2) == 'i')
				{
					result.description = "Tri";
				}
			//The first part takes in the keyword camera, then adds all numbers to that specific vector, then places them within an output value.

		}
		else if (line.at(i) == 'b' && (i + 2) < line.length()) {
			if (line.at(i + 1) == 'a')
				if (line.at(i + 2) == 'g')
				{
					result.description = "Bag";
					info = returnNumbers(i, line);
					result.data = info;
				}
		}
		else if (line.at(i) == 'l' && (i + 2) < line.length()) {
			if (line.at(i + 1) == 'o')
				if (line.at(i + 2) == 'c')
				{
					bool twoDigit = (returnDigit(line.at(i + 5)) >= 0);
					string firstNum(1, line.at(i + 4));
					string secondNum(1, line.at(i + 5));
					if (twoDigit)
					{
						result.description = "kid-loc " + firstNum + secondNum;
						info = returnNumbers(i + 6, line);
						result.data = info;

					}
					else {
						result.description = "kid-loc " + firstNum;
						info = returnNumbers(i + 5, line);
						result.data = info;
					}
				}
		}

	}
	if (result.description == "NULL")
	{
		info = returnNumbers(2, line);
		result.data = info;
	}
	return result;

}

vector<double> returnNumbers(int start, string line)
{
	vector<double> returnVal;
	//state1, not number
	bool state1 = false;
	//state2, number
	bool state2 = false;
	//state3, decimal
	bool state3 = false;
	double currentNum = 0.0;
	double decimalAdd = 0.0;
	bool addZero = false;
	bool negativeNumber = false;
	bool zeroFound = false;
	for (int i = start; i < line.length(); i++)
	{
		if (returnDigit(line.at(i)) == 0 && state1)
		{
			zeroFound = true;
		}
		if (returnDigit(line.at(i)) == -2 && zeroFound == true)
		{
			zeroFound = false;
		}
		else if (addZero)
		{
			returnVal.push_back(0.0);
			state1 = true;
			state2 = false;
			state3 = false;
			addZero = false;
		}
		else if (returnDigit(line.at(i)) == -3)
		{
			state1 = true;
			state2 = false;
			state3 = false;
			negativeNumber = true;

		}
		else if (currentNum != 0.0 && returnDigit(line.at(i)) == -1) //we have a full number
		{
			if (negativeNumber) {
				currentNum = currentNum * -1;
				negativeNumber = false;
			}
			returnVal.push_back(currentNum);
			state1 = true;
			state2 = false;
			state3 = false;
			currentNum = 0.0;
			decimalAdd = 0.0;
		}
		else if (returnDigit(line.at(i)) == -1)
		{
			if (zeroFound)
			{
				returnVal.push_back(0);
				zeroFound = false;
			}
			state1 = true;
			state2 = false;
			state3 = false;
		}
		else if (returnDigit(line.at(i)) == -2 && state2 == true)
		{
			state1 = false;
			state2 = false;
			state3 = true;
		}
		else
		{
			state1 = false;
			state2 = true;
			state3 = false;
		}
		//for state 1, just wait.
		//for state 2, we have a number and must now create it.
		if (state2)
		{
			while (returnDigit(line.at(i)) >= 0)
			{
				currentNum = currentNum * 10;
				currentNum += returnDigit(line.at(i));
				i++;
			}
			i--;
		}
		if (state3)
		{
			i += 1;
			int countDigits = 0;
			while (returnDigit(line.at(i)) >= 0) {
				decimalAdd = decimalAdd * 10;
				decimalAdd += returnDigit(line.at(i));
				i++;
				countDigits++;
			}

			double divisor = pow(10, countDigits);
			decimalAdd = decimalAdd / divisor;
			currentNum = currentNum + decimalAdd;
			if (currentNum == 0.0)
				addZero = true;
			i--;
		}

	}
	return returnVal;
}

int returnDigit(char digit)
{
	int returnVal = -1;
	if (digit == '0' || digit == '1' || digit == '2' || digit == '3' || digit == '4' || digit == '5' || digit == '6' || digit == '7' || digit == '8' || digit == '9')
	{
		returnVal = (int)digit - 48;
	}
	else if (digit == '.')
	{
		returnVal = -2;
	}
	else if (digit == '-')
	{
		returnVal = -3;
	}
	return returnVal;
}