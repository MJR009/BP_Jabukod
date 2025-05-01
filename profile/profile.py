# 1) perf stat -e cycles,instructions --repeat 10 ./out - PERFORMANCE
# 2) valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes ./out - CALLS
#### otevřít callgrind.out.<pid> v LCacheGrind
# 3) /usr/bin/time -v ./out - PRO DLOUHO BĚŽICÍ PROGRAMY
#### externí time s více možnostmi
# 4) MANUÁLNÍ RDTSC
# *) grof se nehodí, protože by bylo třeba řešit, bylo by potřeba linkovat k C a volat mcount v prolozích

print("Hello world!")
