use axum::{http::StatusCode, Json};
use axum::extract::{Multipart, State};
use serde::Serialize;

use crate::cffi;

use super::state::{AppState, TLOCAL};

#[derive(Serialize)]
pub struct EncRes {
    pub code: Vec<f32>,
}

pub async fn encode_hd(State(state): State<AppState>, mut multipart: Multipart) -> (StatusCode, Json<EncRes>) {
    let mut code = { vec![] };
    while let Some(field) = multipart.next_field().await.unwrap() {
        let name = field.name().unwrap().to_string();
        if name != "img" {
            continue;
        }
        let data = field.bytes().await.unwrap();
        code = state.pool.lock().unwrap().install(|| {
            let image = cffi::decode_jpeg_from_bytes(data.into()).unwrap();
            TLOCAL.with_borrow_mut(|encoder| {
                encoder.encode(&image)
            })
        })
    }
    let res = EncRes { code };
    (StatusCode::OK, Json(res))
}
