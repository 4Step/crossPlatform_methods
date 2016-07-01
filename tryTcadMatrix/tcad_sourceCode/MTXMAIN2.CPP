#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "mtxerr.h"
#include "CaliperMTX.h"
#include "mtxclass.hpp"


/*
    - This example uses matrices from the TransCAD tutorial folder.
    - To run the compiled program copy CaliperMtx.dll to the output folder
 
    - Compile for debugging:
        cl /Gr /Od /Zi /EHsc GISDK\Matrices\mtxmain2.cpp GISDK\Matrices\mtxclass.cpp /link CaliperMTX.lib

    - The CPP example (mtxmain.cpp and mtxclass.cpp) include only a subset of the callable functions available.
      A complete list of functions can be found in CaliperMtx.h. Any function there need to be exposed in 
      MTXClass.hpp and MTXClass.cpp.

    - NOTE: to run this example edit the path to the input matrices
 */

#define NUM_MATRIX_FILES 3
static char *MATRIX_FILES[] = 
{ 
    "tutorial\\UTILITY.MTX",
    "tutorial\\BUSFARE.MTX",
    "tutorial\\STRANGE.MTX"
};


/*
 * Example of C++ class using the matrix DLL.
 * This program should produce the following output:
 *
 * C:\Users\Matrix_dll>MatrixMain.exe
 * UTILITY.MTX <matrix cores: 2 rows: 5 columns: 5>
 * UTILITY.MTX <matrix cores: 2 rows: 5 columns: 5>
 * BUSFARE.MTX <matrix cores: 3 rows: 5 columns: 5>
 * BUSFARE.MTX <matrix cores: 3 rows: 5 columns: 5>
 * Cannot load matrix from file: STRANGE.MTX (file not found or not a matrix)
 *
 */
int main(int argc, char *argv[])
{

    MatrixClass*    matrixInstance;     // Pointer to the CPP matrix class 
    char            matrix_file[128];   // Matrix file name variable
    char            matrix_content[128];
    short           status;
    int             i = 0, j = 0;
    float           *data_values = NULL, // vector to store matrix values
                    RowTotal;
    long            nRows, nCols;

    while ( i < NUM_MATRIX_FILES ) {
       
    strcpy(matrix_file,MATRIX_FILES[i]);
    matrixInstance = new MatrixClass(matrix_file);
    if (matrixInstance->GetStatus() != TC_OKAY) 
        {
        printf("Cannot load matrix from file: %s (file not found or not a matrix)\n", matrix_file); 
        }
    else 
        {
        matrixInstance->ToString(matrix_content);
        cout << matrix_file << " " << *matrixInstance << "\n";
        cout << matrix_file << " " << matrix_content << "\n";


        // Get dimensions of matrix file
        nRows = matrixInstance->GetBaseNRows();
        nCols = matrixInstance->GetBaseNCols();
        data_values = (float*)malloc(nCols * sizeof(float));

        matrixInstance->SetCore(0);         // set the currency to the first core in the matrix file
        for (i = 0; i < nRows; i++)
        {
            status = matrixInstance->GetBaseVector(i, MATRIX_ROW, FLOAT_TYPE, data_values);
            RowTotal = 0;
            for (j = 0; j < nCols; j++)
            {
                if (data_values[j] != flt_miss)
                {
                    RowTotal += data_values[j];
                }
            }
        cout << matrix_file << "Row Totals: Row " << i << ": " << RowTotal << "\n";


        }

    free(data_values);

/* Example of getting elements one by one
        float value;
        short status, r, c;
        for (r = 1; r < 6; r++)
        {
            for (c = 1; c < 6; c++)
            {
                status = matrixInstance->GetElement(r, c, FLOAT_TYPE, &value);
                cout << matrix_file << "[" << r << "," << c << "]=" << value << "\n";
            }
        }
*/  

    

        
        delete matrixInstance;
        }
    i++;
    }

    return 0;
}




