#pragma once

void UseMenu(string name, int ItemCount);

class Menu
{
public:
	Menu() {};
	~Menu() {};
	virtual void printMenu() = 0;
};

class MenuAdmin : public Menu
{
public:
	void printMenu() override;
};

class MenuStudent : public Menu
{
	void printMenu() override;
};

class MenuSign : public Menu
{
public:
	void printMenu() override;
};

class Factory
{
public:

	virtual unique_ptr<Menu> runMenu() = 0;
};

class AdminFactory : public Factory
{
public:

	unique_ptr<Menu>runMenu() override
	{
		return make_unique<MenuAdmin>();
	}
};

class SignFactory : public Factory
{
public:

	unique_ptr<Menu>runMenu() override
	{
		return make_unique<MenuSign>();
	}
};

class StudentFactory : public Factory
{
public:

	unique_ptr<Menu>runMenu() override
	{
		return make_unique<MenuStudent>();
	}
};

void MenuAdmin::printMenu()
{
	system("cls");
	int ItemCount = 5;
	UseMenu("admin", ItemCount);
}


void MenuSign::printMenu()
{
	int ItemCount = 3;
	UseMenu("sign", ItemCount);
}

void MenuStudent::printMenu()
{
	system("cls");
	int ItemCount = 4;
	UseMenu("student", ItemCount);
}
