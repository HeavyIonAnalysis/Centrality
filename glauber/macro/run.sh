#!/bin/bash

LOGDIR=/lustre/nyx/cbm/users/$USER/fit/test_E/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

sbatch --partition debug -D "/lustre/nyx/cbm/users/$USER/fit/test_E/log" --export=ALL run_glauberfit1.sh
