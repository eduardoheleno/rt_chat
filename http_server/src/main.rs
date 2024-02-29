mod controllers;
mod models;
mod database;

use axum::{
    routing::{get, post},
    http::StatusCode,
    Json,
    Router
};

use controllers::user_controller;

async fn root() -> &'static str {
    "Hello, World!"
}

#[tokio::main]
async fn main() -> Result<(), std::io::Error> {
    let app = Router::new()
        .route("/", get(root));

    let listener = tokio::net::TcpListener::bind("0.0.0.0:3001").await?;
    axum::serve(listener, app).await?;

    Ok(())
}
