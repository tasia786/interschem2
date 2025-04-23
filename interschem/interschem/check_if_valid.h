#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
using namespace std;

bool isBalanced(const string& expr);
bool isElementaryFunction(const string& token);
bool isValidToken(const string& token);
bool isValidExpression(const string& expr);
bool isFunctionCall(const string& token, size_t& i);

bool isBalanced(const string& expr)
{
    stack<char> S;
    for (char ch : expr) {
        if (ch == '(') {
            S.push(ch);
        }
        else if (ch == ')') {
            if (S.empty()) return false;
            S.pop();
        }
    }
    return S.empty();
}

bool isElementaryFunction(const string& token)
{
    return (token=="not" or token == "sin" or token == "cos" or token == "tan" or token == "ctg" or token == "log" or token == "sqrt");
}

bool isFunctionCall(const string& token, size_t& i)
{
    size_t len = token.size();
    string func;

    // Colectam numele functiei
    while (i < len and isalpha(token[i]))
    {
        func += token[i];
        i++;
    }

    // Verificam daca este o functie elementara
    if (isElementaryFunction(func))
    {
        if (i < len and token[i] == '(')
        {
            i++;
            string argument;
            int paranthesisCount = 1;

            // Colectam argumentul functiei pana gasim paranteza inchisa
            while (i < len and paranthesisCount > 0)
            {
                if (token[i] == '(')
                    paranthesisCount++;
                else
                    if (token[i] == ')')
                        paranthesisCount--;

                if (paranthesisCount > 0)
                    argument += token[i];
                i++;
            }

            if (paranthesisCount == 0) {
                return isValidExpression(argument);  // Verificam daca argumentul este o expresie valida
            }
        }
    }

    return false;
}

bool isValidToken(const string& token)
{
    if (token.empty()) return false;

    size_t len = token.size();
    size_t i = 0;

    // Verificam daca este o variabila (de exemplu, A, B, C, etc.)
    if (len == 1 and token[0] >= 'A' and token[0] <= 'Z') {
        return true;
    }

    if (token[0] == '-' or token[0] == '+' or token[0] == '!' and len > 1)
        i++;

    // Verificam daca este o functie elementara (sin, cos, etc. )
    if (isalpha(token[i])) {
        return isFunctionCall(token, i);
    }

    // Verificam daca este un numar valid
    bool allDigits = true;
    int point_count = 0;
    if (token[0] == '.')
    {
        i++;
        point_count++;
    }
    if (token[0] == '-' or token[0] == '+' or token[0] == '!') {
        if (len == 1) return false;
        if (token[1] == '0' and len > 2 and token[2] != '.') return false;
        if (token[1] == '.') {
            i++;
            point_count++;
        }
        i++;
    }

    // Daca numarul incepe cu 0 si are mai mult de un caracter, este invalid
    if (token[i] == '0' and len > i + 1 and token[i + 1] != '.') return false;

    // Verificam daca restul tokenului sunt doar cifre
    for (; i < len and point_count <= 1; i++) {
        if (token[i] == '.') {
            i++;
            point_count++;
        }
        if (!isdigit(token[i]) and i < len) {
            allDigits = false;
            break;
        }
        if (point_count > 1) {
            allDigits = false;
            break;
        }
    }

    // Daca toate caracterele sunt cifre, este un numar valid
    if (allDigits) return true;

    // Verificam daca este o expresie intre paranteze
    if (token[0] == '(' and token[len - 1] == ')') {
        string innerExpr = token.substr(1, len - 2);
        return isValidExpression(innerExpr); // verificam expresia interna
    }

    return false;
}

