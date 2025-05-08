# Martin Jabůrek
# obfuscation_noIO.sh
#
# Meaurument of programs including obfuscations and combinations

RED='\033[0;31m'
DEFAULT='\033[0m'

COMPILER_PATH="../build/jabukod"

SRC="./noIO_programs"
BUILD="./noIO_programs/build"

MEASURED="binary_search exp gcd quick_sort" # ackermann.jk would take too long to do using perf
SETTINGS="opaque interleave signedness literal clone array all" # forge not applied, only renames



mkdir -p $BUILD

# Ackermann /usr/bin/time
$COMPILER_PATH $SRC/ackermann.jk -o $BUILD/ackermann > /dev/null
echo -e "${RED}Measure ackermann.jk NORMAL /usr/bin/time${DEFAULT}"
/usr/bin/time -v $BUILD/ackermann

for OPTION in $SETTINGS;
do
    $COMPILER_PATH $SRC/ackermann.jk -O $OPTION -o $BUILD/ackermann > /dev/null

    echo -e "${RED}Measure ackermann.jk ${OPTION} /usr/bin/time${DEFAULT}"
    /usr/bin/time -v $BUILD/ackermann
done

# all perf
for PROGRAM in $MEASURED;
do
    # non-obfscated
    $COMPILER_PATH $SRC/$PROGRAM.jk -o $BUILD/$PROGRAM > /dev/null
    echo -e "${RED}Measure ${PROGRAM}.jk NORMAL ${DEFAULT}"
    perf stat -e cycles,instructions --repeat 100 $BUILD/$PROGRAM

    # obfuscated
    for OPTION in $SETTINGS;
    do
        # build
        $COMPILER_PATH $SRC/$PROGRAM.jk -O $OPTION -o $BUILD/$PROGRAM > /dev/null

        # run
        echo -e "${RED}Measure ${PROGRAM}.jk -O ${OPTION}${DEFAULT}"
        perf stat -e cycles,instructions --repeat 100 $BUILD/$PROGRAM
    done

    echo -e "${RED}=================================================${DEFAULT}\n"
done
