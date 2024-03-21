import { Body, Controller, Post, UseInterceptors } from '@nestjs/common';
import { CreateUserDto } from './dto/create-user.dto';
import { UserService } from './user.service';
import { User } from './user.entity';
import { DefaultUserValidation } from './validations/default-user-validation.pipe';
import { CreateUserInterceptor } from './interceptors/create-user.interceptor';

@Controller('user')
export class UserController {
  constructor(private userService: UserService) {}

  @UseInterceptors(CreateUserInterceptor)
  @Post()
  async create(@Body(new DefaultUserValidation()) createUserDto: CreateUserDto): Promise<User> {
    return await this.userService.insert(createUserDto);
  }

  @Post('test')
  salve(): string {
    return 'test';
  }

  // private delay(ms: number): Promise<any> {
  //   return new Promise(resolve => setTimeout(resolve, ms));
  // }
}
