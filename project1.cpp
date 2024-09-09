#include <iostream>
using namespace std;

class SparseRow {
    protected:
        int row; //Row#
        int col; //Column#
        int value; //We will assume that all our values will be integers
    public:
        SparseRow (){row = -1; col = -1; value = 0;} //default constructor; row=-1;col=-1;value=0
        SparseRow(int r, int c, int v) {row = r; col = c; value = v;} //constructor with three arguments
        void display() {cout << row << ", " << col << ", " << value << endl;} // print Row#, Column#, value

        friend ostream& operator<< (ostream& s, const SparseRow sp) { //overload the << operator
            s << sp.row << ", " << sp.col << ", " << sp.value << endl;  // output the row, column, and value
            return s;
        } 
        
        //other methods that are necessary such as get and set 
        int getRow() {return row;}  // Gets the row number
        int getCol() {return col;} // Gets the column number
        int getValue() {return value;} // Gets the value
        void setRow(int r) {row = r;} // Sets the row number
        void setCol(int c) {col = c;} // Sets the column number
        void setValue(int v) {value = v;} // Sets the value

};
class SparseMatrix {
    protected:
        int noRows; //Number of rows of the original matrix
        int noCols; //Number of columns of the original matrix
        int commonValue; //read from input
        int noNonSparseValues;  //Number of non-sparse values
        SparseRow* myMatrix; //Array
    public:
        SparseMatrix () {   // Default constructor
            int noRows = 0; int noCols = 0; int commonValue = 0; int noNonSparseValues = 0; 
            myMatrix = NULL;
        }
        SparseMatrix (int n, int m, int cv, int noNSV) {    // Constructor with four arguments

            noRows = n; noCols = m; commonValue = cv; noNonSparseValues = noNSV; // Set the number of rows, columns, common value, and number of non-sparse values
            if(noNSV > 0) { // If the number of non-sparse values is greater than 0
                myMatrix = new SparseRow[noNonSparseValues];
            }
        }

        SparseMatrix* Transpose () { //Matrix Transpose
            SparseMatrix* transposedMatrix = new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues); // Create a new SparseMatrix object

            for(int i = 0; i < noNonSparseValues; i++) {  // Iterate through the number of non-sparse values
                int row = myMatrix[i].getRow();
                int col = myMatrix[i].getCol();
                int value = myMatrix[i].getValue();


                if(row != col) { // If the row is not equal to the column
                    transposedMatrix -> myMatrix[i] = SparseRow(col, row, value);
                
                } else { // If the row is equal to the column
                    transposedMatrix -> myMatrix[i] = SparseRow(row, col, value);
                }
            }
            return transposedMatrix; // Return the temporary matrix
        }
