#define __OCR__
#include "ocr.h"

/* STREAM Headers */
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <sys/time.h> 

#ifndef STREAM_ARRAY_SIZE
#	define STREAM_ARRAY_SIZE	100
#endif

#ifdef NTIMES
#if NTIMES <= 1
#	define NTIMES	2
#endif
#endif

#ifndef NTIMES
#	define NTIMES	3
#endif

#ifndef OFFSET
#	define OFFSET	0
#endif

#define HLINE "-------------------------------------------------------------\n"

#ifndef MIN
#	define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#ifndef MAX
#	define MAX(x,y) ((x)>(y)?(x):(y))
#endif

#ifndef STREAM_TYPE
#	define STREAM_TYPE double
#endif

#ifndef NUM_OP
#	define NUM_OP 	4
#endif

static STREAM_TYPE	*a, *b, *c;

static double	avgtime[4] = {0}, maxtime[4] = {0},
				mintime[4] = {FLT_MAX,FLT_MAX,FLT_MAX,FLT_MAX};


static char *label[4] = {"Copy:      ", "Scale:     ", "Add:       ", "Triad:     "};

static double	bytes[4] = {
	2 * sizeof(STREAM_TYPE) * STREAM_ARRAY_SIZE,
	2 * sizeof(STREAM_TYPE) * STREAM_ARRAY_SIZE,
	3 * sizeof(STREAM_TYPE) * STREAM_ARRAY_SIZE,
	3 * sizeof(STREAM_TYPE) * STREAM_ARRAY_SIZE
};

extern double mysecond();
extern void checkSTREAMresults();
#ifdef TUNED
extern void tuned_STREAM_Copy();
extern void tuned_STREAM_Scale(STREAM_TYPE scalar);
extern void tuned_STREAM_Add();
extern void tuned_STREAM_Triad(STREAM_TYPE scalar);
#endif
#ifdef _OPENMP
extern int omp_get_num_threads();
#endif


ocrGuid_t copy(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	ssize_t j;
	STREAM_TYPE ** data = depv[0].ptr;
	u64 trial = paramv[0] + 3;

	// time and executing copy
	// c = a where a = data[0], c = data[2]
	data[trial][0] = mysecond();
	for (j = 0; j < STREAM_ARRAY_SIZE+OFFSET; j++) {
		data[2][j] = data[0][j];
	}
	data[trial][0] = mysecond() - data[trial][0];

	// repackaging guid
	ocrGuid_t * guids = (ocrGuid_t *) depv[0].ptr;
	ocrGuid_t * guidArray;
	ocrGuid_t guidArrayGuid;
	DBCREATE(&guidArrayGuid,(void **) &guidArray, sizeof(ocrGuid_t)*sizeof(depv), 0, NULL_GUID, NO_ALLOC);
	guidArray[0] = guids[0];
	guidArray[1] = guids[1];
	guidArray[2] = guids[2];
	guidArray[3] = guids[3];
	for (j = 0; j < NTIMES; j++) {
		guidArray[j+3] = guids[j+3];
	}
	PRINTF("FINISHED COPY\n");
	return guidArrayGuid;
}

ocrGuid_t scale(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	ssize_t j;
	STREAM_TYPE scalar = 3.0;
	STREAM_TYPE ** data = depv[0].ptr;
	u64 trial = paramv[0] + 3;

	// time and executing scale
	// b = scalar * c where b = data[0], c = data[1], scalar = 3.0
	data[trial][1] = mysecond();
	for (j = 0; j < STREAM_ARRAY_SIZE+OFFSET; j++) {
		data[0][j] = scalar * data[1][j];
	}
	data[trial][1] = mysecond() - data[trial][1];

	// repackaging guid
	ocrGuid_t * guids = (ocrGuid_t *) depv[0].ptr;
	ocrGuid_t * guidArray;
	ocrGuid_t guidArrayGuid;
	DBCREATE(&guidArrayGuid,(void **) &guidArray, sizeof(ocrGuid_t)*sizeof(depv), 0, NULL_GUID, NO_ALLOC);
	guidArray[0] = guids[0];
	guidArray[1] = guids[1];
	guidArray[2] = guids[2];
	guidArray[3] = guids[3];
	for (j = 0; j < NTIMES; j++) {
		guidArray[j+3] = guids[j+3];
	}
	PRINTF("Finished SCALE\n");
	return guidArrayGuid;
}