bool isValidExpression(const string& expr) {
    if (!isBalanced(expr)) return false;

    size_t len = expr.size();
    size_t i = 0;

    while (i < len) {
        string token;

        // Colectam caracterele pana la un operator
        int paranthesisCount = 0;
        while (i < len) {
            if (expr[i] == '(')
                paranthesisCount++;
            else if (expr[i] == ')')
                paranthesisCount--;
            else if ((expr[i] == '+' or expr[i] == '-' or expr[i] == '*' or expr[i] == '/' or expr[i] == '!') and paranthesisCount == 0) {
                if ((expr[i] == '-' or expr[i] == '+' or expr[i] == '!') && (i == 0 or expr[i - 1] == '(')) {
                    token += expr[i];
                    i++;
                }
                else break;
            }
            // Verificam operatorii logici && si ||
            else if (expr.size() >= i + 2)
                if ((expr.substr(i, 2) == "&&" or expr.substr(i, 2) == "||") and paranthesisCount == 0) {
                    break;
                }

            token += expr[i];
            ++i;
        }

        //cout << "current token: " << token << endl;
        // Verificam token-ul curent
        if (!isValidToken(token)) return false;

        // Trece peste operatorii aritmetici si logici
        if (i < len and (expr[i] == '+' or expr[i] == '-' or expr[i] == '*' or expr[i] == '/' or expr[i] == '!'))
            ++i;
        else if (expr.size() >= 2 and i < len and (expr.substr(i, 2) == "&&" or expr.substr(i, 2) == "||"))
            i += 2;
    }
    if (expr[len - 1] == '+' or expr[len - 1] == '-' or expr[len - 1] == '*' or expr[len - 1] == '/' or expr[len - 1] == '!' or
        (len >= 2 and (expr.substr(len - 2, 2) == "&&" or expr.substr(len - 2, 2) == "||")))
        return false; // Expresia nu trebuie sa se termine cu un operator
    return true;
}

bool isValidValue(string expr)
{
    if (expr.empty()) return false;

    size_t i = 0;
    size_t len = expr.size();
    bool hasDigits = false;
    int pointCount = 0;

    // Verificăm semnul unar
    if (expr[i] == '-' or expr[i] == '+') {
        i++;
        if (i == len) return false; // Un string cu doar "+" sau "-" nu este valid
    }

    if (expr[i] == '0' && i + 1 < len && expr[i + 1] != '.') {
        return false; 
    }

    for (; i < len; i++) {
        if (isdigit(expr[i])) {
            hasDigits = true;
        }
        else if (expr[i] == '.') {
            pointCount++;
            if (pointCount > 1) return false; // Mai mult de un punct
        }
        else {
            return false; // Caracter invalid
        }
    }

    // Validăm dacă avem cel puțin o cifră
    return hasDigits;
}

bool check_type3(string expression) //atribuire
{
    int operand = expression.find("<-");
    if (operand == string::npos) return false;
    else {
        if (operand != 1 or expression[0] < 'A' or expression[0]>'Z') return false;
        string term2 = expression.substr(3, expression.size());
        if (!isValidExpression(term2)) return false;
    }
    return true;
}

bool check_type4(string expression) {
    size_t operand = string::npos;
    int len = 0;

    // Căutăm primul operator
    if (expression.empty())
        return false;
    if (expression.find("<=") != string::npos) {
        operand = expression.find("<=");
        len = 2;
    }
    else if (expression.find(">=") != string::npos) {
        operand = expression.find(">=");
        len = 2;
    }
    else if (expression.find("==") != string::npos) {
        operand = expression.find("==");
        len = 2;
    }
    else if (expression.find("!=") != string::npos) {
        operand = expression.find("!=");
        len = 2;
    }
    else if (expression.find('<') != string::npos) {
        operand = expression.find('<');
        len = 1;
    }
    else if (expression.find('>') != string::npos) {
        operand = expression.find('>');
        len = 1;
    }

    if (len == 0 and isValidExpression(expression)) return true;

    // Dacă nu găsim niciun operator, nu este valid
    if (operand == string::npos) return false;

    // Împărțim expresia în termeni
    string term1 = expression.substr(0, operand);
    string term2 = expression.substr(operand + len);

    if (term1.empty() or term2.empty()) return false;

    // Verificăm termenii
    if (!isValidExpression(term1) || !isValidExpression(term2)) return false;

    return true;
}


bool check_type5(string expression) //afisare
{
    if (expression.empty()) return false;
    if (isValidExpression(expression) or expression[0] == '"' and expression[expression.size() - 1] == '"') return true;
    return false;
}

