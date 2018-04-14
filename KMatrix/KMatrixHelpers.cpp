//
//  KMatrixHelpers.cpp
//  KMatrix
//
//  Created by Grant Giesbrecht on 4/14/18.
//  Copyright © 2018 IEGA. All rights reserved.
//

#include "KMatrixHelpers.hpp"
#include <vector>
#include <IEGA/stdutil.hpp>
#include <IEGA/string_manip.hpp>

const char* matrix_bounds_excep::what() const throw(){
    return "Attempted to access element out of bounds";
}

const char* matrix_multiplication_exception::what() const throw(){
    return "Attempted to multiply matricies of the wrong size";
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
