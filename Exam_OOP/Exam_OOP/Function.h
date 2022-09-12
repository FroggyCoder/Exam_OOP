#pragma once

#include <iostream>
#include <string>
#include <exception>
#include"MyExceptions.h"
#include"Menu.h"
#include"User.h"

using namespace std;

bool CheckAdmin()
{
	try
	{
		string path = Admin::filename;
		ifstream fin(path);

		if (fin.fail())
		{
			throw ExceptionAdmin("���� ������������ �� ������. ", 1);
		}

		fin.close();
		return true;
	}

	catch (ExceptionAdmin& ex)
	{
		gotoxy(25, 7);
		cout << ex.what() << "��� ������: " << ex.GetError() << "\n";
		gotoxy(25, 8);
		SetColor(Red, Black);
		cout << "� ������� ��� ������������������� ��������������.\n";
		SetColor(15, Black);
		gotoxy(25, 9);
		cout << "�������� �����������" << "\n";

		return false;
	}

	catch (...)
	{
		gotoxy(25, 16);
		cout << "������ � ������� bool CheckAdmin()";
	}
}

//------------------------------------------------

string* MenuItems(string user); // �������, ������� ���������� ��� ������ ����.

void gotoxy(int, int);

void ChangeCursorStatus(bool);

void AdminFun1(); // #1. ��������, ��������, ����������� �������������.
void AdminFun2(); // #2. �������� ����������."
void AdminFun3(); // #3. ���������� �������������.";
void AdminFun4(); // #4. ��������� ������ � ������ ��������������";

void MenuFun20(); //  ����� ���� ����� ������������ � �������
void MenuFun21(); // ����� ���� ����������� ������������ � �������

void StudentFun1(); //#1. ����� ����� ������������.";
void StudentFun2(); //#2. ���������� ������������
void StudentFun3(); //#3 ����������� ������ ���������� ������

void ExitOption(); // ������� ������


//����������� �������� ����� ������������
class UserName
{
public:

	UserName() {};
	UserName(string user, string fio, int ItemCount) : user(user), fio(fio), ItemCount(ItemCount) {}
	~UserName() {};

	void SetUser(string user) { this->user = user; }
	void SetItemCount(int ItemCount) { this->ItemCount = ItemCount; }
	void SetFIO(string fio) { this->fio = fio; }

	string GetUser() { return this->user; }
	string GetFIO()
	{
		unique_ptr<Student> student(new Student);

		string path = student->GetUserLogins() + "/" + md5(user) + ".txt";

		ifstream sfin;
		sfin.exceptions(ifstream::badbit | ifstream::failbit);

		if (user != "�����������������, ��� �������" && user != "admin")
		{
			try
			{
				sfin.open(path);
				int count = -3;
				while (count)
				{
					getline(sfin, fio);
					count++;
				}

				sfin.close();

			}
			catch (const ifstream::failure& ex)
			{
				gotoxy(25, 13);
				cout << ex.what() << "\n��� ������: " << ex.code() << "\n";
				system("pause");
			}
		}

		return fio;
	};

private:

	string user = "�����������������, ��� �������";
	string fio = "Sign in / Sign up";
	int ItemCount = 3;
};

unique_ptr<UserName> userName(new UserName);


