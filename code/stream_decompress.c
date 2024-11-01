/**
流式解压缩
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define COMPRESS_CHUNK 10  // 输入源文件缓冲区
#define DECOMPRESS_CHUNK 10 // 解压时输出缓冲区大小

void process_data_handler(const unsigned char *data, size_t size)
{
    fwrite(data, 1, size, stdout);
}

void decompress_data(FILE *source)
{
    unsigned char in[COMPRESS_CHUNK];
    unsigned char out[DECOMPRESS_CHUNK];

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK) {
        fprintf(stderr, "inflateInit failed\n");
        return;
    }

    int flush;
    do {
        strm.avail_in = fread(in, 1, COMPRESS_CHUNK, source);
        if (ferror(source)) {
            inflateEnd(&strm);
            fprintf(stderr, "fread failed\n");
            return;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        do {
            strm.avail_out = DECOMPRESS_CHUNK;
            strm.next_out = out;
            inflate(&strm, flush);
            size_t have = DECOMPRESS_CHUNK - strm.avail_out;
            process_data_handler(out, have);
        } while (strm.avail_out == 0);
    } while (flush != Z_FINISH);

    inflateEnd(&strm);
}

int main(int argc, char *argv[])
{
    FILE *source = fopen(argv[1], "rb");
    if (!source) {
        fprintf(stderr, "Failed to open files\n");
        return 1;
    }

    decompress_data(source);

    fclose(source);
    return 0;
}