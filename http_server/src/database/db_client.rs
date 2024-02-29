use mysql_async::{
    Pool,
    Error,
    Conn
};

pub struct DatabaseClient {
    pool: Pool,
    conn: Conn
}

impl DatabaseClient {
    pub async fn new(database_url: &str) -> Result<DatabaseClient, Error> {
        let pool = Pool::new(database_url);
        let conn = pool.get_conn().await?;

        Ok(
            DatabaseClient { pool, conn }
        )
    }
}

// TODO: find a way to implement async drop
// impl AsyncDrop for DatabaseClient {

// }
