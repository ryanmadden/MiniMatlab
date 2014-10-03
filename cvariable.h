// cvariable.h
// MiniMatlab
// Ryan Madden, Northwestern University
// Copyright, 2014
//
// Interface for the cvariable class

#include <iostream>
#include <vector>
#include "CMatrix.h"
using namespace std;

class CVariable
{
    friend ostream& operator<<(ostream& out, CVariable& cvar);
private:
    char* name;
    CMatrix value; //value is now a CMatrix
public:
    CVariable(const char* init_name);
    CVariable(const char* init_name, double init_value);
    CVariable(const char* init_name, CMatrix init_value);
    CVariable(const CVariable& copy);
    const char* getName() const;
    virtual CMatrix& operator*();
    virtual CMatrix operator*() const;
    virtual CMatrix& operator=(double newValue);
    virtual CVariable& operator=(CVariable& newValue);
    virtual void print();
    virtual ~CVariable();
};

class CVarDB
{
    friend ostream& operator<<(ostream& out, CVarDB& cdb);
private:
    vector<CVariable> db;
public:
    CVarDB();
    bool add(CVariable& newVar);
    CVariable* search(const char* name);
    bool isThere(const char* name);
    virtual void print();
    ~CVarDB();
};