SparseMatrix* Multiply(SparseMatrix* M) { // Matrix Multiplication
    if (noCols != M->noRows) { // Check if the number of columns of the second matrix is equal to the number of rows of the first matrix
        cout << "Matrix multiplication is not possible" << endl;
        return nullptr;
    }

    SparseMatrix* output = new SparseMatrix(noRows, M->noCols, commonValue, 0); // Create a new SparseMatrix object

    int** tempMatrix = new int*[noRows]; // Create a 2D array with the number of rows
    for (int i = 0; i < noRows; ++i) { // Iterate through the number of rows
        tempMatrix[i] = new int[M->noCols];
        for (int j = 0; j < M->noCols; j++) { // Iterate through the number of columns
            tempMatrix[i][j] = 0; // Set the element to 0 instead of commonValue
        }
    }

    for (int i = 0; i < noRows; i++) { // Iterate through the number of non-sparse values
        for (int j = 0; j < M->noCols; j++) { // Iterate through the number of columns
            int amount = 0; // Initialize amount for each column
            for (int k = 0; k < noCols; k++) { // Iterate through the number of columns
                int ptA = commonValue;
                int ptB = commonValue;

                for (int z = 0; z < noNonSparseValues; z++) { // Iterate through the number of non-sparse values
                    if (myMatrix[z].getRow() == i && myMatrix[z].getCol() == k) { // If the row and column are equal to i and k
                        ptA = myMatrix[z].getValue();
                        break; // Break once the value is found
                    }
                }

                for (int z = 0; z < M->noNonSparseValues; z++) { // Iterate through the number of non-sparse values
                    if (M->myMatrix[z].getRow() == k && M->myMatrix[z].getCol() == j) { // If the row and column are equal to k and j
                        ptB = M->myMatrix[z].getValue();
                        break; // Break once the value is found
                    }
                }

                amount += ptA * ptB; // Add the product of ptA and ptB to the amount
            }

            if (amount != commonValue) {
                tempMatrix[i][j] = amount;
            }
        }
    }

    int total = 0;
    for (int i = 0; i < noRows; i++) { // Iterate through the number of rows
        for (int j = 0; j < M->noCols; ++j) { // Iterate through the number of columns
            if (tempMatrix[i][j] != commonValue) { // If the temporary matrix is not equal to the common value
                total++;
            }
        }
    }

    output->myMatrix = new SparseRow[total]; // Set the number of non-sparse values to the checker
    output->setNoNonSparseValues(total); // Set the number of non-sparse values in the output SparseMatrix object
    int index = 0;
    for (int i = 0; i < noRows; i++) { // Iterate through the number of rows
        for (int j = 0; j < M->noCols; j++) { // Iterate through the number of columns
            if (tempMatrix[i][j] != commonValue) { // If the temporary matrix is not equal to the common value
                output->myMatrix[index] = SparseRow(i, j, tempMatrix[i][j]);
                index++;
            }
        }
    }

    for (int i = 0; i < noRows; ++i) {
        delete[] tempMatrix[i];
    }

    delete[] tempMatrix;
    return output; // Return the result
}
    
    SparseMatrix* Addition(SparseMatrix& M) { // Matrix Addition
    if (noRows != M.getNoRows() || noCols != M.getNoCols()) { // Check if the number of rows and columns are the same
        cout << "Matrix addition is not possible" << endl;
        return nullptr;
    }

    SparseMatrix* result = new SparseMatrix(noRows, noCols, commonValue, 0); // Create a new SparseMatrix object

    int** output = new int*[noRows]; // Create a 2D array with the number of rows
    for (int i = 0; i < noRows; ++i) { // Iterate through the number of rows
        output[i] = new int[noCols];
        for (int j = 0; j < noCols; ++j) { // Iterate through the number of columns
            output[i][j] = commonValue;
        }
    }

    for (int i = 0; i < noNonSparseValues; ++i) { // Iterate through the number of non-sparse values
        int row = myMatrix[i].getRow();
        int col = myMatrix[i].getCol();
        output[row][col] = myMatrix[i].getValue();
    }

    for (int j = 0; j < M.getNoNonSparseValues(); ++j) { // Iterate through the number of non-sparse values
        int row = M.myMatrix[j].getRow();
        int col = M.myMatrix[j].getCol();
        output[row][col] += M.myMatrix[j].getValue();
    }

    int total = 0;
    for (int i = 0; i < noRows; i++) { // Iterate through the number of rows
        for (int j = 0; j < noCols; j++) {
            if (output[i][j] != commonValue) {
                total++;
            }
        }
    }

    result->myMatrix = new SparseRow[total]; // Set the number of non-sparse values to the checker
    result->setNoNonSparseValues(total); // Create a new SparseRow object with the checker

    int index = 0;
    for (int i = 0; i < noRows; i++) { // Iterate through the number of rows
        for (int j = 0; j < noCols; j++) {
            if (output[i][j] != commonValue) {
                result->myMatrix[index] = SparseRow(i, j, output[i][j]);
                index++;
            }
        }
    }

    for (int i = 0; i < noRows; ++i) {
        delete[] output[i];
    }
    delete[] output;

    return result; // Return the result
}

        friend ostream& operator<< (ostream& s, const SparseMatrix& sm) { //overload the << operator
            s << sm.noRows << ", " << sm.noCols << ", " << sm.commonValue << + ", " << endl;
            return s;
        }

        void displayMatrix () { //Display the matrix in its original format

            for(int i = 0; i < noNonSparseValues; i++) { // Iterate through the number of non-sparse values
                cout << myMatrix[i];
            }

        }

        //other methods that are necessary such as get and set
        int getNoRows() {return noRows;}
        int getNoCols() {return noCols;}
        int getCommonValue() {return commonValue;}
        int getNoNonSparseValues() {return noNonSparseValues;}
        void setNoRows(int n) {noRows = n;}
        void setNoCols(int m) {noCols = m;}
        void setCommonValue(int cv) {commonValue = cv;}
        void setNoNonSparseValues(int nsv) {noNonSparseValues = nsv;}


        void readMatrix() {     //Read the matrix from the user input and store it in the matrix format
            int counter = 0; // Initialize the counter to 0
            for(int i = 0; i < noRows; i++) { // Iterate through the number of rows
        
                for(int j = 0; j < noCols; j++) { // Iterate through the number of columns
        
                    int v;
                    cin >> v; // Read the value from the user input
                    if(v != commonValue && counter < noNonSparseValues) { // If the value is not equal to the common value and the counter is less than the number of non-sparse values
                        myMatrix[counter].setRow(i);
                        myMatrix[counter].setCol(j);
                        myMatrix[counter].setValue(v);
                        counter++;
                    }
                }
            }   
        }

        void matrixFormat() { //Display the matrix in the matrix format
            int index = 0; // Initialize the index to 0
            for(int i = 0; i < noRows; i++) { // Iterate through the number of rows
                for(int j = 0; j < noCols; j++) {   // Iterate through the number of columns
                    if(myMatrix[index].getRow() == i && myMatrix[index].getCol() == j) { // If the row and column are equal to i and j
                        cout << myMatrix[index].getValue() << " ";
                        index++;
                    } else { // If the row and column are not equal to i and j
                        cout << commonValue << " ";
                    }
                }
                cout << endl;
            }
        }

};

