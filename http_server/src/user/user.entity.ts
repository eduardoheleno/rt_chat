import { Column, CreateDateColumn, Entity, PrimaryGeneratedColumn } from "typeorm";

@Entity()
export class User {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ unique: true })
  nickName: string;

  @Column({ select: false })
  password: string;

  @CreateDateColumn()
  createdAt: Date;
}
