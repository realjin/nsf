#include "reader_nsf.h"

#include <alisttpl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _NSF_DEBUG_ 1

alisttpl_struct_impl(id_map_entry);
alisttpl_struct_impl(doc_term);


id_map* nsf_create_id_map(char* fns[], int n)
{
	id_map* m = create_id_map_entry_alist();
	int i;
	
	for(i=0;i<n;i++)	{
		nsf_load_bow_idnsfid(m, fns[i]);
	}
	return m;
}


void nsf_load_bow_idnsfid(id_map* m, char* fn)
{
	id_map_entry* e;
	char line[100];
	FILE* f;

	f = fopen(fn, "r");

	while((fgets(line, sizeof(line), f))!=0)	{
		e = (id_map_entry*)malloc(sizeof(id_map_entry));
		sscanf(line, "%d %s", &e->id, &e->docid);

		add_id_map_entry(m, e);
	}

	fclose(f);

	//printf("map size now=%d\n", m->size);
}

//--------------ii-------------------------
ii* nsf_create_ii(char* fns[], int n)
{
	ii* ind;
	FILE* f;
	char line[100];
	__u32 tid;

	ii_term* ii_t;
	posting* ii_pst;

	doc_term* dt;
	doc_term_alist* dtlist;

	int i;

	int tmp = 0;

	ind = create_ii();
	tid = -1;

	dtlist = create_doc_term_alist();

#if 1
	printf("generate doc_term list\n");
#endif
	for(i=0;i<n;i++)	{
		f = fopen(fns[i], "r");

		while(fgets(line, sizeof(line), f)!=0)	{
			dt = (doc_term*)malloc(sizeof(doc_term));
			sscanf(line, "%d %d %d", &dt->did, &dt->tid, &dt->tf);

			add_doc_term(dtlist, dt);
#if 1
			if((tmp++)%100000==0)	{
				printf("in doc_term generation: round %d\n", tmp);
			}
#endif
			
		}

		fclose(f);
	}

#if 0
	printf("sort list\n");
#endif
	
	//nsf_sort_doc_term_by_tid(dtlist);


#if 1
	printf("generate index\n");
#endif

	//init ii using empty terms
	printf("1. init using empty terms\n");
	for(i=0;i<40000;i++)	{
		ii_t = ii_create_term();
		ii_t->tid = i;
		add_ii_term(ind, ii_t);
	}


	printf("2. adding\n");
	for(i=0;i<dtlist->size;i++)	{
		ii_pst = (posting*)malloc(sizeof(posting));
		ii_pst->did = dtlist->list[i]->did;
		ii_pst->tf = dtlist->list[i]->tf;
#if 1
		if(i%100000==1)	{
			printf("adding #%d dt\n", i);
		}
#endif
		add_posting(ind->list[dtlist->list[i]->tid]->postings, ii_pst);
	}

	
	
/*
	
	for(i=0;i<dtlist->size;i++)	{
		ii_pst = (posting*)malloc(sizeof(posting));
		ii_pst->did = dtlist->list[i]->did;
		ii_pst->tf = dtlist->list[i]->tf;

		if(dtlist->list[i]->tid != tid)	{ //doc in diff term
			if(tid!=-1)	{	// save last term if not first dt
				add_ii_term(ind, ii_t);
			}

			ii_t = ii_create_term();
			ii_t->tid = dtlist->list[i]->tid;

			tid = dtlist->list[i]->tid;
		}

		add_posting(ii_t->postings, ii_pst);
	}

	add_ii_term(ind, ii_t);
*/
	return ind;
}

