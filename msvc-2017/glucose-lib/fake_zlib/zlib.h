#pragma once

#include <stdio.h>

typedef FILE* gzFile;

int gzread( gzFile file, void* buf, unsigned int len );
gzFile gzdopen( int fd, const char *mode );
gzFile gzopen( const char *path, const char *mode );
int gzclose( gzFile file );