void UseMenu(string user, int ItemCount)
{
	ChangeCursorStatus(false);

	int MenuChoice = 1; // ��� ���������� �������� ������� �������. 
	char key; // ��� ����� ����� (������� �����, ������� ���� � �. �.);

	typedef void (*TMenuOption)();

	TMenuOption* MenuOption = new TMenuOption[ItemCount];// ������ ���������� �� ������� (������������).

	if (user == "admin")
	{
		MenuOption[0] = AdminFun1;
		MenuOption[1] = AdminFun2;
		MenuOption[2] = AdminFun3;
		MenuOption[3] = AdminFun4;
		MenuOption[4] = ExitOption;
	}

	else if (user == "sign")
	{
		MenuOption[0] = MenuFun20;
		MenuOption[1] = MenuFun21;
		MenuOption[2] = ExitOption;
	}

	else
	{
		MenuOption[0] = StudentFun1;
		MenuOption[1] = StudentFun2;
		MenuOption[2] = StudentFun3;
		MenuOption[3] = ExitOption;
	}


	while (1) // ���� ���� �� ���������, ������� ��� ����� ������� ������.
	{

		for (int i = 0; i < ItemCount; i++) // ����� ����.
		{

			gotoxy(23, 7 + i);
			MenuChoice == i + 1 ? cout << " -> " : cout << "    ";

			cout << MenuItems(user)[i] << endl; // ����� �������� ����
		}

		gotoxy(0, 0);
		cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";

		key = _getch();

		switch (key) // �������� ���������� �����.
		{
		case '\r': // ���� ��������� ���� - �Enter�.
			try
			{
				(*MenuOption[MenuChoice - 1])(); // ������� ������� ������� 'cursor-1' �
											   //    ������ "��������� �� �������".
			}
			catch (...)
			{
				cout << "catch (...) Function.h";
			}

			break;

		case 'P': // 'up arrow' 
			MenuChoice++;
			if (MenuChoice > ItemCount)
				MenuChoice = 1;
			break;

		case 'H': // 'down arrow'
			MenuChoice--;
			if (MenuChoice < 1)
				MenuChoice = ItemCount;
			break;

		case 27:// (Esc)
			try { (*MenuOption[ItemCount - 1])(); }
			catch (...) {}
			break;
		default:
			if (key >= '1' && key <= char(ItemCount + '0'))

			{
				try { (*MenuOption[int(key) - '0' - 1])(); }
				catch (...) {}
			}
		}
	}

	delete[]MenuOption;
}

