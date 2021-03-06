//! @file
//! @brief Options module - header file.
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
#ifndef _ICT_OPTIONS_HEADER
#define _ICT_OPTIONS_HEADER
//============================================
#include <string>
#include <vector>
#include <sstream>
#include <type_traits>
//============================================
namespace ict { namespace  options { 
//===========================================
//! Struktura przechowuj??ca opis dla danego j??zyka.
struct description_t{
    //! Wskazanie j??zyka - je??li puste, to domys??ny opis.
    std::string locale;
    //! Opis w danym j??zyku.
    std::string desc;
};
//! Typ przechowuj??cy opisy w r????nych j??zykach.
typedef std::vector<description_t> locale_desc_t;
//! Typ przechowuj??cy argumenty funkcji main, kt??re nie zosta??y rozpozanane jako opcje.
typedef std::vector<std::string> notOption_t;
//! Typ zliczaj??cy liczb?? wyst??pienia danej opcji.
typedef unsigned int counter_t;
//===========================================
//! 
//! @brief Procesuje parametry funkcji main.
//! 
//! @param argc Pierwszy parametr funkcji main.
//! @param argv Drugi parametr funkcji main.
//! @return int Zwraca:
//!   @li 0 - je??li sukces;
//!   @li -1 - je??li b????d - nierozpoznana opcja.
//!   @li -2 - je??li b????d - brak parametru obowi??zkowego.
//! 
int process(int argc,char *const argv[]);
//! 
//! @brief Zwraca tekst pomocy.
//! 
//! @param os Strumie?? wyj??ciowy do tekstu pomocy.
//! @param locale Wskazanie lokalizacji (j??zyk itp.) 
//! 
void help(std::ostream & os,const std::string & locale="");
//! 
//! @brief Udost??pnia list?? argument??w z funkcji main, kt??re nie s?? opcjami.
//! 
//! @return const notOption_t& 
//! 
const notOption_t & noOptions();
//===========================================
//! Podstawowy interfejs opcji.
class interface {
private:
    //! Przechowuje list?? kr??tkich nazw opcji.
    std::string shortOption;
    //! Przechowuje list?? d??ugich nazw opcji (rozdzielone spacj?? lub przecinkiem).
    std::string longOption;
    //! Przechowuje domy??ln?? warto???? (je??li jest).
    std::string defaultValue;
    //! Przechowuje opis opcji.
    locale_desc_t description;
    //! 
    //! @brief Rejestruje obiekt do obs??ugi opcji.
    //! 
    //! @param ptr Wska??nik do opbiektu obs??uguj??cego opcj??.
    //! 
    static void registerOpt(interface*ptr);
protected:
    //! 
    //! @brief Zapisuje warto???? opcji.
    //! 
    //! @param val Warto???? w postaci string do zapisania.
    //! @param target Zmienna, do kt??rej ma by?? podstawiona warto????.
    //! @return Je??li:
    //!  @li 0 - warto???? ustawiona poprawnie;
    //!  @li -1 - wyst??pi?? b????d.
    //! 
    template <class Value> int setValue(const std::string & val,Value & target){
        try{
            std::istringstream s(val);
            s>>target;
        } catch (...){
            return(-1);
        }
        return(0);
    }
    int setValue(const std::string & val,std::string & target){
        target.assign(val);
        return(0);
    }
    //! 
    //! @brief Inicjuje parametr.
    //! 
    //! @param shortOpt Lista kr??tkich nazw opcji (pierwsza wolna jest u??ywana).
    //! @param longOpt Lista d??ugich nazw opcji (rozdzielone spacj?? lub przecinkiem - pierwsza wolna jest u??ywana).
    //! @param defaultVal Domy??lna warto???? (je??li jest).
    //! @param defaultDesc Opis opcji (domy??lny).
    //! @param localeDesc opis opcji (w r??znych j??zykach).
    //! 
    void init(
        const std::string & shortOpt="",
        const std::string & longOpt="",
        const std::string & defaultVal="",
        const std::string & defaultDesc="",
        const locale_desc_t & localeDesc={}
    );
public:
    //! 
    //! @brief Zapisuje warto???? opcji.
    //! 
    //! @param val Warto???? w postaci string do zapisania.
    //! @return Je??li:
    //!  @li 0 - warto???? ustawiona poprawnie;
    //!  @li -1 - wyst??pi?? b????d.
    //! 
    virtual int setValue(const std::string & val="")=0;
    //! 
    //! @brief Zwraca typ opcji.
    //! 
    //! @return Typ opcji. 
    //! 
    virtual const std::string & getType() const=0;
    //! 
    //! @brief Czy??ci warto???? opcji (i ustawia ewentualnie warto???? domy??ln??).
    //! 
    virtual void clear()=0;
    //! 
    //! @brief Informuje czy warto???? tego parametru jest obowi??zkowa.
    //! 
    virtual bool isValueRequired()=0;
    //! 
    //! @brief Zwraca list?? kr??tkich nazw opcji.
    //! 
    const std::string & getShortOption() const {return(shortOption);}
    //! 
    //! @brief Zwraca list?? d??ugich nazw opcji.
    //! 
    const std::string & getLongOption() const {return(longOption);}
    //! 
    //! @brief Zwraca domy??ln?? warto???? (je??li jest).
    //! 
    const std::string & getDefaultValue() const {return(defaultValue);}
    //! 
    //! @brief Zwraca opis opcji.
    //! 
    const locale_desc_t & getDescription() const {return(description);}
};
//! Obiekt, kt??ry tylko zlicza liczb?? wyst??pie?? danej opcji.
class counter : public interface{
private:
    //! Informacja, czy ustawiona warto???? jest warto??ci?? domy??ln??.
    bool dv=true;
public:
    //! Przechowuje typ obiektu.
    static const std::string type;
    //! Przechowuje warto???? obiektu (liczb?? wyst??pie?? danej opcji).
    counter_t value=0;
    //! 
    //! @brief Konstruktor.
    //! 
    //! @param shortOpt Lista kr??tkich nazw opcji (pierwsza wolna jest u??ywana).
    //! @param longOpt Lista d??ugich nazw opcji (rozdzielone spacj?? lub przecinkiem - pierwsza wolna jest u??ywana).
    //! @param defaultVal Domy??lna warto???? (je??li jest).
    //! @param defaultDesc Opis opcji (domy??lny).
    //! @param localeDesc opis opcji (w r??znych j??zykach).
    //! 
    counter(
        const std::string & shortOpt="",
        const std::string & longOpt="",
        const std::string & defaultVal="",
        const std::string & defaultDesc="",
        const locale_desc_t & localeDesc={}
    ){
        init(shortOpt,longOpt,defaultVal,defaultDesc,localeDesc);
        //clear();
    }
    //! Patrz ict::options::interface::setValue()
    int setValue(const std::string & val=""){
        if (dv){
            value=0;
            dv=false;    
        }
        value++;
        return(0);
    }
    //! Patrz ict::options::interface::getType()
    const std::string & getType() const {return type;}
    //! Patrz ict::options::interface::clear()
    void clear(){
        if (!getDefaultValue().empty()){
            interface::setValue(getDefaultValue(),value);
        } else {
            value=0;
        }
        dv=true;
    }
    //! Patrz ict::options::interface::isValueRequired()
    bool isValueRequired(){
        return false;
    }
};
inline const std::string counter::type("counter");
//! Obiekt, kt??ry przechowuje pojedyncz?? warto???? opcji (je??li opcja pojawia si?? klika razy, to warto???? jest nadpisywana).
template <class Type> class single : public interface{
private:
    //! Zawiera informacj?? czy podanie warto??ci przy tej opcji jest obowi??zkowe.
    bool valueRequired;
    //! Wzorzec resetuj??cy warto???? (dla podstawowych typ??w).
    template<class T> typename std::enable_if<std::is_fundamental<T>::value>::type clear(T & v){
        v=0;
    }
    //! Wzorzec resetuj??cy warto???? (dla innych ni?? podstawowe typ??w).
    template<class T> typename std::enable_if<!std::is_fundamental<T>::value>::type clear(T & v){
        v.clear();
    }
public:
    //! Przechowuje typ obiektu.
    static const std::string type;
    //! Przechowuje liczb?? wyst??pienia danej opcji.
    counter_t counter=0;
    //! Przechowuje warto???? obiektu - ostatni?? warto???? podan?? przy danej opcji.
    Type value;
    //! 
    //! @brief Konstruktor.
    //! 
    //! @param shortOpt Lista kr??tkich nazw opcji (pierwsza wolna jest u??ywana).
    //! @param longOpt Lista d??ugich nazw opcji (rozdzielone spacj?? lub przecinkiem - pierwsza wolna jest u??ywana).
    //! @param defaultVal Domy??lna warto???? (je??li jest).
    //! @param defaultDesc Opis opcji (domy??lny).
    //! @param localeDesc opis opcji (w r??znych j??zykach).
    //! @param required Informacja, czy podanie warto??ci przy tej opcji jest obowi??zkowe.
    //! 
    single(
        const std::string & shortOpt="",
        const std::string & longOpt="",
        const std::string & defaultVal="",
        const std::string & defaultDesc="",
        const locale_desc_t & localeDesc={},
        bool required=true
    ):valueRequired(required){
        init(shortOpt,longOpt,defaultVal,defaultDesc,localeDesc);
        //clear();
    }
    //! Patrz ict::options::interface::setValue()
    int setValue(const std::string & val=""){
        counter++;
        if (!val.empty()) {
            if (interface::setValue(val,value)) return(-1);
        }
        return(0);
    }
    //! Patrz ict::options::interface::getType()
    const std::string & getType() const {return type;}
    //! Patrz ict::options::interface::clear()
    void clear(){
        counter=0;
        clear(value);
        if (!getDefaultValue().empty()){
            interface::setValue(getDefaultValue(),value);
        }
    }
    //! Patrz ict::options::interface::isValueRequired()
    bool isValueRequired(){
        return valueRequired;
    }
};
template <class Type> const std::string single<Type>::type("single");
//! Obiekt, kt??ry przechowuje zestaw warto???? opcji (podawanych w kolejnych wyst??pieniach opcji).
template <class Type> class vector : public interface{
private:
    //! Informacja, czy ustawiona warto???? jest warto??ci?? domy??ln??.
    bool dv=true;
    //! Zawiera informacj?? czy podanie warto??ci przy tej opcji jest obowi??zkowe.
    bool valueRequired;
public:
    //! Przechowuje typ obiektu.
    static const std::string type;
    //! Przechowuje liczb?? wyst??pienia danej opcji.
    counter_t counter=0;
    //! Przechowuje zestaw warto???? obiektu - wszystkie warto??ci podane przy danej opcji.
    std::vector<Type> value;
    //! 
    //! @brief Konstruktor.
    //! 
    //! @param shortOpt Lista kr??tkich nazw opcji (pierwsza wolna jest u??ywana).
    //! @param longOpt Lista d??ugich nazw opcji (rozdzielone spacj?? lub przecinkiem - pierwsza wolna jest u??ywana).
    //! @param defaultVal Domy??lna warto???? (je??li jest). Je??li zestaw warto??ci, to musz?? by?? rozdzielone znakiem nowej linii ('\n').
    //! @param defaultDesc Opis opcji (domy??lny).
    //! @param localeDesc opis opcji (w r??znych j??zykach).
    //! @param required Informacja, czy podanie warto??ci przy tej opcji jest obowi??zkowe.
    //! 
    vector(
        const std::string & shortOpt="",
        const std::string & longOpt="",
        const std::string & defaultVal="",
        const std::string & defaultDesc="",
        const locale_desc_t & localeDesc={},
        bool required=true
    ):valueRequired(required){
        init(shortOpt,longOpt,defaultVal,defaultDesc,localeDesc);
        //clear();
    }
    //! Patrz ict::options::interface::setValue()
    int setValue(const std::string & val=""){
        counter++;
        if (dv){
            value.clear();
            dv=false;
        }
        if (!val.empty()) {
            Type v;
            if (interface::setValue(val,v)) return(-1);
            value.emplace_back(v);
        }
        return(0);
    }
    //! Patrz ict::options::interface::getType()
    const std::string & getType() const {return type;}
    //! Patrz ict::options::interface::clear()
    void clear(){
        std::string val;
        std::istringstream s(getDefaultValue());
        counter=0;
        value.clear();
        while (std::getline(s,val,'\n')){
            Type v;
            interface::setValue(val,v);
            value.emplace_back(v);
        }
        dv=true;
    }
    //! Patrz ict::options::interface::isValueRequired()
    bool isValueRequired(){
        return valueRequired;
    }
};
template <class Type> const std::string vector<Type>::type("vector");
//===========================================
} }
//============================================
#endif