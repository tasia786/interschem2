#pragma once
#include <iostream>
#include <string>
#include <raylib.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "structs.h"
using namespace std;

void draw_type1(block& shape) {
	DrawRectangleRounded(Rectangle{ shape.x, shape.y, shape.width, shape.height }, 0.5f, 5, shape.color);

	//connection points
	DrawCircle(shape.x + shape.width / 2, shape.y + shape.height, 12, BEIGE);

	//text
	int fontSize = 25;
	int textWidth = MeasureText(shape.text.c_str(), fontSize);
	int textHeight = fontSize;
	int textX = shape.x + (shape.width - textWidth) / 2;
	int textY = shape.y + (shape.height - textHeight) / 2;
	DrawText(shape.text.c_str(), textX, textY, fontSize, BLACK);
}

void draw_type2(block& shape) {
	int fontSize = 25;
	float top_width = 100;
	float bottom_width = 75;
	float height = shape.height;

	Vector2 top_left = { shape.x + (shape.width - top_width) / 2, shape.y };
	Vector2 top_right = { shape.x + (shape.width + top_width) / 2, shape.y };
	Vector2 bottom_left = { shape.x + (shape.width - bottom_width) / 2, shape.y + height };
	Vector2 bottom_right = { shape.x + (shape.width + bottom_width) / 2, shape.y + height };

	// Desenăm două triunghiuri pentru a forma trapezul
	DrawTriangle(top_left, bottom_left, bottom_right, shape.color);
	DrawTriangle(top_left, bottom_right, top_right, shape.color);

	//conenction points
	DrawCircle(shape.x + shape.width / 2, shape.y, 12, BEIGE);
	DrawCircle(shape.x + shape.width / 2, shape.y + shape.height, 12, BEIGE);

	// Centrăm textul
	int textWidth = MeasureText("A", fontSize);
	int textHeight = fontSize;
	int textX = shape.x + (shape.width - textWidth) / 2;
	int textY = shape.y + (shape.height - textHeight) / 2;
	string temp;
	temp += shape.var;
	DrawText(temp.c_str(), textX, textY, fontSize, BLACK);
}

void draw_type3(block& shape)
{
	int fontSize = 25;
	// Calculăm lățimea dreptunghiului în funcție de lungimea textului
	int textWidth = MeasureText(shape.text.c_str(), fontSize);
	int rectWidth = textWidth + 20;  // Adăugăm o margine pentru aspect
	int rectHeight = shape.height;

	// Desenăm dreptunghiul cu lățimea ajustată
	DrawRectangle(shape.x, shape.y, rectWidth, rectHeight, SKYBLUE);

	//connection points
	DrawCircle(shape.x + shape.width / 2, shape.y, 12, BEIGE);
	DrawCircle(shape.x + shape.width / 2, shape.y + shape.height, 12, BEIGE);

	// Centrăm textul în interiorul dreptunghiului
	int textX = shape.x + (rectWidth - textWidth) / 2;
	int textY = shape.y + (rectHeight - fontSize) / 2;
	shape.width = rectWidth;
	DrawText(shape.text.c_str(), textX, textY, fontSize, BLACK);
}

void draw_type4(block& shape) {
	int fontSize = 25;
	int textWidth = MeasureText(shape.text.c_str(), fontSize);
	int offset = 30; // Margine suplimentară

	// Calculăm dimensiunile triunghiului pe baza textului și offsetului
	shape.width = textWidth + 2 * offset;  // Baza triunghiului
	shape.height = shape.width / 2;       // Înălțimea triunghiului, proporțională cu baza

	// Calculăm vârfurile triunghiului
	Vector2 top = { shape.x + shape.width / 2, shape.y };                     // Vârful de sus
	Vector2 left = { shape.x, shape.y + shape.height };                      // Colțul stânga jos
	Vector2 right = { shape.x + shape.width, shape.y + shape.height };       // Colțul dreapta jos

	// Desenăm triunghiul
	DrawTriangle(top, left, right, shape.color);

	// Punctele de conexiune
	DrawCircle(top.x, top.y, 12, BEIGE); // Punct de sus
	DrawCircle(left.x, left.y, 12, BEIGE); // Stânga jos
	DrawCircle(right.x, right.y, 12, BEIGE); // Dreapta jos

	// Centrăm textul în triunghi
	int textX = shape.x + offset;  // Textul începe după marginea triunghiului
	int textY = shape.y + shape.height - fontSize; // Textul este centrat pe verticală
	DrawText(shape.text.c_str(), textX, textY, fontSize, BLACK);
}

