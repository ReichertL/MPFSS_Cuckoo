#include "fss.h"
#include "floram_util.h"
#include "ackutil.h"
#include <omp.h>

struct fss_offline {
	size_t size;
	size_t blockmultiple;
	size_t startlevel;
	size_t endlevel;
	void * Z;
	bool * advicebits_l;
	bool * advicebits_r;
	void * level_data;
	void * level_bits;
	void * keyL;
	void * keyR;
	uint32_t readycount;
};

void fss_offline_start(fss_offline * fsso, uint8_t * blocks) {
	memcpy(fsso->level_data, blocks, (1ll<<fsso->startlevel) * BLOCKSIZE);
	fsso->readycount = 0;
}

void fss_offline_push_Z(fss_offline * fsso, uint8_t * Z, bool advicebit_l, bool advicebit_r, size_t level) {
	memcpy(&fsso->Z[(level- fsso->startlevel - 1)*BLOCKSIZE], Z, BLOCKSIZE);
	fsso->advicebits_l[level- fsso->startlevel - 1] = advicebit_l;
	fsso->advicebits_r[level- fsso->startlevel - 1] = advicebit_r;
	floram_atomic_inc(&fsso->readycount);
}

void fss_offline_readblockvector(uint8_t * local_output, bool * local_bit_output, fss_offline * fsso) {	

	floram_set_procs_for_data_size(BLOCKSIZE * ((1ll<<fsso->endlevel) + (1ll<<(fsso->endlevel-1))));
	#pragma omp parallel
	{
		size_t thislevel = fsso->startlevel;
		size_t thislevelblocks = (1ll<<fsso->startlevel);
		size_t nextlevelblocks = (fsso->size + (1ll<<(fsso->endlevel - thislevel -1)) - 1) / (1ll<<(fsso->endlevel - thislevel -1));
		size_t expansion_stride;
		uint32_t readycount_cache=0;

		uint64_t* a = (uint64_t *)fsso->level_data;
		uint8_t* a2 = (uint8_t *)fsso->level_data;
		uint64_t* b = (uint64_t *)local_output;
		uint8_t* b2 = (uint8_t *)local_output;
		uint64_t* t;
		uint8_t* t2;
		uint64_t* z;
		bool advicebit_l, advicebit_r;
		bool * a_bits = fsso->level_bits;
		bool * b_bits = local_bit_output;
		bool * t_bits;

		if (thislevel == fsso->endlevel - 1 && b2 == local_output) {
			expansion_stride = (BLOCKSIZE * fsso->blockmultiple);
		} else {
			expansion_stride = BLOCKSIZE;
		}

#ifdef ORAM_PROFILE_SCHEDULING
		#pragma omp single nowait
		printf("START FSS OFFLINE LEVEL 0 %lld\n", current_timestamp());
#endif

		#pragma omp for schedule(guided)
		for (size_t ii = 0; ii < 4*(nextlevelblocks/8); ii+=4) {
			offline_prg_oct(&b2[ii*2*expansion_stride], &b2[(ii*2+1)*expansion_stride], &b2[(ii*2+2)*expansion_stride], &b2[(ii*2+3)*expansion_stride],
							&b2[(ii*2+4)*expansion_stride], &b2[(ii*2+5)*expansion_stride], &b2[(ii*2+6)*expansion_stride], &b2[(ii*2+7)*expansion_stride],
							&a2[ii*BLOCKSIZE],  &a2[ii*BLOCKSIZE], &a2[(ii+1)*BLOCKSIZE], &a2[(ii+1)*BLOCKSIZE],
							&a2[(ii+2)*BLOCKSIZE], &a2[(ii+2)*BLOCKSIZE],&a2[(ii+3)*BLOCKSIZE], &a2[(ii+3)*BLOCKSIZE],
							fsso->keyL, fsso->keyR, fsso->keyL, fsso->keyR,
							fsso->keyL, fsso->keyR, fsso->keyL, fsso->keyR);
			a_bits[ii] = a2[ii*BLOCKSIZE] & 1;
			a_bits[ii+1] = a2[(ii+1)*BLOCKSIZE] & 1;
			a_bits[ii+2] = a2[(ii+2)*BLOCKSIZE] & 1;
			a_bits[ii+3] = a2[(ii+3)*BLOCKSIZE] & 1;
		}

		#pragma omp single
		for (size_t ii = 4*(nextlevelblocks/8); ii < thislevelblocks; ii++) {
			if ((ii+1)*2 <= nextlevelblocks) {
				offline_prg(&b2[ii*2*expansion_stride], &a2[ii*BLOCKSIZE], fsso->keyL);
				offline_prg(&b2[(ii*2+1)*expansion_stride], &a2[ii*BLOCKSIZE], fsso->keyR);
			} else if (ii*2+1 <= nextlevelblocks) {
				offline_prg(&b2[ii*2*expansion_stride], &a2[ii*BLOCKSIZE], fsso->keyL);
			}
			a_bits[ii] = a2[ii*BLOCKSIZE] & 1;
		}

#ifdef ORAM_PROFILE_SCHEDULING
		#pragma omp single nowait
		printf("END FSS OFFLINE LEVEL 0 %lld\n", current_timestamp());
#endif

		for (thislevel = fsso->startlevel +1; thislevel < fsso->endlevel; thislevel++) {
			if (readycount_cache < thislevel - fsso->startlevel) {
				while ((readycount_cache = floram_atomic_read(&fsso->readycount)) < thislevel - fsso->startlevel) floram_usleep(SLEEP_TIME);
			}

#ifdef ORAM_PROFILE_SCHEDULING
			#pragma omp single nowait
			printf("START FSS OFFLINE LEVEL %d %lld\n", thislevel,current_timestamp());
#endif

			thislevelblocks = nextlevelblocks;
			nextlevelblocks = (fsso->size + (1ll<<(fsso->endlevel - thislevel -1)) - 1) / (1ll<<(fsso->endlevel - thislevel -1));
			if (thislevel == fsso->endlevel -1) nextlevelblocks = fsso->size;

			advicebit_l = fsso->advicebits_l[thislevel - fsso->startlevel -1];
			advicebit_r = fsso->advicebits_r[thislevel - fsso->startlevel -1];

			z = &((uint64_t *)fsso->Z)[(thislevel - fsso->startlevel -1) * (BLOCKSIZE/sizeof(uint64_t))];

			t2 = b2; t = b; t_bits = b_bits;
			b2 = a2; b = a; b_bits = a_bits;
			a2 = t2; a = t; a_bits = t_bits;

			if (thislevel == fsso->endlevel - 1 && b2 == local_output) {
				expansion_stride = (BLOCKSIZE * fsso->blockmultiple);
			} else {
				expansion_stride = BLOCKSIZE;
			}

			#pragma omp for schedule(guided)
			for (size_t ii = 0; ii < 4*(nextlevelblocks/8); ii+=4) {
				a_bits[ii] = (a2[ii*BLOCKSIZE] & 1) ^ (b_bits[ii/2] & advicebit_l);
				a_bits[ii+1] = (a2[(ii+1)*BLOCKSIZE] & 1) ^ (b_bits[ii/2] & advicebit_r);
				a_bits[ii+2] = (a2[(ii+2)*BLOCKSIZE] & 1) ^ (b_bits[(ii+2)/2] & advicebit_l);
				a_bits[ii+3] = (a2[(ii+3)*BLOCKSIZE] & 1) ^ (b_bits[(ii+2)/2] & advicebit_r);

				if (b_bits[ii/2]) {
					#pragma omp simd aligned(a,z:16)
					for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
						a[ii*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= z[jj];
						a[(ii+1)*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= z[jj];
					}
				}
				if (b_bits[(ii+2)/2]) {
					#pragma omp simd aligned(a,z:16)
					for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
						a[(ii+2)*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= z[jj];
						a[(ii+3)*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= z[jj];
					}
				}

				offline_prg_oct(&b2[ii*2*expansion_stride], &b2[(ii*2+1)*expansion_stride], &b2[(ii*2+2)*expansion_stride], &b2[(ii*2+3)*expansion_stride],
								&b2[(ii*2+4)*expansion_stride], &b2[(ii*2+5)*expansion_stride], &b2[(ii*2+6)*expansion_stride], &b2[(ii*2+7)*expansion_stride],
								&a2[ii*BLOCKSIZE],  &a2[ii*BLOCKSIZE], &a2[(ii+1)*BLOCKSIZE], &a2[(ii+1)*BLOCKSIZE],
								&a2[(ii+2)*BLOCKSIZE],  &a2[(ii+2)*BLOCKSIZE], &a2[(ii+3)*BLOCKSIZE], &a2[(ii+3)*BLOCKSIZE],
								fsso->keyL, fsso->keyR, fsso->keyL, fsso->keyR,
								fsso->keyL, fsso->keyR, fsso->keyL, fsso->keyR);
			}

			#pragma omp single
			for (size_t ii = 4*(nextlevelblocks/8); ii < thislevelblocks ; ii++) {

				if (ii%2 == 0) {
					a_bits[ii] = (a2[ii*BLOCKSIZE] & 1) ^ (b_bits[ii/2] & advicebit_l);
				} else {
					a_bits[ii] = (a2[ii*BLOCKSIZE] & 1) ^ (b_bits[ii/2] & advicebit_r);
				}

				if (b_bits[ii/2]) {
					#pragma omp simd aligned(a,z:16)
					for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
						a[ii*(BLOCKSIZE/sizeof(uint64_t))+jj] ^= z[jj];
					}
				}
			
				if ((ii+1)*2 <= nextlevelblocks) {
					offline_prg(&b2[ii*2*expansion_stride], &a2[ii*BLOCKSIZE], fsso->keyL);
					offline_prg(&b2[(ii*2+1)*expansion_stride], &a2[ii*BLOCKSIZE], fsso->keyR);
				} else if (ii*2+1 <= nextlevelblocks) {
					offline_prg(&b2[ii*2*expansion_stride], &a2[ii*BLOCKSIZE], fsso->keyL);
				}
			}

#ifdef ORAM_PROFILE_SCHEDULING
		#pragma omp single nowait
		printf("END FSS OFFLINE LEVEL %d %lld\n", thislevel,current_timestamp());
#endif
		}

		while (floram_atomic_read(&fsso->readycount) < thislevel - fsso->startlevel) floram_usleep(SLEEP_TIME);

#ifdef ORAM_PROFILE_SCHEDULING
		#pragma omp single nowait
		printf("START FSS LEVEL %d %lld\n", thislevel,current_timestamp());
#endif

		thislevelblocks = nextlevelblocks;

		advicebit_l = fsso->advicebits_l[thislevel - fsso->startlevel -1];
		advicebit_r = fsso->advicebits_r[thislevel - fsso->startlevel -1];

		z = &((uint64_t *)fsso->Z)[(thislevel - fsso->startlevel -1) * (BLOCKSIZE/sizeof(uint64_t))];

		t2 = b2; t = b; t_bits = b_bits;
		b2 = a2; b = a; b_bits = a_bits;
		a2 = t2; a = t; a_bits = t_bits;
		
		#pragma omp for schedule(guided)
		for (size_t ii = 0; ii < thislevelblocks; ii++) {
			if (b == local_output) {
				if (ii%2 == 0) {
					a_bits[ii] = (a2[ii*BLOCKSIZE] & 1) ^ (b_bits[ii/2] & advicebit_l);
				} else {
					a_bits[ii] = (a2[ii*BLOCKSIZE] & 1) ^ (b_bits[ii/2] & advicebit_r);
				}

				if (b_bits[ii/2]) {
					#pragma omp simd aligned(b,a,z:16)
					for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
						b[ii*((BLOCKSIZE * fsso->blockmultiple)/sizeof(uint64_t))+jj] = a[ii*(BLOCKSIZE/sizeof(uint64_t))+jj] ^ z[jj];
					}
				} else {
					memcpy(&b[ii*((BLOCKSIZE * fsso->blockmultiple)/sizeof(uint64_t))], &a[ii*(BLOCKSIZE/sizeof(uint64_t))], BLOCKSIZE);
				}
			} else {
				if (ii%2 == 0) {
					a_bits[ii] = (a2[ii*(BLOCKSIZE * fsso->blockmultiple)] & 1) ^ (b_bits[ii/2] & advicebit_l);
				} else {
					a_bits[ii] = (a2[ii*(BLOCKSIZE * fsso->blockmultiple)] & 1) ^ (b_bits[ii/2] & advicebit_r);
				}

				if (b_bits[ii/2]) {
					#pragma omp simd aligned(b,a,z:16)
					for (uint8_t jj = 0; jj < BLOCKSIZE/sizeof(uint64_t); jj++) {
						a[ii*((BLOCKSIZE * fsso->blockmultiple)/sizeof(uint64_t))+jj] ^= z[jj];
					}
				}
			}
		}

		#pragma omp single
		if (b == local_output) memcpy(b_bits, a_bits, thislevelblocks*sizeof(bool));

		if (fsso->blockmultiple > 1) {
			#pragma omp for schedule(guided)
			for (size_t ii = 0; ii < 8*(thislevelblocks/8); ii+=8)  {
				for (size_t jj = 1; jj < fsso->blockmultiple; jj++) {
					// Note to self: this is ridiculous. Define a macro.
					// Further note to self: actually, the non-encapsulation of offline_prg_oct and offline_prg
					// is just as ridiculous, if not more so. There has to be a better way. TODO: find it.
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
				}
			}

			#pragma omp single
			for (size_t ii = 8*(thislevelblocks/8); ii < thislevelblocks ; ii++) {
				for (size_t jj = 1; jj < fsso->blockmultiple; jj++) {
					offline_prg(
						&local_output[(ii) * (BLOCKSIZE*fsso->blockmultiple) + (jj * BLOCKSIZE)],
						&local_output[(ii) * (BLOCKSIZE*fsso->blockmultiple) + ((jj-1) * BLOCKSIZE)],
						fsso->keyL
					);
				}
			}
		}

#ifdef ORAM_PROFILE_SCHEDULING
		#pragma omp single nowait
		printf("END FSS LEVEL %d %lld\n", thislevel,current_timestamp());
#endif
	}
}

