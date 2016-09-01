#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
	
	//Input from LiDAR serial port
	ifstream infile("/dev/ttyACM1");
	string data;
	infile >> data;
	cout << data << endl;

	return 0;

}
