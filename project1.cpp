#include <iostream>
#include <fstream>

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
        SparseMatrix (int n, int m, int cv, int noNSV) {

            noRows = n; noCols = m; commonValue = cv; noNonSparseValues = noNSV; 
            // myMatrix = new SparseRow[noRows * noCols];
            if(noNSV > 0) {
                myMatrix = new SparseRow[noNonSparseValues];
            }
        }

        SparseMatrix* Transpose () { //Matrix Transpose
            SparseMatrix* temp = new SparseMatrix[noNonSparseValues];
            for(int i = 0; i < noNonSparseValues; i++) {
                if(myMatrix[i].getRow() == myMatrix[i].getCol()) {
                    temp[i].setNoRows(myMatrix[i].getRow());
                    temp[i].setNoCols(myMatrix[i].getCol());
                    temp[i].setCommonValue(myMatrix[i].getValue());
                } else {
                    temp[i].setNoRows(myMatrix[i].getCol());
                    temp[i].setNoCols(myMatrix[i].getRow());
                    temp[i].setCommonValue(myMatrix[i].getValue());
                }
            }
            return temp;

        }
        SparseMatrix* Multiply (SparseMatrix* M) {
            SparseMatrix* temp = new SparseMatrix[noNonSparseValues];

            if((*M).getNoCols() != (*M).getNoRows()) {
                cout << "Matrix multiplication is not possible" << endl;
            }

            int i, j, k;
            int* tempMatrix = new int[(*M).noRows * (*M).noCols];
            for (i = 0; i < noRows * (*M).noCols; i++) {
                tempMatrix[i] = commonValue;
            }
            for (i = 0; i < noNonSparseValues; i++) {
                for (j = 0; j < (*M).noCols; j++) {
                    tempMatrix[myMatrix[i].getRow() * (*M).noCols + j] += myMatrix[i].getValue() * (*M).myMatrix[j].getValue();
                }
            }
            for (i = 0; i < noRows; i++) {
                for (j = 0; j < (*M).noCols; j++) {
                    if (tempMatrix[i * (*M).noCols + j] != commonValue) {
                        temp -> myMatrix[temp -> noNonSparseValues].setRow(i);
                        temp -> myMatrix[temp -> noNonSparseValues].setCol(j);
                        temp -> myMatrix[temp -> noNonSparseValues].setValue(tempMatrix[i * (*M).noCols + j]);
                        temp -> noNonSparseValues++;
                    }
                }
            }
            return temp;

        }
        SparseMatrix* Addition (SparseMatrix& M) {
        
            if(noRows != M.getNoRows() || noCols != M.getNoCols()) { // Check if the number of rows and columns are the same
                cout << "Matrix addition is not possible" << endl;
                return nullptr;
            }

            SparseMatrix* result = new SparseMatrix[M.getNoRows(), M.getNoCols(), M.getCommonValue(), M.getNoNonSparseValues()];

            int** output = new int*[noRows];
            for (int i = 0; i < noRows; i++) {
                output[i] = new int[noCols];
            }

            for(int i = 0; i < noNonSparseValues; i++) {
                int row = myMatrix[i].getRow();
                int col = myMatrix[i].getCol();

                output[row][col] += myMatrix[i].getValue();
            }

            for(int j = 0; j < M.getNoNonSparseValues(); j++) {
                int row = M.myMatrix[j].getRow();
                int col = M.myMatrix[j].getCol();

                output[row][col] += M.myMatrix[j].getValue();
            }

            for(int i = 0; i < noRows; i++) {
                for(int j = 0; j < noCols; j++) {
                    if(output[i][j] != 0) {
                        result -> myMatrix[i].setRow(i);
                        result -> myMatrix[i].setCol(j);
                        result -> myMatrix[i].setValue(output[i][j]);
                    }
                }
            }

            for(int i = 0; i < noRows; i++) {
                for(int j = 0; j < noCols; j++) {
                    cout << output[i][j] << " ";
                }
            }

            return result;
        }

        friend ostream& operator<< (ostream& s, const SparseMatrix& sm) {
            s << sm.noRows << ", " << sm.noCols << ", " << sm.commonValue << + ", " << endl;
            return s;
        }

        void displayMatrix () { //Display the matrix in its original format

            for(int i = 0; i < noNonSparseValues; i++) {
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


        void readMatrix() {
            int counter = 0;
            for(int i = 0; i < noRows; i++) {
        
                for(int j = 0; j < noCols; j++) {
        
                    int v;
                    cin >> v;
                    if(v != commonValue && counter < noNonSparseValues) {
                        myMatrix[counter].setRow(i);
                        myMatrix[counter].setCol(j);
                        myMatrix[counter].setValue(v);
                        counter++;
                    }
                }
            }   
        }

        void matrixFormat() {
            int index = 0;
            for(int i = 0; i < noRows; i++) {
                for(int j = 0; j < noCols; j++) {
                    if(myMatrix[index].getRow() == i && myMatrix[index].getCol() == j) {
                        cout << myMatrix[index].getValue() << " ";
                        index++;
                    } else {
                        cout << commonValue << " ";
                    }
                }
                cout << endl;
            }
        }

};

int main () {
    int n, m, cv, noNSV;
    SparseMatrix* temp;
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);

    firstOne -> readMatrix();
    

    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
    //Write the Statements to read in the second matrix

    secondOne -> readMatrix();



    cout << "First one in sparse matrix format" << endl;
    (*firstOne).displayMatrix();

    cout << "After transpose" << endl;
    (*firstOne).Transpose();

    cout << "First one in matrix format" << endl;
    firstOne -> matrixFormat();

    cout << "Second one in sparse matrix format" << endl;
    (*secondOne).displayMatrix();

    cout << "After transpose" << endl;
    (*secondOne).Transpose();

    cout << "Second one in matrix format" << endl;
    secondOne -> matrixFormat();

    cout << "Matrix Addition Result" << endl;
    temp = (*firstOne).Addition(*secondOne);

    temp -> matrixFormat();

    // if(temp != nullptr) {
    //     cout << *temp;
    //     (*temp).displayMatrix();
    // }
    
    cout << "Matrix Multiplication Result" << endl;
    if(temp != nullptr) {
        delete[] temp;
    }
    temp = ((*firstOne).Multiply(secondOne));
    (*temp).displayMatrix();

    delete[] temp;
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
*/