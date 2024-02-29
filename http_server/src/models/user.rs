pub struct User {
    user_id: Option<u32>,
    user_name: String,
    user_password: String
}

impl User {
    pub fn new(user_id: Option<u32>, user_name: String, user_password: String) -> User {
        User { user_id, user_name, user_password }
    }
}
