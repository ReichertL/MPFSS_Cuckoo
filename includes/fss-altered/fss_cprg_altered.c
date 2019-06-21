#include "fss_cprg_altered.h"
#include "oram_fssl/floram_util.h"
#include "ackutil.h"
#include <omp.h>

struct fss_cprg_offline {
	size_t size;
	size_t blockmultiple;
	size_t startlevel;
	size_t thislevel;
	size_t endlevel;
	size_t thislevelblocks;
	size_t nextlevelblocks;
	void * Z;
	bool * advicebits_l;
	bool * advicebits_r;
	uint8_t * level_data;
	uint64_t * lda;
	uint64_t * ldb;
	uint8_t * lda2;
	uint8_t * ldb2;
	bool * level_bits;
	bool * lba;
	bool * lbb;
	void * keyL;
	void * keyR;
};

typedef struct block_t {
	uint64_t data[BLOCKSIZE/sizeof(uint64_t)];
} block_t;

void block_xor(block_t * a, block_t * b) {
	#pragma omp simd
	for (uint8_t ii = 0; ii < BLOCKSIZE/sizeof(uint64_t); ii++) {
		a->data[ii] ^= b->data[ii];
	}
}

#pragma omp declare reduction(^: block_t: block_xor(&omp_out, &omp_in)) initializer (omp_priv = { 0 })

void fss_cprg_offline_start(uint8_t * local_output, bool * local_bit_output, uint64_t * accumulator_L, uint64_t * accumulator_R, fss_cprg_offline * fsso) {
	fsso->thislevel = 0;
	fsso->thislevelblocks = 1;
	fsso->nextlevelblocks = 2;

#ifdef ORAM_PROFILE_SCHEDULING
	printf("START FSS CPRG OFFLINE LEVEL %d %lld\n", fsso->thislevel,current_timestamp());
#endif

	fsso->lda = (uint64_t *)fsso->level_data;
	fsso->lda2 = (uint8_t *)fsso->level_data;
	fsso->ldb = (uint64_t *)local_output;
	fsso->ldb2 = (uint8_t *)local_output;
	fsso->lba = fsso->level_bits;
	fsso->lbb = local_bit_output;

	memset(accumulator_L, 0, BLOCKSIZE);
	memset(accumulator_R, 0, BLOCKSIZE);
	get_random_bytes(fsso->lda2, BLOCKSIZE);
	if (ocCurrentParty() == 1) fsso->lda2[0] &= 0xFE;
	else fsso->lda2[0] |= 1;

	uint64_t * lda = fsso->lda;
	uint64_t * ldb = fsso->ldb;

	block_t accL;
	block_t accR;

	offline_prg(&fsso->ldb2[0], fsso->lda2, fsso->keyL);
	offline_prg(&fsso->ldb2[BLOCKSIZE], fsso->lda2, fsso->keyR);
	#pragma omp simd aligned(ldb, accumulator_L, accumulator_R:16) 
	for (size_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
		accumulator_L[jj] ^= ldb[jj];
		accumulator_R[jj] ^= ldb[BLOCKSIZE/sizeof(uint64_t)+jj];
	}

	fsso->lba[0] = fsso->lda2[0] & 1;

	if (fsso->thislevel == fsso->endlevel) {
		memcpy(fsso->ldb, fsso->lda, BLOCKSIZE*fsso->blockmultiple);
		memcpy(accumulator_L, fsso->lda, BLOCKSIZE*fsso->blockmultiple);
		memcpy(fsso->lbb, fsso->lba, sizeof(bool));
	}

#ifdef ORAM_PROFILE_SCHEDULING
	printf("END FSS CPRG OFFLINE LEVEL %d %lld\n", fsso->thislevel,current_timestamp());
#endif
}

