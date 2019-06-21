// File I/O

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <Winuser.h>


struct Participant
{
	int id = 0;
	std::string surname = "surname";
	std::string name = "name";
	std::string country = "country";
	int mark = 0;
};

struct Country_mark
{
	std::string name = "name";
	int number = 0;
	int mark = 0;
};

int get_players_number(std::ifstream&);
int get_marks_number(std::ifstream&);
void insertion_sort(Participant*, int);
void insertion_sort(Country_mark*, int);

int main()
{
	// Pathes to the I/O files 
	std::string in_address = "C:\\Users\\User\\source\\repos\\Lab_4\\Lab_4\\Iofiles\\infile.txt";
	std::string out_address = "C:\\Users\\User\\source\\repos\\Lab_4\\Lab_4\\Iofiles\\outfile.txt";

	// Open the reading file
	std::ifstream infile(in_address);
	if (!infile.is_open())
	{
		std::cout << "Could not open the infile.";
		infile.close();
		return -1;
	}

	// Open the writing file
	std::ofstream outfile(out_address, std::ios::out | std::ios::trunc);
	if (!outfile.is_open())
	{
		std::cout << "Could not open the outfile.";
		outfile.close();
		return -2;
	}




	/*
		LOGIC
	*/

	// Determine number of the players == number of lines 
	int players_number = get_players_number(infile);

	// Create an struct array of this size
	Participant* player = new Participant[players_number];

	// Read data from the infile to the Participant array	//
	// And calculating average marks on the fly				//


	// Define the nubmber of marks 
	int marks_number = get_marks_number(infile);


	// Supplemetary variables to find the average 
	int* temp_marks = new int[marks_number];
	int max = 0, min = INT_MAX;
	int sum = 0;
	std::string temp;

	for (int i = 0; i < players_number; i++)
	{
		// Skip comments lines 
		infile >> temp;
		if (temp[0] == '#')
		{
			i--;
			getline(infile, temp);
			continue;
		}

		sum = 0;
		max = 0;
		min = INT_MAX;

		player[i].id = i + 1;

		// Skip number, surname, name
		// Can we use seek?
		// infile >> player[i].surname;



		player[i].surname = temp;
		infile >> player[i].name;
		infile >> player[i].country;

		// Calculate an average mark

		// Parse 
		for (int j = 0; j < marks_number; j++)
		{
			infile >> temp;
			
			try
			{
				temp_marks[j] = stoi(temp);
			}
			catch (const std::exception&)
			{
				std::cout << "The input file is corrupted!" << std::endl;
				
				delete[] temp_marks;
				delete[] player;
				
				infile.close();
				outfile.close();

				system("pause");
			}


		}

		// Find the min and the max
		for (int j = 0; j < marks_number; j++)
		{
			if (temp_marks[j] < min)
				min = temp_marks[j];

			if (temp_marks[j] > max)
				max = temp_marks[j];
		}

		// Find the sum
		for (int j = 0; j < marks_number; j++)
			sum += temp_marks[j];

		// Calculate average withot the min and max
		player[i].mark = (sum - min - max) / (marks_number - 2);
	}

	delete[] temp_marks;



	// Insertion sort of player array using binary search by the mark parameter
	insertion_sort(player, players_number);

	int spaces_number = 0;

	for (int i = 0; i < players_number; i++)
	{

		std::cout << i + 1;
		std::cout << "\t";
		std::cout << player[i].id;
		std::cout << "\t";
		std::cout << player[i].surname;
		std::cout << " ";
		std::cout << player[i].name;

		spaces_number = 30 - player[i].surname.length() - player[i].name.length();
		for (int i = 0; i < spaces_number; i++)
		{
			std::cout << " ";
		}

		std::cout << player[i].country;
		
		spaces_number = 20 - player[i].country.length();
		for (int i = 0; i < spaces_number; i++)
		{
			std::cout << " ";
		}

		std::cout << player[i].mark;
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;


	for (int i = 0; i < players_number; i++)
	{

		outfile  << i + 1;
		outfile  << "\t";
		outfile  << player[i].id;
		outfile  << "\t";
		outfile  << player[i].surname;
		outfile  << " ";
		outfile  << player[i].name;

		spaces_number = 30 - player[i].surname.length() - player[i].name.length();
		for (int i = 0; i < spaces_number; i++)
		{
			outfile  << " ";
		}

		outfile  << player[i].country;

		spaces_number = 20 - player[i].country.length();
		for (int i = 0; i < spaces_number; i++)
		{
			outfile  << " ";
		}

		outfile  << player[i].mark;
		outfile  << std::endl;
	}
	outfile  << "\n\n";
	

	// Sort by country into country groups in the 
	Participant temp_participant;
	for (int i = 0; i < players_number; i++)
	{
		for (int j = i + 1; j < players_number; j++)
		{
			if (player[i].country == player[j].country)
			{
				// Put this participant near the country that we are examining
				// By swaping 
				temp_participant = player[i + 1];
				player[i + 1] = player[j];
				player[j] = temp_participant;

				i++;
			}
		}

	}



	// Find the number of countries 
	int country_number = 1;

	if (players_number == 0)
		country_number = 0;

	for (int i = 0; i < players_number - 1; i++)
	{
		if (player[i].country != player[i + 1].country)
			country_number++;
	}

	// Create an array of structs, which constist of a name of a county and its average mark
	Country_mark* country_marks = new Country_mark[country_number];

	// Fill the array 
	int count = 1;
	int unique_country_count = 0;
	sum = 0;

	for (int i = 0; i < players_number - 1; i++)
	{
		sum += player[i].mark;

		if (player[i].country == player[i + 1].country)
		{
			count++;

			// The last country is from the same coutry as the previous one 
			if (i == players_number - 2)
			{
				country_marks[unique_country_count].name = player[i].country;
				country_marks[unique_country_count].number = count;
				country_marks[unique_country_count].mark = sum / count;

			}

		}
		else
		{
			country_marks[unique_country_count].name = player[i].country;
			country_marks[unique_country_count].number = count;
			country_marks[unique_country_count].mark = sum / count;
			unique_country_count++;

			sum = 0;
			count = 1;

			// The last country is NOT from the same coutry as the previous one 
			if (i == players_number - 2)
			{
				country_marks[unique_country_count].name = player[i + 1].country;
				country_marks[unique_country_count].number = count;
				country_marks[unique_country_count].mark = player[i + 1].mark / count;
			}

		}
	}

	// Insertion sort of the countries array using binary search by the mark parameter
	insertion_sort(country_marks, country_number);

	//Write results to the outfile 
	for (int i = 0; i < country_number; i++)
	{
		if (i == 0)
			outfile << "******** TOP 1 ********" << std::endl;
		else if (i == 1)
			outfile << "******** TOP 2 ********" << std::endl;
		else if (i == 2)
			outfile << "******** TOP 3 ********" << std::endl;
		else
			outfile << "Place #" << i + 1 << std::endl;

		outfile << country_marks[i].name << std::endl;
		outfile << "Country average: " << country_marks[i].mark << std::endl;
		outfile << std::endl;

		for (int j = 0; j < players_number; j++)
		{

			if (country_marks[i].name == player[j].country)
			{
				outfile << player[j].surname;
				outfile << "\t";

				outfile << player[j].name;
				outfile << "\t";

				outfile << player[j].mark;
				outfile << "\t"; 

					outfile << std::endl;
			}
		}

		outfile << std::endl << std::endl << std::endl;
	}


	// Print results in the console
	for (int i = 0; i < country_number; i++)
	{
		if (i == 0)
			std::cout << "******** TOP 1 ********" << std::endl;
		else if (i == 1)
			std::cout << "******** TOP 2 ********" << std::endl;
		else if (i == 2)
			std::cout << "******** TOP 3 ********" << std::endl;
		else
			std::cout << "Place #" << i + 1 << std::endl;

		std::cout << country_marks[i].name << std::endl;
		std::cout << "Country average: " << country_marks[i].mark << std::endl;
		std::cout << std::endl;

		for (int j = 0; j < players_number; j++)
		{

			if (country_marks[i].name == player[j].country)
			{
				std::cout << player[j].surname;
				std::cout << "\t";

				std::cout << player[j].name;
				std::cout << "\t";

				std::cout << player[j].mark;
				std::cout << "\t"; \

					std::cout << std::endl;
			}
		}

		std::cout << std::endl << std::endl << std::endl;
	}


	// Free the allocated memory
	delete[] player;
	delete[] country_marks;

	// Close all files
	infile.close();
	outfile.close();

	system("pause");
}






/*
		FUNCTIONS
*/


int get_players_number(std::ifstream &infile)
{
	int players_number = 0;
	std::string temp;

	while (getline(infile, temp))
	{
		if (temp[0] != '#')
			players_number++;
	}

	infile.clear();
	infile.seekg(0, std::ios::beg);

	return players_number;
}

int get_marks_number(std::ifstream& infile)
{
	int marks_number = 0;
	std::string temp;
	int temp_int;

	// Skip the surname and check on comments lines 
	while (infile >> temp)
	{
		// Check if the line is a comment line
		if (temp[0] == '#')
		{
			getline(infile, temp);
			continue;
		}
		else
		{
			break;
		}
	}

	// Also skip first name and the country
	infile >> temp;
	infile >> temp;

	while (infile >> temp)
	{
		try
		{
			temp_int = stoi(temp);
			marks_number++;
		}
		catch (const std::exception&)
		{
			break;
		}
	}




	infile.clear();
	infile.seekg(0, std::ios::beg);


	return marks_number;
}


void insertion_sort(Participant* player, int players_number)
{
	Participant temp_participant;
	int beg, middle, end;

	for (int i = 1; i < players_number; i++)
	{
		if (player[i].mark >= player[i - 1].mark)
		{
			// The corner case, when the parameter bigger than the first parameter 
			if (player[i].mark >= player[0].mark)
			{
				// Shift elemets to the right and insert the element
				temp_participant = player[i];
				for (int j = i - 1; j >= 0; j--)
				{
					player[j + 1] = player[j];
				}
				player[0] = temp_participant;

				continue;
			}

			// Set beg and end to default
			beg = 0;
			end = i;

			// Using binary search determine the place of the element 
			while (true)
			{
				middle = (beg + end) / 2;

				// The valid position condition
				if (player[middle].mark >= player[i].mark
					&&	player[middle + 1].mark <= player[i].mark)
				{
					// Shift elemets to the right and insert the element
					temp_participant = player[i];
					for (int j = i - 1; j >= middle + 1; j--)
					{
						player[j + 1] = player[j];
					}
					player[middle + 1] = temp_participant;

					break;
				}
				else
				{
					if (player[i].mark < player[middle].mark)
						beg = middle;
					else
						end = middle;
				}
			}
		}
	}
}


void insertion_sort(Country_mark* country_marks, int country_number)
{

	Country_mark temp_struct;
	int beg, end, middle;

	for (int i = 1; i < country_number; i++)
	{
		if (country_marks[i].mark >= country_marks[i - 1].mark)
		{
			// The corner case, when the parameter bigger than the first parameter 
			if (country_marks[i].mark >= country_marks[0].mark)
			{
				// Shift elemets to the right and insert the element
				temp_struct = country_marks[i];
				for (int j = i - 1; j >= 0; j--)
				{
					country_marks[j + 1] = country_marks[j];
				}
				country_marks[0] = temp_struct;

				continue;
			}

			beg = 0;
			end = i;

			// Using binary search determine the place of the element 
			while (true)
			{
				middle = (beg + end) / 2;

				// The valid position condition
				if (country_marks[middle].mark >= country_marks[i].mark
					&&	country_marks[middle + 1].mark <= country_marks[i].mark)
				{
					// Shift elemets to the right and insert the element
					temp_struct = country_marks[i];
					for (int j = i - 1; j >= middle + 1; j--)
					{
						country_marks[j + 1] = country_marks[j];
					}
					country_marks[middle + 1] = temp_struct;

					break;
				}
				else
				{
					if (country_marks[i].mark < country_marks[middle].mark)
						beg = middle;
					else
						end = middle;

				}
			}
		}
	}
}