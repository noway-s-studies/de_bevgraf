#define _USE_MATH_DEFINES

#include "ts_libgraph.h"
#include "ts_matrix.h"
#include <math.h>
#include <stdio.h>

int LeftCutter(_Polygon Poly, Point* NewPoints, int LineX);
int RightCutter(_Polygon Poly, Point* NewPoints, int LineX);
int TopCutter(_Polygon Poly, Point* NewPoints, int LineY);
int BottomCutter(_Polygon Poly, Point* NewPoints, int LineY);

Point GetPoint(int X, int Y) {
	Point R;
	R.X = X;
	R.Y = Y;

	return R;
}

Point3D GetPoint3D(float X, float Y, float Z) {
	Point3D R;
	R.X = X;
	R.Y = Y;
	R.Z = Z;
	R.W = 1;

	return R;
}

_Polygon CreatePolygon(int ScreenX, int ScreenY, int Radius, int Sides) {
	int i;

	_Polygon Result;
	Result.Points = (Point*) malloc(sizeof(Point) * Sides);
	Result.Sides = Sides;

	float Rad_Interval = (M_PI * 2) / Sides;

	for (i = 0; i < Sides; i++) {
		Result.Points[i].X = ScreenX + Radius * cos(i * Rad_Interval);
		Result.Points[i].Y = ScreenY + Radius * sin(i * Rad_Interval);
	}

	return Result;
}

void ScalePolygon(_Polygon P, double ScaleBy) {
	_Matrix* A = CreateMatrix(3, 3);
	A->Values[0][0] = ScaleBy;
	A->Values[1][1] = ScaleBy;
	A->Values[2][2] = 1;

	int Index;
	for (Index = 0; Index < P.Sides; Index++) {
		//eltolás
		Point Center = GetCenteroid(P);

		_Matrix* C = CreateMatrix(3, 3);
		C->Values[0][0] = 1;
		C->Values[1][1] = 1;
		C->Values[2][2] = 1;
		C->Values[0][2] = -Center.X;
		C->Values[1][2] = -Center.Y;

		_Matrix* B = CreateMatrix(3, 1);
		B->Values[0][0] = P.Points[Index].X;
		B->Values[1][0] = P.Points[Index].Y;
		B->Values[2][0] = 1;

		_Matrix* Result = MultiplyMatrix(C, B);

		float X = Result->Values[0][0];
		float Y = Result->Values[1][0];

		free(C);
		free(B);
		free(Result);

		//"Skalarizálás"

		B = CreateMatrix(3, 1);
		B->Values[0][0] = X;
		B->Values[1][0] = Y;
		B->Values[2][0] = 1;

		Result = MultiplyMatrix(A, B);

		X = Result->Values[0][0];
		Y = Result->Values[1][0];
		
		free(B);
		free(Result);

		//Visszatolás

		C = CreateMatrix(3, 3);
		C->Values[0][0] = 1;
		C->Values[1][1] = 1;
		C->Values[2][2] = 1;
		C->Values[0][2] = Center.X;
		C->Values[1][2] = Center.Y;

		B = CreateMatrix(3, 1);
		B->Values[0][0] = X;
		B->Values[1][0] = Y;
		B->Values[2][0] = 1;

		Result = MultiplyMatrix(C, B);
		
		P.Points[Index].X = Result->Values[0][0];
		P.Points[Index].Y = Result->Values[1][0];

		free(C);
		free(B);
		free(Result);
	}

	free(A);
}

