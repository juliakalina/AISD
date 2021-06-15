#ifndef __INFO_H_INCLUDED__
#define __INFO_H_INCLUDED__

typedef struct info_t
{
	float x;
	float y;
	char* msg;
} info_t;

info_t InfoMakeCopy(const info_t* info);
void InfoPrint(const info_t* info);
void InfoDelete(info_t* info);

#endif /* !__INFO_H_INCLUDED__ */
