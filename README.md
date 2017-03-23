# terminal-programs
Various small terminal applications for things I do often such as quick statistical analysis on tabular data.

## Executables

### `stat_table`

```
Overview: Tool for computing various statistical properties of data stored in a table format.

Usage: stat_table <options> [input file]

Parse options:
  --help                     produce this help message
  -s [ --skip ] arg (=0)     skip the first n rows of the input file 
                             [default=0]
  -d [ --delta ] arg (=1)    only include every n'th row of the input file 
                             [default=1]
  -c [ --comments ] arg (=#) list of comment symbols [default="#"]
  -h [ --human ]             human readable output
  --col arg                  which columns to parse, will parse all of this 
                             option is unspecified
```

## Libraries

### [irubataru_io.hpp](include/irubataru_io.hpp)

`Table_Reader<Type>`

A header only class that reads data stored in a tabular format and returns the data as a `std::vector<std::vector<Type>>`.
The class has various customisations, such as the ability to specify comment characters, which columns to read, ready every
nth line, skip the first n lines, and so on.

### [irubataru_math.hpp](include/irubataru_math.hpp)

Header only library that includes various mathematical (mostly statistical) functions such as the mean of a list or the standard
deviation.
