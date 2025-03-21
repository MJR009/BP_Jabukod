# Jabukód compiler
### Martin Jabůrek
### Bachelor Thesis Project

// TODO description

### Project structure

Top level project directories are arranged as follows:

```
BP_Jabukod
├── assembly
│   └── experiments
├── grammar
│   ├── Jabukod.g4
│   └── notes
├── project
│   ├── cmake
│   ├── CMakeLists.txt
│   ├── main.cpp
│   └── src
├── README.md
├── samples
│   ├── rules
│   └── tokens
├── TODO
└── tool
    └── antlr-4.13.2-complete.jar
```

### Requirements

Project requirements are analogous to ANTLR4 version 4.13.2:

- Minimum C++ standard C++17
- Minimum CMake version 2.8
- `java` installed (needed for `make visualise`, tested with `openjdk 21.0.6 2025-01-21`)
- `javac` installed (needed for `make visualise`)
- `dot` installed ( -//- )
- // TODO

### Usage

Opening the project for the first time in an IDE will likely result in many erors appearing.
These are dependency error coming from files that are generating during the build process.

The following is the recommended command for first time building the program:

`cmake -B build -S project && make -C build`<br>

The resulting executable `jabukod` will be located in the `build` subdirectory.
To run, do:

// TODO

There are also other build options. All of the available options are as follows, they all consider make being run from
the `build` subdirectory or with `-C build` argument:
- `make` <br> Build the compiler binary.
- `make visualise INPUT_FILE=<file from subdir. 'samples'>` <br> Opens a window with a syntax tree visualisation (please note this target requires `java`, `javac` and `dot` installed).
- `make pack` <br> Creates a `Jabukod.zip` file with all project files needed to build the project.

As the build process needs to download and build the ANTLR4 runtime, initial
build may take a long time.
