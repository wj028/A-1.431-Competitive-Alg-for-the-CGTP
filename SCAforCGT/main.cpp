#pragma once


#include <fstream>
#include <cmath>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <ctime>


using namespace std;

int N;
int* p;
int ToCo, ToD;

class MyClass
{
public:
	int testCount;
	int NumD;
	int NumP;

	MyClass();
	~MyClass();

	// test on a subset with one test
	bool TestGroup(int* pHead, int size)
	{
		bool isHasOne = false;
		for (int i = 0; i < size; i++)
		{
			if (pHead[i] == 1)
			{
				isHasOne = true;
				break;
			}
		}
		testCount++;
		return isHasOne;
	}

	// test one item with one test
	int TestIndividual(int* pHead, int size, int& firstOneIndex)
	{
		for (int i = 0; i < size; i++)
		{
			testCount++;
			if (pHead[i] == 1)
			{
				firstOneIndex = i + 1;
				NumD++;
				break;
			}
		}
		return firstOneIndex;
	}

	//test each item in a subset
	int TestIndiviAll(int* pHead, int size)
	{
		NumP = 0;
		for (int i = 0; i < size; i++)
		{
			testCount++;
			if (pHead[i] == 1)
			{
				NumD++;
			}
			else
			{
				NumP++;
			}
		}
		return NumP;
	}

	//indentify one item as defective without test
	int IndentifyID(int* pHead, int size)
	{
		return NumD++;
	}
};

MyClass::MyClass()
	: testCount(0)
	, NumD(0)
	, NumP(0)
{
}

MyClass::~MyClass()
{
}


// read data from file
int Read_Data(const char* InputName, const char* OutputName)
{
	ToCo = 0;
	ToD = 0;

	FILE* stream;
	if ((stream = freopen(OutputName, "w", stdout)) == NULL) exit(-1);
	ifstream FIN(InputName);
	int C1 = 0;

	if (FIN)
	{
		int DD = 0;
		FIN >> N >> DD;
		p = new int[N];
		for (int i = 0; i < N; i++)
		{
			FIN >> p[i];
			if (p[i] == 1) { C1++; }
		}
		cout << "N = " << N << " || d = " << C1 << endl;
		cout << "Data already >>>>>> " << endl;
	}
	else
	{
		cerr << "No such file: " << endl;
		throw(-1);
	}
	return 000;
}


// 1994Du
int Get00Ai(int i)
{
	return 3 * pow(2, i);
}


// 2005S: original Ai
int Get0Ai(int i)
{
	return pow(4, i + 1);
}


// 2014C: zig zag Ai
int Get1Ai(int i)
{
	return pow(2, i);
}


// 2022W: improved Ai
int GetAi(int i)
{
	if (i <= 2)
	{
		return i + 1;
	}
	else
	{
		return 3 * pow(2, i - 2);
	}
}


