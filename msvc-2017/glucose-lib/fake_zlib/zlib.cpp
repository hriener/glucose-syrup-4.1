#include "zlib.h"
#include <stdexcept>

int gzread( gzFile file, void* buf, unsigned int len )
{
   return fread( buf, 1, len, file );
}

gzFile gzdopen( int fd, const char *mode )
{
   if ( fd == 0 )
      return stdin;
   throw std::runtime_error( "fake zlib: unexpected param for gzdopen: fd != 0" );
}

gzFile gzopen( const char *path, const char *mode )
{
   return fopen( path, mode );
}

int gzclose( gzFile file )
{
   return fclose( file );
}