void fss_offline_parallelizer(void* fss, fss_offline * fsso, void* indexp, void * local_output, void * local_bit_output, void* pd, fss_traverser_fn fn, fss_pusher_fn fn2, facb_fn cbfn, void* cbpass) {

	omp_set_nested(true);

	#pragma omp parallel num_threads(3)
	{
		//OpenMP seems to get along with obliv-c just fine, so long as obliv-c only uses the master thread.
		#pragma omp master
		{
			fn(fss, indexp);	
			if (*cbfn!=NULL) {
				cbfn(cbpass, NULL);
			}
		}
		

		#pragma omp single
		{
			#pragma omp task
			{
				if (omp_get_num_threads() > 1) fn2(fss, fsso, pd);
				else fn2(fss, fsso, NULL);
			}			

			#pragma omp task
			fss_offline_readblockvector(local_output, local_bit_output, fsso);
		}
	}
}

fss_offline * fss_offline_new(size_t size, size_t blockmultiple, size_t startlevel, uint8_t * keyL, uint8_t * keyR) {
	fss_offline * fsso = malloc(sizeof(fss_offline));
	fsso->size = size;
	fsso->blockmultiple = blockmultiple;
	fsso->startlevel = startlevel;
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

void fss_offline_free(fss_offline * fsso) {
	free(fsso->level_data);
	free(fsso->level_bits);
	free(fsso->advicebits_l);
	free(fsso->advicebits_r);
	free(fsso->Z);
	free(fsso->keyL);
	free(fsso->keyR);
	free(fsso);
}