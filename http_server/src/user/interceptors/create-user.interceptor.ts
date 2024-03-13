import { CallHandler, ExecutionContext, Injectable, NestInterceptor } from "@nestjs/common";
import { Observable, map } from "rxjs";

interface Response<User> {
  data: User;
}

@Injectable()
export class CreateUserInterceptor<User> implements NestInterceptor<User, Response<User>> {
  intercept(context: ExecutionContext, next: CallHandler): Observable<Response<User>> {
    return next
      .handle()
      .pipe(
	map(user => {
	  delete user.password;
	  return { ...user };
	})
      );
  }
}