int main () { //Main function
    int n, m, cv, noNSV;
    SparseMatrix* temp; // Create a temporary SparseMatrix object
    cin >> n >> m >> cv >> noNSV; // Read the number of rows, columns, common value, and number of non-sparse values from the user input
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV); // Create a new SparseMatrix object

    // Write the Statements to read in the first matrix
    firstOne -> readMatrix(); // Read the matrix
     

    cin >> n >> m >> cv >> noNSV; // Read the number of rows, columns, common value, and number of non-sparse values from the user input
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV); // Create a new SparseMatrix object
    //Write the Statements to read in the second matrix

    secondOne -> readMatrix(); // Read the matrix



    cout << "First one in sparse matrix format" << endl;
    (*firstOne).displayMatrix();

    cout << "After transpose" << endl;
    temp = (*firstOne).Transpose(); // Transpose the first matrix
    temp -> displayMatrix(); // Display the transposed first matrix


    cout << "First one in matrix format" << endl;
    firstOne -> matrixFormat();

    delete[] temp;

    cout << "Second one in sparse matrix format" << endl;
    (*secondOne).displayMatrix();

    cout << "After transpose" << endl;
    temp = (*secondOne).Transpose(); // Transpose the second matrix
    temp -> displayMatrix(); // Display the transposed second matrix

    cout << "Second one in matrix format" << endl;
    secondOne -> matrixFormat();

    cout << "Matrix addition Result" << endl;
    SparseMatrix* addedMatrix = firstOne -> Addition(*secondOne); // Add the first matrix with the second matrix
    if(addedMatrix != nullptr) {
        addedMatrix -> matrixFormat();
        delete addedMatrix;
    }


    cout << "Matrix multiplication Result" << endl;
    SparseMatrix* multipleMatrix = firstOne -> Multiply(secondOne);
    
    if(multipleMatrix != nullptr) {
        multipleMatrix -> matrixFormat();
        delete multipleMatrix;
    }

    return 0;
}

