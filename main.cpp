#include <iostream>
#include <string>

using namespace std;

struct ATerm
{
    int Coefficient;
    int Exponent;
    ATerm* Next;
};
typedef ATerm*  Polynomial;

const int MAX_NUM_POLYNOMIALS = 10;

void getPolynomials(string[], int&);
bool validateNum(string);
bool validatePolynomial(string, int);
bool checkCharacters(string);
bool checkSyntax(string);
bool checkExponentOrder(string);
void removeSpaces(string&);
char toUpper(char);
void parseStringPolynomials(string[], Polynomial[], int);
int parseCoefficient(string, int);
int parseExponent(string, int);
void appendTerm(Polynomial&, int, int);
Polynomial Add(Polynomial, Polynomial);
Polynomial Multipy(Polynomial, Polynomial);
Polynomial multiplyTerm(Polynomial, Polynomial);
Polynomial Copy(Polynomial);
void displayAddition(Polynomial, Polynomial[],  int);
void displayMultiplication(Polynomial, Polynomial[],  int);
void displayPolynomial(Polynomial);

int main()
{
    Polynomial heads[MAX_NUM_POLYNOMIALS];
    Polynomial headSum = nullptr, headProduct = nullptr;
    string polynomials[MAX_NUM_POLYNOMIALS];
    int numPolynomials;

    getPolynomials(polynomials, numPolynomials);
    parseStringPolynomials(polynomials, heads, numPolynomials);

    cout << polynomials[0] << "///" << endl;

    for (int i = 0; i < numPolynomials; i++)
    {
        headSum = Add(heads[i], headSum);
    }

    for (int i = 0; i < numPolynomials; i++)
    {
        headProduct = Multipy(heads[i], headProduct);
    }

    displayAddition(headSum, heads, numPolynomials);
    displayMultiplication(headProduct, heads, numPolynomials);

    return 0;
}

//===============================================================================
//Lets the user input the number of polynomials and, then, each polynomial
//===============================================================================

void getPolynomials(string polynomials[], int& numPolynomials)
{
    string sNumPolynomials;

    cout << "Number of polynomials -> ";
    getline(cin, sNumPolynomials);

    while (!validateNum(sNumPolynomials) || stoi(sNumPolynomials) < 1 || stoi(sNumPolynomials) > 10)
    {
        cout << "Error! Number of polynomials -> ";
        getline(cin, sNumPolynomials);
    }

    numPolynomials = stoi(sNumPolynomials);

    for (int i = 0; i < numPolynomials; i++)
    {
        cout << "Input polynomial " << i + 1 << " -> ";
        getline(cin, polynomials[i]);
        removeSpaces(polynomials[i]);

        while (!validatePolynomial(polynomials[i], numPolynomials))
        {
            cout << "Error! Input polynomial " << i + 1 << " -> ";
            getline(cin, polynomials[i]);
            removeSpaces(polynomials[i]);
        }
    }
}

//===============================================================================
//Checks whether an input contains only numeric characters
//Returns true if the string is composed of only characters '0' through '9'
//Returns false otherwise
//===============================================================================

bool validateNum(string num)
{
    int length = num.length();

    for (int i = 0; i < length; i++)
    {
        if (num[i] < '0' || num[i] > '9')
        {
            return false;
        }
    }

    return true;
}

//===============================================================================
//Checks whether an input is a valid polynomial according to the specification
//Returns true if the input is valid
//Returns false otherwise
//===============================================================================

bool validatePolynomial(string polynomial, int numPolynomials)
{
    bool valid = true;

    if (!checkCharacters(polynomial) || !checkSyntax(polynomial) || !checkExponentOrder(polynomial))
    {
      valid = false;
    }

    return valid;
}

//===============================================================================
//Checks whether an input contains characters that are not allowed in a polynomial
//Returns true if the input is any combination of these characters: spaces, numbers, '+',
// '-', 'X', 'x'
//Returns false otherwise
//===============================================================================

bool checkCharacters(string polynomial)
{
    bool correct = true;
    int length = polynomial.length();

    if (length == 0)
    {
        correct = false;
    }

    for (int i = 0; i < length && correct; i++)
    {
        if (polynomial[i] != ' ' && polynomial[i] != 'x' && polynomial[i] != 'X' && polynomial[i] != '^'
            && polynomial[i] != '+' && polynomial[i] != '-' && (polynomial[i] < '0' || polynomial[i] > '9'))
        {
            correct = false;
        }
    }

    return correct;
}

