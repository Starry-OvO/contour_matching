use contour_matching::api::{app::AppState, encode};
use axum::{routing::post, Router};
use std::sync::{Arc, Mutex};
use rayon::ThreadPoolBuilder;
use thread_local::ThreadLocal;

#[tokio::main(flavor = "current_thread")]
async fn main() {
    let pool = Arc::new(Mutex::new(ThreadPoolBuilder::new().num_threads(8).build().unwrap()));
    let tlocal = Arc::new(ThreadLocal::new());
    let state = AppState { pool, tlocal };

    let routes = Router::new().route("/encode", post(encode::encode_hd)).with_state(state);

    let listener = tokio::net::TcpListener::bind("0.0.0.0:3737").await.unwrap();
    axum::serve(listener, routes).await.unwrap();
}
