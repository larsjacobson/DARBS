import { Component, OnInit } from '@angular/core';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { ActivatedRoute, Router } from '@angular/router';
import { StateService } from '../../services/state.service';
import { BiomesService } from '../../services/biomes.service';
import { Biome } from '../../models/Biome.model';
import { Subscription } from 'rxjs';

@Component({
  selector: 'app-modify-biome',
  templateUrl: './modify-biome.component.html',
  styleUrls: ['./modify-biome.component.scss']
})
export class ModifyBiomeComponent implements OnInit {

  biome: Biome;
  biomeForm: FormGroup;
  loading = false;
  errorMessage: string;
  part: number;

  private partSub: Subscription;

  constructor(private formBuilder: FormBuilder,
              private route: ActivatedRoute,
              private router: Router,
              private state: StateService,
              private biomesService: BiomesService) { }

  ngOnInit() {
    this.loading = true;
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
    this.state.mode$.next('form');
    this.route.params.subscribe(
      (params) => {
        this.biomesService.getBiomeById(params.id).then(
          (biome: Biome) => {
            this.biome = biome;
            this.biomeForm.get('title').setValue(this.biome.title);
            this.biomeForm.get('description').setValue(this.biome.description);
            this.biomeForm.get('imageUrl').setValue(this.biome.imageUrl);
            this.loading = false;
          }
        );
      }
    );
  }

  onSubmit() {
    this.loading = true;
    const biome = new Biome();
    biome.title = this.biomeForm.get('title').value;
    biome.description = this.biomeForm.get('description').value;
    biome.imageUrl = this.biomeForm.get('imageUrl').value;
    biome._id = new Date().getTime().toString();
    biome.userId = this.biome.userId;
    this.biomesService.modifyBiome(this.biome._id, biome).then(
      () => {
        this.biomeForm.reset();
        this.loading = false;
        switch (this.part) {
          case 1:
          case 2:
            this.router.navigate(['/crud/all-biomes']);
            break;
          case 3:
            this.router.navigate(['/crud/all-biomes']);
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
