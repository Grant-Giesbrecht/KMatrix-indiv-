//
//  KMatrix.hpp
//  KMatrix
//
//  Created by Grant Giesbrecht on 1/26/18.
//  Copyright © 2018 IEGA. All rights reserved.
//

#ifndef KMatrix_hpp
#define KMatrix_hpp

#include <stdio.h>
#include <string>
#include <GiesbrechtCPP/string_manip.hpp>
#include <iostream>
#include <exception>

template <class T>
class KMatrix {
public:

    //Initializers
    KMatrix();
    KMatrix(int rows, int cols);
    KMatrix(std::string init);
    KMatrix(T** init, int rows, int cols);
    KMatrix(T init, int rows, int cols);
    KMatrix(std::vector<std::vector<T> > init);
    KMatrix(const KMatrix<T>& init);
    ~KMatrix();

    //Open/close functions
    void clear();
    void clear(int rows, int cols);

    friend void swap(KMatrix<T>& first, KMatrix<T>& second);
    
    //Operators
    KMatrix<T>& operator+=(const KMatrix<T>& rv);
    KMatrix<T>& operator*=(const KMatrix<T>& rv);
    KMatrix<T>& operator-=(const KMatrix<T>& rv);
    KMatrix<T>& operator/=(const KMatrix<T>& rv);
    KMatrix<T>& operator+=(const T& rv);
    KMatrix<T>& operator*=(const T& rv);
    KMatrix<T>& operator-=(const T& rv);
    KMatrix<T>& operator/=(const T& rv);
    KMatrix<T>& operator=(KMatrix rh);
    T& operator()(int r, int c);
    T get(int r, int c) const;
    bool operator=(std::string rv);
//    bool operator=(Eigen::MatrixXd rv);
    bool operator=(std::vector<std::vector<double> > rv);

    //

    size_t rows() const;
    size_t cols() const;
//    void setSize(int rows, int cols);

    std::string to_string(std::string options="");

    T max();
    T min();
    T range();
    T avg();
    T stdev();

    //Arithmetic Functions
//    KMatrix crossprd(KMatrix& rv);
//    double dotprd(KMatrix& rv);
    KMatrix transpose();
    KMatrix conjugate();
    KMatrix adjoint(); //Conjugate transpose
    KMatrix inverse();
    bool is_invertable();
    KMatrix pseudoinverse(); //Moore-penrose //DO THIS
    double determinant();

    //Static Functions
    static KMatrix zero(int r, int c);
    static KMatrix zero(int rc);
    static KMatrix constant(T val, int r, int c);
    static KMatrix makeRange(T start, T step_size, T end, int rows=1);
//    static std::vector<std::vector<double> > KMatrix_to_vector(KMatrix km);

    //Other
    void setElementMultMode(bool em);
    std::vector<std::vector<T> >& getMat();
    bool& getElementMultMode();
    
private:

    std::vector<std::vector<T> > mat;
    bool element_mult_mode = true;

};

template <class T>
KMatrix<T> operator+(KMatrix<T> lv, const KMatrix<T>& rv);

template <class T>
KMatrix<T> operator-(KMatrix<T> lv, const KMatrix<T>& rv);

template <class T>
KMatrix<T> operator*(KMatrix<T> lv, const KMatrix<T>& rv);

template <class T>
KMatrix<T> operator/(KMatrix<T> lv, const KMatrix<T>& rv);

typedef KMatrix<double> KMat;

template <class T>
KMatrix<T> matrixMult(const KMatrix<T>& a, const KMatrix<T>& b);

template <class T>
KMatrix<T> elementMult(const KMatrix<T>& a, const KMatrix<T>& b);

bool matrixFromString(std::string input, std::vector<std::vector<double> >& out);
bool matrixFromString(std::string input, std::vector<std::vector<int> >& out);

std::string limited_template_to_string(int x);
//std::string limited_template_to_string(long int x);
//std::string limited_template_to_string(long long int x);
//std::string limited_template_to_string(unsigned int x);
//std::string limited_template_to_string(unsigned long int x);
//std::string limited_template_to_string(unsigned long long int x);
//std::string limited_template_to_string(float x);
std::string limited_template_to_string(double x);
std::string limited_template_to_string(std::string x);
std::string limited_template_to_string(bool x);
std::string limited_template_to_string(char x);

