//! @file
//! @brief Suboptions module - header file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2021
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2021, ICT-Project Mariusz Ornowski (ict-project.pl)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the ICT-Project Mariusz Ornowski nor the names
of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************/
#ifndef _ICT_SUBOPTIONS_HEADER
#define _ICT_SUBOPTIONS_HEADER
//============================================
#include <string>
#include <map>
#include <iostream>
#include <stdexcept>
#include "options.hpp"
//============================================
namespace ict { namespace  options { 
//===========================================
//! Mapa przechwująca subopcje
typedef std::map<std::string,std::string> suboptions_t;
//! 
//! @brief Przetwarza subopcje.
//! 
//! @param input Wejściowy ciąg znaków.
//! @param tokens Lista dozwolonych tokenów (rozdzielonych przecinkami).
//! @param output Lista subopcji.
//! @return Jeśli:
//!  @li 0 - wartość ustawiona poprawnie;
//!  @li -1 - wystąpił błąd.
//! 
int subprocess(const std::string & input,const std::string & tokens,suboptions_t & output);
//! Obiekt przechowujący subopcje.
template <const std::string & Tokens> class suboptions: public suboptions_t{
public:
    //! Zwraca listę dozwolonych tokenów.
    const std::string & getTokens(){return Tokens;}
    friend std::istream &operator>>(std::istream  &input, suboptions<Tokens> & so) {
        std::string tmp;
        input>>tmp;
        if (subprocess(tmp,Tokens,so)) throw std::invalid_argument("suboptions error");
        return input;         
    }
};
//===========================================
} }
//============================================
#endif