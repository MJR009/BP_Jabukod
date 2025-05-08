# Martin Jabůrek
# C_comparison_noIO.sh
#
# Compare chosen .jk and .c prorams, ommiting all output functionality.
# A specialised program creating valgrind massif output logs to measure stack memory consumption.

RED='\033[0;31m'
DEFAULT='\033[0m'

COMPILER_PATH="../build/jabukod"

SRC="./noIO_programs"
C_SRC="./noIO_C"

PROGRAM_BUILD="./noIO_programs/build"
C_BUILD="./noIO_C/build/"

MEASURED="ackermann binary_search quick_sort"



# build

mkdir -p $PROGRAM_BUILD
mkdir -p $C_BUILD

for PROGRAM in $MEASURED;
do
    $COMPILER_PATH $SRC/$PROGRAM.jk -o $PROGRAM_BUILD/$PROGRAM > /dev/null
    gcc -O0 $C_SRC/$PROGRAM"_noIO.c" -o $C_BUILD/$PROGRAM
done



# memory

for PROGRAM in $MEASURED;
do
    valgrind --tool=massif --stacks=yes --massif-out-file=$PROGRAM_BUILD/$PROGRAM.massif.out $PROGRAM_BUILD/$PROGRAM
    valgrind --tool=massif --stacks=yes --massif-out-file=$C_BUILD/$PROGRAM.massif.out $C_BUILD/$PROGRAM
done
