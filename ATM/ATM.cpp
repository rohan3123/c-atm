// ATM.cpp : This file contains the 'main' function. Program execution begins and ends there.
//by Rohan Gazi (N0989380)
#include <fstream>
#include <iostream>
#include <limits>
#include <string.h>
#include <list>

using namespace std;


//////////////////////////////////////////////////////////////////classes//////////////////////////////////////////////////////////////////////////////

//class transfer to pay other user
class Transfer
{
	private:
		char name[30];
		int password;
		double balance;
		int sortcode;

	public:
		//getters 
		char* names(void)
		{
			// Return username
			return name;
		}

		int sortcodes(void)
		{
			// Return the sortcode

			return sortcode;
		}

		// Function to display the data
		void showData(void)
		{
			cout << "Name:" << name <<", Sortcode:" << sortcode << endl;
		}

		//Memeber methods
		void deposit(int, double);
		int searchUser(int);
};

//class profile to get user details
class Profile
{
	private:
		char name[30];
		int password;
		double balance;
		int sortcode;

	public:
		//getters 
		char* names(void)
		{
			// Return username
			return name;
		}

		int passwords(void)
		{
			// Return the password
			return password;
		}

		int sortcodes(void)
		{
			// Return the sortcode
			return sortcode;
		}


		// Function to get the data
		void getUser(void)
		{
			//user enters details to create account
			cout << "\n----------------CREATE ACCOUNT----------------\n" << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nEnter name:";
			cin.getline(name, 30);
			while (true)
			{
				cout << "\nEnter number pin password:";
				cin >> password;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (cin)
				{
					break;
				}
				else
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\nError enter in digit password" << endl;
				}
			}

			//sets balance to 0
			balance = 0;

			//generates sort code for user
			srand(time(NULL));
			int first = rand() % 10;
			int second = rand() % 10;
			int third = rand() % 10;
			int forth = rand() % 10;

			int sort = first << second << third << forth;

			cout << "\nSortcode: " << sort << endl;

			sortcode = sort;
		}

		// Function to display the data
		void showUser(void)
		{
			cout << "Name:" << name << ", Password:" << password << ", Balance:" << balance << ", Sortcode:" << sortcode << endl;
		}

		// Member methods
		int createAccount();
		int viewUsers();
		void deposit(char*, double);
		void withdraw(char*, double);
		int searchUser(char*, int);
		int searchAllUser(char*);
		void transfer(char*, double);
		void interest(char*);
		void pay(double);
};

//////////////////////////////////////////////////////////////class functions////////////////////////////////////////////////////////////////////////////

//function to create a new account
int Profile::createAccount()
{
	Profile profile;

	ofstream file;

	// Open file in write mode
	file.open("profiles.txt", ios::out | ios::app);
	if (!file)
	{
		cout << "Error in creating file" << endl;
		return 0;
	}

	// get account details from user
	profile.getUser();

	// Write into file
	file.write((char*)&profile, sizeof(profile));

	file.close();

	return 0;
}

//view all users
int Profile::viewUsers()
{

	Profile profile;

	//initlizse the file
	ifstream myFile;

	//Open file in read mode
	myFile.open("profiles.txt", ios::in);
	//error checking
	if (!myFile)
	{
		cout << "Error in opening file..";
		return 0;
	}
	//read data from file
	myFile.read((char*)&profile, sizeof(profile));
	while (!myFile.eof())
	{
		//show user data
		profile.showUser();
		myFile.read((char*)&profile, sizeof(profile));
	}

	myFile.close();
	return 0;
}

//deposit money 
void Profile::deposit(char* uname, double b)
{

	Profile profile;
	fstream file, temp;

	//opens file
	file.open("profiles.txt", ios::in | ios::out | ios::ate);
	temp.open("temp.txt", ios::out | ios::app);
	file.seekg(0);

	//read file and puts its in class first time
	file.read((char*)&profile, sizeof(profile));

	// Till end of the file
	while (!file.eof())
	{
		if (!strcmp(profile.names(), uname))
		{
			profile.balance = profile.balance + b;
			temp.write((char*)&profile, sizeof(profile));
			
		}
		else 
		{
			temp.write((char*)&profile, sizeof(profile));
		}
		//reads from file and puts it in lcass again 
		file.read((char*)&profile, sizeof(profile));
	}
	file.close();
	temp.close();

	const int ret = remove("profiles.txt");
	//overwrites file to save new data in
	if (ret == 0)
	{
		const int ren = rename("temp.txt", "profiles.txt");

		if (ren == 0)
		{
			cout << "\nOverwrite successful" << endl;
		}
		else
		{
			cout << "Error failed overwrite.." << endl;
		}
	}
	else
	{
		cout << "\nError with removing file.." << endl;
	}
}

