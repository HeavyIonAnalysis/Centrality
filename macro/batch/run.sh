#!/bin/bash
LOGDIR=/lustre/cbm/users/$USER/glauber/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

MAX_JOB_NUMBER=3131

JOBS=1-3131

sbatch --job-name=glauber \
       -t 00:10:00 \
       --partition main \
       --output=$LOGDIR/out/%j.out.log \
       --error=$LOGDIR/error/%j.err.log \
       -a $JOBS \
       -- $PWD/batch_run.sh
