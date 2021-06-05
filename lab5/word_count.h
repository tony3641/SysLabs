//Author: Tony Han
//Date: 4/11/2021

#ifndef _WORD_COUNT_H
#define _WORD_COUNT_H

void word_count(int m, int r, char **text);

struct mapper_msg
{
    int r;
    char *text;
    char *save_ptr;
};

struct reducer_msg{
	int m;
	int id;
};


#endif
