# Basic option definition

The main idea of this library is to give a possibility to define an option, which value is parsed from command line arguments, in any file of the source code (in global scope).

Three main templates may be used for an option defintion:
* `ict::options::counter` - the simple counter that holds number of occurrences for given option;
* `ict::options::single<Type>` - the single value of given type (`Type`) for given option;
* `ict::options::vector<Type>` - the set of values (`std::vector`) of given type (`Type`) for given option.

Defining an option following params should be provided:
* `shortOpt` List of short option names (the first not taken one is used);
* `longOpt`  List of long option names (comma separated - the first not taken one is used);
* `defaultVal` Default value of the option (if exists);
* `defaultDesc` Default description of the option;
* `localeDesc` Option description for different locales.

## Defining an option
### Number of occurrences of given option

Example:
```c
#include "options.hpp"
ict::options::counter param1("v","verbose","0","Description.");//Option definition.
param1.value;//Holds number of occurrences of '-v' option (short version) and '--verbose' option (long version) in command line arguments. 
```

### Single value provided with given option

Example:
```c
#include "options.hpp"
ict::options::single<int> param2("t","timeout","100","Description.");
param2.value;//Holds value provided with '-t' option (short version) or '--timeout' option (long version) in command line arguments. If not provided then value is 100. If provided more than once - the last value.
param2.counter;//Holds number of occurrences of '-t' option (short version) and '--timeout' option (long version) in command line arguments.
```

### Set of values provided with given option

Example:
```c
#include "options.hpp"
ict::options::vector<std::string> param3("f","file","","Description.");
param3.value;//Holds values (`std::vector`) provided with '-f' option (short version) or '--file' option (long version) in command line arguments.
param3.counter;//Holds number of occurrences of '-f' option (short version) and '--file' option (long version) in command line arguments.
```

## Parsing command line arguments

Options may be defined in any file of the source code but in order to parse command line arguments and set values of all options special function should be called in main function: `ict::options::process(argc,argv);`.

In case of a need to present description of all options following function should be used: `ict::options::help(ostream,locale)`.

Command line arguments that was not recognized as options can be found here (as `std::vector`): `ict::options::noOptions()`.