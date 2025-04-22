# Jabukód compiler
#### Martin Jabůrek
### Bachelor Thesis Project
### Compiler generating the target code secured against reverse analysis

// TODO description - copy english abstract?

All testing and development was done on a x86_64 machine using <b>Ubuntu 24.04.2 LTS</b> operating
system.

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
│   ├── SemanticErrors.h
│   ├── main.h
│   ├── main.cpp
│   └── CMakeLists.txt
├── samples
├── tool
│   └── antlr-4.13.2-complete.jar
├── README.md
└── assignment.pdf
```

### Requirements

Project requirements are analogous to ANTLR4 version 4.13.2:

- Minimum C++ standard <b>C++17</b>
- Minimum <b>CMake 2.8</b>
- `java` installed (needed for `make visualise`, tested with `openjdk 21.0.6 2025-01-21`)
- `javac` installed ( -//- )
- `dot` installed ( -//- )

Some of the outputs use ANSI escape sequences which may not visualise text as
intended on some machines.

### Usage

Opening the project for the first time in an IDE will likely result in many errors appearing.
These are dependency errors from files to be generated during build.

The following is the recommended command for first time <b>building</b> the program:

`cmake -B build -S project && make -C build`<br>

The resulting <b>executable</b> `jabukod` will be located in the `build` subdirectory.
<b>
As the build process needs to download and build the ANTLR4 runtime, initial
build may take a long time.
</b>
</br>
</br>
Also, due to several deprecation warnings shown during the build of the ANTLR4 runtime,
build is not processed without warnings. For this compiler, they pose no
limitation to intended functionality.

---

To run the generated compiler, do:

`./jabukod [-a] [-d] [-D] [-g] [-h] [-o path_to_binary] path_to_program`

<b>Command line arguments</b> can be used generally with `path_to_program` always last.
`[]` mark voluntary arguments. Semantics of each of the argument above are:

- `-a` - only execute source program <b>a</b>nalysis, do not compile
- `-d` - generate executable with <b>d</b>ebug info
- `-D` - compile and <b>D</b>ebug generated binary
- `-g` - print <b>g</b>raphical representation of compilation
- `-h` - print <b>h</b>elp message
- `-o` - explicitly specified <b>o</b>utput file and directory (default is `out.s`)
- `path_to_program` - mandatory input file

For clarity and correct syntax highligting in Visual Studio Code,
Jabukód source file names should be followed by `.jk` filename extension.

---

There are also other build options. All of the available options are as follows, they all consider make being run from
the `build` subdirectory or with `-C build` argument:
- `make` <br> Build the compiler binary.
- `make visualise INPUT_FILE=[file from subdir. 'samples']` <br> Opens a window
with a syntax tree visualisation (note this target requires `java`, `javac` and `dot` installed).
- `make pack` <br> Creates a `Jabukod.zip` file with everything needed for build.

---

### Syntax highlighting

A part of this project is a custom extension for syntax highlighting of Jabukód files
with `.jk` filename extension. For correct functionality, move cloned repository containing the extension
to `~/.vscode/extensions/`. The extension is purposefully not available publicly.
