#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_QUANTITY = 11, MIN_QUANTITY = 2, MAX_LENGTH = 14;
const int MAX_SIZE = 40;//for strings in general

void printInstructions();
void createString(char[]);
void getNumbers(char[][MAX_SIZE + 1], int&);
bool validNumber(string, int&);
bool validNumber(string);
void copyString(string, char[]);
void insertAt(char[], char, int);
int findLength(string);
void addNumbers(char[][MAX_SIZE + 1], int, char[]);
char addDigits(char[], int&);
int a_to_i(char);
char i_to_a(int);
void multiplyNumbers(char[], char[], char[][MAX_SIZE + 1], int&, char[]);
void appendZeros(char[][MAX_SIZE + 1], int&);
void multiplyDigits(char[], char, char [][MAX_SIZE + 1], int);
void insertCommas(char[]);
void displayAddition(char [][MAX_SIZE + 1], int, char[]);
void displayMultiplication(char[], char[], char[]);
void printSpace(int);
void printString(char[]);


int main()
{
    char numbers[MAX_QUANTITY][MAX_SIZE + 1];
    int numbersSize;
    char sum[MAX_SIZE + 1];
    char products[MAX_LENGTH][MAX_SIZE + 1];
    int productsSize;
    char finalProduct[MAX_SIZE + 1];

    printInstructions();

    for (int i = 0; i < MAX_QUANTITY; i++)
    {
        createString(numbers[i]);
    }

    getNumbers(numbers, numbersSize);

    createString(sum);
    createString(finalProduct);
    productsSize = findLength(numbers[1]);

    for (int i = 0; i < productsSize; i++)
    {
        createString(products[i]);
    }

    addNumbers(numbers, numbersSize, sum);
    multiplyNumbers(numbers[0], numbers[1], products, productsSize, finalProduct);

    for (int i = 0; i < MAX_QUANTITY; i++)
    {
        insertCommas(numbers[i]);
    }

    insertCommas(sum);
    insertCommas(finalProduct);

    for (int i = 0; i < productsSize; i++)
    {
        insertCommas(products[i]);
    }

    cout << "\nThe sum of:\n\n";
    displayAddition(numbers, numbersSize, sum);

    cout << "\n\nThe product of:\n\n";
    displayMultiplication(numbers[0], numbers[1], finalProduct);
    displayAddition(products, productsSize, finalProduct);


    return 0;
}

//===========================================================================
//Displays instructions
//===========================================================================

void printInstructions()
{
    cout << "ADDITION AND MULTIPLICATION OF BIG NUMBERS\n"
         << "This program adds 2 and up to 11 very large numbers.\n"
         << "It multiplies the first two numbers showing the intermediate steps.\n\n";
}

//===========================================================================
//Initializes the first element of a C-string to a null character
//===========================================================================

void createString(char s[])
{
    s[0] = '\0';
}

//===========================================================================
//Lets the user input the quantity of numbers and the numbers
//===========================================================================

void getNumbers(char numbers[][MAX_SIZE + 1], int& size)
{
    string temp;

    cout << "\nHow many numbers? -> ";
    getline(cin, temp);

    while(!validNumber(temp, size) || size < MIN_QUANTITY || size > MAX_QUANTITY)
    {
        cout << "Error (only 2 and up to " << MAX_QUANTITY << ")! How many numbers? -> ";
        getline(cin, temp);
    }


    for (int i = 0; i < size; i++)
    {
        cout << "\nInput number #" << i + 1 << " -> ";
        getline(cin, temp);

        while(!validNumber(temp))
        {
            cout << "Error! Input number (only up to " << MAX_LENGTH << " digits) #" << i + 1 << " -> ";
            getline(cin, temp);
        }

        copyString(temp, numbers[i]);
    }
}

//===========================================================================
//Validates an input for the quantity of numbers
//Returns true if an input is a number between 0 and 99
//Returns false otherwise
//===========================================================================

bool validNumber(string input, int& num)
{
    int i = 0;

    for(; input[i] != 0; i++)
    {
        if(input[i] < '0' || input[i] > '9')
        {
            return false;
        }
    }

    num = 0;

    if(i == 1)
    {
        num = input[0] - '0';
        return true;
    }

    else if(i == 2)
    {
        num = (input[0] - '0') * 10 + input[1] - '0';
        return true;
    }

    else
        return false;
}

//===========================================================================
//Validates an input for the numbers
//Returns true if an input is a number
//Returns false otherwise
//===========================================================================

bool validNumber(string input)
{
    int i = 0;

    for(; input[i] != 0; i++)
    {
        if(input[i] < '0' || input[i] > '9')
        {
            return false;
        }
    }

    if(i == 0 || i > MAX_LENGTH)
    {
        return false;
    }

    return true;
}

//===========================================================================
//Copies a string
//===========================================================================

void copyString(string s, char c_s[])
{
    int length = findLength(s);

    for (int i = length - 1; i >= 0; i--)
    {
        insertAt(c_s, s[i], 0);
    }
}

//===========================================================================
//Inserts a character at a particular location
//===========================================================================

void insertAt(char s[], char c, int index)
{
    int size = findLength(s);

    for (int i = size; i >= index; i--)
    {
        s[i + 1] = s[i];
    }
    s[index] = c;
}

//===========================================================================
//Finds and returns the length of a string
//===========================================================================

int findLength(string input)
{
    int length = 0;

    while (input[length] != 0)
    {
        length++;
    }

    return length;
}

//===========================================================================
//Adds a list of numbers stored as a string and stores the sum in a single string
//===========================================================================