//===============================================================================
//Checks whether an input has a syntax of a valid polynomial according to the
//specification
//Returns true if the input conforms
//Returns false otherwise
//===============================================================================

bool checkSyntax(string polynomial)
{
    bool correct = true, foundNum;
    int len = polynomial.length();

    for (int i = 0; i < len && correct; i++)
    {
        foundNum = false;

        while (polynomial[i] >= '0' && polynomial[i] <= '9')
        {
            foundNum = true;
            i++;
        }

        while (polynomial[i] == ' ')
        {
            i++;
        }

        if (foundNum && polynomial[i] >= '0' && polynomial[i] <= '9')
        {
            correct = false;
        }
    }

    removeSpaces(polynomial);
    len = polynomial.length();

    for (int i = 0; i < len; i++)
    {
        polynomial[i] = toUpper(polynomial[i]);
    }

    if (correct && (polynomial[0] == '^' || polynomial[len - 1] == '^' || polynomial[len - 1] == '+' || polynomial[len - 1] == '-') || polynomial[len - 1] == 'X')
    {
        correct = false;
    }

    if (correct && len == 1)
    {
        if (polynomial[0] == '+' || polynomial[0] == '-')
        {
            correct = false;
        }
    }

    for (int i = 0; i < len - 1 && correct; i++)
    {
        if (polynomial[i] == 'X' && polynomial[i + 1] != '^')
        {
            correct = false;
        }
        else if (polynomial[i] >= '0' && polynomial[i] <= '9' && polynomial[i + 1] == '^')
        {
            correct = false;
        }
        else if (polynomial[i] == '^' && (polynomial[i + 1] < '0' || polynomial[i + 1] > '9'))
        {
            correct = false;
        }
        else if (polynomial[i] == '+' && (polynomial[i + 1] == '^' || polynomial[i + 1] == '+'))
        {
            correct = false;
        }
        else if (polynomial[i] == '-' && (polynomial[i + 1] == '^' || polynomial[i + 1] == '+' || polynomial[i + 1] == '+'))
        {
            correct = false;
        }
    }

    return correct;
}

//===============================================================================
//Checks whether a polynomial's exponents  are in descending order
//Returns true  if the polynomial's exponents  are in descending order
//Returns false otherwise
//===============================================================================

bool checkExponentOrder(string polynomial)
{
    bool correct = true;
    int length = polynomial.length(), index = 0;
    int exponentLast = -1, exponentCurrent, numExponents = 0;

    while (index < length)
    {
        exponentCurrent = parseExponent(polynomial, index);
        numExponents++;

        if (exponentCurrent >= exponentLast && numExponents > 1)
        {
            correct  = false;
        }

        exponentLast = exponentCurrent;

        while (polynomial[index] == ' ' || polynomial[index] == '+' || polynomial[index] == '-')
        {
            index++;
        }

        while (index < length && polynomial[index] != '+' && polynomial[index] != '-')
        {
            index++;
        }
    }

    return correct;
}

//===============================================================================
//Removes all the space characters in a string
//===============================================================================

void removeSpaces(string& input)
{
    int length = input.length();

    for (int i = 0; i < length; i++)
    {
        if (input[i] == ' ')
        {
            input.erase(i, 1);
            length--;
            i--;
        }
    }
}

//===============================================================================
//Capitalizes a lower-case letter
//Returns an upper case form of an input if  the character is a lower case alphabet
//Returns the input itself otherwise
//===============================================================================

char toUpper(char letter)
{
    if(letter >= 'a' && letter <= 'z')
    {
        letter += 'A' - 'a';
        return letter;
    }
    else
        return letter;
}

//===============================================================================
//Parses an array of string polynomials into an array of linked lists
//===============================================================================

void parseStringPolynomials(string polynomials[], Polynomial heads[], int numPolynomials)
{
    for (int i = 0; i < numPolynomials; i++)
    {
        int length = polynomials[i].length();
        int index = 0;

        heads[i] = nullptr;

        while (index < length)
        {
            int coefficient, exponent;

            coefficient = parseCoefficient(polynomials[i], index);
            exponent = parseExponent(polynomials[i], index);
            appendTerm(heads[i], coefficient, exponent);

            while (polynomials[i][index] == ' ' || polynomials[i][index] == '+' || polynomials[i][index] == '-')
            {
                index++;
            }

            while (index < length && polynomials[i][index] != '+' && polynomials[i][index] != '-')
            {
                index++;
            }
        }
    }
}

