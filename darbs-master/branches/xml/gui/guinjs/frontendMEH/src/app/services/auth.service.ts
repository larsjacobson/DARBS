import { Injectable } from '@angular/core';
import { Router } from '@angular/router';
import { BehaviorSubject } from 'rxjs';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class AuthService {

  isAuth$ = new BehaviorSubject<boolean>(false);
  token: string;
  userId: string;
  username: string;
  constructor(private router: Router,
              private http: HttpClient) {}

  createNewUser(username: string, email: string, password: string) {
    return new Promise((resolve, reject) => {
      this.http.post(
        'http://localhost:3000/api/auth/signup',
        { username: username, email: email, password: password })
        .subscribe(
          () => {
            this.login(email, password).then(
              () => {
                resolve();
              }
            ).catch(
              (error) => {
                reject(error);
              }
            );
          },
          (error) => {
            reject(error);
          }
        );
    });
  }

  login(email: string, password: string) {
    return new Promise((resolve, reject) => {
        this.http.post(
          'http://localhost:3000/api/auth/login',
          { email: email, password: password})
          .subscribe(
            (authData: { token: string, userId: string, username:string }) => {
              this.token = authData.token;
              this.userId = authData.userId;
              this.username=authData.username;
              this.isAuth$.next(true);
              resolve();
            },
            (error) => {
              reject(error);
            }
        );

        //this.http.

    });
  }

  logout() {
    this.isAuth$.next(false);
    this.userId = null;
    this.token = null;
    this.username = null;
  }
}
