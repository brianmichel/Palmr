#ifndef _TESTDATA_H_
#define _TESTDATA_H_

#include "TextPost.h"

typedef struct TestData {
    int count;
    TextPost post1;
    TextPost post2;
    TextPost post3;
} TestData;

TestData initialize_test_data();
#endif
