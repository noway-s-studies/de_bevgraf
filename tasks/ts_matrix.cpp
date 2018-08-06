#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "ts_matrix.h"

_Matrix* CreateMatrix(int Rows, int Cols) {
	_Matrix* Result = (_Matrix*) malloc((sizeof(_Matrix)));
	Result->Rows = Rows;
	Result->Cols = Cols;

	Result->Values = (float**) malloc(sizeof(float*) * Rows);

	int Index;
	for (Index = 0; Index < Rows; Index++) {
		Result->Values[Index] = (float*) calloc(Cols, sizeof(float));

		/*int Current;
		for (Current = 0; Current < Cols; Current++) {
			Result->Values[Index][Current] = 0;
		}*/
	}

	return Result;
}

void FreeMatrix(_Matrix* M) {
	int i;
	for (i = 0; i < M->Rows; i++) {
		free(M->Values[i]);
	}

	free(M->Values);
	free(M);
}

_Matrix* CreateMatrixByValues(int Rows, int Cols, float* Values) {
	_Matrix* Result = CreateMatrix(Rows, Cols);
	
	int Row, Col, i = 0;
	for (Row = 0; Row < Rows; Row++) {
		for (Col = 0; Col < Cols; Col++) {
			Result->Values[Row][Col] = Values[i];
			i++;
		}
	}

	return Result;
}

_Matrix* MultiplyMatrix(_Matrix* A, _Matrix* B) {
	if (A->Cols != B->Rows) {
		return FALSE;
	}

	_Matrix* result = CreateMatrix(A->Rows, B->Cols);

	int CurrentRow, CurrentCol, Inner;
	for (CurrentRow = 0; CurrentRow < result->Rows; CurrentRow++) {
		for (CurrentCol = 0; CurrentCol < result->Cols; CurrentCol++) {
			for (Inner = 0; Inner < A->Cols; Inner++) {
				result->Values[CurrentRow][CurrentCol] += A->Values[CurrentRow][Inner] * B->Values[Inner][CurrentCol];
			}
		}
	}

	return result;
}
/*
float Determinant(_Matrix* M) {
	float** v = M->Values;

	float determ = 0;

	determ+=(v[0][0])*(v[1][1]*v[2][2]*v[3][3]+v[1][2]*v[2][3]*v[3][1]+
                                v[1][3]*v[2][1]*v[3][2]-v[1][3]*v[2][2]*v[3][1]
                                -v[1][2]*v[2][1]*v[3][3]-v[1][1]*v[2][3]*v[3][2]);
    determ+=(-1)*(v[0][1])*(v[1][0]*v[2][2]*v[3][3]+v[1][2]*v[2][3]*v[3][0]+
                                v[1][3]*v[2][0]*v[3][2]-v[1][3]*v[2][2]*v[3][0]
                                -v[1][2]*v[2][0]*v[3][3]-v[1][0]*v[2][3]*v[3][2]);
    determ+=(v[0][2])*(v[1][0]*v[2][1]*v[3][3]+v[1][1]*v[2][3]*v[3][0]+
                                v[1][3]*v[2][0]*v[3][1]-v[1][3]*v[2][1]*v[3][0]
                                -v[1][1]*v[2][0]*v[3][3]-v[1][0]*v[2][3]*v[3][1]);
    determ+=(-1)*(v[0][3])*(v[1][0]*v[2][1]*v[3][2]+v[1][1]*v[2][2]*v[3][0]+
                                v[1][2]*v[2][0]*v[3][1]-v[1][2]*v[2][1]*v[3][0]
                                -v[1][1]*v[2][0]*v[3][2]-v[1][0]*v[2][2]*v[3][1]);

	return determ;
}
*/

float Determinant4(_Matrix* M, _Matrix* NewMatrix) {
	_Matrix* SmallMatrix = CreateMatrix(3, 3);
	int SourceRow, SourceCol;
	int TempRow, TempCol;
	int NewRow = 0, NewCol = 0;

	float D = 0, Determ = 0;

	for (SourceRow = 0; SourceRow < 4; SourceRow++) {
		for (SourceCol = 0; SourceCol < 4; SourceCol++) {
			//3*3-as részmátrix átmásolása
			for (TempRow = 0; TempRow < 3; TempRow++) {
				for (TempCol = 0; TempCol < 3; TempCol++) {
					if (TempRow < SourceRow)
						NewRow = TempRow;
					else
						NewRow = TempRow + 1;

					if (TempCol < SourceCol)
						NewCol = TempCol;
					else
						NewCol = TempCol + 1;

					SmallMatrix->Values[TempRow][TempCol] = M->Values[NewRow][NewCol];
				}
			}

			//PrintMatrix(SmallMatrix);

			//Determináns meghatározása
			D = Determinant3(SmallMatrix);
			//printf("%.2f\n", D);
			
			if ((SourceRow + SourceCol) % 2 == 0)
				NewMatrix->Values[SourceCol][SourceRow] = D;
			else
				NewMatrix->Values[SourceCol][SourceRow] = D * (-1);
		}
	}

	for (SourceRow = 0; SourceRow < 4; SourceRow++) {
		Determ += NewMatrix->Values[SourceRow][0] * M->Values[0][SourceRow];
	}

	FreeMatrix(SmallMatrix);

	return Determ;
}

float Determinant3(_Matrix* M) {
	return M->Values[0][0]*M->Values[1][1]*M->Values[2][2]+
			   M->Values[0][1]*M->Values[1][2]*M->Values[2][0]+
			   M->Values[0][2]*M->Values[1][0]*M->Values[2][1]-
			   M->Values[0][2]*M->Values[1][1]*M->Values[2][0]-
			   M->Values[0][1]*M->Values[1][0]*M->Values[2][2]-
			   M->Values[0][0]*M->Values[2][1]*M->Values[1][2];
}

_Matrix* InvertMatrix(_Matrix* M) {
	int Row, Col;

	_Matrix* Temp = CreateMatrix(4, 4);
	float Determ = Determinant4(M, Temp);

	//printf("Determ: %.2f\n\n", Determ);

	for (Row = 0; Row < 4; Row++) {
		for (Col = 0; Col < 4; Col++) {
			Temp->Values[Row][Col] = Temp->Values[Row][Col] / Determ;
		}
	}

	return Temp;
}

void PrintMatrix(_Matrix* M) {
	int Rows, Cols;
	for (Rows = 0; Rows < M->Rows; Rows++) {
		for (Cols = 0; Cols < M->Cols; Cols++) {
			printf("%.2f\t", M->Values[Rows][Cols]);
		}
		printf("\n");
	}
	printf("\n");
}