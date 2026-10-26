#!/bin/bash

ulimit -t 30      # timeout
ulimit -v 1048576 # 1 GB of memory
ulimit -f 1000    # number of written files
ulimit -c 0       # no core dumps; that's slow

NUM_OK=0
NUM_TOTAL=0
NUM_LINKER=0
NUM_CODEGEN=0
NUM_ASSEMBLY=0
NUM_EXECUTION=0
NUM_OUTPUT=0

for f in testes/*.udf ; do
  ((++NUM_TOTAL))
  BASENAME=$(basename -s .udf "$f")
  DIRNAME=$(dirname "$f")
  ASM="$DIRNAME/$BASENAME.asm"
  OBJ="$DIRNAME/$BASENAME.o"
  OUTFILE="$DIRNAME/$BASENAME.out"
  EXE="$DIRNAME/$BASENAME.exe"

  echo -n -e "$f:\t"

  if ! ./udf -o "$ASM" "$f" &> /dev/null ; then
    echo "FAILED CODEGEN"
    ((++NUM_CODEGEN))
    continue
  fi
  if ! yasm -felf32 -o "$OBJ" "$ASM" &> /dev/null ; then
    echo "FAILED ASSEMBLY"
    ((++NUM_ASSEMBLY))
    continue
  fi
  if ! ld -m elf_i386 -o "$EXE" "$OBJ" -lrts -L$HOME/compiladores/root/usr/lib &> /dev/null ; then
    echo "FAILED LINKER"
    ((++NUM_LINKER))
    continue
  fi
  if ! "$EXE" &> "$OUTFILE" ; then
    echo "FAILED EXECUTION"
    ((++NUM_EXECUTION))
    # Clean up .o and .exe
    rm -f "$OBJ" "$EXE"
    continue
  fi


  rm -f "$OBJ" "$EXE"

  if ! diff -q "$OUTFILE" "testes/expected/$BASENAME.out" &> /dev/null ; then
    echo "FAILED OUTPUT"
    diff -u "$OUTFILE" "testes/expected/$BASENAME.out"
    ((++NUM_OUTPUT))
    continue
  fi
  echo "OK"
  ((++NUM_OK))
done

echo
echo "OK Tests: $NUM_OK out of $NUM_TOTAL"
echo "FAILED LINKER: $NUM_LINKER"
echo "FAILED CODEGEN: $NUM_CODEGEN"
echo "FAILED ASSEMBLY: $NUM_ASSEMBLY"
echo "FAILED EXECUTION: $NUM_EXECUTION"
echo "FAILED OUTPUT: $NUM_OUTPUT"