/* LLM and GitHub Copilot Usage Documentation 

   Prompt: "Generate a method for reading the matrix from the user input and storing it in the matrix format."
   Rationale: I chose this prompt for Copilot to generate a method that can read the inputs of the input file and store them in the matrix format.
    I was having trouble with the displayMatrix() method because I was not able to display the matrix in the correct format. I thought that if I could read the matrix in the correct format,
    then I could display it in the correct format as well. I thought that this prompt would help me with that.
    The prompt generated the readMatrix() method which reads the matrix from the user input and stores it in the matrix format. This method helped me to read the matrix in the correct format and fully implement
    its method with another method that I had already implemented. This prompt helped me to understand how to read the matrix in the correct format and store it in the matrix format.
   Inremental Development: The prompt enabled myself to fully increment the development of the readMatrix() method, which I use as a helper method for the firstOne and secondOne object, which are SparseMatrix objects.
    By having this method, I was able to apply the readMatrix() method and point to the displayMatrix() method to display the matrix in the correct format.

   Prompt: "Generate a summary of the implementation of the operator<< method."
   Rationale: This prompt was personally for gaining a better understanding of how the operator override methods works. I was having difficulties grasping the concept of how the operator<< method works and 
    how it is implemented. I thought that if I could get a summary of the implementation of the operator<< method, then I could understand how it works and how it is implemented. Using this knowledge, I could better grasp 
    the operator<< method and how it is implemented in the SparseRow and SparseMatrix classes. This includes how the method was applied to output the row, column, and value of the SparseRow object and the SparseMatrix object.
   Incremental Development: The prompt enabled me to fully understand how the operator<< method works and how it is implemented. I started settled with the oeprator<< method asking to simply output the rows, column, and value of the other class objects.
    Now, I use this operator<< method to help output the row, column, and value of the SparseRow object and the SparseMatrix object towards other methods in their body statements.

   Prompt: "Generate diagonal stored value for the Transpose() method and swap the rows and columns."
   Rationale: The reason why I gave this prompt to Copilot was not because of any particular issue I was facing, but rather to see how AutoCopilot would implement the swaps and the diagonal values. The Transpose() Method is very udnerstandable, 
   and straight forward, but I wanted to see how Copilot would implement the swaps and the diagonal values. I thought that this prompt would help me better grasp the use of the SparseMatrix and SparseRow class handling.
   The prompt also provided a swap method for the changes in the row and column, but I utimately decided to use the setRow() and setCol() methods to change the row and column values rather than using the swap method.
    Incremental Development: The prompt enabled me to fully understand how the Transpose() method works and how it is implemented. I started with the Transpose() method asking to simply swap the rows and columns of the SparseMatrix object.
    Now, I use this Transpose() method to help swap the rows and columns of the SparseMatrix object and the SparseRow object towards other methods in their body statements. 

   Prompt: "Generate a body statement for Multiply() method to enable multiplication between the first matricies row and the second matricies column."
   Rationale: I've asked Copilot to generate such body statement due to the lack of understanding matrix multiplication in terms of how the rows and columns are oprated.
   I thought that this part would help guide me into creating the setup for the matrix multiplication, especially since I had a sense with what it would like to output.
   By having Multiply(), I can be able to multiply 2 matricies together and output the result in the correct format using the readMatrix() and matrixFormat() methods.
   Incremental Development: The prompt enabled me to fully understand how the Multiply() method works and how it is implemented. I started with the Multiply() method asking to simply multiply the first matrix with the second matrix.
   Using this knowledge by Copilot, and other sources that can explain deeper into the structural body of the method, I can now implement Multiply to gain the output between 2 matricies.

*/