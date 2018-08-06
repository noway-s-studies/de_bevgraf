#include <GL/glut.h>
#include <Windows.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define _TS_OPENGL_LIB <GL/gl.h>
#include "ts_libgraph.h"
#include "ts_matrix.h"

//Ablak adatai
WND Window;

_Matrix* M;
_Matrix* G;
Point p[6];

int Vect[] = {
	3, -3, 3, -3, 3, -3
};

double sqr(double X) {
	return X*X;
}

_Matrix* FillMatrix_2t() {
	float vals[] = {
		-1, 3, -3, 1,
		3, -6, 0, 4, 
		-3, 3, 3, 1,
		1, 0, 0, 0
	};

	for (int i = 0; i < 16; i++) {
		vals[i] *= (1.0 / 6.0);
	}

	return CreateMatrixByValues(4, 4, vals);
}

void init() {
	//glEnable(GL_LINE_STIPPLE);
	glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	glPointSize(8.0);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, Window.Width, 0.0, Window.Height);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
}

void DrawCurve(Point p1, Point p2, Point p3, Point p4) {
	float x = 0;
	int i;

	//G mátrix
	float vals[] = {
		p1.X, p2.X, p3.X, p4.X,
		p1.Y, p2.Y, p3.Y, p4.Y,
		0,		0,		0,		0,
		1,		1,		1,		1
	};
	_Matrix* G = CreateMatrixByValues(4, 4, vals);

	//G*M mátrix
	_Matrix* GM = MultiplyMatrix(G, M);

	//Görbe rajzolása
	glBegin(GL_LINE_STRIP);

	for (i = 0; i < 300 && x<=1; i++) {
		//T mátrix
		_Matrix* T = CreateMatrix(4, 1);
		T->Values[0][0] = x*x*x;
		T->Values[1][0] = x*x;
		T->Values[2][0] = x;
		T->Values[3][0] = 1;

		_Matrix* GMT = MultiplyMatrix(GM, T);

		x += 0.01;

		glVertex2f(GMT->Values[0][0], GMT->Values[1][0]);

		FreeMatrix(GMT);
		FreeMatrix(T);
	}
	glEnd();

	FreeMatrix(G);
	FreeMatrix(GM);
}

int GetNextPoint(int Index, int Sides) {
	if (Index > Sides)
		return Index-Sides+1;
	else
		return Index+1;
}

void OnDraw() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(0.0, 0.5, 0.0);
	glLineWidth(1.0);

	int i;

	//Pontok rajzolása
	glBegin(GL_POINTS);
	for (i = 0; i < 6; i++) {
		glVertex2f(p[i].X, p[i].Y);
	}
	glEnd();

	DrawCurve(p[5], p[0], p[1], p[2]);
	DrawCurve(p[0], p[1], p[2], p[3]);
	DrawCurve(p[1], p[2], p[3], p[4]);
	DrawCurve(p[2], p[3], p[4], p[5]);
	DrawCurve(p[3], p[4], p[5], p[0]);
	DrawCurve(p[4], p[5], p[0], p[1]);

	glutSwapBuffers();
}

void OnRefresh(int Value) {
	for (int i = 0; i < 6; i++) {
		if (p[i].Y > Window.Height || p[i].Y < 0) Vect[i] *= -1;
		p[i].Y += Vect[i];
	}

	glutPostRedisplay();
	glutTimerFunc(10, OnRefresh, 0);
}

int main(int argc, char** argv) {
	Window.Height = 400;
	Window.Width = 600;

	p[0] = GetPoint(150, 175);
	p[1] = GetPoint(250, 250);
	p[2] = GetPoint(300, 250);
	p[3] = GetPoint(350, 175);
	p[4] = GetPoint(275, 100);
	p[5] = GetPoint(200, 100);

	M = FillMatrix_2t();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(Window.Width, Window.Height);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("Copyright (C) 2013: TomiSoft");
	init();

	glutDisplayFunc(OnDraw);
	glutTimerFunc(0, OnRefresh, 0);

	glutMainLoop();
	
	FreeMatrix(M);
	FreeMatrix(G);
	return 0;
}