#include "TextPost.h"
#include "PalmrDB.h"
#include <PalmOS.h>
#include <stdio.h>

static UInt16 text_post_packed_size(TextPost* post);
void pack_text_post(TextPost* post, MemPtr recordP);

Boolean initialize_text_post(TextPost* textPost)
{
    textPost->type = PostTypeText;
    textPost->author = "Brian Michel & Ken";
    textPost->body = "Just a bunch of text to go here whatever";

    return true;
}

Boolean destroy_text_post(TextPost* textPost)
{
    //TODO dynamically deallocate strings
    return true;
}

void create_test_records(DmOpenRef* database)
{
    int i = 0;
    for (i = 0; i < 10; i++) {
        Char* author = "";
        TextPost post;
        UInt16 index;

        sprintf(author, "Author %i", i);
        initialize_text_post(&post);
        post.author = author;

        write_text_post_record(database, &post, &index);
    }
}

void write_text_post_record(DmOpenRef* database, TextPost* post, UInt16* index)
{
    MemHandle recordH;
    MemPtr recordP;
    Err error;

    recordH = DmNewHandle(*database, (UInt32)text_post_packed_size(post));
    recordP = MemHandleLock(recordH);

    pack_text_post(post, recordP);

    MemPtrUnlock(recordP);

    error = DmAttachRecord(*database, index, recordH, 0);
    if (error) {
        MemHandleFree(recordH);
    }
}

void pack_text_post(TextPost* post, MemPtr recordP)
{
    UInt32 offset = 0;
    UInt32 authorLength = StrLen(post->author);
    UInt32 bodyLength = StrLen(post->body);
    Char* postCode = post_type_to_code(post->type);

    // Write Type To Record
    DmWrite(recordP, offset, postCode, StrLen(postCode));
    offset += sizeof(PostType);

    // Write Author To Record
    DmWrite(recordP, offset, post->author, authorLength);
    offset += authorLength;

    DmWrite(recordP, offset, post->body, bodyLength);
}

static UInt16 text_post_packed_size(TextPost* post)
{
    UInt16 size = 0;

    size += StrLen(post->author);
    size += StrLen(post->body);
    size += sizeof(PostType);

    return size;
}
