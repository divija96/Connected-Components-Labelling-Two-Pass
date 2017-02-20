#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include<math.h>

#define THRESHOLD 0

int merge(int [],int, int);
void merger(int [],int, int);
void flatten(int [],int);
omp_lock_t *lock_array;
int check_pixel_values(int a, int b)
{
  if(a==b)
  {
    return 1;
  }
  else
  {
    return 0;
  }
  if(abs(a-b) <= THRESHOLD)
    return 1;
  else
    return 0;
}

int main(int argc, char *argv[])
{
  long long int row=0,chart=0,column=0,p_size,chunk,num_iter,start,size;
  int num,i=0,j=0,count=1;
  int tid,nthreads;
  int image[100][100],label[100][100],p[100000];
  char ch;
  double time_spent,t1,t2;
  struct timespec tp,tp1;

  nthreads = atoi(argv[1]);
  printf("enter number of rows and columns");
  scanf("%lld %lld",&row,&column);
  row = row + 2;
  column = column +2;
  chart = row * column;
  lock_array = (omp_lock_t *)calloc(chart, sizeof(omp_lock_t ));
  for(i=0;i<row;i++)
  {
    for(j=0;j<column;j++)
    {
      if(i==0 || i == row - 1 || j == 0 || j == column - 1)
        image[i][j] = -1;
      else
      {
        scanf("%d",&num);
        image[i][j] = num;
      }
    }
  }
  chart=row*column;
  for (i=0; i<chart; i++)
  omp_init_lock(&(lock_array[i]));

  num_iter = (row-2)/2;
  if((row % 2) == 1)
  num_iter = num_iter + 1;

	clock_gettime(CLOCK_REALTIME,&tp);
  t1 = (((double)tp.tv_sec) * 1000000) + (((double)tp.tv_nsec) / 1000) ;
  omp_set_num_threads(nthreads);
  #pragma omp parallel default(shared) private(count,tid,i,j,start,chunk,size)
  {
    tid = omp_get_thread_num();
    chunk = num_iter/nthreads ;
    size = 2 * chunk;
    start = 1 + tid*(chunk*2);
    #pragma omp for schedule(static,chunk)
    for(i=1; i<(row-1); i=(i+2))
    {
      if((i == start) || ((i-start) == (nthreads*chunk*2)))
        count = i*column;
      for(j=1; j<(column-1); j++)
      {
         if((i == start) || ((i-start) == (nthreads*chunk*2)))
        {
          if(image[i][j] > 0)
          {
            if(check_pixel_values(image[i][j], image[i][j-1]) == 1)
              label[i][j] = label[i][j-1];
            else
            {
              if(check_pixel_values(image[i][j], image[i+1][j-1]) == 1)
                label[i][j] = label[i+1][j-1];
              else
              {
                label[i][j] = count;
                p[count] = count;
                count++;
              }
            }
          }
          if(check_pixel_values(image[i][j], image[i+1][j]) == 1)
            label[i+1][j] = label[i][j];
          else
          {
            if(image[i+1][j] > 0 && image[i+1][i] != 555)
            {
              if(check_pixel_values(image[i+1][j], image[i][j-1]) == 1)
                label[i+1][j] = label[i][j-1];
              else
              {
                if(check_pixel_values(image[i+1][j], image[i+1][j-1]) == 1)
                  label[i+1][j] = label[i+1][j-1];
                else
                {
                  label[i+1][j] = count;
                  p[count] = count;
                  count++;
                }
              }
            }
          }

        }
        else
        {
          if(image[i][j] > 0)
          {
            if(check_pixel_values(image[i][j], image[i][j-1]) == 0)
            {
              if(check_pixel_values(image[i][j], image[i-1][j]) == 1)
              {
                label[i][j] = label[i-1][j];
                if(check_pixel_values(image[i][j], image[i+1][j-1]) == 1)
                  merge(p,label[i][j],label[i+1][j-1]);
              }
              else
              {
                if(check_pixel_values(image[i][j], image[i+1][j-1]) == 1)
                {
                  label[i][j] = label[i+1][j-1];
                  if(check_pixel_values(image[i][j], image[i-1][j-1]) == 1)
                    merge(p,label[i][j],label[i-1][j-1]);
                  if(check_pixel_values(image[i][j], image[i-1][j+1]) == 1)
                    merge(p,label[i][j],label[i-1][j+1]);
                }
                else
                {
                  if(check_pixel_values(image[i][j], image[i-1][j-1]) == 1)
                  {
                    label[i][j] = label[i-1][j-1];
                    if(check_pixel_values(image[i][j], image[i-1][j+1]) == 1)
                      merge(p,label[i][j],label[i-1][j+1]);
                  }
                  else
                  {
                    if(check_pixel_values(image[i][j], image[i-1][j+1]) == 1)
                      label[i][j] = label[i-1][j+1];
                    else
                    {
                      label[i][j] = count;
                      p[count] = count;
                      count++;
                    }
                  }
                }
              }
            }
            else
            {
              label[i][j] = label[i][j-1];
              if(check_pixel_values(image[i][j], image[i-1][j]) == 0)
              {
                if(check_pixel_values(image[i][j], image[i-1][j+1]) == 1)
                  merge(p,label[i][j],label[i-1][j+1]);
              }
            }
          }
          if(check_pixel_values(image[i][j], image[i+1][j]) == 1)
            label[i+1][j] = label[i][j];
          else
          {
            if(image[i+1][j] > 0 && image[i+1][j] != 555)
            {
              if(check_pixel_values(image[i+1][j], image[i][j-1]) == 1)
                label[i+1][j] = label[i][j-1];
              else
              {
                if(check_pixel_values(image[i+1][j], image[i+1][j-1]) == 1)
                  label[i+1][j] = label[i+1][j-1];
                else
                {
                  label[i+1][j] = count;
                  p[count] = count;
                  count++;
                }
              }
            }
          }
        }
        
      }
    }
    #pragma omp for schedule(dynamic)
    for( i= (1+size);i<(row-1);i=(i+size))
    {
      for(j=1;j<(column-1);j++)
      {
        if(label[i][j] != 0)
        {
          if(label[i-1][j] != 0)
          merger(p,label[i][j],label[i-1][j]);
          else
          {
            if(label[i-1][j-1] != 0)
            merger(p,label[i][j],label[i-1][j-1]);
            if(label[i-1][j+1] != 0)
            merger(p,label[i][j],label[i-1][j+1]);
          }
        }
      }
    }
  }
	clock_gettime(CLOCK_REALTIME,&tp1);
  t2 = (((double)tp1.tv_sec) * 1000000) + (((double)tp1.tv_nsec) / 1000) ;
  time_spent = t2 -t1;
  printf("%f\n",time_spent);

  p_size = chart;

  omp_set_num_threads(nthreads);

  flatten(p,p_size);

  omp_set_num_threads(nthreads);

  #pragma omp parallel default(shared) private(tid,i,j)
  {
    #pragma omp for schedule(dynamic)
    for(i=1;i<(row-1);i++)
    {
      for(j=1;j<(column-1);j++)
      {
        label[i][j] = p[label[i][j]];
      }
    }
  }

  for(i=1;i<(row-1);i++)
  {
    for(j=1;j<(column-1);j++)
    {
      printf("%d ", label[i][j]);
    }
    printf("\n");
  }
}

