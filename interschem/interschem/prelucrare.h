#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cmath>
#include <stdio.h>
#include "structs.h"
using namespace std;

float values[30]; //valorile variabilelor A,B,...,Z 

void create_connection(int index1, int index2)
{
	if (shapes[index1].type == 4)
	{
		if (shapes[index1].next == 0)
		{
			shapes[index1].left = &shapes[index2];
			shapes[index1].next = 1 - shapes[index1].next;
		}
		else
		{
			shapes[index1].right = &shapes[index2];
			shapes[index1].next = 1 - shapes[index1].next;
		}
	}
	else
		shapes[index1].left = &shapes[index2];
}

void delete_connection(block* p)
{
	for (int i = 0; i < shapes.size(); i++)
	{
		if (shapes[i].left == p)
			shapes[i].left = NULL;
		if (shapes[i].right == p)
			shapes[i].right = NULL;
	}
}

float ctg(float x)
{
	return 1.0 / tan(x);
}

float basic_expression(float nr1, float nr2, char op)
{
	if ((op == '/' || op == '%') && nr2 == 0)
	{
		impossible_op = true;
		return -1;
	}

	switch (op)
	{
	case '+':
		return nr1 + nr2;
	case '-':
		return nr1 - nr2;
	case '*':
		return nr1 * nr2;
	case '/':
		return nr1 / nr2;
	case '%':
		return (int(nr1) % int(nr2)) * 1.0;
	case '>':
		return nr1 > nr2;
	case '<':
		return nr1 < nr2;
	case '=':
		return nr1 == nr2;
	case '!':
		return nr1 != nr2;
	case '&':
		return nr1 && nr2;
	case '|':
		return nr1 || nr2;
	case 'm':
		return nr1 >= nr2;
	case 'w':
		return nr1 <= nr2;
	}
	return 0;
}

float basic_unary_expression(float nr, char op) ///g = ctg, q = sqrt
{
	if ((nr == 0 && op == 'g') || (nr <= 0 && op == 'l') || (nr < 0 && op == 'q'))
	{
		impossible_op = true;
		return -1;
	}

	switch (op)
	{
	case 'a':
		return nr;
	case 'b':
		return -nr;
	case 's':
		return sin(nr);
	case 'c':
		return cos(nr);
	case 't':
		return tan(nr);
	case 'g':
		return ctg(nr);
	case 'l':
		return log(nr);
	case 'q':
		return sqrt(nr);
	case 'n':
		return !nr;

	}
}

int order_oper(char op)
{
	if (op == 'n' || op == 'a' || op == 'b')
		return 6;
	if (op == '*' || op == '/' || op == '%')
		return 5;
	if (op == '+' || op == '-')
		return 4;
	if (op == '>' || op == '<' || op == 'm' || op == 'w')
		return 3;
	if (op == '=' || op == '!')
		return 2;
	if (op == '&')
		return 1;
	if (op == '|')
		return 0;
	return 7; //pentru sqrt, sin - la !sin(x) mai intai fac sin(x)
}

char operator_symbol(string s, int start, int stop)
{
	char op = s[start];
	if (stop > start)
	{
		if (s[start] == 'c' && s[start + 1] == 't')
			op = 'g';
		if (s[start] == 's' && s[start + 1] == 'q')
			op = 'q';
		if (s[start] == '>' && s[start + 1] == '=')
			op = 'm';
		if (s[start] == '<' && s[start + 1] == '=')
			op = 'w';
	}
	return op;
}

