#!/bin/bash

LOGDIR=/lustre/nyx/cbm/users/$USER/fit/test_E/urqmd/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

sbatch --partition main -D "/lustre/nyx/cbm/users/$USER/fit/test_E/urqmd/log" --export=ALL run_glauberfit1Energy.sh
