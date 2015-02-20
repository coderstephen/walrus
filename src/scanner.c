#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

/**
 * Opens a file in read-only mode and creates a scanner context for it.
 */
ScannerContext* scanner_open(char* filename)
{
    // open the given file
    FILE* stream = fopen(filename, "r");
    if (stream == NULL) {
        return NULL;
    }

    // create a context pointer
    ScannerContext* context = (ScannerContext*)malloc(sizeof(ScannerContext));
    context->line = 0;
    context->column = 0;
    context->stream = stream;

    return context;
}

/**
 * Opens a string as a stream and creates a scanner context for it.
 */
ScannerContext* scanner_open_string(char* string)
{
    // create a stream for the char array
    FILE* stream = fmemopen(string, strlen(string), "r");
    if (stream == NULL) {
        return NULL;
    }

    // create a context pointer
    ScannerContext* context = (ScannerContext*)malloc(sizeof(ScannerContext));
    context->line = 0;
    context->column = 0;
    context->stream = stream;

    return context;
}

/**
 * Gets the next character in the stream.
 *
 * Does not work on multibyte characters.
 */
char scanner_next(ScannerContext* context)
{
    char character = fgetc(context->stream);

    // new line reached
    if (character == '\n') {
        context->line++;
        context->column = 0;
    } else {
        context->column++;
    }

    return character;
}

/**
 * Reads a character from a scanner context relative to the current position.
 */
char scanner_peek(ScannerContext* context, long int offset)
{
    // move to offset
    fseek(context->stream, offset, SEEK_CUR);

    // get the character
    char character = fgetc(context->stream);

    // return to previous position
    fseek(context->stream, 1 - offset, SEEK_CUR);

    return character;
}

/**
 * Gets the string of characters between a given offset and the current position.
 */
char* scanner_get_string(ScannerContext* context, long int offset)
{
    assert(offset != 0);

    // where are we?
    long int position = ftell(context->stream);
    // how many chars we will get
    long int length = labs(offset);
    // allocate the string
    char* string = malloc(length + 1);

    // if offset is negative, seek backwards
    if (offset < 0) {
        fseek(context->stream, offset, SEEK_CUR);
    }

    // and append current char
    for (int i = 0; i < length; ++i) {
        string[i] = fgetc(context->stream);
    }
    // add string terminator
    string[length] = '\0';

    // move back to original position
    fseek(context->stream, position, SEEK_SET);
    return string;
}

/**
 * Closes a scanner context and its associated input stream.
 */
Error scanner_close(ScannerContext* context)
{
    // context shouldn't be null
    assert(context != NULL);

    // close the stream handle
    fclose(context->stream);

    // free the pointer
    free(context);

    return E_SUCCESS;
}
