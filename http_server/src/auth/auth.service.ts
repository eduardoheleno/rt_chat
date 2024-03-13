import { BadRequestException, Injectable } from '@nestjs/common';
import { JwtService } from '@nestjs/jwt';
import { UserService } from 'src/user/user.service';

import * as bcrypt from 'bcrypt';

@Injectable()
export class AuthService {
  constructor(
    private userService: UserService,
    private jwtService: JwtService
  ) {}

  async signIn(nickName: string, password: string): Promise<{ access_token: string }> {
    const user = await this.userService.findOneByNickName(nickName);
    if (!user) {
      throw new BadRequestException();
    }

    const isCorrectPassword = bcrypt.compareSync(password, user.password);
    if (!isCorrectPassword) {
      throw new BadRequestException();
    }

    const payload = { sub: user.id, nickName: user.nickName };

    return {
      access_token: await this.jwtService.signAsync(payload)
    };
  }
}
