#!/bin/bash
#SBATCH -J GlauberFit
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=00:20:00
#SBATCH --array=0-5

INDEX=$(echo "$SLURM_ARRAY_TASK_ID * 0.2" | bc -l)
FILE=/lustre/nyx/cbm/users/$USER/soft/centrality/input/test_input.root

K0=1
K1=10
MULTMIN=$1

EXE_DIR=/lustre/nyx/cbm/users/$USER/soft/centrality/build/
OUT_DIR=/lustre/nyx/cbm/users/$USER/test/centrality/$MULTMIN/

source /lustre/nyx/cbm/users/klochkov/soft/root6/root-6.10.08/install/bin/thisroot.sh

mkdir -p $OUT_DIR/$SLURM_ARRAY_TASK_ID
cd $OUT_DIR/$SLURM_ARRAY_TASK_ID

cp $EXE_DIR/glauber ./

echo $F
echo $NK
echo $MULTMIN

./glauber $F $K0 $K1 $MULTMIN $FILE &> log.txt

rm glauber

echo "DONE"
