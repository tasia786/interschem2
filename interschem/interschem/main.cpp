#include <iostream>
#include <string>
#include <raylib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "check_if_valid.h"
#include "button.h"
#include "structs.h"
#include "draw.h"
#include "prelucrare.h"
#include "collision.h"

extern "C" {
#include "GLFW/glfw3.h"
}

using namespace std;

string get_string_from_keyboard(const char* prompt) {
	string input;
	while (true) {
		BeginDrawing();
		ClearBackground(Dark_Cyan);
		DrawText(prompt, 960 - MeasureText(prompt, 35) / 2, 540, 35, WHITE);
		DrawText(input.c_str(), 960 + MeasureText(prompt, 35) / 2 + 10, 540, 35, WHITE);
		EndDrawing();

		if (IsKeyPressed(KEY_ENTER)) break;

		// Verificam daca tasta Backspace este apasata
		if (IsKeyPressed(KEY_BACKSPACE) && !input.empty()) {
			input.pop_back(); // Stergem ultimul caracter din sir
		}

		// Folosim GetCharPressed pentru a captura caracterele tastate
		int key = GetCharPressed();
		if (key > 0 && key < 127) {  // Caractere imprimabile standard
			input += (char)key;
		}
	}
	return input;
}

bool IsMouseOverBlock(const block& shape) {
	Vector2 mousePoint = GetMousePosition();
	return (mousePoint.x > shape.x && mousePoint.x < (shape.x + shape.width) &&
		mousePoint.y > shape.y && mousePoint.y < (shape.y + shape.height));
}

void DeleteSelectedBlock(vector<block>& shapes, vector<Connection>& connections, int& selectedIndex, int& cnt_start, int& selectedBlock, int& selectedPoint) {
	if (selectedIndex >= 0 && selectedIndex < shapes.size()) {
		// Eliminăm conexiunile asociate blocului șters
		auto it = connections.begin();
		while (it != connections.end()) {
			if (it->blockIndex1 == selectedIndex || it->blockIndex2 == selectedIndex) {
				it = connections.erase(it); // Eliminăm conexiunea asociată blocului șters
			}
			else {
				// Ajustăm indicii pentru blocurile rămase
				if (it->blockIndex1 > selectedIndex) it->blockIndex1--;
				if (it->blockIndex2 > selectedIndex) it->blockIndex2--;
				++it;
			}
		}

		// Ajustăm indicii blocurilor rămase
		for (int i = selectedIndex + 1; i < shapes.size(); ++i) {
			if (shapes[i].index > selectedIndex) {
				shapes[i].index--;
			}
		}

		// Dacă blocul șters este de tip 1, decrementăm cnt_start
		if (shapes[selectedIndex].type == 1) cnt_start--;

		// Ștergem blocul din shapes
		delete_connection(&shapes[selectedIndex]);
		shapes.erase(shapes.begin() + selectedIndex);

		// Resetăm selecția după ștergere
		selectedIndex = -1;

		// Resetăm selecția pentru punctele și blocurile asociate
		selectedBlock = -1;
		selectedPoint = -1;
	}
}

int FindConnectionPoint(vector<block>& shapes, Vector2 mousePos, int& blockIndex) {
	for (int i = 0; i < shapes.size(); i++)
	{
		const block& shape = shapes[i];
		// Punctele de conectare depind de tipul blocului
		vector<Vector2> points;
		if (shapes[i].type == 1)
			points.push_back({ shape.x + shape.width / 2, shape.y + shape.height });
		else
			if (shapes[i].type == 6)
				points.push_back({ shape.x + shape.width / 2, shape.y });
			else
				if (shape.type == 4)  // Triunghi
					points = { {shape.x + shape.width / 2, shape.y},{shape.x, shape.y + shape.height},{shape.x + shape.width, shape.y + shape.height} };
				else points = { { shape.x + shape.width / 2, shape.y } ,{ shape.x + shape.width / 2, shape.y + shape.height } };

		for (int j = 0; j < points.size(); j++)
		{
			if (CheckCollisionPointCircle(mousePos, points[j], 15))
			{
				blockIndex = shapes[i].index;
				if (points.size() == 1 and shapes[i].type == 1)
					return 1;
				if (points.size() == 1 and shapes[i].type == 6)
					return 0;
				if (points.size() == 2)
					return j;
				if (points.size() == 3)
					return j + 3;
			}
		}
	}
	return -1; // Niciun punct de conectare gasit
}

