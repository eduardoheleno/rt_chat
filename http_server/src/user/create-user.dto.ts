import { IsString } from "class-validator";

export class CreateUserDto {
  @IsString()
  nickName: string;

  @IsString()
  password: string;
}
