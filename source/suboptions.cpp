//! @file
//! @brief Suboptions module - Source file.
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
//============================================
#include "suboptions.hpp"
#include <vector>
#include <stdlib.h>
//============================================
namespace ict { namespace  options { 
//============================================
#include "tokenizer.cpp"
//============================================
typedef std::vector<char*> suboptions_vector_t;
int subprocess(const std::string & input,const std::string & tokens,suboptions_t & output){
    tokens_t tk;
    suboptions_vector_t sv;
    tokenizer(tokens,tk);
    for (std::string & t : tk) {
        sv.emplace_back(&t[0]);
    }
    sv.emplace_back(nullptr);
    {
        std::string i(input);
        char *subopts;
        char *value;
        subopts=&i[0];
        while (*subopts!='\0') {
            int k=getsubopt(&subopts,&sv[0],&value);
            if (k<tk.size()){
                output[tk.at(k)]=value?value:"";
            }
        }
    }
    return(0);
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
#include "test.hpp"

static int testProcess(const std::vector<std::string> & input){
    std::vector<std::string> a(input);
    std::vector<char*> v;
    for (auto & s : a) v.push_back(&s[0]);
    v.push_back(nullptr);
    return(ict::options::process(a.size(),&v[0]));
}
std::string opcja1("ala,ma,kota");
ict::options::single<ict::options::suboptions<opcja1>> subparam1("moplf","","ala=nic","Opis subparam1");
ict::options::vector<ict::options::suboptions<opcja1>> subparam2("hjli","","ala=nic\nma=koty","Opis subparam2");

REGISTER_TEST(suboptions,tc1){
    int out=0;
    std::istringstream s("jeden,dwa=alfa,trzy=beta");
    const static std::string opcja("jeden,dwa,trzy");
    ict::options::suboptions<opcja> o;
    s>>o;
    if (!out) if (o.size()!=3) out=101;
    if (!out) if (o.count("jeden")!=1) out=102;
    if (!out) if (o.at("jeden")!="") out=103;
    if (!out) if (o.count("dwa")!=1) out=104;
    if (!out) if (o.at("dwa")!="alfa") out=105;
    if (!out) if (o.count("trzy")!=1) out=106;
    if (!out) if (o.at("trzy")!="beta") out=107;
    return(out);
}
REGISTER_TEST(suboptions,tc2){
    int out=0;
    out=testProcess({"test","-m","ala=foo,kota=alfa,ma=beta"});
    if (!out) if (subparam1.value.size()!=3) out=101;
    if (!out) if (subparam1.value.count("ala")!=1) out=102;
    if (!out) if (subparam1.value.at("ala")!="foo") out=103;
    if (!out) if (subparam1.value.count("ma")!=1) out=104;
    if (!out) if (subparam1.value.at("ma")!="beta") out=105;
    if (!out) if (subparam1.value.count("kota")!=1) out=106;
    if (!out) if (subparam1.value.at("kota")!="alfa") out=107;    
    return(out);
}
REGISTER_TEST(suboptions,tc3){
    int out=0;
    out=testProcess({"test","-h","ala=foo,kota=alfa,ma=beta"});
    if (!out) if (subparam2.value.size()!=1) out=101;
    if (!out) if (subparam2.value.at(0).size()!=3) out=102;
    if (!out) if (subparam2.value.at(0).count("ala")!=1) out=103;
    if (!out) if (subparam2.value.at(0).at("ala")!="foo") out=104;
    if (!out) if (subparam2.value.at(0).count("ma")!=1) out=105;
    if (!out) if (subparam2.value.at(0).at("ma")!="beta") out=106;
    if (!out) if (subparam2.value.at(0).count("kota")!=1) out=107;
    if (!out) if (subparam2.value.at(0).at("kota")!="alfa") out=108;    
    return(out);
}
REGISTER_TEST(suboptions,tc5){
    int out=0;
    out=testProcess({"test"});
    if (!out) if (subparam1.value.size()!=1) out=101;
    if (!out) if (subparam1.value.count("ala")!=1) out=102;
    if (!out) if (subparam1.value.at("ala")!="nic") out=103;
    if (!out) if (subparam2.value.size()!=2) out=104;
    if (!out) if (subparam2.value.at(0).size()!=1) out=105;
    if (!out) if (subparam2.value.at(0).count("ala")!=1) out=106;
    if (!out) if (subparam2.value.at(0).at("ala")!="nic") out=107;
    if (!out) if (subparam2.value.at(1).size()!=1) out=108;
    if (!out) if (subparam2.value.at(1).count("ma")!=1) out=109;
    if (!out) if (subparam2.value.at(1).at("ma")!="koty") out=110;
    return(out);
}
#endif
//===========================================