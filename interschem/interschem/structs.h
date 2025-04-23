#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
using namespace std;

struct block
{
    int type;
    int index;
    char var = 'a'; //numele variabilei
    block* left = NULL;
    block* right = NULL;
    bool read = 0; //daca a fost introdus sirul de caractere !=NULL
    bool next = 0; //pentru if, sa stiu daca adaug stg sau dr
    int verified = 0; //pentru verificarea structurii

    float x, y;
    float width, height;
    bool isDragging=false;
    string text;
    Color color;
};



struct Connection {
	int blockIndex1, pointIndex1; // Primul bloc și punctul de conectare
	int blockIndex2, pointIndex2; // Al doilea bloc și punctul de conectare
};

vector<block> shapes;
vector<Connection> connections;
int idx = 0;
bool impossible_op= 0;

string output;

Color Dark_Cyan = Color{ 0, 25, 25, 255 };
Color Dark_Red = Color{204,0,0,255};
Color Light_Red = Color{255,102,102,255};
Color Dark_Green = Color{0,153,0,255};
Color Light_Green = Color{0,255,0,255};

int screenWidth=2880;
int screenHeight=1800;

Rectangle space = { 350, 80, screenWidth-965,screenHeight-150 };
