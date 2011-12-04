#include "reader_nsf.h"
#include <index.h>
#include <docvector.h>
#include <cluster.h>

//test clustering
int main()
{
	cluster_list_list* cll = create_cluster_list_list();
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

	printf("to save dv\n");

	dv_save_docs(docs, "nsf.dv");

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
