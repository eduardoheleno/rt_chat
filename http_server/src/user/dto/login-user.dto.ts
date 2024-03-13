import { IsString } from "class-validator";

export class LoginUserDto {
  @IsString()
  nickName: string;

  @IsString()
  password: string;
}
