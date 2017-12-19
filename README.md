# GenSol - Makefile generator
GenSol is a tool that can generate makefile automatically.

## Dependence
GenSol uses rapidjson to parse json and boost::filesystem to enum files, so you need to prepare it before compiling.

## Compile
Use `make task_release` to build release version, and `make task_debug` to build debug.

## Usage
The command format is like this:  
`gensol [options] [inputfile] ...`

currently valid options are:  
"`-h`" : show help  
"`-o FILE`" : indecate the output file name [default: "makefile"]
