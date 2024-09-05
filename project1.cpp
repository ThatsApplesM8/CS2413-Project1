#include <iostream>
#include <fstream>
using namespace std;

class SparseRow {
    protected:
        int row;    // Row#
        int col;    // Column#
        int value;  // We will assume that all our values will be integers

    public:
        SparseRow ()  { // default constructor; row=-1;col=-1;value-0
            row = -1;
            col = -1;
            value = 0;
        }   
        void display() {  // print Row#, Col#, value
            cout << row << " " << col << " " << value << endl;
        }

        friend ostream& operator<< (ostream& s, const SparseRow& sr) {
            s << sr.row << " " << sr.col << " " << sr.value << endl;
            return s;
        }

        // other methods that are necessary such as get and set
        int getRow() {
            return row;
        }

        int getCol() {
            return col;
        }

        int getValue() {
            return value;
        }

        void setRow(int r) {
            row = r;
        }

        void setCol(int c) {
            col = c;
        }

        void setValue(int v) {
            value = v;
        }



};



class SparseMatrix {
    protected:
        int noRows; // Number of rows of the original matrix
        int noCols; // Number of columns of the original matrix
        int commonValue; // Read from input
        int noNonSparseValues;
        SparseRow* myMatrix;    // Array

    public: 
        SparseMatrix() {// Default constructor
            noRows = 0;
            noCols = 0;
            commonValue = 0;
            noNonSparseValues = 0;
            myMatrix = NULL;
        }
        SparseMatrix (int n, int m, int cv) {
            noRows = n;
            noCols = m;
            commonValue = cv;
            myMatrix = new SparseRow[noRows * noCols];
        }
        SparseMatrix* Transpose () {// Matrix Transpose
            SparseMatrix* temp = new SparseMatrix(noCols, noRows, commonValue);
            for (int i = 0; i < noNonSparseValues; i++) {
                temp -> myMatrix[i].setRow(myMatrix[i].getCol());
                temp -> myMatrix[i].setCol(myMatrix[i].getRow());
                temp -> myMatrix[i].setValue(myMatrix[i].getValue());
            }
            temp -> setNoNonSparseValues(noNonSparseValues);
            return temp;
        }
        SparseMatrix* Multiply (SparseMatrix& M) {
            SparseMatrix* temp = new SparseMatrix(noRows, M.noCols, commonValue);
            int i, j, k;
            int* tempMatrix = new int[noRows * M.noCols];
            for (i = 0; i < noRows * M.noCols; i++) {
                tempMatrix[i] = commonValue;
            }
            for (i = 0; i < noNonSparseValues; i++) {
                for (j = 0; j < M.noCols; j++) {
                    tempMatrix[myMatrix[i].getRow() * M.noCols + j] += myMatrix[i].getValue() * M.myMatrix[j].getValue();
                }
            }
            for (i = 0; i < noRows; i++) {
                for (j = 0; j < M.noCols; j++) {
                    if (tempMatrix[i * M.noCols + j] != commonValue) {
                        temp -> myMatrix[temp -> noNonSparseValues].setRow(i);
                        temp -> myMatrix[temp -> noNonSparseValues].setCol(j);
                        temp -> myMatrix[temp -> noNonSparseValues].setValue(tempMatrix[i * M.noCols + j]);
                        temp -> noNonSparseValues++;
                    }
                }
            }
            return temp;
        }
        SparseMatrix* Add (SparseMatrix& M) {
            SparseMatrix* temp = new SparseMatrix(noRows, noCols, commonValue);
            int i, j;
            int* tempMatrix = new int[noRows * noCols];
            for (i = 0; i < noRows * noCols; i++) {
                tempMatrix[i] = commonValue;
            }
            for (i = 0; i < noNonSparseValues; i++) {
                tempMatrix[myMatrix[i].getRow() * noCols + myMatrix[i].getCol()] = myMatrix[i].getValue();
            }
            for (i = 0; i < M.noNonSparseValues; i++) {
                tempMatrix[M.myMatrix[i].getRow() * noCols + M.myMatrix[i].getCol()] += M.myMatrix[i].getValue();
            }
            for (i = 0; i < noRows; i++) {
                for (j = 0; j < noCols; j++) {
                    if (tempMatrix[i * noCols + j] != commonValue) {
                        temp -> myMatrix[temp -> noNonSparseValues].setRow(i);
                        temp -> myMatrix[temp -> noNonSparseValues].setCol(j);
                        temp -> myMatrix[temp -> noNonSparseValues].setValue(tempMatrix[i * noCols + j]);
                        temp -> noNonSparseValues++;
                    }
                }
            }
            return temp;
        }
        
