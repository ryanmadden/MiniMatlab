//  cvariable.cpp
//  MiniMatlab
//
//  Created by Ryan Madden on 2/23/14.

#include "cvariable.h"
#include "string"

using namespace std;

/** CVariable - Default constructor
 * Initialize name to init_name and value to 0
 * @param init_name The name to be assigned
 */
CVariable::CVariable(const char* init_name)
{
    name = new char[20];
    strncpy(name, init_name, 20);
    value = *new CMatrix();
}

/** CVariable - Constructor with name and value
 * Initialize name to init_name and value to init_value
 * @param init_name The string to be assigned to name
 * @param init_value The value to be assigned to value
 */
CVariable::CVariable(const char* init_name, double init_value)
{
    name = new char[20];
    strncpy(name, init_name, 20);
    value = *new CMatrix(init_value);
}

/** CVariable - Constructor with name and CMatrix
 * Initialize name to init_name and value to the passed CMatrix
 * @param init_name The string to be assigned to name
 * @param init_value The CMatrix to be assigned to value
 */
CVariable::CVariable(const char* init_name, CMatrix init_value)
{
    name = new char[20];
    strncpy(name, init_name, 20);
    value = init_value;
}

/** CVariable - Copy constructor
 * Initializes name and value to be the same as the passed CVariable
 * @param copy The CVariable to be copied
 */
CVariable::CVariable(const CVariable& copy)
{
    name = new char[20];
    strncpy(name, copy.getName(), 20);
    value = *copy;
}

/** CVariable - getName()
 * Returns the name of this CVariable
 * @return The name of this CVariable
 */
const char* CVariable::getName() const
{
    return name;
}

/** CVariable - operator*()
 * Overloads the * operator to return the value when used with a CVariable and no parameters
 * @return A reference to the value of this CVariable
 */
CMatrix& CVariable::operator*()
{
    return value;
}

/** CVariable - operator*()
 * Overloads the * operator to return the value when used with a CVariable and no parameters
 * @return The value of this CVariable
 */
CMatrix CVariable::operator*() const
{
    return value;
}

/** CVariable - operator=(double newValue)
 * Overloads the = operator to update the value of this CVariable to the passed value
 * @param The string used to update value
 * @return A reference to this CVariable
 */
CMatrix& CVariable::operator=(double newValue)
{
    CMatrix temp = *new CMatrix(newValue);
    value = temp;
    return value;
}

/** CVariable - operator=(CVariable& newValue)
 * Overloads the = operator to update the value of this CVariable to the value of the passed reference CVariable
 * @param A reference to the CVariable who's value is used to update the value of this CVariable
 * @return A reference to this CVariable
 */
CVariable& CVariable::operator=(CVariable& newValue)
{
    if (this == &newValue)
    {
        return *this;
    }
    else
    {
        value = *newValue;
        return *this;
    }
}

/** CVariable - print()
 * Prints the name and value of this CVariable in the format specified by the sample output
 * @return The name of this CVariable
 */
void CVariable::print()
{
    cout << this->getName() << ":" << endl;
    printMatrix(this->value);
}

/** CVariable - ~CVariable()
 * Destructor function
 */
CVariable::~CVariable()
{
    delete name;
}

/** CVarDB - Default constructor
 * Initializes the database with just the "ans" variable
 */
CVarDB::CVarDB()
{
    db.push_back(*new CVariable("ans", 0)); // ans has a value of 0
}

/** CVarDB - add()
 * Adds a new CVariable to the database
 * @return True if the addition was successful, false otherwise
 */
bool CVarDB::add(CVariable& newVar)
{
    for (int i = 0; i < db.size(); i++)
    {
        if (strcmp(db[i].getName(), newVar.getName()) == 0) //If the names are the same
        {
            return false;
        }
    }
    db.push_back(newVar);
    return true;
}

/** CVarDB - search()
 * Searches the database to see if the passed name exists
 * If it exists, return a reference to that CVariable
 * If it does not exist, return a null pointer
 * @param name The name to be searched for
 * @return A reference to the CVariable that was searched for, or null if that name does not exist in the database
 */
CVariable* CVarDB::search(const char* name)
{
    for (int i = 0; i < db.size(); i++)
    {
        if (strcmp(db[i].getName(), name) == 0) //If the names are the same
        {
            return &db[i]; //Return the reference
        }
    }
    return nullptr; //Return null pointer
}

/** CVarDB - isThere()
 * Checks to see if a CVariable with the passed name is in the database
 * @param name The name to be searched for
 * @return True if the name is in the database, false otherwise
 */
bool CVarDB::isThere(const char *name)
{
    for (int i = 0; i < db.size(); i++)
    {
        if (strcmp(db[i].getName(), name) == 0) //If the names match
        {
            return true; //The variable is in database
        }
    }
    return false; //The variable is not in database
}

/** CVarDB - print()
* Prints out the database by calling each CVariable's print function
*/
void CVarDB::print()
{
    cout << endl;
    for (int i = 0; i < db.size(); i++) //For each CVariable in the database
    {
        db[i].print(); //Print the CVariable
    }
}

//Destructor function
CVarDB::~CVarDB()
{
    //destrucotr
}