// main.cpp
// MiniMatlab
// Ryan Madden, Northwestern University
// Copyright, 2014

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "cvariable.h"

using namespace std;

enum OP {ASN, ADD, MIN, MUL, DIV, PRE_INC, PRE_DEC,POST_INC, POST_DEC, ADD_EQ, MIN_EQ, MUL_EQ, DIV_EQ}; // enumeration of operator types

//function declaration
void Interpreter(string&);
bool IsAUnary(char*, int [][2], char*, OP&); // if the command line is valid unary operation
bool IsAAsn(char*, int [][2], char*, char*); // if it is a valid assignment
bool IsABinary3(char*, int [][2], char*, char*, OP&); // if it is a valid binary operation
bool IsABinary5(char*, int [][2], char*, char*, char*, OP&);// if it is a valid assignment and binary operation


bool Partitioner(char*, int [][2], int&); // partitioning the command line into segments
bool IsAChar(char); // if input is a letter
bool IsADigit(char); // if it is a digit
bool IsAOperator(char); // if it is an operator

bool assign(const char* lhs, const char* rhs);
bool unary_op(const char* variable, OP unary_op);
bool binary_op(const char* left, OP op, const char* right);
bool binary_assign(const char* assign_var, const char* left, OP op, const char* right);
bool IsACompoundOp(char* Buffer, int segmt[][2], char* operand_1, char* operand_2, OP& op);
bool compound_op(char* operand_1, OP op, char* operand_2);


//The global database
CVarDB database;

int main()
{
    // print welcome message
    cout << endl;
    cout << "\t\t\tWelcome to MiniMatlab" << endl;
    cout << "\t\tRyan Madden, Northwestern University "<< endl;
    cout << "\tUse this as you would the MATLAB console" << endl;
    cout << "\tType 'who' to see assigned variables; type 'quit' to quit" << endl;
    
    //Declare global database
    database = *new CVarDB();
    
    //Get the input
    string Buffer;
    while (getline(cin, Buffer))
    {
        if (Buffer.compare("quit")==0) // if the input is 'quit'
        {
            cout<<"\nThank you. "; //quit the program
            break;
        }
        else if(Buffer.compare("who") == 0) //print the variables
        {
            cout << "Variables:";
            database.print();
        }
        else
        {
            // process the command line
            Interpreter(Buffer);
        }
    }
    
    
    
    cout<<"Now closing..."<<endl;
    
    return 0;
}



/**
 * assign - This function handles the assignment case
 * The function checks for the existence of the variables used, creating them and adding them to the databse
 * if they do not already exist. If the variables do exist, they are updated
 * @param lhs The string name of the LHS of the assignment
 * @param rhs The string name of the RHS of the assignment
 * @return True if the assignment was successful, false if not
 */
