//
//  KVector.hpp
//  KMatrix
//
//  Created by Grant Giesbrecht on 1/26/18.
//  Copyright © 2018 IEGA. All rights reserved.
//

#ifndef KVector_hpp
#define KVector_hpp

#include <stdio.h>
#include <string>
#include <GiesbrechtCPP/string_manip.hpp>
#include <iostream>
#include <exception>
#include <cmath>
#include "KMatrix.hpp"
#include "KMatrixHelpers.hpp"

template <class T>
class KVector : public KMatrix<T> {
    
public:
	
	//KVector native constructors
	KVector();
	KVector(int elements);
	KVector(const KVector<T>& init);
	KVector(std::vector<T> init);
	KVector(std::string init);
	KVector(T* init, int elements);
	KVector(T init, int elements);
	
	//From KMatrix (redefined to prevent multi-row vectors)
	KVector(int rows, int cols);
	KVector(T** init, int rows, int cols);
	KVector(T init, int rows, int cols);
	KVector(std::vector<std::vector<T> > init);
	KVector(const KMatrix<T>& init);
	
	void clear();
	void clear(int elements);
	
	T get(int element) const;
	std::vector<T> get_vec();
	KVector<T>& operator=(KVector rh);
	KVector<T>& operator=(std::string rv);
	KVector<T>& operator=(std::vector<double> rv);
	
	size_t size();
	void setSize(size_t ns);
	
	static KVector zero(size_t elements); //TODO
	static KVector constant(T val, size_t elements); //TODO
	static KVector range(T start, T step_size, T end); //TODO
	
	T& operator[](int idx);
	
private:
	
	/*
	 These functions are from KMatrix and do not apply to KVector
	 */
	
	using KMatrix<T>::clear;
	
	using KMatrix<T>::rows;
	using KMatrix<T>::cols;
//	using KMatrix<T>::setSize;
	
	using KMatrix<T>::get;
	using KMatrix<T>::get_rowv;
	using KMatrix<T>::operator=;
	
	using KMatrix<T>::operator();
	
	using KMatrix<T>::range;
	using KMatrix<T>::zero;
	using KMatrix<T>::constant;
	
	using KMatrix<T>::setElementMultMode;
	using KMatrix<T>::getElementMultMode;
	
	//TODO: Block some matrix operations (such as invert)
	
};

typedef KVector<double> KVec;



//template <class T>
//class KMatrix {
//public:
//
//	//Initializers
//	KMatrix();
//	KMatrix(int rows, int cols);
//	KMatrix(std::string init);
//	KMatrix(T** init, int rows, int cols);
//	KMatrix(T init, int rows, int cols);
//	KMatrix(std::vector<std::vector<T> > init);
//	KMatrix(const KMatrix<T>& init);
//	~KMatrix();
//
//	//Open/close functions
//	void clear();
//	void clear(int rows, int cols);
//
//	friend void swap(KMatrix<T>& first, KMatrix<T>& second);
//
//	//Operators
//	KMatrix<T>& operator+=(const KMatrix<T>& rv);
//	KMatrix<T>& operator*=(const KMatrix<T>& rv);
//	KMatrix<T>& operator-=(const KMatrix<T>& rv);
//	KMatrix<T>& operator/=(const KMatrix<T>& rv);
//	KMatrix<T>& operator+=(const T& rv);
//	KMatrix<T>& operator*=(const T& rv);
//	KMatrix<T>& operator-=(const T& rv);
//	KMatrix<T>& operator/=(const T& rv);
//	KMatrix<T>& operator=(KMatrix rh);
//	T& operator()(int r, int c);
//	T get(int r, int c) const;
//	std::vector<T> get_rowv(size_t row);
//	bool operator=(std::string rv);
//	//    bool operator=(Eigen::MatrixXd rv);
//	bool operator=(std::vector<std::vector<double> > rv);
//
//	//
//
//	size_t rows() const;
//	size_t cols() const;
//	//    void setSize(int rows, int cols);
//
//	std::string to_string(std::string options="");
//
//	T max();
//	T min();
//	T range();
//	T avg();
//	T stdev();
//
//	//Arithmetic Functions
//	//    KMatrix crossprd(KMatrix& rv);
//	//    double dotprd(KMatrix& rv);
//	KMatrix transpose();
//	KMatrix conjugate();
//	KMatrix adjoint(); //Conjugate transpose
//	KMatrix inverse();
//	bool is_invertable();
//	KMatrix pseudoinverse(); //Moore-penrose //DO THIS
//	double determinant();
//
//	//Static Functions
//	static KMatrix zero(int r, int c);
//	static KMatrix zero(int rc);
//	static KMatrix constant(T val, int r, int c);
//	static KMatrix range(T start, T step_size, T end, int rows=1);
//	//    static std::vector<std::vector<double> > KMatrix_to_vector(KMatrix km);
//
//	//Other
//	void setElementMultMode(bool em);
//	std::vector<std::vector<T> >& getMat();
//	bool& getElementMultMode();
//
//protected:
//
//	std::vector<std::vector<T> > mat;
//	bool element_mult_mode = true;
//
//	matrix_bounds_excep mat_bnd_ex;
//	matrix_multiplication_exception mat_mult_ex;
//};


