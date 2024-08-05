use std::ffi::{c_uchar, c_void};

extern "C" {
    fn c_decode_jpeg_from_bytes(p_src: *const c_uchar, len: usize) -> *mut c_void;
    fn c_destruct_image(vp_image: *mut c_void);
}

pub struct Image {
    ptr: *mut c_void,
}

pub fn decode_jpeg_from_bytes(bytes: Vec<u8>) -> Result<Image, ()> {
    let p_src = bytes.as_ptr();
    let len = bytes.len();

    let p_dst;
    unsafe {
        p_dst = c_decode_jpeg_from_bytes(p_src, len);
    }

    if p_dst.is_null() {
        Err(())
    } else {
        Ok(Image { ptr: p_dst })
    }
}

impl Image {
    pub fn new(bytes: Vec<u8>) -> Image { decode_jpeg_from_bytes(bytes).unwrap() }

    pub fn as_ptr(&self) -> *const c_void {
        self.ptr
    }
}

impl Drop for Image {
    fn drop(&mut self) {
        unsafe {
            c_destruct_image(self.ptr);
        }
    }
}

unsafe impl Send for Image {}
