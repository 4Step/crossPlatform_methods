#include <stdio.h>
#include <iostream>

// compile with: /EHsc
using namespace std;

#include "mtxerr.h"
#include "CaliperMtx.h"
#include "mtxclass.hpp"

// Matrix dll status variables:

int _matrix_dll_status = TC_OKAY;
int _matrix_dll_loaded = 0;

// Constructors:

MatrixClass::MatrixClass(char *file_name)
{
    if (_matrix_dll_loaded==0) 
    {
        InitMatDLL(&(_matrix_dll_status));
        _matrix_dll_loaded=1; 
    }
    hMatrix = MATRIX_LoadFromFile(file_name,CONTROL_AUTOMATIC);
}

// Returns any one of the values defined in Status.h

int MatrixClass::GetStatus()
{
    return _matrix_dll_status;
}


// Destructor:

MatrixClass::~MatrixClass()
{
    if ( hMatrix != NULL ) 
    {
        MATRIX_Done(hMatrix);
    }
}


// Operator overloaded using a member function

ostream& operator<<(ostream& s,MatrixClass& m)
{
    s << "<matrix cores: " << m.GetNCores() << " rows: " << m.GetNRows() << " columns: " << m.GetNCols() << ">";
    return s;
}

short MatrixClass::GetElement(long idRow, long idCol, DATA_TYPE Type, void *p)
{
    if ( hMatrix != NULL ) 
    {
        return MATRIX_GetElement(hMatrix, idRow, idCol, Type, p);
    }
    else
        return TC_INVINPUT;
}


short MatrixClass::GetVector(long idRow, MATRIX_DIM dim, DATA_TYPE Type, void *p)
{
    if ( hMatrix != NULL ) 
    {
        return MATRIX_GetVector(hMatrix, idRow, dim, Type, p);
    }
    else
        return TC_INVINPUT;
}

short MatrixClass::GetBaseVector(long index, MATRIX_DIM dim, DATA_TYPE Type, void *p)
{
    if ( hMatrix != NULL ) 
    {
        return MATRIX_GetBaseVector(hMatrix, index, dim, Type, p);
    }
    else
        return TC_INVINPUT;
}
