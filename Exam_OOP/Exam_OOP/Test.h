#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "MD5.h"

#include "conio.h"
#include "ctype.h"

using namespace std;

class RegLog
{

public:
	void reg_admin();
	bool login_in();
	void reg_in();
};

void RegLog::reg_admin()
{
	ofstream reg_adminFile;
	reg_adminFile.open("RegAdminFile.txt");

	bool cont = false;
	char pas, con;
	string login_in;
	string password_in;

	vector <char> vecPassword, vecConfirm;

	while (!cont)
	{
		int i = 0;
		cout << "Зарегестрируйте админа" << endl;
		cout << "Логин: ";
		getline(cin, login_in);
		cout << "Пароль: ";
		getline(cin, password_in);

		while ((pas = _getch()) != '\r')
		{
			vecPassword.push_back(pas);
			_putch('*');
		}
		string password_in(vecPassword.begin(), vecPassword.end());
		cout << endl;
		string confirm_pas(vecConfirm.begin(), vecConfirm.end());

		if (password_in.length() > 3)
		{
			password_in = md5(password_in);
			reg_adminFile << login_in << endl;
			reg_adminFile << password_in << endl;
			reg_adminFile.close();

			cout << endl;
			cout << "" << endl;
			cout << endl;
			cont = true;
		}
	}
}