float calculate_expression(string s)
{
	stack <float> valori;
	stack <char> operatori;

	int start = 0, stop = s.size() - 1;

	//pentru + - unar
	if (s[0] == '+')
		s[0] = 'a';
	if (s[0] == '-')
		s[0] = 'b';

	for (int i = start; i < stop; i++)
	{
		if (s[i] == '(' && s[i + 1] == '+')
			s[i + 1] = 'a';
		if (s[i] == '(' && s[i + 1] == '-')
			s[i + 1] = 'b';
	}


	for (int i = start; i <= stop; i++) ///merge inclusiv pana la stop
	{
		if (s[i] == ' ')
			continue;
		else if (s[i] >= 'A' && s[i] < 'Z')
		{
			int index = s[i] - 'A';
			valori.push(values[index]);
		}
		else if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.')
		{
			float nr = 0;
			while (s[i] >= '0' && s[i] <= '9' && i <= stop)
			{
				nr = nr * 10 + int(s[i] - '0');
				i++;
			}

			if (s[i] == '.' && i < stop)
			{
				i++;
				float aux = 1.0 / 10;
				while (s[i] >= '0' && s[i] <= '9' && i <= stop)
				{
					nr = nr + aux * int(s[i] - '0');
					aux = aux * 1.0 / 10;
					i++;
				}
			}

			i--;
			valori.push(nr);
		}
		else if (s[i] == '(')
			operatori.push(s[i]);
		else if (s[i] == ')')
		{
			while (!operatori.empty() && operatori.top() != '(') ///ordinea
			{
				bool ok = 0;
				char op = operatori.top();
				operatori.pop();

				float nr1 = valori.top();
				valori.pop();

				if ((op >= 'a' && op <= 'z' && op != 'w' && op != 'm'))
				{
					float rezultat = basic_unary_expression(nr1, op);
					valori.push(rezultat);
					ok = 1;
				}

				if (ok == 0)
				{
					float nr2 = valori.top();
					valori.pop();
					float rezultat = basic_expression(nr2, nr1, op);
					valori.push(rezultat);
				}

			}
			if (!operatori.empty())
				operatori.pop();
		}
		else
		{
			char op_nou = operator_symbol(s, i, stop);
			if (s[i] >= 'a' && s[i] <= 'z')
			{
				while (s[i] >= 'a' && s[i] <= 'z' && i <= stop)      //pentru sin cos ctg ln sqrt
					i++;
				i--;
			}
			if (op_nou == '&' || op_nou == '|' || op_nou == '!' || op_nou == '=' || op_nou == 'm' || op_nou == 'w')
				i++;                                    //pentru expr boolene

			while (!operatori.empty() && operatori.top() != '(' && order_oper(operatori.top()) >= order_oper(op_nou))
			{
				bool ok = 0;
				char op = operatori.top();
				operatori.pop();

				float nr1 = valori.top();
				valori.pop();

				if ((op >= 'a' && op <= 'z' && op != 'w' && op != 'm'))
				{
					float rezultat = basic_unary_expression(nr1, op);
					valori.push(rezultat);
					ok = 1;
				}

				if (ok == 0)
				{
					float nr2 = valori.top();
					valori.pop();
					float rezultat = basic_expression(nr2, nr1, op);
					valori.push(rezultat);
				}
			}
			operatori.push(op_nou);
		}
	}

	while (!operatori.empty())
	{
		bool ok = 0;
		char op = operatori.top();
		operatori.pop();

		float nr1 = valori.top();
		valori.pop();

		if ((op >= 'a' && op <= 'z' && op != 'w' && op != 'm'))
		{
			float rezultat = basic_unary_expression(nr1, op);
			valori.push(rezultat);
			ok = 1;
		}

		if (ok == 0)
		{
			float nr2 = valori.top();
			valori.pop();
			float rezultat = basic_expression(nr2, nr1, op);
			valori.push(rezultat);
		}
	}
	return valori.top();
}

void prelucrate_type2(block b)
{
	int index = b.var - 'A';
	values[index] = calculate_expression(b.text);
}

void prelucrate_type3(block b)
{
	int index = b.text[0] - 'A';
	values[index] = calculate_expression(b.text.substr(3));
}

bool prelucrate_type4(block b)
{
	return calculate_expression(b.text);
}

void prelucrate_type5(block b)
{
	int i = 0;
	while (b.text[i] == ' ') // Ignoră spațiile la început
		i++;

	string result;
	if (b.text[i] == '"') {
		int last = b.text.size() - 1;
		while (b.text[last] == ' ') // Elimină spațiile de la final
			last--;
		b.text[last] = 0; // elimină caracterul de închidere "
		result = b.text.substr(i + 1);
	}
	else {
		result = to_string(calculate_expression(b.text));
	}
	result += " ";

	// Poziția de start și lățimea maximă
	float startX = screenWidth / 1.244;
	float maxWidth = screenWidth - startX - 45;

	output += result;

	//adding \n
	string temp;
	int cnt = 0;
	for (auto ch : output)
	{
		temp += ch;
		if (MeasureText(temp.c_str(), 35) > maxWidth)
		{
			output = output.substr(0, cnt) + '\n' + output.substr(cnt);
			temp = output.substr(cnt);
		}
		cnt++;
	}

	output.erase(remove(output.begin(), output.end(), '\0'), output.end());
	//cout << output;
}

