#include "reader_nsf.h"
#include <index.h>
#include <docvector.h>

#include <object.h>
#include <cluster.h>
#include <hierarchical.h>
#include <similarity.h>


//test hac()
static object_space* create_object_space_from_dvlist(dv_docs* docs)
{
	dv_doc* d;	
	int i,j;

	object_space* space;
	object* o;
	object_att* att;

	space = (object_space*)malloc(sizeof(object_space));
	space->dim = 30800;
	space->all = (cluster*)docs;


	/*
	//space = obj_create_obj_space(ind->terms->size);
	
	//free ind
	fi_destroy_fi(ind);
	printf("ind freed!\n");

	for(i=0;i<docs->size;i++)	{
		d = docs->list[i];
		o = obj_create_obj();
		
		o->id = d->did;

		for(j=0;j<d->vector->size;j++)	{
			att = (object_att*)malloc(sizeof(object_att));
			att->id = d->vector->list[j]->id;
			att->v = d->vector->list[j]->v;

			add_object_att(o->atts, att);
		}
		
		if(i%1000==1)	{
			printf("craete obj space from fi and dvdocs: adding object %d\n", i);
		}
		add_object(space->all, o);
	}
	*/

	return space;
}

//test hac
int main()
{

	int i,j;
	//fi* ind;
	dv_docs* docs;

	//printf("a. loading fi\n");
	//ind = load_fi("nsf");

	printf("b. loading dv\n");
	docs = dv_load_docs("nsf.dv");


	//for(i=0;i<docs->size;i++)	{
		//for(j=0;j<docs->list[i]->vector->size;j++)	{
			//printf("dv att %d: %f\n", docs->list[i]->vector->list[j]->id, docs->list[i]->vector->list[j]->v);
	//		printf("dv att 0: %d - %f\n", docs->list[i]->vector->list[0]->id, docs->list[i]->vector->list[0]->v);
			//if(i>20)	break;
		//}
	//}
	

	printf("c. creating object space\n");
	object_space* space = create_object_space_from_dvlist(docs);

	printf("d. clustering\n");
	hier_clustering(space, csim_nearest_nb);



}

//test clustering
int main7()
{
	cluster_list_list* cll = create_cluster_list_alist();
}

//test docvector loading
int main6()
{
	dv_docs* docs;
	docs = dv_load_docs("nsf.dv");
	printf("size=%d\n", docs->size);
}
//test docvector creation(from fi) and saving
int main5()
{

	fi* ind;

	dv_docs* docs;

	printf("sizeof double=%d\n", sizeof(double));

	ind = load_fi("nsf");

	printf("dsize=%d\n", ind->docs->size);

	printf("to create dvlist\n");

	docs = dv_create_dv_from_fi(ind);

	printf("dvlist creation complete! size=%d\n", docs->size);
/*
	printf("to save dv\n");

	dv_save_docs(docs, "nsf.dv");
	*/

}

//test load and resave
int main4()
{
	fi* ind;
	ind = load_fi("nsf");
	save_fi(ind, "nsfcopy");
}

//test save fi
int main3()
{
	fi* ind;

	FILE* f;
	char line[100];
	int n;
	char* fns[10];

	f = popen("find ../dataset/nsf/bow -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	//f = popen("find ../dataset/nsf/tmp -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	n = 0;
	while((fgets(line,sizeof(line),f))!=0)	{
		fns[n] = (char*)malloc(sizeof(line));
		memcpy(fns[n], line, strlen(line)-1);
		fns[n][strlen(line)-1]=0;
		printf("%s\n",fns[n]);
		n++;
#if 0
		if(n==1)	{
			break;
		}
#endif
	}
	pclose(f);

	ind = nsf_create_fi(fns, n);	
	save_fi(ind, "nsf");

}

int main2()
{
	di* ind;

	FILE* f;
	char line[100];
	int n;
	char* fns[10];

	f = popen("find ../dataset/nsf/bow -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	//f = popen("find ../dataset/nsf/tmp -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	n = 0;
	while((fgets(line,sizeof(line),f))!=0)	{
		fns[n] = (char*)malloc(sizeof(line));
		memcpy(fns[n], line, strlen(line)-1);
		fns[n][strlen(line)-1]=0;
		printf("%s\n",fns[n]);
		n++;
#if 0
		if(n==1)	{
			break;
		}
#endif
	}
	pclose(f);

	ind = nsf_create_di(fns, n);
	printf("saving di\n");
	save_di(ind, "main.di");
	//save_ii();
	printf("end\n");
}

int main1()
{
	ii* ind;

	FILE* f;
	char line[100];
	int n;
	char* fns[10];

	f = popen("find ../dataset/nsf/bow -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	//f = popen("find ../dataset/nsf/tmp -type f -printf '%T@ %p\n' | grep \"docwords\" | sort -k 2 -n | sed 's/^[^ ]* //'", "r");
	n = 0;
	while((fgets(line,sizeof(line),f))!=0)	{
		fns[n] = (char*)malloc(sizeof(line));
		memcpy(fns[n], line, strlen(line)-1);
		fns[n][strlen(line)-1]=0;
		printf("%s\n",fns[n]);
		n++;
#if 0
		if(n==1)	{
			break;
		}
#endif
	}
	pclose(f);


	ind = nsf_create_ii(fns, n);
	printf("saving ii\n");
	save_ii(ind, "main.ii");
	//save_ii();
	printf("end\n");
}