void fss_cprg_offline_process_round(uint8_t * accumulator_L, uint8_t * accumulator_R, uint8_t * z, bool advicebit_l, bool advicebit_r, fss_cprg_offline * fsso) {
	fsso->thislevel += 1;
	fsso->thislevelblocks = fsso->nextlevelblocks;
	fsso->nextlevelblocks = (fsso->size + (1ll<<(fsso->endlevel - fsso->thislevel -1)) - 1) / (1ll<<(fsso->endlevel - fsso->thislevel -1));
	if (fsso->thislevel == fsso->endlevel -1) fsso->nextlevelblocks = fsso->size;

#ifdef ORAM_PROFILE_SCHEDULING
	printf("START FSS CPRG OFFLINE LEVEL %d %lld\n", fsso->thislevel,current_timestamp());
#endif

	uint64_t* t; uint8_t* t2; bool * tb;
	size_t expansion_stride;

	t2 = fsso->ldb2; t = fsso->ldb; tb = fsso->lbb;
	fsso->ldb2 = fsso->lda2; fsso->ldb = fsso->lda; fsso->lbb = fsso->lba;
	fsso->lda2 = t2; fsso->lda = t; fsso->lba = tb;

	uint64_t * lda = fsso->lda;
	uint64_t * ldb = fsso->ldb;

	block_t accL = {0};
	block_t accR = {0};

	if (fsso->thislevel == fsso->endlevel - 1 && (fsso->thislevel % 2) == 0) {
		expansion_stride = (BLOCKSIZE * fsso->blockmultiple);
	} else {
		expansion_stride = BLOCKSIZE;
	}

	floram_set_procs_for_data_size(BLOCKSIZE * (fsso->nextlevelblocks + fsso->thislevelblocks));
	#pragma omp parallel for reduction(^:accL,accR) schedule(guided)
	for (size_t ii = 0; ii < 4*(fsso->nextlevelblocks/8); ii+=4) {
		fsso->lba[ii] = (fsso->lda2[ii*BLOCKSIZE] & 1) ^ (fsso->lbb[ii/2] & advicebit_l);
		fsso->lba[ii+1] = (fsso->lda2[(ii+1)*BLOCKSIZE] & 1) ^ (fsso->lbb[ii/2] & advicebit_r);
		fsso->lba[ii+2] = (fsso->lda2[(ii+2)*BLOCKSIZE] & 1) ^ (fsso->lbb[(ii+2)/2] & advicebit_l);
		fsso->lba[ii+3] = (fsso->lda2[(ii+3)*BLOCKSIZE] & 1) ^ (fsso->lbb[(ii+2)/2] & advicebit_r);

		if (fsso->lbb[ii/2]) {
			#pragma omp simd aligned(lda,z:16)
			for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
				lda[ii*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= ((uint64_t *)z)[jj];
				lda[(ii+1)*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= ((uint64_t *)z)[jj];
			}
		}

		if (fsso->lbb[(ii+2)/2]) {
			#pragma omp simd aligned(lda,z:16)
			for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
				lda[(ii+2)*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= ((uint64_t *)z)[jj];
				lda[(ii+3)*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= ((uint64_t *)z)[jj];
			}
		}

		offline_prg_oct(&fsso->ldb2[ii*2*expansion_stride], &fsso->ldb2[(ii*2+1)*expansion_stride], &fsso->ldb2[(ii*2+2)*expansion_stride], &fsso->ldb2[(ii*2+3)*expansion_stride],
						&fsso->ldb2[(ii*2+4)*expansion_stride], &fsso->ldb2[(ii*2+5)*expansion_stride], &fsso->ldb2[(ii*2+6)*expansion_stride], &fsso->ldb2[(ii*2+7)*expansion_stride],
						&fsso->lda2[ii*BLOCKSIZE],  &fsso->lda2[ii*BLOCKSIZE], &fsso->lda2[(ii+1)*BLOCKSIZE], &fsso->lda2[(ii+1)*BLOCKSIZE],
						&fsso->lda2[(ii+2)*BLOCKSIZE],  &fsso->lda2[(ii+2)*BLOCKSIZE], &fsso->lda2[(ii+3)*BLOCKSIZE], &fsso->lda2[(ii+3)*BLOCKSIZE],
						fsso->keyL, fsso->keyR, fsso->keyL, fsso->keyR,
						fsso->keyL, fsso->keyR, fsso->keyL, fsso->keyR);

		#pragma omp simd aligned(ldb:16)
		for (size_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
			accL.data[jj] ^= ldb[ii*2*expansion_stride/sizeof(uint64_t)+jj] ^ ldb[(ii*2+2)*expansion_stride/sizeof(uint64_t)+jj]
								^ ldb[(ii*2+4)*expansion_stride/sizeof(uint64_t)+jj] ^ ldb[(ii*2+6)*expansion_stride/sizeof(uint64_t)+jj];
			accR.data[jj] ^= ldb[(ii*2+1)*expansion_stride/sizeof(uint64_t)+jj] ^ ldb[(ii*2+3)*expansion_stride/sizeof(uint64_t)+jj]
								^ ldb[(ii*2+5)*expansion_stride/sizeof(uint64_t)+jj] ^ ldb[(ii*2+7)*expansion_stride/sizeof(uint64_t)+jj];
		}
	}

	for (size_t ii = 4*(fsso->nextlevelblocks/8); ii < fsso->thislevelblocks ; ii++) {

		if (ii%2 == 0) {
			fsso->lba[ii] = (fsso->lda2[ii*BLOCKSIZE] & 1) ^ (fsso->lbb[ii/2] & advicebit_l);
		} else {
			fsso->lba[ii] = (fsso->lda2[ii*BLOCKSIZE] & 1) ^ (fsso->lbb[ii/2] & advicebit_r);
		}

		if (fsso->lbb[ii/2]) {
			#pragma omp simd aligned(lda,z:16)
			for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
				lda[ii*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= ((uint64_t *)z)[jj];
			}
		}
	
		if ((ii+1)*2 <= fsso->nextlevelblocks) {
			offline_prg(&fsso->ldb2[ii*2*expansion_stride], &fsso->lda2[ii*BLOCKSIZE], fsso->keyL);
			offline_prg(&fsso->ldb2[(ii*2+1)*expansion_stride], &fsso->lda2[ii*BLOCKSIZE], fsso->keyR);
			#pragma omp simd aligned(ldb:16)
			for (size_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
				accL.data[jj] ^= ldb[ii*2*expansion_stride/sizeof(uint64_t)+jj];
				accR.data[jj] ^= ldb[(ii*2+1)*expansion_stride/sizeof(uint64_t)+jj];
			}
		} else if (ii*2+1 <= fsso->nextlevelblocks) {
			offline_prg(&fsso->ldb2[ii*2*expansion_stride], &fsso->lda2[ii*BLOCKSIZE], fsso->keyL);
			#pragma omp simd aligned(ldb:16)
			for (size_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
				accL.data[jj] ^= ldb[ii*2*expansion_stride/sizeof(uint64_t)+jj];
			}
		}
	}

	for (size_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
		((uint64_t *)accumulator_L)[jj] = accL.data[jj];
		((uint64_t *)accumulator_R)[jj] = accR.data[jj];
	}

#ifdef ORAM_PROFILE_SCHEDULING
	printf("END FSS CPRG OFFLINE LEVEL %d %lld\n", fsso->thislevel,current_timestamp());
#endif
}

void fss_cprg_offline_finalize(uint8_t * accumulator, uint8_t * z, bool advicebit_l, bool advicebit_r, fss_cprg_offline * fsso) {
	fsso->thislevel += 1;
	fsso->thislevelblocks = fsso->nextlevelblocks;

#ifdef ORAM_PROFILE_SCHEDULING
	printf("START FSS CPRG OFFLINE LEVEL %d %lld\n", fsso->thislevel,current_timestamp());
#endif

	uint64_t* t; uint8_t* t2; bool * tb;

	t2 = fsso->ldb2; t = fsso->ldb; tb = fsso->lbb;
	fsso->ldb2 = fsso->lda2; fsso->ldb = fsso->lda; fsso->lbb = fsso->lba;
	fsso->lda2 = t2; fsso->lda = t; fsso->lba = tb;

	uint64_t * lda = fsso->lda;
	uint64_t * ldb = fsso->ldb;
	uint8_t * local_output;

	block_t acc = {0};

	if (fsso->thislevel%2==0) {
		local_output = fsso->ldb2;

		floram_set_procs_for_data_size(BLOCKSIZE * fsso->thislevelblocks * 2);
		#pragma omp parallel for reduction(^:acc) schedule(guided)
		for (size_t ii = 0; ii < fsso->thislevelblocks; ii++) {
			if (ii%2 == 0) {
				fsso->lba[ii] = (fsso->lda2[ii*BLOCKSIZE] & 1) ^ (fsso->lbb[ii/2] & advicebit_l);
			} else {
				fsso->lba[ii] = (fsso->lda2[ii*BLOCKSIZE] & 1) ^ (fsso->lbb[ii/2] & advicebit_r);
			}

			if (fsso->lbb[ii/2]) {
				#pragma omp simd aligned(ldb,lda,z:16)
				for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
					ldb[ii*((BLOCKSIZE*fsso->blockmultiple)/sizeof(uint64_t))+jj] = lda[ii*(BLOCKSIZE/sizeof(uint64_t))+jj] ^ ((uint64_t *)z)[jj];
					acc.data[jj] ^= ldb[ii*((BLOCKSIZE*fsso->blockmultiple)/sizeof(uint64_t))+jj];
				}
			} else {
				memcpy(&fsso->ldb[ii*((BLOCKSIZE*fsso->blockmultiple)/sizeof(uint64_t))], &fsso->lda[ii*(BLOCKSIZE/sizeof(uint64_t))], BLOCKSIZE);
				#pragma omp simd aligned(ldb:16)
				for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
					acc.data[jj] ^= ldb[ii*((BLOCKSIZE*fsso->blockmultiple)/sizeof(uint64_t))+jj];
				}
			}
		}

		memcpy(fsso->lbb, fsso->lba, fsso->thislevelblocks*sizeof(bool));

	} else {
		local_output = fsso->lda2;

		floram_set_procs_for_data_size(BLOCKSIZE * fsso->thislevelblocks);
		#pragma omp parallel for reduction(^:acc) schedule(guided)
		for (size_t ii = 0; ii < fsso->thislevelblocks; ii++) {
			if (ii%2 == 0) {
				fsso->lba[ii] = (fsso->lda2[ii*(BLOCKSIZE*fsso->blockmultiple)] & 1) ^ (fsso->lbb[ii/2] & advicebit_l);
			} else {
				fsso->lba[ii] = (fsso->lda2[ii*(BLOCKSIZE*fsso->blockmultiple)] & 1) ^ (fsso->lbb[ii/2] & advicebit_r);
			}

			if (fsso->lbb[ii/2]) {
				#pragma omp simd aligned(lda,z:16)
				for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
					lda[ii*((BLOCKSIZE*fsso->blockmultiple)/sizeof(uint64_t))+jj] ^= ((uint64_t *)z)[jj];
					acc.data[jj] ^= lda[ii*((BLOCKSIZE*fsso->blockmultiple)/sizeof(uint64_t))+jj];
				}
			} else {
				#pragma omp simd aligned(lda:16)
				for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
					acc.data[jj] ^= lda[ii*((BLOCKSIZE*fsso->blockmultiple)/sizeof(uint64_t))+jj];
				}
			}
		}
	}

	for (size_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
		((uint64_t *)accumulator)[jj] = acc.data[jj];
	}

	for (size_t jj = 1; jj < fsso->blockmultiple; jj++) {
		for (size_t ii = 0; ii < BLOCKSIZE/sizeof(uint64_t); ii++) acc.data[ii] = 0;

		#pragma omp parallel for reduction(^:acc) schedule(guided)
		for (size_t ii = 0; ii < 8*(fsso->thislevelblocks/8); ii+=8)  {
			offline_prg_oct(
				&local_output[(ii+0) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+1) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+2) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+3) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+4) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+5) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+6) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+7) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii+0) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				&local_output[(ii+1) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				&local_output[(ii+2) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				&local_output[(ii+3) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				&local_output[(ii+4) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				&local_output[(ii+5) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				&local_output[(ii+6) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				&local_output[(ii+7) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				fsso->keyL,
				fsso->keyL,
				fsso->keyL,
				fsso->keyL,
				fsso->keyL,
				fsso->keyL,
				fsso->keyL,
				fsso->keyL
			);
			#pragma omp simd aligned(local_output:16)
			for (size_t kk = 0; kk < BLOCKSIZE/sizeof(uint64_t); kk++) {
				acc.data[kk] ^= ((uint64_t *)(&local_output[(ii+0) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk]
								^ ((uint64_t *)(&local_output[(ii+1) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk]
								^ ((uint64_t *)(&local_output[(ii+2) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk]
								^ ((uint64_t *)(&local_output[(ii+3) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk]
								^ ((uint64_t *)(&local_output[(ii+4) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk]
								^ ((uint64_t *)(&local_output[(ii+5) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk]
								^ ((uint64_t *)(&local_output[(ii+6) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk]
								^ ((uint64_t *)(&local_output[(ii+7) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk];
			}
		}

		for (size_t ii = 8*(fsso->thislevelblocks/8); ii < fsso->thislevelblocks ; ii++) {
			offline_prg(
				&local_output[(ii) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
				&local_output[(ii) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
				fsso->keyL
			);

			for (size_t kk = 0; kk < BLOCKSIZE/sizeof(uint64_t); kk++) {
				acc.data[kk] ^= ((uint64_t *)(&local_output[(ii) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)]))[kk];
			}
		}

		for (size_t ii = 0; ii < BLOCKSIZE/sizeof(uint64_t); ii++) {
			((uint64_t *)accumulator)[jj*BLOCKSIZE/sizeof(uint64_t)+ii] = acc.data[ii];
		}
	}

#ifdef ORAM_PROFILE_SCHEDULING
	printf("END FSS CPRG OFFLINE LEVEL %d %lld\n", fsso->thislevel,current_timestamp());
#endif
}

void fss_cprg_offline_parallelizer(void* fss, void* indexp, void *blockdelta, void * local_output, void * local_bit_output, void* pd, fss_cprg_traverser_fn fn, facb_fn cbfn, void* cbpass) {

	omp_set_nested(true);

	//#pragma omp parallel num_threads(2)
	//{
		//OpenMP seems to get along with obliv-c just fine, so long as obliv-c only uses the master thread.
		//#pragma omp master
		//{
			fn(blockdelta, local_output, local_bit_output, fss, indexp);
		//}
		
		if (*cbfn!=NULL) {
			#pragma omp single
			{
				#pragma omp task
				{
					if (omp_get_num_threads() > 1) cbfn(cbpass, pd);
					else cbfn(cbpass, NULL);
				}
			}
		}
	//}
}

fss_cprg_offline * fss_cprg_offline_new(size_t size, size_t blockmultiple, uint8_t * keyL, uint8_t * keyR) {
	fss_cprg_offline * fsso = malloc(sizeof(fss_cprg_offline));
	fsso->size = size;
	fsso->blockmultiple = blockmultiple;
	fsso->startlevel = 0;
	fsso->endlevel = LOG2LL(size) + (((1 << LOG2LL(size)) < size)? 1:0);
	posix_memalign(&fsso->level_data,16,(1ll<<fsso->endlevel) * BLOCKSIZE);
	posix_memalign(&fsso->Z,16,(fsso->endlevel - fsso->startlevel) * BLOCKSIZE);
	fsso->advicebits_l = malloc((fsso->endlevel - fsso->startlevel) * sizeof(bool));
	fsso->advicebits_r = malloc((fsso->endlevel - fsso->startlevel) * sizeof(bool));
	fsso->level_bits = malloc(size * sizeof(bool));

	offline_prg_init();
	fsso->keyL = offline_prg_keyschedule(keyL);
	fsso->keyR = offline_prg_keyschedule(keyR); 

	return fsso;
}

void fss_cprg_offline_free(fss_cprg_offline * fsso) {
	free(fsso->level_data);
	free(fsso->level_bits);
	free(fsso->advicebits_l);
	free(fsso->advicebits_r);
	free(fsso->Z);
	free(fsso->keyL);
	free(fsso->keyR);
	free(fsso);
}