//withdrawing money
void Profile::withdraw(char* uname, double b)
{

	Profile profile;
	fstream file, temp;
	file.open("profiles.txt", ios::in | ios::out | ios::ate);
	//open temperory file
	temp.open("temp.txt", ios::out | ios::app);
	file.seekg(0);
	file.read((char*)&profile, sizeof(profile));

	while (!file.eof())
	{
		if (!strcmp(profile.names(), uname))
		{
			if (profile.balance < b)
			{
				cout << "\nInsufficient balance to transfer";
			}
			else
			{
				profile.balance = profile.balance - b;
				temp.write((char*)&profile, sizeof(profile));
				
			}
		}
		else
		{
			temp.write((char*)&profile, sizeof(profile));
		}
		file.read((char*)&profile, sizeof(profile));
	}

	file.close();
	temp.close();

	int ret = remove("profiles.txt");
	//overwrites file to save new data in
	if (ret == 0) 
	{
		int ren = rename("temp.txt", "profiles.txt");

		if (ren == 0)
		{
			cout << "\nOverriten successful" << endl;
		}
		else
		{
			cout << "Error failed overwrite.." << endl;
		}
	}
	else
	{
		cout << "\nError with removing file.." << endl;
	}
}

//intrest for money 
void Profile::interest(char* uname)
{
	Profile profile;
	fstream file, temp;

	file.open("profiles.txt", ios::in | ios::out | ios::ate);
	temp.open("temp.txt", ios::out | ios::app);
	file.seekg(0);
	file.read((char*)&profile, sizeof(profile));

	// Till end of the file
	while (!file.eof())
	{
		if (!strcmp(profile.names(), uname))
		{
			string choice;
			//interest rate
			double percentage = 3.3;
			double total;
			cout << "\n----------------INTEREST----------------\n";
			cout << "\nCurrent interest rate is: " << percentage << "%";
			cout << "\nEnter wait period\n1. 1 month\n2. 2 months\n3. 4 months\n:";
			cin >> choice;

			//calculates interest
			if (choice == "1")
			{
				total = profile.balance * (percentage / 100);
				profile.balance = profile.balance + total;
				cout << "\nInterest gained: " << total;
				cout << "\nBalance is: " << profile.balance;
				temp.write((char*)&profile, sizeof(profile));
			}
			else if (choice == "2")
			{
				total = (profile.balance * (percentage) / 100) * 2;
				profile.balance = profile.balance + total;
				cout << "\nInterest gained: " << total;
				cout << "\nBalance is: " << profile.balance;
				temp.write((char*)&profile, sizeof(profile));
			}
			else if (choice == "3")
			{
				total = (profile.balance * (percentage) / 100) * 4;
				profile.balance = profile.balance + total;
				cout << "\nInterest gained: " << total;
				cout << "\nBalance is: " << profile.balance;
				temp.write((char*)&profile, sizeof(profile));
			}
			else
			{
				cout << "\nEnter in valid input" << endl;;
			}

		}
		else
		{
			temp.write((char*)&profile, sizeof(profile));
		}
		file.read((char*)&profile, sizeof(profile));
	}
	file.close();
	temp.close();

	const int ret = remove("profiles.txt");
	//overwrites file to save new data in
	if (ret == 0)
	{
		const int ren = rename("temp.txt", "profiles.txt");

		if (ren == 0)
		{
			cout << "\nOverwrite successful" << endl;
		}
		else
		{
			cout << "Error failed overwrite.." << endl;
		}
	}
	else
	{
		cout << "\nError with removing file.." << endl;
	}
}

//search for specific user
int Profile::searchUser(char* uname, int pass)
{
	Profile profile;
	fstream myFile;

	myFile.open("profiles.txt", ios::in);
	if (!myFile)
	{
		cout << "Error in opening file..";
		return 0;
	}

	//reads data from file and puts it in class
	myFile.read((char*)&profile, sizeof(profile));

	//itarrate through file
	while (!myFile.eof())
	{
		//if data matches up with data entered
		if (!strcmp(profile.names(), uname))
		{
			if (profile.passwords() == pass)
			{
				return 1;
			}
		}
		//puts next line into class to ittrate through 
		myFile.read((char*)&profile, sizeof(profile));
	}

	myFile.close();
	return 0;
}

