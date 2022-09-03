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

inline void Admin::ChangeLogin()
{
	int gotx = 6;
	ofstream fout;
	fout.exceptions(ofstream::badbit | ofstream::failbit);

	try
	{
		gotoxy(25, ++gotx);
		cout << "Изменение логина и пароля администратора системы тестирования\n";
		string path = Admin::filename;

		gotoxy(25, ++gotx);
		cout << "Введите новый логин администратора: ";
		string login;
		cin >> login;
		string md5login;
		md5login = md5(login);

		gotoxy(25, ++gotx);
		cout << "Введите новый пароль администратора: ";
		string pass;
		cin >> pass;
		string md5pass;
		md5pass = md5(pass);

		//сохраняем в файл введенный логи и пароль
		fout.open(path);
		fout << md5login << "\n";
		fout << md5pass;
		fout.close();

		gotoxy(25, ++gotx);
		cout << "Операция успешна.";
		gotoxy(25, ++gotx);
		system("pause");
	}

	catch (const ofstream::failure& ex)
	{
		gotoxy(25, ++gotx);
		cout << ex.what() << " Код ошибки: " << ex.code();
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

	//папка для хранения личных данных в одном файле по каждому пользоввателю
	string path2 = this->GetUsersFolder() + "/" + this->GetUsersInfo();

	string key, data, key_u, data_u;
	ifstream ifs;
	int gotx = 6;
	system("cls");

	gotoxy(25, ++gotx);
	cout << "Удаление пользователей системы тестирования";
	try
	{
		ifs.open(path);

		if (!ifs.is_open())
		{
			gotoxy(25, ++gotx);
			SetColor(Red, Black);
			cout << "Ошибка открытия файла c данными студентов";
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
				cout << "Ошибка открытия файла Admin::DeleteUser() path2";
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
				cout << "Введите мобильный номер студента для удаления: ";
				cin >> key;

				auto it = mp.find(key);

				if (it != mp.end())
				{
					system("cls");
					int gotx = 6;
					gotoxy(25, ++gotx);
					cout << "Удаление пользователей системы тестирования";

					gotoxy(25, ++gotx);
					cout << "Студент найден в базе: ";
					SetColor(10, Black);
					cout << it->first << " " << it->second << endl;
					SetColor(15, Black);
					gotoxy(25, ++gotx);
					cout << "Подтвердите логин студента для удаления: ";
					string login;
					cin >> data;
					login = this->GetUserLogins() + "/" + md5(data) + ".txt";

					string userfolder = this->GetUsersFolder() + "/" + data + "/";

					if (remove(login.c_str()) != 0)
					{
						gotoxy(25, ++gotx);
						SetColor(Red, Black);
						perror("Ошибка удаления файла с логином студента");
						SetColor(15, Black);
						gotoxy(25, ++gotx);
						system("pause");
					}

					else
					{
						gotoxy(25, ++gotx);
						puts("Оперция удаления студента успешно завершена");

						if (remove(path.c_str()) != 0)
						{
							gotoxy(25, ++gotx);
							SetColor(Red, Black);
							perror("Ошибка удаления файла с базой студентов");
							SetColor(15, Black);
							gotoxy(25, ++gotx);
							system("pause");
						}

						else
						{
							gotoxy(25, ++gotx);
							puts("Оперция удаления старого файла базы студентов успешно завершена");
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
					cout << "Cтудент с указанным номером телефона не найден в базе.";
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

