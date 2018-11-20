#include <iostream>
#include <string>
#include <math.h>
using namespace std;


struct Camera {
	float eye[3] = { 0,0,5 };
	int id = 9;
	float d_max = 100;
	float min_sub_pt = 10;
	//float min_sub_pt[3] = { 0,0,0 };
	float v_width = 5;
	float v_height = 5;
	//Frustrum Vertices that are Near
	float frust_Vbl[3] = { 0,0,0 };
	float frust_Vtl[3] = { 0,0,0 };
	float frust_Vbr[3] = { 0,0,0 };
	float frust_Vtr[3] = { 0,0,0 };
	//Frustrum Vertices that are Far
	float frust_Wbl[3] = { 0,0,0 };
	float frust_Wtl[3] = { 0,0,0 };
	float frust_Wbr[3] = { 0,0,0 };
	float frust_Wtr[3] = { 0,0,0 };
	//Frustrum Plane Normals
	float frust_Nn[3] = { 0,0,0 };
	float frust_Nf[3] = { 0,0,0 };
	float frust_Nl[3] = { 0,0,0 };
	float frust_Nr[3] = { 0,0,0 };
	float frust_Nb[3] = { 0,0,0 };
	float frust_Nt[3] = { 0,0,0 };
};

void calcNearFrustrum(Camera);
void calcFarFrustrum(Camera);
void calcFrustPlanes(Camera);
void vectMult(float[], float);
void dotProd(float[], float[], float[]);
void vectAdd(float[], float[]);
void vectAdd(float[], float);
void showVect(float[]);
void negVect(float[]);
int main()
{
	//input should be: Model Space, World Space, and ID of cam
	int clip_cam = 9; //cam to clip against
	int meh; //just used to pause before exiting
	
	Camera main_cam;
	


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
	//near plane vertices
	float temp_D[3] = { 0, 0, 1 };
	float temp_U[3] = { 0, 1, 0 };
	float temp_R[3] = { 1, 0, 0 };

	// Vbl = E + dmin*D + umin*U + rmin*R
	vectMult(temp_D, cam.min_sub_pt); //dmin
	vectMult(temp_U, -cam.v_height / 2); //umin
	vectMult(temp_R, -cam.v_width / 2); //rmin
	
	vectAdd(cam.frust_Vbl, cam.eye);
	vectAdd(cam.frust_Vbl, temp_D);
	vectAdd(cam.frust_Vbl, temp_U);
	vectAdd(cam.frust_Vbl, temp_R);

	showVect(cam.frust_Vbl);

	// Vtl = E + dmin*D + umax*U + rmin*R
	temp_U[0] = 0; temp_U[1] = 1; temp_U[2] = 0; //reset needed temps
	vectMult(temp_U, cam.v_height / 2); //umax

	vectAdd(cam.frust_Vtl, cam.eye);
	vectAdd(cam.frust_Vtl, temp_D);
	vectAdd(cam.frust_Vtl, temp_U);
	vectAdd(cam.frust_Vtl, temp_R);

	showVect(cam.frust_Vtl);

	// Vbr = E + dmin*D + umin*U + rmax*R
	temp_U[0] = 0; temp_U[1] = 1; temp_U[2] = 0; //reset needed temps
	temp_R[0] = 1; temp_R[1] = 0; temp_R[2] = 0; //reset needed temps
	vectMult(temp_U, -cam.v_height / 2); //umin
	vectMult(temp_R, cam.v_width / 2); //rmax

	vectAdd(cam.frust_Vbr, cam.eye);
	vectAdd(cam.frust_Vbr, temp_D);
	vectAdd(cam.frust_Vbr, temp_U);
	vectAdd(cam.frust_Vbr, temp_R);

	showVect(cam.frust_Vbr);

	// Vtr = E + dmin*D + umax*U + rmax*R
	temp_U[0] = 0; temp_U[1] = 1; temp_U[2] = 0; //reset needed temps
	vectMult(temp_U, cam.v_height / 2); //umax

	vectAdd(cam.frust_Vtr, cam.eye);
	vectAdd(cam.frust_Vtr, temp_D);
	vectAdd(cam.frust_Vtr, temp_U);
	vectAdd(cam.frust_Vtr, temp_R);

	showVect(cam.frust_Vtr);
}

