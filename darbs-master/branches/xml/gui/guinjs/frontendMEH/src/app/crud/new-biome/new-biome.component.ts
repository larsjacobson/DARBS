import { Component, OnDestroy, OnInit } from '@angular/core';
import { StateService } from '../../services/state.service';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { Biome } from '../../models/Biome.model';
import { BiomesService } from '../../services/biomes.service';
import { Router } from '@angular/router';
import { Subscription } from 'rxjs';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-new-biome',
  templateUrl: './new-biome.component.html',
  styleUrls: ['./new-biome.component.scss']
})
export class NewBiomeComponent implements OnInit, OnDestroy {

  public biomeForm: FormGroup;
  public loading = false;
  public part: number;
  public userId: string;
  public errorMessage: string;

  private partSub: Subscription;

  constructor(private state: StateService,
              private formBuilder: FormBuilder,
              private biomesService: BiomesService,
              private router: Router,
              private auth: AuthService) { }

  ngOnInit() {
    this.state.mode$.next('form');
    this.biomeForm = this.formBuilder.group({
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
    this.userId = this.part >= 3 ? this.auth.userId : 'userID40282382';
  }

  onSubmit() {
    this.loading = true;
    const biome = new Biome();
    biome.title = this.biomeForm.get('title').value;
    biome.description = this.biomeForm.get('description').value;
    biome.imageUrl = this.biomeForm.get('imageUrl').value;
    biome._id = new Date().getTime().toString();
    biome.userId = this.userId;
    this.biomesService.createNewBiome(biome).then(
      () => {
        this.biomeForm.reset();
        this.loading = false;
        switch (this.part) {
          case 1:
          case 2:
            this.router.navigate(['/crud/all-biomes']);
            break;
          case 3:
            this.router.navigate(['/all-biomes']);
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
