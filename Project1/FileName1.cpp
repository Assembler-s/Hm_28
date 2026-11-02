#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

class FileName
{
private:
	string name;

public:
	FileName(const string& name) : name(name) {};

	string getName() const
	{
		return name;
	}

	string getExtension() const
	{
		if (name.rfind(".") != string::npos)
		{
			size_t indexPoint = name.rfind(".") + 1;
			
			if (name.size() > indexPoint)
			{
				return name.substr(indexPoint);
			}
		}
		return "";
	}

	string getBaseName() const
	{
		if (name.rfind(".") != string::npos)
		{
			size_t indexPoint = name.rfind(".");
			if (indexPoint > 0)
			{
				return name.substr(0, indexPoint);
			}
		}
		return name;
	}

	void changeExtension(const string& newExtension)
	{
		name = getBaseName() + "." + newExtension;
	}

	bool isImage() const
	{
		if (this->getExtension() == "png" || this->getExtension() == "jpg" || this->getExtension() == "gif")
			return true;

		return false;
	}

	void print() const
	{
		cout << name << " | Имя: " << this->getBaseName() << " | расширение: [" << this->getExtension() << "]";
		
		if (isImage())
			cout << " | изображение";
		cout << endl;
	}

	bool operator < (const FileName& other) const
	{
		return name < other.name;
	}
};
///////////////////////////////////////////

// level 2
class MessageTemplate
{
private:
	string text;
	static void replaceAll(string& text, const string& from, const string& to);

public:
	MessageTemplate(const string& text_) : text(text_) {};

	int countPlaceholders() const
	{
		size_t len_text = text.length();
		size_t counter = 0;
		for (int i = 0; i < len_text; i++)
		{
			if (text[i] == '{')
				counter++;
		}

		return counter;
	}

	string fill(const string& name, int score) const
	{
		string copy_text = text;
		replaceAll(copy_text, "{name}", name);
		replaceAll(copy_text, "{score}", to_string(score));

		return copy_text;
	}
};

void MessageTemplate::replaceAll(string& text, const string& from, const string& to)
{
	size_t position = text.find(from);
	while (position != string::npos)
	{
		text.replace(position, from.length(), to);
		position = text.find(from, position + to.length());
	}
}

void parseStudent(const string& line)
{
	size_t first = line.find(';');
	size_t second = line.find(';', first + 1);

	string name = line.substr(0, first);
	string ageText = line.substr(first + 1, second - first - 1);
	string email = line.substr(second + 1);

	int age = stoi(ageText);

	cout << "Ім'я: " << name << endl;
	cout << "Вік: " << age << ", наступного року: " << age + 1 << endl;
	cout << "Пошта: " << email << endl;
}


int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	// level 1
	FileName report_file{ "report.final.docx" };
	FileName photo_file("photo.png");
	FileName rdm_file("README");

	report_file.print();
	photo_file.print();
	rdm_file.print();
	
	report_file.changeExtension("pdf");
	rdm_file.changeExtension("md");
	report_file.print();
	rdm_file.print();

	FileName copy = report_file;
	copy.changeExtension("txt");
	copy.print();
	report_file.print();

	if (photo_file < report_file)
		cout << photo_file.getName() << " < " << report_file.getName() << endl;
	else 
		cout << report_file.getName() << " < " << photo_file.getName() << endl;
	///////////////////////////////

	// level 2
	cout << "-----------\n";
	MessageTemplate message{ "Вітаю, {name}! Ваш бал за модуль: {score}. {name}, так тримати!" };
		
	string names[3] = { "Олена", "Марко", "Тарас" };
	int scores[3] = { 11, 9, 12 };

	cout << message.countPlaceholders() << endl;
	for (int i = 0; i < 3; i++)
		cout << message.fill(names[i], scores[i]) << endl;
	////////////////////////////////////////////

	// level 3
	cout << "-----------\n";
	parseStudent("Олена Коваль;20;olena.koval@itstep.org");
		


	return 0;
}

/*1) С char* пришлось бы вручную:
1. Найти последнюю точку через цикл.
2. Определить позицию точки.
3. Вычислить длину расширения.
4. Выделить динамическую память.
5. Скопировать расширение через strcpy.
6. Не забыть освободить выделенную память.*/

/*2 rfind(".") возвращает string::npos, поскольку точки нет. 
getExtension() проверяет это условие и возвращает пустую строку "".*/

/*3) std::string самостоятельно управляет динамической памятью. Поэтому стандартные конструктор 
копирования и оператор присваивания выполняют корректное копирование. Отдельный деструктор не нужен.*/

/*4) Два строковых литерала — это const char*. Для них оператор + не определён. Но std::string 
перегружает + для объединения строки с C-строкой.*/

/*5) Чтобы не искать повторно уже обработанную часть строки. Если имя студента — "{name}", поиск продолжится
после вставленного значения, поэтому подстановка не запустится бесконечно.*/

/*6) stoi преобразует строку "20" в число 20. К строке нельзя применить числовое сложение: "20" + 1 не означает 21.
Сначала нужно получить int, увеличить его, а затем при необходимости преобразовать обратно
в строку через to_string().*/