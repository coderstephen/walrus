#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "symbol_table.h"


/**
 * Creates a new lexical scope-aware symbol table.
 */
SymbolTable* symbol_table_create()
{
    // allocate space for the symbol table struct
    SymbolTable* table = malloc(sizeof(SymbolTable));
    table->sheaf_tail = NULL;
    table->stack_top = NULL;

    return table;
}

/**
 * Begins a new lexical scope in the symbol table.
 */
Error symbol_table_begin_scope(SymbolTable* table)
{
    // we need a new symbol map for this scope, so allocate one now
    SymbolMap* map = malloc(sizeof(SymbolMap));
    // create the array for the hashtable
    map->entries = malloc(sizeof(SymbolEntry*) * SYMBOL_MAP_SIZE);
    // ugh, we also need to zero-initialize the hashtable to avoid confusion later
    memset(map->entries, 0, sizeof(SymbolEntry*) * SYMBOL_MAP_SIZE);

    // now, add the new map to the sheaf
    map->previous = table->sheaf_tail;
    table->sheaf_tail = map;

    // also, push the new scope onto the scope stack
    ScopeStackNode* old_top = table->stack_top;
    table->stack_top = malloc(sizeof(ScopeStackNode));
    table->stack_top->previous = old_top;
    table->stack_top->next = NULL;
    table->stack_top->map = map;
    if (old_top != NULL) {
        old_top->next = table->stack_top;
    }

    return E_SUCCESS;
}

/**
 * Closes a previously opened lexical scope.
 */
Error symbol_table_end_scope(SymbolTable* table)
{
    if (table->stack_top == NULL) {
        return error(E_BAD_POINTER, "No symbol table scope created to end.");
    }

    // pop the current scope off the stack
    ScopeStackNode* old_top = table->stack_top;
    table->stack_top = table->stack_top->previous;
    if (table->stack_top != NULL) {
        table->stack_top->next = NULL;
    }

    // destroy the popped stack node
    free(old_top);

    return E_SUCCESS;
}

/**
 * Looks up a symbol in the symbol table.
 *
 * Looks for the symbol in each of the symbol maps in the sheaf, working down
 * the scope stack. Yeah, not terribly efficient, is it? Just look at those nasty
 * nested for loops. Worst case is pretty bad, but average case isn't too shabby.
 */
SymbolEntry* symbol_table_lookup(SymbolTable* table, char* symbol)
{
    // get the symbol hash first
    unsigned int hash = symbol_hash(symbol);

    // loop over each map in the stack
    for (ScopeStackNode* scope = table->stack_top; scope != NULL; scope = scope->previous) {
        // go to the hash position in the symbol map and loop over each entry at
        // the hash's location
        for (SymbolEntry* entry = scope->map->entries[hash]; entry != NULL; entry = entry->next) {
            // check if the current entry matches the symbol we are looking for
            if (strcmp(entry->symbol, symbol) == 0) {
                // we finally found it!
                return entry;
            }
        }
    }

    // symbol not found
    return NULL;
}

/**
 * Inserts a symbol into the symbol table.
 */
Error symbol_table_insert(SymbolTable* table, char* symbol)
{
    if (table->stack_top == NULL) {
        return error(E_BAD_POINTER, "No symbol table scope to insert into.");
    }

    // create a symbol entry for the given symbol
    SymbolEntry* entry = malloc(sizeof(SymbolEntry));
    entry->symbol = symbol;
    entry->next = NULL;

    // get the hash for the symbol
    unsigned int hash = symbol_hash(symbol);

    // insert the entry into the hashtable, into the start of the "bucket"
    entry->next = table->stack_top->map->entries[hash];
    table->stack_top->map->entries[hash] = entry;

    return E_SUCCESS;
}

/**
 * Destroys a symbol table and all of its contents.
 */
Error symbol_table_destroy(SymbolTable** table)
{
    if (table == NULL || *table == NULL) {
        return E_BAD_POINTER;
    }

    // free any remaining scope stack items
    ScopeStackNode* previous_node = (*table)->stack_top;
    while (previous_node != NULL) {
        ScopeStackNode* current = previous_node;
        previous_node = current->previous;
        free(current);
    }

    // free all symbol maps
    SymbolMap* previous_map = (*table)->sheaf_tail;
    while (previous_map != NULL) {
        SymbolMap* current = previous_map;
        previous_map = current->previous;
        free(current);
    }

    // free the table
    free(*table);
    *table = NULL;

    return E_SUCCESS;
}

/**
 * Computes the hash of a symbol.
 *
 * Just a real simple hashing algorithm.
 */
unsigned int symbol_hash(char* symbol)
{
    unsigned int hashval;
    for (hashval = 0; *symbol != '\0'; symbol++) {
        hashval = *symbol + 31 * hashval;
    }
    return hashval % SYMBOL_MAP_SIZE;
}