class matrix_bounds_excep: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Attempted to access element out of bounds";
    }
} mat_bnd_ex;

class matrix_multiplication_exception: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Attempted to multiply matricies of the wrong size";
    }
} mat_mult_ex;

/*
  IMPLEMENTATION

 */

/*----------------------------------------------------------------
-------------------------- INITIALIZERS --------------------------
----------------------------------------------------------------*/

/*
 Initialize to no specific size or value (will be 0x0).
 */
template <class T>
KMatrix<T>::KMatrix(){

}

/*
 Initializes the matrix to the specified size. Each element is populated with the type's defualt constructor

 rows - number of rows
 cols - number of columns per row
 */
template <class T>
KMatrix<T>::KMatrix(int rows, int cols){

    clear(rows, cols);

}

/*
 Initializes the matrix from a string. (ONLY WORKS FOR KMatrix<double>. ALL OTHER TYPES WILL THROW AN EXCEPTION).
 If it fails without throwing an exception, the matrix will be of size 0x0.

 init - string to initialize matrix. String matrix representation is defined by:
        * Surrounding the string with square brackets ('[]') is optional.
        * Commas separate variables in one row
        * Semicolons separate rows
        * The value in between commas (or semicolons) will be interpreted as a double
 */
template <class T>
KMatrix<T>::KMatrix(std::string init){

//    if (strcmp(typeid(T).name(), "d") != 0){ //doubles only
//        //Wrong data type, do nothing
//        std::cout << "Wrong data type bruv (" << typeid(T).name() << ")" << std::endl;
//    }else{
//        std::vector<std::vector<double> mat_temp;
    if (!matrixFromString(init, mat)){
        mat.clear();
    }
//    }

}

/*
 Initializes the matrix to the size 'rows'x'cols', populating each element with the value of 'init' at that cell.
 
 init - array from which to fill each element
 rows - number of rows in matrix
 cols - number of columns in matrix
 */
template <class T>
KMatrix<T>::KMatrix(T** init, int rows, int cols){
    
    KMatrix<T>::clear();
    for (int r = 0 ; r < rows ; r++){
        std::vector<T> temp;
        temp.resize(cols);
        for (int i = 0 ; i < cols ; i++){
            temp[i] = init[r][i];
        }
        mat.push_back(temp);
    }
    
}

/*
 Initializes the matrix to the size 'rows'x'cols', populating each element with the value 'init'.
 
 init - value with which to fill each element
 rows - number of rows in matrix
 cols - number of columns in matrix
 */
template <class T>
KMatrix<T>::KMatrix(T init, int rows, int cols){
    
    KMatrix<T>::clear();
    for (int r = 0 ; r < rows ; r++){
        std::vector<T> temp;
        temp.resize(cols);
        for (int i = 0 ; i < cols ; i++){
            temp[i] = init;
        }
        mat.push_back(temp);
    }
    
}

/*
 Populates a matrix from the 2D vector 'init'.
 
 init - vector from which to initialize matrix
 */
template <class T>
KMatrix<T>::KMatrix(std::vector<std::vector<T> > init){

    //Determine maximum number of columns
    size_t max_len = -1;
    for (int r = 0 ; r < init.size() ; r++){
        if (init[r].size() > max_len){
            max_len = init[r].size();
        }
    }
    
    //Resize matrix
    clear(init.size(), max_len);
    
    //Populate matrix
    for (int r = 0; r < init.size() ; r++){
        for (int c = 0 ; c < init[r].size() ; c++){
            mat[r][c] = init[r][c];
        }
    }
    
}

/*
 
 */
template <class T>
KMatrix<T>::KMatrix(const KMatrix<T>& init){

    //Resize matrix
    clear(init.rows(), init.cols());
    
    element_mult_mode = init.element_mult_mode;
    
    //Populate matrix
    for (int r = 0; r < init.rows() ; r++){
        for (int c = 0 ; c < init.cols() ; c++){
            mat[r][c] = init.get(r, c);
        }
    }
    
}