//search users
int Profile::searchAllUser(char* uname)
{
	Profile profile;
	fstream myFile;

	// Open the file
	myFile.open("profiles.txt", ios::in);
	if (!myFile)
	{
		cout << "Error in opening file..";
		return 0;
	}
	myFile.read((char*)&profile, sizeof(profile));
	while (!myFile.eof())
	{
		if (!strcmp(profile.names(), uname))
		{
			profile.showUser();
			return 0;
		}
		myFile.read((char*)&profile, sizeof(profile));
	}

	myFile.close();
	return 0;
}

//transfer money to other user
void Profile::transfer(char* uname, double b)
{
	Profile profile;
	fstream file, temp;
	file.open("profiles.txt", ios::in | ios::out | ios::ate);
	//open temperory file
	temp.open("temp.txt", ios::out | ios::app);
	file.seekg(0);
	file.read((char*)&profile, sizeof(profile));

	while (!file.eof())
	{
		if (!strcmp(profile.names(), uname))
		{
			if (profile.balance < b)
			{
				cout << "\nInsufficient balance to transfer";
			}
			else
			{
				profile.balance = profile.balance - b;
				temp.write((char*)&profile, sizeof(profile));
				
			}
		}
		else
		{
			temp.write((char*)&profile, sizeof(profile));
		}
		file.read((char*)&profile, sizeof(profile));
	}

	file.close();
	temp.close();

	//output balance
	cout << "\nBalance:" << profile.balance << endl;

	int ret = remove("profiles.txt");
	//overwrites file to save new data in
	if (ret == 0)
	{
		int ren = rename("temp.txt", "profiles.txt");

		if (ren == 0)
		{
			cout << "\nOverwriten successful" << endl;
		}
		else
		{
			cout << "Error failed overwrite.." << endl;
		}
	}
	else
	{
		cout << "\nError with removing file.." << endl;
	}
}

//search for user to transfer money
int Transfer::searchUser(int sort)
{
	Transfer transfer;
	fstream myFile;

	myFile.open("profiles.txt", ios::in);
	if (!myFile)
	{
		cout << "Error in opening file..";
		return 0;
	}

	myFile.read((char*)&transfer, sizeof(transfer));
	while (!myFile.eof())
	{

		if (transfer.sortcodes() == sort)
		{
			cout << "User found: " << transfer.names() << " , Sortcode: " << transfer.sortcodes() << endl;
			return 1;
		}
		myFile.read((char*)&transfer, sizeof(transfer));
	}

	myFile.close();
	return 0;
}

//deposit money 
void Transfer::deposit(int sort, double b)
{

	Transfer transfer;
	fstream file, temp;

	file.open("profiles.txt", ios::in | ios::out | ios::ate);
	temp.open("temp.txt", ios::out | ios::app);
	file.seekg(0);
	file.read((char*)&transfer, sizeof(transfer));

	// Till end of the file
	while (!file.eof())
	{
		if (transfer.sortcodes() == sort)
		{
			transfer.balance = transfer.balance + b;
			cout << "\nTransfer sucessful" << endl;
			temp.write((char*)&transfer, sizeof(transfer));
		}
		else 
{
			temp.write((char*)&transfer, sizeof(transfer));
		}
		file.read((char*)&transfer, sizeof(transfer));
	}
	file.close();
	temp.close();

	const int ret = remove("profiles.txt");
	//overwrites file to save new data in
	if (ret == 0)
	{
		const int ren = rename("temp.txt", "profiles.txt");

		if (ren == 0)
		{
			cout << "\nOverwrite successful" << endl;
		}
		else
		{
			cout << "Error failed overwrite.." << endl;
		}
	}
	else
	{
		cout << "\nError with removing file.." << endl;
	}
}

////////////////////////////////////////////////////////////////function code/////////////////////////////////////////////////////////////////////////

