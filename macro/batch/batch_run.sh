#!/bin/bash

GLAUBER_FIT_SOFT=/lustre/cbm/users/$USER/soft/Centrality/install/bin # Place of your Centrality framwework installation, bin subdirectory
source $GLAUBER_FIT_SOFT/CentralityConfig.sh

INDEX=${SLURM_ARRAY_TASK_ID}

NF=51 # How many equidistant values of f in [0; 1] range should be considered
# 51 means that they f will be 0, 0.02, 0.04, ..., 0.98, 1

# Prepare values of f and k from the job id value:
F=$(($(($INDEX-1))%$NF))
K=$(($(($(($INDEX-1))/$NF))+1))
f=$(echo "$F / $(($NF-1))" | bc -l)

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
