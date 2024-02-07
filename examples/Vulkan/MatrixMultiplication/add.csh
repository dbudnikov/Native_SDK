/*!**********************************************************************
\File			add.csh
\Title			Naive Vector Multiplication

************************************************************************/

/************************************************************************
* Versions and defines have been ommited from this file
* This is because matrix sizes need to be added into the
* source code during run time
* A list of defines:
			Defined at runtime
					version - 320 es
					WG_X_SIZE - Size of local x work group
					WG_Y_SIZE - Size of local y work group
					W     - Vector size
			Defined in templated shader
					V0  (W)
					V1  (W)
					V2  (W)
************************************************************************/

/*********************** shader  Explanantion **************************
This is a naive implementation of matrix multiplication
There is no no optimisations, each incation just straight forwardly
calculates a cell in the product matrix. A is entered as transposed to
trial different memory layouts

No local memory
Input V0 V1
Output V2
************************************************************************/
shared float[WG_X_SIZE] V0Cache;
shared float[WG_X_SIZE] V1Cache;
shared float[WG_X_SIZE] V2Cache;

#if 0
void main()
{
	uint x = gl_GlobalInvocationID.x;
	float sum = 0.0;
    //{1, 1, 1} , {1, 1, 3}. {2, 2, 3}, {1, 2, 2}, {1, 3, 3}, {1, 2, 3}, {3, 3, 3}, {2, 3, 3}, {1, 1, 2}, {2, 2, 2}
	for (int k = 0; k < L; ++k) 
    { 
        //sum += V0[x] * V1[x];
        sum += (V0[x] * V0[x] * V0[x] +
                V0[x] * V0[x] * V2[x] +
                V1[x] * V1[x] * V2[x] +
                V0[x] * V1[x] * V1[x] +
                V0[x] * V2[x] * V2[x] +
                V0[x] * V1[x] * V2[x] +
                V2[x] * V2[x] * V2[x] +
                V1[x] * V2[x] * V2[x] +
                V0[x] * V0[x] * V1[x] +
                V1[x] * V1[x] * V1[x]);
    }
	V3[x] = sum;
}
#else
void main()
{
	uint x = gl_GlobalInvocationID.x;
    int local = int(gl_LocalInvocationID.x);
    V0Cache[local] = V0[x];
    V1Cache[local] = V1[x];
    V2Cache[local] = V2[x];
    barrier();
	float sum = 0.0;
    //{1, 1, 1} , {1, 1, 3}. {2, 2, 3}, {1, 2, 2}, {1, 3, 3}, {1, 2, 3}, {3, 3, 3}, {2, 3, 3}, {1, 1, 2}, {2, 2, 2}
	for (int k = 0; k < L; ++k)
    { 
        //sum += V0Cache[local] * V1Cache[local];
        sum += (V0Cache[local] * V0Cache[local] * V0Cache[local] +
                V0Cache[local] * V0Cache[local] * V2Cache[local] +
                V1Cache[local] * V1Cache[local] * V2Cache[local] +
                V0Cache[local] * V1Cache[local] * V1Cache[local] +
                V0Cache[local] * V2Cache[local] * V2Cache[local] +
                V0Cache[local] * V1Cache[local] * V2Cache[local] +
                V2Cache[local] * V2Cache[local] * V2Cache[local] +
                V1Cache[local] * V2Cache[local] * V2Cache[local] +
                V0Cache[local] * V0Cache[local] * V1Cache[local] +
                V1Cache[local] * V1Cache[local] * V1Cache[local]);
    }
	V3[x] = sum;
}
#endif