typedef KVector<double> KVec;

/*----------------------------------------------------------------
 -------------------------- INITIALIZERS --------------------------
 ----------------------------------------------------------------*/

/*
 Initialize to no specific size or value (will be 0).
 */
template <class T>
KVector<T>::KVector(){
	
}

/*
 Initializes the kvector to the specified size. Each element is populated with the type's defualt constructor
 
 elements - number of elements
 
 */
template <class T>
KVector<T>::KVector(int elements){
	
	clear(elements);
	
	KMatrix<T>::element_mult_mode = true;
	
}

template <class T>
KVector<T>::KVector(const KVector<T>& init){
	
	//Resize matrix
	clear(init.size());
	
	KMatrix<T>::element_mult_mode = true;
	
	//Populate matrix
	for (int c = 0 ; c < init.size() ; c++){
		KMatrix<T>::mat[0][c] = init[c];
	}
	
}

template <class T>
KVector<T>::KVector(std::vector<T> init){

	clear();
	
	KMatrix<T>::mat.push_back(init);
	
	KMatrix<T>::element_mult_mode = true;
}

//From KMatrix (redefined to prevent multi-row vectors)

/*
 Initializes the KVector from a string. (ONLY WORKS FOR KVector<double>. ALL OTHER TYPES WILL THROW AN EXCEPTION).
 If it fails without throwing an exception, the vector will be of size 0.
 
 init - string to initialize matrix. String matrix representation is defined by:
 * Surrounding the string with square brackets ('[]') is optional.
 * Commas separate variables in one row
 * Semicolons separate rows
 * The value in between commas (or semicolons) will be interpreted as a double
 */
template <class T>
KVector<T>::KVector(std::string init){
	
	//Clear matrix
	KMatrix<T>::mat.clear();
	
	//Create a temporary matrix
	std::vector<std::vector<T> > temp_mat;
	
	//Read the string
	if (matrixFromString(init, temp_mat)){
		if (temp_mat.size() > 0){ //If the temp matrix isn't empty, copy the first row only
			KMatrix<T>::mat.push_back(temp_mat[0]);
		}
	}
	
	KMatrix<T>::element_mult_mode = true;
}

template <class T>
KVector<T>::KVector(T* init, int elements){
	
	clear();
	std::vector<T> temp;
	temp.resize(elements);
	for (int i = 0 ; i < elements ; i++){
		temp[i] = init[i];
	}
	KMatrix<T>::mat.push_back(temp);
	
	KMatrix<T>::element_mult_mode = true;
	
}


