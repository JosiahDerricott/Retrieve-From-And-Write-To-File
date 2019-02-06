////
// Name: Josiah Derricott
// Section: B
// Program: Double the Input Number And Store It Back
//
// Description: This program takes an input file (that the user can name or not) and
//	gets any numbers from the file as ints, doubles them, then outputs them to an output
//	file (that the user can name or not). If errors occur, the program stops and errors
//	are outputed to an 'error.dump' file for futher analysis.
////

#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

// Function Prototypes
bool getInt(ifstream&, bool&, int&, int&);
void openAndProcessFile(string inputFileName = "data", string outputFileName = "data");
void startFileLookup(bool allowUserInput);
void createDumpFile(string error, const int& errorCount);
void checkFilenameInvalid(string& input, const string& retryMsg);

int main()
{
	startFileLookup(true);

	system("pause");
	return 0;
}

/*
	Checks if the given file name contains any
	invalid file name characters
*/
void checkFilenameInvalid(string& input, const string& retryMsg)
{
	while (input.find_first_of("/\\?%*:|\"<>") != string::npos || input.length() > 255)
	{
		cout << "\n >> Error: Your file name contains one or more invalid characters or is too long!\n\n"
			<< retryMsg;

		getline(cin, input);
	}
}

/*
	Allow user to either enter in the input file name and the output 
	file name or or not enter anything names and go with the defaults
*/
void startFileLookup(bool allowUserInput)
{
	cout << "Name: Josiah Derricott" << endl
		<< "Program: Double The Input Number And Store It Back" << endl
		<< "----------------------------------\n\n";

	if (allowUserInput)
	{
		string inputName, outputName;

		cout << "Please enter the input file name (or leave empty for no name): ";
		getline(cin, inputName);

		// Check that the input file name is valid
		checkFilenameInvalid(inputName, "Please enter the input file name (or leave empty for no name): ");

		cout << endl << "Please enter the output file name (or leave empty for no name): ";
		getline(cin, outputName);

		// Check that the output file name is valid
		checkFilenameInvalid(outputName, "Please enter the output file name (or leave empty for no name): ");

		if (inputName.empty() && outputName.empty())
		{
			openAndProcessFile();

			return;
		}
		else if (outputName.empty())
		{
			openAndProcessFile(inputName);

			return;
		}
		else if (inputName.empty())
		{
			openAndProcessFile("data", outputName);

			return;
		}
		else
		{
			openAndProcessFile(inputName, outputName);

			return;
		}
	}
	else
	{
		openAndProcessFile();
	}
}

/*
	Check if a given stream has good input (for int) or a bad
	input. Handle this accordingly.
*/
bool getInt(ifstream& sin, bool& goodFlag, int& storageVar, int& errorCount)
{
	sin >> storageVar; // MUST try to insert from sin to get any errors

	if (sin.good())
	{
		goodFlag = true;
		return true;
	}
	else if (sin.eof())
	{
		cout << "\n >> End of file encountered!" << endl;

		goodFlag = false;

		return false;
	}
	else if (sin.bad() || !sin.is_open()) // File could not be opened or input was corrupted
	{
		cout << "\n >> Hard I/O error found! Exiting..." << endl;

		errorCount++;

		createDumpFile(">> Hard I/O error found!", errorCount); // Error occured, output it to dump file

		goodFlag = false;

		return false;
	}
	else if (sin.fail()) // Stream failed because of bad inserstion
	{
		cout << "\n >> Input invalid - skipping rest of line..." << endl;
		sin.clear();

		char c;

		while (sin.get(c) && c != '\n') // If next char can not be stored, go to end of line then try to store again
		{
			errorCount++;

			goodFlag = false;
		}

		sin >> storageVar;

		if (sin.good())
		{
			goodFlag = true;

			return true;
		}
		else
		{
			if (sin.eof())
			{
				goodFlag = true;

				cout << "\n >> End of file found while skipping line. ";
			}
			else
			{
				cout << "\n >> Error encountered while skipping line. ";

				errorCount++;

				createDumpFile(">> An error occured while attempting to skip to the next line!", errorCount);
			}

			cout << "Exiting..." << endl;

			return false;
		}
	}
}

/*
	Open and process an input file of any given name 
	and output to an output file of any given name
*/
void openAndProcessFile(string inputFileName, string outputFileName)
{
	int quedNum = 0, errorCount = 0;
	bool goodFlag = true;
	string border = "-------------------------------";

	ifstream fin(inputFileName + ".input"); // Try to open the input file
	ofstream fout; // We only create the file output if we have no initial I/O errors

	while (getInt(fin, goodFlag, quedNum, errorCount))
	{
		if (goodFlag)
		{
			if (!fout.is_open()) // If output file is not created, create it!
				fout.open(outputFileName + ".output");

			cout << "\nValue from input: " << quedNum << endl;

			quedNum *= 2;
			fout << quedNum << endl;

			cout << "Value stored in output file: " << quedNum << endl;

			cout << border.c_str() << endl;
		}
		else
		{
			break;
		}
	}

	if (goodFlag)
	{
		cout << "\n::..:: Finished! ::..::" << endl;
	}
	else
	{
		cout << "\n::..:: Exited with " << errorCount << " Error(s)! ::..::\n\nSee 'error.dump' file for more info.\n\n";
	}

	fin.close();
	fout.close();
}

void createDumpFile(string error, const int& errorCount)
{
	ofstream dumpFile;

	if (!dumpFile.is_open())
	{
		dumpFile.open("error.dump");
	}

	dumpFile << "File closed with " << errorCount << " error(s)!" << endl
		<< error;

	dumpFile.close();
}