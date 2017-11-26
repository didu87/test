#include <iostream>
#include <fstream>
#include <random>

using namespace std;

const int LINES=100*1024;
const int MAX_LEN=100*1024; 

int main()
{
	ofstream fout("test3.txt");
	random_device rd;
	mt19937 gen(rd());
	for(int i=0;i<LINES;i++)
	{
		int m=gen()%MAX_LEN+1;
		for(int j=0;j<m;j++)
		{
			int x=gen()%10;
			fout<<x<<' ';
		}
		fout<<endl;
	}
	return 0;
}
