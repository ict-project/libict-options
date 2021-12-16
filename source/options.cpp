//! @file
//! @brief Options module - Source file.
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
#include "options.hpp"
#include <unistd.h>
#include <map>
#include <set>
#include <getopt.h>
//============================================
namespace ict { namespace  options { 
//============================================
#include "tokenizer.cpp"
//============================================
typedef std::map<char,interface*> shortOptionMap_t;
typedef std::map<std::string,interface*> longOptionMap_t;
typedef std::vector<interface*> optionMap_t;
typedef std::set<interface*> optionSet_t;
shortOptionMap_t & getShortOptionMap(){
    static shortOptionMap_t m;
    return(m);
}
longOptionMap_t & getLongOptionMap(){
    static longOptionMap_t m;
    return(m);
}
optionMap_t & getOptionMap(){
    static optionMap_t m;
    return(m);
}
notOption_t & getNotOption(){
    static notOption_t m;
    return(m);
}
//============================================
void interface::registerOpt(interface*ptr){
    if (ptr){
        bool found=false;
        tokens_t tokens;
        getOptionMap().push_back(ptr);
        for (const char & c : ptr->getShortOption()) if (std::isalnum(c)){
            if (!getShortOptionMap().count(c)){
                getShortOptionMap()[c]=ptr;
                found=true;
                break;
            }
        }
        tokenizer(ptr->getLongOption(),tokens);
        for (const std::string & t : tokens) {
            if (!getLongOptionMap().count(t)){
                getLongOptionMap()[t]=ptr;
                found=true;
                break;
            }
        }
        if (!found) for(unsigned int k=1;k<1000;k++){
            std::string s("param-");
            s+=std::to_string(k);
            if (!getLongOptionMap().count(s)){
                getLongOptionMap()[s]=ptr;
                found=true;
                break;
            }
        }
    }
}
void interface::init(
    const std::string & shortOpt,
    const std::string & longOpt,
    const std::string & defaultVal,
    const std::string & defaultDesc,
    const locale_desc_t & localeDesc
){
    shortOption=shortOpt;
    longOption=longOpt;
    defaultValue=defaultVal;
    description=localeDesc;
    description.emplace_back(description_t{"",defaultDesc});
    registerOpt(this);
}

//============================================
typedef std::vector<struct option> longOptionsVector_t;
int process(int argc,char * const argv[]){
    int option_index=0;
    std::string shortOptionsString(":");
    longOptionsVector_t longOptionsVector;
    {
        for (auto & ptr : getOptionMap()) ptr->clear();
        getNotOption().clear();
    }
    for (shortOptionMap_t::const_iterator it=getShortOptionMap().cbegin();it!=getShortOptionMap().cend();++it){
        shortOptionsString+=it->first;
        if (it->second->getType()!=counter::type){//Parametr
            shortOptionsString+=':';
            if (!it->second->isValueRequired()){//Parametr nieobowiązkowy
                shortOptionsString+=':';
            }
        }
    }
    for (longOptionMap_t::const_iterator it=getLongOptionMap().cbegin();it!=getLongOptionMap().cend();++it){
        if (it->second->getType()==counter::type){//Brak parametru
            longOptionsVector.emplace_back(option{it->first.c_str(),no_argument,NULL,0});
        } else {
            if (it->second->isValueRequired()){//Parametr obowiązkowy
                longOptionsVector.emplace_back(option{it->first.c_str(),required_argument,NULL,0});
            } else {//Parametr nieobowiązkowy
                longOptionsVector.emplace_back(option{it->first.c_str(),optional_argument,NULL,0});
            }
        }
    }
    longOptionsVector.emplace_back(option{NULL,no_argument,NULL,0});
    optind=1;
    {
        int c;
        while (1) {
            int option_index=0;
            c=getopt_long(argc,argv,shortOptionsString.c_str(),longOptionsVector.data(),&option_index);
            if (c==-1) {
                break;
            } else if (std::isalnum(c)){//Zapisanie krótkiej opcji (jeśli jest)
                if (getShortOptionMap().count(c)) 
                    if (getShortOptionMap()[c]->setValue(optarg?optarg:"")!=0) 
                        return(-3);
            } else switch (c) {
                case 0://Zapisanie długiej opcji
                    if (getLongOptionMap().count(longOptionsVector.at(option_index).name)) 
                        if (getLongOptionMap()[longOptionsVector.at(option_index).name]->setValue(optarg?optarg:"")!=0) 
                            return(-3);
                break;
                case '?': //Short option character is not in optstring
                    return(-1);
                break;
                case ':': //Missing mandatory parameter
                    return(-2);
                break;
                default:
                break;
            }
        }
        while (optind<argc){
            getNotOption().push_back(argv[optind++]);
        }
    }
    return(0);
}
void help(interface * ptr,std::ostream & os,const std::string & locale){
    bool present=false;
    bool first=true;
    for (shortOptionMap_t::const_iterator it=getShortOptionMap().cbegin();it!=getShortOptionMap().cend();++it) if (it->second==ptr){
        if (first){
            first=false;
        } else {
            os<<',';
        }
        os<<'-'<<it->first;
    }
    for (longOptionMap_t::const_iterator it=getLongOptionMap().cbegin();it!=getLongOptionMap().cend();++it) if (it->second==ptr) {
        if (first){
            first=false;
        } else {
            os<<',';
        }
        os<<"--"<<it->first;
    }
    os<<' ';
    if (ptr->getType()!=counter::type){
        if (ptr->isValueRequired()){//Parametr obowiązkowy
            os<<"<mandatory param>";
        } else {//Parametr nieobowiązkowy
            os<<"<optional param>";
        }
        os<<' ';
    }
    first=true;
    for (const description_t & d : ptr->getDescription()) {
        if (locale==d.locale){
            present=true;
            if (first){
                first=false;
            } else {
                os<<' ';
            }
            os<<d.desc;
        }
    }
    if (!present) for (const description_t & d : ptr->getDescription()) {
        if (d.locale.empty()){
            present=true;
            if (first){
                first=false;
            } else {
                os<<' ';
            }
            os<<d.desc;
        }
    }
    first=true;
    if (!ptr->getDefaultValue().empty()){
        os<<" Default: ";
        if (ptr->getType()==vector<int>::type){
            std::string val;
            std::istringstream s(ptr->getDefaultValue());
            while (std::getline(s,val,'\n')){
                if (first){
                    first=false;
                } else {
                    os<<',';
                };
                os<<"\""<<val<<"\"";
            }
        } else {
            os<<"\""<<ptr->getDefaultValue()<<"\"";
        }
        os<<".";
    }
    os<<std::endl;
}
void help(std::ostream & os,const std::string & locale){
    optionSet_t optionSet;
    for (shortOptionMap_t::const_iterator it=getShortOptionMap().cbegin();it!=getShortOptionMap().cend();++it){
        if (!optionSet.count(it->second)){
            optionSet.insert(it->second);
            help(it->second,os,locale);
        }
    }
    for (longOptionMap_t::const_iterator it=getLongOptionMap().cbegin();it!=getLongOptionMap().cend();++it){
        if (!optionSet.count(it->second)){
            optionSet.insert(it->second);
            help(it->second,os,locale);
        }
    }
}
const notOption_t & noOptions(){
    return(getNotOption());
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
ict::options::counter param1("cfj","ala ma kota","5","Opis param1 (licznik domyślnie ustawiony na 5).");
ict::options::counter param2("ush","brak kotow","","Opis param2 (licznik bez domyślnej wartości).");
ict::options::single<int> param3("cbe","calkiem inny parametr","-50","Opis param3 (int domyślnie ustawiony na -50).");
ict::options::single<int> param4("kvbe","inny param","","Opis param4 (int bez domyślnej wartości).");
ict::options::vector<double> param5("cipsf","inna tabela parametrow","67.6\n98.0","Opis param5 (tablica double domyślnie ustawiona na 67.6 i 98.0).");
ict::options::vector<double> param6("obfe","","","Opis param6 (tablica double bez domyślnej wartości).");
ict::options::counter param7("","","","Opis param7 (licznik bez domyślnej wartości) i bez nazwy.");
ict::options::single<std::string> param8("uvfg","string","Ala ma kota","Opis param8 (string domyślnie ustawiony na Ala ma kota).");
ict::options::single<std::string> param9("lgso","inny string","","Opis param9 (string bez domyślnej wartości).");

REGISTER_TEST(options,tc1){
    int out=0;
    ict::options::help(std::cout);
    return(out);
}
REGISTER_TEST(options,tc2){
    int out=0;
    if (testProcess({"test","--nieznana","334"})!=-1) out=-1;
    return(out);
}
REGISTER_TEST(options,tc3){
    int out=0;
    if (testProcess({"test","--inny"})!=-2) out=-1;
    return(out);
}
REGISTER_TEST(options,tc4){
    int out=0;
    out=testProcess({"test","--inny","7","-b","899"});
    return(out);
}
REGISTER_TEST(options,tc5){
    int out=0;
    out=testProcess({"test"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc6){
    int out=0;
    out=testProcess({"test","-b","73737"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=1) out=103;
    if (!out) if (param3.value!=73737) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc7){
    int out=0;
    out=testProcess({"test","--calkiem","-89244"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=1) out=103;
    if (!out) if (param3.value!=-89244) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc8){
    int out=0;
    out=testProcess({"test","--calkiem","-6824","-b","84"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=2) out=103;
    if (!out) if (param3.value!=84) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc9){
    int out=0;
    out=testProcess({"test","-c"});
    if (!out) if (param1.value!=1) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc10){
    int out=0;
    out=testProcess({"test","-c","--ala","-c"});
    if (!out) if (param1.value!=3) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc11){
    int out=0;
    out=testProcess({"test","-i","784.09"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=1) out=107;
    if (!out) if (param5.value.size()!=1) out=108;
    if (!out) if (param5.value.at(0)!=784.09) out=109;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc12){
    int out=0;
    out=testProcess({"test","-i","784.09","--inna","-9853.1e-12"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=2) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=784.09) out=109;
    if (!out) if (param5.value.at(1)!=-9853.1e-12) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc13){
    int out=0;
    out=testProcess({"test","-l","String"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=1) out=116;
    if (!out) if (param9.value!="String") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc14){
    int out=0;
    out=testProcess({"test","-l","String","-l","String2"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=2) out=116;
    if (!out) if (param9.value!="String2") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc15){
    int out=0;
    out=testProcess({"test","-v","test1"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=1) out=114;
    if (!out) if (param8.value!="test1") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
REGISTER_TEST(options,tc16){
    int out=0;
    out=testProcess({"test","-v","test1","inne","nie","parametry"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=1) out=114;
    if (!out) if (param8.value!="test1") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=3) out=118;
    if (!out) if (ict::options::noOptions().at(0)!="inne") out=120;
    if (!out) if (ict::options::noOptions().at(1)!="nie") out=121;
    if (!out) if (ict::options::noOptions().at(2)!="parametry") out=122;
    return(out);
}
REGISTER_TEST(options,tc17){
    int out=0;
    out=testProcess({"test","-ccc","inne","nie","parametry"});
    if (!out) if (param1.value!=3) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=0) out=103;
    if (!out) if (param3.value!=-50) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=3) out=118;
    if (!out) if (ict::options::noOptions().at(0)!="inne") out=120;
    if (!out) if (ict::options::noOptions().at(1)!="nie") out=121;
    if (!out) if (ict::options::noOptions().at(2)!="parametry") out=122;
    return(out);
}
REGISTER_TEST(options,tc18){
    int out=0;
    out=testProcess({"test","--calkiem=7535"});
    if (!out) if (param1.value!=5) out=101;
    if (!out) if (param2.value!=0) out=102;
    if (!out) if (param3.counter!=1) out=103;
    if (!out) if (param3.value!=7535) out=104;
    if (!out) if (param4.counter!=0) out=105;
    if (!out) if (param4.value!=0) out=106;
    if (!out) if (param5.counter!=0) out=107;
    if (!out) if (param5.value.size()!=2) out=108;
    if (!out) if (param5.value.at(0)!=67.6) out=109;
    if (!out) if (param5.value.at(1)!=98) out=110;
    if (!out) if (param6.counter!=0) out=111;
    if (!out) if (param6.value.size()!=0) out=112;
    if (!out) if (param7.value!=0) out=113;
    if (!out) if (param8.counter!=0) out=114;
    if (!out) if (param8.value!="Ala ma kota") out=115;
    if (!out) if (param9.counter!=0) out=116;
    if (!out) if (param9.value!="") out=117;
    if (!out) if (ict::options::noOptions().size()!=0) out=118;
    return(out);
}
#endif
//===========================================