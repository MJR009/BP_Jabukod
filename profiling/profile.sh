# 1) perf stat -e cycles,instructions --repeat 10 ./out - PERFORMANCE
# 2) valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes ./out - CALLS
#    ( flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo )
#    ( flatpak install flathub org.kde.kcachegrind )
#    flatpak run org.kde.kcachegrind
#### otevřít callgrind.out.<pid> v LCacheGrind
# 3) /usr/bin/time -v ./out - PRO DLOUHO BĚŽICÍ PROGRAMY
#### externí time s více možnostmi
# 4) MANUÁLNÍ RDTSC
# *) grof se nehodí, protože by bylo třeba řešit, bylo by potřeba linkovat k C a volat mcount v prolozích

# 1) compare C and Jabukód
gcc -O0 -o ackermann ackermann.c
gcc -O0 -o binary_search binary_search.c
gcc -O0 -o quick_sort quick_sort.c

ls -l # size

../../build/jabukod ackermann.jk -o ./build/ackermann
../../build/jabukod binary_search.jk -o ./build/binary_search
../../build/jabukod quick_sort.jk -o ./build/quick_sort

/usr/bin/time -v ./build/ackermann
perf stat -e cycles,instructions --repeat 10 ./build/binary_search
perf stat -e cycles,instructions --repeat 10 ./build/quick_sort

# podobně pro C
