#!/bin/bash
#SBATCH -J GlauberFit
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=08:00:00
#SBATCH --array=0-50

X=50

K=$(($SLURM_ARRAY_TASK_ID % $X + 1))

DIR=/lustre/nyx/cbm/users/$USER/soft/centrality/

EXE_DIR=$DIR/build_urqmd/
OUT_DIR=/lustre/nyx/cbm/users/$USER/fit/test_E/urqmd/

source /lustre/nyx/cbm/users/klochkov/soft/root/root6/v6-12-06-cxx11/install/bin/thisroot.sh

mkdir -p $OUT_DIR/$K
cd $OUT_DIR/$K

cp $EXE_DIR/glauber ./

./glauber E $K $K $DIR &> log.txt

rm glauber

echo "DONE"