ocrGuid_t add(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	ssize_t j;
	STREAM_TYPE ** data = depv[0].ptr;
	u64 trial = paramv[0] + 3;

	// time and executing add
	// c = a + b where a = data[0], b = data[1], c = data[2]
	data[trial][2] = mysecond();
	for (j = 0; j < STREAM_ARRAY_SIZE+OFFSET; j++) {
		data[2][j] = data[0][j] + data[1][j];
	}
	data[trial][2] = mysecond() - data[trial][2];

	// repackaging guid
	ocrGuid_t * guids = (ocrGuid_t *) depv[0].ptr;
	ocrGuid_t * guidArray;
	ocrGuid_t guidArrayGuid;
	DBCREATE(&guidArrayGuid,(void **) &guidArray, sizeof(ocrGuid_t)*sizeof(depv), 0, NULL_GUID, NO_ALLOC);
	guidArray[0] = guids[0];
	guidArray[1] = guids[1];
	guidArray[2] = guids[2];
	guidArray[3] = guids[3];
	for (j = 0; j < NTIMES; j++) {
		guidArray[j+3] = guids[j+3];
	}
	PRINTF("Finished ADD\n");
	return guidArrayGuid;
}

ocrGuid_t triad(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	ssize_t j;
	STREAM_TYPE scalar = 3.0;
	STREAM_TYPE ** data = depv[0].ptr;
	u64 trial = paramv[0] + 3;

	// time and executing triad
	// a = b + scalar * c where a = data[0], b = data[1], c = data[2], scalar = 3.0
	data[trial][3] = mysecond();
	for (j = 0; j < STREAM_ARRAY_SIZE+OFFSET; j++) {
		data[0][j] = data[1][j] + scalar * data[2][j];
	}
	data[trial][3] = mysecond() - data[trial][3];

	// repackaging guid
	ocrGuid_t * guids = (ocrGuid_t *) depv[0].ptr;
	ocrGuid_t * guidArray;
	ocrGuid_t guidArrayGuid;
	DBCREATE(&guidArrayGuid,(void **) &guidArray, sizeof(ocrGuid_t)*sizeof(depv), 0, NULL_GUID, NO_ALLOC);
	guidArray[0] = guids[0];
	guidArray[1] = guids[1];
	guidArray[2] = guids[2];
	guidArray[3] = guids[3];
	for (j = 0; j < NTIMES; j++) {
		guidArray[j+3] = guids[j+3];
	}
	PRINTF("Finished TRIAD\n");
	return guidArrayGuid;
}

ocrGuid_t pipelineEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]){
	ssize_t j;
	ocrGuid_t * guids = (ocrGuid_t *) depv[0].ptr;
	ocrGuid_t doneEventGuid = guids[3+NTIMES];

	// DB packaging for first vector operation (COPY) 
	ocrGuid_t * guidArray;
	ocrGuid_t guidArrayGuid;
	DBCREATE(&guidArrayGuid,(void **) &guidArray, sizeof(ocrGuid_t)*(sizeof(depv)-1), 0, NULL_GUID, NO_ALLOC);
	guidArray[0] = guids[0];
	guidArray[1] = guids[1];
	guidArray[2] = guids[2];
	guidArray[3] = guids[3];
	for (j = 0; j < NTIMES; j++) {
		guidArray[j+3] = guids[j+3];
	}


	// COPY
	// EDT Template for copy
	ocrGuid_t copyEdtTemplateGuid;
	ocrEdtTemplateCreate(&copyEdtTemplateGuid, copy, 1, 1);

	// EDT for copy
	ocrGuid_t copyEdtGuid;
	ocrGuid_t copyOutput;
	ocrEdtCreate(&copyEdtGuid, copyEdtTemplateGuid, EDT_PARAM_DEF, paramv, EDT_PARAM_DEF, &guidArrayGuid,
		 EDT_PROP_NONE, NULL_GUID, &copyOutput);


	// SCALE
	// EDT Template for scale
	ocrGuid_t scaleEdtTemplateGuid;
	ocrEdtTemplateCreate(&scaleEdtTemplateGuid, scale, 1, 1);

	// EDT for scale
	ocrGuid_t scaleEdtGuid;
	ocrGuid_t scaleOutput;
	ocrEdtCreate(&scaleEdtGuid, scaleEdtTemplateGuid, EDT_PARAM_DEF, paramv, EDT_PARAM_DEF, &copyOutput,
		EDT_PROP_NONE, NULL_GUID, &scaleOutput);


	// ADD
	// EDT Template for add
	ocrGuid_t addEdtTemplateGuid;
	ocrEdtTemplateCreate(&addEdtTemplateGuid, add, 1, 1);

	// EDT for add
	ocrGuid_t addEdtGuid;
	ocrGuid_t addOutput;
	ocrEdtCreate(&addEdtGuid, addEdtTemplateGuid, EDT_PARAM_DEF, paramv, EDT_PARAM_DEF, &scaleOutput,
		EDT_PROP_NONE, NULL_GUID, &addOutput);


	// TRIAD
	// EDT Template for triad
	ocrGuid_t triadEdtTemplateGuid;
	ocrEdtTemplateCreate(&triadEdtTemplateGuid, triad, 1, 1);

	// EDT for triad
	ocrGuid_t triadEdtGuid;
	ocrGuid_t triadOutput;
	ocrEdtCreate(&triadEdtGuid, triadEdtTemplateGuid, EDT_PARAM_DEF, paramv, EDT_PARAM_DEF, &addOutput,
		EDT_PROP_NONE, NULL_GUID, &triadOutput);

	// Event triggering end of pipeline and iteration
	ocrAddDependence(triadOutput, doneEventGuid, 0, DB_MODE_RO);
	 if (paramv[0] + 1 == paramv[1]) {
		ocrAddDependence(doneEventGuid, paramv[3], 0, DB_MODE_RO);
	}
	return NULL_GUID;
}