bool assign(const char* lhs, const char* rhs)
{
    CMatrix temp_num; //Used to store the assignment value to avoid problems in assigning a variable to itself
    if (database.isThere(lhs))
    {
        if (database.isThere(rhs))
        {
            temp_num = **database.search(rhs); //Assign value to temp_num in case LHS and RHS are same variable
            **database.search(lhs) = temp_num; //Complete the assignment
            cout << lhs << " =" << endl;
            printMatrix(temp_num);
            return true;
        }
        else
        {
            if (IsADigit(rhs[0]))
            {
                temp_num = *new CMatrix(atof(rhs)); //Convert string of numbers to a double
                **database.search(lhs) = temp_num; //Complete the assignment
                cout << lhs << " =" << endl;
                printMatrix(temp_num);
                return true;
            }
            else if (rhs[0] == '[')
            {
                temp_num = *new CMatrix(rhs);
                **database.search(lhs) = temp_num;
                cout << lhs << " =" << endl;
                printMatrix(temp_num);
                return true;
            }
            else
            {
                return false; //If it's not a variable or a number, the assignment fails
            }
        }
    }
    else //If the LHS doesn't already exist
    {
        if (database.isThere(rhs))
        {
            CVariable newvar = *new CVariable(lhs, *rhs); //Create a new variable with the name of LHS and the value of RHS
            cout << lhs << " =" << endl;
            printMatrix(*newvar);
            return database.add(newvar); //Add to the database
        }
        else
        {
            if (IsADigit(rhs[0]))
            {
                CVariable newvar = *new CVariable(lhs, *new CMatrix(atof(rhs)));
                cout << lhs << " =" << endl;
                printMatrix(*newvar);
                return database.add(newvar); //Add to the database
            }
            else if (rhs[0] == '[') //If it's a matrix
            {
                CVariable newvar = *new CVariable(lhs, *new CMatrix(rhs));
                cout << lhs << " =" << endl;
                printMatrix(*newvar); //Print the matrix
                return database.add(newvar);
            }
            else //If it's not a variable, matrix, or number then the assignment fails
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * assign - This function handles the unary operation case
 * The function checks for the existence of the variable used, creating it and it to the databse
 * if it do not already exist. If the variable does exist, it are updated based on the operator
 * Note: The code was split to handle individual cases in anticipation of further modification
 * @param variable The string name of the variable used
 * @param unary_op The unary operator used
 * @return True if the assignment was successful, false if not
 */
bool unary_op(const char* variable, OP unary_op)
{
    CMatrix curr_value; //The current value
    CMatrix add_value; //Matrix of all ones
    curr_value = **database.search(variable);
    int row = curr_value.getNRow();
    int col = curr_value.getNCol();
    add_value = *new CMatrix(row, col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            add_value.element(i, j) = 1;
        }
    }
    switch(unary_op){
        case PRE_INC: //++x
            **database.search(variable) = curr_value + add_value;
            cout << variable << " =" << endl;
            printMatrix(**database.search(variable));
            return true;
        case PRE_DEC: //--x
            **database.search(variable) = curr_value - add_value;
            cout << variable << " =" << endl;
            printMatrix(**database.search(variable));
            return true;;
        case POST_INC: //x++
            **database.search(variable) = curr_value + add_value;
            cout << variable << " =" << endl;
            printMatrix(**database.search(variable));
            return true;
        case POST_DEC: //x--
            **database.search(variable) = curr_value - add_value;
            cout << variable << " =" << endl;
            printMatrix(**database.search(variable));
            return true;
        default: //Not a unary operator
            cout << "UNRECOGNIZED OPERATOR" ;
            return false;
    }
}

/**
 * compound_op - This function handles the compound operators: +=, -=, *=, and /=
 * This function gets the values of the two operands and performs the appropriate math function
 * based on teh type of compound_op. The function then assigns the new value to the variable on
 * the LHS of the operation.
 * @param operand_1 The variable being assigned to, on the LHS of the operation
 * @param compound_op The compound operator symbol
 * @param operand_2 The variable or digit being used for the operation, on the RHS of the operation
 * @return True if the assignment was successful, false if not
 */
bool compound_op(char* operand_1, OP compound_op, char* operand_2)
{
    CMatrix curr_value; //The current value of operand_1 (LHS)
    CMatrix ass_value; //The current value of operand_2 (RHS)
    if (database.isThere(operand_1)) //If LHS exists in database
    {
        curr_value = **database.search(operand_1); //Get its value
    }
    else
    {
        return false;
    }
    if (database.isThere(operand_2)) //If RHS exists in database
    {
        ass_value = **database.search(operand_2); //Get its value
    }
    else if(IsADigit(operand_2[0])) //If RHS is a digit
    {
        ass_value = *new CMatrix(atof(operand_2)); //Convert to double
    }
    else if(operand_2[0] == '[') //If it's a matrix
    {
        ass_value = *new CMatrix(operand_2); //Make a new matrix for the assignment value
    }
    else //Operation fails
    {
        return false;
    }
    switch(compound_op){ //Check which compound operator is being used
        case ADD_EQ: //x += y
            **database.search(operand_1) = curr_value + ass_value;
            cout << operand_1 << " =" << endl;
            printMatrix(**database.search(operand_1));
            return true;
        case MIN_EQ: //x -= y
            **database.search(operand_1) = curr_value - ass_value;
            cout << operand_1 << " =" << endl;
            printMatrix(**database.search(operand_1));
            return true;;
        case MUL_EQ: //x *= y
            **database.search(operand_1) = curr_value * ass_value;
            cout << operand_1 << " =" << endl;
            printMatrix(**database.search(operand_1));
            return true;
        case DIV_EQ: //x /= y
            **database.search(operand_1) = curr_value / ass_value;
            cout << operand_1 << " =" << endl;
            printMatrix(**database.search(operand_1));
            return true;
        default: //Not a unary operator
            cout << "UNRECOGNIZED OPERATOR" ;
            return false;
    }
}

/**
 * binary_op - This function handles the binary operation case by calling binary_assign with "ans" as the
 * assign_var parameter. Binary operation is equal to a binary assignment assigning to the ans variable
 * Precondition: The parameter left is either the name of a variable that already exists in database
 * or is a double stored in a char*
 * @param left The string name of the LHS of the operation
 * @param right The string name of the RHS of the operation
 * @return True if the assignment was successful, false if not
 */
bool binary_op(const char* left, OP op, const char* right)
{
    return binary_assign("ans", left, op, right);
}

/**
 * binary_assign - This function handles the binary assignment case
 * The function checks for the existence of the variables used, creating them and adding them to the databse
 * if they do not already exist. If the variables do exist, their values are used to calculate an assignment value
 * which is then used to update the variable on the LHS of the assignment
 * @param assign_var The string name of the variable being updated by the assignment
 * @param left The string name of the LHS of the operation
 * @param right The string name of the RHS of the operation
 * @return True if the assignment was successful, false if not
 */
bool binary_assign(const char* assign_var, const char* left, OP op, const char* right)
{
    CMatrix value1; //Value of LHS
    CMatrix value2; //Value of RHS
    CMatrix assign_val; //Value to be assigned to assign_var
    if (database.isThere(left)) //If LHS exists in database
    {
        value1 = **database.search(left); //Get its value
    }
    else if (IsADigit(left[0])) //If LHS is a digit
    {
        value1 = *new CMatrix(atof(left)); //Convert to double and store in value1
    }
    else if (left[0] == '[') //If it's a matrix
    {
        value1 = *new CMatrix(left); //Make a new cmatrix with that value
    }
    else //If it's not a variable that exists, a matrix, or a digit
    {
        return false; //Operation fails
    }
    if (database.isThere(right)) //If RHS exists in datbase
    {
        value2 = **database.search(right); //Get its value
    }
    else if (IsADigit(right[0])) //If RHS is a digit
    {
        value2 = *new CMatrix(atof(right)); //Convert to double and store in value1
    }
    else if (right[0] == '[') //If it's a matrix
    {
        value2 = *new CMatrix(right); //Make a new CMatrix with the value
    }
    else //If it's not a variable that exists, a matrix, or a digit
    {
        return false; //Operation fails
    }
    switch(op){ //Perform the appropriate mathematical operation
        case ADD: //+
            assign_val = value1 + value2;
            break;
        case MIN: //-
            assign_val = value1 - value2;
            break;
        case MUL: //*
            assign_val = value1 * value2;
            break;
        case DIV: //(/)
            assign_val = value1/value2;
            break;
        default: //otherwise
            cout << "BAD OPERATOR" ; //Operation fails
            return false;
            break;
    }
    if (!database.isThere(assign_var)) //If assign var exists, update it
    {
        CVariable temp = *new CVariable(assign_var, assign_val);
        database.add(temp);
        cout << assign_var << " =" << endl;
        printMatrix(*temp);
        return true;
    }
    else //Otherwise create assign_var with the calculated value and add it to the database
    {
        **database.search(assign_var) = assign_val;
        cout << assign_var << " =" << endl;
        printMatrix(assign_val);
        return true;
    }
    
}

////////////////////////////////////////////////////////////////////////////////
// functions related to the command line interpreter

// We only allow:
// type asn:   e.g., a = b              (iSeg = 3)
// type unary: e.g., a++                (iSeg = 2)
// type binary:e.g., a + b or c = a+b   (iSeg = 3 or 5)


void Interpreter(string& Input)
{
    char Buffer[500]; // array buffer to hold the command line
    strncpy(Buffer, Input.c_str(), sizeof(Buffer));
    Buffer[sizeof(Buffer) - 1] = 0;
    
    int segmt[10][2]; // record the location of each segment
    
    int iSeg = 0; // number of segments in command line
    
    bool Tag = false;
    
    OP  op; // the operator in the command line
    
    char res[50], operand_1[50], operand_2[50]; // result_name, operand_1 and operand_2
    
    if(Partitioner(Buffer,segmt,iSeg))// if the command line can be successfully segment
    {
        
        switch (iSeg) // valid cases can be : 2, 3 or 5
        {
            case 2: // the command line is possible to be unary operation
            {
                Tag = IsAUnary(Buffer,segmt,operand_1,op); // if it is a valid unary operation
                if(Tag)
                {   // it is a valid unary operation, output the result
                    unary_op(operand_1, op); //Call unary_op to handle variable manipulation
                }
                break;
            }
            case 3: // the command line is possible to be binary operation or assignment
            {
                if(IsAAsn(Buffer,segmt,res,operand_1)) // if it is a valid assignment
                {   // it is a valid assignment, output the result
                    Tag = true;
                    assign(res, operand_1); //Call assign to handle variable manipulation
                }
                else if(IsABinary3(Buffer,segmt,operand_1,operand_2,op)) // if it is a binary operation with assignment to default variable ans
                {   // it is a binary operation, output the result
                    Tag = true;
                    binary_op(operand_1, op, operand_2); //Call bianry_op to handle variable manipulation
                }
                else if(IsACompoundOp(Buffer,segmt, operand_1, operand_2, op))
                {
                    Tag = true;
                    compound_op(operand_1, op, operand_2);
                }
                break;
            }
            case 5: // the command line is possible to be binary operation and assignment
            {
                Tag = IsABinary5(Buffer,segmt,res,operand_1,operand_2,op); // if it is a binary operation with assignment to a specified variable
                if(Tag)
                {   // it is a binary operation, output the result
                    binary_assign(res, operand_1, op, operand_2); //Call binary_assign to handle variable manipulation
                }
                break;
            }
        }
    }
    
    
    
    if (Tag == false) // if the command line is not valid
    {
        cout<<"Sorry, do not understand." <<endl;
    }
    
}

// If the input command line is a valid unary operation
// Input:
// char* Buffer   : the input command line (known)
// int segmt[][2] : the location of each segment in the command line (known)
// char* operand  : the operand in the command line (to fill in)
// OP& op         : the operator (to fill in)
// Output:
// boolean variable : 0 -- not a valid unary operation, 1 -- valid unary operation
bool IsAUnary(char* Buffer, int segmt[][2], char* operand, OP& op)
{
    bool Tag = false;
    char piece[2][50];
    for (int i = 0 ; i<2; i++)
    {
        for(int j = 0; j<segmt[i][1]; j++)
        {
            piece[i][j] = Buffer[j+segmt[i][0]];
        }
        piece[i][segmt[i][1]] = '\0';
    }
    if(strcmp(piece[0],"++")==0)
    {
        op = PRE_INC;
        strcpy(operand, piece[1]);
        Tag = true;
    }
    else if (strcmp(piece[0],"--")==0)
    {
        op = PRE_DEC;
        strcpy(operand, piece[1]);
        Tag = true;
    }
    else if(strcmp(piece[1],"++")==0)
    {
        op = POST_INC;
        strcpy(operand, piece[0]);
        Tag = true;
    }
    else if (strcmp(piece[1],"--")==0)
    {
        op = POST_DEC;
        strcpy(operand, piece[0]);
        Tag = true;
    }
    
    return Tag;
}

// If the input command line is a valid assignment
// Input:
// char* Buffer   : the input command line (known)
// int segmt[][2] : the location of each segment in the command line (known)
// char* res      : the reseult_name in the command line (to fill in)
// char* operand  : the operand in the command line (to fill in)
// Output:
// boolean variable : 0 -- not a valid assignment, 1 -- valid assignment

bool IsAAsn(char* Buffer, int segmt[][2], char* res, char* operand)
{
    bool Tag = false;
    char piece[3][50];
    for (int i = 0 ; i<3; i++)
    {
        for(int j = 0; j<segmt[i][1]; j++)
        {
            piece[i][j] = Buffer[j+segmt[i][0]];
        }
        piece[i][segmt[i][1]] = '\0';
    }
    if(strcmp(piece[1],"=")==0 && IsAChar(Buffer[segmt[0][0]])) // if the #2 piece is "="
    {
        strcpy(res, piece[0]);
        strcpy(operand, piece[2]);
        Tag = true;
    }
    return Tag;
}

// If the input command line is a binary operation
// Input:
// char* Buffer   : the input command line (known)
// int segmt[][2] : the location of each segment in the command line (known)
// char* operand_1: the operand_1 in the command line (to fill in)
// char* operand_2: the operand_2 in the command line (to fill in)
// OP& op         : the operator (to fill in)
// Output:
// boolean variable : 0 -- not a valid binary operation, 1 -- valid binary operation

bool IsABinary3(char* Buffer, int segmt[][2], char* operand_1, char* operand_2, OP& op)
{
    bool Tag = false;
    char piece[3][50];
    for (int i = 0 ; i<3; i++)
    {
        for(int j = 0; j<segmt[i][1]; j++)
        {
            piece[i][j] = Buffer[j+segmt[i][0]];
        }
        piece[i][segmt[i][1]] = '\0';
    }
    if(strcmp(piece[1],"+")==0) // if the #2 piece is an operator
    {
        op = ADD;
        Tag = true;
    }
    else if(strcmp(piece[1],"-")==0)
    {
        op = MIN;
        Tag = true;
    }
    else if(strcmp(piece[1],"*")==0)
    {
        op = MUL;
        Tag = true;
    }
    else if(strcmp(piece[1],"/")==0)
    {
        op = DIV;
        Tag = true;
    }
    if (Tag)
    {
        strcpy(operand_1, piece[0]);
        strcpy(operand_2, piece[2]);
    }
    return Tag;
}

bool IsACompoundOp(char* Buffer, int segmt[][2], char* operand_1, char* operand_2, OP& op)
{
    bool Tag = false;
    char piece[3][50];
    for (int i = 0 ; i<3; i++)
    {
        for(int j = 0; j<segmt[i][1]; j++)
        {
            piece[i][j] = Buffer[j+segmt[i][0]];
        }
        piece[i][segmt[i][1]] = '\0';
    }
    if(strcmp(piece[1],"+=")==0) // if the #2 piece is an operator
    {
        op = ADD_EQ;
        Tag = true;
    }
    else if(strcmp(piece[1],"-=")==0)
    {
        op = MIN_EQ;
        Tag = true;
    }
    else if(strcmp(piece[1],"*=")==0)
    {
        op = MUL_EQ;
        Tag = true;
    }
    else if(strcmp(piece[1],"/=")==0)
    {
        op = DIV_EQ;
        Tag = true;
    }
    if (Tag)
    {
        strcpy(operand_1, piece[0]);
        strcpy(operand_2, piece[2]);
    }
    return Tag;
}
// If the input command line is a binary operation and assignment
// Input:
// char* Buffer   : the input command line (known)
// int segmt[][2] : the location of each segment in the command line (known)
// char* res      : the reseult_name in the command line (to fill in)
// char* operand_1: the operand_1 in the command line (to fill in)
// char* operand_2: the operand_2 in the command line (to fill in)
// OP& op         : the operator (to fill in)
// Output:
// boolean variable : 0 -- not a valid binary operation and assignment, 1 -- valid binary operation and assignment

bool IsABinary5(char* Buffer, int segmt[][2], char* res , char* operand_1, char* operand_2, OP& op)
{
    bool Tag = false;
    char piece[5][50];
    for (int i = 0 ; i<5; i++)
    {
        for(int j = 0; j<segmt[i][1]; j++)
        {
            piece[i][j] = Buffer[j+segmt[i][0]];
        }
        piece[i][segmt[i][1]] = '\0';
    }
    //If the second piece is = and the first is a character
    if(strcmp(piece[1],"=")==0 && IsAChar(Buffer[segmt[0][0]]))
    {
        if(strcmp(piece[3],"+")==0) // if the #4 piece is an operator
        {
            op = ADD;
            Tag = true;
        }
        else if(strcmp(piece[3],"-")==0)
        {
            op = MIN;
            Tag = true;
        }
        else if(strcmp(piece[3],"*")==0)
        {
            op = MUL;
            Tag = true;
        }
        else if(strcmp(piece[3],"/")==0)
        {
            op = DIV;
            Tag = true;
        }
        if (Tag)
        {
            strcpy(res, piece[0]);
            strcpy(operand_1, piece[2]);
            strcpy(operand_2, piece[4]);
        }
    }
    
    return Tag;
}

////////////////////////////////////////////////////////////////////////////////
// functions from MP#2

bool Partitioner(char* Buffer,  int segmt[][2], int& iSeg)
{
    // indicator
    int st=0;
    int ed=0;
    bool error_code = true;
    
    int len = (int)strlen(Buffer);
    
    
    while(st<len)
    {
        while(Buffer[st]==' '&&st<len)st++;
        
        ed = st;
        
        if(st==len)
        {
            if (iSeg==0)
            {
                error_code = false;
            }
            break;
        }
        else
        {
            if(IsAChar(Buffer[st]))
            {
                while((ed<len)&&(IsAChar(Buffer[ed])||IsADigit(Buffer[ed])||Buffer[ed]=='_'))ed++;
            }
            else if(IsADigit(Buffer[st]))
            {
                while((ed<len)&&(IsADigit(Buffer[ed])||Buffer[ed]=='.'))ed++;
            }
            else if(IsAOperator(Buffer[st]))
            {
                while((ed<len)&&(IsAOperator(Buffer[ed])))ed++;
            }
            else if(Buffer[st]=='[')
            {
                ed = st+1;
                while((ed<len)&&(Buffer[ed]!=']'))
                {
                    if(IsADigit(Buffer[ed])||(Buffer[ed]==',')||(Buffer[ed]==';')||(Buffer[ed]==' ')||(Buffer[ed]=='.'))
                    {
                        ed++;
                    }
                    else
                    {
                        cout<<"Expect a ']' here."<<endl;
                        error_code = false;
                        break;
                    }
                }
                ed++;
            }
            else if(Buffer[st]==';')ed++;
            else if(Buffer[st] == '\r')
            {
                break;
            }
            else
            {
                error_code = false;
            }
            
        }
        if(error_code)
        {
            segmt[iSeg][0] = st;
            segmt[iSeg][1] = ed-st;
            iSeg = iSeg + 1;
            st = ed;
        }
        else
        {
            break;
        }
    }
    
    return error_code;
}

bool IsAChar(char str)
{
    bool Tag = false;
    if((str>='A'&&str<='Z')||(str>='a'&&str<='z')) Tag = true;
    return Tag;
    
}

bool IsADigit(char str)
{
    bool Tag = false;
    if((str>='0'&&str<='9')||(str=='.')) Tag = true;
    return Tag;
    
}

bool IsAOperator(char str)
{
    bool Tag = false;
    if((str=='+')||(str=='-')||(str=='*')||(str=='/')||(str=='=')) Tag = true;
    return Tag;
    
}

