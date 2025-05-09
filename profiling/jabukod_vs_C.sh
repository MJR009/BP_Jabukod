# jabukod_vs_C.sh
# Martin Jabůrek
#
# Compare chosen .jk and .c programs
# Ommits all output functionality for fair comparison

RED='\033[0;31m'
DEFAULT='\033[0m'

COMPILER_PATH="../build/jabukod"

JABUKOD_SRC_DIR="./jabukod"
C_SRC_DIR="./C"

JABUKOD_BUILD_DIR="./jabukod/build"
C_BUILD_DIR="./C/build"

PROGRAMS="ackermann binary_search quick_sort"
PROGRAMS_NO_ACKE="binary_search quick_sort"



# Compile

mkdir -p $JABUKOD_BUILD_DIR
mkdir -p $C_BUILD_DIR

for PROGRAM in $PROGRAMS;
do
    echo -e "${RED}${COMPILER_PATH} ${JABUKOD_SRC_DIR}/${PROGRAM}.jk -o ${JABUKOD_BUILD_DIR}/${PROGRAM} > /dev/null${DEFAULT}"
    $COMPILER_PATH $JABUKOD_SRC_DIR/$PROGRAM.jk -o $JABUKOD_BUILD_DIR/$PROGRAM > /dev/null

    echo -e "${RED}gcc -O0 ${C_SRC_DIR}/${PROGRAM}.c -o ${C_BUILD_DIR}/${PROGRAM}${DEFAULT}"
    gcc -O0 $C_SRC_DIR/$PROGRAM.c -o $C_BUILD_DIR/$PROGRAM
done

# Run time

echo -e "${RED}/usr/bin/time -v ${JABUKOD_BUILD_DIR}/ackermann${DEFAULT}"
/usr/bin/time -v $JABUKOD_BUILD_DIR/ackermann

echo -e "${RED}/usr/bin/time -v ${C_BUILD_DIR}/ackermann${DEFAULT}"
/usr/bin/time -v $C_BUILD_DIR/ackermann

for PROGRAM in $PROGRAMS_NO_ACKE;
do
    echo -e "${RED}perf stat -e cycles,instructions --repeat 100 ${JABUKOD_BUILD_DIR}/${PROGRAM}${DEFAULT}"
    perf stat -e cycles,instructions --repeat 100 $JABUKOD_BUILD_DIR/$PROGRAM

    echo -e "${RED}perf stat -e cycles,instructions --repeat 100 ${C_BUILD_DIR}/${PROGRAM}${DEFAULT}"
    perf stat -e cycles,instructions --repeat 100 $C_BUILD_DIR/$PROGRAM
done

# File size

echo -e "${RED}ls -l ${JABUKOD_BUILD_DIR}${DEFAULT}"
ls -l $JABUKOD_BUILD_DIR

echo -e "${RED}ls -l ${C_BUILD_DIR}${DEFAULT}"
ls -l $C_BUILD_DIR

# Memory consumption

for PROGRAM in $PROGRAMS;
do
    echo -e "${RED}valgrind --tool=massif --stacks=yes --massif-out-file=${JABUKOD_BUILD_DIR}/${PROGRAM}.massif.out ${JABUKOD_BUILD_DIR}/${PROGRAM}${DEFAULT}"
    valgrind --tool=massif --stacks=yes --massif-out-file=$JABUKOD_BUILD_DIR/$PROGRAM.massif.out $JABUKOD_BUILD_DIR/$PROGRAM

    echo -e "${RED}ms_print ${JABUKOD_BUILD_DIR}/${PROGRAM}.massif.out${DEFAULT}"
    ms_print $JABUKOD_BUILD_DIR/$PROGRAM.massif.out

    echo -e "${RED}valgrind --tool=massif --stacks=yes --massif-out-file=${C_BUILD_DIR}/${PROGRAM}.massif.out ${C_BUILD_DIR}/${PROGRAM}${DEFAULT}"
    valgrind --tool=massif --stacks=yes --massif-out-file=$C_BUILD_DIR/$PROGRAM.massif.out $C_BUILD_DIR/$PROGRAM

    echo -e "${RED}ms_print ${C_BUILD_DIR}/${PROGRAM}.massif.out${DEFAULT}"
    ms_print $C_BUILD_DIR/$PROGRAM.massif.out
done