void addNumbers(char numbers[][MAX_SIZE + 1], int size, char sum[])
{
    int lengths[size];
    int longest;
    int carry = 0;
    char digits[MAX_LENGTH + 1];
    char temp;

    for (int i = 0; i < size; i++)
    {
        lengths[i] = findLength(numbers[i]);
    }

    longest = lengths[0];

    for (int i = 1; i < size; i++)
    {
        if (lengths[i] > longest)
        {
            longest = lengths[i];
        }
    }

    for (int j = 0; j < longest; j++)
    {
        digits[0] = '\0';

        for (int i = 0; i < size; i++)
        {
            if (lengths[i] > 0)
            {
                temp = numbers[i][lengths[i] - 1];
                insertAt(digits, temp, 0);
                lengths[i]--;
            }
        }

        temp = addDigits(digits, carry);
        insertAt(sum, temp, 0);
    }

    if (carry > 0 && carry < 10)
    {
        temp = i_to_a(carry);
        insertAt(sum, temp, 0);
    }
    else if (carry >= 10)
    {
        carry -= 10;
        temp = i_to_a(carry);
        insertAt(sum, temp, 0);
        insertAt(sum, '1', 0);
    }
}

//===========================================================================
//Adds the numbers in a string and returns the one’s place of the sum
//===========================================================================

char addDigits(char digits[], int& carry)
{
    int sum = 0, temp;
    int tensPlace = 0;

    for (int i = 0; digits[i] != '\0'; i++)
    {
        temp = a_to_i(digits[i]);
        sum += temp;
    }

    sum += carry;

    if (sum < 10)
    {
        carry = 0;
        return (i_to_a(sum));
    }
    else
    {
        while (sum >= 10)
        {
            sum -= 10;
            tensPlace++;
        }

        carry = tensPlace;
        return (i_to_a(sum));
    }
}

//===========================================================================
//Converts a character to an integer data type
//===========================================================================

int a_to_i(char charNum)
{
    if (charNum >= '0' && charNum <= '9')
    {
        return (charNum - '0');
    }
    else
    {
        return charNum;
    }
}

//===========================================================================
//Converts an integer to a character data type
//===========================================================================

char i_to_a(int num)
{
    if (num >= 0 && num <= 9)
    {
        return (num + '0');
    }
    else
    {
        return num;
    }
}

//===========================================================================
//Multiplies two numbers stored as a string and stores the product in a single string
//===========================================================================

void multiplyNumbers(char num1[], char num2[], char products[][MAX_SIZE + 1], int& productsSize, char finalProduct[])
{
    appendZeros(products, productsSize);

    for (int i = productsSize - 1, j = 0; i >= 0; i--, j++)
    {
        multiplyDigits(num1, num2[i], products, j);
    }

    addNumbers(products, productsSize, finalProduct);
}

//===========================================================================
//Appends zeros to strings
//===========================================================================

void appendZeros(char products[][MAX_SIZE + 1], int& productsSize)
{
    for (int i = productsSize - 1, j = 0; i >= 0; i--, j++)
    {
        int trailingZeros = j;

        while (trailingZeros > 0)
        {
            insertAt(products[j], '0', 0);
            trailingZeros--;
        }
    }
}

//===========================================================================
//Multiplies two numbers (one stored as a string and another one stored as a character) and puts the result in another string
//===========================================================================

void multiplyDigits(char number[], char digit, char products[][MAX_SIZE + 1], int index)
{
    int digitNum = a_to_i(digit);
    char copiesOfNumber[MAX_LENGTH][MAX_SIZE + 1];

    for (int i = 0; i < digitNum; i++)
    {
        createString(copiesOfNumber[i]);
        copyString(number, copiesOfNumber[i]);
    }

    if (digitNum == 0)
    {
        int numberLength = findLength(number);

        createString(copiesOfNumber[0]);

        for (int i = 0; i < numberLength; i++)
        {
            insertAt(copiesOfNumber[0], '0', 0);
        }
        digitNum++;
    }

    addNumbers(copiesOfNumber, digitNum, products[index]);
}

//===========================================================================
//Inserts commas in a string (of number)
//===========================================================================

void insertCommas(char number[])
{
    int size = findLength(number);

    for (int i = 1; i < size; i++)
    {
        if ((size - i) % 3 == 0)
        {
            insertAt(number, ',', i);
            size++;
            i++;
        }
    }
}

//===========================================================================
//Displays the addition of a list of numbers
//===========================================================================

void displayAddition(char numbers[][MAX_SIZE + 1], int size, char sum[])
{
    int longest = findLength(sum);
    int length;

    for (int i = 0; i < (size - 1); i++)
    {
        length = findLength(numbers[i]);
        printSpace(3 + longest - length);
        printString(numbers[i]);
        cout << endl;
    }

    cout << "+> ";
    length = findLength(numbers[size - 1]);
    printSpace(longest - length);
    printString(numbers[size - 1]);
    cout << endl;

    for (int i = 0; i < longest + 3; i++)
    {
        cout << "-";
    }
    cout << endl;

    printSpace(3);
    printString(sum);
}

//===========================================================================
//Displays the first part of the multiplication of a list of numbers
//===========================================================================

void displayMultiplication(char num1[], char num2[], char finalProduct[])
{
    int longest = findLength(finalProduct);
    int length;

    length = findLength(num1);
    printSpace(3 + longest - length);
    printString(num1);
    cout << endl;

    cout << "*> ";
    length = findLength(num2);
    printSpace(longest - length);
    printString(num2);
    cout << endl;

    for (int i = 0; i < longest + 3; i++)
    {
        cout << "-";
    }
    cout << endl;
}

//===========================================================================
//Prints a space of a specified length
//===========================================================================

void printSpace(int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << ' ';
    }
}

//===========================================================================
//Prints a string
//===========================================================================

void printString(char s[])
{
    for (int i = 0; s[i] != 0; i++)
    {
        cout << s[i];
    }
}




