/*
 * Copyright 2011 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

void setup() {
  int rtn; 

  FILE* f = fopen("towrite", "w");
  assert(f);
  rtn = fwrite("abcdef", 6, 1, f);
  assert(rtn = 6);
  fclose(f);

  f = fopen("toread", "w");
  assert(f);
  rtn = fwrite("abcdef", 6, 1, f);
  assert(rtn = 6);
  fclose(f);

  assert(chmod("toread", 0444) == 0);
}

int main() {
  setup();

  struct stat s;
  int f = open("towrite", O_WRONLY);
  assert(f);
  int f2 = open("toread", O_RDONLY);
  assert(f2);

  fstat(f, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 6);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("ftruncate(10): %d\n", ftruncate(f, 10));
  printf("errno: %s\n", strerror(errno));
  fstat(f, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 10);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("ftruncate(4): %d\n", ftruncate(f, 4));
  printf("errno: %s\n", strerror(errno));
  fstat(f, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 4);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("ftruncate(-1): %d\n", ftruncate(f, -1));
  printf("errno: %s\n", strerror(errno));
  fstat(f, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 4);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("truncate(2): %d\n", truncate("towrite", 2));
  printf("errno: %s\n", strerror(errno));
  fstat(f, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 2);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("open(O_TRUNC)\n");
  open("towrite", O_WRONLY | O_TRUNC);
  printf("errno: %s\n", strerror(errno));
  fstat(f, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 0);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("truncate(readonly, 2): %d\n", truncate("toread", 2));
  printf("errno: %s\n", strerror(errno));
  fstat(f2, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 6);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("truncate(empty_path, 2): %d\n", truncate("", 2));
  printf("errno: %s\n", strerror(errno));
  printf("\n");

  printf("ftruncate(readonly, 4): %d\n", ftruncate(f2, 4));
  printf("errno: %s\n", strerror(errno));
  fstat(f2, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 6);
  memset(&s, 0, sizeof s);
  errno = 0;
  printf("\n");

  printf("open(readonly, O_TRUNC)\n");
  open("toread", O_RDONLY | O_TRUNC);
  printf("errno: %s\n", strerror(errno));
  fstat(f2, &s);
  printf("st_size: %lld\n", s.st_size);
  assert(s.st_size == 6);
  memset(&s, 0, sizeof s);
  errno = 0;

  return 0;
}
