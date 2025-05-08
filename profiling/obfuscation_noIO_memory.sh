# Martin Jabůrek
# obfuscation_noIO_memory.sh
#
# Meaurument of programs including obfuscations and combinations.

COMPILER_PATH="../build/jabukod"

SRC="./noIO_programs"
BUILD="./noIO_programs/build"

MEASURED="ackermann binary_search exp gcd quick_sort"
SETTINGS="opaque interleave signedness literal clone array all" # forge not applied, only renames



mkdir -p $BUILD
mkdir -p $BUILD/massif

for PROGRAM in $MEASURED;
do
    $COMPILER_PATH $SRC/$PROGRAM.jk -o $BUILD/$PROGRAM > /dev/null # non-obfuscated
    valgrind --tool=massif --stacks=yes --massif-out-file=$BUILD/massif/$PROGRAM.massif.out $BUILD/$PROGRAM

    for OPTION in $SETTINGS;
    do
        $COMPILER_PATH $SRC/$PROGRAM.jk -O $OPTION -o $BUILD/"${PROGRAM}_${OPTION}" > /dev/null
        valgrind --tool=massif --stacks=yes --massif-out-file=$BUILD/massif/"${PROGRAM}_${OPTION}.massif.out" $BUILD/$PROGRAM
    done
done