template <class T>
KMatrix<T>::~KMatrix(){

}

/*
 Clears the matrix, resulting in a 0x0 matrix
 */
template <class T>
void KMatrix<T>::clear(){
    mat.clear();
}

/*
 Clears the matrix, and resizes the matrix to 'rows' x 'cols'. Each element is populated by the type's default constructor.

 rows - number of rows
 cols - number of columns per row

 Void return
 */
template <class T>
void KMatrix<T>::clear(int rows, int cols){

    KMatrix<T>::clear();
    for (int r = 0 ; r < rows ; r++){
        std::vector<T> temp;
        temp.resize(cols);
        mat.push_back(temp);
    }
}

template <class T>
void swapMat(KMatrix<T>& first, KMatrix<T>& second){ //friend
    std::swap(first.getMat(), second.getMat());
    std::swap(first.getElementMultMode(), second.getElementMultMode());
}

//Operators

template <class T>
KMatrix<T>& KMatrix<T>::operator+=(const KMatrix<T>& rv){
    
    int row_max = (rv.rows() > this->rows())? (int)rv.rows() : (int)this->rows();
    int col_max = (rv.cols() > this->cols())? (int)rv.cols() : (int)this->cols();
    
    KMatrix out = zero(row_max, col_max);
    
    //Add origional matrix components
    for (int r = 0 ; r < this->rows() ; r++){
        for (int c = 0 ; c < this->cols() ; c++){
            out(r, c) = this->operator()(r, c);
        }
    }
    
    //Add rv matrix components
    for (int r = 0 ; r < rv.rows() ; r++){
        for (int c = 0 ; c < rv.cols() ; c++){
            out(r, c) += rv.get(r, c);
        }
    }

//    *this = out;
    swapMat(*this, out);
    
    return *this;
}

template <class T>
KMatrix<T>& KMatrix<T>::operator*=(const KMatrix<T>& rv){

    if (element_mult_mode){ //Perform element-wise multiplication
        *this = elementMult(*this, rv);
        std::cout << "EM" << std::endl;
    }else{ //Perform matrix multiplication
        *this = matrixMult((*this), rv);
        std::cout << "MM" << std::endl;
    }
    
    return *this;
}

template <class T>
KMatrix<T>& KMatrix<T>::operator-=(const KMatrix<T>& rv){
    
    int row_max = (rv.rows() > this->rows())? (int)rv.rows() : (int)this->rows();
    int col_max = (rv.cols() > this->cols())? (int)rv.cols() : (int)this->cols();
    
    KMatrix out = zero(row_max, col_max);
    
    //Add origional matrix components
    for (int r = 0 ; r < this->rows() ; r++){
        for (int c = 0 ; c < this->cols() ; c++){
            out(r, c) = this->operator()(r, c);
        }
    }
    
    //Add rv matrix components
    for (int r = 0 ; r < rv.rows() ; r++){
        for (int c = 0 ; c < rv.cols() ; c++){
            out(r, c) -= rv.get(r, c);
        }
    }
    
    //    *this = out;
    swapMat(*this, out);
    
    return *this;
    
}

template <class T>
KMatrix<T>& KMatrix<T>::operator/=(const KMatrix<T>& rv){
    
    //Check that the matricies' dimensions match
    if (this->rows() != rv.rows() || this->cols() != rv.cols()){
        throw mat_mult_ex;
    }
    
    //Iterate through each element, calculate result
    T sum;
    for (unsigned int r = 0 ; r < this->rows() ; r++){
        for (unsigned int c = 0 ; c < this->rows() ; c++){
            
            //Calculate product
            this->operator()(r, c) /= rv.get(r, c);
            
        }
    }
    
    return *this;
}

//template <class T>
//KMatrix<T>& KMatrix<T>::operator+=(const double& rv){
//
//}

//template <class T>
//KMatrix<T>& KMatrix<T>::operator*=(const double& rv){
//
//}

//template <class T>
//KMatrix<T>& KMatrix<T>::operator-=(const double& rv){
//
//}