ocrGuid_t iterEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	// Iteration Done Event
	ssize_t j;
	ocrGuid_t iterDoneEventGuid;
	ocrEventCreate(&iterDoneEventGuid, OCR_EVENT_ONCE_T, false);


	// DB setup for pipeline 
	ocrGuid_t * guids = (ocrGuid_t *) depv[0].ptr;
	ocrGuid_t * guidArray;
	ocrGuid_t guidArrayGuid;
	DBCREATE(&guidArrayGuid,(void **) &guidArray, sizeof(ocrGuid_t)*(sizeof(depv)+1), 0, NULL_GUID, NO_ALLOC);
	guidArray[0] = guids[0];
	guidArray[1] = guids[1];
	guidArray[2] = guids[2];
	for (j = 0; j < NTIMES; j++) {
		guidArray[j+3] = guids[j+3];
	}
	guidArray[3+NTIMES] = iterDoneEventGuid;


	// EDT Template for pipeline
	ocrGuid_t pipeTemplateGuid;
	ocrEdtTemplateCreate(&pipeTemplateGuid, pipelineEdt, sizeof(paramv), 1);

	// EDT for pipeline
	ocrGuid_t pipeGuid;
	ocrEdtCreate(&pipeGuid, pipeTemplateGuid, EDT_PARAM_DEF, paramv, EDT_PARAM_DEF, &guidArrayGuid, 
		EDT_PROP_NONE, NULL_GUID, NULL);


	// Next iteration setup
	u64 i = paramv[0];
	u64 nbIt = paramv[1];
	i++;
	PRINTF("Pushing iteration %lu\n", i);
	if (i < nbIt) {
		ocrGuid_t ndepv[1];
		ndepv[0] = iterDoneEventGuid;
		paramv[0] = i;

		ocrGuid_t nextIterTemplateGuid;
		ocrEdtTemplateCreate(&nextIterTemplateGuid, iterEdt, sizeof(paramv), 1);

		ocrGuid_t nextIterGuid;
		ocrEdtCreate(&nextIterGuid, nextIterTemplateGuid, EDT_PARAM_DEF, paramv, EDT_PARAM_DEF, ndepv,
			EDT_PROP_NONE, NULL_GUID, NULL);
	}
	//PRINTF("ITR END\n");
	return NULL_GUID;
}

