for i in {1..8};
do
    echo "p = $i"
    mpirun -np $i ./4_9 1000000
done