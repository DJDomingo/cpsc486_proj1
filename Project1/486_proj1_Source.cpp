#include <iostream>
#include <string>
#include "math.h"
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

//Prototypes:
void calcNearFrustrum(Camera);
void calcFarFrustrum(Camera);
void calcFrustPlanes(Camera);
void printTri(Triangle);
int main()
{
	//input should be: Model Space, World Space, and ID of cam
	int clip_cam = 9; //cam to clip against
	int meh; //just used to pause before exiting

	Camera main_cam;
	//main_cam.eye = gfx::Vector(0, 0, 5);

	Triangle test;
	test.pts[0] = gfx::Vector(-1, -1, 0);
	test.pts[1] = gfx::Vector(1, -1, 0);
	printTri(test);


	//view frustrum (6 panels, 8 points)
	calcNearFrustrum(main_cam);
	cout << endl;
	calcFarFrustrum(main_cam);
	cout << endl;
	calcFrustPlanes(main_cam);

	cin >> meh;
	return 0;
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
}

void calcFarFrustrum(Camera cam) {
	// D U R
	gfx::Vector temp_D(0, 0, 1);
	gfx::Vector temp_U(0, 1, 0);
	gfx::Vector temp_R(1, 0, 0);
	double temp_dist = cam.d_max / cam.min_sub_pt;
	cout << "Far Frustrums:\n";

	// Wbl = E + dmax/dmin(dmin*D + umin*U + rmin*R)
	cam.frust_Wbl = (cam.eye + temp_dist*(cam.min_sub_pt*temp_D - (cam.v_height / 2)*temp_U - (cam.v_width / 2)*temp_R));
	cout << "Wbl: " << cam.frust_Wbl << endl;

	// Wtl = E + dmin*D + umax*U + rmin*R
	cam.frust_Wtl = (cam.eye + temp_dist*(cam.min_sub_pt*temp_D + (cam.v_height / 2)*temp_U - (cam.v_width / 2)*temp_R));
	cout << "Wtl: " << cam.frust_Wtl << endl;

	// Wbr = E + dmin*D + umin*U + rmax*R
	cam.frust_Wbr = (cam.eye + temp_dist*(cam.min_sub_pt*temp_D - (cam.v_height / 2)*temp_U + (cam.v_width / 2)*temp_R));
	cout << "Wbr: " << cam.frust_Wbr << endl;

	// Wtr = E + dmin*D + umax*U + rmax*R
	cam.frust_Wtr = (cam.eye + temp_dist*(cam.min_sub_pt*temp_D + (cam.v_height / 2)*temp_U + (cam.v_width / 2)*temp_R));
	cout << "Wtr: " << cam.frust_Wtr << endl;
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
	cam.frust_Nl = (cam.min_sub_pt*temp_R - (-cam.v_width/2) *temp_D) * (1/sqrt(cam.min_sub_pt*cam.min_sub_pt + (cam.v_width / 2)*(cam.v_width / 2)));
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
}

void printTri(Triangle tri) {
	cout << "Triangle " << tri.id << endl;
	for (int i = 0; i < 3; i++) {
		cout << "point " << i << ": " << tri.pts[i] << endl;
	}
}
