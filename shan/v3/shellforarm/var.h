/* var.h
 * definitions of structure used to implement variables in the
 * IO program.
 */


struct var {
	char *v_name;
	char *v_value;
	int v_flag;
#define V_LOCAL 0
#define V_READONLY 1
#define V_EXPORTED 2
	struct var *v_next;
};

#define VHASHSIZE 128

