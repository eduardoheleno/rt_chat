import { Injectable } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { User } from './user.entity';
import { Repository } from 'typeorm';
import { CreateUserDto } from './dto/create-user.dto';
import * as bcrypt from 'bcrypt';

@Injectable()
export class UserService {
  constructor(
    @InjectRepository(User)
    private userRepository: Repository<User>
  ) {}

  readonly SALT_ROUNDS = 10;

  async insert(createUserDto: CreateUserDto): Promise<User> {
    createUserDto.password = await bcrypt.hash(
      createUserDto.password,
      this.SALT_ROUNDS
    );

    return this.userRepository.save(createUserDto);
  }

  findOneByNickName(nickName: string): Promise<User | undefined> {
    return this.userRepository.createQueryBuilder('user')
      .where('user.nickName = :nickName', { nickName })
      .addSelect('user.password')
      .getOne();
  }
}
