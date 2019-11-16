#include <iostream>
#include <map>
#include <ctype.h>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

//gets a map by reference, an open input file stream, and a bool by reference that is changed to false if there is an error
//Populates the map with the pairs of letters that are in the file
void getdict(map<char, char> &mymap, ifstream& fin, bool& error) 
{
	string pair;
	int i = 0;
	char used[26];
	char missing;
	while (fin >> pair)
	{
		missing = 'a' + i;
		if (pair[0] != missing)
		{
			cerr << "MISSING LETTER " << missing << endl;
			error = true;
			return;
		}
		if (pair.length() != 2)
		{
			cerr << "FORMATTING ERROR " << pair << endl;
			error = true;
			return;
		}
		if (pair[0] == pair[1])
		{
			cerr << "MAPPING ERROR " << pair[0] << endl;
			error = true;
			return;
		}
		for (int j = 0; j < i; j++)
		{			
			if (pair[1] == used[j])
			{
				cerr << "DUPLICATE CIPHERTEXT " << pair[1] << endl;
				error = true;
				return;
			}
		}
		mymap[pair[0]] = pair[1];
		used[i] = pair[1];
		i++;
	}
}
//Takes an array of size 26 and puts a random alphabet where no letter is on its normal position
void randomizeABC(char arr[], int size)  
{
	char abc[26];
	for (int i = 0; i < 26; i++)
	{
		abc[i] = 'a' + i;
	}
	srand(time(NULL));
	for (int i = 0; i < 26; i++)
	{
		int key = rand() % size;
		while (abc[key] == ('a' + i))
		{
			key = rand() % size;
		}
		arr[i] = abc[key];
		abc[key] = abc[size - 1];
		size--;
	}
}
//returns false if argc less than 2 or more than 4. else returns true
bool argcValidation(int argc) 
{
	if (argc < 2)
	{
		cerr << "MISSING COMMAND" << endl;
		return false;
	}
	else if (argc > 4)
	{
		cerr << "TOO MANY ARGUMENTS" << endl;
		return false;
	}
	else
	{
		return true;
	}
}

