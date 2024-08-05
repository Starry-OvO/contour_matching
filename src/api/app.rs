use std::cell::RefCell;
use std::sync::{Arc, Mutex};
use rayon::ThreadPool;
use thread_local::ThreadLocal;
use crate::cffi::FourierEncoder;

#[derive(Clone)]
pub struct AppState {
    pub pool: Arc<Mutex<ThreadPool>>,
    pub tlocal: Arc<ThreadLocal<RefCell<FourierEncoder>>>,
}
