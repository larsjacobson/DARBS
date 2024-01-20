import { Injectable } from '@angular/core';
import { Subject } from 'rxjs';
import { Agent } from '../models/Agent.model';
import { HttpClient } from '@angular/common/http';

import { BiomesService } from './biomes.service';

@Injectable({
  providedIn: 'root'
})
export class AgentsService {

  constructor(private http: HttpClient) {}

  private agents: Agent[] = [
    {
      _id: '001018201',
      userId: 'ASB112162768',
      locId: '120981371823',
      title: 'Amazing biome',
      description: 'This place was amazing. Trees and clean seaside'},
    {
        _id: '02222201',
        userId: 'ASB1u12162768',
        locId: '1209881371823',
        title: 'Amazing biomes',
        description: 'This place was ok. Trees and clean seaside'},
  ];
  public agents$ = new Subject<Agent[]>();

  getAgents(locId: string) {
    this.http.get('http://localhost:3000/api/agents/').subscribe(
      (agents: Agent[]) => {
        if (agents) {
          this.agents = agents.filter(function( obj ) {
          return obj.locId == locId;
          });
          this.emitAgents();
        }
      },
      (error) => {
        console.log(error);
      }
    );
  }

  emitAgents() {
    this.agents$.next(this.agents);
  }

  getAgentById(id: string) {
    return new Promise((resolve, reject) => {
      this.http.get('http://localhost:3000/api/agents/' + id).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  createNewAgent(agent: Agent) {
    return new Promise((resolve, reject) => {
      this.http.post('http://localhost:3000/api/agents', agent).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  createNewAgentWithFile(agent: Agent, image: File) {
    return new Promise((resolve, reject) => {
      const agentData = new FormData();
      agentData.append('agent', JSON.stringify(agent));
      //agentData.append('image', image, agent.title);
      this.http.post('http://localhost:3000/api/agents', agentData).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  modifyAgent(id: string, agent: Agent) {
    return new Promise((resolve, reject) => {
      this.http.put('http://localhost:3000/api/agents/' + id, agent).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  modifyAgentWithFile(id: string, agent: Agent, image: File | string) {
    return new Promise((resolve, reject) => {
      let agentData: Agent | FormData;
      if (typeof image === 'string') {
        //agent.imageUrl = image;
        agentData = agent;
      } else {
        agentData = new FormData();
        agentData.append('agent', JSON.stringify(agent));
        //agentData.append('image', image, agent.title);
      }
      this.http.put('http://localhost:3000/api/agents/' + id, agentData).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  deleteAgent(id: string) {
    return new Promise((resolve, reject) => {
      this.http.delete('http://localhost:3000/api/agents/' + id).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }
}
