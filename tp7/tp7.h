//
// Created by XuZY on 2023/10/26.
//

#ifndef TP7__TP7_H_
#define TP7__TP7_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h> // 引入 bool
#include <unistd.h>


#define DIM (1048576) // 64 * 64 ;
#define Dim 1024

#define NB_SPHERE (100)
#define INF 1000000
#define NBTH 8 // number of thread ;


typedef struct {
	float x, y, z;
	float r, g, b;
	float radius;
} sphere;

typedef struct {
	u_int8_t r,g,b;
}pixel;

void showPixel(pixel p);

float hit(sphere s,float ox,float oy,float *n);

void generateRandomSpheres(sphere *sphere,unsigned  nbSphere,unsigned dim);
// list de sphere ;nombre de sphere; dimension de image

void generateImg(sphere *sphere,pixel *pixel);

float getRandom_in_float(float min,float max);

float carre(float x);

bool is_intersect(sphere s1,sphere s2);

void showInfoSphere(sphere s);
// 显示球体的数值

void *thread_func(int* ll);
// 计算的球,hit 的返回值,llf 的值


#endif //TP7__TP7_H_