void draw_type5(block& shape) {
	int fontSize = 25;
	int textWidth = MeasureText(shape.text.c_str(), fontSize);
	float bottom_width = textWidth + 50;
	float top_width = bottom_width / 1.5f;
	float height = shape.height;

	// Calculăm vârfurile trapezului
	Vector2 top_left = { shape.x + (shape.width - top_width) / 2, shape.y };
	Vector2 top_right = { shape.x + (shape.width + top_width) / 2, shape.y };
	Vector2 bottom_left = { shape.x + (shape.width - bottom_width) / 2, shape.y + height };
	Vector2 bottom_right = { shape.x + (shape.width + bottom_width) / 2, shape.y + height };

	// Desenăm două triunghiuri pentru a forma trapezul
	DrawTriangle(top_left, bottom_left, bottom_right, shape.color);
	DrawTriangle(top_left, bottom_right, top_right, shape.color);

	//connection points
	DrawCircle(shape.x + shape.width / 2, shape.y, 12, BEIGE);
	DrawCircle(shape.x + shape.width / 2, shape.y + shape.height, 12, BEIGE);

	// Centrăm textul
	int textX = shape.x + (shape.width - textWidth) / 2;
	int textY = shape.y + (shape.height - fontSize) / 2;
	DrawText(shape.text.c_str(), textX, textY, fontSize, BLACK);

	// Actualizăm hitbox-ul
	shape.width = top_width;
}

void draw_type6(block& shape) {
	DrawRectangleRounded(Rectangle{ shape.x, shape.y, shape.width, shape.height }, 0.5f, 5, shape.color);

	//connection points
	DrawCircle(shape.x + shape.width / 2, shape.y, 12, BEIGE);

	//text
	int fontSize = 25;
	int textWidth = MeasureText(shape.text.c_str(), fontSize);
	int textHeight = fontSize;
	int textX = shape.x + (shape.width - textWidth) / 2;
	int textY = shape.y + (shape.height - textHeight) / 2;
	DrawText(shape.text.c_str(), textX, textY, fontSize, BLACK);
}

void draw_misc() {
	Color cyan = { 80, 200, 255, 255 };
	Color tiffany = { 129, 216, 208, 255 };
	DrawRectangle(0, 0, screenWidth, 50, tiffany);
	DrawRectangle(0, 50, 320, screenHeight, tiffany);
	DrawRectangle(screenWidth-580,50, 580, screenHeight-50, tiffany);
	DrawText("Choose a type of block: ", 50, 15, 35, BLACK);
	DrawText("Output: ", screenWidth-565, 15, 35, BLACK);
}

Vector2 GetConnectionPoint(const block& b, int index) {
	if (index == 0 || index == 3) return { b.x + b.width / 2, b.y }; // sus mijloc
	if (index == 1) return { b.x + b.width / 2, b.y + b.height };   // jos mijloc
	if (index == 4) return { b.x, b.y + b.height };                 // stânga jos
	if (index == 5) return { b.x + b.width, b.y + b.height };       // dreapta jos
	return { b.x, b.y };                                            // fallback
}

const int cellSize = 20;
// Funcția pentru a face accesibile celulele din jurul unui punct
void makeAccessibleAround(vector<vector<int>>& grid, int x, int y) {
	if (x >= 0 && y >= 0 && x < grid[0].size() && y < grid.size()) {
		grid[y][x] = 0;  
	}
}

// Funcția pentru a ajusta punctele de start și final
void adjustStartAndEndPoints(vector<vector<int>>& grid, pair<int, int>& start, pair<int, int>& end) {
	// Dacă punctul de start este pe marginea unui bloc, îl facem accesibil
	int startX = start.first, startY = start.second;
	int endX = end.first, endY = end.second;

	if (grid[startY][startX] == 1) {
		makeAccessibleAround(grid, startX, startY);
	}

	// Dacă punctul de final este pe marginea unui bloc, îl facem accesibil
	if (grid[endY][endX] == 1) {
		makeAccessibleAround(grid, endX, endY);
	}
}

vector<Vector2> LeeAlgorithm(vector<vector<int>>& grid, pair<int, int> start, pair<int, int> end) {
	adjustStartAndEndPoints(grid, start, end);

	int rows = grid.size();
	int cols = grid[0].size();

	vector<vector<int>> dist(rows, vector<int>(cols, -1));
	queue<pair<int, int>> q;
	vector<pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

	dist[start.second][start.first] = 0;
	q.push(start);

	int maxSteps = rows * cols;  // Limita de pași pentru algoritmul Lee

	while (!q.empty()) {
		pair<int, int> current = q.front();
		q.pop();
		int x = current.first;
		int y = current.second;

		if (dist[y][x] > maxSteps) {
			cout << "Max steps exceeded: " << dist[y][x] << endl; // Mesaj de debug
			break;  // Opriți algoritmul dacă a depășit limita de pași
		}

		for (const auto& dir : directions) {
			int nx = x + dir.first;
			int ny = y + dir.second;

			if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
				if (grid[ny][nx] == 0 && dist[ny][nx] == -1) {
					dist[ny][nx] = dist[y][x] + 1;
					q.push({ nx, ny });
					if (nx == end.first && ny == end.second) break;
				}
			}
		}
	}

	vector<Vector2> path;
	int x = end.first, y = end.second;

	if (dist[y][x] == -1) {  // Dacă nu s-a găsit calea
		cout << "No path found, using alternative method" << endl;
		return path;
		// Apelează aici logica alternativă de desenare
	}

	// Dacă am găsit calea
	while (dist[y][x] != 0) {
		path.push_back({ x * cellSize + cellSize / 2.0f, y * cellSize + cellSize / 2.0f });
		for (const auto& dir : directions) {
			int nx = x - dir.first;
			int ny = y - dir.second;

			if (nx >= 0 && nx < cols && ny >= 0 && ny < rows && dist[ny][nx] == dist[y][x] - 1) {
				x = nx;
				y = ny;
				break;
			}
		}
	}

	path.push_back({ start.first * cellSize + cellSize / 2.0f, start.second * cellSize + cellSize / 2.0f });
	reverse(path.begin(), path.end());
	return path;
}

