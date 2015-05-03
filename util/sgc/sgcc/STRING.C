/************************************************************************
 *                      GENESIS STRING.C                                *
 *                                                                      *
 *                         by Kaneda                                    *
 *                                                                      *
 * Based on DJCPP libraries sources (C) 1993 DJ Delorie                 *
 * Copyright (c) 1990 The Regents of the University of California.      *
 *                                                                      *
 * It seems a lot of (c) DJ Delorie are first (c) R.U.C, so i only      *
 * copyrighted (c) DJ Delorie                                           *
 * R.U.C, mail me if you aren't agree                                   *
 *                                                                      *
 * History                                                              *
 *      ??Nov98         First try                                       *
 *      09Jan99         Pointers modif doesn't work...ROM is ReadOnly   *
 *                      Must use array of char...                       *
 *                      STR_LEN define the max lenght of returned value *
 *                                                                      *
 ************************************************************************/

#include "genesis.h"
#include "string.h"

/*********** for str comparaison ****************/
static uchar charmap[] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
	'\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
	'\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
	'\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
	'\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
	'\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
	'\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
	'\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
	'\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
	'\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
	'\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
	'\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
	'\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
	'\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
	'\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
	'\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
	'\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
	'\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
	'\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};



/**************** STRCAT ************************
 * Return "sappend"                             *
 *                                              *
 * Kaneda Jan/99                                *
 ************************************************/
char *strcat(s, append)
char s[];
char append[];
{

        char save[STR_LEN];
        uint i,j;

        for (i=0; i<strlen(s); i++)
               save[i]=s[i];

        for (j=0; j<strlen(append); j++)
                save[j+i] = append[i];

        return (save);
}


/**************** STRCPY ************************
 * Copy string FROM into string TO              *
 *                                              *
 * Kaneda Jan/99                                *
 ************************************************/
char *strcpy(to, from)
char to[];
char from[];
{
        uint i;

        for (i=0; i<strlen(from); i++)
                to[i]=from[i];

        return(to);
}


/**************** STRLEN ************************
 * Return the length of the string              *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
uint strlen(str)
char *str;
{
        register char *s;

	for (s = str; *s; ++s);
	return(s - str);
}







/**************** STRCHR ************************
 * Look for the char c in s                     *
 * Return the first position it find            *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strchr(s,c)
char *s;
int c;
{
  while (*s)
  {
    if (*s == c)
    {
      return (char *)s;
    }
    s++;
  }
  if (c == 0)
    return (char *)s;
  return (char *)0;
}


/**************** STRCMP ************************
 * Compare 2 strings                            *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
uint strcmp(s1, s2)
char *s1, *s2;
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
        return (*(uchar *)s1 - *(uchar *)--s2);
}



/***************** STRNCAT **********************
 * Concatenate src on the end of dst.           *
 * At most n+1 bytes being appended.            *
 * Return dst.                                  *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strncat(dst, src, n)
char *dst;
char *src;
uint n;
{
        char *d = dst;
        char *s = src;

	if (n != 0)
        {
		while (*d != 0)
			d++;
		do
                {
			if ((*d = *s++) == 0)
				break;
			d++;
                } while (--n != 0);

                *d = 0;
	}

        return (dst);
}

/***************** STRNCMP **********************
 * Compare 2 strings on at most N+1 bytes       *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
int strncmp(s1, s2, n)
char *s1, *s2;
uint n;
{

	if (n == 0)
		return (0);
	do
        {
		if (*s1 != *s2++)
                        return (*(uchar *)s1 - *(uchar *)--s2);
		if (*s1++ == 0)
			break;
	} while (--n != 0);

        return (0);
}

/***************** STRNCPY **********************
 * Copy src to dst, truncating or null-padding  *
 * to always copy n bytes.                      *
 * Return dst.                                  *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strncpy(dst, src, n)
char *dst;
char *src;
uint n;
{
        char *d = dst;
        char *s = src;

        if (n != 0)
        {
                do
                {
                        if ((*d++ = *s++) == 0)
                        {
				/* NUL pad the remaining n-1 bytes */
				while (--n != 0)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}
	return (dst);
}


/***************** STRPBRK **********************
 * Find the first occurrence in s1 of a         *
 * character in s2 (excluding NUL).             *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strpbrk(s1, s2)
char *s1, *s2;
{
        char *scanp;
        int c, sc;

	while ((c = *s1++) != 0) {
		for (scanp = s2; (sc = *scanp++) != 0;)
			if (sc == c)
				return ((char *)(s1 - 1));
	}
	return (NULL);
}

/***************** STRTOK ***********************
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strtok(s, delim)
char *s;
char *delim;
{
	register char *spanp;
	register int c, sc;
	char *tok;
        char *last;

	if (s == NULL && (s = last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

/***************** STRSEP ***********************
 * Get next token from string *stringp, where   *
 * tokens are nonempty strings separated by     *
 * characters from delim.                       *
 *                                              *
 * Writes NULs into the string at *stringp to   *
 * end tokens.                                  *
 * delim need not remain constant from call to  *
 * call.                                        *
 * On return, *stringp points past the last NUL *
 * written (if there might be further tokens),  *
 * or is NULL (if there are definitely no more  *
 * tokens).                                     *
 *                                              *
 * If *stringp is NULL, strtoken returns NULL.  *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strsep(stringp, delim)
char **stringp;
char *delim;
{
	register char *s;
        register char *spanp;
	register int c, sc;
	char *tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}



/***************** STRLWR ***********************
 * Convert a string in a Lowercase string       *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strlwr(s)
char *s;
{
  char *p = s;

  while (*s)
  {
    if ((*s >= 'A') && (*s <= 'Z'))
      *s += 'a'-'A';
    s++;
  }
  return p;
}

/***************** STRUPR ***********************
 * Convert a string in a Uppercase string       *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
char *strupr(s)
char *s;
{
  char *p = s;
  while (*s)
  {
    if ((*s >= 'a') && (*s <= 'z'))
      *s += 'A'-'a';
    s++;
  }
  return p;
}


/**************** STRCASECMP ********************
 * Compare the case of 2 strings                *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
int strcasecmp(s1, s2)
char *s1, *s2;
{
        register uchar *cm = charmap,
                       *us1 = (uchar *)s1,
                       *us2 = (uchar *)s2;

	while (cm[*us1] == cm[*us2++])
		if (*us1++ == '\0')
			return (0);
	return (cm[*us1] - cm[*--us2]);
}


/**************** STRNCASECMP *******************
 * Compare the case of 2 strings on their       *
 * first n bytes                                *
 *                                              *
 *      (C) 1993 DJ Delorie                     *
 ************************************************/
int strncasecmp(s1, s2, n)
char *s1, *s2;
uint n;
{
	if (n != 0) {
                register uchar *cm = charmap,
                               *us1 = (uchar *)s1,
                               *us2 = (uchar *)s2;

		do {
			if (cm[*us1] != cm[*us2++])
				return (cm[*us1] - cm[*--us2]);
			if (*us1++ == '\0')
				break;
		} while (--n != 0);
	}
	return (0);
}



