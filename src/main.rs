use std::sync::{Arc, Mutex};

use axum::{Router, routing::post};
use rayon::ThreadPoolBuilder;

use contour_matching::api::{encode, state::AppState};

#[tokio::main(flavor = "current_thread")]
async fn main() {
    let pool = Arc::new(Mutex::new(
        ThreadPoolBuilder::new()
            .num_threads(8)
            .build()
            .unwrap()
    ));
    let state = AppState { pool };

    let routes = Router::new().
        route("/encode", post(encode::encode_hd)).
        with_state(state);

    let listener = tokio::net::TcpListener::bind("0.0.0.0:3737").await.unwrap();
    axum::serve(listener, routes).await.unwrap();
}
