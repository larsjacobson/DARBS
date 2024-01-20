import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { PartOneComponent } from './part-one/part-one.component';
import { DefaultComponent } from './default/default.component';
import { ReportfaultComponent } from './reportfault/reportfault.component';

import { StuffListComponent } from './part-one/stuff-list/stuff-list.component';
import { NewThingComponent } from './part-one/new-thing/new-thing.component';
import { SingleThingComponent } from './part-one/single-thing/single-thing.component';
import { ModifyThingComponent } from './part-one/modify-thing/modify-thing.component';

import { BiomesListComponent } from './crud/biomes-list/biomes-list.component';
import { NewBiomeComponent } from './crud/new-biome/new-biome.component';
import { SingleBiomeComponent } from './crud/single-biome/single-biome.component';
import { ModifyBiomeComponent } from './crud/modify-biome/modify-biome.component';

import { AgentsListComponent } from './crud/agents-list/agents-list.component';
import { NewAgentComponent } from './crud/new-agent/new-agent.component';
import { SingleAgentComponent } from './crud/single-agent/single-agent.component';
//import { ModifyAgentComponent } from './crud/modify-agent/modify-agent.component';

import { LoginComponent } from './auth/login/login.component';
import { SignupComponent } from './auth/signup/signup.component';
import { AuthGuard } from './services/auth-guard.service';

const routes: Routes = [
  /*
  { path: '', component: DefaultComponent,
    children: [
      { path: 'new-thing', component: NewThingComponent, canActivate: [AuthGuard] },
      { path: 'all-stuff', component: StuffListComponent, canActivate: [AuthGuard] },
      { path: 'thing/:id', component: SingleThingComponent, canActivate: [AuthGuard] },
      { path: 'modify-thing/:id', component: ModifyThingComponent, canActivate: [AuthGuard] },
      { path: 'auth/login', component: LoginComponent },
      { path: 'auth/signup', component: SignupComponent },
      { path: '', pathMatch: 'full', redirectTo: 'auth/login' },
      { path: '**', redirectTo: 'all-stuff' }
    ]
  },
  */
  { path: '', component: DefaultComponent,
    children: [
      { path: 'new-biome', component: NewBiomeComponent, canActivate: [AuthGuard] },
      { path: 'report', component: ReportfaultComponent, canActivate: [AuthGuard] },
      { path: 'all-biomes', component: BiomesListComponent, canActivate: [AuthGuard] },
      { path: 'biome/:id', component: SingleBiomeComponent, canActivate: [AuthGuard] },
      { path: 'biome/:id/new-agent', component: NewAgentComponent, canActivate: [AuthGuard] },
      { path: 'biome/:locId/agent/:id', component: SingleAgentComponent, canActivate: [AuthGuard] },
      { path: 'modify-biome/:id', component: ModifyBiomeComponent, canActivate: [AuthGuard] },
      { path: 'auth/login', component: LoginComponent },
      { path: 'auth/signup', component: SignupComponent },
      { path: '', pathMatch: 'full', redirectTo: 'auth/login' },
      { path: '**', redirectTo: 'all-biomes' }
    ]
  },
  { path: 'part-one', component: PartOneComponent,
    children: [
      { path: 'new-thing', component: NewThingComponent },
      { path: 'all-stuff', component: StuffListComponent },
      { path: 'thing/:id', component: SingleThingComponent },
      { path: 'modify-thing/:id', component: ModifyThingComponent },
      { path: '', pathMatch: 'full', redirectTo: 'all-stuff' },
      { path: '**', redirectTo: 'all-stuff' }
    ]
  }
];

@NgModule({
  imports: [
    RouterModule.forRoot(routes)
  ],
  exports: [
    RouterModule
  ],
  providers: [
    AuthGuard
  ]
})
export class AppRoutingModule {}
