#  Suboption parsing

## Example

```c
std::string listOfNames("ala,ma,kota");//List of names of suboptions. 
ict::options::single<ict::options::suboptions<listOfNames>> param4("s","subparam","","Description.");
param4.value;//Holds value provided with '-s' option (short version) or '--subparam' option (long version) in command line arguments. Value is a `std::map<std::string,std::string>` type where suboption names are mapped to suboption values (if provided - see 'man getsubopt').
param4.counter;//Holds number of occurrences of '-s' option (short version) and '--subparam' option (long version) in command line arguments.
```