block* search_for_start_button(vector<block> blocks)
{
	for (int i = 0; i < blocks.size(); i++)
		if (blocks[i].type == 1)
			return blocks[i].left;
	return NULL;
}

void initialise_verified_to_0(vector<block>& shapes) ///besides the start one
{
	for (int i = 0; i < shapes.size(); i++)
		if (shapes[i].type != 1)
			shapes[i].verified = 0;
		else
			shapes[i].verified = 1;
}

bool verify_stop_blocks(block* p, int& nr) //numar si cate stopuri sunt sa fie cel putin 1
{
	if (p == NULL)
		return false;
	if (p->verified == 0)
	{
		p->verified = 1;
		if (p->type == 6)
		{
			nr++;
			return true;
		}
		if (p->type == 4)
			return (verify_stop_blocks(p->left, nr) && verify_stop_blocks(p->right, nr));
		return verify_stop_blocks(p->left, nr);
	}
	return 1;
}

bool verify_shapes(vector<block>& shapes)
{
	initialise_verified_to_0(shapes);
	block* p = search_for_start_button(shapes);
	if (p == NULL)
		return false;
	int nr = 0;
	bool ok = verify_stop_blocks(p, nr);
	return ok && (nr >= 1);
}

void compile(vector <block>& shapes)
{
	if (verify_shapes(shapes))
	{
		block* p = search_for_start_button(shapes);
		while (p != NULL)
		{
			bool ok = 0;
			switch (p->type)
			{
			case 2:
				prelucrate_type2(*p);
				break;
			case 3:
				prelucrate_type3(*p);
				break;
			case 4:
			{
				if (prelucrate_type4(*p))
					p = p->left;
				else p = p->right;
				ok = 1;
			}
			break;
			case 5:
				prelucrate_type5(*p);
				break;
			}
			if (ok == 0)
				p = p->left;
		}
	}
	else {
		output = "input incorect";
		cout << endl << "input incorect" << endl;
	}
}

int was_read[30];

void align_text(int aliniere, FILE* fout)
{
	for (int i = 1; i <= aliniere; i++)
		fprintf(fout, " ");
}

void align_text_2(FILE* fout)
{
	for (int i = 1; i <= 4; i++)
		fprintf(fout, " ");
}


int is_loop(vector<block>& shapes, block* start, block* p)  //la apelul initial p=start->left
{
	int val = p->verified;
	if (p == start)
		return 1;
	if (p->type == 6)
		return 0;
	if (p->verified == 2)
		return -1;
	p->verified = 2;
	int rez;
	if (p->type == 4)
		rez = is_loop(shapes, start, p->right);
	else rez = is_loop(shapes, start, p->left);
	p->verified = val;
	return rez;
} //1 e bucla, 0 nu e, -1 eroare (nu are echivalent)

void initialise_for_loop(vector<block>& shapes, block* start)
{
	block* p = start->left;
	while (p != start)
	{
		p->verified = 0;
		if (p->type == 4)
			p = p->right;
		else p = p->left;
	}
}                   ///cazul cand de asupra este un if sau while si nu simplu liniar

