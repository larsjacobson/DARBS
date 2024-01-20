import { Component, OnInit } from '@angular/core';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { ActivatedRoute, Router } from '@angular/router';
import { StateService } from '../../services/state.service';
import { AgentsService } from '../../services/agents.service';
import { Agent } from '../../models/Agent.model';
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-modify-agent',
  templateUrl: './modify-agent.component.html',
  styleUrls: ['./modify-agent.component.scss']
})
export class ModifyAgentComponent implements OnInit {

  agent: Agent;
  agentForm: FormGroup;
  loading = false;
  errorMessage: string;
  part: number;

  private partSub: Subscription;

  constructor(private formBuilder: FormBuilder,
              private route: ActivatedRoute,
              private router: Router,
              private state: StateService,
              private agentsService: AgentsService) { }

  ngOnInit() {
    this.loading = true;
    this.agentForm = this.formBuilder.group({
      title: [null, Validators.required],
      description: [null, Validators.required],
      price: [0, Validators.required],
      imageUrl: [null, Validators.required]
    });
    this.partSub = this.state.part$.subscribe(
      (part) => {
        this.part = part;
      }
    );
    this.state.mode$.next('form');
    this.route.params.subscribe(
      (params) => {
        this.agentsService.getAgentById(params.id).then(
          (agent: Agent) => {
            /*this.agent = agent;
            this.agentForm.get('title').setValue(this.agent.title);
            this.agentForm.get('description').setValue(this.agent.description);
            this.agentForm.get('price').setValue(this.agent.price / 100);
            this.agentForm.get('imageUrl').setValue(this.agent.imageUrl);
            this.loading = false;*/
          }
        );
      }
    );
  }

  onSubmit() {
    this.loading = true;
    const agent = new Agent();
    /*agent.title = this.agentForm.get('title').value;
    agent.description = this.agentForm.get('description').value;
    agent.price = this.agentForm.get('price').value * 100;
    agent.imageUrl = this.agentForm.get('imageUrl').value;
    agent._id = new Date().getTime().toString();
    agent.userId = this.agent.userId;*/
    this.agentsService.modifyAgent(this.agent._id, agent).then(
      () => {
        this.agentForm.reset();
        this.loading = false;
        switch (this.part) {
          case 1:
          case 2:
            this.router.navigate(['/crud/all-agents']);
            break;
          case 3:
            this.router.navigate(['/crud/all-agents']);
            break;
        }
      },
      (error) => {
        this.loading = false;
        this.errorMessage = error.message;
      }
    );
  }

}
