import { Component, OnDestroy, OnInit } from '@angular/core';
import { StateService } from '../../services/state.service';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { Agent } from '../../models/Agent.model';

import { BiomesService } from '../../services/biomes.service';
import { AgentsService } from '../../services/agents.service';
import { Router } from '@angular/router';
import { Subscription } from 'rxjs';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-new-agent',
  templateUrl: './new-agent.component.html',
  styleUrls: ['./new-agent.component.scss']
})
export class NewAgentComponent implements OnInit, OnDestroy {

  public agentForm: FormGroup;
  public loading = false;
  public part: number;
  public userId: string;
  public locId: string;
  public errorMessage: string;

  private partSub: Subscription;

  constructor(private state: StateService,
              private formBuilder: FormBuilder,
              private agentsService: AgentsService,
              private router: Router,
              private auth: AuthService,
              private loc: BiomesService) { }

  ngOnInit() {
    this.state.mode$.next('form');
    this.agentForm = this.formBuilder.group({
      title: [null, Validators.required],
      description: [null, Validators.required],
    });
    this.partSub = this.state.part$.subscribe(
      (part) => {
        this.part = part;
      }
    );
    this.userId = this.part >= 3 ? this.auth.userId : 'userID40282382';
    this.locId = this.part >= 3 ? this.loc.locId : 'locID40282382';
  }

  onSubmit() {
    this.loading = true;
    const agent = new Agent();
    agent.title = this.agentForm.get('title').value;
    agent.description = this.agentForm.get('description').value;
    agent._id = new Date().getTime().toString();
    agent.userId = this.userId;
    agent.locId=this.locId;
    this.agentsService.createNewAgent(agent).then(
      () => {
        this.agentForm.reset();
        this.loading = false;
        switch (this.part) {
          case 1:
          case 2:
            this.router.navigate(['/crud/all-agents']);
            break;
          case 3:
            this.router.navigate(['/biome/'+this.locId]);
            break;
        }
      }
    ).catch(
      (error) => {
        this.loading = false;
        this.errorMessage = error.message;
      }
    );
  }

  ngOnDestroy() {
    this.partSub.unsubscribe();
  }

}