//-----------------------------------------------------------------------------------------------
string* MenuItems(string user)
{
	if (user == "admin")
	{
		string* item = new string[5];
		item[0] = "#1. ��������, ��������, ����������� �������������.";
		item[1] = "#2. �������� ����������.";
		item[2] = "#3. ���������� �������������.";
		item[3] = "#4. ��������� ������ � ������ ��������������";
		item[4] = "#5. �����.";
		return item;
	}

	else if (user == "sign")
	{
		string* item = new string[3];
		item[0] = "#1. ���� c ������� �����.";
		item[1] = "#2. �����������.";
		item[2] = "#3. �����.";
		return item;
	}

	else
	{
		string* item = new string[4];
		item[0] = "#1. ������ ����� ������������.";
		item[1] = "#2. ���������� ������������� ������������.";
		item[2] = "#3. ����������� ���������� ������������.";
		item[3] = "#4. �����.";
		return item;
	}
}
//-----------------------------------------------------------------------------------------------
void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
//-----------------------------------------------------------------------------------------------
void AdminFun1()
{
	char key = '1'; // ��� ����� ����� (������� �����, ������� ���� � �. �.);

	do
	{
		system("cls");
		gotoxy(0, 0);
		cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";
		int gotx = 6;

		gotoxy(25, ++gotx);

		cout << "��������, ��������, ����������� �������������." << endl;
		gotoxy(25, ++gotx);
		cout << "#1. ��������";
		gotoxy(25, ++gotx);
		cout << "#2. ��������";
		gotoxy(25, ++gotx);
		cout << "#3. �����������";
		gotoxy(25, ++gotx);
		cout << "#4. ����� ���� �������������";
		gotoxy(25, ++gotx);
		cout << "#0. �����";

		gotoxy(25, ++gotx);
		cout << "������� ����� ����: ";

		cin >> key;

		switch (key) // �������� ���������� �����.
		{
		case '1': // ��������.			
		{
			unique_ptr<Student> st(new Student);
			st->Registration();
			//_getch();
			break;
		}

		case '2': // ��������
		{
			unique_ptr<Admin> ad(new Admin);
			ad->DeleteUser();
			break;
		}

		case '3': // �����������
		{
			unique_ptr<Admin> ad(new Admin);
			ad->Modification();
			break;
		}

		case '4': // ����� �� ����� ������ ������������������ �������������
		{
			system("cls");
			unique_ptr<Admin> ad(new Admin);
			ad->PrintStudentsFull();

			_getch();
			break;
		}

		case '0': // �����
		{
			break;
		}

		default:
			break;
		}
	} while (key != '0');


	system("cls");
}
//-----------------------------------------------------------------------------------------------
void AdminFun2()
{
	system("cls");
	gotoxy(0, 0);
	cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";

	unique_ptr<Admin> admin(new Admin);
	admin->ShowUserGrade(userName->GetFIO());
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void AdminFun3()
{
	char key = '1';

	do
	{
		system("cls");
		gotoxy(0, 0);
		cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";

		gotoxy(25, 7);

		cout << "���������� �������������." << endl;
		gotoxy(25, 8);
		cout << "#1. �������� ��������� ������������";
		gotoxy(25, 9);
		cout << "#2. �������������� ��������� ������������";
		gotoxy(25, 10);
		cout << "#3. �������� ������";
		gotoxy(25, 11);
		cout << "#4. �������� �������� ������ � ����������";
		gotoxy(25, 12);
		cout << "#0. �����";

		gotoxy(25, 13);
		cout << "������� ����� ����: ";

		cin >> key;

		switch (key) // �������� ���������� �����.
		{
		case '1': // �������� ��������			
		{
			unique_ptr<Admin> ad(new Admin);
			ad->AddCategories();
			break;
		}

		case '2': // �������������� ��������	
		{
			unique_ptr<Admin> ad(new Admin);

			ad->EditCategories();

			break;
		}

		case '3': // �������� ������
		{
			unique_ptr<Admin> ad(new Admin);
			ad->AddTests();
			break;
		}

		case '4': // �������������� ������
		{
			unique_ptr<Admin> ad(new Admin);

			ad->AddTestsName();

			break;
		}

		case '0': // �����
		{
			break;
		}

		default:
			break;
		}
	} while (key != '0');


	system("cls");
}

void AdminFun4()
{
	system("cls");

	gotoxy(0, 0);
	cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	unique_ptr<Admin>admin(new Admin);

	admin->ChangeLogin();

	system("cls");
}


//-----------------------------------------------------------------------------------------------
void MenuFun20() //���� ������������ � �������
{
	system("cls");

	gotoxy(0, 0);
	cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;
	gotoxy(25, 7);

	//���� ����� �����
	unique_ptr<Student> user(new Student);

	string name;

	name = user->UserSignIn();

	if (name == "admin")
	{
		userName->SetUser("admin");
		userName->SetFIO("������������� �������");
		unique_ptr<Menu> menu;
		shared_ptr<Factory> main_menu;
		shared_ptr<AdminFactory> ma(new AdminFactory);
		main_menu = ma;
		menu = main_menu->runMenu();
		menu->printMenu();
	}

	else if (name == userName->GetUser())
	{
		gotoxy(25, 12);
		cout << "���� � ������� �� ������";
	}

	else
	{
		userName->SetUser(name);
		unique_ptr<Menu> menu;
		shared_ptr<Factory> main_menu;
		shared_ptr<StudentFactory> ma(new StudentFactory);
		main_menu = ma;
		menu = main_menu->runMenu();
		menu->printMenu();
	}

	_getch();
	system("cls");
}
//-----------------------------------------------------------------------------------------------
void MenuFun21() // ����������� ������������
{
	system("cls");

	gotoxy(0, 0);
	cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	gotoxy(25, 7);
	unique_ptr<Student> st(new Student);
	st->Registration();

	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun1() // ����������� ������������
{
	system("cls");

	gotoxy(0, 0);
	cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";

	unique_ptr<Student> student(new Student);
	student->SetLogin(userName->GetUser());

	student->NewTest();

	gotoxy(25, 7);
	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun2()
{
	system("cls");

	gotoxy(0, 0);
	cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	gotoxy(25, 7);
	unique_ptr<Student> student(new Student);
	student->SetLogin(userName->GetUser());
	student->ContinueTest();

	system("cls");
}

//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void StudentFun3()
{
	system("cls");

	gotoxy(0, 0);
	cout << "������������: " << userName->GetUser() << " (" << userName->GetFIO() << ")";;

	unique_ptr<Student> student(new Student);
	student->SetLogin(userName->GetUser());
	student->ShowUserGrade(userName->GetFIO());

	system("cls");
}

//-----------------------------------------------------------------------------------------------
void ExitOption()
{
	gotoxy(30, 15);
	cout << "�����..." << endl;
	_getch();
	exit(0);
}
//-----------------------------------------------------------------------------------------------
void ChangeCursorStatus(bool Visible)
{
	CONSOLE_CURSOR_INFO* c = new CONSOLE_CURSOR_INFO;
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	if (Visible)
	{
		c->bVisible = TRUE;
		c->dwSize = 0;
	}
	else
	{
		c->bVisible = FALSE;
		c->dwSize = 1;
	}

	SetConsoleCursorInfo(h, c);
}
