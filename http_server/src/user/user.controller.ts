import { Body, Controller, Post } from '@nestjs/common';
import { CreateUserDto } from './create-user.dto';
import { CreateValidation } from './validations/create-validation.pipe';

@Controller('user')
export class UserController {
  @Post()
  create(@Body(new CreateValidation()) createUserDto: CreateUserDto) {
    console.log(createUserDto);
  }
}