//===============================================================================
//Parses the coefficient of a term of a string polynomial
//Returns the coefficient
//===============================================================================

int parseCoefficient(string polynomial, int index)
{
    string s_coefficient;

    while (polynomial[index] == ' ' || polynomial[index] == '+')
    {
        index++;
    }

    if (polynomial[index] == '-')
    {
        s_coefficient += polynomial[index];
        index++;
    }

    while (polynomial[index] == ' ')
    {
        index++;
    }

    if (polynomial[index] >= '0' && polynomial[index] <= '9')
    {
        while (polynomial[index] >= '0' && polynomial[index] <= '9')
        {
            s_coefficient += polynomial[index];
            index++;
        }

        return stoi(s_coefficient);
    }
    else
    {
        s_coefficient += '1';

        return stoi(s_coefficient);
    }
}

//===============================================================================
//Parses the exponent of a term of string polynomial
//Returns the exponent
//===============================================================================

int parseExponent(string polynomial, int index)
{
    string s_exponent;
    int length = polynomial.length();

    while (polynomial[index] == ' ' || polynomial[index] == '+' || polynomial[index] == '-')
    {
        index++;
    }

    while (index < length && polynomial[index] != '+' && polynomial[index] != '-')
    {
        if (polynomial[index] == '^')
        {
            index++;

            while (polynomial[index] >= '0' && polynomial[index] <= '9')
            {
                s_exponent += polynomial[index];
                index++;
            }

            return stoi(s_exponent);
        }

        index++;
    }

    return 0;
}

//===============================================================================
//Appends a term (node) to a polynomial linked list
//===============================================================================

void appendTerm(Polynomial& head, int coefficient, int exponent)
{
    Polynomial term, tempTerm;

    term = new ATerm;
    term->Coefficient = coefficient;
    term->Exponent = exponent;
    term->Next = nullptr;

    if (!head)
    {
        head = term;
    }
    else
    {
        tempTerm = head;

        while (tempTerm->Next)
        {
            tempTerm = tempTerm->Next;
        }

        tempTerm->Next = term;
    }
}

//===============================================================================
//Adds two polynomial linked lists
//Returns the sum (also a polynomial linked list)
//===============================================================================

Polynomial Add(Polynomial poly1, Polynomial poly2)
{
    Polynomial polySum = nullptr;
    Polynomial temp1 = poly1, temp2 = poly2;
    int tempExponent, tempCoefficient;

    while (temp1 && temp2)
    {
        if (temp1->Exponent > temp2->Exponent)
        {
            tempCoefficient = temp1->Coefficient;
            tempExponent = temp1->Exponent;
            appendTerm(polySum, tempCoefficient, tempExponent);
            temp1 = temp1->Next;
        }
        else if (temp1->Exponent < temp2->Exponent)
        {
            tempCoefficient = temp2->Coefficient;
            tempExponent = temp2->Exponent;
            appendTerm(polySum, tempCoefficient, tempExponent);
            temp2 = temp2->Next;
        }
        else
        {
            tempCoefficient = temp1->Coefficient + temp2->Coefficient;
            tempExponent = temp1->Exponent;
            appendTerm(polySum, tempCoefficient, tempExponent);
            temp1 = temp1->Next;
            temp2 = temp2->Next;
        }
    }

    while (temp1)
    {
        tempCoefficient = temp1->Coefficient;
        tempExponent = temp1->Exponent;
        appendTerm(polySum, tempCoefficient, tempExponent);
        temp1 = temp1->Next;
    }

    while (temp2)
    {
        tempCoefficient = temp2->Coefficient;
        tempExponent = temp2->Exponent;
        appendTerm(polySum, tempCoefficient, tempExponent);
        temp2 = temp2->Next;
    }

    return polySum;
}

//===============================================================================
//Multiplies two polynomial linked lists
//Returns the product (also a polynomial linked list)
//===============================================================================

