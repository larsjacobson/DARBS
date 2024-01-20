import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { PartOneComponent } from './part-one/part-one.component';
import { DefaultComponent } from './default/default.component';
import { AppRoutingModule } from './app-routing.module';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { HeaderComponent } from './header/header.component';

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

import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { MatProgressSpinnerModule } from '@angular/material';
import { LoginComponent } from './auth/login/login.component';
import { SignupComponent } from './auth/signup/signup.component';
import { HTTP_INTERCEPTORS, HttpClientModule } from '@angular/common/http';
import { AuthInterceptor } from './interceptors/auth-interceptor';
import { SidebarComponent } from './sidebar/sidebar.component';
import { ReportfaultComponent } from './reportfault/reportfault.component';

@NgModule({
  declarations: [
    AppComponent,
    PartOneComponent,
    DefaultComponent,
    HeaderComponent,
    SidebarComponent,

    StuffListComponent,
    NewThingComponent,
    SingleThingComponent,
    ModifyThingComponent,

    BiomesListComponent,
    NewBiomeComponent,
    SingleBiomeComponent,
    ModifyBiomeComponent,

    AgentsListComponent,
    NewAgentComponent,
    SingleAgentComponent,
    //ModifyAgentComponent,

    LoginComponent,
    SignupComponent,
    ReportfaultComponent

  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    ReactiveFormsModule,
    BrowserAnimationsModule,
    MatProgressSpinnerModule,
    HttpClientModule
  ],
  providers: [{provide: HTTP_INTERCEPTORS, useClass: AuthInterceptor, multi: true}],
  bootstrap: [AppComponent]
})
export class AppModule { }
