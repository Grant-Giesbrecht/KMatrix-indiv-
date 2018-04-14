//
//  KMatrixHelpers.hpp
//  KMatrix
//
//  Created by Grant Giesbrecht on 4/14/18.
//  Copyright © 2018 IEGA. All rights reserved.
//

#ifndef KMatrixHelpers_hpp
#define KMatrixHelpers_hpp

#include <stdio.h>
#include <string>

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
    virtual const char* what() const throw();
};

class matrix_multiplication_exception: public std::exception
{
    virtual const char* what() const throw();
};

#endif /* KMatrixHelpers_hpp */
