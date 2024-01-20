import { Component, OnDestroy, OnInit } from '@angular/core';
import { StateService } from '../../services/state.service';
import { ActivatedRoute, Params, Router } from '@angular/router';
import { Agent } from '../../models/Agent.model';
import { AgentsService } from '../../services/agents.service';
import { Subscription } from 'rxjs';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-single-agent',
  templateUrl: './single-agent.component.html',
  styleUrls: ['./single-agent.component.scss']
})
export class SingleAgentComponent implements OnInit, OnDestroy {

  public agent: Agent;
  public loading: boolean;
  public userId: string;
  public part: number;

  private partSub: Subscription;

  constructor(private state: StateService,
              private router: Router,
              private route: ActivatedRoute,
              private agentsService: AgentsService,
              private auth: AuthService) { }

  ngOnInit() {
    this.loading = true;
    this.state.mode$.next('single-agent');
    this.userId = this.auth.userId ? this.auth.userId : 'userID40282382';
    this.route.params.subscribe(
      (params: Params) => {
        this.agentsService.getAgentById(params.id).then(
          (agent: Agent) => {
            this.loading = false;
            this.agent = agent;
          }
        );
      }
    );
    this.partSub = this.state.part$.subscribe(
      (part) => {
        this.part = part;
        if (part >= 3) {
          this.userId = this.auth.userId;
        }
      }
    );
  }

  onGoBack() {
    if (this.part === 1) {
      this.router.navigate(['/crud/all-agents']);
    } else if (this.part === 3) {
      this.router.navigate(['/all-agents']);
    }
  }

  onModify() {
    switch (this.part) {
      case 1:
      case 2:
        this.router.navigate(['/crud/modify-agent/' + this.agent._id]);
        break;
      case 3:
        this.router.navigate(['/modify-agent/' + this.agent._id]);
        break;
    }
  }

  onDelete() {
    this.loading = true;
    this.agentsService.deleteAgent(this.agent._id).then(
      () => {
        this.loading = false;
        switch (this.part) {
          case 1:
          case 2:
            this.router.navigate(['/crud/all-agents']);
            break;
          case 3:
            this.router.navigate(['/all-agents']);
            break;
        }
      }
    );
  }

  ngOnDestroy() {
    this.partSub.unsubscribe();
  }
}
