#include "video.h"
#include "../../lib/vid/fb_info.h"
#include "../../lib/string/string.h"

extern unsigned char rm_video_blob[];
extern unsigned int  rm_video_blob_len;

fb_info_t *fb = (fb_info_t*)FB_INFO_ADDR;

static void video_init(void) {
    memcpy((void*)TRAMPOLINE_ADDR, rm_video_blob, rm_video_blob_len);
    void (*fn)(void) = (void (*)(void))TRAMPOLINE_ADDR;
    fn();
}

int video(int argv, char *argc[]) {
    video_init();
    return 0;
}
