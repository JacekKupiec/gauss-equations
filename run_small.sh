ROW_SIZE=3000
COLUMN_SIZE=3001
LOOP_COUNT=20

echo basic
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./basic r p $ROW_SIZE $COLUMN_SIZE
done

echo basic + def_vec
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./basic_def_vec r p $ROW_SIZE $COLUMN_SIZE
done

echo basic + man_vec
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./basic_man_vec r p $ROW_SIZE $COLUMN_SIZE
done

echo basic + fsp
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./basic_fsp r p $ROW_SIZE $COLUMN_SIZE
done

echo optimal - aff
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./optimal r p $ROW_SIZE $COLUMN_SIZE
done

export OMP_NUM_THREADS=116
export KMP_AFFINITY=scatter

echo basic + affinity
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./basic r p $ROW_SIZE $COLUMN_SIZE
done

echo optimal - def_vec
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./optimal_def_vec r p $ROW_SIZE $COLUMN_SIZE
done

echo opitmal - man_vec
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./optimal_man_vec r p $ROW_SIZE $COLUMN_SIZE
done

echo optimal - fsp
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./optimal_fsp r p $ROW_SIZE $COLUMN_SIZE
done

echo optimal
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./optimal r p $ROW_SIZE $COLUMN_SIZE
done

echo sequential
for ((i=1;i<=$LOOP_COUNT;i++)); 
do 
   ./sequential r p $ROW_SIZE $COLUMN_SIZE
done