void DrawConnections(const vector<block>& shapes, const vector<Connection>& connections) {
	// Dimensiuni fereastră și chenar
	const int gridWidth = screenWidth / cellSize;   // Număr de celule pe lățime
	const int gridHeight = screenHeight / cellSize; // Număr de celule pe înălțime

	// Dimensiunile și poziția chenarului
	const int borderX = 335;
	const int borderY = 65;
	const int borderWidth = screenWidth/1.435;
	const int borderHeight = screenHeight/1.046;

	// Inițializare matrice grid
	vector<vector<int>> grid(gridHeight, vector<int>(gridWidth, 1)); // 1 = obstacol

	// Marcăm zona din interiorul chenarului ca liberă
	int startX = borderX / cellSize;
	int endX = (borderX + borderWidth) / cellSize;
	int startY = borderY / cellSize;
	int endY = (borderY + borderHeight) / cellSize;

	for (int y = startY; y < endY; y++) {
		for (int x = startX; x < endX; x++) {
			grid[y][x] = 0; // Liber
		}
	}

	// Marcăm blocurile ca obstacole în grid
	for (const auto& shape : shapes) {
		int blockStartX = shape.x / cellSize;
		int blockEndX = (shape.x + shape.width) / cellSize;
		int blockStartY = shape.y / cellSize;
		int blockEndY = (shape.y + shape.height) / cellSize;
		for (int y = blockStartY; y <= blockEndY; y++) {
			for (int x = blockStartX; x <= blockEndX; x++) {
				grid[y][x] = 1; // Obstacol
			}
		}
	}

	// Procesăm fiecare conexiune și generăm liniile
	for (const auto& connection : connections) {
		const block& block1 = shapes[connection.blockIndex1];
		const block& block2 = shapes[connection.blockIndex2];

		// Calcul puncte inițiale
		Vector2 point1 = GetConnectionPoint(block1, connection.pointIndex1);
		Vector2 point2 = GetConnectionPoint(block2, connection.pointIndex2);

		// Convertire coordonate în grid
		int startX = point1.x / cellSize;
		int startY = point1.y / cellSize;
		int endX = point2.x / cellSize;
		int endY = point2.y / cellSize;

		// Găsire cale cu algoritmul lui Lee
		vector<Vector2> path = LeeAlgorithm(grid, { startX, startY }, { endX, endY });

		if (path.empty()) {
			// Dacă calea este goală, folosește desenarea alternativă
			float offset = 20.0f; // Offset-ul pentru linia alternativă
			Vector2 temp1 = { point1.x, point1.y + offset };
			Vector2 temp2 = { point2.x, point2.y - offset };

			DrawLineEx(point1, temp1, 5.0f, ORANGE);
			DrawLineEx(temp1, { (point1.x + point2.x) / 2, point1.y + offset }, 5.0f, ORANGE);
			DrawLineEx({ (point1.x + point2.x) / 2, point1.y + offset },
				{ (point1.x + point2.x) / 2, point2.y - offset }, 5.0f, ORANGE);
			DrawLineEx({ (point1.x + point2.x) / 2, point2.y - offset }, temp2, 5.0f, ORANGE);
			DrawLineEx(temp2, point2, 5.0f, ORANGE);
		}
		else {
			// Desenare linie utilizând calea generată
			for (size_t i = 0; i < path.size() - 1; ++i) {
				Vector2 p1 = path[i];
				Vector2 p2 = path[i + 1];
				DrawLineEx(p1, p2, 5.0f, ORANGE);
			}
		}
	}
}

//DrawLineEx(point1, temp1, 3.0f, ORANGE);
//DrawLineEx(temp1, { (point1.x + point2.x) / 2, point1.y + offset }, 3.0f, ORANGE);
//DrawLineEx({ (point1.x + point2.x) / 2,point1.y + offset }, { (point1.x + point2.x) / 2,point2.y - offset }, 3.0f, ORANGE);
//DrawLineEx({ (point1.x + point2.x) / 2, point2.y - offset }, temp2, 3.0f, ORANGE);
//DrawLineEx(temp2, point2, 3.0f, ORANGE);