void calcFarFrustrum(Camera cam) {
	//far plane vertices
	float temp_D[3] = { 0, 0, 1 };
	float temp_U[3] = { 0, 1, 0 };
	float temp_R[3] = { 1, 0, 0 };
	float temp_dist = cam.d_max / cam.min_sub_pt;

	// Wbl = E + dmax/dmin(dmin*D + umin*U + rmin*R)
	vectMult(temp_D, cam.min_sub_pt); //dmin
	vectMult(temp_U, -cam.v_height / 2); //umin
	vectMult(temp_R, -cam.v_width / 2); //rmin

	vectAdd(cam.frust_Wbl, temp_D);
	vectAdd(cam.frust_Wbl, temp_U);
	vectAdd(cam.frust_Wbl, temp_R);
	vectMult(cam.frust_Wbl, temp_dist);
	vectAdd(cam.frust_Wbl, cam.eye);

	showVect(cam.frust_Wbl);

	// Wtl = E + dmin*D + umax*U + rmin*R
	temp_U[0] = 0; temp_U[1] = 1; temp_U[2] = 0; //reset needed temps
	vectMult(temp_U, cam.v_height / 2); //umax

	vectAdd(cam.frust_Wtl, temp_D);
	vectAdd(cam.frust_Wtl, temp_U);
	vectAdd(cam.frust_Wtl, temp_R);
	vectMult(cam.frust_Wtl, temp_dist);
	vectAdd(cam.frust_Wtl, cam.eye);

	showVect(cam.frust_Wtl);

	// Wbr = E + dmin*D + umin*U + rmax*R
	temp_U[0] = 0; temp_U[1] = 1; temp_U[2] = 0; //reset needed temps
	temp_R[0] = 1; temp_R[1] = 0; temp_R[2] = 0; //reset needed temps
	vectMult(temp_U, -cam.v_height / 2); //umin
	vectMult(temp_R, cam.v_height / 2); //rmax

	vectAdd(cam.frust_Wbr, temp_D);
	vectAdd(cam.frust_Wbr, temp_U);
	vectAdd(cam.frust_Wbr, temp_R);
	vectMult(cam.frust_Wbr, temp_dist);
	vectAdd(cam.frust_Wbr, cam.eye);

	showVect(cam.frust_Wbr);

	// Wtr = E + dmin*D + umax*U + rmax*R
	temp_U[0] = 0; temp_U[1] = 1; temp_U[2] = 0; //reset needed temps
	vectMult(temp_U, cam.v_height / 2); //umax

	vectAdd(cam.frust_Wtr, temp_D);
	vectAdd(cam.frust_Wtr, temp_U);
	vectAdd(cam.frust_Wtr, temp_R);
	vectMult(cam.frust_Wtr, temp_dist);
	vectAdd(cam.frust_Wtr, cam.eye);

	showVect(cam.frust_Wtr);
}

void calcFrustPlanes(Camera cam) {
	// normals
	float temp_D[3] = { 0, 0, 1 };
	float temp_U[3] = { 0, 1, 0 };
	float temp_R[3] = { 1, 0, 0 };

	// NearPlane
	dotProd(cam.frust_Nn, temp_D, cam.eye);
	vectAdd(cam.frust_Nn, cam.min_sub_pt);
	showVect(cam.frust_Nn);

	// FarPlane
	dotProd(cam.frust_Nf, temp_D, cam.eye);
	vectAdd(cam.frust_Nf, cam.d_max);
	negVect(cam.frust_Nf);
	showVect(cam.frust_Nf);

	// LeftPlane
	// RightPlane
	// BotPlane
	// TopPlane
}

void vectMult(float vect[], float val) {
	for (int i = 0; i < 3; i++) {
		vect[i] *= val;
	}
}

void dotProd(float ans[], float vect[], float vect2[]) {
	for (int i = 0; i < 3; i++) {
		ans[i] = vect[i] * vect2[i];
	}
}

void vectAdd(float vect[], float val[]) {
	for (int i = 0; i < 3; i++) {
		vect[i] += val[i];
	}
}

void vectAdd(float vect[], float val) {
	for (int i = 0; i < 3; i++) {
		vect[i] += val;
	}
}

void showVect(float vect[]) {
	for (int i = 0; i < 3; i++) {
		cout << vect[i] << ", ";
	}
	cout << endl;
}

void negVect(float vect[]) {
	for (int i = 0; i < 3; i++) {
		vect[i] = -vect[i];
	}
}