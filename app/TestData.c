#include "TestData.h"

TestData initialize_test_data()
{
    TestData data;
    TextPost post1;
    TextPost post2;
    TextPost post3;

    initialize_text_post_full(&post1, "brianmichel", "This really sucks, please end me.");
    initialize_text_post_full(&post2, "screegs", "Hope everyone really enjoyed hackday!");
    initialize_text_post_full(&post3, "brianmichel", "FRANK OCEAN DROPPED A NEW ALBUM!");

    data.count = 3;
    data.post1 = post1;
    data.post2 = post2;
    data.post3 = post3;

    return data;
}
