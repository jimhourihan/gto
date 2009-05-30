
#include "zhacks.h"

z_off_t gzseek_raw(gzFile file, z_off_t raw_offset)
{
    gz_stream *s = (gz_stream*)file;

    if (s == NULL || s->mode != 'r') return -1;

    s->start = raw_offset;

    s->z_err = Z_OK;
    s->z_eof = 0;
    s->back = EOF;
    s->stream.avail_in = 0;
    s->stream.next_in = s->inbuf;
    s->crc = crc32(0L, Z_NULL, 0);
    inflateReset(&s->stream);
    s->in = 0;
    s->out = 0;
    return fseek(s->file, s->start, SEEK_SET);
}
