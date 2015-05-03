#ifndef _INC_STRING_

#define STR_LEN 40


/********* defined *********/
char *  strcat(s, append);
char *  strcpy(to, from);
uint    strlen(str);


char *  strchr(s, c);
uint    strcmp(s1, s2);
char *  strncat(dst, src, n);
int     strncmp(s1, s2, n);
char *  strncpy(dst, src, n);
char *  strpbrk(s, c);
char *  strtok(s, delim);
char *  strlwr(s);
char *  strupr(s);
int     strcasecmp(s1, s2);
int     strncasecmp(s1, s2, n);
char *  strsep(stringp, delim);

/****** to define **********/
/*
size_t  strcspn(const char *_s1, const char *_s2);
char *  strrchr(const char *_s, int _c);
size_t  strspn(const char *_s1, const char *_s2);
char *  strstr(const char *_s1, const char *_s2);
size_t  strxfrm(char *_s1, const char *_s2, size_t _n);

char *  stpcpy(char *_dest, const char *_src);
*/

/*** impossible or unnecessary to define ***/
/*
char *  strerror(int _errcode);
void *  memchr(const void *_s, int _c, size_t _n);
int memcmp(const void *_s1, const void *_s2, size_t _n);
void *  memcpy(void *_dest, const void *_src, size_t _n);
void *  memmove(void *_s1, const void *_s2, size_t _n);
void *  memset(void *_s, int _c, size_t _n);
int strcoll(const char *_s1, const char *_s2);
void *  memccpy(void *_to, const void *_from, int c, size_t n);
char *  rindex(const char *_string, int _c);
int stricmp(const char *_s1, const char *_s2);
int strnicmp(const char *_s1, const char *_s2, size_t _n);
int bcmp(const void *_ptr1, const void *_ptr2, int _length);
void *  bcopy(const void *_a, void *_b, size_t _len);
void *  bzero(void *ptr, size_t _len);
int ffs(int _mask);
char *  index(const char *_string, int _c);
char *  strdup(const char *_s);
*/

#define _INC_STRING_

#endif
