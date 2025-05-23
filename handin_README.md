# Jabukód compiler README

Martin Jabůrek <br/>
xjabur02@stud.fit.vutbr.cz

## Bachelor Thesis Project
## Compiler generating the target code secured against reverse analysis

### Abstract

This thesis focuses on designing a new programming language and the implementation of
its compiler. Besides that, it deals with creating and studying commonly used and new
methods of securing generated code against reverse analysis. Both the widespread and
the new protections are implemented directly as parts of the compiler. Another part of
the new language are unusual program structures, which not only better its usability for
programming but also offer further options for obfuscating generated code. The result is
a functioning language compiler and its study, which shows and evaluates the effects of
implemented methods on code security and their influence on programs during runtime.
The resulting compiler and language can be used and further developed for the purposes of
generating secured executables without the need for additional processing afterwards.

## Project structure

Essential top-level project directories and files are arranged as follows:

```
BP_Jabukod/
+-- docs/                          - Contains Doxyfile and generated documentation files
|   +-- images/
|   \-- Doxyfile
+-- grammar/                       - The Jabukód language grammar
|   \-- Jabukod.g4
+-- profiling/                     - Simple scripts for profiling generated binaries, contains
|   +-- C/                           examples of measurements used for thesis text
|   +-- jabukod/
|   +-- measurements_1/
|   +-- measurements_2/
|   +-- jabukod_vs_c.sh
|   \-- obfuscations.sh
+-- project/                       - The source code of the Jabukód compiler
|   +-- cmake/
|   +-- include/
|   +-- src/
|   +-- CMakeLists.txt
|   +-- common.h
|   \-- main.cpp
+-- samples/                       - Simple algorithms and programs showcasing Jabukód usage
+-- test_programs/                 - Programs used for testing the compiler
+-- tool/                          - ANTLR4 jar tool
|   \-- antlr-4.13.2-complete.jar 
+-- README.md
+-- LICENSE.txt
+-- Guide_CZ.pdf
+-- Guide_EN.pdf
\-- assignment.pdf
```

## Requirements

