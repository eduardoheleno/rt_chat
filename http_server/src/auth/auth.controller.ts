import { Body, Controller, HttpCode, HttpStatus, Post } from '@nestjs/common';
import { AuthService } from './auth.service';
import { DefaultUserValidation } from 'src/user/validations/default-user-validation.pipe';
import { LoginUserDto } from 'src/user/dto/login-user.dto';

@Controller('auth')
export class AuthController {
  constructor(private authService: AuthService) {}

  @HttpCode(HttpStatus.OK)
  @Post('login')
  async signIn(
    @Body(new DefaultUserValidation()) loginUserDto: LoginUserDto
  ): Promise<{ access_token: string }> {
    const authData = await this.authService.signIn(
      loginUserDto.nickName,
      loginUserDto.password
    );

    return authData;
  }
}
