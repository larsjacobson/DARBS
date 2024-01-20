import { Component, OnDestroy, OnInit } from '@angular/core';
import { StateService } from '../../services/state.service';
import { BiomesService } from '../../services/biomes.service';
import { Subscription } from 'rxjs';
import { Biome } from '../../models/Biome.model';
import { Router } from '@angular/router';

@Component({
  selector: 'app-biomes-list',
  templateUrl: './biomes-list.component.html',
  styleUrls: ['./biomes-list.component.scss']
})
export class BiomesListComponent implements OnInit, OnDestroy {

  public biomes: Biome[] = [];
  public part: number;
  public loading: boolean;

  private biomesSub: Subscription;
  private partSub: Subscription;

  constructor(private state: StateService,
              private biomesService: BiomesService,
              private router: Router) { }

  ngOnInit() {
    this.loading = true;
    this.state.mode$.next('list');
    this.biomesSub = this.biomesService.biomes$.subscribe(
      (biomes) => {
        this.biomes = biomes;
        this.loading = false;
      }
    );
    this.partSub = this.state.part$.subscribe(
      (part) => {
        this.part = part;
      }
    );
    this.biomesService.getBiomes();
  }

  onProductClicked(id: string) {
    if (this.part === 1) {
    } else if (this.part === 3) {
      this.router.navigate(['/biome/' + id]);
    }
  }

  ngOnDestroy() {
    this.biomesSub.unsubscribe();
    this.partSub.unsubscribe();
  }

}
