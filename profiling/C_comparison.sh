# Martin Jabůrek
# C_comparison.sh
#
# Compare chosen .jk and .c prorams with their full functionality.
# Unused for profiling in thesis !

RED='\033[0;31m'
DEFAULT='\033[0m'

COMPILER_PATH="../build/jabukod"

SRC="./programs"
C_SRC="./C"

PROGRAM_BUILD="./programs/build"
C_BUILD="./C/build/"

MEASURED="ackermann binary_search quick_sort"



# build

mkdir -p $PROGRAM_BUILD
mkdir -p $C_BUILD

for PROGRAM in $MEASURED;
do
    $COMPILER_PATH $SRC/$PROGRAM.jk -o $PROGRAM_BUILD/$PROGRAM > /dev/null
    gcc -O0 $C_SRC/$PROGRAM.c -o $C_BUILD/$PROGRAM
done



# run 

echo -e "${RED}Measure ackermann.jk${DEFAULT}"
/usr/bin/time -v $PROGRAM_BUILD/ackermann
echo -e "${RED}Measure ackermann.c${DEFAULT}"
/usr/bin/time -v $C_BUILD/ackermann

echo -e "${RED}Measure binary_search.jk${DEFAULT}"
perf stat -e cycles,instructions --repeat 10 $PROGRAM_BUILD/binary_search
echo -e "${RED}Measure binary_search.c${DEFAULT}"
perf stat -e cycles,instructions --repeat 10 $C_BUILD/binary_search

echo -e "${RED}Measure quick_sort.jk${DEFAULT}"
perf stat -e cycles,instructions --repeat 10 $PROGRAM_BUILD/quick_sort
echo -e "${RED}Measure quick_sort.c${DEFAULT}"
perf stat -e cycles,instructions --repeat 10 $C_BUILD/quick_sort
