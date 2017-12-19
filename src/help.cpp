#include <cstdio>
static char helpString[] = 
"Usage: gen [options] [file] ...\n"
"Options:\n"
"    -h --help       Print this message and exit.\n"
"    -o FILE         Output to FILE.";

void help() {
    puts(helpString);
}