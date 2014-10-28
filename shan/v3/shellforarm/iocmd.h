/*
 * string_list structure could be anything, but it
 * is currently implemented as a singly linked list of
 * character pointers.
 */

struct string_list {
	char *string;
	struct string_list *next;
};

/*
 * io_cmd structures describe the list of I/O manipulations
 * which must be performed in order that the command to be executed 
 * have the proper input ouput environment for its execution.
 * the command describes what must be done; file_name and
 * fd_source describe object which must be implemented.
 * It is expected that there will be one io_cmd for each file
 * descriptor; thus the f.d. can be used as an index to the
 * relevant structure. It might be better to only have filename,
 * and do the analysis of its contents (e.g. &2) later,
 * rather than in the parsing phase. This needs more thought, though.
 */

struct io_cmd {
	int command;	/* what to do; most often NOTHING */
#define IO_NOTHING 0x0
#define IO_READ 0x1
#define IO_WRITE 0x2
#define IO_APPEND 0x4
#define IO_DO_OPEN 0x8
#define IO_DO_DUP 0x10
#define IO_DO_CLOSE 0x20
#define IO_DO_HERE 0x40
	char *file_name;
	int fd_source;
};

#define MAXFILE 10

/*
 * Each command_line structure corresponds to a simple-command
 * described in the grammar for the Bourne Shell (published in the
 * BSTJ, August 1978, pp. 1971-1990
 * The I/O redirection information may be too much; this has to be examined
 * in some more detail.
 */

struct command_line {
  	struct io_cmd IoCmd[MAXFILE];
	struct string_list *ArgList;
	int Background;  /* asynchronous execution flag */
	struct command_line *next;
};


