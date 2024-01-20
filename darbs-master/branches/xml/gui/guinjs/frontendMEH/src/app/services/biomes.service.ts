import { Injectable } from '@angular/core';
import { Subject } from 'rxjs';
import { Biome } from '../models/Biome.model';
import { HttpClient } from '@angular/common/http';

@Injectable({
  providedIn: 'root'
})
export class BiomesService {

  locId: string;
  constructor(private http: HttpClient) {}

  private biomes: Biome[] = [
    {
      _id: '324sdfmoih3',
      title: 'My biome',
      description: 'All about my biome',
      imageUrl: 'https://c.pxhere.com/photos/30/d6/photographer_camera_lens_slr_photography_hands-1079029.jpg!d',
      userId: 'will'
    },
    {
      _id: '324sdfmoih4',
      title: 'Another biome',
      description: 'All about my biome',
      imageUrl: 'https://www.publicdomainpictures.net/pictures/10000/velka/1536-1249273362hbHb.jpg',
      userId: 'will'
    },
  ];
  public biomes$ = new Subject<Biome[]>();

  getBiomes() {
    this.http.get('http://localhost:3000/api/biomes').subscribe(
      (biomes: Biome[]) => {
        if (biomes) {
          this.biomes = biomes;
          this.emitBiomes();
        }
      },
      (error) => {
        console.log(error);
      }
    );
  }

  emitBiomes() {
    this.biomes$.next(this.biomes);
  }

  getBiomeById(id: string) {
    return new Promise((resolve, reject) => {
      this.http.get('http://localhost:3000/api/biomes/' + id).subscribe(
        (response) => {
          this.locId=id;
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  createNewBiome(biome: Biome) {
    return new Promise((resolve, reject) => {
      this.http.post('http://localhost:3000/api/biomes', biome).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  createNewBiomeWithFile(biome: Biome, image: File) {
    return new Promise((resolve, reject) => {
      const biomeData = new FormData();
      biomeData.append('biome', JSON.stringify(biome));
      biomeData.append('image', image, biome.title);
      this.http.post('http://localhost:3000/api/biomes', biomeData).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  modifyBiome(id: string, biome: Biome) {
    return new Promise((resolve, reject) => {
      this.http.put('http://localhost:3000/api/biomes/' + id, biome).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  modifyBiomeWithFile(id: string, biome: Biome, image: File | string) {
    return new Promise((resolve, reject) => {
      let biomeData: Biome | FormData;
      if (typeof image === 'string') {
        biome.imageUrl = image;
        biomeData = biome;
      } else {
        biomeData = new FormData();
        biomeData.append('biome', JSON.stringify(biome));
        biomeData.append('image', image, biome.title);
      }
      this.http.put('http://localhost:3000/api/biomes/' + id, biomeData).subscribe(
        (response) => {
          resolve(response);
        },
        (error) => {
          reject(error);
        }
      );
    });
  }

  deleteBiome(id: string) {
    return new Promise((resolve, reject) => {
      this.http.delete('http://localhost:3000/api/biomes/' + id).subscribe(
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
