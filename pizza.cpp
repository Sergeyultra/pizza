#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;


struct Pipe {
	string name;
	int length;
	int diameter;
	bool inRepare = true;
};

struct CS {
	string name;
	int wrkshopsCount;
	vector <bool> WS;
	float efficiency;
};


float InputNum(float min, float max) {
	float num;
	cout << "\n\n> ";
	while ((cin >> num).fail() || (num < min) || (num > max)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "\n\n> ";
	}
	cout << endl;
	return num;
}

int Choose(int min, int max) {
	int num;
	cout << "\n\n> ";
	while ((cin >> num).fail() || (num < min) || (num > max)) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "\n> ";
	}
	cout << endl;
	return num;
}

int Menu() {
	std::cout << "\n1.Добавить трубу\n2.Добавить КС\n3.Список объектов\n4.Редактировать трубы\n5.Редактировать цеха\n6.Сохранить\n7.Загрузить\n0.Выход\n\n";
	int number = Choose(0, 7);
	return number;
}

void AddPipe(Pipe& pipe) {
	int t = 0;
	cout << "Название трубы:\n\n> ";
	string name;
	cin.ignore(10000, '\n');
	getline(cin, pipe.name);
	cout << endl;
	cout << "Длина трубы:";
	float length = InputNum(0, INT_MAX);
	pipe.length = length;
	cout << "Диаметр трубы: ";
	float diameter = InputNum(0, INT_MAX);
	pipe.diameter = diameter;
}

void ViewPipes(const Pipe& pipe) {
	cout << "Название трубы: " << pipe.name << endl;
	cout << "Длина трубы: " << pipe.length << endl;
	cout << "Диаметр трубы: " << pipe.diameter << endl;
	if (pipe.inRepare)
		cout << "Состояние: ВКЛ." << endl << endl;
	else
		cout << "Состояние: ВЫКЛ." << endl << endl;
}
void AddCS(CS& cs) {
	vector <bool> WSs;
	cout << "Ввидите название КС:\n\n";
	cin.ignore(10000, '\n');
	getline(cin, cs.name);
	cout << "Введите число цехов:\n\n";
	int count = Choose(1, 10000);
	for (int j = 0; j < count; j++) {
		cs.WS.push_back(true);
	}
	cout << "Введите эффективность(1,100):\n\n";
	float effic = InputNum(1, 100);
	cs.efficiency = effic;
	cs.wrkshopsCount = count;
}
void ViewCSs(const CS& cs) {
	if (cs.WS.size() != 0) {
		cout << "Название КС: " << cs.name << endl;
		cout << "Число Цехов: " << cs.wrkshopsCount << endl << endl;
		cout << "Эффективность КС: " << cs.efficiency << "%" << endl << endl;
		for (int j = 0; j < cs.WS.size(); j++) {
			if (cs.WS[j])
				cout << "\tЦех №" << j + 1 << "      " << "В рабочем состоянии" << endl;
			else
				cout << "\tЦех №" << j + 1 << "      " << "Не в рабочем состоянии" << endl;
		}
		cout << endl;
	}
}

void PipeChange(Pipe& pipe) {
	cout << "Состояние:\n1.Работает\n2.Требуется ремонт\n";
	int n = Choose(1, 2);
	if (n == 2)
		pipe.inRepare = true;
	else
		pipe.inRepare = false;
}

void CSChange(CS& cs) {
	if (cs.wrkshopsCount > 0) {
		ViewCSs(cs);
		cout << "\n\tВыберете Цех для редактирования: ";
		int id = Choose(1, cs.WS.size());
		cs.WS[id - 1] = !cs.WS[id - 1];
	}
	else
		cout << "\nНет цехов для редактирования\n";
}

void SaveData(Pipe& pipe, CS& cs) {
	ofstream file;
	file.open("saves.txt", ios_base::out);
	if (!file.is_open()) {
		cout << "Ошибка чтения файла!";
	}
	else {
		if (pipe.length > 0 and pipe.diameter > 0) {
			file << 1 << endl;
			file << pipe.name << endl;
			file << pipe.length << " " << pipe.diameter << " " << pipe.inRepare << endl;
		}
		else {
			file << 0 << endl;
		}
		if (cs.WS.size() > 0) {
			file << 1 << endl;
			file << cs.name << endl;
			file << cs.wrkshopsCount << " " << cs.efficiency << endl;
			for (bool WS : cs.WS) {
				file << WS << " ";
			}
		}
		else {
			file << 0 << endl;
		}
	}
	file.close();
}

void LoadData(Pipe& pipe, CS& cs) {
	ifstream file("saves.txt", ios::in);
	if (file.is_open()) {
		bool existPipe;
		file >> existPipe;
		if (existPipe) {
			file.ignore(10000, '\n');
			getline(file, pipe.name);
			file >> pipe.length;
			file >> pipe.diameter;
			file >> pipe.inRepare;
		}
		bool existCS;
		file >> existCS;
		if (existCS) {
			file.ignore(10000, '\n');
			getline(file, cs.name);
			file >> cs.wrkshopsCount;
			file >> cs.efficiency;
			cs.WS.resize(cs.wrkshopsCount);
			for (bool WS : cs.WS) {
				file >> WS;
			}
		}
	}
	else {
		cout << "\n\nОшибка загрузки файла saves.txt\n\n";
	}
	file.close();
}

int main()
{
	Pipe pipe;
	CS cs;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	while (true) {
		int number = Menu();
		switch (number) {
		case 1:
			AddPipe(pipe);
			break;
		case 2:
			AddCS(cs);
			break;
		case 3:
			ViewPipes(pipe);
			ViewCSs(cs);
			break;
		case 4:
			PipeChange(pipe);
			break;
		case 5:
			CSChange(cs);
			break;
		case 6:
			SaveData(pipe, cs);
			break;
		case 7:
			LoadData(pipe, cs);
			break;
		case 0:
			exit(0);
		default:
			system("cls");
			cout << "Ошибка ввода\n";
			break;
		}
	}
	return 0;
}