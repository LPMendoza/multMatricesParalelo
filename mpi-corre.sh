#!/bin/bash

#PBS -l nodes=8:ppn=1,walltime=00:01:00

#PBS -N mpi_curso08_programa

#PBS -q staff

#PBS -d /mnt/zfs-pool/home/curso08/ejercicios/suma


#PBS -j oe

#PBS -V

#PBS -S /bin/bash

source $MODULESHOME/init/bash
module purge
module load Compilers/Parallel-Studio-XE-2018

NPROCS=`wc -l < $PBS_NODEFILE`

cat ${PBS_NODEFILE} | sort -u > $PBS_O_WORKDIR/machines.LINUX

mpirun -np $NPROCS -machinefile machines.LINUX ./main  


