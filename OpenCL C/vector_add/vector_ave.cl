__kernel void vector_ave(__global const float *x,
						__global float *restrict z)
{
	int index = get_local_id(0);
	int lsize = get_local_size(0);
	int gidx = get_group_id(0);
	*z += x [lsize*gidx + index];
}
