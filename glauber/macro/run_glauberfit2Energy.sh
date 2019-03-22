#!/bin/bash
#SBATCH -J GlauberFit
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=00:20:00
#SBATCH --array=0-49

DIR=/lustre/nyx/cbm/users/$USER/soft/centrality/
K0=1
K1=20

EXE_DIR=$DIR/build_klo/
OUT_DIR=/lustre/nyx/cbm/users/$USER/fit/test_E/klo

source /lustre/nyx/cbm/users/klochkov/soft/root/root6/v6-12-06-cxx11/install/bin/thisroot.sh

mkdir -p $OUT_DIR/$SLURM_ARRAY_TASK_ID
cd $OUT_DIR/$SLURM_ARRAY_TASK_ID

cp $EXE_DIR/glauber ./

./glauber E $K0 $K1 $DIR &> log.txt

rm glauber

echo "DONE"