template <class T>
KVector<T>::KVector(T init, int elements){
	
	KMatrix<T>::clear();
	std::vector<T> temp;
	temp.resize(elements);
	for (int i = 0 ; i < elements ; i++){
		temp[i] = init;
	}
	KMatrix<T>::mat.push_back(temp);
	
	KMatrix<T>::element_mult_mode = true;
}

/*
 *****************************************  ALERT  **************************************************
 THIS FUNCTION IS CARRIED OVER FROM KMATRIX - IT IS NOT DESIGNED TO BE USED WITH KVECTOR
 No errors will be thrown, but 'rows' variable will be unused and code readibility will suffer.
 ****************************************************************************************************
 
 Initializes the kvector to the size 'cols', populating each element with the value of 'init' at that cell.
 
 rows - Unused
 cols - number of columns in matrix
 */
template <class T>
KVector<T>::KVector(int rows, int cols){
	
	clear(cols);
	
	KMatrix<T>::element_mult_mode = true;
}

/*
 *****************************************  ALERT  **************************************************
       THIS FUNCTION IS CARRIED OVER FROM KMATRIX - IT IS NOT DESIGNED TO BE USED WITH KVECTOR
    No errors will be thrown, but 'rows' variable will be unused and code readibility will suffer.
 ****************************************************************************************************
 
 Initializes the kvector to the size 'cols', populating each element with the value of 'init' at that cell.
 
 init - array from which to fill each element
 rows - Unused
 cols - number of columns in matrix
 */
template <class T>
KVector<T>::KVector(T** init, int rows, int cols){
	
	clear();
	std::vector<T> temp;
	temp.resize(cols);
	for (int i = 0 ; i < cols ; i++){
		temp[i] = init[0][i];
	}
	KMatrix<T>::mat.push_back(temp);
	
	KMatrix<T>::element_mult_mode = true;
}

/*
 *****************************************  ALERT  **************************************************
 	THIS FUNCTION IS CARRIED OVER FROM KMATRIX - IT IS NOT DESIGNED TO BE USED WITH KVECTOR
 	No errors will be thrown, but 'rows' variable will be unused and code readibility will suffer.
 ****************************************************************************************************
 
 Initializes the matrix to the size 'cols', populating each element with the value 'init'.
 
 init - value with which to fill each element
 rows - unused
 cols - number of columns in matrix
 */
template <class T>
KVector<T>::KVector(T init, int rows, int cols){
	
	KMatrix<T>::clear();
	std::vector<T> temp;
	temp.resize(cols);
	for (int i = 0 ; i < cols ; i++){
		temp[i] = init;
	}
	KMatrix<T>::mat.push_back(temp);
	
	KMatrix<T>::element_mult_mode = true;
}

/*
 *****************************************  ALERT  **************************************************
 	THIS FUNCTION IS CARRIED OVER FROM KMATRIX - IT IS NOT DESIGNED TO BE USED WITH KVECTOR
 	No errors will be thrown, but only the first row will be used and code readibility will suffer.
 ****************************************************************************************************
 
 Populates a KVector from the 2D vector 'init'.
 
 init - vector from which to initialize matrix
 */
template <class T>
KVector<T>::KVector(std::vector<std::vector<T> > init){

	clear();
	if (init.size() > 0){
		KMatrix<T>::mat.push_back(init[0]);
	}
	
	KMatrix<T>::element_mult_mode = true;
}

/*
 *****************************************  ALERT  **************************************************
 THIS FUNCTION IS CARRIED OVER FROM KMATRIX - IT IS NOT DESIGNED TO BE USED WITH KVECTOR
 No errors will be thrown, but 'rows' variable will be unused and code readibility will suffer.
 ****************************************************************************************************
 
 Initializes the KVector from the first row of a KMatrix
 */
template <class T>
KVector<T>::KVector(const KMatrix<T>& init){
	
	clear();
	
	if (init.rows() > 0){
		KMatrix<T>::mat.push_back(init.get_rowv(0));
	}
	
	KMatrix<T>::element_mult_mode = true;
}

