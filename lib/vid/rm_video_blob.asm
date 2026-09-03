BITS 32
global rm_video_blob
global rm_video_blob_len

section .rodata
rm_video_blob:
    incbin "build/rm_video.bin"
rm_video_blob_end:
rm_video_blob_len:
    dd rm_video_blob_end - rm_video_blob
