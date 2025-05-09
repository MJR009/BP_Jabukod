# obfuscations.sh
# Martin Jabůrek
#
# Measurements of chosen .jk programs without obfuscations, with them and with all combined.

RED='\033[0;31m'
DEFAULT='\033[0m'

COMPILER_PATH="../build/jabukod"

SRC_DIR="./jabukod"

BUILD_DIR="./jabukod/build"

PROGRAMS="ackermann binary_search exp gcd quick_sort"
PROGRAMS_NO_ACKE="binary_search exp gcd quick_sort"

SETTINGS="opaque interleave signedness literal clone array all" # forge not applied, only does renaming (slight memory impact can be neglected)



# Compile

mkdir -p $BUILD_DIR

for PROGRAM in $PROGRAMS;
do
    echo -e "${RED}${COMPILER_PATH} ${SRC_DIR}/${PROGRAM}.jk -o ${BUILD_DIR}/${PROGRAM} > /dev/null${DEFAULT}"
    $COMPILER_PATH $SRC_DIR/$PROGRAM.jk -o $BUILD_DIR/$PROGRAM > /dev/null

    for SETTING in $SETTINGS;
    do
        FILENAME="${PROGRAM}_${SETTING}"
        echo -e "${RED}${COMPILER_PATH} ${SRC_DIR}/${PROGRAM}.jk -O ${SETTING} -o ${BUILD_DIR}/${FILENAME} > /dev/null${DEFAULT}"
        $COMPILER_PATH $SRC_DIR/$PROGRAM.jk -O $SETTING -o $BUILD_DIR/$FILENAME > /dev/null
    done
done

# Run time

echo -e "${RED}/usr/bin/time -v ${BUILD_DIR}/ackermann${DEFAULT}"
/usr/bin/time -v $BUILD_DIR/ackermann

for SETTING in $SETTINGS;
do
    FILENAME="ackermann_${SETTING}"
    echo -e "${RED}/usr/bin/time -v ${BUILD_DIR}/${FILENAME}${DEFAULT}"
    /usr/bin/time -v $BUILD_DIR/$FILENAME
done

for PROGRAM in $PROGRAMS_NO_ACKE;
do
    echo -e "${RED}perf stat -e cycles,instructions --repeat 100 ${BUILD_DIR}/${PROGRAM}${DEFAULT}"
    perf stat -e cycles,instructions --repeat 100 $BUILD_DIR/$PROGRAM

    for SETTING in $SETTINGS;
    do
        FILENAME="${PROGRAM}_${SETTING}"
        echo -e "${RED}perf stat -e cycles,instructions --repeat 100 ${BUILD_DIR}/${FILENAME}${DEFAULT}"
        perf stat -e cycles,instructions --repeat 100 $BUILD_DIR/$FILENAME
    done
done

# File size

echo -e "${RED}ls -l ${BUILD_DIR}${DEFAULT}"
ls -l $BUILD_DIR

# Memory consumption

for PROGRAM in $PROGRAMS;
do
    echo -e "${RED}valgrind --tool=massif --stacks=yes --massif-out-file=${BUILD_DIR}/${PROGRAM}.massif.out ${BUILD_DIR}/${PROGRAM}${DEFAULT}"
    valgrind --tool=massif --stacks=yes --massif-out-file=$BUILD_DIR/$PROGRAM.massif.out $BUILD_DIR/$PROGRAM

    echo -e "${RED}ms_print ${BUILD_DIR}/${PROGRAM}.massif.out${DEFAULT}"
    ms_print $BUILD_DIR/$PROGRAM.massif.out

    for SETTING in $SETTINGS;
    do
        FILENAME="${PROGRAM}_${SETTING}"

        echo -e "${RED}valgrind --tool=massif --stacks=yes --massif-out-file=${BUILD_DIR}/${FILENAME}.massif.out ${BUILD_DIR}/${FILENAME}${DEFAULT}"
        valgrind --tool=massif --stacks=yes --massif-out-file=$BUILD_DIR/$FILENAME.massif.out $BUILD_DIR/$FILENAME

        echo -e "${RED}ms_print ${BUILD_DIR}/${FILENAME}.massif.out${DEFAULT}"
        ms_print $BUILD_DIR/$FILENAME.massif.out
    done
done
