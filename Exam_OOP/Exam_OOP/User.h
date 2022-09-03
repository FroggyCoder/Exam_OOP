#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include<map>
#include <experimental/filesystem>
#include <ctime>
#include<windows.h>
#include<vector>
#include <stdio.h>
#include <iomanip>

#include"MyExceptions.h"
#include"Menu.h"
#include"Encryption.h"
#include"md5.h"


using namespace std;

namespace fs = std::experimental::filesystem;

void gotoxy(int, int);
const string currentDateTime();

enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};



HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int text, int background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void SetColor(int text, ConsoleColor background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}


class User
{
public:

	User() { login = "�����������������, ��� �������"; }
	User(string login, string pass) : login(login), pass(pass) {}

	virtual void Registration() = 0;
	virtual void ShowUserGrade(string userName) = 0;

	void SetLogin(string login) { this->login = login; }

	void Remove_test(int n, string path_users_name_test, string second, string test_time); //����� �������� ����������� �����

	string GetLogin() { return this->login; }
	string GetUserLogins() { return this->folder; } //������ ����� ��� �������� ��������������� ������ �������������
	string UserSignIn();
	string GetUsersFolder() { return this->UsersFolder; }
	string GetUsersInfo() { return this->usersinfo; }
	string GetTestFolder() { return TestFolder; }
	string GetTestsName() { return TestsName; }
	string GetCategoriesFile() { return CategoriesFile; }
	string GetStudentsFile() { return StudentsFile; }

	int GetGrade(int ball_count, int n);

	~User() {};

protected:

	string login;
	string pass;
	string usersinfo = "UsersInfo.txt";
	string UsersFolder = "UsersFolder"; // ����� ��� �������� ����������� ������������ ���������
	static string folder; //��� ����� ��� ���������� ��������������� ������ ���������
	string TestFolder = "TestFolder"; // ����� ��� �������� �������� ������������
	string TestsName = "TestsName.txt"; // ���� � ������� �������� �������� ������ � ���������
	string CategoriesFile = "CategoriesFile.txt"; // ���� � ������� �������� ��������� � �������
	string StudentsFile = "StudentsFile.txt"; // ���� � ������� �������� ������ � ��� ���������	
};

string User::folder = "StudentLogin";

class Admin : public User
{
public:
	Admin() {};
	~Admin() {};

	string GetFilename() { return filename; } //������ �������� ����� � ������� � ������� �������������� �������	

	void ChangeLogin(); //��������� ������ � ������ ��������������  �������
	void DeleteUser();//�������� ������������
	void Modification();//����������� ������������
	void PrintStudents();//����� ���� ������������������ ������������� ������� ������������ ������
	void PrintStudentsFull();//����� ���� ��������� ������ ������������� ������� ������������

	void AddCategories();//���������� ��������� ��� ������
	void EditCategories();//�������������� ��������� ��� ������
	void AddTestsName();//���������� �������� ������
	void AddTests();//���������� ������

	//void ShowUserTests();

	friend bool CheckAdmin();

	void Registration() override;
	void ShowUserGrade(string userName) override;

private:
	static string filename;

};

string Admin::filename = "admin.txt";

class Student : public User
{
public:
	Student() {};
	~Student() {};
	string GetStudentName() { return this->name; }

	void Registration() override;
	void ShowUserGrade(string userName) override;

	void NewTest(); // ������ ������ ������������
	void ContinueTest(); // ���������� ������������� ���� ������������

private:
	string filename;
	string name;
	string telephone;
	string adress;
};