void RotatePolygon(_Polygon P, Point Pnt, double Angle) {
	int Index;

	for (Index = 0; Index < P.Sides; Index++) {
		//Az egyszerû, ésszerû megoldás:
		//float X = (P.Points[Index].X - Pnt.X);
		//float Y = (P.Points[Index].Y - Pnt.Y);

		//P.Points[Index].X = Pnt.X + (X * cos(Angle) - Y * sin(Angle));
		//P.Points[Index].Y = Pnt.Y + (X * sin(Angle) + Y * cos(Angle));

		//Ahogy kérték:

		//Eltolás az origo-ba
		_Matrix* A = CreateMatrix(3, 3);
		A->Values[0][0] = 1;
		A->Values[1][1] = 1;
		A->Values[2][2] = 1;
		A->Values[0][2] = -Pnt.X;
		A->Values[1][2] = -Pnt.Y;

		_Matrix* B = CreateMatrix(3, 1);
		B->Values[0][0] = P.Points[Index].X;
		B->Values[1][0] = P.Points[Index].Y;
		B->Values[2][0] = 1;

		_Matrix* Result = MultiplyMatrix(A, B);

		float X = Result->Values[0][0];
		float Y = Result->Values[1][0];

		free(A);
		free(B);
		free(Result);
		
		//Forgatás
		A = CreateMatrix(3, 3);
		B = CreateMatrix(3, 1);

		A->Values[0][0] = cos(Angle);
		A->Values[0][1] = -sin(Angle);
		A->Values[1][0] = sin(Angle);
		A->Values[1][1] = cos(Angle);
		A->Values[2][2] = 1;

		B->Values[0][0] = X;
		B->Values[1][0] = Y;
		B->Values[2][0] = 1;

		Result = MultiplyMatrix(A, B);
		X = Result->Values[0][0];
		Y = Result->Values[1][0];

		free(Result);
		free(A);
		free(B);

		//Visszatolás
		A = CreateMatrix(3, 3);
		A->Values[0][0] = 1;
		A->Values[1][1] = 1;
		A->Values[2][2] = 1;
		A->Values[0][2] = Pnt.X;
		A->Values[1][2] = Pnt.Y;

		B = CreateMatrix(3, 1);
		B->Values[0][0] = X;
		B->Values[1][0] = Y;
		B->Values[2][0] = 1;

		Result = MultiplyMatrix(A, B);

		P.Points[Index].X = Result->Values[0][0];
		P.Points[Index].Y = Result->Values[1][0];

		free(A);
		free(B);
		free(Result);
	}
}

_Polygon CreateStar(int ScreenX, int ScreenY, int Radius, int Sides) {
	_Polygon Inner = CreatePolygon(ScreenX, ScreenY, Radius / 2, Sides);
	_Polygon Outer = CreatePolygon(ScreenX, ScreenY, Radius, Sides);
	RotatePolygon(Outer, GetCenteroid(Outer), (M_PI / Sides));

	_Polygon Result;
	Result.Sides = Sides * 2;
	Result.Points = (Point*) malloc(sizeof(Point) * Sides * 2);

	int Index, ResultIndex = 0;

	for (Index = 0, ResultIndex = 0; Index < Sides; Index++, ResultIndex++) {
		Result.Points[ResultIndex] = Inner.Points[Index];
		ResultIndex++;
		Result.Points[ResultIndex] = Outer.Points[Index];
	}

	return Result;
}

Point GetCenteroid(_Polygon Pg) {
	int Current;
	int SumX = 0;
	int SumY = 0;

	for (Current = 0; Current < Pg.Sides; Current++) {
		SumX += Pg.Points[Current].X;
		SumY += Pg.Points[Current].Y;
	}

	Point Result;
	Result.X = (float)SumX / Pg.Sides;
	Result.Y = (float)SumY / Pg.Sides;

	return Result;
}

int GetPreviousPoint(_Polygon Poly, int Index) {
	return ((Index - 1 < 0) ? Poly.Sides : Index) - 1;
}

int GetNextPoint(_Polygon Poly, int Index) {
	return (Index + 1 == Poly.Sides) ? 0 : Index + 1;
}

BOOL GetNearbyPoints(_Polygon Poly, Point* Pnt, int* PrevIndex, int* NextIndex) {
	int PointIndex = -1;
	int Counter;

	for (Counter = 0; Counter < Poly.Sides; Counter++) {
		if (&Poly.Points[Counter] == Pnt) {
			PointIndex = Counter;
		}
	}

	if (PointIndex == -1) {
		return FALSE;
	}

	*PrevIndex = GetPreviousPoint(Poly, PointIndex);
	*NextIndex = GetNextPoint(Poly, PointIndex);

	return TRUE;
}