//template <class T>
//KMatrix<T>& KMatrix<T>::operator/=(const double& rv){
//
//}

template <class T>
KMatrix<T>& KMatrix<T>::operator=(KMatrix rh){
    swapMat(*this, rh);
    return *this;
}

template <class T>
T& KMatrix<T>::operator()(int r, int c){
    
    if(r >= rows() || c >= cols()){
        throw mat_bnd_ex;
    }
    
    return KMatrix<T>::mat[r][c];
}

template <class T>
T KMatrix<T>::get(int r, int c) const{
    return KMatrix<T>::mat[r][c];
}

template <class T>
bool KMatrix<T>::operator=(std::string rv){

}

//template <class T>
//bool KMatrix<T>::operator=(Eigen::MatrixXd rv){
//
//}

template <class T>
bool KMatrix<T>::operator=(std::vector<std::vector<double> > rv){

}

//
template <class T>
size_t KMatrix<T>::rows() const{
    return mat.size();
}

template <class T>
size_t KMatrix<T>::cols() const{
    
    if (mat.size() < 1) return 0;
    
    return mat[0].size();
}

//void setSize(int rows, int cols){
//
//}

/*
 Converts the matrix to a string and prints it. Only going to work for doubles and other basic types supported by std::to_string()!
 
 options - string containing output options. Options are specified as one character flags
     flags:
         [ or ] : surround each line with brackets
         | : surround each line with pipe character
         o : ouput as one line
         m : output as multiple lines, one for each row
         u : print booleans in uppercase
         " : surround strings with double quotes
         ' : surround strings with single quotes
 
 Returns string representing matrix
 
 TODO:
 Later, add option so that all columns' values align with eachother
 */
template <class T>
std::string KMatrix<T>::to_string(std::string options){

    bool use_brackets = false; //flag = [ || ]
    bool use_pipe = false; //flag = | (pipe, not l or 1)
    bool one_line = true; //flag = o (multiline = m)
    bool bool_uppercase = false; //flag = u (print booleans in uppercase)
    bool quote_strings = false; //flag = \" (print strings surrounded by double quotes)
    bool single_quote_strings = false; //flag = \" (print strings surrounded by double quotes)
    
    //Read in options
    if (options.find("[", 0) != std::string::npos || options.find("]") != std::string::npos){
        use_brackets = true;
        use_pipe = false;
    }
    if (options.find("|", 0) != std::string::npos){
        use_brackets = false;
        use_pipe = true;
    }
    if (options.find("m") != std::string::npos){
        one_line = false;
    }
    if (options.find("o") != std::string::npos){
        one_line = true;
    }
    if (options.find("u") != std::string::npos){
        bool_uppercase = true;
    }
    if (options.find("\"") != std::string::npos){
        quote_strings = true;
        single_quote_strings = false;
    }
    if (options.find("'") != std::string::npos){
        single_quote_strings = true;
        quote_strings = false;
    }
    
    std::string out;
    
    for (int r = 0 ; r < KMatrix<T>::mat.size() ; r++){
        
        //Add beginning of line character if output uses multiple lines
        if (!one_line){
            if (use_brackets){
                out = out + "[ ";
            }else if(use_pipe){
                out = out + "| ";
            }
        }
        
        //Loop through each element of the row...
        for (int c = 0 ; c < KMatrix<T>::mat[r].size() ; c++){
            
            if (strcmp(typeid(T).name(), "d") == 0 || strcmp(typeid(T).name(), "i") == 0 || strcmp(typeid(T).name(), "l") == 0 || strcmp(typeid(T).name(), "x") == 0 || strcmp(typeid(T).name(), "j") == 0 || strcmp(typeid(T).name(), "m") == 0 || strcmp(typeid(T).name(), "y") == 0 || strcmp(typeid(T).name(), "f") == 0 || strcmp(typeid(T).name(), "e") == 0 || strcmp(typeid(T).name(), "c") == 0 ){ //Values for which std::to_string() are defined
                out = out + limited_template_to_string(KMatrix<T>::mat[r][c]); //Add next element
//            }else if(strcmp(typeid(T).name(), "b") == 0){ //Bools
//                out = out + bool_to_str(KMatrix<T>::mat[r][c], bool_uppercase); //Add next element
//            }else if(strcmp(typeid(T).name(), "c") == 0){ //Chars
//                out = out + std::to_string((int)(KMatrix<T>::mat[r][c])); //Add next element
            }else if(strcmp(typeid(T).name(), "b") == 0 ){
                if (bool_uppercase){
                    out = out + to_uppercase(limited_template_to_string(KMatrix<T>::mat[r][c]));
                }else{
                    out = out + limited_template_to_string(KMatrix<T>::mat[r][c]);
                }
            }else if(typeid(T) == typeid(std::string)){
                if (quote_strings){
                    out = out + '"' + limited_template_to_string(KMatrix<T>::mat[r][c]) +'"';
                }else if (quote_strings){
                    out = out + '\'' + limited_template_to_string(KMatrix<T>::mat[r][c]) +'\'';
                }else{
                    out = out + limited_template_to_string(KMatrix<T>::mat[r][c]);
                }
            }else{
                out = out + "?";
            }
            
//            out = out + limited_template_to_string(mat[r][c]); //Add next element

            if (c+1 != KMatrix<T>::mat[r].size()){ //If not at end of row, add comma
                out = out + ", ";
            }
        }
        
        //Add end of line character, whatever is appropriate
        if (!one_line){
            if (use_brackets){
                out = out + " ]";
            }else if(use_pipe){
                out = out + " |";
            }
            out = out + '\n';
        }else if(r+1 < mat.size()){
            out = out + " ; ";
        }
        
    }
    
    //If one line, add brackets or pipes as specified
    if (one_line){
        if (use_brackets){
            out = "[ " + out + " ]";
        }else if(use_pipe){
            out = "| " + out + " |";
        }
    }
    
    return out;
}

