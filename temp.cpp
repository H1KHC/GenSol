# include <stdio.h>
#define ATTR(attr)	"\033[" attr "m"
#define AND			";"
#define RESET		"0"
#define BOLD		"1"
#define UNDERLINE	"4"
#define BLINK		"5"
#define REV			"7"
#define CLR			"8"
#define RED			"31"
#define GREEN		"32"
#define YELLOW		"33"
#define BLUE		"34"
#define MAGENTA		"35"
#define CYAN		"36"
#define GREY		"37"
#define BRED		"41"
#define BGREEN		"42"
#define BYELLOW		"43"
#define BBLUE		"44"
#define BMAGENTA	"45"
#define BCYAN		"46"
#define BGREY		"47"
int main() {
	printf(ATTR(CLR AND BRED) "AA\033[32mBB" ATTR(RESET) "\n");
	return( 0 );
}