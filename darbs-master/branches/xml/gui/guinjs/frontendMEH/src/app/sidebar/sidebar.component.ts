import { Component, OnInit } from '@angular/core';

import { AuthService } from '../services/auth.service';
import { Router } from '@angular/router';
@Component({
  selector: 'app-sidebar',
  templateUrl: './sidebar.component.html',
  styleUrls: ['./sidebar.component.scss']
})
export class SidebarComponent implements OnInit {

  public username: string;
  constructor(//private state: StateService,
              private auth: AuthService,
              private router: Router) { }

  ngOnInit() {
    this.username=this.auth.username;
    console.log(this.username);
  }
  onLogout() {
    this.auth.logout();
    this.router.navigate(['auth/login']);
  }
}