/*
 Returns the maximum value contained in the matrix. If no values are contrained (size is 0x0), the function returns the type's default initialization value.
 
 Returns the maximum value.
 */
template <class T>
T KMatrix<T>::max(){
    
    T max_val{};
    
    //Ensure matrix has 1 or more cells
    if (rows() > 0 && cols() > 0){
        max_val = mat[0][0];
    }else{
        return max_val; //Else return max_val unaltered
    }
    
    //Scan for greatesst value
    for (int r = 0 ; r < rows() ; r++){
        for (int c = 0 ; c < cols() ; c++){
            if (mat[r][c] > max_val){
                max_val = mat[r][c];
            }
        }
    }
    
    return max_val;
}

/*
 Returns the minimum value contained in the matrix. If no values are contrained (size is 0x0), the function returns the type's default initialization value.
 
 Returns the minimum value.
 */
template <class T>
T KMatrix<T>::min(){
    T min_val{};
    
    //Ensure matrix has 1 or more cells
    if (rows() > 0 && cols() > 0){
        min_val = mat[0][0];
    }else{
        return min_val; //Else return max_val unaltered
    }
    
    //Scan for lowest value
    for (int r = 0 ; r < rows() ; r++){
        for (int c = 0 ; c < cols() ; c++){
            if (mat[r][c] < min_val){
                min_val = mat[r][c];
            }
        }
    }
    
    return min_val;
}

/*
 Returns the range of values in the matrix (ie. minimum value subtracted from the maximum value).
 
 Returns the range of values contrained in the matrix.
 */
template <class T>
T KMatrix<T>::range(){
    return max() - min();
}

/*
 Calculates the average value (arithmetic mean) of all the values contained in the matrix. (NOTE: This function may not work for all data types).
 */
template <class T>
T KMatrix<T>::avg(){
    
    T sum = 0;
    for (size_t r = 0 ; r < rows() ; r++){
        for (size_t c = 0 ; c < cols() ; c++){
            sum += get(r, c);
        }
    }
    
    return sum/rows()/cols();
    
}

/*
 Calculates the standard deviation of the values in the matrix.
 
 Returns the standard deviation of the values in the matrix.
 */