//function that holds log in menu
void login()
{
	//vairables
	Profile profile;
	char uname[30];
	int pass;
	string choice;
	int found = 0;

	//list for history
	list<double> transactions;

	cout << "\n----------------LOGIN----------------\n" << endl;
	//enter in log in details
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "Enter name:";
	cin.getline(uname, 30);
	//error check for password pin
	while (true)
	{
		cout << "\nEnter password:";
		cin >> pass;
		if (cin)
		{
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nError enter in digt password" << endl;
		}

	}

	//finds account
	found = profile.searchUser(uname, pass);

	//takes user to user menu
	if (found)
	{
		//menu loop 
		while (true)
		{
			cout << "\n----------------USER MENU----------------\n" << endl;
			cout << "\n1. Deposit     6. Interest\n2. Withdraw    7. Transfer\n3. History\n4. View account\n5. Exit \n:";
			cin >> choice;

			if (choice == "1")
			{
				cout << "\n----------------DEPOSIT----------------\n" << endl;
				double b;
				cout << "\nEnter amount to deposit:";
				cin >> b;

				//error checking
				if (!cin)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\nEnter in valid ammount" << endl;
				}
				else
				{
					//puts ammount in list
					transactions.push_front(b);
					//deposits to account
					profile.deposit(uname, b);
				}
			}
			else if (choice == "2")
			{
				cout << "\n----------------WITHDRAW----------------\n" << endl;
				double b;
				cout << "\nEnter amount to deposit:";
				cin >> b;

				//error checking
				if (!cin)
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "\nEnter in valid ammount" << endl;
				}
				else
				{
					//puts ammount in list
					transactions.push_front(-b);
					//withdraws from account
					profile.withdraw(uname, b);
				}

			}
			else if (choice == "3")
			{
				cout << "\n----------------HISTORY----------------\n" << endl;
				cout << "\nHistory of account activity:" << endl;
				//itarattes through list and outputs
				for (auto v : transactions)
				{
					cout << v << ", ";
				}
			}

			else if (choice == "4")
			{
				cout << "\n----------------VIEW ACCOUNT----------------\n" << endl;
				profile.searchAllUser(uname);
			}
			else if (choice == "5")
			{
				break;
			}
			else if (choice == "6")
			{
				profile.interest(uname);
			}
			else if (choice == "7")
			{
				Transfer transfer;

				int sort;
				//error handeling
				while (true)
				{
					cout << "\n----------------TRANSFER----------------\n" << endl;
					cout << "\nEnter in sortcode:";
					cin >> sort;

					if (!cin)
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "\nError enter valid sortcode" << endl;
					}
					else
					{
						//locates user to transfer money to
						int found1 = transfer.searchUser(sort);

						//takes user to transfer
						if (found1 == 1)
						{
							cout << "\nTransfer to this user?\n1. Yes\n2. No\n:";
							string choice;
							cin >> choice;
							if (choice == "1")
							{
								while (true)
								{
									double b;
									cout << "\nEnter amount to transfer:";
									cin >> b;

									//error checking
									if (!cin)
									{
										cin.clear();
										cin.ignore(numeric_limits<streamsize>::max(), '\n');
										cout << "\nEnter in valid ammount" << endl;
									}
									else
									{
										transactions.push_front(-b);
										profile.transfer(uname, b);
										transfer.deposit(sort, b);
										break;
									}
								}
							}
							else if (choice == "2")
							{
								break;
							}
							else
							{
								cout << "\nEnter in valid input" << endl;
								break;
							}
						}
						else
						{
							cout << "\nUnable to locate user" << endl;
							break;
						}
						break;
					}
				}
			}
			else
			{
				cout << "Enter in valid input" << endl;
			}
		}
	}
	else
	{
		cout << "\nNo account found" << endl;
	}
}

//admin account
void admin()
{
	Profile profile;

	int pass;
	string choice;

	cout << "\n----------------ADMIN----------------\n" << endl;
	cout << ("\nthe password is: 1234") << endl;
	cout << "\nEnter password:" << endl;
	cin >> pass;

	//admin password
	if (pass == 1234)
	{
		//menu loop 
		while (true)
		{
			cout << "\n----------------ADMIN----------------\n" << endl;
			cout << "\n1. View all accounts \n2. Exit\n:";
			cin >> choice;
			if (choice == "1")
			{
				cout << "\n----------------VIEW ALL ACCOUNTS----------------\n" << endl;
				profile.viewUsers();
			}
			else if (choice == "2")
			{
				//break loop
				break;
			}
			else
			{
				cout << "Enter in valid input" << endl;
			}
		}
	}
	else {
		cout << "\nDetails are incorrect" << endl;
		cin.get();
	}
}

//starting function
void startMenu()
{
	Profile profile;

	//menu loop
	while (true)
	{
		// System("cls");
		cout << "\n----------------MENU----------------\n1. Login\n2. Create account \n3. Admin \n4. Exit\n:";
		string choice;
		cin >> choice;

		if (choice == "1")
		{
			login();
		}
		else if (choice == "2")
		{
			profile.createAccount();
		}
		else if (choice == "3")
		{
			admin();
		}
		else if (choice == "4")
		{
			break;
		}
		//error handling :)
		else
		{
			cout << "\nEnter in correct value" << endl;
		}
	}
}

//////////////////////////////////////////////////////////////////main//////////////////////////////////////////////////////////////////////////////

//starting code
int main()
{
	//Main start up menu
	startMenu();

	return 0;
}
