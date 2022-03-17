#!/bin/bash
#SBATCH -J GlauberFit
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=08:00:00
#SBATCH --array=15-50

INDEX=$(echo "$SLURM_ARRAY_TASK_ID * 0.02" | bc -l)
FILE=/lustre/nyx/cbm/users/klochkov/na61/pbpb/30agev/data/flow/psd_centr/pbpb_30agev_data_config/all_corrections/pi_neg_debug/qa.root

K0=1
K1=30
MULTMIN=$1

EXE_DIR=/lustre/nyx/cbm/users/klochkov/soft/CentralityFramework/build/
OUT_DIR=/lustre/nyx/cbm/users/klochkov/data/centrality/na61/pb30pb/glauber/npart/pbpb_30agev_data_pions/$MULTMIN/

source /lustre/nyx/cbm/users/klochkov/soft/root6/root-6.10.08/install/bin/thisroot.sh

mkdir -p $OUT_DIR/$SLURM_ARRAY_TASK_ID
cd $OUT_DIR/$SLURM_ARRAY_TASK_ID

cp $EXE_DIR/glauber ./

echo $INDEX
echo $NK
echo $MULTMIN

./glauber $INDEX $K0 $K1 $MULTMIN $FILE &> log.txt

rm glauber

echo "DONE"
