#include <stdio.h>
#include <stdlib.h>
/*#include <stdarg.h>*/
/*#include <limits.h>*/
/*#include <ctype.h>*/
#include "bstrlib.h"
/*#include "bstraux.h"*/

static bstring dumpOut[16];
static int rot = 0;

static char * dumpBstring (const struct tagbstring * b) {
	rot = (rot + 1) % (unsigned)16;
	if (dumpOut[rot] == NULL) {
		dumpOut[rot] = bfromcstr ("");
		if (dumpOut[rot] == NULL) return "FATAL INTERNAL ERROR";
	}
	dumpOut[rot]->slen = 0;
	if (b == NULL) {
		bcatcstr (dumpOut[rot], "NULL");
	} else {
		static char msg[256];
		sprintf (msg, "%p", (void *)b);
		bcatcstr (dumpOut[rot], msg);

		if (b->slen < 0) {
			sprintf (msg, ":[err:slen=%d<0]", b->slen);
			bcatcstr (dumpOut[rot], msg);
		} else {
			if (b->mlen > 0 && b->mlen < b->slen) {
				sprintf (msg, ":[err:mlen=%d<slen=%d]", b->mlen, b->slen);
				bcatcstr (dumpOut[rot], msg);
			} else {
				if (b->mlen == -1) {
					bcatcstr (dumpOut[rot], "[p]");
				} else if (b->mlen < 0) {
					bcatcstr (dumpOut[rot], "[c]");
				}
				bcatcstr (dumpOut[rot], ":");
				if (b->data == NULL) {
					bcatcstr (dumpOut[rot], "[err:data=NULL]");
				} else {
					bcatcstr (dumpOut[rot], "\"");
					bcatcstr (dumpOut[rot], (const char *) b->data);
					bcatcstr (dumpOut[rot], "\"");
				}
			}
		}
	}
	return (char *) dumpOut[rot]->data;
}

int main (int argc, char * argv[]) {

    bstring b = bfromcstr ("Hello WORLDDDD");
    if (!b) {
        fprintf (stderr, "Out of memory");
    } else {
        puts ((char *) b->data);
    }

	bdestroy (b);

    b = bfromcstralloc (64, "123456789012345678901234567890123456789012345678901234567890123");
    if (b)
    {
        b->data[63] = 'x';
        puts ((char *) b->data);
        printf("dump is %s\n", dumpBstring(b));
    }
    else
    {
        puts("bfromcstralloc failed");
    }

	bdestroy (b);

    b = blk2bstr ("AWHOLEnewworld", sizeof("wholenewworld")-3);
    puts ((char *) b->data);

    char *cstr = bstr2cstr(b, '\0');
    puts(cstr);
    free(cstr);

    cstr = bstr2cstr(b, '0');
    puts(cstr);
    free(cstr);

    bstring copy = bstrcpy(b);
    printf("copy is %s\n", (char *) copy->data);

    bdestroy (b);
    b = bfromcstralloc (64, "123456789012345678901234567890123456789012345678901234567890123");

    bassign(copy, b);
    printf("copy is %s\n", (char *) copy->data);
    printf("b is %s\n", (char *) b->data);

    bdestroy (b);
    b = blk2bstr ("AWHOLEnewworld", sizeof("wholenewworld"));

    bassign(copy, b);
    printf("copy is %s\n", (char *) copy->data);
    printf("b is %s\n", (char *) b->data);

    bdestroy(b);
    bdestroy(copy);

    int i = 0;
    b = bfromcstralloc (64, "123456789012345678901234567890123456789012345678901234567890123");
    struct bstrList *blist = bsplit(b, '0');
    printf("made blist. qty is %d, mlen is %d. entry[0] is %s\n", blist->qty, blist->mlen, blist->entry[0]->data);
    for(i=0; i<blist->qty; i++)
    {
        printf("entry[%d] is %s\n", i, blist->entry[i]->data);
    }
    bstrListDestroy(blist);

    blist = bsplit(b, '\0');
    printf("made blist. qty is %d, mlen is %d. entry[0] is %s\n", blist->qty, blist->mlen, blist->entry[0]->data);
    for(i=0; i<blist->qty; i++)
    {
        printf("entry[%d] is %s\n", i, blist->entry[i]->data);
    }
    bstrListDestroy(blist);

    struct tagbstring split = bsStatic ("123");
    blist = bsplitstr(b, &split);
    printf("made blist. qty is %d, mlen is %d. entry[0] is %s\n", blist->qty, blist->mlen, blist->entry[0]->data);
    for(i=0; i<blist->qty; i++)
    {
        printf("entry[%d] is %s\n", i, blist->entry[i]->data);
    }
    bdestroy(b);
/*
 *
 *#define CHUNK 1024 [> read 1024 bytes at a time <]
 *    char buf[CHUNK];
 *    FILE *file;
 *    size_t nread;
 *
 *    file = fopen("./LICENSE", "r");
 *    if (file) {
 *        while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
 *            fwrite(buf, 1, nread, stdout);
 *        if (ferror(file)) {
 *            [> deal with error <]
 *        }
 *        fclose(file);
 *    }
 */


    FILE *lic_fd = fopen("./LICENSE", "r");

    struct bStream *bslic_fd = bsopen((bNread) fread, lic_fd);

    bstring lic_bstr = bread( (bNread) fread, lic_fd);

    printf("bstring is %s", lic_bstr->data);

    return 0;
}
