import { Component, OnDestroy, OnInit } from '@angular/core';
import { StateService } from '../../services/state.service';
import { ActivatedRoute, Params, Router } from '@angular/router';
import { Biome } from '../../models/Biome.model';
import { BiomesService } from '../../services/biomes.service';
import { Subscription } from 'rxjs';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-single-biome',
  templateUrl: './single-biome.component.html',
  styleUrls: ['./single-biome.component.scss']
})
export class SingleBiomeComponent implements OnInit, OnDestroy {

  public biome: Biome;
  public loading: boolean;
  public userId: string;
  public part: number;

  private partSub: Subscription;

  constructor(private state: StateService,
              private router: Router,
              private route: ActivatedRoute,
              private biomesService: BiomesService,
              private auth: AuthService) { }

  ngOnInit() {
    this.loading = true;
    this.state.mode$.next('single-biome');
    this.userId = this.auth.userId ? this.auth.userId : 'userID40282382';
    this.route.params.subscribe(
      (params: Params) => {
        this.biomesService.getBiomeById(params.id).then(
          (biome: Biome) => {
            this.loading = false;
            this.biome = biome;
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
      this.router.navigate(['/crud/all-biomes']);
    } else if (this.part === 3) {
      this.router.navigate(['/all-biomes']);
    }
  }

  onModify() {
    switch (this.part) {
      case 1:
      case 2:
        this.router.navigate(['/crud/modify-biome/' + this.biome._id]);
        break;
      case 3:
        this.router.navigate(['/modify-biome/' + this.biome._id]);
        break;
    }
  }

  onDelete() {
    this.loading = true;
    this.biomesService.deleteBiome(this.biome._id).then(
      () => {
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
    );
  }

  ngOnDestroy() {
    this.partSub.unsubscribe();
  }
}
