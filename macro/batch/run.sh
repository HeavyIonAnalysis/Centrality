#!/bin/bash
LOGDIR=/lustre/cbm/users/$USER/glauber/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

# MAX_JOB_NUMBER must be equal to the product of NF from the batch_run.sh file and
# number of K values which you want to scan. E.g. if NF=51, and you want to scan
# for K=1, 2, ..., 9, 10 set MAX_JOB_NUMBER = 51*10=510
MAX_JOB_NUMBER=510

JOBS=1-$MAX_JOB_NUMBER

sbatch --job-name=glauber \
       -t 00:10:00 \
       --partition main \
       --output=$LOGDIR/out/%j.out.log \
       --error=$LOGDIR/error/%j.err.log \
       -a $JOBS \
       -- $PWD/batch_run.sh