//--------------fi-------------------------
fi* nsf_create_fi(char* fns[], int n)
{
	fi* ind;
	
	FILE* f;
	int nf;
	char line[100];
	int maxtid;
	int maxdid;

	doc_term* d_t;
	int i,j;
	di_doc* d;
	di_dterm* dt;
	ii_term* t;
	posting* pst;
	
	
	//find maxtid and maxdid to get doccol and termcol size
#if _NSF_DEBUG_
	printf("1. get doccol and termcol size: \n");
#endif
	maxtid = 0;
	maxdid = 0;
	for(nf=0;nf<n;nf++)	{
		f = fopen(fns[nf], "r");
		
		while(fgets(line, sizeof(line), f)!=0)	{
			d_t = (doc_term*)malloc(sizeof(doc_term));
			sscanf(line, "%d %d %d", &d_t->did,&d_t->tid,&d_t->tf);

			if(d_t->did>maxdid)	{
				maxdid = d_t->did;
			}
			if(d_t->tid>maxtid)	{
				maxtid = d_t->tid;
			}

		}
		fclose(f);
	}
#if _NSF_DEBUG_
	printf("maxdid = %d, maxtid = %d.\n", maxdid, maxtid);
#endif

	ind = fi_create_fi();
	ind->doccol->docs->size = maxdid;
	ind->termcol->terms->size = maxtid;


	//alloc space for di and ii
	printf("2. alloc space for di and ii.\n");
	for(i=0;i<maxdid;i++)	{
		d = di_create_doc();
		d->did = i+1;
		add_di_doc(ind->di, d);
	}
	for(i=0;i<maxtid;i++)	{
		t = ii_create_term();
		t->tid = i+1;
		add_ii_term(ind->ii, t);
	}

	//construct di and ii
	printf("3. construct di and ii.\n");
	for(nf=0;nf<n;nf++)	{
		f = fopen(fns[nf], "r");

		while(fgets(line, sizeof(line), f)!=0)	{
			d_t = (doc_term*)malloc(sizeof(doc_term));
			sscanf(line, "%d %d %d", &d_t->did,&d_t->tid,&d_t->tf);
			//printf("to add pst: %d %d %d\n", d_t->did,d_t->tid,d_t->tf);

			dt = (di_dterm*)malloc(sizeof(di_dterm));
			pst = (posting*)malloc(sizeof(posting));
			
			//save dterm in di
			dt->tid = d_t->tid;
			dt->tf = d_t->tf;
			add_di_dterm(ind->di->list[d_t->did-1]->terms, dt);

			//save posting in ii
			pst->did = d_t->did;
			pst->tf = d_t->tf;
			add_posting(ind->ii->list[d_t->tid-1]->postings, pst);
		}

		fclose(f);
	}

	return ind;

}

//--------------di-------------------------


di* nsf_create_di(char* fns[], int n)
{
}
/*
	 di* nsf_create_di(char* fns[], int n)
	 {
	 di* ind;

	 int i;
	 FILE* f;
	 __u32 did;
	 char line[100];
	 doc_term* dtpair;
	 di_doc* d;
	 di_dterm* dt;

	 ind = create_di();

	 for(i=0;i<n;i++)	{
	 f = fopen(fns[i], "r");

	 did = -1;
	 d = 0;

	 while( (fgets(line,sizeof(line), f)) !=0 ){
	 dtpair = (doc_term*)malloc(sizeof(doc_term));
	 sscanf(line, "%d %d %d", &dtpair->did, &dtpair->tid, &dtpair->tf);
#if 1
if(dtpair->did%1000 < 5)	{
printf("[progress] did=%d\n", dtpair->did);
}
#endif
if(dtpair->did!=did)	{	//if term in same doc
if(d)	{
di_add_doc(ind, d);
//printf("ind size=%d\n", ind->size);
}
//d = (di_doc*)malloc(sizeof(di_doc));
d = di_create_doc();
d->did = dtpair->did;
did = dtpair->did;
}

dt = (di_dterm*)malloc(sizeof(di_dterm));
dt->tid = dtpair->tid;
dt->tf = dtpair->tf;

di_add_dterm(d, dt);

free(dtpair);
}
if(d)	{
di_add_doc(ind, d);
//printf("ind size=%d\n", ind->size);
}

fclose(f);
}

return ind;
}
 */

//------------------------------------------doc terms
/* not used yet */
void nsf_sort_doc_term_by_tid(doc_term_alist* dtlist)
{
	int i,j;
	__u32 maxtid;
	int maxj;
	doc_term* dt;
	for(i=0;i<dtlist->size-1;i++)	{
#if 1
		//if(i%100==1)	{
		printf("sort dtlist by tid: round %d/%d\n", i, dtlist->size);
		//}
#endif
		maxj = 0;
		maxtid = dtlist->list[0];
		for(j=1;j<dtlist->size-i;j++)	{
			if(dtlist->list[j]->tid > maxtid)	{
				maxtid = dtlist->list[j]->tid;
				maxj = j;
			}
		}
		if(maxj != dtlist->size-i-1)	{
			dt = dtlist->list[maxj];
			dtlist->list[maxj] = dtlist->list[dtlist->size-i-1];
			dtlist->list[dtlist->size-i-1] = dt;
		}
	}
}


void nsf_load_doc_term(doc_term_alist* dlist, char* fn)
{
	doc_term* d;
	char line[100];
	FILE *f;
	f = fopen(fn, "r");

	while( (fgets(line,sizeof(line), f)) !=0 ){
		d = (doc_term*)malloc(sizeof(doc_term));
		//	printf("line=%s\n", line);
		sscanf(line, "%d %d %d", &d->did, &d->tid, &d->tf);
		add_doc_term(dlist, d);
	}

	fclose(f);
}


doc_term_alist* nsf_create_doc_term_list(char* fns[], int n)
{
	doc_term_alist* dlist = create_doc_term_alist();
	int i;

	for(i=0;i<n;i++)	{
		nsf_load_doc_term(dlist, fns[i]);
	}

	return dlist;

}
