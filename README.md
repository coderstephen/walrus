# Walrus
A lightweight compiler for the Decaf programming language.

## Compiling the source
You will need a GNU compiler in your `$PATH` installed to compile. [Cygwin](http://cygwin.com) or [MinGW-w64](http://mingw-w64.sourceforge.net) should be sufficient for compiling on Windows machines, as Walrus is POSIX-compatible and uses minimal library functions.

All builds and tests are done using [GNU make](http://www.gnu.org/software/make/). To compile all sources and produce an executable, simply run:

```sh
make
```

That's it. No crazy stuff.

## Running tests
You can run all tests by running:

```sh
make test
```

Sets of input files will be compared to expected output files to find any errors with scanning.

## Usage
To compile a Decaf program, pass the source code files to Walrus:

```sh
bin/walrus inputfile1 inputfile2...
```

To just run the scanner, set the `-s` option. You can also pass the `-T` option to print out the scanned tokens to STDOUT for testing and debugging purposes.
