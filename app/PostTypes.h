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

static char* post_type_to_string(PostType type)
{
    switch (type) {
    default:
    // Intentional Fallthrough
    case PostTypeUnknown:
        return "Unknown";
    case PostTypeText:
        return "Text";
    case PostTypePhoto:
        return "Photo";
    case PostTypeQuote:
        return "Quote";
    case PostTypeVideo:
        return "Video";
    case PostTypeChat:
        return "Chat";
    case PostTypeAudio:
        return "Audio";
    }
}

#endif
