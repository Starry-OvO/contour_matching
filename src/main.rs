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

    let app = Router::new().route("/encode", post(encode::encode_hd)).with_state(state);

    // run it with hyper on localhost:3000
    let listener = tokio::net::TcpListener::bind("0.0.0.0:3000").await.unwrap();
    axum::serve(listener, app).await.unwrap();
}