bool write_code(vector<block>& shapes, block* p, block* loop, int aliniere, FILE* fout) // loop = the address of the loop, null if not
{
	if (p == loop)
		return 1;
	if (p->verified == 1)
		return 0;
	p->verified = 1;

	switch (p->type)
	{
	case 1:
		return write_code(shapes, p->left, loop, aliniere, fout);
		break;

	case 2:
	{
		align_text(aliniere, fout);
		fprintf(fout, "float %c = %s;\n", p->var, p->text.c_str());
		return write_code(shapes, p->left, loop, aliniere, fout);
	}
	break;

	case 3:
	{
		align_text(aliniere, fout);
		fprintf(fout, "%c = %s;\n", p->text[0], p->text.substr(3).c_str());
		return write_code(shapes, p->left, loop, aliniere, fout);
	}
	break;

	case 4:
	{
		int val = is_loop(shapes, p, p->left);
		int nr1, nr2;

		if (val == 1)
		{
			initialise_for_loop(shapes, p);
			align_text(aliniere, fout);
			fprintf(fout, "while (%s)\n", p->text.c_str());
			align_text(aliniere, fout);
			fprintf(fout, "{\n");
			nr1 = write_code(shapes, p->left, p, aliniere + 4, fout);
			align_text(aliniere, fout);
			fprintf(fout, "}\n");
			nr2 = write_code(shapes, p->right, loop, aliniere, fout);
		}
		else if (val == 0)
		{
			align_text(aliniere, fout);
			fprintf(fout, "if (%s)\n", p->text.c_str());
			align_text(aliniere, fout);
			fprintf(fout, "{\n");
			nr1 = write_code(shapes, p->left, loop, aliniere + 4, fout);
			align_text(aliniere, fout);
			fprintf(fout, "}\n");
			align_text(aliniere, fout);
			fprintf(fout, "else\n");
			align_text(aliniere, fout);
			fprintf(fout, "{\n");
			nr2 = write_code(shapes, p->right, loop, aliniere + 4, fout);
			align_text(aliniere, fout);
			fprintf(fout, "}\n");
		}
		else
		{
			return 0;
		}
		return nr1 && nr2;
	}
	break;

	case 5:
	{
		align_text(aliniere, fout);
		fprintf(fout, "cout << %s;\n", p->text.c_str());
		return write_code(shapes, p->left, loop, aliniere, fout);
	}
	break;

	case 6:
	{
		align_text(aliniere, fout);
		fprintf(fout, "return 0;\n");
		return 1;
	}
	break;
	}
	return 1;
}

bool ctg_used;
void search_for_undeclared_variables(vector<block>& shapes, block* p)
{
	if (p->type == 6 || p->verified == 1)
		return;
	p->verified = 1;

	//pt ctg
	if (p->type != 1 && p->type != 6)
	{
		int j = 0;
		while (p->text[j] == ' ')
			j++;
		if (p->text[j] != '"')
			for (int i = 0; i < p->text.size() - 1; i++)
				if (p->text[i] == 'c' && p->text[i + 1] == 't')
					ctg_used = 1;
	}

	//pt variabile
	if (p->type == 2)
	{
		int index = p->var - 'A';
		was_read[index] = 1;
	}
	else
	{
		int j = 0;
		while (p->text[j] == ' ')
			j++;
		if (p->type != 5 || p->text[j] != '"')
		{
			for (int i = 0; i < p->text.size(); i++)
				if (p->text[i] >= 'A' && p->text[i] <= 'Z')
				{
					int index = p->text[i] - 'A';
					if (was_read[index] == 0)
						was_read[index] = 2;
				}
		}
	}
	if (p->type == 4)
	{
		search_for_undeclared_variables(shapes, p->right);
		search_for_undeclared_variables(shapes, p->left);
	}
	else search_for_undeclared_variables(shapes, p->left);
}

bool check[500];
void check_if_returns(vector<block>& shapes, block* p)
{
	if (p == NULL)
		return;
	if (p->verified == 1)
		check[p->index] = 1;
	else
	{
		p->verified = 1;
		check_if_returns(shapes, p->left);
		if (p->type == 4)
		{
			check[p->right->index] = 1;
			check_if_returns(shapes, p->right);
		}
	}
}

int index_of_start(vector<block>& shapes)
{
	for (int i = 0; i < shapes.size(); i++)
		if (shapes[i].type == 1)
			return shapes[i].index;
	return -1;
}

