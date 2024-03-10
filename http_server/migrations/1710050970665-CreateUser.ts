import { MigrationInterface, QueryRunner } from "typeorm";

export class CreateUser1710050970665 implements MigrationInterface {

  public async up(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.query(
      `CREATE TABLE user (
	id int NOT NULL AUTO_INCREMENT,
	firstName varchar(255) COLLATE utf8mb4_general_ci NOT NULL,
	lastName varchar(255) COLLATE utf8mb4_general_ci NOT NULL,
	isActive tinyint NOT NULL DEFAULT '1',
	PRIMARY KEY (id)
      )`
    );
  }

  public async down(queryRunner: QueryRunner): Promise<void> {
    await queryRunner.query(
      `DROP TABLE user`
    )
  }
}
