#!/bin/bash
#SBATCH -J GlauberFit
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=00:20:00
#SBATCH --array=0-50

F=$(echo "$SLURM_ARRAY_TASK_ID * 0.02" | bc -l)

DIR=/lustre/nyx/cbm/users/$USER/soft/centrality/
K0=1
K1=10

EXE_DIR=$DIR/build_gol/
OUT_DIR=/lustre/nyx/cbm/users/$USER/fit/test_M/gol

source /lustre/nyx/cbm/users/klochkov/soft/root/root6/v6-12-06-cxx11/install/bin/thisroot.sh

mkdir -p $OUT_DIR/$SLURM_ARRAY_TASK_ID
cd $OUT_DIR/$SLURM_ARRAY_TASK_ID

cp $EXE_DIR/glauber ./

echo $F

./glauber M $F $K0 $K1 $DIR &> log.txt

rm glauber

echo "DONE"