void write_code_2(vector<block>& shapes, block* p, FILE* fout)
{
	if (p->verified == 1)
	{
		align_text_2(fout);
		if (p->type == 1)
			fprintf(fout, "goto start;\n");
		else
			fprintf(fout, "goto eticheta%d;\n", p->index);
		return;
	}
	else
	{
		p->verified = 1;
		if (check[p->index])
		{
			align_text_2(fout);
			fprintf(fout, "eticheta%d:\n", p->index);
		}

		switch (p->type)
		{
		case 2:
		{
			align_text_2(fout);
			fprintf(fout, "float %c = %s;\n", p->var, p->text.c_str());
			write_code_2(shapes, p->left, fout);
		}
		break;

		case 3:
		{
			align_text_2(fout);
			fprintf(fout, "%c = %s;\n", p->text[0], p->text.substr(3).c_str());
			write_code_2(shapes, p->left, fout);
		}
		break;

		case 4:
		{
			align_text_2(fout);
			fprintf(fout, "if (!(%s))\n", p->text.c_str());
			align_text_2(fout);
			align_text_2(fout);
			fprintf(fout, "goto eticheta%d;\n", p->right->index);

			write_code_2(shapes, p->left, fout);
			align_text_2(fout);
			fprintf(fout, "goto final;\n");

			write_code_2(shapes, p->right, fout);
		}
		break;

		case 5:
		{
			align_text_2(fout);
			fprintf(fout, "cout << %s;\n", p->text.c_str());
			write_code_2(shapes, p->left, fout);
		}
		break;

		case 6:
		{
			align_text_2(fout);
			fprintf(fout, "return 0;\n");
		}
		break;
		}
	}
}

bool ok; //1 fisierul cod, 0 fisierul cod2
void create_code(vector<block>& shapes)
{
	if (!verify_shapes(shapes))
	{
		FILE* fout = fopen("cod.txt", "w+");
		if (!fout)
			return; // de afisat eroare
		fprintf(fout, "input incorect");
		fclose(fout);
	}
	else
	{
		FILE* fout = fopen("cod.txt", "w+");
		if (!fout)
			return; // de afisat eroare
		initialise_verified_to_0(shapes);
		fprintf(fout, "#include <iostream> \n#include <cmath> \nusing namespace std; \n\n");

		block* start = search_for_start_button(shapes);

		for (int i = 0; i < 30; i++)
			was_read[i] = 0;
		ctg_used = 0;
		search_for_undeclared_variables(shapes, start);

		if (ctg_used == 1)
			fprintf(fout, "float ctg(float x)\n{\n    return 1.0 / tan(x);\n}\n\n");

		int nr = 0;
		for (int i = 0; i < 30; i++)
			if (was_read[i] == 2)
				nr++;
		if (nr)
		{
			fprintf(fout, "float ");
			int i = 0, cnt = 0;
			for (i = 0, cnt = 0; i < 30 && cnt < nr - 1; i++)
				if (was_read[i] == 2)
				{
					fprintf(fout, "%c, ", char('A' + i));
					cnt++;
				}
			while (i < 30 && cnt < nr)
			{
				if (was_read[i] == 2)
				{
					fprintf(fout, "%c;", char('A' + i));
					cnt++;
				}
				i++;
			}
			fprintf(fout, "\n");
		}
		initialise_verified_to_0(shapes);
		fprintf(fout, "int main() {\n");

		ok = write_code(shapes, start, NULL, 4, fout);
		fprintf(fout, "}");

		if (ok == 0)
		{
			fclose(fout); // Close file
			fout = fopen("cod.txt", "w");
			if (!fout)
				return; // de afisat eroare pe ecran

			initialise_verified_to_0(shapes);
			fprintf(fout, "#include <iostream> \n#include <cmath> \nusing namespace std; \n\n");

			block* start = search_for_start_button(shapes);

			for (int i = 0; i < 30; i++)
				was_read[i] = 0;
			ctg_used = 0;

			search_for_undeclared_variables(shapes, start);

			if (ctg_used == 1)
				fprintf(fout, "float ctg(float x)\n{\n    return 1.0 / tan(x);\n}\n\n");

			nr = 0;
			for (int i = 0; i < 30; i++)
				if (was_read[i] == 2)
					nr++;
			if (nr)
			{
				fprintf(fout, "float ");
				int i = 0, cnt = 0;
				for (i = 0, cnt = 0; i < 30 && cnt < nr - 1; i++)
					if (was_read[i] == 2)
					{
						fprintf(fout, "%c, ", char('A' + i));
						cnt++;
					}
				while (i < 30 && cnt < nr)
				{
					if (was_read[i] == 2)
					{
						fprintf(fout, "%c;", char('A' + i));
						cnt++;
					}
					i++;
				}
				fprintf(fout, "\n");
			}
			initialise_verified_to_0(shapes);
			fprintf(fout, "int main() {\n");

			for (int i = 0; i < 500; i++)
				check[i] = 0;
			initialise_verified_to_0(shapes);
			check_if_returns(shapes, start);

			int index_start = index_of_start(shapes);
			if (check[index_start])
			{
				align_text_2(fout);
				fprintf(fout, "start:\n");
			}

			initialise_verified_to_0(shapes);
			write_code_2(shapes, start, fout);

			align_text_2(fout);
			fprintf(fout, "final:\n");
			align_text_2(fout);
			fprintf(fout, "return 0;\n");
			fprintf(fout, "}");

		}
		fclose(fout);
	}
}