Polynomial Multipy(Polynomial poly1, Polynomial poly2)
{
    Polynomial polyProduct = nullptr, termProduct = nullptr;
    Polynomial temp1 = poly1, temp2 = poly2;

    if (temp1 && temp2)
    {
        while (temp1)
        {
            termProduct = multiplyTerm(temp1, temp2);
            polyProduct = Add(termProduct, polyProduct);
            temp1 = temp1->Next;
        }
    }
    else if (temp1)
    {
        polyProduct = Copy(temp1);
    }
    else if (temp2)
    {
        polyProduct = Copy(temp2);
    }

    return polyProduct;
}

//===============================================================================
//Multiplies two terms (nodes) of two polynomial linked lists
//Returns the product (also a term of a polynomial linked list)
//===============================================================================

Polynomial multiplyTerm(Polynomial term, Polynomial poly)
{
    Polynomial tempTerm = term, tempPoly = poly;
    Polynomial product = nullptr;
    int tempCoefficient, tempExponent;


    while (tempPoly)
    {
        tempCoefficient = tempTerm->Coefficient * tempPoly->Coefficient;
        tempExponent = tempTerm->Exponent + tempPoly->Exponent;

        appendTerm(product, tempCoefficient, tempExponent);
        tempPoly = tempPoly->Next;
    }

    return product;
}

//===============================================================================
//Copies a polynomial linked list
//Returns the copy
//===============================================================================

Polynomial Copy(Polynomial poly)
{
    Polynomial tempPoly = poly;
    Polynomial copyOfPoly = nullptr;
    int tempExponent, tempCoefficient;

    while (tempPoly)
    {
        tempCoefficient = tempPoly->Coefficient;
        tempExponent = tempPoly->Exponent;
        appendTerm(copyOfPoly, tempCoefficient, tempExponent);
        tempPoly = tempPoly->Next;
    }

    return copyOfPoly;
}

//===============================================================================
//Displays the addition of polynomials as specified
//===============================================================================

void displayAddition(Polynomial headSum, Polynomial heads[],  int numPolynomials)
{
    cout << "\nSum = ";

    for (int i = 0; i < numPolynomials; i++)
    {
        if (i > 0)
        {
            cout << " + ";
        }

        cout << "( ";
        displayPolynomial(heads[i]);
        cout << ")";
    }

    cout << "\n    =  ";
    displayPolynomial(headSum);
    cout << endl;
}

//===============================================================================
//Displays the multiplication of polynomials as specified
//===============================================================================

void displayMultiplication(Polynomial headProduct, Polynomial heads[],  int numPolynomials)
{
    cout << "\nProduct = ";

    for (int i = 0; i < numPolynomials; i++)
    {
        if (i > 0)
        {
            cout << " * ";
        }

        cout << "( ";
        displayPolynomial(heads[i]);
        cout << ")";
    }

    cout << "\n        =  ";
    displayPolynomial(headProduct);
}

//===============================================================================
//Displays a polynomial as specified
//===============================================================================

void displayPolynomial(Polynomial tempHead)
{
    int index = 0;

    while (tempHead)
        {
            if (tempHead->Coefficient != 0)
            {
                if (index == 0 && ((tempHead->Coefficient != -1 && tempHead->Coefficient != 1) || tempHead->Exponent == 0))
                {
                    cout << tempHead->Coefficient;
                }
                else if (index == 0 && tempHead->Coefficient == -1)
                {
                    cout << "-";
                }
                else if (tempHead->Coefficient > 1 || tempHead->Coefficient == 1 && tempHead->Exponent == 0)
                {
                    cout << "+  " << tempHead->Coefficient;
                }
                else if (tempHead->Coefficient == 1 && tempHead->Exponent != 0 && index != 0)
                {
                    cout << "+  ";
                }
                else if (tempHead->Coefficient < -1 || tempHead->Coefficient == -1 && tempHead->Exponent == 0)
                {
                    cout << "-  " << -(tempHead->Coefficient);
                }
                else if (tempHead->Coefficient == -1 && tempHead->Exponent != 0)
                {
                    cout << "-  ";
                }

                if (tempHead->Exponent >= 1)
                {
                    cout << 'x'  << "^" << tempHead->Exponent;
                }

                cout << " ";
                index++;
            }

            tempHead = tempHead->Next;
        }

        if (index == 0)
        {
            cout << 0 << " ";
        }
}

