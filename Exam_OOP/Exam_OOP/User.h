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
	Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Brown = 6, LightGray = 7, DarkGray = 8,
	LightBlue = 9, LightGreen = 10, LightCyan = 11, LightRed = 12, LightMagenta = 13, Yellow = 14, White = 15
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

	User() { login = "Зарегистрируйтесь, или войдите"; }
	User(string login, string pass) : login(login), pass(pass) {}

	virtual void Registration() = 0;
	virtual void ShowUserGrade(string userName) = 0;

	void SetLogin(string login) { this->login = login; }

	void Remove_test(int n, string path_users_name_test, string second, string test_time); //метод удаления пройденного теста

	string GetLogin() { return this->login; }
	string GetUserLogins() { return this->folder; } //геттер папки для хранения регистрационных данных пользователей
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
	string UsersFolder = "UsersFolder"; // папка для хранения реузльтатов тестирования студентов
	static string folder; //имя папки для сохранения регистрационных данных студентов
	string TestFolder = "TestFolder"; // папка для хранения вопросов тестирования
	string TestsName = "TestsName.txt"; // файл в котором хранятся названия тестов в категории
	string CategoriesFile = "CategoriesFile.txt"; // файл в котором хранятся категории с тестами
	string StudentsFile = "StudentsFile.txt"; // файл в котором хранятся логины и ФИО студентов	
};

string User::folder = "StudentLogin";

class Admin : public User
{
public:
	Admin() {};
	~Admin() {};

	string GetFilename() { return filename; } //геттер названия файла с логином и паролем администратора системы	

	void ChangeLogin(); //изменение логина и пароля администратора  системы
	void DeleteUser();//удаление пользовтаеля
	void Modification();//модификация пользовтаеля
	void PrintStudents();//вывод всех зарегистрированных пользователей системы тестирования кратко
	void PrintStudentsFull();//вывод всех анктетных данных пользователей системы тестирования

	void AddCategories();//добавление категорий для тестов
	void EditCategories();//редкатирование категорий для тестов
	void AddTestsName();//добавление названий тестов
	void AddTests();//добавление тестов

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

	void NewTest(); // запуск нового тестирования
	void ContinueTest(); // продолжить незавершенный тест тестирования

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
	cout << "Вход в систему\n";

	gotoxy(25, ++gotx);
	cout << "Введите логин (e-mail): ";
	cin >> this->login;

	unique_ptr<Admin> ad(new Admin);

	string path = ad->GetFilename();

	if (this->login + ".txt" == path)
	{
		system("cls");
		gotoxy(25, ++gotx);
		cout << "Введите логин (e-mail) администратора системы: ";
		string adminlogin, md5adminlogin;
		cin >> ad->login;
		gotoxy(25, ++gotx);
		cout << "Введите пароль администратора системы: ";
		string adminpass, md5adminpass;
		cin >> ad->pass;

		md5adminlogin = md5(ad->login);
		md5adminpass = md5(ad->pass);

		ifstream afin;

		afin.exceptions(ifstream::badbit | ifstream::failbit);

		try
		{
			afin.open(path);
			//afin >> adminlogin;
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
			cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
			system("pause");
		}

		if (md5adminlogin == adminlogin && md5adminpass == adminpass)
		{
			return "admin";
		}

		else
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка входа";
			return "Зарегистрируйтесь, или войдите";
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
			cout << "Пользователь с таким логином не найден.";
			gotoxy(25, ++gotx);
			system("pause");
			//return this->login;
			return "Зарегистрируйтесь, или войдите";
		}

		ifstream ufin(path);
		ufin.exceptions(ifstream::badbit | ifstream::failbit);

		//проверяем есть ли пользовательский файл с веденным логином

		bool ckfile = ufin.is_open();

		//bool ckfile = ufin.open(path);

		try
		{
			//проверяем есть ли пользователь с таким логином
			if (ckfile)
			{
				gotoxy(25, ++gotx);
				cout << "Введите пароль: ";
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
					cout << "Вы успешно вошли в систему тестирования." << endl;
					gotoxy(25, ++gotx);
					system("pause");
					cin.ignore();
					ufin.close();
					return this->GetLogin();
				}

				else
				{
					throw ExceptionUser("Вы ввели неверный пароль пользователя", 4);
					//return this->login;
				}
			}
		}

		catch (const ofstream::failure& ex)
		{
			gotoxy(25, ++gotx);
			cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
			gotoxy(25, ++gotx);
			system("pause");
		}

		catch (ExceptionUser& ex)
		{
			gotoxy(25, ++gotx);
			cout << ex.what() << " Код ошибки: " << ex.GetError() << "\n";
			gotoxy(25, ++gotx);
			system("pause");

			return "Зарегистрируйтесь, или войдите";
		}

		catch (...)
		{
			gotoxy(25, ++gotx);
			cout << "Ошибка в string User::UserSignIn()";
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
	cout << "Регистрация администратора системы тестирования";

	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		string path = Admin::filename;

		gotoxy(25, ++gotx);
		cout << "Логин администратора:";
		SetColor(Red, Black);
		cout << "admin";
		SetColor(15, Black);
		string login = "admin";
		//cin >> login;
		string md5login;
		md5login = md5(login);

		gotoxy(25, ++gotx);
		cout << "Введите пароль администратора: ";
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
		cout << ex.what() << " Код ошибки: " << ex.code() << "\n";
		system("pause");
	}

	catch (...)
	{
		gotoxy(25, ++gotx);
		cout << "Catch Admin::RegistrationAdmin()\n";
	}

	gotx++;
	gotoxy(25, ++gotx);
	cout << "Администратор успешно зарегистрирован. Можете войти в систему с веденными данными";
	gotx++;
	gotoxy(25, ++gotx);
	system("pause");
}