- Minimum C++ standard <b>C++17</b>
- Minimum CMake version <b>2.8</b>
- `java` installed (needed for `make visualise`, tested with `openjdk 21.0.6 2025-01-21`)
- `javac` installed ( -//- )
- `dot` installed ( -//- )
- `doxygen` installed (needed for `make docs` and `make clean_docs`)

## Usage

### Build

Opening the project for the first time in an IDE will likely result in many errors appearing.
These are dependency errors from files to be generated during the build.

The following is the recommended command for <b>building</b> the program:

`cmake -B build -S project && make -C build`<br>

To speed up the build process, build in parallel with `-j$(nproc)`:

`cmake -B build -S project && make -j$(nproc) -C build`<br>

The resulting <b>executable</b> `jabukod` will be located in the `build` subdirectory.
<b>
As the build process needs to download and build the ANTLR4 runtime, initial
build may take a long time.
</b>
</br>
</br>
Also, due to several deprecation warnings shown during the build of the ANTLR4 runtime,
the build is not processed without warnings. For this compiler they pose no
limitation to intended functionality.

### Running

To run the generated compiler, do:

`./jabukod [-a] [-c] [-d] [-D] [-g] [-h] [-o path_to_binary] [-O selected_obfuscations] path_to_program`

<b>Command line arguments</b> can be used generally, with extra non-option arguments ignored.
`[]` mark voluntary arguments. The semantics of each of the arguments above are:

- `-a` - only execute source program <b>a</b>nalysis, do not compile
- `-c` - embed <b>c</b>lock cycle measurement with `rdtsc` into the generated binary
- `-d` - generate executable with <b>d</b>ebug info
- `-D` - compile and <b>D</b>ebug generated binary
- `-g` - print <b>g</b>raphical representation of compilation
- `-h` - print <b>h</b>elp message
- `-o` - explicitly specified <b>o</b>utput file and directory (default is `out.s`)
- `-O` - compile with <b>O</b>bfuscations; mandatory following argument determines which are done
- `path_to_program` - mandatory input file

It is recommended to not use the `-c` flag, as the implementation can
be replaced with programs such as `perf`, even better suited for profiling.

When using `-O`, it has to be followed by an argument composed of <b>comma-delimited</b>
names, which represent obfuscations to apply. There are the following options:

- `opaque` - obfuscate using opaque predicates
- `interleave` - code interleaving
- `signedness` - <b>self designed</b> signedness obfuscation
- `forge` - <b>self designed</b> symbolic information forging
- `literal` - literal expansion
- `clone` - function cloning
- `array` - array restructuring
- <b>`all` - will apply all available obfuscations</b>
- <b>`annote` - adds annotation comments for certain obfuscations in the generated assembly file</b>

Probabilities of applying certain obfuscations can be tweaked at the bottom of the `common.h` file.

Some of the command line outputs use ANSI escape sequences, which may not visualise text as
intended on some machines.

#### Examples

Compile `program.jk` into an executable called `prog`<br/> 
`./jabukod program.jk -o prog`

Only do analysis for `program.jk` and show visual representation<br/>
`./jabukod program.jk -ga`

Compile and debug<br/>
`./jabukod program.jk -D`

Compile `program.jk` with opaque predicates and function interleaving obfuscations and add annotations<br/>
`./jabukod program.jk -O opaque,interleave,annote`

### Code Examples

Examples of Jabukód programs are stored in the `samples` directory. Tests from `test_programs` directory
can also be used as a reference for possible code structures.

For clarity and correct syntax highlighting in Visual Studio Code,
Jabukód source file names should be followed by `.jk` filename extension.

### Other make targets

There are also other build options. All of the available options are as follows; they all consider make being run from
the `build` subdirectory or with `-C build` argument:
- `make` <br> Build the compiler binary.
- `make clean` <br> Delete the generated `jabukod` executable.
- `make visualise INPUT_FILE=[file from samples subdirectory]` <br> Opens a window
with a syntax tree visualization (note this target requires `java`, `javac` and `dot` installed).
- `make pack` <br> Creates a `Jabukod.zip` file with everything needed for build.
- `make docs` <br> Generates code documentation into the `docs` subdirectory, including generating
the LaTeX pdf.
- `make clean_docs` <br> deletes all products of documentation generation (`html` and `latex` subdirectories).

## Code documentation

Code documentation generated with `make docs` is located in `docs` directory.
It is automatically generated using `Doxygen` from code comments.

After `make docs` is run, the PDF documentation is also put into the `build` directory
under the name `xjabur02_Jabukod_documentation.pdf`.

## Testing

Programs used for testing are located in `test_programs` directory. Testing was done manually
all throughout the development.

All testing and development was done on an x86_64 machine using <b>Ubuntu 24.04.2 LTS</b> operating
system. Correct functionality was verified on the `merlin` FIT VUT faculty server.

## Profiling

For profiling, two bash scripts are available:
- `profiling/jabukod_vs_C.sh` - Compares execution speed, generated file size, and memory consumption
of chosen algorithms implemented both in Jabukód and C. C programs are compiled using `gcc -O0`.
- `profiling/obfuscations.sh` - Compares execution speed, generated file size, and memory consumption of
a set of sample programs written in Jabukód, compiled with different obfuscations applied.

There are several programs that were used for profiling the generated executable files:
- `perf stat` - execution time in clock cycles.
- `/usr/bin/time` - execution time of longer-running programs in seconds.
- `valgrind --tool=massif` and `ms_print` for measuring used stack memory.

All profiled programs have output functionality removed to ensure fair comparison.

The subdirectories `profiling/measurement_1` and `profiling/measurement_2` contain
measurements done using the scripts. These measurements were used in the thesis text.

## Syntax highlighting

A part of this project is a custom extension for syntax highlighting of Jabukód files
with `.jk` filename extension. For correct functionality, move cloned repository containing the extension
to `~/.vscode/extensions/`. The extension is purposefully not available through Visual Studio
Code marketplace, as it is part of this bachelor thesis project.

You can clone the extension code from: <br/>
https://github.com/MJR009/Jabukod_syntax-highlighting

## License

With the approval of the thesis supervisor and vice dean for Bc. study, the source for
this thesis project was made available publicly on the authors GitHub.
It can be accessed through:<br/>
https://github.com/MJR009/BP_Jabukod

<b>The following is the license text included in the README.md in the repository. The LICENSE.txt itself can be viewed in the repository.</b>

This project uses the <b>GNU General Public License v3.0</b>. The only copyrighted dependency of this project is
ANTLR4 parser generator, available at: <br/>
https://github.com/antlr/antlr4

ANTLR4 uses the 'BSD 3-Clause "New" or "Revised" License'.
The original license and copyright notice is preserved in `/project/cmake/LICENSE.txt` of this
repository. The subdirectory also contains CMake files taken
over and edited from ANTLR4 repository for guaranteing correct
functionality and interoperability.

This repository also contains the tools binary in the `/tool/` subdirectory.
The same license holds here. As the tool was downloaded from the official ANTLR4 website,
the license present there is linked below: <br/>
https://www.antlr.org/license.html
