
#ifndef __ZHACKS_H__
#define __ZHACKS_H__

#include <stdio.h>
#include <zlib.h>

//
// Relevant parts of the gzip file header structure (from RFC 1952):
//
//   0   1   2   3   4   5   6   7   8   9
// +---+---+---+---+---+---+---+---+---+---+
// |ID1|ID2|CM |FLG|     MTIME     |XFL|OS | (more-->)
// +---+---+---+---+---+---+---+---+---+---+
//
//   10  11  ...
// +---+---+=================================+
// | XLEN  |...XLEN bytes of "extra field"...| (more-->)
// +---+---+=================================+
//

#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */

//
// This the the internal struct behind zlib's 'gzFile' type, which is
// typedef'd to a void pointer in zlib.h.  We need access to a couple
// of things the library doesn't normally provide.  The alternative 
// was to re-implement all the gz* functions ourselves.
//
typedef struct gz_stream {
    z_stream stream;
    int      z_err;   /* error code for last stream operation */
    int      z_eof;   /* set if end of input file */
    FILE     *file;   /* .gz file */
    Byte     *inbuf;  /* input buffer */
    Byte     *outbuf; /* output buffer */
    uLong    crc;     /* crc32 of uncompressed data */
    char     *msg;    /* error message */
    char     *path;   /* path name for debugging only */
    int      transparent; /* 1 if input file is not a .gz file */
    char     mode;    /* 'w' or 'r' */
    z_off_t  start;   /* start of compressed data in file (header skipped) */
    z_off_t  in;      /* bytes into deflate or inflate */
    z_off_t  out;     /* bytes out of deflate or inflate */
    int      back;    /* one character push-back */
    int      last;    /* true if push-back is last character */
} gz_stream;


//
// Seek directly to a flush point in a gz file.  Also resets
// the decompressor.  No checking is done to make sure there 
// is actually a flush point at the given offset.
//
z_off_t gzseek_raw(gzFile file, z_off_t raw_offset);


#endif    // End #ifdef __ZHACKS_H__
