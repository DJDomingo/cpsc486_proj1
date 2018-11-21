#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<math.h>

using namespace std;

//functions made
void pause();
vector<double> returnNumbers(int i, string); //returns numbers from a line within a string.
int returnDigit(char digit);
struct output
{
	string description;
	vector<double> data;
};
output scan(string);

int main() {

	ifstream myfile("test.txt");
	ofstream myfile2("output.txt");
	if (myfile.fail())
	{
		cout << "\n** ERROR - the file cannot be found! One has been created. Please place the data in test.txt";
		ofstream myfile3("test.txt");
		pause();
		return 0;
	}
	else {
		cout << "The file test.txt is found.\nlooking through data.\n";
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
					vec.at(vec.size()-1).data.push_back(scan(line).data.at(i));
				}
			}
			else
				vec.push_back(scan(line));
		}
		cout << "vector of size : " << vec.size() << endl;
		for (int i = 0; i < vec.size(); i++)
		{
			cout << vec.at(i).description <<endl;
			for (int j = 0; j < vec.at(i).data.size(); j++)
			{
				cout << vec.at(i).data.at(j)<< endl;
			}
		}
		pause();

	}
	return 0;
}



void pause() {//simple pausing function for readability
	string junk;
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
						result.description = "kid-loc " + firstNum+secondNum;
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
	bool addZero=false;
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
		else if (returnDigit(line.at(i))==-3)
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
		else if(returnDigit(line.at(i))==-2 && state2==true)
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
			while (returnDigit(line.at(i))>=0)
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
			int countDigits=0;
			while (returnDigit(line.at(i)) >= 0) {
				decimalAdd = decimalAdd * 10;
				decimalAdd += returnDigit(line.at(i));
				i++;
				countDigits++;
			}

			double divisor=pow(10, countDigits);
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
		returnVal= (int)digit - 48;
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