template <class T>
T KMatrix<T>::stdev(){
    std::cout << "I AM CALCULATING STDEV INCORRECTLY!!" << std::endl;
    T average = this->avg();
    
    T sum = 0;
    for (size_t r = 0 ; r < rows() ; r++){
        for (size_t c = 0 ; c < cols() ; c++){
            sum += (get(r, c)-average)*(get(r, c)-average);
        }
    }
    
    return sum/rows()/cols();
    
}

//Arithmetic Functions

//template <class T>
//KMatrix<T> KMatrix<T>::crossprd(KMatrix<T>& rv){
//
//}
//
//template <class T>
//double KMatrix<T>::dotprd(KMatrix<T>& rv){
//
//}

template <class T>
KMatrix<T> KMatrix<T>::transpose(){

}

template <class T>
KMatrix<T> KMatrix<T>::conjugate(){

}

template <class T>
KMatrix<T> KMatrix<T>::adjoint(){ //Conjugate transpose

}

template <class T>
KMatrix<T> KMatrix<T>::inverse(){

}

template <class T>
bool KMatrix<T>::is_invertable(){

}

template <class T>
KMatrix<T> KMatrix<T>::pseudoinverse(){ //Moore-penrose //DO THIS

}

template <class T>
double KMatrix<T>::determinant(){

}

//Static Functions

/*
 Returns a KMatrix initialized with all values of '0' of the size 'r'x'c'
 
 r - number of rows
 c - number of columns
 
 Returns specified matrix
 */
template <class T>
KMatrix<T> KMatrix<T>::zero(int r, int c){

    KMatrix out;
    
    out.clear(r, c);
    for (int rr = 0 ; rr < r ; rr++){
        for (int cc = 0 ; cc < c ; cc++){
            out(rr, cc) = 0;
        }
    }
    
    return out;
}

/*
 Creates a KMatrix and initializes it to the size 'r'x'c', with the value 'val' in every cell.
 
 val - value to install in every cell
 r - number of rows in output matrix
 c - number of columns in output matrix
 
 Returns the resulting matrix
 */
template <class T>
KMatrix<T> KMatrix<T>::constant(T val, int r, int c){
    
    KMatrix out;
    
    out.clear(r, c);
    for (int rr = 0 ; rr < r ; rr++){
        for (int cc = 0 ; cc < c ; cc++){
            out(rr, cc) = val;
        }
    }
    
    return out;
}

/*
 Creates a KMatrix and initializes it to the size required to fit values ranging from 'start' to 'end', in steps of 'step_size' in each row.
 
 start - starting value for range
 step_size - size to increment between each cell
 end - ending value (cell's will not have values that exceed 'end'
 rows - number of rows. Each row will have identical contents
 
 Returns the resulting matrix
 */
template <class T>
KMatrix<T> KMatrix<T>::makeRange(T start, T step_size, T end, int rows){

//    unsigned int idx;
    std::vector<std::vector<T> > vals;
    std::vector<T> temp_vals;
    for (T i = start ; i <= end ; i += step_size){
        temp_vals.push_back(i);
    }
    
    for (int i = 0 ; i < rows ; i++){
        vals.push_back(temp_vals);
    }
    
    KMatrix result(vals);
    
    return vals;
}

/*
 Set if matrix operates in linear algebrea mode (how matricies are multiplied by eachother)

 lin_alg - if true, multiplies as matrix, else element-wise multiplication

 Void return
 */
template <class T>
void KMatrix<T>::setElementMultMode(bool em){
    KMatrix<T>::element_mult_mode = em;
}

/*
 Multiply two matricies using matrix multiplication.
 
 a - left matrix
 b - right matrix
 
 Returns the result matrix
 */
template <class T>
KMatrix<T> matrixMult(const KMatrix<T>& a, const KMatrix<T>& b){
    
    //Check that the matricies can be multiplied
    if (a.rows() != b.cols()){
        throw mat_mult_ex;
    }
    
    KMatrix<T> result(b.rows(), a.cols());
    
    //Iterate through each element, calculate result
    T sum;
    for (unsigned int r = 0 ; r < result.rows() ; r++){
        for (unsigned int c = 0 ; c < result.rows() ; c++){
            
            //Calculate sum of products
            sum = 0;
            for (unsigned int i = 0 ; i < result.rows() ; i++){
                sum += a.get(r, i) * b.get(i, c);
            }
            result(r, c) = sum;
            
        }
    }
    
    return result;
}

