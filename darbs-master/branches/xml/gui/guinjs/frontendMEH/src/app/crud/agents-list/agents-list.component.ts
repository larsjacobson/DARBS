import { Component, OnDestroy, OnInit } from '@angular/core';
import { StateService } from '../../services/state.service';
import { AgentsService } from '../../services/agents.service';
import { BiomesService } from '../../services/biomes.service';
import { Subscription } from 'rxjs';
import { Agent } from '../../models/Agent.model';
import { Router } from '@angular/router';

@Component({
  selector: 'app-agents-list',
  templateUrl: './agents-list.component.html',
  styleUrls: ['./agents-list.component.scss']
})
export class AgentsListComponent implements OnInit, OnDestroy {

  public agents: Agent[] = [];
  public part: number;
  public loading: boolean;

  private agentsSub: Subscription;
  private partSub: Subscription;

  public locId: string;

  constructor(private state: StateService,
              private agentsService: AgentsService,
              private router: Router,
              private loc: BiomesService) { }

  ngOnInit() {
    this.loading = true;
    this.state.mode$.next('list');
    this.locId=this.loc.locId;
    this.agentsSub = this.agentsService.agents$.subscribe(
      (agents) => {
        this.agents = agents;
        this.loading = false;
      }
    );
    this.partSub = this.state.part$.subscribe(
      (part) => {
        this.part = part;
      }
    );
    this.agentsService.getAgents(this.locId);
  }

  onProductClicked(id: string) {
    if (this.part === 1) {
    } else if (this.part === 3) {
      this.router.navigate(['biome/'+this.locId+'/agent/' + id]);
    }
  }

  ngOnDestroy() {
    this.agentsSub.unsubscribe();
    this.partSub.unsubscribe();
  }

}
