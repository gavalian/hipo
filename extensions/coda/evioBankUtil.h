
/* evioBankUtil.h - evio bank handling routines (Sergey Boyarinov) */

#ifndef __EVIOBANKUTIL_h__
#define __EVIOBANKUTIL_h__

#define GET_PUT_INIT \
  unsigned char *b08, *b08out, *b08save; \
  unsigned short *b16, *b16out; \
  unsigned int *b32, *b32out; \
  unsigned long long *b64, *b64out; \
  float *f32, *f32out; \
  double *f64, *f64out

#define GET8(ret_val) \
  ret_val = *b08++

#define GET16(ret_val) \
  b16 = (unsigned short *)b08; \
  ret_val = *b16; \
  b08+=2

#define GET32(ret_val) \
  b32 = (unsigned int *)b08; \
  ret_val = *b32; \
  b08+=4

#define GET64(ret_val) \
  b64 = (unsigned long long *)b08; \
  ret_val = *b64; \
  b08+=8

#define GETF32(ret_val) \
  f32 = (float *)b08; \
  ret_val = *f32; \
  b08+=4

#define GETF64(ret_val) \
  f64 = (double *)b08; \
  ret_val = *f64; \
  b08+=8

#define PUT8(ret_val) \
  *b08out++ = ret_val 

#define PUT16(ret_val) \
  b16 = (unsigned short *)b08out; \
  *b16 = ret_val; \
  b08out+=2

#define PUT32(ret_val) \
  b32 = (unsigned int *)b08out; \
  *b32 = ret_val; \
  b08out+=4

#define PUT64(ret_val) \
  b64 = (unsigned long long *)b08out; \
  *b64 = ret_val; \
  b08out+=8

#define PUTF32(ret_val) \
  f32 = (float *)b08out; \
  *f32 = ret_val; \
  b08out+=4

#define PUTF64(ret_val) \
  f64 = (double *)b08out; \
  *f64 = ret_val; \
  b08out+=8

#define PRINT_BUFFER(start_ptr_val, end_ptr_val) \
  b08save = b08; /*save just in case since we'll change it here*/ \
  b08 = (unsigned char *)(start_ptr_val); \
  while(b08<(unsigned char *)(end_ptr_val))	\
  { \
    unsigned int tmp; \
    GET32(tmp); \
    printf("== 0x%08x\n",tmp); \
  } \
  b08 = b08save



/* prototypes */

#ifdef __cplusplus
extern "C" {
#endif
  int evOpenEvent( unsigned int *buf, int eventtag);

  int evOpenFrag( unsigned int *buf, int fragtag, int fragnum);
  int evOpenBank( unsigned int *buf, int fragtag, int fragnum, int banktag, int banknum, int banktype, char *fmt, int *ind_data);
  int evCloseBank( unsigned int *buf, int fragtag, int fragnum, int banktag, int banknum, unsigned char *b08);

  int evLinkFrag( unsigned int *buf, int fragtag, int fragnum);
  int evLinkBank( unsigned int *buf, int fragtag, int fragnum, int banktag, int banknum, int *nbytes, int *ind_data);

  int evCopyFrag( unsigned int *buf, int fragtag, int fragnum, unsigned int *bufout);
  int evCopyBank( unsigned int *buf, int fragtag, int fragnum, int banktag, int banknum, unsigned int *bufout);

  int evDropFrag( unsigned int *buf, int fragtag, int fragnum);
  int evDropBank( unsigned int *buf, int fragtag, int fragnum, int banktag, int banknum);

#ifdef __cplusplus
}
#endif



#endif
