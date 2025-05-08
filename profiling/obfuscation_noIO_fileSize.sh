# Martin Jabůrek
# obfuscation_noIO_fileSize.sh
#
# Meaurument of programs including obfuscations and combinations.

RED='\033[0;31m'
DEFAULT='\033[0m'

COMPILER_PATH="../build/jabukod"

SRC="./noIO_programs"
BUILD="./noIO_programs/build"

MEASURED="ackermann binary_search exp gcd quick_sort"
SETTINGS="opaque interleave signedness literal clone array all" # forge not applied, only renames



mkdir -p $BUILD

for PROGRAM in $MEASURED;
do
    $COMPILER_PATH $SRC/$PROGRAM.jk -o $BUILD/$PROGRAM > /dev/null # non-obfuscated

    for OPTION in $SETTINGS;
    do
        $COMPILER_PATH $SRC/$PROGRAM.jk -O $OPTION -o $BUILD/"${PROGRAM}_${OPTION}" > /dev/null
    done

    rm $BUILD/*.s
    rm $BUILD/*.o
    ls -l $BUILD
    rm $BUILD/*
done
