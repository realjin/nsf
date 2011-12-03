#include <alisttpl.h>

#include <index.h>


typedef struct _idmap_entry	{
	uint id;
	char docid[20];
}id_map_entry;
alisttpl_struct(id_map_entry);
typedef id_map_entry_alist id_map;

typedef struct _doc_term	{
	uint did;
	uint tid;
	uint tf;
}doc_term;
alisttpl_struct(doc_term);



id_map* nsf_create_id_map(char* fns[], int n);
void nsf_load_bow_idnsfid(id_map* m, char* fn);

void nsf_load_doclist(char* fn);

//----create ii from nsf file----
ii* nsf_create_ii(char* fns[], int n);
void nsf_sort_doc_term_by_tid(doc_term_alist* dtlist);

//----create di from nsf file----
di* nsf_create_di(char* fns[], int n);
void nsf_load_doc_term(doc_term_alist* dlist, char* fn);
doc_term_alist* nsf_create_doc_term_list(char* fns[], int n);
