#include "pch.h"
#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <list>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <windows.h>


using namespace std;

namespace colorful {
	struct FigletText {
		std::string* text;
		int textlenght;
	};
	vector<string> split(const string& s, char delim) {
		vector<string> result;
		stringstream ss(s);
		string item;

		while (getline(ss, item, delim)) {
			result.push_back(item);
		}

		return result;
	}

	FigletText MakeFigletText(string text, string font) {

		int i;
		smatch match;
		smatch match2;
		regex e("flf2a\\$ (.+?) ");
		regex e1("flf2a\\$ (.+?) (.+?) (.+?) (.+?) (.+?)(\r\n| )");
		regex_search(font, match, e);
		regex_search(font, match2, e1);
		int FigletCharHeight = stoi(match[1]);
		int  startingIndex = stoi(match2[5]) + 2;
		string sfont = font;
		int j = 0;
		for (char c : sfont) {
			sfont.erase(std::remove(sfont.begin(), sfont.end(), '\r'), sfont.end());
		}
		vector<string> FiFont = split(sfont, '\n');
		string* dest = FiFont.data();
		string* final = new string[FigletCharHeight];
		for (char c : text) {
			int asciipos = (int)c - 32;
			int figletspos = (startingIndex - 1) + FigletCharHeight * asciipos;
			int figletepos = figletspos + FigletCharHeight;

			for (i = figletspos, j = 0; i < figletepos; i++, j++) {
				final[j] += dest[i];
				for (char c : final[j]) {
					final[j].erase(std::remove(final[j].begin(), final[j].end(), '@'), final[j].end());
				}
			}
		}

		FigletText rvalue;

		rvalue.text = final;
		rvalue.textlenght = FigletCharHeight;

		return rvalue;
	}

	FigletText MakeFigletTextFromFile(string text, string path) {
		ifstream t(path);
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

		FigletText result = MakeFigletText(text, str);

		return result;
	}

	bool Values[16] = { true,false,false,false,false,false,false,true,false,false,false,false,false,false,false,false };
	enum Alignment { Right, Middle, Left };
	struct ColorfulString {
		std::string value;
		COLORREF color;
		int arrlenght;
	};

	ColorfulString* operator +(ColorfulString text1, ColorfulString text2)
	{
		ColorfulString* arr = new ColorfulString[2];
		arr[0] = text1;
		arr[1] = text2;
		arr[0].arrlenght = 2;
		return arr;
	}
	ColorfulString* operator +(ColorfulString* texts, ColorfulString text2) {
		int i;
		int arraysize = texts[0].arrlenght + 1;
		ColorfulString* newarray = new ColorfulString[arraysize];

		for (i = 0; i < arraysize - 1; i++) {
			newarray[i] = texts[i];

		}
		newarray[arraysize - 1] = text2;
		newarray[0].arrlenght = arraysize;

		return newarray;
	}

	void PlaceCursorLeft(int lenght) {

		CONSOLE_SCREEN_BUFFER_INFOEX info;
		info.cbSize = sizeof(info);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfoEx(hConsole, &info);

		int yref = info.dwCursorPosition.Y;
		int xmax = info.dwMaximumWindowSize.X;

		COORD Point;

		Point.Y = yref;
		Point.X = xmax - lenght;

		SetConsoleCursorPosition(hConsole, Point);
	}

	void PlaceCursorMiddle(int lenght) {
		CONSOLE_SCREEN_BUFFER_INFOEX info;
		info.cbSize = sizeof(info);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfoEx(hConsole, &info);

		int yref = info.dwCursorPosition.Y;
		int xmax = info.dwMaximumWindowSize.X;

		COORD Point;

		Point.Y = yref;
		Point.X = (xmax - lenght) / 2;

		SetConsoleCursorPosition(hConsole, Point);
	}

	int SetColor(COLORREF color)
	{
		CONSOLE_SCREEN_BUFFER_INFOEX info;
		info.cbSize = sizeof(info);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfoEx(hConsole, &info);



		int i, chosen = 17;
		for (i = 0; i < 16; i++) {

			COLORREF a = info.ColorTable[i];

			if (info.ColorTable[i] == color) {
				chosen = i;

				return chosen;
			}

		}
		for (i = 0; i < 16; i++) {

			if (!Values[i]) {
				chosen = i;
				Values[i] = true;
				break;
			}


		}
		if (chosen == 17) {
			throw;
		}
		else {
			info.ColorTable[chosen] = color;
			SetConsoleScreenBufferInfoEx(hConsole, &info);
			return chosen;
		}
	}

	void Write(std::string text, COLORREF color = RGB(192, 192, 192), Alignment alignment = Right) {
		int postion = SetColor(color);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, postion);
		switch (alignment)
		{
		case Right: {
			std::cout << text;
			break;
		}
		case Middle: {
			PlaceCursorMiddle(text.length());
			std::cout << text;
			break;
		}
		case Left: {
			PlaceCursorLeft(text.length());
			std::cout << text;
			break;
		}
		default:
			break;
		}
		SetConsoleTextAttribute(hConsole, 7);
	}

	void WriteLine(std::string text, COLORREF color = RGB(192, 192, 192), Alignment alignmnet = Right) {

		Write(text, color, alignmnet);
		Write("\r\n");
	}

	void Write(ColorfulString* text, Alignment alignment = Right) {
		int i;
		int lenght = 0;
		int arrsize = text[0].arrlenght;
		for (i = 0; i < arrsize; i++) {
			lenght += text[i].value.length();
		}
		switch (alignment)
		{
		case Right:
		{
			break;
		}
		case Middle:
		{
			PlaceCursorMiddle(lenght);
			break;
		}
		case Left:
		{
			PlaceCursorLeft(lenght);
			break;
		}
		default:
			break;
		}
		for (i = 0; i < arrsize; i++) {
			std::string a = text[i].value;
			COLORREF b = text[i].color;
			Write(a, b);
		}
	}

	void WriteLine(ColorfulString* text, Alignment alignment = Right) {
		Write(text, alignment);
		Write("\r\n");
	}

	void SetGlobalTextColor(COLORREF color) {
		int position = SetColor(color);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, position);
	}

	void ResetGlobalTextColor() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 7);
	}

	ColorfulString MakeColorfulString(std::string text, COLORREF color = RGB(192, 192, 192)) {

		ColorfulString a;
		a.value = text;
		a.color = color;

		return a;
	}

	void WriteTitle(FigletText text, COLORREF color = RGB(192, 192, 192), Alignment alignment = Right) {
		int i;
		for (i = 0; i < text.textlenght; i++) {
			switch (alignment)
			{
			case Right:
			{
				WriteLine(text.text[i], color);
				break;
			}
			case Middle:
			{
				WriteLine(text.text[i], color, Middle);
				break;
			}
			case Left:
			{
				WriteLine(text.text[i], color, Left);
				break;
			}
			default:
				break;
			}
		}
	}

	void WriteTitle(string text, string PathorFont, COLORREF color = RGB(192, 192, 192), Alignment alignment = Right, bool isFont = false) {
		if (isFont) {
			FigletText a = MakeFigletText(text, PathorFont);
			WriteTitle(a, color, alignment);
		}
		else {
			FigletText a = MakeFigletTextFromFile(text, PathorFont);
			WriteTitle(a, color, alignment);
		}
	}
}