// Procedure DIG
int DIG(int i, int size, int* p, int& index)
{
	MyClass my;
	double size1 = 0;

	while (size >= 1)
	{
		if (size > 1)
		{
			size1 = ceil(size / 2);
			if (!my.TestGroup(p + index, size1))
			{
				size = size - size1;
				index += size1;
			}
			else
			{
				size = size - size1;
			}
		}
		else
		{
			my.IndentifyID(p + index, size);
			index += size;
			size--;
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
	return 0;
}


// Test 3 items (1994Du)
int Test1_3(int size, int* p, int& index)
{
	MyClass my;
	int x, y, z;

	x = 1;
	y = 1;
	z = 1;

	int g = 0;
	if (!my.TestGroup(p + index, x)) { index += x; g++; }
	else { my.IndentifyID(p + index, x); index += x; }

	if (!my.TestGroup(p + index, y)) { index += y; g++; }
	else { my.IndentifyID(p + index, y); index += y; }

	if (g == 2) { my.IndentifyID(p + index, z); index += z; }

	ToCo += my.testCount;
	ToD += my.NumD;
	return 0;
}


// 3-Test (1994Du)
int Test2_3(int i, int size, int* p, int& index)
{
	MyClass my;
	int x, y = 0;

	x = (size <= pow(2, i)) ? size : pow(2, i);
	y = ((int)size - x <= pow(2, i + 1)) ? (int)size - x : pow(2, i + 1);


	if (y == 0)
	{
		DIG(i, x, p, index);
	}
	else
	{
		if (!my.TestGroup(p + index, x))
		{
			index += x;
		}
		else
		{
			DIG(i, x, p, index);
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
	return 0;
}


// Procedure Big_3 (1994Du)
void Big_3(int size, int* pp, int& index, int K)
{
	MyClass my;
	int i = 0, pd = 0;
	int t = 0;
	int oneIndex = 0;

	while (index < size)
	{
		if (t == 0)
		{
			if (t > Get00Ai(i) && pd == K)
			{
				if (!my.TestGroup(pp + index, size - index))
				{
					index += (size - index);
				}
				else
				{
					t = (size - index <= Get00Ai(i)) ? size - index : Get00Ai(i);
				}
			}
			else
			{
				t = (size - index <= Get00Ai(i)) ? size - index : Get00Ai(i);
			}
		}
		else
		{
			int statu = my.TestGroup(pp + index, t);
			if (statu == 0)
			{
				index += t;
				i = i + 2; pd++;
			}
			else
			{
				if (t <= 2)
				{
					my.TestIndiviAll(pp + index, t);
					index += t;
					i = 0; pd = 0;
				}
				else if (t == 3)
				{
					Test1_3(t, pp, index);
					i = 0;
				}
				else
				{
					Test2_3(i, t, pp, index);
					i = 0;
				}
			}
			t = 0;
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
}


// Test 4 items (2005ST)
void Test1_4(int size, int* p, int& index)
{
	MyClass my;
	int x, y, z, w = 0;

	x = 1;
	y = 1;
	z = 1;
	w = 1;

	int g = 0;
	if (!my.TestGroup(p + index, x)) { index += x; g++; }
	else { my.IndentifyID(p + index, x); index += x; }

	if (!my.TestGroup(p + index, y)) { index += y; g++; }
	else { my.IndentifyID(p + index, y);  index += y; }

	if (!my.TestGroup(p + index, z)) { index += z; g++; }
	else { my.IndentifyID(p + index, z); index += z; }

	if (g == 3) { my.IndentifyID(p + index, w); index += w; }

	ToCo += my.testCount;
	ToD += my.NumD;
}


// 4-Test (2005ST)
int Test2_4(int i, int size, int* p, int& index)
{
	MyClass my;
	int x, y, z, w = 0;

	x = (size <= pow(4, i)) ? size : pow(4, i);
	y = (size - x <= pow(4, i)) ? (int)size - x : pow(4, i);
	z = ((long)size - x - y <= pow(4, i)) ? (int)size - x - y : pow(4, i);
	w = ((int)size - x - y - z <= pow(4, i)) ? (int)size - x - y - z : pow(4, i);

	if (y == 0 && z == 0 && w == 0)
	{
		DIG(i, x, p, index);
	}
	else
	{
		if (!my.TestGroup(p + index, x))
		{
			index += x;
			if (!my.TestGroup(p + index, y))
			{
				index += y;
				if (!my.TestGroup(p + index, z))
				{
					index += z;
					DIG(i, w, p, index);
				}
				else
				{
					DIG(i, z, p, index);
				}
			}
			else
			{
				DIG(i, y, p, index);
			}
		}
		else
		{
			DIG(i, x, p, index);
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
	return 0;
}


// 4-Split
int Split_4(int i, int size, int* p, int& index, int oneIndex)
{
	MyClass my;
	int Y, Z, U, V = 0;

	if (size == 1)
	{
		my.IndentifyID(p + index, size);
		index += size;
	}
	else if (size == 2 || size == 3)
	{
		my.TestIndividual(p + index, size, oneIndex);
		index += oneIndex;
	}
	else
	{
		Y = (size <= pow(2, i - 2)) ? size : pow(2, i - 2);
		Z = ((int)size - Y <= pow(2, i - 2)) ? (int)size - Y : pow(2, i - 2);
		U = ((int)size - Y - Z <= pow(2, i - 3)) ? (int)size - Y - Z : pow(2, i - 3);
		V = ((int)size - Y - Z - U <= pow(2, i - 3)) ? (int)size - Y - Z - V : pow(2, i - 3);

		if (Z == 0 && U == 0 && V == 0)
		{
			DIG(i, Y, p, index);
		}
		else
		{
			if (!my.TestGroup(p + index, Y))
			{
				index += Y;
				if (!my.TestGroup(p + index, Z))
				{
					index += Z;
					if (!my.TestGroup(p + index, U))
					{
						index += U;
						DIG(i, V, p, index);
					}
					else
					{
						DIG(i, U, p, index);
					}
				}
				else
				{
					DIG(i, Z, p, index);
				}
			}
			else
			{
				DIG(i, Y, p, index);
			}
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
	return 0;
}


// Procedure Big_4 (2005ST)
void Big_4(int size, int* pp, int& index, int K)
{
	MyClass my;
	int i = 0, pd = 0;
	int t = 0;
	int oneIndex = 0;

	while (index < size)
	{
		if (t == 0)
		{
			if (t > Get0Ai(i) && pd == K)
			{
				if (!my.TestGroup(pp + index, size - index))
				{
					index += (size - index);
				}
				else
				{
					t = (size - index <= Get0Ai(i)) ? size - index : Get0Ai(i);
				}
			}
			else
			{
				t = (size - index <= Get0Ai(i)) ? size - index : Get0Ai(i);
			}
		}
		else
		{
			int statu = my.TestGroup(pp + index, t);
			if (statu == 0)
			{
				index += t;
				i++; pd++;
			}
			else
			{
				if (t <= 3)
				{
					my.TestIndiviAll(pp + index, t);
					index += t;
				}
				else if (t == 4)
				{
					Test1_4(t, pp, index);
				}
				else
				{
					Test2_4(i, t, pp, index);
					i = 0;
				}
			}
			t = 0;
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
}


// original zig-zag approach
void Procedure_Z(int size, int* pp, int& index)
{
	MyClass my;
	int i, t = 0;
	int one_Index = 0;

	i = ceil(log(size) / log(2));
	while (index < size)
	{
		t = ((size - index) <= Get1Ai(i)) ? (size - index) : Get1Ai(i);

		if (!my.TestGroup(pp + index, t))
		{
			i++; index += t;
		}
		else
		{
			if (i > 0)
			{
				DIG(i, t, pp, index);
				i--;
			}
			else
			{
				my.IndentifyID(pp + index, t);
				index += t;
			}
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
}


// Z^1
void Procedure_Z1(int size, int* pp, int& index)
{
	MyClass my;
	int i, t = 0;
	int one_Index = 0;

	i = ceil(log(4 * size / 3) / log(2));
	while (index < size)
	{
		t = ((size - index) <= GetAi(i)) ? (size - index) : GetAi(i);

		//for (int i = index; i < size; i++)
		//{
		//	cout << pp[i];
		//}cout << endl;

		if (!my.TestGroup(pp + index, t))
		{
			i++; index += t;
		}
		else
		{
			if (i > 0)
			{
				Split_4(i, t, pp, index, one_Index);
				i--;
			}
			else
			{
				my.IndentifyID(pp + index, t);
				index += t;
			}
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
}


// Z^2
void Procedure_Z2(int size, int* pp, int index)
{
	MyClass my;
	int i = 0, pd = 0, TS = 0;
	int t = 0;
	int oneIndex = 0;

	while (index < size)
	{
		if (t == 0)
		{
			if (t > GetAi(i) && pd == 6)
			{
				if (!my.TestGroup(pp + index, size - index))
				{
					index += (size - index);
				}
				else
				{
					t = (size - index <= GetAi(i)) ? size - index : GetAi(i);
				}
			}
			else
			{
				t = (size - index <= GetAi(i)) ? size - index : GetAi(i);
			}
		}
		else
		{
			int statu = my.TestGroup(pp + index, t);
			if (statu == 0)
			{
				index += t;
				i++; pd++;
			}
			else if (statu == 1 && i == 1)
			{
				my.TestIndiviAll(pp + index, t);
				index += t;

				if (my.NumP == 1)
				{
					i++; pd++; TS = 1;
				}
				else
				{
					i--; pd = 0; TS = 0;
				}
			}
			else
			{
				if (TS == 1 && i == 2)
				{
					my.TestIndiviAll(pp + index, t);
					index += t;
					i--; pd = 0; TS = 0;
				}
				else
				{
					pd = 0;
					if (i != 0)
					{
						Split_4(i, t, pp, index, oneIndex);
						i--;
						index += oneIndex;
					}
					else
					{
						my.IndentifyID(pp + index, t);
						index += t;
					}
				}
			}
			t = 0;
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
}


/*-----------------------------------*/
/**********(Du et al. 2005)***********/
/*-----------------------------------*/
void Algorithm_Du(const char* InputName, const char* OutputName, double& MDu)
{
	MyClass my;
	int index = 0;
	ToCo = 0; ToD = 0;

	//k > 40, k = ceil(0.4 * (1/sigma)), 0 < sigma <= 0.01
	int K = 10;

	if (N >= 3)
	{
		Big_3(N, p, index, K);
	}
	else
	{
		my.TestIndiviAll(p + index, N);
	}

	ToCo += my.testCount;
	ToD += my.NumD;
	MDu += ToCo;
	cout << " -------------------- " << endl;
	cout << "\t\t" << "M_{Du}(d|n) = " << ToCo << endl;
	cout << "\t\t" << "d_{Du} = " << ToD << endl;
}


/*-----------------------------------*/
/******(Schlaghoof&Triesch 2005)******/
/*-----------------------------------*/
void Algorithm_ST(const char* InputName, const char* OutputName, double& MST)
{
	MyClass my;
	int index = 0;
	ToCo = 0; ToD = 0;

	//k > 40, k = ceil(0.4 * (1/sigma)), 0 < sigma <= 0.01
	int K = 41;

	if (N >= 4)
	{
		Big_4(N, p, index, K);
	}
	else
	{
		my.TestIndiviAll(p + index, N);
	}

	ToCo += my.testCount;
	ToD += my.NumD;
	MST += ToCo;
	cout << " -------------------- " << endl;
	cout << "\t\t" << "M_{ST}(d|n) = " << ToCo << endl;
	cout << "\t\t" << "d_{ST} = " << ToD << endl;
}


/*-----------------------------------*/
/********(Cheng et al. 2014)**********/
/*-----------------------------------*/
void Algorithm_Z(const char* InputName, const char* OutputName, double& MZ)
{
	MyClass my;
	int index = 0;
	ToCo = 0; ToD = 0;

	Procedure_Z(N, p, index);
	ToCo += my.testCount;
	ToD += my.NumD;
	MZ += ToCo;
	cout << " -------------------- " << endl;
	cout << "\t\t" << "M_{Z}(d|n) = " << ToCo << endl;
	cout << "\t\t" << "d_{Z} = " << ToD << endl;
}


/*-----------------------------------*/
/**********(Wu et al. 2022)***********/
/*-----------------------------------*/
void Algorithm_Zstar(const char* InputName, const char* OutputName, double& MZstar)
{
	MyClass my;
	int index = 0;
	ToCo = 0; ToD = 0;

	Procedure_Z1(N, p, index);
	ToCo += my.testCount;
	ToD += my.NumD;
	MZstar += ToCo;
	cout << " -------------------- " << endl;
	cout << "\t\t" << "M_{Z^*}(d|n) = " << ToCo << endl;
	cout << "\t\t" << "d_{Z^*} = " << ToD << endl;

}


/*-----------------------------------*/
/*******(1.45-competitive Alg)********/
/*-----------------------------------*/
void Algorithm_Zc(const char* InputName, const char* OutputName, double& MZc)
{
	MyClass my;
	int index = 0;
	int size, oneIndex;
	int n1, nR1, n2, nR2 = 0;
	ToCo = 0; ToD = 0;

	n1 = floor(N / 4);
	nR1 = N - 4 * n1;

	//test R1
	if (nR1 != 0)
	{
		my.TestIndiviAll(p + index, nR1);
		index += nR1;
		cout << "R_1 is not empty" << endl;
	}

	//test S^1_v for v = 1,2,3,4 simultaneously and get alpha_1
	if (index < N)
	{
		/************************************************************************/
		int* p1 = new int[N];

		for (int i = 0; i < N; i++) { p1[i] = 2; }
		int index1 = index;
		int alpha_1 = 0;

		for (int j = 0; j < 4; j++)
		{
			if (!my.TestGroup(p + index1, n1))
			{
				index1 += n1;
			}
			else
			{
				memcpy(p1 + (int)alpha_1 * n1, p + index + (int)j * n1, sizeof(int) * (n1));
				index1 += n1;
				alpha_1++;
			}
		}
		int A1 = alpha_1 * n1;
		int* pCopy = new int[A1];
		for (int i = 0; i < A1; i++) { pCopy[i] = 2; }
		memcpy(pCopy, p1, sizeof(int) * A1);
		/************************************************************************/


		if (alpha_1 == 0)
		{
			cout << "All items in S^1_v are pure" << endl;
		}
		else if (alpha_1 == 1)
		{
			cout << "One subset of S_n is contaimnated" << endl;
			if (n1 != 1)
			{
				Procedure_Z1(n1, pCopy, index);
			}
			else
			{
				my.NumD++;
			}
		}
		else if (alpha_1 == 2)
		{
			cout << "Two subsets of S_n are contaimnated" << endl;
			n2 = floor(n1 / 2);
			nR2 = 2 * n1 - 4 * n2;
			int nt = alpha_1 * n1;

			/************************************************************************/
			//test R2
			if (nR2 != 0)
			{
				my.TestIndiviAll(pCopy + index, nR2);
				index += nR2;
				cout << "R_2 is not empty" << endl;
			}

			//test S^2_v for v = 1,2,3,4 simultaneously and get alpha_2
			int* p2 = new int[nt];
			for (int i = 0; i < nt; i++) { p2[i] = 2; }
			int index2 = index;
			int alpha_2 = 0;

			for (int j = 0; j < 4; j++)
			{
				if (!my.TestGroup(pCopy + index2, n2))
				{
					index2 += n2;
				}
				else
				{
					memcpy(p2 + (int)alpha_2 * n2, pCopy + index + (int)j * n2, sizeof(int) * (n2));
					index2 += n2;
					alpha_2++;
				}
			}
			int A2 = alpha_2 * n2;
			int* pCCopy = new int[A2];
			for (int i = 0; i < A2; i++) { pCCopy[i] = 2; }
			memcpy(pCCopy, p2, sizeof(int) * A2);
			/************************************************************************/

			if (alpha_2 == 1)
			{
				if (n2 != 1)
				{
					cout << "One subset of S^2_v is contaimnated" << endl;
					Procedure_Z1(A2, pCCopy, index);
				}
				else
				{
					my.NumD++;
				}
			}
			else if (alpha_2 == 2)
			{
				cout << "At most two subsets of S^2_v are contaimnated" << endl;
				Procedure_Z1(A2, pCCopy, index);
			}
			else
			{
				cout << "At least three subsets of S^2_v are contaimnated" << endl;
				Procedure_Z2(A2, pCCopy, 0);
			}
		}
		else
		{
			cout << "At least three subsets of S_n are contaimnated" << endl;
			//for (int i = 0; i < alpha_1 * n1; i++) 
			//{ 
			//	cout << pCopy[i];
			//	if ((i+1) % n1 == 0) { cout << " || "; }
			//} cout << endl;
			Procedure_Z2(A1, pCopy, 0);
		}
	}
	ToCo += my.testCount;
	ToD += my.NumD;
	MZc += ToCo;
	cout << " -------------------- " << endl;
	cout << "\t\t" << "M_{Z^c}(d|n) = " << ToCo << endl;
	cout << "\t\t" << "d_{Z^c} = " << ToD << endl;
}


/*-----------------------------------*/
/*******(Up-Zig-Zag Strategy)*********/
/*-----------------------------------*/
void UpZigZag_Z2(const char* InputName, const char* OutputName, double& MZ2)
{
	MyClass my;
	int index = 0;
	ToCo = 0; ToD = 0;

	Procedure_Z2(N, p, index);
	ToCo += my.testCount;
	ToD += my.NumD;
	MZ2 += ToCo;
	cout << " -------------------- " << endl;
	cout << "\t\t" << "M_{Z^2}(d|n) = " << ToCo << endl;
	cout << "\t\t" << "d_{Z^2} = " << ToD << endl;
}



int main(int argc, char** argv)
{
	double MDu = 0., MST = 0., MZ = 0., MZstar = 0., MZ2 = 0., MZc = 0.;
	MyClass my;
	const char* InputName, * OutputName;
	string strFileName;
	ifstream getParaData("D:\\Research file\\XJTU_PhD\\SourceCodes_Paper\\Github_1.45ForCGTP\\A-1.45-Competitive-Alg-for-the-CGTP\\Lists\\Lists_1000_10_1.txt", std::ios::in);

	int List;
	for (List = 0; List < 1000; List++)
	{
		if (!getParaData)
		{
			cerr << "EORROR: could not open data file" << endl;
			throw(-1);
		}
		getParaData >> strFileName;

		InputName = strFileName.data();
		char* t1 = new char[strlen(InputName) + 200];
		strcpy(t1, "D:\\Research file\\XJTU_PhD\\SourceCodes_Paper\\Github_1.45ForCGTP\\A-1.45-Competitive-Alg-for-the-CGTP\\Instances_New\\1000-10-1000\\");
		strcat(t1, InputName);
		InputName = t1;
		OutputName = strFileName.data();
		char* t2 = new char[strlen(OutputName) + 200];
		strcpy(t2, "D:\\Research file\\XJTU_PhD\\SourceCodes_Paper\\Github_1.45ForCGTP\\A-1.45-Competitive-Alg-for-the-CGTP\\Results_New\\1000-10-1000\\");
		strcat(t2, OutputName);
		OutputName = t2;

		Read_Data(InputName, OutputName);
		cerr << List + 1 << endl;

		cout << "--------------------------------------------------------------" << endl;
		printf("\t\t Output Result \n");

		cout << "--------------------------------------------------------------" << endl;
		cout << "Algorithm Du Begin to Solve: " << endl;
		cout << " -------------------- " << endl;
		Algorithm_Du(InputName, OutputName, MDu);

		cout << "--------------------------------------------------------------" << endl;
		cout << "Algorithm ST Begin to Solve: " << endl;
		cout << " -------------------- " << endl;
		Algorithm_ST(InputName, OutputName, MST);

		cout << "--------------------------------------------------------------" << endl;
		cout << "Algorithm Z Begin to Solve: " << endl;
		cout << " -------------------- " << endl;
		Algorithm_Z(InputName, OutputName, MZ);

		cout << "--------------------------------------------------------------" << endl;
		cout << "Algorithm Z^* Begin to Solve: " << endl;
		cout << " -------------------- " << endl;
		Algorithm_Zstar(InputName, OutputName, MZstar);

		cout << "--------------------------------------------------------------" << endl;
		cout << "Strategy Up-Zig-Zag Begin to Solve: " << endl;
		cout << " -------------------- " << endl;
		UpZigZag_Z2(InputName, OutputName, MZ2);

		cout << "--------------------------------------------------------------" << endl;
		cout << "Algorithm Z^c Begin to Solve: " << endl;
		cout << " -------------------- " << endl;
		Algorithm_Zc(InputName, OutputName, MZc);

		printf("\n");
		cout << "----------------------Algorithm Terminate--------------------" << endl;
	}

	cout << "List = " << List;
	printf("\nThe average number of tests: \n");
	cout << "MDu = " << MDu / List << endl;
	cout << "MST = " << MST / List << endl;
	cout << "MZ = " << MZ / List << endl;
	cout << "MZstar = " << MZstar / List << endl;
	cout << "MZ2 = " << MZ2 / List << endl;
	cout << "MZc = " << MZc / List << endl;
	cout << MDu / List << " " << MST / List << " " << MZ / List << " " << MZstar / List;
	cout << " " << MZ2 / List << " " << MZc / List << endl;
	return 1;
}