        friend ostream& operator<< (ostream& s, const SparseMatrix& sm) {
            s << sm.noRows << " " << sm.noCols << " " << sm.commonValue << " " << sm.noNonSparseValues << endl;
            for (int i = 0; i < sm.noNonSparseValues; i++) {
                s << sm.myMatrix[i];
            }
            return s;
        }
        void displayMatrix() {        // Display the matrix in its original format
            int i, j, k;
            int* tempMatrix = new int[noRows * noCols];
            for (i = 0; i < noRows * noCols; i++) {
                tempMatrix[i] = commonValue;
            }
            for (i = 0; i < noNonSparseValues; i++) {
                tempMatrix[myMatrix[i].getRow() * noCols + myMatrix[i].getCol()] = myMatrix[i].getValue();
            }
            for (i = 0; i < noRows; i++) {
                for (j = 0; j < noCols; j++) {
                    cout << tempMatrix[i * noCols + j] << " ";
                }
                cout << endl;
            }

        }
        ~SparseMatrix() {
            delete[] myMatrix;
        }


        // Other methods that are necessary such as get and set
        int getRows() {
            return noRows;
        }

        int getCols() {
            return noCols;
        }

        int getCommonValue() {
            return commonValue;
        }

        void setRows(int r) {
            noRows = r;
        }

        void setCols(int c) {
            noCols = c;
        }

        void setCommonValue(int cv) {
            commonValue = cv;
        }
        void setNoNonSparseValues(int nsv) {
            noNonSparseValues = nsv;
        }
};



int main() {
    int n, m, cv, noNSV;
    SparseMatrix* temp;

    cin >> n >> m >> cv >> noNSV;

    SparseMatrix* firstOne = new SparseMatrix(n, m, cv);


    cout << n << " " << m << " " << cv << " " << noNSV << endl;


    // Write the statements to read in the first matrix
    temp = new SparseMatrix(n, m, cv);
    for(int i = 0; i < noNSV; i++) {
        int r, c, v;
        cin >> r >> c >> v;
        temp[i].setRows(r);
        temp[i].setCols(c);
        temp[i].setCommonValue(v);
    }

    for(int i = 0; i < noNSV; i++) {
        firstOne -> setRows(temp[i].getRows());
        firstOne -> setCols(temp[i].getCols());
        firstOne -> setCommonValue(temp[i].getCommonValue());

    }

    firstOne -> setNoNonSparseValues(noNSV);

    delete[] temp;


    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* secondOne = new SparseMatrix(n, m, cv);

    // Write the statements to read in the second matrix
    temp = new SparseMatrix(n, m, cv);

    for(int i = 0; i < noNSV; i++) {
        int r, c, v;
        cin >> r >> c >> v;
        temp[i].setRows(r);
        temp[i].setCols(c);
        temp[i].setCommonValue(v);
    }

    for(int i = 0; i < noNSV; i++) {
        secondOne -> setRows(temp[i].getRows());
        secondOne -> setCols(temp[i].getCols());
        secondOne -> setCommonValue(temp[i].getCommonValue());

    }

    secondOne -> setNoNonSparseValues(noNSV);

    delete[] temp;

    cout << "First one in matrix format" << endl;
    (*firstOne).displayMatrix();

    cout << "First one in sparse matrix format" << endl;
    cout << (*firstOne);

    cout << "Second one in matrix format" << endl;
    (*secondOne).displayMatrix();

    cout << "Second one in sparse matrix format" << endl;
    cout << (*secondOne);

    cout << "Transpose of the first one in matrix" << endl;
    cout << (*(*firstOne).Transpose());

    cout << "Matrix Addition Result" << endl;
    
    temp = ((*firstOne).Add(*secondOne));
    cout << temp;
    (*temp).displayMatrix();

    cout << "Matrix Multiplication Result" << endl;

    temp = (*firstOne).Multiply(*secondOne);
    cout << temp;
    (*temp).displayMatrix();


    delete firstOne;
    delete secondOne;

    return 0;

}