ocrGuid_t checkResultsEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	STREAM_TYPE ** guids = (STREAM_TYPE **) depv[0].ptr;
	STREAM_TYPE * a = guids[0];
	STREAM_TYPE * b = guids[1];
	STREAM_TYPE * c = guids[2];
	STREAM_TYPE aj,bj,cj,scalar;
	STREAM_TYPE aSumErr,bSumErr,cSumErr;
	STREAM_TYPE aAvgErr,bAvgErr,cAvgErr;
	double epsilon;
	ssize_t j;
	int k, ierr, err;

	/* reproduce initialization */
	aj = 1.0;
	bj = 2.0;
	cj = 0.0;
	/* a[] is modified during timing check */
	aj = 2.0E0 * aj;
	/* now execute timing loop */
	scalar = 3.0;
	for (k = 0; k < NTIMES; k++) {
		cj = aj;
		bj = scalar * cj;
		cj = aj + bj;
		aj = bj + scalar * cj;
	}

	/* accumulate deltas between observed and expected results */
	aSumErr = 0.0;
	bSumErr = 0.0;
	cSumErr = 0.0;
	for (j = 0; j < STREAM_ARRAY_SIZE; j++) {
		aSumErr += abs(a[j] - aj);
		bSumErr += abs(b[j] - bj);
		cSumErr += abs(c[j] - cj);
		// if (j == 417) PRINTF("Index 417: c[j]: %f, cj: %f\n",c[j],cj);	// MCCALPIN
	}
	aAvgErr = aSumErr / (STREAM_TYPE) STREAM_ARRAY_SIZE;
	bAvgErr = bSumErr / (STREAM_TYPE) STREAM_ARRAY_SIZE;
	cAvgErr = cSumErr / (STREAM_TYPE) STREAM_ARRAY_SIZE;

	if (sizeof(STREAM_TYPE) == 4) {
		epsilon = 1.e-6;
	}
	else if (sizeof(STREAM_TYPE) == 8) {
		epsilon = 1.e-13;
	}
	else {
		PRINTF("WEIRD: sizeof(STREAM_TYPE) = %lu\n",sizeof(STREAM_TYPE));
		epsilon = 1.e-6;
	}

	err = 0;
	if (abs(aAvgErr/aj) > epsilon) {
		err++;
		PRINTF ("Failed Validation on array a[], AvgRelAbsErr > epsilon (%e)\n",epsilon);
		PRINTF ("     Expected Value: %e, AvgAbsErr: %e, AvgRelAbsErr: %e\n",aj,aAvgErr,abs(aAvgErr)/aj);
		ierr = 0;
		for (j = 0; j < STREAM_ARRAY_SIZE; j++) {
			if (abs(a[j]/aj-1.0) > epsilon) {
				ierr++;
#ifdef VERBOSE
				if (ierr < 10) {
					PRINTF("         array a: index: %ld, expected: %e, observed: %e, relative error: %e\n",
						j,aj,a[j],abs((aj-a[j])/aAvgErr));
				}
#endif
			}
		}
		PRINTF("     For array a[], %d errors were found.\n",ierr);
	}
	if (abs(bAvgErr/bj) > epsilon) {
		err++;
		PRINTF ("Failed Validation on array b[], AvgRelAbsErr > epsilon (%e)\n",epsilon);
		PRINTF ("     Expected Value: %e, AvgAbsErr: %e, AvgRelAbsErr: %e\n",bj,bAvgErr,abs(bAvgErr)/bj);
		PRINTF ("     AvgRelAbsErr > Epsilon (%e)\n",epsilon);
		ierr = 0;
		for (j=0; j<STREAM_ARRAY_SIZE; j++) {
			if (abs(b[j]/bj-1.0) > epsilon) {
				ierr++;
#ifdef VERBOSE
				if (ierr < 10) {
					PRINTF("         array b: index: %ld, expected: %e, observed: %e, relative error: %e\n",
						j, bj, b[j], abs((bj-b[j])/bAvgErr));
				}
#endif
			}
		}
		PRINTF("     For array b[], %d errors were found.\n",ierr);
	}
	if (abs(cAvgErr/cj) > epsilon) {
		err++;
		PRINTF ("Failed Validation on array c[], AvgRelAbsErr > epsilon (%e)\n",epsilon);
		PRINTF ("     Expected Value: %e, AvgAbsErr: %e, AvgRelAbsErr: %e\n",cj,cAvgErr,abs(cAvgErr)/cj);
		PRINTF ("     AvgRelAbsErr > Epsilon (%e)\n",epsilon);
		ierr = 0;
		for (j = 0; j < STREAM_ARRAY_SIZE; j++) {
			if (abs(c[j]/cj-1.0) > epsilon) {
				ierr++;
#ifdef VERBOSE
				if (ierr < 10) {
					PRINTF("         array c: index: %ld, expected: %e, observed: %e, relative error: %e\n",
						j,cj,c[j],abs((cj-c[j])/cAvgErr));
				}
#endif
			}
		}
		PRINTF("     For array c[], %d errors were found.\n",ierr);
	}
	if (err == 0) {
		PRINTF ("Solution Validates: avg error less than %e on all three arrays\n",epsilon);
	}
#ifdef VERBOSE
	PRINTF ("Results Validation Verbose Results: \n");
	PRINTF ("	Expected a(1), b(1), c(1): %-10.2f %-10.2f %-10.2f \n", aj, bj, cj);
	PRINTF ("	Observed a(1), b(1), c(1): %-10.2f %-10.2f %-10.2f \n", a[1], b[1], c[1]);
	PRINTF ("	Rel Errors on a, b, c    : %-10.2e %-10.2e %-10.2e \n", abs(aAvgErr/aj), abs(bAvgErr/bj), abs(cAvgErr/cj));
