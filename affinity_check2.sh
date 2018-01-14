TIMEFORMAT=%R

export KMP_AFFINITY=compact
echo $KMP_AFFINITY
for a in 56 60 116 120 128 176 180 212 236 ; do
	export OMP_NUM_THREADS=$a
	echo $OMP_NUM_THREADS
	time ./parallel_def_man_vec_avoid r p 15000 15001
done

export KMP_AFFINITY=scatter
echo $KMP_AFFINITY
for a in 56 60 116 120 128 176 180 212 236 ; do
	export OMP_NUM_THREADS=$a
	echo $OMP_NUM_THREADS
	time ./parallel_def_man_vec_avoid r p 15000 15001
done

export KMP_AFFINITY=balanced
echo $KMP_AFFINITY
for a in 56 60 116 120 128 176 180 212 236 ; do
	export OMP_NUM_THREADS=$a
	echo $OMP_NUM_THREADS
	time ./parallel_def_man_vec_avoid r p 15000 15001
done
