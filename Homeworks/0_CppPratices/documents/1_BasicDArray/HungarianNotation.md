# 匈牙利命名法

Charles Simonyi is credited with first discussing Hungarian Notation. It is a variable naming convention that includes C++ information about the variable in its name (such as data type, whether it is a reference variable or a constant variable, etc). Every company and programmer seems to have their own flavor of Hungarian Notation. The following is just what we thought seemed easy for beginning students to understand.

| Prefix                | Type                                 | Example                                                      |
| --------------------- | ------------------------------------ | ------------------------------------------------------------ |
| `b`                   | boolean                              | `bool bStillGoing;`                                          |
| `c`                   | character                            | `char cLetterGrade;`                                         |
| `str`                 | C++ String                           | `string strFirstName;`                                       |
| `si`                  | short integer                        | `short siChairs;`                                            |
| `i`                   | integer                              | `int iCars;`                                                 |
| `li`                  | long integer                         | `long liStars;`                                              |
| `f`                   | floating point                       | `float fPercent;`                                            |
| `d`                   | double-precision floating point      | `double dMiles;`                                             |
| `ld`                  | long double-precision floating point | `long double ldLightYears;`                                  |
| `sz`                  | Old-Style Null Terminated String     | `char szName[NAME_LEN];`                                     |
| `if`                  | Input File Stream                    | `ifstream ifNameFile;`                                       |
| `is`                  | Input Stream                         | `void fct(istream &risIn);`                                  |
| `of`                  | Output File Stream                   | `ofstream ofNameFile;`                                       |
| `os`                  | Output Stream                        | `void fct(ostream &rosIn);`                                  |
| `S`                   | declaring a struct                   | `struct SPoint {`                                            |
| `C`                   | declaring a class                    | `class CPerson {`                                            |
| struct name or abbrev | declaring an instance of a struct    | `SPoint pointLeft;`</br>`SPoint ptLeft; // or abbrev. (be consistent)` |
| class name or abbrev  | declaring an instance of a class     | `CPerson personFound;`</br>`CPerson perFound; // or abbrev. (be consistent)` |

**The following table contains letters that go before the above prefixes.** 

| Pre-prefix | Type                                 | Example                            |
| ---------- | ------------------------------------ | ---------------------------------- |
| `u`        | unsigned                             | `unsigned short usiStudents;`      |
| `k`        | constant formal parameter            | `void fct(const long kliGalaxies)` |
| `r`        | reference formal parameter           | `void fct(long &rliGalaxies)`      |
| `s`        | static                               | `static char scChoice;`            |
| `rg`       | array (stands for range)             | `float rgfTemp[MAX_TEMP];`         |
| `m_`       | member variable of a struct or class | `char m_cLetterGrade;`             |
| `p`        | pointer to a single thing            | `char *pcGrade;`                   |
| `prg`      | dynamically allocated array          | `char *prgcGrades;`                |