string User::UserSignIn()
{
	int gotx = 6;
	gotoxy(25, ++gotx);
	cout << "���� � �������\n";

	gotoxy(25, ++gotx);
	cout << "������� ����� (e-mail): ";
	cin >> this->login;

	unique_ptr<Admin> ad(new Admin);

	string path = ad->GetFilename();

	if (this->login + ".txt" == path)
	{
		system("cls");
		gotoxy(25, ++gotx);
		cout << "������� ����� (e-mail) �������������� �������: ";
		string adminlogin, md5adminlogin;
		cin >> ad->login;
		gotoxy(25, ++gotx);
		cout << "������� ������ �������������� �������: ";
		string adminpass, md5adminpass;
		cin >> ad->pass;

		md5adminlogin = md5(ad->login);
		md5adminpass = md5(ad->pass);

		ifstream afin;

		afin.exceptions(ifstream::badbit | ifstream::failbit);

		try
		{
			afin.open(path);

			int count = 0;
			while (!afin.eof())
			{
				if (count)
					getline(afin, adminpass);
				else
					getline(afin, adminlogin);
				count++;
			}

			afin.close();
		}
		catch (const ifstream::failure& ex)
		{
			gotoxy(25, ++gotx);
			cout << ex.what() << " ��� ������: " << ex.code() << "\n";
			system("pause");
		}

		if (md5adminlogin == adminlogin && md5adminpass == adminpass)
		{
			return "admin";
		}

		else
		{
			gotoxy(25, ++gotx);
			cout << "������ �����";
			return "�����������������, ��� �������";
		}
	}

	else
	{
		path = this->folder + "/" + md5(this->login) + ".txt";

		try
		{
			ifstream ufin(path);
			ufin.exceptions(ifstream::badbit | ifstream::failbit);
		}
		catch (...)
		{

			gotoxy(25, ++gotx);
			cout << "������������ � ����� ������� �� ������.";
			gotoxy(25, ++gotx);
			system("pause");
			//return this->login;
			return "�����������������, ��� �������";
		}

		ifstream ufin(path);
		ufin.exceptions(ifstream::badbit | ifstream::failbit);

		//��������� ���� �� ���������������� ���� � �������� �������

		bool ckfile = ufin.is_open();

		//bool ckfile = ufin.open(path);

		try
		{
			//��������� ���� �� ������������ � ����� �������
			if (ckfile)
			{
				gotoxy(25, ++gotx);
				cout << "������� ������: ";
				string userpass, md5userpass;
				cin >> this->pass;

				md5userpass = md5(this->pass);

				int count = -2;
				while (count)
				{
					getline(ufin, userpass);
					count++;
				}

				if (md5userpass == userpass)
				{
					gotoxy(25, ++gotx);
					cout << "�� ������� ����� � ������� ������������." << endl;
					gotoxy(25, ++gotx);
					system("pause");
					cin.ignore();
					ufin.close();
					return this->GetLogin();
				}

				else
				{
					throw ExceptionUser("�� ����� �������� ������ ������������", 4);
					//return this->login;
				}
			}
		}

		catch (const ofstream::failure& ex)
		{
			gotoxy(25, ++gotx);
			cout << ex.what() << " ��� ������: " << ex.code() << "\n";
			gotoxy(25, ++gotx);
			system("pause");
		}

		catch (ExceptionUser& ex)
		{
			gotoxy(25, ++gotx);
			cout << ex.what() << " ��� ������: " << ex.GetError() << "\n";
			gotoxy(25, ++gotx);
			system("pause");

			return "�����������������, ��� �������";
		}

		catch (...)
		{
			gotoxy(25, ++gotx);
			cout << "������ � string User::UserSignIn()";
			gotoxy(25, ++gotx);
			system("pause");
		}

		ufin.close();
		return this->login;;
	}
}

inline void Admin::Registration()
{
	int gotx = 10;
	gotoxy(25, ++gotx);
	cout << "����������� �������������� ������� ������������";

	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		string path = Admin::filename;

		gotoxy(25, ++gotx);
		cout << "����� ��������������:";
		SetColor(Red, Black);
		cout << "admin";
		SetColor(15, Black);
		string login = "admin";
		//cin >> login;
		string md5login;
		md5login = md5(login);

		gotoxy(25, ++gotx);
		cout << "������� ������ ��������������: ";
		string pass;
		cin >> pass;
		string md5pass;
		md5pass = md5(pass);

		fout.open(path);
		fout << md5login << "\n";
		fout << md5pass;
		fout.close();
	}

	catch (const ifstream::failure& ex)
	{
		gotoxy(25, ++gotx);
		cout << ex.what() << " ��� ������: " << ex.code() << "\n";
		system("pause");
	}

	catch (...)
	{
		gotoxy(25, ++gotx);
		cout << "Catch Admin::RegistrationAdmin()\n";
	}

	gotx++;
	gotoxy(25, ++gotx);
	cout << "������������� ������� ���������������. ������ ����� � ������� � ��������� �������";
	gotx++;
	gotoxy(25, ++gotx);
	system("pause");
}

inline void Admin::ChangeLogin()
{
	int gotx = 6;
	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		gotoxy(25, ++gotx);
		cout << "��������� ������ � ������ �������������� ������� ������������\n";
		string path = Admin::filename;

		gotoxy(25, ++gotx);
		cout << "������� ����� ����� ��������������: ";
		string login;
		cin >> login;
		string md5login;
		md5login = md5(login);

		gotoxy(25, ++gotx);
		cout << "������� ����� ������ ��������������: ";
		string pass;
		cin >> pass;
		string md5pass;
		md5pass = md5(pass);

		//��������� � ���� ��������� ���� � ������
		fout.open(path);
		fout << md5login << "\n";
		fout << md5pass;
		fout.close();

		gotoxy(25, ++gotx);
		cout << "�������� �������.";
		gotoxy(25, ++gotx);
		system("pause");
	}

	catch (const ofstream::failure& ex)
	{
		gotoxy(25, ++gotx);
		cout << ex.what() << " ��� ������: " << ex.code();
		gotoxy(25, ++gotx);
		system("pause");
	}

	catch (...)
	{
		gotoxy(25, ++gotx);
		cout << "Catch Admin::ChangeLogin()";
		gotoxy(25, ++gotx);
		system("pause");
	}
}

