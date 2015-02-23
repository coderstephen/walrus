#include <stdio.h>
#include "error.h"

/**
 * Exits the program with an error message.
 */
void throw_error(Error code, char* error, int line, int column)
{
    fprintf(stderr, "Error(%d) at line %d, column %d: %s\n", code, error, line, column);
    exit(code);
}
