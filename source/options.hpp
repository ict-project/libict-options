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
//! Struktura przechowująca opis dla danego języka.
struct description_t{
    //! Wskazanie języka - jeśli puste, to domysłny opis.
    std::string locale;
    //! Opis w danym języku.
    std::string desc;
};
//! Typ przechowujący opisy w różnych językach.
typedef std::vector<description_t> locale_desc_t;
//! Typ przechowujący argumenty funkcji main, które nie zostały rozpozanane jako opcje.
typedef std::vector<std::string> notOption_t;
//! Typ zliczający liczbę wystąpienia danej opcji.
typedef unsigned int counter_t;
//===========================================
//! 
//! @brief Procesuje parametry funkcji main.
//! 
//! @param argc Pierwszy parametr funkcji main.
//! @param argv Drugi parametr funkcji main.
//! @return int Zwraca:
//!   @li 0 - jeśli sukces;
//!   @li -1 - jeśli błąd - nierozpoznana opcja.
//!   @li -2 - jeśli błąd - brak parametru obowiązkowego.
//! 
int process(int argc,char *const argv[]);
//! 
//! @brief Zwraca tekst pomocy.
//! 
//! @param os Strumień wyjściowy do tekstu pomocy.
//! @param locale Wskazanie lokalizacji (język itp.) 
//! 
void help(std::ostream & os,const std::string & locale="");
//! 
//! @brief Udostępnia listę argumentów z funkcji main, które nie są opcjami.
//! 
//! @return const notOption_t& 
//! 
const notOption_t & noOptions();
//===========================================
//! Podstawowy interfejs opcji.
class interface {
private:
    //! Przechowuje listę krótkich nazw opcji.
    std::string shortOption;
    //! Przechowuje listę długich nazw opcji (rozdzielone spacją lub przecinkiem).
    std::string longOption;
    //! Przechowuje domyślną wartość (jeśli jest).
    std::string defaultValue;
    //! Przechowuje opis opcji.
    locale_desc_t description;
    //! 
    //! @brief Rejestruje obiekt do obsługi opcji.
    //! 
    //! @param ptr Wskaźnik do opbiektu obsługującego opcję.
    //! 
    static void registerOpt(interface*ptr);
protected:
    //! 
    //! @brief Zapisuje wartość opcji.
    //! 
    //! @param val Wartość w postaci string do zapisania.
    //! @param target Zmienna, do której ma być podstawiona wartość.
    //! @return Jeśli:
    //!  @li 0 - wartość ustawiona poprawnie;
    //!  @li -1 - wystąpił błąd.
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
    //! @param shortOpt Lista krótkich nazw opcji (pierwsza wolna jest używana).
    //! @param longOpt Lista długich nazw opcji (rozdzielone spacją lub przecinkiem - pierwsza wolna jest używana).
    //! @param defaultVal Domyślna wartość (jeśli jest).
    //! @param defaultDesc Opis opcji (domyślny).
    //! @param localeDesc opis opcji (w róznych językach).
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
    //! @brief Zapisuje wartość opcji.
    //! 
    //! @param val Wartość w postaci string do zapisania.
    //! @return Jeśli:
    //!  @li 0 - wartość ustawiona poprawnie;
    //!  @li -1 - wystąpił błąd.
    //! 
    virtual int setValue(const std::string & val="")=0;
    //! 
    //! @brief Zwraca typ opcji.
    //! 
    //! @return Typ opcji. 
    //! 
    virtual const std::string & getType() const=0;
    //! 
    //! @brief Czyści wartość opcji (i ustawia ewentualnie wartość domyślną).
    //! 
    virtual void clear()=0;
    //! 
    //! @brief Informuje czy wartość tego parametru jest obowiązkowa.
    //! 
    virtual bool isValueRequired()=0;
    //! 
    //! @brief Zwraca listę krótkich nazw opcji.
    //! 
    const std::string & getShortOption() const {return(shortOption);}
    //! 
    //! @brief Zwraca listę długich nazw opcji.
    //! 
    const std::string & getLongOption() const {return(longOption);}
    //! 
    //! @brief Zwraca domyślną wartość (jeśli jest).
    //! 
    const std::string & getDefaultValue() const {return(defaultValue);}
    //! 
    //! @brief Zwraca opis opcji.
    //! 
    const locale_desc_t & getDescription() const {return(description);}
};
//! Obiekt, który tylko zlicza liczbę wystąpień danej opcji.
class counter : public interface{
private:
    //! Informacja, czy ustawiona wartość jest wartością domyślną.
    bool dv=true;
public:
    //! Przechowuje typ obiektu.
    static const std::string type;
    //! Przechowuje wartość obiektu (liczbę wystąpień danej opcji).
    counter_t value=0;
    //! 
    //! @brief Konstruktor.
    //! 
    //! @param shortOpt Lista krótkich nazw opcji (pierwsza wolna jest używana).
    //! @param longOpt Lista długich nazw opcji (rozdzielone spacją lub przecinkiem - pierwsza wolna jest używana).
    //! @param defaultVal Domyślna wartość (jeśli jest).
    //! @param defaultDesc Opis opcji (domyślny).
    //! @param localeDesc opis opcji (w róznych językach).
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
//! Obiekt, który przechowuje pojedynczą wartość opcji (jeśli opcja pojawia się klika razy, to wartość jest nadpisywana).
template <class Type> class single : public interface{
private:
    //! Zawiera informację czy podanie wartości przy tej opcji jest obowiązkowe.
    bool valueRequired;
    //! Wzorzec resetujący wartość (dla podstawowych typów).
    template<class T> typename std::enable_if<std::is_fundamental<T>::value>::type clear(T & v){
        v=0;
    }
    //! Wzorzec resetujący wartość (dla innych niż podstawowe typów).
    template<class T> typename std::enable_if<!std::is_fundamental<T>::value>::type clear(T & v){
        v.clear();
    }
public:
    //! Przechowuje typ obiektu.
    static const std::string type;
    //! Przechowuje liczbę wystąpienia danej opcji.
    counter_t counter=0;
    //! Przechowuje wartość obiektu - ostatnią wartość podaną przy danej opcji.
    Type value;
    //! 
    //! @brief Konstruktor.
    //! 
    //! @param shortOpt Lista krótkich nazw opcji (pierwsza wolna jest używana).
    //! @param longOpt Lista długich nazw opcji (rozdzielone spacją lub przecinkiem - pierwsza wolna jest używana).
    //! @param defaultVal Domyślna wartość (jeśli jest).
    //! @param defaultDesc Opis opcji (domyślny).
    //! @param localeDesc opis opcji (w róznych językach).
    //! @param required Informacja, czy podanie wartości przy tej opcji jest obowiązkowe.
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
//! Obiekt, który przechowuje zestaw wartość opcji (podawanych w kolejnych występieniach opcji).
template <class Type> class vector : public interface{
private:
    //! Informacja, czy ustawiona wartość jest wartością domyślną.
    bool dv=true;
    //! Zawiera informację czy podanie wartości przy tej opcji jest obowiązkowe.
    bool valueRequired;
public:
    //! Przechowuje typ obiektu.
    static const std::string type;
    //! Przechowuje liczbę wystąpienia danej opcji.
    counter_t counter=0;
    //! Przechowuje zestaw wartość obiektu - wszystkie wartości podane przy danej opcji.
    std::vector<Type> value;
    //! 
    //! @brief Konstruktor.
    //! 
    //! @param shortOpt Lista krótkich nazw opcji (pierwsza wolna jest używana).
    //! @param longOpt Lista długich nazw opcji (rozdzielone spacją lub przecinkiem - pierwsza wolna jest używana).
    //! @param defaultVal Domyślna wartość (jeśli jest). Jeśli zestaw wartości, to muszą być rozdzielone znakiem nowej linii ('\n').
    //! @param defaultDesc Opis opcji (domyślny).
    //! @param localeDesc opis opcji (w róznych językach).
    //! @param required Informacja, czy podanie wartości przy tej opcji jest obowiązkowe.
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