int merge (int p[100000], int x, int y)
{
  int rootx,rooty,z;
  rootx = x;
  rooty = y;
  while( p[rootx] != p[rooty] )
  {
    if ( p[rootx] > p[rooty] )
    {
      if( rootx == p[rootx] )
      {
        p[rootx] = p[rooty];
        return(p[rootx]);
      }
      z= p[rootx];
      p[rootx] = p[rooty];
      rootx = z;
    }
    else
    {
      if( rooty == p[rooty])
      {
        p[rooty] = p[rootx];
        return(p[rootx]);
      }
      z = p[rooty];
      p[rooty] = p[rootx];
      rooty = z;
    }
  }
  return(p[rootx]);
}

void merger (int p[100000], int x, int y)
{
  int rootx,rooty,z,success;
  rootx = x;
  rooty = y;
  while( p[rootx] != p[rooty] )
  {
    if ( p[rootx] > p[rooty] )
    {
      if( rootx == p[rootx] )
      {
        omp_set_lock(&(lock_array[rootx]));
        success = 0;
        if( rootx == p[rootx] )
        {
          p[rootx] = p[rooty];
          success = 1;
        }
        omp_unset_lock(&(lock_array[rootx]));
        if (success == 1)
        break;
      }
      z= p[rootx];
      p[rootx] = p[rooty];
      rootx = z;
    }
    else
    {
      if( rooty == p[rooty] )
      {
        omp_set_lock(&(lock_array[rooty]));
        success = 0;
        if( rooty == p[rooty] )
        {
          p[rooty] = p[rootx];
          success = 1;
        }
        omp_unset_lock(&(lock_array[rooty]));
        if (success == 1)
        break;
      }
      z = p[rooty];
      p[rooty] = p[rootx];
      rooty = z;
    }
  }
}

void flatten(int p[1000000], int size)
{
  int k = 1,i;
  #pragma omp parallel default(shared)
  {
    #pragma omp for schedule(dynamic) private(i)
    for(i=1;i < size; i++)
    {
      if(p[i] < i)
      p[i] = p[p[i]];
      else
      {
        #pragma omp critical
        {
          p[i] = k;
          k++;
        }
      }
    }
  }
  printf("%d\n",k-1);
}