/*
 Multiply two matricies using element-wise multiplication.
 
 a - left matrix
 b - right matrix
 
 Returns the product matrix
 */
template <class T>
KMatrix<T> elementMult(const KMatrix<T>& a, const KMatrix<T>& b){

    //Check that the matricies' dimensions match
    if (a.rows() != b.rows() || a.cols() != b.cols()){
        throw mat_mult_ex;
    }
    
    KMatrix<T> result(a.rows(), a.cols());
    
    //Iterate through each element, calculate result
    T sum;
    for (unsigned int r = 0 ; r < result.rows() ; r++){
        for (unsigned int c = 0 ; c < result.rows() ; c++){
            
            //Calculate product
            result(r, c) = a.get(r, c) * b.get(r, c);
            
        }
    }
    
    return result;
    
}

/*
 Creates a 2D vector of int from a string. The result is saved to 'out'.
 
 input - string interpreted as a matrix
 out - 2D vector in which result is saved
 
 Returns true if creating vector was successful
 */
bool matrixFromString(std::string input, std::vector<std::vector<int> >& out){
    
    std::vector<std::vector<double> > temp;
    bool ret = matrixFromString(input, temp);
    
    out.clear();
    std::vector<int> temp_int;
    for (int i = 0 ; i < temp.size() ; i++){
        temp_int.clear();
        for (int j = 0 ; j < temp[i].size() ; j++){
            temp_int.push_back((int)temp[i][j]);
        }
        out.push_back(temp_int);
    }
    
    return ret;
}

/*
 Creates a 2D vector of doubles from a string. The result is saved to 'out'.

 input - string interpreted as a matrix
 out - 2D vector in which result is saved
 
 Returns true if creating vector was successful
 */
bool matrixFromString(std::string input, std::vector<std::vector<double> >& out){

    int c = -1;
    int r = 0;
    std::vector<double> values;

    ensure_whitespace(input, ",;");
    std::vector<std::string> tokens = parse(input, " ");

    int ticker = 0;
    bool add_line = false;
    bool req_commas = false;
    char phase = 'b';
    for (int i = 0 ; i < tokens.size() ; i++){
        if (isnum(tokens[i])){

            if (req_commas && phase != 'c' && phase != 'n'){
                std::cout << "ERROR: Failed to create matrix from string - missing comma before token " << i << ", '" << tokens[i] << "'. String:\n\t'" << input << "'" << std::endl;
                return false;
            }

            values.push_back(strtod(tokens[i]));
            ticker++;
            add_line = true;
            phase = 'v'; //var added
        }else if(tokens[i] == ";"){

            if (phase == 'c'){
                std::cout << "ERROR: Failed to create matrix from string - semicolon can not follow a comma. String:\n\t'" << input << "'" << std::endl;
                return false;
            }else if(phase == 'b'){
                std::cout << "ERROR: Failed to create matrix from string - semicolon can not be first symbol. String:\n\t'" << input << "'" << std::endl;
                return false;
            }

            if (c == -1){
                c = ticker;
            }else if(c != ticker){
                std::cout << "ERROR: Failed to create matrix from string:\n\t'" << input << "'" << std::endl;
                return false;
            }
            phase = 'n'; //new row
            add_line = false;
            r++;
            ticker = 0;
        }else if(tokens[i] == ","){
            if ( r == 0 && ticker == 1){ //begin
                req_commas = true;
                phase = 'c';
            }else if(req_commas && 'v'){
                phase = 'c'; //Ready for next var
            }else{
                std::cout << "ERROR: Failed to create matrix from string due to invalid comma. String:\n\t'" << input << "'" << std::endl;
                return false;
            }
        }else{

        }
    }

    if (add_line){
        c = ticker;
        r++;
    }

    if (r < 0 || c < 0) return false;
    //Resize and fill array
    out.clear();
    for (int rr = 0 ; rr < r ; rr++){
        std::vector<double> temp;
        for (int cc = 0 ; cc < c ; cc++){
            temp.push_back(values[rr*c + cc]);
//                out[rr][cc] = ;
        }
        out.push_back(temp);
    }

    return true;
}