/*
 Clears the vector, resulting in a 0 element vector
 */
template <class T>
void KVector<T>::clear(){
	
	KMatrix<T>::mat.clear();
}

/*
 Clears the KVector, and resizes to the length 'elements'. Each element is populated by the type's default constructor.
 
 elements - number of elements in the KVector
 
 Void return
 */
template <class T>
void KVector<T>::clear(int elements){
	
	KVector<T>::clear();
	std::vector<T> temp;
	temp.resize(elements);
	KMatrix<T>::mat.push_back(temp);
}

/*
 Returns the number of elements in the KVector
 */
template <class T>
size_t KVector<T>::size(){
	
	if (KMatrix<T>::mat.size() > 0){
		return KMatrix<T>::mat[0].size();
	}else{
		return 0;
	}
	
}

/*
 
 */
template <class T>
T& KVector<T>::operator[](int idx){
	
	if(KMatrix<T>::mat.size() < 1 || idx >= this->size()){
		throw KMatrix<T>::mat_bnd_ex;
	}
	
	return KMatrix<T>::mat[0][idx];
}

//template <class T>
//T& KVector<T>::operator()(int element){
//
//	//Check bounds
//	if(KMatrix<T>::mat.size() < 1 || element > KMatrix<T>::mat[0].size()){
//		throw KMatrix<T>::mat_bnd_ex;
//	}
//
//	return KMatrix<T>::mat[0][element];
//}

template <class T>
T KVector<T>::get(int element) const{

	//Check bounds
	if(KMatrix<T>::mat.size() < 1 || element > KMatrix<T>::mat[0].size()){
		throw KMatrix<T>::mat_bnd_ex;
	}

	return KMatrix<T>::mat[0][element];
}

template <class T>
std::vector<T> KVector<T>::get_vec(){
	
	//Check bounds, throw error if violated
	if (KMatrix<T>::mat.size() < 1){
		throw KMatrix<T>::mat_bnd_ex;
	}
	
	//Return row
	return KMatrix<T>::mat[0];
}

/*
 
 */
template <class T>
KVector<T>& KVector<T>::operator=(KVector rh){
	swapMat(*this, rh);
	return *this;
}

template <class T>
KVector<T>& KVector<T>::operator=(std::string init){
	
	//Create a temporary matrix
	std::vector<std::vector<T> > temp_mat;
	
	//Read the string
	if (matrixFromString(init, temp_mat)){
		if (temp_mat.size() > 0){ //If the temp matrix isn't empty, copy the first row only
			if (KMatrix<T>::mat.size() != 1){
				clear();
				std::vector<double> temp;
				KMatrix<T>::mat.push_back(temp);
			}
			KMatrix<T>::mat[0] = (temp_mat[0]);
		}
	}
	
	return *this;
}

//template <class T>
//bool KMatrix<T>::operator=(Eigen::MatrixXd rv){
//
//}

template <class T>
KVector<T>& KVector<T>::operator=(std::vector<double> rv){
	if (KMatrix<T>::mat.size() != 1){
		clear();
		std::vector<double> temp;
		KMatrix<T>::mat.push_back(temp);
	}
	KMatrix<T>::mat[0] = (rv);
	
	return *this;
}

/*
 Resizes the KVector to 'ns' elements
 
 ns - new length of vector
 
 Void return
 */
template <class T>
void KVector<T>::setSize(size_t ns){
	if (KMatrix<T>::mat.size() < 1){
		std::vector<double> temp;
		KMatrix<T>::mat.push_back(temp);
	}
	
	KMatrix<T>::mat[0].resize(ns);
}

/*
 
 */
template <class T>
KVector<T> range(T start, T step_size, T end){
	return KVector<T>(KMatrix<T>::range(start, step_size, end, 1));
}

#endif /* KVector_hpp */
