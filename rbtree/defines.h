#ifndef _DEFINES_H_
#define _DEFINES_H_

#if !defined(RELEASE)
#define ASSERT(x)       do { if (!(x))      while (1); } while (0)
#else
#define ASSERT(x)
#endif

#endif