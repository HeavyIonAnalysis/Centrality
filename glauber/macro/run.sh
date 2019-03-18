#!/bin/bash

LOGDIR=/lustre/nyx/cbm/users/$USER/fit/test_klo/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

sbatch --partition main -D "/lustre/nyx/cbm/users/$USER/fit/test_klo/log" --export=ALL run_glauberfit1.sh
