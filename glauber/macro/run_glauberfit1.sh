#!/bin/bash
#SBATCH -J GlauberFit
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=00:20:00
#SBATCH --array=0-19

X=20

K=$(($SLURM_ARRAY_TASK_ID % $X + 1))
F=$(($SLURM_ARRAY_TASK_ID / $X))

DIR=/lustre/nyx/cbm/users/$USER/soft/centrality/

EXE_DIR=$DIR/build/
OUT_DIR=/lustre/nyx/cbm/users/$USER/fit/test_E/

source /lustre/nyx/cbm/users/klochkov/soft/root/root6/v6-12-06-cxx11/install/bin/thisroot.sh

mkdir -p $OUT_DIR/$F/$K
cd $OUT_DIR/$F/$K
F=$(echo "$F * 0.02" | bc -l)

cp $EXE_DIR/glauber ./

echo $F
#echo $NK

./glauber $F $K $K $DIR &> log.txt

rm glauber

echo "DONE"
