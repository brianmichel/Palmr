#ifndef _POSTTYPES_H_
#define _POSTTYPES_H_

typedef enum PostType {
    PostTypeUnknown,
    PostTypeText,
    PostTypePhoto,
    PostTypeQuote,
    PostTypeVideo,
    PostTypeChat,
    PostTypeAudio
} PostType;

static char* post_type_to_code(PostType type)
{
    switch (type) {
    default:
    // Intentional Fallthrough
    case PostTypeUnknown:
        return "Un";
    case PostTypeText:
        return "Te";
    case PostTypePhoto:
        return "Ph";
    case PostTypeQuote:
        return "Qu";
    case PostTypeVideo:
        return "Vi";
    case PostTypeChat:
        return "Ch";
    case PostTypeAudio:
        return "Au";
    }
}

#endif
