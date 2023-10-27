#include "tp7.h"

sphere *sph;
pixel *img;

int main() {
//	printf("Hello, World!\n");
	sph = (sphere *)malloc(sizeof(sphere) * NB_SPHERE);
	img = (pixel *)malloc(sizeof(pixel) * DIM);
	generateRandomSpheres(sph, NB_SPHERE, 64);
	generateImg(sph, img);
	printf("P3 %d %d %d\n", Dim, Dim, 255);
	for (int i = 0; i < DIM; i++) {
		showPixel(img[i]);
	}
	free(sph);
	free(img);

	return 0;
}

void showPixel(pixel p) {
	printf("%d %d %d\n", p.r, p.g, p.b);
}

float hit(sphere s, float ox, float oy, float *n) {
	float dx = ox - s.x;
	float dy = oy - s.y;
	if (dx * dx + dy * dy < s.radius * s.radius) {
		float dz = sqrtf(s.radius * s.radius - dx * dx - dy * dy);
		*n = dz / sqrtf(s.radius * s.radius);
//		printf("[%f]\n", dz / sqrtf(s.radius * s.radius));
		return dz + s.z;
	}
	return -INF;
}

float getRandom_in_float(float min, float max) {
	float scale = rand() / (float)RAND_MAX;
	// RAND_MAX
	return min + scale * (max - min);
}

float carre(float x) {
	return x * x;
}

bool is_intersect(sphere s1, sphere s2) {
	float d; // 两个球体质心的举例
	d = sqrtf(carre(s1.x - s2.x) + carre(s1.y - s2.y) + carre(s1.z - s2.z));
	return d < s1.radius + s2.radius;
}

void generateRandomSpheres(sphere *sphere, unsigned nbSphere, unsigned dim) {
	bool insec = true; // 是否相交
	for (int n = 0; n < nbSphere; n++) {
		insec = true;
		while (insec) {
			sphere[n].x = getRandom_in_float(0, Dim);
			sphere[n].y = getRandom_in_float(0, Dim);
			sphere[n].z = getRandom_in_float(0, Dim);
			sphere[n].r = getRandom_in_float(0.0f, 1.0f);
			sphere[n].g = getRandom_in_float(0.0f, 1.0f);
			sphere[n].b = getRandom_in_float(0.0f, 1.0f);
			sphere[n].radius = getRandom_in_float(30.0f, 150.0f);
//			sphere[n].radius = rand() % 100;

			insec = false;
			for (int j = 0; j < n && !insec; j++) {
				insec = is_intersect(sphere[n], sphere[j]);
			}
		}
//		showInfoSphere(sphere[n]);
	}
}

void showInfoSphere(sphere s) {
	printf("[%f,%f,%f]||[%f,%f,%f]||%f\n", s.r, s.g, s.b, s.x, s.y, s.z, s.radius);
}

void generateImg(sphere *sphere, pixel *pixel) {

	void **res = (void **)malloc(sizeof(void *) * NBTH);
	int s[8];
	pthread_t *myth = (pthread_t *)malloc(sizeof(pthread_t) * NBTH);
	for (int i = 0; i < NBTH; ++i) {
		pthread_t temp;
		myth[i] = temp;
	}
//	int *ll = (int *)malloc(sizeof(int) * 2);
	int **ll = (int**)malloc(sizeof (int*)*8);
//	int **ll;
	for (int i = 0; i < 8; ++i) {
		ll[i] = (int *)malloc(sizeof (int)*2);
	}
	for (int i = 0; i < 8; i++) {
		ll[i][0] = i * 128;
		ll[i][1] = (i + 1) * 128 ;
//		printf("[%d,%d]\n", ll[0], ll[1]);
		if (pthread_create(&(myth[i]), NULL, thread_func, ll[i])) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
//		sleep(1);
	}
	for (int i = 0; i < 8; ++i) {
		if((s[i] = pthread_join(myth[i], &(res[i])))){
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}
	free(res);
	free(myth);
	free(ll);
}

void *thread_func(int *ll) {
	int i,j;
	unsigned offset;
	for (i = ll[0]; i < ll[1]; ++i) {
		for (j = 0; j < Dim; ++j) {
			offset = i * Dim + j;
			float ox = i;
			float oy = j;

			float r = 0.0f,g = 0.0f,b = 0.0f;
			float maxz = -INF;
			for (unsigned idSphere = 0;idSphere < NB_SPHERE;idSphere++){
				float n,t = hit(sph[idSphere],ox,oy,&n);
				if (t > maxz){
					float fscale = n;
					r = sph[idSphere].r * fscale;
					g = sph[idSphere].g * fscale;
					b = sph[idSphere].b * fscale;
					maxz = t;
				}
			}

			img[offset].r = (int)(r*255);
			img[offset].g = (int)(g*255);
			img[offset].b = (int)(b*255);

		}
	}
//	printf("end of %d\n",ll[0]);
	return 0;
}
