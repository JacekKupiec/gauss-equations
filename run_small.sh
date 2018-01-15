echo basic
for ((i=1;i<=20;i++)); 
do 
   ./basic
done

echo basic + def_vec
for ((i=1;i<=20;i++)); 
do 
   ./basic_def_vec
done

echo optimal - def_vec
for ((i=1;i<=20;i++)); 
do 
   ./optimal_def_vec
done

echo basic + man_vec
for ((i=1;i<=20;i++)); 
do 
   ./basic_man_vec
done

echo opitmal - man_vec
for ((i=1;i<=20;i++)); 
do 
   ./opitmal_man_vec
done

echo basic + fsp
for ((i=1;i<=20;i++)); 
do 
   ./basic_fsp
done

echo optimal - fsp
for ((i=1;i<=20;i++)); 
do 
   ./optimal_fsp
done

echo optimal - aff
for ((i=1;i<=20;i++)); 
do 
   ./optimal
done

echo optimal
export OMP_NUM_THREADS=116
export KMP_AFFINITY=scatter
for ((i=1;i<=20;i++)); 
do 
   ./optimal
done

echo basic + affinity
for ((i=1;i<=20;i++)); 
do 
   ./basic
done

echo sequential
for ((i=1;i<=20;i++)); 
do 
   ./sequential
done