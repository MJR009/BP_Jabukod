RED='\033[0;31m'
DEFAULT='\033[0m'

COMPILER_PATH="../build/jabukod"

SRC="./no_IO"
C_SRC="./noIO_C"

PROGRAM_BUILD="./no_IO/build"
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



# run 

echo -e "${RED}Measure ackermann.jk /usr/bin/time${DEFAULT}"
/usr/bin/time -v $PROGRAM_BUILD/ackermann
echo -e "${RED}Measure ackermann.c /usr/bin/time${DEFAULT}"
/usr/bin/time -v $C_BUILD/ackermann

for PROGRAM in $MEASURED;
do
    echo -e "${RED}Measure ${PROGRAM}.jk${DEFAULT}"
    perf stat -e cycles,instructions --repeat 10 $PROGRAM_BUILD/$PROGRAM
    echo -e "${RED}Measure ${PROGRAM}.c${DEFAULT}"
    perf stat -e cycles,instructions --repeat 10 $C_BUILD/$PROGRAM
done
