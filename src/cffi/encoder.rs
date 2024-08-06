use std::ffi::{c_float, c_void};
use super::image::Image;

extern "C" {
    fn cm_construct_fourier_encoder() -> *mut c_void;
    fn cm_destruct_fourier_encoder(vp_encoder: *mut c_void);
    fn cm_encode(vp_encoder: *mut c_void, p_src: *const c_void, p_dst: *mut c_float);
}

pub struct FourierEncoder {
    ptr: *mut c_void,
}


impl FourierEncoder {
    pub fn new() -> FourierEncoder {
        unsafe {
            FourierEncoder {
                ptr: cm_construct_fourier_encoder(),
            }
        }
    }

    pub fn as_ptr(&self) -> *const c_void {
        self.ptr
    }

    pub fn encode(&mut self, image: &Image) -> Vec<f32> {
        const CODE_LEN: usize = 16;
        let mut dst: Vec<f32> = Vec::with_capacity(std::mem::size_of::<f32>() * CODE_LEN);
        unsafe {
            cm_encode(self.ptr, image.as_ptr(), dst.as_mut_ptr());
            dst.set_len(CODE_LEN);
        }
        dst
    }
}

impl Drop for FourierEncoder {
    fn drop(&mut self) {
        unsafe {
            cm_destruct_fourier_encoder(self.ptr);
        }
    }
}

unsafe impl Send for FourierEncoder {}
