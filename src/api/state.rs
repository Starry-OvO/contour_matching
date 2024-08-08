use std::cell::RefCell;
use std::sync::{Arc, Mutex};
use rayon::ThreadPool;
use crate::cffi::FourierEncoder;

thread_local! {
    pub static TLOCAL: RefCell<FourierEncoder> = RefCell::new(FourierEncoder::new());
}

#[derive(Clone)]
pub struct AppState {
    pub pool: Arc<Mutex<ThreadPool>>,
}