int main(int argc, char* argv[])
{
	//Calls argcvalidation which returns false if there is an unexpected amount of arguments
	if (argcValidation(argc) == false) 
	{
		return -1;
	}
	//Validates the second argument
	if (!((strcmp(argv[1], "-r") == 0) || (strcmp(argv[1], "-g") == 0) || (strcmp(argv[1], "-e") == 0) || (strcmp(argv[1], "-d") == 0)))
	{
		cout << argv[1] << " NOT A VALID COMMAND" << endl;
		return -1;
	}
	//Performs a rot13 substitution on a file if there is one, else takes input and does the substitution
	if (strcmp(argv[1], "-r") == 0)
	{
		map <char, char> rot;
		char abc[26];					//array to hold the regular alphabet
		for (int i = 0; i < 26; i++)	//populate alphabet array with alphabet
		{
			abc[i] = 'a' + i;
		}
		for (int i = 0; i < 26; i++)	//populate rot13 map matching each letter with another letter that is 13 positions far
		{
			
			rot[abc[i]] = abc[(i + 13) % 26];
		}
		if (argc == 3) // read from file
		{
			ifstream fin;
			fin.open(argv[2]);
			if (fin.is_open() == false)
			{
				cerr << argv[2] << " FILE COULD NOT BE OPENED" << endl;
				return -1;
			}
			else
			{
				string input;
				while (getline(fin, input))
				{
					int length = input.length();
					for (int i = 0; i < length; i++)
					{
						if (isalpha(input[i]))
						{
							if (isupper(input[i]))
							{
								char s = rot[tolower(input[i])];
								s = s - 32;
								cout << s;
							}
							else
							{
								cout << rot[input[i]];
							}
						}
						else
						{
							cout << input[i];
						}
					}
					cout << endl;
				}
				fin.close();
			}
		}
		
		if (argc == 2) // standard input
		{
			string input;
			while ( getline(cin,input) )
			{
				int length = input.length();
				for (int x = 0; x < length; x++)
				{
					if (isalpha(input[x]))
					{
						if (isupper(input[x]))
						{
							char s = rot[tolower(input[x])];
							s = s - 32;
							cout << s;
						}
						else
						{
							cout << rot[input[x]];
						}
					}
					else
					{
						cout << input[x];
					}
				}
				cout << endl;
			}
		}

	}

	if (strcmp(argv[1], "-g") == 0)
	{
		map<char, char> caesar;
		int size = 26;
		char randABC[26];
		char abc[26];
		for (int i = 0; i < 26; i++)
		{
			abc[i] = 'a' + i;
		}
		//randABC has randomly generated alphabet
		randomizeABC(randABC, size);
		
		for (int i = 0; i < 26; i++)
		{
			caesar[abc[i]] = randABC[i];
		}

		int i = 0;
		char alp[26], ralp[26];
		for (map<char, char>::iterator it = caesar.begin(); it != caesar.end(); it++)
		{
			alp[i] = it->first;
			ralp[i] = it->second;
			i+=1;
		}
		for (int j = 0; j < 26; j++)
		{
			cout << alp[j] << ralp[j] << ' ';
		}
	}

	if (strcmp(argv[1], "-e") == 0)
	{
		if (argc < 2)
		{
			cerr << "NO DICTIONARY GIVEN";
			return -1;
		}
		else if (argc > 4)
		{
			cerr << "TOO MANY ARGUMENTS";
			return -1;
		}

		bool error = false;
		map<char, char> encrypt;
		ifstream fin;
		fin.open(argv[2]);
		if (fin.is_open() == false)
		{
			cerr << argv[2] << " DICTIONARY COULD NOT BE OPENED" << endl;
			return -1;
		}
		
		getdict(encrypt, fin, error);
		if (error == true)
			return -1;

		fin.close();
		map<char, char> decrypt;
		for (map<char, char>::iterator it = encrypt.begin(); it != encrypt.end(); it++)
		{
			decrypt[it->second] = it->first;
		}
		
		if (argc == 4)
		{
			ifstream finf;
			finf.open(argv[3]);
			if (finf.is_open() == false)
			{
				cerr << argv[3] << " FILE COULD NOT BE OPENED" << endl;
				return -1;
			}
			string input;
			while (getline(finf, input))
			{
				int length = input.length();
				for (int i = 0; i < length; i++)
				{
					if (isalpha(input[i]))
					{
						if (isupper(input[i]))
						{
							char s = encrypt[tolower(input[i])];
							s = s - 32;
							cout << s;
						}
						else
						{
							cout << encrypt[input[i]];
						}
					}
					else
					{
						cout << input[i];
					}
				}
				cout << endl;
			}
			finf.close();
		}
		else if (argc == 3)
		{
			string input;
			while (getline(cin, input))
			{
				int length = input.length();
				for (int x = 0; x < length; x++)
				{
					if (isalpha(input[x]))
					{
						if (isupper(input[x]))
						{
							char s = encrypt[tolower(input[x])];
							s = s - 32;
							cout << s;
						}
						else
						{
							cout << encrypt[input[x]];
						}
					}
					else
					{
						cout << input[x];
					}
				}
				cout << endl;
			}
		}

	}
	if (strcmp(argv[1], "-d") == 0)
	{
		if (argc < 2)
		{
			cerr << "NO DICTIONARY GIVEN";
			return -1;
		}
		else if (argc > 4)
		{
			cerr << "TOO MANY ARGUMENTS";
			return -1;
		}

		bool error = false;
		map<char, char> encrypt;
		ifstream fin;
		fin.open(argv[2]);
		if (fin.is_open() == false)
		{
			cerr << argv[2] << " DICTIONARY COULD NOT BE OPENED" << endl;
			return -1;
		}
		getdict(encrypt, fin, error);
		if (error == true)
			return -1;

		fin.close();
		map<char, char> decrypt;
		for (map<char, char>::iterator it = encrypt.begin(); it != encrypt.end(); it++)
		{
			decrypt[it->second] = it->first;
		}
		if (argc == 4)
		{
			ifstream finf;
			finf.open(argv[3]);
			if (finf.is_open() == false)
			{
				cerr << argv[3] << " FILE COULD NOT BE OPENED" << endl;
				return -1;
			}
			string input;
			while (getline(finf, input))
			{
				int length = input.length();
				for (int i = 0; i < length; i++)
				{
					if (isalpha(input[i]))
					{
						if (isupper(input[i]))
						{
							char s = decrypt[tolower(input[i])];
							s = s - 32;
							cout << s;
						}
						else
						{
							cout << decrypt[input[i]];
						}
					}
					else
					{
						cout << input[i];
					}
				}
				cout << endl;
			}
			finf.close();
		}
		else if (argc == 3)
		{
			
			string input;
			while (getline(cin, input))
			{
				int length = input.length();
				for (int x = 0; x < length; x++)
				{
					if (isalpha(input[x]))
					{
						if (isupper(input[x]))
						{
							char s = decrypt[tolower(input[x])];
							s = s - 32;
							cout << s;
						}
						else
						{
							cout << decrypt[input[x]];
						}
					}
					else
					{
						cout << input[x];
					}
				}
				cout << endl;
			}
		}

	}


	/*
	Both -e and -d support an optional third argument, which is the file to read from. If it is notprovided, the program should read from standard input. 
	The output should be generated to thestandard output. If a filename is provided but the file cannot open for any reason, the programshould print the 
	filename followed by a space and FILE COULD NOT BE OPENED, then stop.In all cases if there are too many command line arguments, your program should 
	print TOOMANY ARGUMENTS, then stop.
	*/








	//Map's contents output

	/*for (map<char, char>::iterator it = rot.begin(); it != rot.end(); it++)
	{
		cout << it->first << " " << it->second << endl;
	}*/

	//Map insertion

	/*map <char, char> rot;
	char abc[26] = { 'a', 'b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v', 'w','x','y','z' };
	for (int i = 0; i < 26; i++)
	{

		rot[abc[i]] = abc[(i + 13) % 26];
	}

	map <char, char> inputMap;
	inputMap.insert(rot.begin(), rot.end());
	cout << "inputMap map is:\n\n";
	for (map<char, char>::iterator it = inputMap.begin(); it != inputMap.end(); it++)
	{
		cout << it->first << " " << it->second << endl;
	}*/
}

