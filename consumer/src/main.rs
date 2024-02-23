use std::thread::sleep;
use std::time::Duration;

use lapin::{
    Connection,
    ConnectionProperties, options::{QueueDeclareOptions, BasicConsumeOptions, BasicAckOptions}, types::FieldTable, message::DeliveryResult
};

#[tokio::main]
async fn main() {
    sleep(Duration::from_secs(5));

    let uri = "amqp://admin:admin@rabbitmq";
    let options = ConnectionProperties::default()
        .with_executor(tokio_executor_trait::Tokio::current())
        .with_reactor(tokio_reactor_trait::Tokio);

    let connection = Connection::connect(uri, options).await.unwrap();
    let channel = connection.create_channel().await.unwrap();

    let _queue = channel
        .queue_declare(
            "hello",
            QueueDeclareOptions::default(),
            FieldTable::default()
        )
        .await
        .unwrap();

    let consumer = channel
        .basic_consume(
            "hello",
            "tag_foo",
            BasicConsumeOptions::default(),
            FieldTable::default()
        )
        .await
        .unwrap();

    consumer.set_delegate(move |delivery: DeliveryResult| async move {
        let delivery = match delivery {
            Ok(Some(delivery)) => delivery,
            Ok(None) => return,
            Err(error) => {
                println!("error {}", error);
                return;
            }
        };

        let message = std::str::from_utf8(&delivery.data).unwrap();
        println!("{message}");

        delivery
            .ack(BasicAckOptions::default())
            .await
            .unwrap();
    });

    std::future::pending::<()>().await;
}