string readFileToString(const string& filePath) {
	// Deschidem fisierul
	ifstream file(filePath);

	// Verificam dacă fisierul a fost deschis corect
	if (!file.is_open()) {
		cerr << "Nu s-a putut deschide fișierul!" << endl;
		return "";
	}

	// Citim tot continutul fisierului intr-un stringstream
	stringstream buffer;
	buffer << file.rdbuf();

	// Convertim continutul stream-ului intr-un string
	return buffer.str();
}

int main() {
	InitWindow(screenWidth, screenHeight, "InterSchem");
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);
	glfwSetWindowPos(window, 0, 100);
	//SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(30);

	//init buttons
	Button start_button(50, 50, 220, 50, "START");
	Button read_button(50, 125, 220, 50, "READ");
	Button assign_button(50, 200, 220, 50, "ASSIGN");
	Button condition_button(50, 275, 220, 50, "CONDITION");
	Button print_button(50, 350, 220, 50, "PRINT");
	Button stop_button(50, 425, 220, 50, "STOP");
	Button delete_button(50, 500, 220, 50, "DELETE BLOCK", RED, Light_Red, Dark_Red);
	Button run_button(50, 575, 220, 50, "RUN", RED, Light_Red, Dark_Red);
	Button save_button(50, 650, 100, 50, "SAVE", GREEN, Light_Green, Dark_Green);
	Button load_button(170, 650, 100, 50, "LOAD", GREEN, Light_Green, Dark_Green);
	Button code_button(50, 725, 220, 50, "CREATE CODE");
	Button copy_button(screenWidth - 130, screenHeight / 4.75, 80, 50, "COPY");
	Button reset_button(50, 800, 220, 50, "RESET", RED, Light_Red, Dark_Red);
	Button exit_button(50, screenHeight-100, 120, 50, "EXIT", RED, Light_Red, Dark_Red);

	int selectedIndex;
	int selectedBlock = -1, selectedPoint = -1;

	int cnt_start = 0;
	string code;

	while (!WindowShouldClose()) {
		start_button.Update();
		read_button.Update();
		assign_button.Update();
		condition_button.Update();
		print_button.Update();
		stop_button.Update();
		delete_button.Update();
		run_button.Update();
		save_button.Update();
		load_button.Update();
		code_button.Update();
		reset_button.Update();
		copy_button.Update();
		exit_button.Update();

		// Detectarea selectiei blocului
		for (int i = 0; i < shapes.size(); i++)
			if (IsMouseOverBlock(shapes[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				selectedIndex = i;

		if (IsKeyDown(KEY_LEFT_ALT) and IsKeyPressed(KEY_ENTER))
			ToggleFullscreen();

		if (delete_button.WasPressed())
		{
			DeleteSelectedBlock(shapes, connections, selectedIndex, cnt_start, selectedBlock, selectedPoint);
			idx--;
		}

		if (start_button.WasPressed() and cnt_start < 1) {
			block newShape;
			newShape = { 1,idx, NULL, NULL, NULL, 0, 0, 0, 970,270,100,50,false,"START", GREEN };
			AvoidOverlapping(newShape);
			idx++;
			cnt_start++;
			shapes.push_back(newShape); // Adaugam forma la vector
		}

		if (read_button.WasPressed()) {
			string variable = get_string_from_keyboard("TYPE VARIABLE:");
			while (variable.size() != 1 or variable[0] < 'A' or variable[0]>'Z')
				variable = get_string_from_keyboard("WRONG VARIABLE (SINGLE CAPITAL LETTER):");
			string value = get_string_from_keyboard("TYPE VALUE:");
			while (!isValidValue(value))
				value = get_string_from_keyboard("WRONG VALUE (REAL):");
			block newShape;
			newShape = { 2,idx,variable[0],NULL,NULL,0,0,0,960, 270, 100, 50, false, value, YELLOW };
			AvoidOverlapping(newShape);
			idx++;
			shapes.push_back(newShape);
			//cout << "Variabila: " << variable << " valoarea: " << value << endl;
		}

		if (assign_button.WasPressed())
		{
			string expr = get_string_from_keyboard("TYPE AN ASSIGNMENT:");
			while (!check_type3(expr))
				expr = get_string_from_keyboard("WRONG ASSIGN (VAR<-EXPR):");
			block newShape;
			newShape = { 3,idx,NULL,NULL,NULL,0,0,0,960,270,100,50, false, expr,SKYBLUE };
			AvoidOverlapping(newShape);
			idx++;
			shapes.push_back(newShape);
			//cout << "Valoarea expresiei: " << newShape.text;
		}

		if (condition_button.WasPressed())
		{
			string expr = get_string_from_keyboard("TYPE A CONDITION:");
			while (!check_type4(expr))
				expr = get_string_from_keyboard("WRONG CONDITION:");
			block newShape;
			newShape = { 4,idx,NULL,NULL,NULL,0,0,0,960,270,200,100,false,expr,PINK };
			AvoidOverlapping(newShape);
			idx++;
			shapes.push_back(newShape);
		}

		if (print_button.WasPressed())
		{
			string expr = get_string_from_keyboard("TYPE AN EXPRESSION/TEXT TO PRINT (expr/\"text\"):");
			while (!check_type5(expr))
				expr = get_string_from_keyboard("WRONG EXPRESSION/TEXT TO PRINT (expr/\"text\"):");
			block newShape;
			newShape = { 5,idx,NULL,NULL,NULL,0,0,0,960,270,100,50,false,expr,MAGENTA };
			AvoidOverlapping(newShape);
			idx++;
			shapes.push_back(newShape);
		}

		if (stop_button.WasPressed())
		{
			block newShape;
			newShape = { 6,idx,NULL,NULL,NULL,0,0,0,960,270,100,50,false,"STOP",RED };
			AvoidOverlapping(newShape);
			idx++;
			shapes.push_back(newShape);
		}

		if (exit_button.WasPressed()) {
			exit(0); // Inchide aplicatia si consola
		}

		if (run_button.WasPressed()) {
			/*for (auto shape : shapes){
				cout << shape.index << " " << shape.var << "=" << shape.text << " " << shape.type << endl;
			}
			cout << endl;
			for (auto conn : connections){
				cout << conn.blockIndex1 << " " << conn.blockIndex2 << endl;
			}*/
			for (auto conn : connections) {
				if (conn.pointIndex1 == 4) {
					shapes[conn.blockIndex1].next = false;
				}
				if (conn.pointIndex1 == 5) {
					shapes[conn.blockIndex1].next = true;
				}
				create_connection(conn.blockIndex1, conn.blockIndex2);
			}
			impossible_op = false;
			output.clear();
			compile(shapes);
		}

		if (save_button.WasPressed()) {
			for (auto conn : connections) {
				if (conn.pointIndex1 == 4) {
					shapes[conn.blockIndex1].next = false;
				}
				if (conn.pointIndex1 == 5) {
					shapes[conn.blockIndex1].next = true;
				}
				create_connection(conn.blockIndex1, conn.blockIndex2);
			}
			create_code(shapes);
			save();
		}

		if (load_button.WasPressed()) {
			load();
		}

		if (code_button.WasPressed()) {
			for (auto conn : connections) {
				if (conn.pointIndex1 == 4) {
					shapes[conn.blockIndex1].next = false;
				}
				if (conn.pointIndex1 == 5) {
					shapes[conn.blockIndex1].next = true;
				}
				create_connection(conn.blockIndex1, conn.blockIndex2);
			}
			create_code(shapes);
			code = readFileToString("cod.txt");
		}

		if (copy_button.WasPressed()) {
			SetClipboardText(code.c_str());
		}

		if (reset_button.WasPressed()) {
			shapes.clear();
			connections.clear();
			idx = 0;
			cnt_start = 0;
			output.clear();
			code.clear();
		}

		if (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetMousePosition();
			int blockIndex;
			int pointIndex = FindConnectionPoint(shapes, mousePos, blockIndex);
			if (pointIndex != -1) { // Daca s-a gasit un punct de conectare
				if (selectedBlock == -1) {
					// Selectam primul punct
					selectedBlock = blockIndex;
					selectedPoint = pointIndex;
					//cout << "Conexiune intre: " << selectedBlock << " si " << blockIndex;
				}
				else {
					// Cream o conexiune
					int cnt = 0;
					for (auto conn : connections)
					{
						if (selectedBlock == conn.blockIndex1 and selectedPoint == conn.pointIndex1)
							connections.erase(connections.begin()+cnt);
						cnt++;
					}
					connections.push_back({ selectedBlock, selectedPoint, blockIndex, pointIndex });
					selectedBlock = -1;
					selectedPoint = -1;
				}
			}
		}

		// Zona prestabilită în care blocurile se pot mișca
		for (auto& shape : shapes) {
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT)) {
				Vector2 mousePoint = GetMousePosition();

				// Verificăm dacă mouse-ul este în interiorul formei
				if (mousePoint.x > shape.x && mousePoint.x < (shape.x + shape.width) &&
					mousePoint.y > shape.y && mousePoint.y < (shape.y + shape.height)) {
					shape.isDragging = true;
				}
			}

			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				shape.isDragging = false;
			}

			if (shape.isDragging) {
				Vector2 mousePoint = GetMousePosition();
				shape.x = mousePoint.x - shape.width / 2;
				shape.y = mousePoint.y - shape.height / 2;

				// Verificăm că blocul nu iese din zona permisă
				if (shape.x < space.x) shape.x = space.x;
				if (shape.y < space.y) shape.y = space.y;
				if (shape.x + shape.width > space.x + space.width) {
					shape.x = space.x + space.width - shape.width;
				}
				if (shape.y + shape.height > space.y + space.height) {
					shape.y = space.y + space.height - shape.height;
				}

				// Verificăm coliziunile cu celelalte blocuri
				for (auto& otherShape : shapes) {
					if (&shape != &otherShape && CheckCollisionBlocks(shape, otherShape)) {
						// Apelăm funcția pentru a rezolva coliziunea
						ResolveCollision(shape, otherShape);
					}
				}
			}
		}

		BeginDrawing();
		ClearBackground(Dark_Cyan);
		draw_misc();
		start_button.Draw();
		read_button.Draw();
		assign_button.Draw();
		condition_button.Draw();
		print_button.Draw();
		stop_button.Draw();
		delete_button.Draw();
		run_button.Draw();
		save_button.Draw();
		load_button.Draw();
		code_button.Draw();
		copy_button.Draw();
		reset_button.Draw();
		exit_button.Draw();

		if (impossible_op)
			output = "IMPOSSIBLE OPERATION";

		DrawText(output.c_str(), screenWidth / 1.244, 50, 35, BLACK);
		DrawText(code.c_str(), screenWidth / 1.244, screenHeight / 4, 35, BLACK);

		DrawConnections(shapes, connections);
		for (auto& shape : shapes) {
			if (shape.type == 1)
				draw_type1(shape);
			if (shape.type == 2)
				draw_type2(shape);
			if (shape.type == 3)
				draw_type3(shape);
			if (shape.type == 4)
				draw_type4(shape);
			if (shape.type == 5)
				draw_type5(shape);
			if (shape.type == 6)
				draw_type6(shape);
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

