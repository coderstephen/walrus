# Semantic Analyzer Report

## QUESTION 1
*How to build your project from the source code. If you use make or ant, tell me. If not, you must include the full command(s) you used to build your compiler. (Remember, if I can’t figure out how to compile your project, your group gets a Fix & Resubmit grade for the project.)*

In order to build our project from source, ensure that you have a GNU C compiler installed, navigate to the root directory of the project, and type 'make' in the command line.  To run the test cases, type 'make test'.


## QUESTION 2
*A brief description of how your group divided the work. Summarize each group member’s contribution to the project.*

Once again, we didn't have a formal system of dividing up the work.  We just did what we could when we were available.

- Stephen: Contributed heavily in implementation of the abstract syntax tree and symbol table
- Tyler: Wrote report, contributed to the semantic analysis step
- Trevor:  Contributed to the creation of the symbol table and semantic analysis process, and provided testing results
- Alex: Contributed to the creation and architecture of the symbol table and the semantic analysis process


## QUESTION 3
*A list of any clarifications, assumptions, or additions to the problem assigned. The project specifications are fairly broad and leave many of the decisions to you. This is an aspect of real software engineering. If you think major clarifications are necessary, please ask the instructor.*

We were able to derive all information we needed to build the parser from the project specification, and therefore have no clarifications or assumptions that need to stated.


## QUESTION 4
*An overview of your design, an analysis of design alternatives you considered (if any), and key design decisions. Be sure to document and justify all design decisions you make. Any decision accompanied by a convincing argument will be accepted. If you realize there are flaws or deficiencies in your design late in the implementation process, discuss those flaws and how you would have done things differently. Also include any changes you made to previous parts and why they were necessary.*

Once the scanner has properly generated up the tokens, they are fed to the parser.  We have created methods to handle each grammar rule in the Decaf language.  To kick off the entire process, we begin by parsing the 'Program' grammar rule.  Since this is a recursive descent parser, this starts the whole chain of method calls until the end of the file is reached. If it encounters an error based on the rules of the grammar we have encoded into our methods, an error message with relevant information is displayed.

As the parser is going through the recursive descent through the program, it adds nodes to the abstract syntax tree when certain groups of nodes have been constructed.  For example, `int foo[100];` will become a single node in the AST when in reality that snippet of code was run through several parsing methods. This was a key goal we wanted to achieve; to make the AST much simpler to work with than a parse tree.

The next step in the process is the analysis of the AST we just constructed using the symbol table.  In our recursive analysis of the AST, our analyzer has several tasks.  First, whenever we encounter a variable or method declaration not already in the symbol table, we add it and other relevant information such as data type and scope.  In that same vein, if we encounter a variable or method call that already exists, we make sure it was previously declared.  If it was previously declared, we add the return information to the tree at that spot - if it wasn't previously declared, raise an error.  Another task of the analyzer is to check for type conflicts generated by expressions.  If there is a mismatch, we raise an error.  Next, the analyzer ensures that data types are the same on both sides of the assignment operator.  The analyzer also checks to make sure that expressions in "if" and "for" are actually boolean.  Finally, our analyzer converts the "-" operators into negative int literals if the operand itself is an int literal.

Our symbol table is implemented as a "sheaf" of hashtables, accompanied by a stack of scope information pointing to a table in the sheaf.  We decided to implement it using this design because it was relatively easy to implement in C. While memory consumption could be an issue for a program with many scopes, we felt that speed was a more important concern; thus, lookups are very fast in our design. Our design also does not support symbol deletion, but maintains all symbols within their respective scopes, though the scope hierarchy is lost at the end of the analysis. This may only be partially useful for debugging, but we did not have time to change it.

In addition, note that our compiler does not support rule 19 of the Decaf language. We decided to not implement this rule due to how parenthesis are parsed by our compiler; they are essentially discarded and only used for grouping during parsing.


## QUESTION 5
*A brief description of interesting implementation issues. This should include any non-trivial algorithms, techniques, and data structures. It should also include any insights you discovered during this phase of the project. (External sources of implementation help, e.g., websites or books, should be cited here.)*

One interesting implementation issue was the C standard library doesn't come with a hashmap implementation, something that is pretty necessary when creating the symbol table.  Thus, we created our own hashmap implementation, which ultimately morphed into one with the symbol table implementation.

Another interesting issue was how to store different AST node types. Since C does not have classes in a traditional sense (which really maps well to the AST problem), we implemented a few struct types for only a few kinds of nodes that had different information, all inheriting from a base struct type using a struct memory layout trick. We then used a form of bitmasking on our enum of node "kinds" to help the program determine which struct type to allocate.

Stephen actually got the idea from the PHP interpreter's implementation of an AST (can be seen [here](https://github.com/php/php-src/blob/master/Zend/zend_ast.h)), which uses many similar techniques.


## QUESTION 6
*A list of known problems with your project, and as much as you know about the cause.*

- If your project fails a provided test case, but you are unable to fix the problem, describe your understanding of the problem.
- If you discover problems in your project in your own testing that you are unable to fix, but are not exposed by the provided test cases, describe the problem as specifically as possible and as much as you can about its cause. If this causes your project to fail hidden test cases, you may still be able to receive some credit for considering the problem. If this problem is not revealed by the hidden test cases, then you will not be penalized for it.

As far as we are aware there are no issues with any aspects of our semantic analyzer, and all the provided test cases run without error.

However, our compiler does not write its debugging information to a file when `--debug` is given as an option. It is printed to the console instead; originally planned to just be a temporary solution.