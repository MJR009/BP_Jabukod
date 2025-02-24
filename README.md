# Jabukód compiler
### Martin Jabůrek
### Bachelor Thesis Project

// TODO description

### Requirements

Project requirements are analogous to ANTLR4 version 4.13.2:

- Minimum C++ standard C++17
- Minimum CMake version 2.8
- `java` installed (needed for `make visualise`, tested with `openjdk 21.0.6 2025-01-21`)
- `javac` installed (needed for `make visualise`)
- `dot` installed ( -//- )
- // TODO

### Usage

The following are recommended steps for first time running the program:

`mkdir build`<br>
`cd build`<br>
`cmake ../project/`<br>
`make`<br>

The resulting executable `jabukod` will be located in the `build` directory.
To run, do:

// TODO

There are also other build options. All of the available options are:
- `make` <br> Build the compiler binary.
- `make visualise INPUT_FILE=<file from subdir. 'samples'>` <br> Opens a window with a syntax tree visualisation (please note this target requires `java`, `javac` and `dot` installed).
- `make pack` <br> Creates a `Jabukod.zip` file with all project files needed to build the project.

As the build process needs to download and build the ANTLR4 runtime, initial
build may take a long time.
