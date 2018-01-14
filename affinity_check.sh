TIMEFORMAT=%R

export KMP_AFFINITY=compact
echo $KMP_AFFINITY
for a in 30 32 56 60 64 90 92 116 120 128 150 152 176 180 210 212 236 240 ; do
	export OMP_NUM_THREADS=$a
	echo $OMP_NUM_THREADS
	time ./parallel_def_man_vec_avoid r p 8000 8001
done

export KMP_AFFINITY=scatter
echo $KMP_AFFINITY
for a in 30 32 56 60 64 90 92 116 120 128 150 152 176 180 210 212 236 240 ; do
	export OMP_NUM_THREADS=$a
	echo $OMP_NUM_THREADS
	time ./parallel_def_man_vec_avoid r p 8000 8001
done

export KMP_AFFINITY=balanced
echo $KMP_AFFINITY
for a in 30 32 56 60 64 90 92 116 120 128 150 152 176 180 210 212 236 240 ; do
	export OMP_NUM_THREADS=$a
	echo $OMP_NUM_THREADS
	time ./parallel_def_man_vec_avoid r p 8000 8001
done