inline void Admin::DeleteUser()
{
	map <string, string> mp;
	map <string, string> mp_users;
	string path = this->GetUsersFolder() + "/" + this->GetStudentsFile();

	//����� ��� �������� ������ ������ � ����� ����� �� ������� �������������
	string path2 = this->GetUsersFolder() + "/" + this->GetUsersInfo();

	string key, data, key_u, data_u;
	ifstream ifs;
	int gotx = 6;
	system("cls");

	gotoxy(25, ++gotx);
	cout << "�������� ������������� ������� ������������";
	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			SetColor(Red, Black);
			cout << "������ �������� ����� c ������� ���������";
			SetColor(15, Black);

			ifs.close();
			gotoxy(25, ++gotx);
			system("pause");
		}
		else
		{
			while (!ifs.eof())
			{
				key = "";
				data = "";
				getline(ifs, key);
				if (key != "")
					getline(ifs, data);

				if (key != "")
					mp[key] = data;
			}
			ifs.close();

			ifs.open(path2);

			if (!ifs.is_open())
			{
				gotoxy(25, ++gotx);
				SetColor(Red, Black);
				cout << "������ �������� ����� Admin::DeleteUser() path2";
				SetColor(15, Black);
				ifs.close();
				gotoxy(25, ++gotx);
				system("pause");
			}

			else
			{
				while (!ifs.eof())
				{
					key_u = "";
					data_u = "";
					getline(ifs, key_u);
					if (key_u != "")
						getline(ifs, data_u);

					if (key_u != "")
						mp_users[key_u] = data_u;
				}
				ifs.close();

				this->PrintStudents();

				gotoxy(25, ++gotx);
				cout << "������� ��������� ����� �������� ��� ��������: ";
				cin >> key;

				auto it = mp.find(key);

				if (it != mp.end())
				{
					system("cls");
					int gotx = 6;
					gotoxy(25, ++gotx);
					cout << "�������� ������������� ������� ������������";

					gotoxy(25, ++gotx);
					cout << "������� ������ � ����: ";
					SetColor(10, Black);
					cout << it->first << " " << it->second << endl;
					SetColor(15, Black);
					gotoxy(25, ++gotx);
					cout << "����������� ����� �������� ��� ��������: ";
					string login;
					cin >> data;
					login = this->GetUserLogins() + "/" + md5(data) + ".txt";

					string userfolder = this->GetUsersFolder() + "/" + data + "/";

					if (remove(login.c_str()) != 0)
					{
						gotoxy(25, ++gotx);
						SetColor(Red, Black);
						perror("������ �������� ����� � ������� ��������");
						SetColor(15, Black);
						gotoxy(25, ++gotx);
						system("pause");
					}

					else
					{
						gotoxy(25, ++gotx);
						puts("������� �������� �������� ������� ���������");

						if (remove(path.c_str()) != 0)
						{
							gotoxy(25, ++gotx);
							SetColor(Red, Black);
							perror("������ �������� ����� � ����� ���������");
							SetColor(15, Black);
							gotoxy(25, ++gotx);
							system("pause");
						}

						else
						{
							gotoxy(25, ++gotx);
							puts("������� �������� ������� ����� ���� ��������� ������� ���������");
							mp.erase(key);
							mp_users.erase(key);

							ofstream ofs;
							ofs.open(path, ostream::app);

							for (auto it = mp.begin(); it != mp.end(); ++it)
							{
								ofs << it->first << "\n" << it->second << "\n";
							}

							ofs.close();

							ofs.open(path2);

							for (auto it = mp_users.begin(); it != mp_users.end(); ++it)
							{
								ofs << it->first << "\n" << it->second << "\n";
							}

							ofs.close();

							gotoxy(25, ++gotx);
							system("pause");
						}

						std::uintmax_t n = fs::remove_all(userfolder.c_str());
					}
				}

				else
				{
					system("cls");
					gotoxy(25, ++gotx);
					SetColor(Red, Black);
					cout << "C������ � ��������� ������� �������� �� ������ � ����.";
					SetColor(15, Black);
					gotoxy(25, ++gotx);
					system("pause");
				}
			}
		}
	}

	catch (...)
	{
		system("cls");
		gotoxy(25, ++gotx);
		cout << "Catch Admin::DeleteUser()";
		gotoxy(25, ++gotx);
		system("pause");
	}
}

