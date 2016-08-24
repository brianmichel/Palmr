#ifndef _TEXTPOST_H_
#define _TEXTPOST_H_

#include "PostTypes.h"
#include <System/SystemPublic.h>

static const int TextPostAuthorMax = 80;
static const int TextPostBodyMax = 100;

typedef struct TextPost {
    PostType type;
    Char* author;
    Char* body;
} TextPost;

Boolean initialize_text_post(TextPost* textPost);
Boolean destroy_text_post(TextPost* textPost);

#endif