BOOL MouseAtPoint(Point P, int MouseX, int MouseY, WND Window) {
	if (P.Y - 5 < Window.Height - MouseY && P.Y + 5 > Window.Height - MouseY) {
		if (P.X - 5 < MouseX && P.X + 5 > MouseX) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL PointIsInRect(_Rect Rect, Point Pnt) {
	if (Pnt.X > Rect.A.X && Pnt.X < Rect.B.X) {
		if (Pnt.Y > Rect.A.Y && Pnt.Y < Rect.B.Y) {
			return TRUE;
		}
	}

	return FALSE;
}

int GetDistance(Point Pnt, Point Line1, Point Line2) {
	int a = Line1.Y - Line2.Y;
	int b = Line2.X - Line1.X;
	int c = (Line2.Y - Line1.Y) * Line1.X - (Line2.X - Line1.X) * Line1.Y;

	int d = (a*Pnt.X + b*Pnt.Y + c) / (a + b);

	return d;
}

BOOL IsPointOnLine(Point Pnt, Point Line1, Point Line2) {
	return GetDistance(Pnt, Line1, Line2) == 0 ? TRUE : FALSE;
}

BOOL IsNegative(int Num) {
	return (Num < 0) ? TRUE : FALSE;
}

BOOL IsSideChanged(Point Pnt, Point NextPos, Point Line1, Point Line2) {
	int Distance1 = GetDistance(Pnt, Line1, Line2);
	int Distance2 = GetDistance(NextPos, Line1, Line2);

	return (IsNegative(Distance1) == IsNegative(Distance2)) ? FALSE : TRUE;
}

_Polygon CutPolygon(_Polygon Pg, _Rect Rect) {
	Point* tempPoints = (Point*) malloc(sizeof(Point) * Pg.Sides * 2);
	Point* NewPointArr = (Point*) malloc(sizeof(Point) * Pg.Sides * 2);
	
	_Polygon temp;
	//Bal vágás
	int NewPoints = LeftCutter(Pg, tempPoints, (Rect.A.X < Rect.B.X) ? Rect.A.X : Rect.B.X);
	temp.Points = tempPoints;
	temp.Sides = NewPoints;
	
	//Jobb vágás
	NewPoints = RightCutter(temp, NewPointArr, (Rect.A.X < Rect.B.X) ? Rect.B.X : Rect.A.X);
	temp.Points = NewPointArr;
	temp.Sides = NewPoints;
	
	//Felsõ vágás
	NewPoints = TopCutter(temp, tempPoints, (Rect.A.Y < Rect.B.Y) ? Rect.B.Y : Rect.A.Y);
	temp.Points = tempPoints;
	temp.Sides = NewPoints;
	
	//Alsó vágás
	NewPoints = BottomCutter(temp, NewPointArr, (Rect.A.Y < Rect.B.Y) ? Rect.A.Y : Rect.B.Y);
	temp.Points = NewPointArr;
	temp.Sides = NewPoints;
	
	return temp;
}

int LeftCutter(_Polygon Poly, Point* NewPoints, int LineX) {
	int i;
	int Index = 0;

	for (i = 0; i < Poly.Sides; i++) {
		int NextCoord = GetNextPoint(Poly, i);

		float Meredekseg = (Poly.Points[i].Y - Poly.Points[NextCoord].Y) / (Poly.Points[i].X - Poly.Points[NextCoord].X);

		//Mindkét pont belül van
		if (Poly.Points[i].X >= LineX && Poly.Points[NextCoord].X >= LineX) {
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
		//Egyik pont belül, másik kívül van
		else if (Poly.Points[i].X >= LineX && Poly.Points[NextCoord].X < LineX) {
			NewPoints[Index].X = LineX;
			NewPoints[Index].Y = Meredekseg * (LineX - Poly.Points[i].X) + Poly.Points[i].Y;

			Index++;
		}
		//Egyik pont kívül, a másik belül van
		else if (Poly.Points[i].X < LineX && Poly.Points[NextCoord].X >= LineX) {
			NewPoints[Index].X = LineX;
			NewPoints[Index].Y = Meredekseg * (LineX - Poly.Points[i].X) + Poly.Points[i].Y;
			Index++;
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
	}

	return Index;
}

int RightCutter(_Polygon Poly, Point* NewPoints, int LineX) {
	int i;
	int Index = 0;

	for (i = 0; i < Poly.Sides; i++) {
		int NextCoord = GetNextPoint(Poly, i);

		float Meredekseg = (Poly.Points[i].Y - Poly.Points[NextCoord].Y) / (Poly.Points[i].X - Poly.Points[NextCoord].X);

		//Mindkét pont belül van
		if (Poly.Points[i].X <= LineX && Poly.Points[NextCoord].X <= LineX) {
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
		//Egyik pont belül, másik kívül van
		else if (Poly.Points[i].X <= LineX && Poly.Points[NextCoord].X > LineX) {
			NewPoints[Index].X = LineX;
			NewPoints[Index].Y = Meredekseg * (LineX - Poly.Points[i].X) + Poly.Points[i].Y;

			Index++;
		}
		//Egyik pont kívül, a másik belül van
		else if (Poly.Points[i].X > LineX && Poly.Points[NextCoord].X <= LineX) {
			NewPoints[Index].X = LineX;
			NewPoints[Index].Y = Meredekseg * (LineX - Poly.Points[i].X) + Poly.Points[i].Y;
			Index++;
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
	}

	return Index;
}

int TopCutter(_Polygon Poly, Point* NewPoints, int LineY) {
	int i;
	int Index = 0;

	for (i = 0; i < Poly.Sides; i++) {
		int NextCoord = GetNextPoint(Poly, i);

		float Meredekseg = (Poly.Points[i].X - Poly.Points[NextCoord].X) / (Poly.Points[i].Y - Poly.Points[NextCoord].Y);

		//Mindkét pont belül van
		if (Poly.Points[i].Y <= LineY && Poly.Points[NextCoord].Y <= LineY) {
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
		//Egyik pont belül, másik kívül van
		else if (Poly.Points[i].Y <= LineY && Poly.Points[NextCoord].Y > LineY) {
			NewPoints[Index].Y = LineY;
			NewPoints[Index].X = Meredekseg * (LineY - Poly.Points[i].Y) + Poly.Points[i].X;

			Index++;
		}
		//Egyik pont kívül, a másik belül van
		else if (Poly.Points[i].Y > LineY && Poly.Points[NextCoord].Y <= LineY) {
			NewPoints[Index].Y = LineY;
			NewPoints[Index].X = Meredekseg * (LineY - Poly.Points[i].Y) + Poly.Points[i].X;
			Index++;
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
	}

	return Index;
}

int BottomCutter(_Polygon Poly, Point* NewPoints, int LineY) {
	int i;
	int Index = 0;

	for (i = 0; i < Poly.Sides; i++) {
		int NextCoord = GetNextPoint(Poly, i);

		float Meredekseg = (Poly.Points[i].X - Poly.Points[NextCoord].X) / (Poly.Points[i].Y - Poly.Points[NextCoord].Y);

		//Mindkét pont belül van
		if (Poly.Points[i].Y >= LineY && Poly.Points[NextCoord].Y >= LineY) {
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
		//Egyik pont belül, másik kívül van
		else if (Poly.Points[i].Y >= LineY && Poly.Points[NextCoord].Y < LineY) {
			NewPoints[Index].Y = LineY;
			NewPoints[Index].X = Meredekseg * (LineY - Poly.Points[i].Y) + Poly.Points[i].X;

			Index++;
		}
		//Egyik pont kívül, a másik belül van
		else if (Poly.Points[i].Y < LineY && Poly.Points[NextCoord].Y >= LineY) {
			NewPoints[Index].Y = LineY;
			NewPoints[Index].X = Meredekseg * (LineY - Poly.Points[i].Y) + Poly.Points[i].X;
			Index++;
			NewPoints[Index] = Poly.Points[NextCoord];
			Index++;
		}
	}

	return Index;
}