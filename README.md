# GenSol - Makefile generator
GenSol is a tool that can generate makefile automatically.

There is a similar tool built using python [here](https://github.com/neko-para/gensol).

## Dependence
GenSol uses rapidjson to parse json and boost to enum files and parse options, so you need to prepare them before compiling.

## Compile
Use `make [task_release]` to build release version, and `make task_debug` to build debug.

## Usage
This is a command line program, so is run in consoles
gensol [options] [input] ...
  -d [ --depth   ] arg (=2)        Max tracing depth
  -h [ --help    ]                 Show this message and exit
  -o [ --output  ] arg (=makefile) Output file name
  -v [ --verbose ]                 Show more log