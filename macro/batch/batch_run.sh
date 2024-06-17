#!/bin/bash

GLAUBER_FIT_SOFT=/lustre/cbm/users/$USER/soft/Centrality/install/bin # Place of your Centrality framwework installation, bin subdirectory
source $GLAUBER_FIT_SOFT/CentralityConfig.sh

INDEX=${SLURM_ARRAY_TASK_ID}

INDEX=$(($INDEX-1))

NF=101 # How many equidistant values of f in [0; 1] range should be considered
# 101 means that they f will be 0, 0.01, 0.02, ..., 0.99, 1

# Prepare values of f and k from the job id value:
F=$(($INDEX%$NF))
K=$(($INDEX/$NF))
f=$(echo "$F * 0.01" | bc -l)

# Set names of directories and executable
PROJECT_DIR=/lustre/cbm/users/$USER/glauber
EXE_DIR=$GLAUBER_FIT_SOFT
EXE=glauber
OUTPUT_DIR=$PROJECT_DIR/outputs
WORK_DIR=$PROJECT_DIR/workdir

mkdir -p $WORK_DIR/$INDEX
mkdir -p $OUTPUT_DIR

cd $WORK_DIR/$INDEX

cp $EXE_DIR/$EXE ./

./$EXE $f $K >& log_${INDEX}.txt

cd ..
mv $INDEX $OUTPUT_DIR