/*
 Access a reference to the 2D vector containing the matrix's data
 
 Retuns a reference to the matrix's vector.
 */
template <class T>
std::vector<std::vector<T> >& KMatrix<T>::getMat(){
    return mat;
}

/*
 Returns the 'multiplying mode' of the matrix.
 
 Returns true if matrix set to multiply in element-wise fashion, false if set to matrix multiplication.
 */
template <class T>
bool& KMatrix<T>::getElementMultMode(){
    return element_mult_mode;
}

/*
 Adds lv and rv.
 
 lv - left matrix value
 rv - right matrix value
 
 Returns sum of lv and rv.
 */
template <class T>
KMatrix<T> operator+(KMatrix<T> lv, const KMatrix<T>& rv){
    lv += rv;
    return lv;
}

/*
 Subtracts rv from lv.
 
 lv - left matrix
 rv - matrix to subtract from left matrix
 
 Returns (lv - rv)
 */
template <class T>
KMatrix<T> operator-(KMatrix<T> lv, const KMatrix<T>& rv){
    lv -= rv;
    return lv;
}

/*
 Multiplies lv and rv. Multiplication mode (matrix multiplication or element-wise multiplication) determined by set_matrix_mult().
 
 lv - left matrix value
 rv - right matrix value
 
 Returns product of lv and rv.
 */
template <class T>
KMatrix<T> operator*(KMatrix<T> lv, const KMatrix<T>& rv){
    lv *= rv;
    return lv;
}

/*
 Divides lv by rv.
 
 lv - left matrix value
 rv - right matrix value
 
 Returns (lv/rv).
 */
template <class T>
KMatrix<T> operator/(KMatrix<T> lv, const KMatrix<T>& rv){
    lv /= rv;
    return lv;
}














































#endif /* KMatrix_hpp */


























//if (strcmp(typeid(T).name(), "d") == 0 || strcmp(typeid(T).name(), "i") == 0 || strcmp(typeid(T).name(), "l") == 0 || strcmp(typeid(T).name(), "x") == 0 || strcmp(typeid(T).name(), "j") == 0 || strcmp(typeid(T).name(), "m") == 0 || strcmp(typeid(T).name(), "y") == 0 || strcmp(typeid(T).name(), "f") == 0 || strcmp(typeid(T).name(), "e") == 0){ //Values for which std::to_string() are defined
//    out = out + std::to_string(KMatrix<T>::mat[r][c]); //Add next element
//}else if(strcmp(typeid(T).name(), "b") == 0){ //Bools
//    out = out + bool_to_str(KMatrix<T>::mat[r][c], bool_uppercase); //Add next element
//}else if(strcmp(typeid(T).name(), "c") == 0){ //Chars
//    out = out + std::to_string((int)(KMatrix<T>::mat[r][c])); //Add next element
//}else{ //Default to just using stream operator - likely to only work for strings
//    out = out + (KMatrix<T>::mat[r][c]); //Add next element
//}

std::string limited_template_to_string(int x){
    return std::to_string(x);
}

//std::string limited_template_to_string(long int x){
//    return std::to_string(x);
//}
//
//std::string limited_template_to_string(long long int x){
//    return std::to_string(x);
//}
//
//std::string limited_template_to_string(unsigned int x){
//    return std::to_string(x);
//}
//
//std::string limited_template_to_string(unsigned long int x){
//    return std::to_string(x);
//}
//
//std::string limited_template_to_string(unsigned long long int x){
//    return std::to_string(x);
//}

std::string limited_template_to_string(std::string x){
    return x;
}

//std::string limited_template_to_string(float x){
//    return std::to_string(x);
//}

std::string limited_template_to_string(double x){
    return std::to_string(x);
}

std::string limited_template_to_string(bool x){
    return bool_to_str(x);
}

std::string limited_template_to_string(char x){
    return std::to_string((int)(x));
}