void save()
{
	FILE* file = fopen("save.txt", "wb");
	if (!file)
	{
		//perror("Failed to open file");
		cout << "eroare";
		return;
	}

	int numElements = shapes.size();
	fwrite(&numElements, sizeof(int), 1, file);

	for (int i = 0; i < numElements; i++)
	{
		fwrite(&shapes[i].type, sizeof(int), 1, file);
		fwrite(&shapes[i].index, sizeof(int), 1, file);
		fwrite(&shapes[i].var, sizeof(char), 1, file);
		fwrite(&shapes[i].x, sizeof(float), 1, file);
		fwrite(&shapes[i].y, sizeof(float), 1, file);
		fwrite(&shapes[i].width, sizeof(float), 1, file);
		fwrite(&shapes[i].height, sizeof(float), 1, file);

		int textLength = shapes[i].text.size();
		fwrite(&textLength, sizeof(int), 1, file);
		fwrite(shapes[i].text.c_str(), sizeof(char), textLength, file);
	}
	int numConnections = connections.size();
	fwrite(&numConnections, sizeof(int), 1, file);

	for (int i = 0; i < numConnections; i++)
	{
		fwrite(&connections[i].blockIndex1, sizeof(int), 1, file);
		fwrite(&connections[i].pointIndex1, sizeof(int), 1, file);
		fwrite(&connections[i].blockIndex2, sizeof(int), 1, file);
		fwrite(&connections[i].pointIndex2, sizeof(int), 1, file);
	}
	fclose(file);
}

void load()
{
	FILE* file = fopen("save.txt", "rb");
	if (!file)
	{
		// perror("Failed to open file");
		cout << "eroare";
		return;
	}

	shapes.clear();

	int numElements = 0;
	fread(&numElements, sizeof(int), 1, file);

	for (int i = 0; i < numElements; i++)
	{
		block temp;

		fread(&temp.type, sizeof(int), 1, file);
		fread(&temp.index, sizeof(int), 1, file);
		fread(&temp.var, sizeof(char), 1, file);
		fread(&temp.x, sizeof(float), 1, file);
		fread(&temp.y, sizeof(float), 1, file);
		fread(&temp.width, sizeof(float), 1, file);
		fread(&temp.height, sizeof(float), 1, file);

		int textLength = 0;
		fread(&textLength, sizeof(int), 1, file);

		char* textBuffer = new char[textLength + 1];
		fread(textBuffer, sizeof(char), textLength, file);
		textBuffer[textLength] = '\0';

		temp.text = string(textBuffer);
		delete[] textBuffer;

		switch (temp.type)
		{
		case 1:
			temp.color = GREEN;
			break;
		case 2:
			temp.color = YELLOW;
			break;
		case 3:
			temp.color = SKYBLUE;
			break;
		case 4:
			temp.color = PINK;
			break;
		case 5:
			temp.color = MAGENTA;
			break;
		case 6:
			temp.color = RED;
			break;
		}
		shapes.push_back(temp);
	}

	// Actualizează idx după încărcarea blocurilor
	idx = shapes.size();  // Setează idx corect pentru a reflecta dimensiunea actuală

	connections.clear();

	int numConnections = 0;
	fread(&numConnections, sizeof(int), 1, file);

	for (int i = 0; i < numConnections; i++)
	{
		Connection conn;
		fread(&conn.blockIndex1, sizeof(int), 1, file);
		fread(&conn.pointIndex1, sizeof(int), 1, file);
		fread(&conn.blockIndex2, sizeof(int), 1, file);
		fread(&conn.pointIndex2, sizeof(int), 1, file);
		connections.push_back(conn);
	}

	// Actualizează idx cu valoarea finală din fișier
	fclose(file);
}
