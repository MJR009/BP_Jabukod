# Jabukód compiler
#### Martin Jabůrek
### Bachelor Thesis Project
### Compiler generating the target code secured against reverse analysis

// TODO description - copy english abstract?

### Project structure

Top level project directories are arranged as follows:

```
BP_Jabukod
├── assembly
├── grammar
│   ├── Jabukod.g4
├── project
│   ├── cmake
│   ├── src
│   ├── include
│   │
│   ├── common.h
│   ├── main.h
│   ├── main.cpp
│   ├── Compile.h
│   ├── Compile.cpp
│   └── CMakeLists.txt
├── README.md
├── samples
└── tool
    └── antlr-4.13.2-complete.jar
```

### Requirements

Project requirements are analogous to ANTLR4 version 4.13.2:

- Minimum C++ standard <b>C++17</b>
- Minimum <b>CMake 2.8</b>
- `java` installed (needed for `make visualise`, tested with `openjdk 21.0.6 2025-01-21`)
- `javac` installed (needed for `make visualise`)
- `dot` installed ( -//- )

Some of the outputs use ANSI escape sequences which may not visualise text as
intended on some machines.

### Usage

Opening the project for the first time in an IDE will likely result in many errors appearing.
These are dependency errors from files to be generated during build.

The following is the recommended command for first time <b>building</b> the program:

`cmake -B build -S project && make -C build`<br>

---

The resulting <b>executable</b> `jabukod` will be located in the `build` subdirectory.
To run, do:

`./jabukod [-d] [-g] [-h] [-o path_to_binary] path_to_program`

<b>Command line arguments</b> can be used generally with `path_to_program` always last.
`[]` mark voluntary arguments. Semantics of each of the argument above are:

- `-d` - generate executable with debug info
- `-g` - print graphical representation of compilation
- `-h` - print help message
- `-o` - explicitly specified output file and directory (default is `out.s`)
- `path_to_program` - mandatory input file

For clarity, Jabukód source files should be followed by `.jk` filename extension.

---

There are also other build options. All of the available options are as follows, they all consider make being run from
the `build` subdirectory or with `-C build` argument:
- `make` <br> Build the compiler binary.
- `make visualise INPUT_FILE=[file from subdir. 'samples']` <br> Opens a window
with a syntax tree visualisation (note this target requires `java`, `javac` and `dot` installed).
- `make pack` <br> Creates a `Jabukod.zip` file with all project files needed to build the project.

<b>
As the build process needs to download and build the ANTLR4 runtime, initial
build may take a long time.
</b>
