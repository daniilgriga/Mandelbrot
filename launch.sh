#!/bin/bash

result_file="data.md"

COMPILERS=("g++" "clang++")
FLAGS=("-O3" "-O2")
ARGS=("--v3" "--v2" "--v1")

cp makefile makefile.bak                                            # save

for compiler in "${COMPILERS[@]}"
do
    sed -i "s/CC = .*/CC = $compiler/g" makefile

    echo "<<< $compiler >>>"
    echo "$compiler" >> "$result_file"

    for flag in "${FLAGS[@]}"
    do
        sed -i "s/-O[0-3]/$flag/g" makefile

        make clean 2>/dev/null
        echo "cleaned"
        make       2>/dev/null
        echo "compiled"

        if [ ! -f "./build/test" ]; then                            # check compile process
            echo "Compile error with $flag"
            continue
        fi

        echo "<<< $flag >>>"

        for arg in "${ARGS[@]}"
            do
                echo "<<< $arg >>>"
                echo "$flag, $arg" >> "$result_file"

                for i in {1..21}
                do
                    ./build/test "$arg" --run=300
                done
            done

        echo "----------------" >> "$result_file"
    done
done

mv makefile.bak makefile
