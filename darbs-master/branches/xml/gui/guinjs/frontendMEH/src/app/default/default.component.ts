import { Component, OnDestroy, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { StateService } from '../services/state.service';
import { AuthService } from '../services/auth.service';

@Component({
  selector: 'app-default',
  templateUrl: './default.component.html',
  styleUrls: ['./default.component.scss']
})
export class DefaultComponent implements OnInit, OnDestroy {
  constructor(private router: Router,
              private state: StateService,
              private auth: AuthService) { }

  ngOnInit() {
      this.auth.isAuth$.next(false);
      this.auth.userId = '';
      this.auth.token = '';
      this.state.part$.next(3);

  }

  onNavigate(endpoint: string) {
    this.router.navigate([endpoint]);
  }

  ngOnDestroy() {
  }
}
