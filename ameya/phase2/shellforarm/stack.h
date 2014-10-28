
/*
 * stacks to "fully" expand arguments
 * which have multiple directories.
 * I'm still not sure that this
 * is the way to do things.
 */


struct list {
	char *l_name;
	struct list *l_next;
};

struct stack {
  	int depth;
	struct list *list;
};

extern struct stack Final;
