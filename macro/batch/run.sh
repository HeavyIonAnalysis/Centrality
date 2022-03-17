#!/bin/bash

LOGDIR=/lustre/nyx/cbm/users/$USER/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

sbatch --partition main -D "/lustre/nyx/cbm/users/$USER/log" --export=ALL run_glauberfit.sh $1
