#pragma once

#include<stdio.h>
#include<string.h>
#include"parser.h"
#define ERROR 0
#define OK 1
typedef int status;
status PrintFile(FILE* fp);