#endif
	return NULL_GUID;
}

ocrGuid_t terminateEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	PRINTF("OCR_SHUTDOWN\n");
	ocrShutdown(); 
	return NULL_GUID;
}

ocrGuid_t mainEdt(u32 paramc, u64* paramv, u32 depc, ocrEdtDep_t depv[]) {
	// DB setup for a, b, c
	ssize_t j, k;
	ocrGuid_t aGuid;
	ocrGuid_t bGuid;
	ocrGuid_t cGuid;
	DBCREATE(&aGuid, (void **) &a, sizeof(STREAM_TYPE)*(STREAM_ARRAY_SIZE+OFFSET), 0, NULL_GUID, NO_ALLOC);
	DBCREATE(&bGuid, (void **) &b, sizeof(STREAM_TYPE)*(STREAM_ARRAY_SIZE+OFFSET), 0, NULL_GUID, NO_ALLOC);
	DBCREATE(&cGuid, (void **) &c, sizeof(STREAM_TYPE)*(STREAM_ARRAY_SIZE+OFFSET), 0, NULL_GUID, NO_ALLOC);
	for (j = 0; j < STREAM_ARRAY_SIZE+OFFSET; j++) {
		a[j] = 1.0;
		b[j] = 2.0;
		c[j] = 0.0;
	}
	// Event signalling last EDT has finished 
	ocrGuid_t lastEDTDoneEventGuid;
	ocrEventCreate(&lastEDTDoneEventGuid, OCR_EVENT_ONCE_T, false);


	// Paramv setup where paramv[0] = current iteration, paramv[1] = max iteration, paramv[2] = Size of DB
	u64 nparamv[4];
	nparamv[0] = 0;
	nparamv[1] = NTIMES;
	nparamv[2] = STREAM_ARRAY_SIZE+OFFSET;
	nparamv[3] = lastEDTDoneEventGuid;


	// DB packaging containing a, b, c, and timing DBs
	STREAM_TYPE ** guidArray;
	ocrGuid_t guidArrayGuid;
	DBCREATE(&guidArrayGuid,(void **) &guidArray, sizeof(ocrGuid_t)*4, 0, NULL_GUID, NO_ALLOC);
	guidArray[0] = a;
	guidArray[1] = b;
	guidArray[2] = c;
	for (j = 0; j < NTIMES; j++) {
		ocrGuid_t timingGuid;
		double  * timingArray;
		DBCREATE(&timingGuid, (void **) &timingArray, sizeof(double)*NUM_OP, 0, NULL_GUID, NO_ALLOC);
		for (k = 0; k < NUM_OP; k++) {
			timingArray[k] = 0;
		}
		guidArray[3 + j] = timingArray;
	}


	// EDT Template for iterator
	ocrGuid_t iterTemplateGuid;
	ocrEdtTemplateCreate(&iterTemplateGuid, iterEdt, sizeof(nparamv), 1);

	// EDT for iterator
	ocrGuid_t iterGuid;
	ocrEdtCreate(&iterGuid, iterTemplateGuid, EDT_PARAM_DEF, nparamv, EDT_PARAM_DEF, &guidArrayGuid,
		EDT_PROP_NONE, NULL_GUID, NULL);

	// EDT Template for check results
	ocrGuid_t checkResultsTemplateGuid;
	ocrEdtTemplateCreate(&checkResultsTemplateGuid, checkResultsEdt, 0, 1);

	// EDT for check results
	ocrGuid_t checkResultsGuid;
	ocrGuid_t checkResultsOutput;
	ocrEdtCreate(&checkResultsGuid, checkResultsTemplateGuid, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, &lastEDTDoneEventGuid,
		EDT_PROP_NONE, NULL_GUID, &checkResultsOutput);

	// EDT Template for terminate
	ocrGuid_t terminateTemplateGuid;
	ocrEdtTemplateCreate(&terminateTemplateGuid, terminateEdt, 0, 1);

	// EDT for terminate
	ocrGuid_t terminateGuid;
	ocrEdtCreate(&terminateGuid, terminateTemplateGuid, EDT_PARAM_DEF, NULL, EDT_PARAM_DEF, &checkResultsOutput,
		EDT_PROP_FINISH, NULL_GUID, NULL);
	return NULL_GUID;
}

#include <sys/time.h>
double mysecond() {
	struct timeval tp;
	struct timezone tzp;
	int i;
	i = gettimeofday(&tp,&tzp);
	return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}